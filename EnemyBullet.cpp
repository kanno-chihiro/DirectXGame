#include "EnemyBullet.h"
#include "MatrixTrans.h"
#include <cassert>


void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity){
	// NULLポインタ
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	EnemyBullettextureHandle_ = TextureManager::Load("RED.png");

	worldTransform_.Initialize();

	// 引数で受け取った初期化座標をセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	
};

void EnemyBullet::Update() {

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	// モデル
	model_->Draw(worldTransform_, viewProjection, EnemyBullettextureHandle_);
};
