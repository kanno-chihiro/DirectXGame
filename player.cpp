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


void Player::Initialize(Model* model, uint32_t textureHandle)
{

	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

};


void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
};


void Player::Draw(ViewProjection &viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldTransform_,viewProjection, textureHandle_);

};
