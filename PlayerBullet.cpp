#include "ImGuiManager.h"
#include "Model.h"
#include <PlayerBullet.h>


void PlayerBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) {
	// NULLポインタ
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("Block.png");

	/*worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};*/
	worldTransform_.Initialize();

	//引数で受け取った初期化座標をセット
	worldTransform_.translation_ = position;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

};

void PlayerBullet::Update() {
	worldTransform_.UpdateMatrix();
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	// モデル
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
};

