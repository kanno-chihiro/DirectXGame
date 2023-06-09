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
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.Initialize();
	
};


void Player::Update() 
{
	// �s���萔�o�b�t�@�ɓ]��

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

	

	//�X�P�[�����O�s��
	Matrix4x4 matScale = {0};//4�s4��
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;
	matScale.m[3][3] = 1;

	//X����]�s��
	Matrix4x4 matRotX = {0};
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sinf(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sinf(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cosf(worldTransform_.rotation_.x);
	matRotX.m[3][3] = 1; 

	// Y����]�s��
	Matrix4x4 matRotY = {0};
	matRotX.m[0][0] = cosf(worldTransform_.rotation_.y);;
	matRotX.m[1][1] = 1;
	matRotX.m[0][2] = -sinf(worldTransform_.rotation_.y);
	matRotX.m[2][0] = sinf(worldTransform_.rotation_.y);
	matRotX.m[2][2] = cosf(worldTransform_.rotation_.y);
	matRotX.m[3][3] = 1; 

	// Z����]�s��
	Matrix4x4 matRotZ = {0};
	matRotX.m[0][0] = cosf(worldTransform_.rotation_.z);;
	matRotX.m[1][0] = sinf(worldTransform_.rotation_.z);;
	matRotX.m[0][1] = -sinf(worldTransform_.rotation_.z);
	matRotX.m[1][1] = cosf(worldTransform_.rotation_.z);
	matRotX.m[2][2] = 1;
	matRotX.m[3][3] = 1; 

	Matrix4x4 matRot =Multiply(Multiply(matRotZ , matRotX) , matRotY);

	//���s�ړ��s����s��
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
	//// �������̂܂܂���SliderFlaot3��playerPos�̒l��ς��Ă���̂Ŏ��ۂ̍��W(translation)��
	//// �ς���Ă��Ȃ��̂ł����ŕύX����
	//worldTransform_.translation_.x = playerPos[0];
	//worldTransform_.translation_.y = playerPos[1];
	//worldTransform_.translation_.z = playerPos[2];
	//ImGui::End;

	// ���E���W
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

	//	�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitX);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitX);


	//�s��̍X�V
	worldTransform_.TransferMatrix();
	
};


void Player::Draw(ViewProjection &viewProjection) {

	//3D���f����`��
	model_->Draw(worldTransform_,viewProjection, textureHandle_);

};
