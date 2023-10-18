#include "Ground.h"
#include <assert.h>

void Ground::Initialize(Model* model) 
{
	assert(model);

	Groundmodel_ = model;

	worldTransform_.Initialize();

	worldTransform_.scale_ = {40.0f, 40.0f, 40.0f};

	worldTransform_.translation_.z = 8.0f;
}

void Ground::Update() 
{
	worldTransform_.UpdateMatrix(); 
}

void Ground::Draw(ViewProjection& viewProjection_)
{
	Groundmodel_->Draw(worldTransform_, viewProjection_);
}