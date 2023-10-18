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


void Player::Initialize(Model* model)
{

	//NULLポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	worldTransform_.scale_ = {40.0f, 40.0f, 40.0f};

	worldTransform_.translation_.z = 8.0f;

};


void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
};


void Player::Draw(ViewProjection &viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldTransform_,viewProjection);

};
