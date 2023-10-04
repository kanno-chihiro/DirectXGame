#pragma once
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "WorldTransform.h"
#include <list>
#include "Sprite.h"


class Player {
public:
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle,Vector3 Startpos);

	// 更新処理
	void Update(ViewProjection& viewProjection);

	// 描画
	void Draw(ViewProjection& viewProjection);

	// 攻撃
	void Attack(Vector3& position);

	// デストラクタ
	~Player();
	
	// ワールド座標を取得
	Vector3 GetWorldPosition() { return worldTransform_.translation_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リスト
	const std::list<PlayerBullet * > & GetBullet() const { return bullets_; }

	//親となるワールドトランスフォームをセット
	void SetPrent(const WorldTransform* parent);

	void DrawUI();

	void UpdateReticle(ViewProjection& viewProjection);



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

	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;


	Vector2 ReticlePos = {640, 360};
	Vector4 color = {1, 1, 1, 1};
	Vector2 anchor = {0.5f, 0.5f};

	// マウス
	void GetmousePoint();
	POINT mausePosition;

	void GetReticlePoint();

	

	
};