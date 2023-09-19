#include "EnemyBullet.h"
#include "ImGuiManager.h"
#include <Model.h>


void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity){
	// NULLポインタ
	assert(model);

	model__ = model;
	// テクスチャ読み込み
	textureHandle__ = TextureManager::Load("Block.png");

	worldTransform_.Initialize();

	// 引数で受け取った初期化座標をセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	
};

void EnemyBullet::Update() {
	worldTransform_.UpdateMatrix();
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	// モデル
	model__->Draw(worldTransform_, viewProjection, textureHandle__);
};
