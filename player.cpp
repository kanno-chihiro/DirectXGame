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

	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

};


void Player::Update() {
	// �s���萔�o�b�t�@�ɓ]��
	worldTransform_.TransferMatrix();
};


void Player::Draw(ViewProjection &viewProjection) {

	//3D���f����`��
	model_->Draw(worldTransform_,viewProjection, textureHandle_);

};
