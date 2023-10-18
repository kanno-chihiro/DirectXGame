#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "MatrixTrans.h"
#include "Player.h"
#include "TextureManager.h"
#include <cassert>
#include "skydome.h"
#include "Ground.h"
#include <fstream>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	//delete model_;
	//// 自キャラの解放
	//delete player_;
	//delete skydomeModel_;
	//delete Groundmodel_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugCamera_ = new DebugCamera(-18, 18);

	//ファイル名を指定してテクスチャを読み込む
	model_.reset (Model::CreateFromOBJ("Player",true));

	model__.reset (Model::CreateFromOBJ("skydome", true));

	GroundModel_.reset(Model::CreateFromOBJ("Ground", true));

	
	//3Dモデルの生成
	//model_.reset(Model::Create());
	

	worldTransform_.Initialize();

	viewProjection_.translation_ = {0, +3, -50};
	viewProjection_.Initialize();

	//自キャラの生成
	player_ = std::make_unique<Player>();
	//自キャラの初期化
	player_->Initialize(model_.get());

	// 背景
	skydome_ = std::make_unique<skydome>();

	skydome_->Initialize(model__.get());

	//地面
	Ground_ = std::make_unique<Ground>();

	Ground_->Initialize(GroundModel_.get());

		// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() {
//自キャラの更新
	player_->Update();
	skydome_->Update();
	Ground_->Update();
	debugCamera_->Update();

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
#endif 

	// カメラ
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

	//自キャラの描画
	player_->Draw(viewProjection_);

	skydome_->Draw(viewProjection_);

	Ground_->Draw(viewProjection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
