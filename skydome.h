#pragma once
#include "Model.h"
#include "WorldTransform.h"



class skydome {
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection_);

	private:
	WorldTransform worldTransform_;

	ViewProjection viewProjection_;

	Model* model_ = nullptr;

};
