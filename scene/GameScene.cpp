#include "GameScene.h"
#include "AxisIndicator.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "ImGuiManager.h"
#include "MatrixTrans.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "TextureManager.h"
#include <cassert>
#include "skydome.h"
#include "RailCamera.h"
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
	delete skydomeModel_;
	for (EnemyBullet* enemyBullet : enemyBullets_)
	{
		delete enemyBullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("Player.png");
	// textureHandle_ = TextureManager::Load("debugfont.png");

	EnemytextureHandle_ = TextureManager::Load("Enemy.png");

	skydomeModel_ = Model::CreateFromOBJ("skydome", true);


	// ビュープロジェクションの初期化
	viewProjection_.farZ = 1000.0f;

	model_ = Model::Create();

	worldTransform_.Initialize();

	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();

	// 自キャラの初期化
	Vector3 playerPosition(0, -3, -40.0f);
	player_->Initialize(model_,textureHandle_,playerPosition);

	// 敵キャラの初期化
	enemy_ = new Enemy();
	Vector3 enemyPosition(0, 0, 0);
	enemy_->Initialize(model_, EnemytextureHandle_,enemyPosition);

	//敵キャラにゲームシーンを渡す
	enemy_->SetGameScene(this);

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//背景
	skydome_ = new skydome();
	skydome_->Initialize(skydomeModel_);


	//レールカメラ
	railCamera_ = new RailCamera();
	Vector3 radian = {0.0f, 0.0f, 0.0f};
	railCamera_->Initialize(player_->GetWorldPosition(), radian);


	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetPrent(&railCamera_->GetWorldTransform());
	LoadEnemyPopData();

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}
void GameScene::Update() {

	// 自キャラの更新
	player_->Update(viewProjection_);
	enemy_->Update();
	skydome_->Update();

	railCamera_->Update();
	debugCamera_->Update();

	

	ImGui::Begin("Debug1");
	// 敵発生処理
	// LoadEnemyPopData();

	UpdateEnemyPopCommands();

	// 敵の行進処理
	EnemyObjUpdate();


	// float3入力ボックス

	// ImGui::InputFloat3("InputFloat3", inputFloat3);

	// float3スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);

	ImGui::ShowDemoWindow();

	ImGui::End();

	// あたり判定
	CheckAllCollisions();

	// debugCamera_->Update();

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_K) == isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;

		// debugCamera->Update();
	} else if (input_->TriggerKey(DIK_K) == isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}

#endif // DEBUG

	// Cameraの処理
	if (isDebugCameraActive_) {

		debugCamera_->Update();

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();

	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	// 仮のシーン切り替え
	if (input_->TriggerKey(DIK_RETURN)) {
		GameSceneEnd_ = true;
	}
	
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画

	player_->Draw(viewProjection_);

	enemy_->Draw(viewProjection_);

	skydome_->Draw(viewProjection_);

	EnemyObjDraw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	player_->DrawUI();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullet();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& EnemyBullets = enemy_->GetBullet();

// 自キャラと敵弾のあたり判定
#pragma region

	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾のあたり判定
	for (EnemyBullet* bullet : EnemyBullets) {
		// 敵弾の座標

		posB = bullet->GetWorldPosition();

		float dx;
		float dy;
		float dz;
		float distance;
		float radius = 0.5f;

		// 座標AとBの距離を求める
		dx = (posB.x - posA.x) * (posB.x - posA.x);
		dy = (posB.y - posA.y) * (posB.y - posA.y);
		dz = (posB.z - posA.z) * (posB.z - posA.z);

		distance = dx + dy + dz;

		// 弾と弾の交差判定
		if (distance <= (radius + radius) * (radius + radius)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();

			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion
// 自弾と敵キャラのあたり判定
#pragma region
	Vector3 PosA, PosB;

	PosA = enemy_->GetWorldPosition();

	for (PlayerBullet* playerbullet : playerBullets) {
		// 自弾の座標
		
		PosB = playerbullet->GetWorldPosition();

		float Dx;
		float Dy;
		float Dz;
		float distance2;
		float radius2 = 1;

		// 距離を求める
		Dx = (PosB.x - PosA.x) * (PosB.x - PosA.x);
		Dy = (PosB.y - PosA.y) * (PosB.y - PosA.y);
		Dz = (PosB.z - PosA.z) * (PosB.z - PosA.z);

		distance2 = Dx + Dy + Dz;

		if (distance2 <= (radius2 + radius2) * (radius2 + radius2)) {
   			enemy_->OnCollision();

			playerbullet->OnCollision();
		}
	}

#pragma endregion
// 自弾と敵弾のあたり判定
#pragma region
	Vector3 BulletposA, BulletposB;

	for (PlayerBullet* BulletPlayer : playerBullets) 
	{
		BulletposA = BulletPlayer->GetWorldPosition();
		for (EnemyBullet* BulletEnemy : EnemyBullets) 
		{
			BulletposB = BulletEnemy->GetWorldPosition();

			float dX;
			float dY;
			float dZ;
			float distance3;
			float radius3 = 1;

			dX = (BulletposB.x - BulletposA.x) * (BulletposB.x - BulletposA.x);
			dY = (BulletposB.y - BulletposA.y) * (BulletposB.y - BulletposA.y);
			dZ = (BulletposB.z - BulletposA.z) * (BulletposB.z - BulletposA.z);

			distance3 = dX + dY + dZ;

			if (distance3 <= (radius3 + radius3) * (radius3 + radius3)) 
			{
				BulletEnemy->OnCollision();

				BulletPlayer->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet)
{
	enemyBullets_.push_back(enemyBullet); 
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");

	assert(file.is_open());

	// ファイルの内容を文字ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (waitFlag_) {
		waitTime_--;

		if (waitTime_ <= 0) {
			// 待機完了
			waitFlag_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// , 区切りで先頭の文字を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメントの行を飛ばす
			continue;
		}

		// POPコマンド

		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			Vector3 pos = {x, y, z};

			// 敵を発生させる
			EnemySpawn(pos);
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			waitFlag_ = true;
			waitTime_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

// 初期化
void GameScene::EnemySpawn(Vector3& position) {
	Enemy* enemy = new Enemy;

	enemy->Initialize(model_, EnemytextureHandle_, position);

	// Enemeyにplayerのアドレスを渡す
	enemy->SetPlayer(player_);

	enemy->SetGameScene(this);

	enemies_.push_back(enemy);
}

// アップデート
void GameScene::EnemyObjUpdate() {
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	enemies_.remove_if([](Enemy* enemy_) {
		if (enemy_->IsDead()) {
			delete enemy_;
			return true;
		}

		return false;
	});

	for (EnemyBullet* Enemybullet : enemyBullets_) {
		Enemybullet->Update();
	}

	enemyBullets_.remove_if([](EnemyBullet* Enemybullet) {
		if (Enemybullet->IsDead()) {
			delete Enemybullet;
			return true;
		}
		return false;
	});
}

// 描画
void GameScene::EnemyObjDraw() {

	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}

	// 弾の描画
	for (EnemyBullet* Enemybullet : enemyBullets_) {
		Enemybullet->Draw(viewProjection_);
	}
}
