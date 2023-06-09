#include<player.h>
#include <cassert>
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "ImGuiManager.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{

	input_ = Input::GetInstance();
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.Initialize();
	
};


void Player::Update() 
{
	// 行列を定数バッファに転送

	const float kClientVeloctiy = 0.2f;
	Vector3 move = {0, 0, 0};
	
	if (input_->PushKey(DIK_UP))
	{
		move.y += kClientVeloctiy;
	}
	if (input_->PushKey(DIK_DOWN)) {

		move.y -= kClientVeloctiy;
	}
	if (input_->PushKey(DIK_RIGHT)) {

		move.x += kClientVeloctiy;
	}
	if (input_->PushKey(DIK_LEFT)) {

		move.x -= kClientVeloctiy;
	}

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);


	worldTransform_.matWorld_=
	MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	
	Vector3 result;
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	

	//スケーリング行列
	Matrix4x4 matScale = {0};//4行4列
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;
	matScale.m[3][3] = 1;

	//X軸回転行列
	Matrix4x4 matRotX = {0};
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sinf(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sinf(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cosf(worldTransform_.rotation_.x);
	matRotX.m[3][3] = 1; 

	// Y軸回転行列
	Matrix4x4 matRotY = {0};
	matRotX.m[0][0] = cosf(worldTransform_.rotation_.y);;
	matRotX.m[1][1] = 1;
	matRotX.m[0][2] = -sinf(worldTransform_.rotation_.y);
	matRotX.m[2][0] = sinf(worldTransform_.rotation_.y);
	matRotX.m[2][2] = cosf(worldTransform_.rotation_.y);
	matRotX.m[3][3] = 1; 

	// Z軸回転行列
	Matrix4x4 matRotZ = {0};
	matRotX.m[0][0] = cosf(worldTransform_.rotation_.z);;
	matRotX.m[1][0] = sinf(worldTransform_.rotation_.z);;
	matRotX.m[0][1] = -sinf(worldTransform_.rotation_.z);
	matRotX.m[1][1] = cosf(worldTransform_.rotation_.z);
	matRotX.m[2][2] = 1;
	matRotX.m[3][3] = 1; 

	Matrix4x4 matRot =Multiply(Multiply(matRotZ , matRotX) , matRotY);

	//平行移動行列を行う
	Matrix4x4 matTrans = {0};

	matTrans.m[0][0] = 1;
	matTrans.m[1][1] = 1;
	matTrans.m[2][2] = 1;
	matTrans.m[3][3] = 1;
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;

	worldTransform_.matWorld_ = Multiply(Multiply( matScale , matRot) , matTrans);

	//ImGui::Begin("Debug");
	//float playerPos[] = {
	//    worldTransform_.translation_.x, worldTransform_.translation_.y,
	//    worldTransform_.translation_.z};
	// ImGui::SliderFlaot3("PlayerPos", playerPos, -18, 18);
	//// ↑処理のままだとSliderFlaot3でplayerPosの値を変えているので実際の座標(translation)が
	//// 変わっていないのでここで変更する
	//worldTransform_.translation_.x = playerPos[0];
	//worldTransform_.translation_.y = playerPos[1];
	//worldTransform_.translation_.z = playerPos[2];
	//ImGui::End;

	// 限界座標
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

	//	範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitX);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitX);


	//行列の更新
	worldTransform_.TransferMatrix();
	
};


void Player::Draw(ViewProjection &viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldTransform_,viewProjection, textureHandle_);

};
