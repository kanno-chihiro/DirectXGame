#include "MatrixTrans.h"

class EnemyBullet {
public:
	// 速度
	Vector3 velocity_ = {0, 0};

	//初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	
	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

	private:

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model__ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle__ = 0u;
};
