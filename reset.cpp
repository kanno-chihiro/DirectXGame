#include "reset.h"
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

#include "TitleScene.h"
reset::reset() {}

reset::~reset() { delete sprite_; }

void reset::Initialize() {

	//シーンのクラス化をするときはCPPに下の三つが必要
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	resettexture_ = TextureManager::Load("reset.png");

	sprite_ = Sprite::Create(resettexture_, {0, 0});
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 1000.0f;

	worldTransform_.Initialize();

	viewProjection_.Initialize();

}

void reset::Update() {

	if (input_->TriggerKey(DIK_SPACE)) {
		ResetSceneEnd_ = true;
	}
	
}



void reset::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	sprite_->Draw();

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
