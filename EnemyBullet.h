#pragma once
#include "Model.h"
#include "WorldTransform.h"

class EnemyBullet {
public:
	

	//初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	
	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);
	
	bool IsDead() const { return isDead_; }

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	
	ViewProjection viewprojection_; 

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t EnemyBullettextureHandle_ = 0u;


	// 速度
	Vector3 velocity_ ;

	// 寿命
	static const int32_t kListTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kListTime;

	// デスフラグ
	bool isDead_ = false;


};

