#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include"ImGuiManager.h"


class RailCamera {
public:
	void Initialize(Vector3 position,Vector3& radian);

	void Update();



	ViewProjection GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	//void Draw();

	private:

		WorldTransform worldTransform_;

		ViewProjection viewProjection_;

		Model* model_ = nullptr;

};
