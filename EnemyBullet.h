﻿#pragma once
#include "Model.h"
#include "WorldTransform.h"

class EnemyBullet {
public:
	
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	
	void Update();

	
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

private:
	
	WorldTransform worldTransform_;

	ViewProjection viewProjection_;

	
	Model* model_ = nullptr;

	
	uint32_t EnemyBulletHandle_ = 0u;

	
	Vector3 velocity_;

	
	static const int32_t kLifeTime = 60 * 5;

	
	int32_t deathTimer_ = kLifeTime;

	
	bool isDead_ = false;
};
