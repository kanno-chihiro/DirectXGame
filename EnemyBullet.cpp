#include "EnemyBullet.h"
#include "ImGuiManager.h"
#include <Model.h>


void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity){
	// NULL�|�C���^
	assert(model);

	model__ = model;
	// �e�N�X�`���ǂݍ���
	textureHandle__ = TextureManager::Load("Block.png");

	worldTransform_.Initialize();

	// �����Ŏ󂯎�������������W���Z�b�g
	worldTransform_.translation_ = position;

	// �����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	
};

void EnemyBullet::Update() {
	worldTransform_.UpdateMatrix();
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	// ���f��
	model__->Draw(worldTransform_, viewProjection, textureHandle__);
};
