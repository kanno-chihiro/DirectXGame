#pragma once
#include "Input.h"
#include "Model.h"
#include <list>
#include "Matrix/MatrixTransform.h"
#include"Vector/VectorTransform.h"
#include <PlayerBullet.h>
/// <summary>
/// 自キャラ
/// </summary>

class Player {
public:
	///<summary>
	///デストラクタ
	/// <summary>
	~Player();

	//弾
	PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;

	///<summary>
	///攻撃
	/// <summary>
	void Attack(Vector3& position);

	///< summary>
	/// 初期化
	///</summary>
	void Initialize(Model*model,uint32_t textureHandle);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>

	void Draw(ViewProjection &viewProjection);

private:

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	Input* input_=nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	MatrixTransform* matrix_ = new MatrixTransform();
	VectorTransform* vectorTransform_ = new VectorTransform();


};