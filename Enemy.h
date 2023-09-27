#pragma once
#include "EnemyBullet.h"
#include "Model.h"
#include "WorldTransform.h"
#include <list>

enum class Phase {
	Approach,
	Leave,
};

//
class Player;

class Enemy {
public:
	
	void Initialize(Model* model, uint32_t textureHandle);

	
	void Update();

	
	void Draw(ViewProjection& viewProjection);

	
	void Fire(Vector3& position_);

	
	~Enemy();

	
	static const int kFireInterval = 50;

	
	void Phase_();

	
	void SetPlayer(Player* player) { player_ = player; }

	
	Vector3 GetWorldPosition();

	void OnCollision();

	const std::list<EnemyBullet * > & GetBullet() const { return Enemybullets_; }

private:
	
	WorldTransform worldTransform_;

	ViewProjection viewProjection_;

	
	Model* model_ = nullptr;

	uint32_t EnemytextureHandle_ = 0u;

	
	Phase phase_ = phase_;

	
	EnemyBullet* Enemybullet_ = nullptr;

	
	std::list<EnemyBullet*> Enemybullets_;

	
	int32_t fireTimer = 0;

	
	Player* player_ = nullptr;
};
