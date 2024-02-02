﻿#include "Player.h"
#include "ImGuiManager.h"
#include "MatrixTrans.h"
#include <cassert>


// bullet_の解放
Player::~Player() {

	for (PlayerBullet* bullet : bullets_) 
	{
		delete bullet_;
	}
	delete sprite2DReticle_;
}



//
// 初期化
void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 Startpos) {

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	// キーボード
	input_ = Input::GetInstance();

	// スケーリング
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	// 回転
	// worldTransform_.rotation_ = {0.0f, 3.14f / 4.0f, 0.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	// 平行移動行列
	worldTransform_.translation_ = Startpos; //{0.0f, 0.0f, 0.0f};
	Startpos.z = 5.0f;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	//3Dレティクルのワールドトランスフォームの初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("Reticle.png");

	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, ReticlePos, color, anchor);
}

//
// 更新処理
void Player::Update(ViewProjection& viewProjection) {

	// デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}

		return false;
	});

	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	const float kRotSpeed = 0.02f;

	

	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;

	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// 自機の旋回
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}

	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	// 移動限界座標
	const float kMoveLimitX = (35.0f);
	const float kMoveLimitY = (18.0f);

	// 範囲を越えない処理

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	
	// 足し算
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	worldTransform_.UpdateMatrix();

	worldTransform_.TransferMatrix();

	Vector3 position = {
	    position.x = worldTransform_.matWorld_.m[3][0],
	    position.y = worldTransform_.matWorld_.m[3][1],
	    position.z = worldTransform_.matWorld_.m[3][2],
	};

	// キャラクター攻撃処理
	Attack(position);

	// 弾更新
	/*if (bullet_)
	{
	    bullet_->Update();
	}*/

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	

	// デバック画面
	/*ImGui::Begin("Debug");

	float playerPos[] =
	{
	    worldTransform_.translation_.x,
	    worldTransform_.translation_.y,
	    worldTransform_.translation_.z
	};
	ImGui::SliderFloat3("PlayerPos", playerPos, 1000, 720);



	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.x = playerPos[1];
	worldTransform_.translation_.x = playerPos[2];

	ImGui::End();*/

	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;

	// 自機から3dレティクルへのオフセット(z+向き)
	Vector3 offset = {0, 0, 1.0f};

	// ベクトルの長さを整える
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	offset = Normalize(offset);

	offset.x *= kDistancePlayerTo3DReticle;
	offset.y *= kDistancePlayerTo3DReticle;
	offset.z *= kDistancePlayerTo3DReticle;

	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_.x = offset.x + GetWorldPosition().x;
	worldTransform3DReticle_.translation_.y = offset.y + GetWorldPosition().y;
	worldTransform3DReticle_.translation_.z = offset.z + GetWorldPosition().z;

	worldTransform3DReticle_.UpdateMatrix();

	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = {
	    worldTransform3DReticle_.matWorld_.m[3][0],
		worldTransform3DReticle_.matWorld_.m[3][1],
	    worldTransform3DReticle_.matWorld_.m[3][2]
	};

	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);


	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
	    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = Transform(positionReticle, matViewProjectionViewport);

	// スプライトのレティクルに座標を設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	// マウスでカーソルを動かす
	GetmousePoint();

	
	UpdateReticle(viewProjection);
}

//
// 描画
void Player::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	/*if (bullet_)
	{
	    bullet_->Draw(viewProjection);

	}*/
}

void Player::Attack(Vector3& position) {

	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾があれば解放する
		/*if (bullet_)
		{
		    delete bullet_;
		    bullet_ = nullptr;
		}*/

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		

		// 自機から照準オブジェクトへのベクトル
		Vector3 pos;
		pos.x = worldTransform3DReticle_.translation_.x - worldTransform_.translation_.x;
		pos.y = worldTransform3DReticle_.translation_.y - worldTransform_.translation_.y;
		pos.z = worldTransform3DReticle_.translation_.z - worldTransform_.translation_.z;

		velocity = {pos.x, pos.y, pos.z};

		velocity = Normalize(velocity);


		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, position, velocity);
		// 弾を登録する
		// bullet_ = newBullet;

		bullets_.push_back(newBullet);
	}
}



void Player::OnCollision() 
{

}

void Player::SetPrent(const WorldTransform* parent) { 
	worldTransform_.parent_ = parent; 
}

// UI描画(レティクル)
void Player::DrawUI() 
{
	sprite2DReticle_->Draw(); 
}

void Player::UpdateReticle(ViewProjection& viewProjection) {
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matVPV =
	    Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));

	Matrix4x4 matInverseVPV = Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(
	    (float)sprite2DReticle_->GetPosition().x, (float)sprite2DReticle_->GetPosition().y, 0);

	Vector3 posFar = Vector3(
	    (float)sprite2DReticle_->GetPosition().x, (float)sprite2DReticle_->GetPosition().y, 1);

	// スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mauseDirection = Subtract(posFar, posNear);
	mauseDirection = Normalize(mauseDirection);

	// カメラから照準オブジェクトへの距離
	const float kDistanceTestObject = 50.0f;

	worldTransform3DReticle_.translation_.x = posNear.x + mauseDirection.x * kDistanceTestObject;
	worldTransform3DReticle_.translation_.y = posNear.y + mauseDirection.y * kDistanceTestObject;
	worldTransform3DReticle_.translation_.z = posNear.z + mauseDirection.z * kDistanceTestObject;

	worldTransform3DReticle_.UpdateMatrix();

	//// デバッグ表示
	//ImGui::Begin("player");
	//ImGui::Text("2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	//ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	//ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	//ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	//    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	//ImGui::End();
}



//  マウス関数
void Player::GetmousePoint() {
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mausePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mausePosition);

	ReticlePos.x = float(mausePosition.x);
	ReticlePos.y = float(mausePosition.y);

	sprite2DReticle_->SetPosition(ReticlePos);
}

void Player::GetReticlePoint() {}





