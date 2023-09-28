#include "skydome.h"
#include <assert.h>

void skydome::Initialize(Model* model) 
{
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.scale_ = {40.0f, 40.0f, 40.0f};

	worldTransform_.translation_.z = 8.0f;
}

void skydome::Update() 
{
	worldTransform_.UpdateMatrix();

}

void skydome::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_);
}