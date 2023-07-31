#include <Enemy.h>
#include "ImGuiManager.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	input_ = Input::GetInstance();
	// NULLポインタチェック
	//初期化
	assert(model);
	model__ = model;
	textureHandle__ = textureHandle;
	worldTransform_.translation_ = {0.0f, 3.0f, +50.0f};
	worldTransform_.Initialize();
};

void Enemy::Update(){

	// 行列を定数バッファに転送

	const float kClientVelocity = 0.2f;
	Vector3 Enemymove = {0, 0, 0};

	// 回転速さ{ラジアン/frame}
	const float kEnemySpeed = 0.2f;

	

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:

		//移動(ベクトルを加算)
		worldTransform_.translation_.z -= kEnemySpeed;
		//規定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Enemy::Phase::Leave;
		}
		break;
	case Enemy::Phase::Leave:
			//移動(ベクトルを加算)
		worldTransform_.translation_.x -= kEnemySpeed;
		worldTransform_.translation_.y += kEnemySpeed;
		worldTransform_.translation_.z -= kEnemySpeed;
		break;
	}


	// 行列の更新
	worldTransform_.UpdateMatrix();
};

void Enemy::Draw(ViewProjection& viewProjection){
	// 3Dモデルを描画
	model__->Draw(worldTransform_, viewProjection, textureHandle__);
};

