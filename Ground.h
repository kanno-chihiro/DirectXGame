#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Ground {
public:

	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection_);

	private:
	WorldTransform worldTransform_;

	ViewProjection viewProjection_;

	Model* Groundmodel_ = nullptr;

};
