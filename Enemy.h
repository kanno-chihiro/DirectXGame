#pragma once
#include "Input.h"
#include "Model.h"
#include <list>
#include "Matrix/MatrixTransform.h"
#include"Vector/VectorTransform.h"
//#include <EnemyBullet.h>

/// <summary>
/// 敵
/// </summary>

class Enemy {
public:
	///< summary>
	/// 初期化
	///</summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>

	void Draw(ViewProjection& viewProjection);

	private:
		//ワールド変換データ
	    WorldTransform worldTransform_;

		//モデル
	    Model* model__ = nullptr;

		Input* input_ = nullptr;

		//テクスチャハンドル
	    uint32_t textureHandle__ = 0u;

		// 行動フェーズ
	    enum class Phase {
		    Approach, // 接近する
		    Leave,    // 離脱する
	    };

	    Phase phase_ = Enemy::Phase::Approach;


};
