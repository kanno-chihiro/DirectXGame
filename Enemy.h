#pragma once
#include "Model.h"
#include <list>
#include "WorldTransform.h"
#include "EnemyBullet.h"

/// <summary>
/// 敵
/// </summary>

enum Phase { 
	Approach,
	Leave
};

class Enemy {
public:

	void Initialize(Model* model, uint32_t textureHandle);



	///< summary>
	/// 初期化
	///</summary>
	


	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>

	void Draw(ViewProjection& viewProjection);

	void Fire(Vector3& position);

	~Enemy();

	static const int kFireinterval = 60;

	void Phase_();

	private:
		//ワールド変換データ
	    WorldTransform worldTransform_;

		ViewProjection viewProjection_;


		//モデル
	    Model* model_ = nullptr;

		

		//テクスチャハンドル
	    uint32_t EnemytextureHandle_ = 0u;

		Phase phase_ = phase_;

	    EnemyBullet * Enemybullet_ = nullptr;

		std::list<EnemyBullet*> Enemybullets_;


		int32_t BulletTimer = 0;

	   

};
