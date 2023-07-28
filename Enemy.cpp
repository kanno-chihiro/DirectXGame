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
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.Initialize();
};

void Enemy::Update(){

	
	// 行列を定数バッファに転送

	const float kClientVelocity = 0.2f;
	Vector3 Enemymove = {0, 0, 0};

	// 回転速さ{ラジアン/frame}
	const float kEnemySpeed = 0.2f;

	worldTransform_.translation_.z -= kEnemySpeed;
	

	// 行列の更新
	worldTransform_.UpdateMatrix();
};

void Enemy::Draw(ViewProjection& viewProjection){
	// 3Dモデルを描画
	model__->Draw(worldTransform_, viewProjection, textureHandle__);
};

