﻿#pragma once
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "WorldTransform.h"
#include <list>

class Player {
public:
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);

	// 更新処理
	void Update();

	// 描画
	void Draw(ViewProjection& viewProjection);

	// 攻撃
	void Attack(Vector3& position);

	// デストラクタ
	~Player();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	ViewProjection viewProjection_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	// 弾
	PlayerBullet* bullet_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;
};
