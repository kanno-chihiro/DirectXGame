#include "MatrixTrans.h"


class PlayerBullet {
public:
	// 速度
	Vector3 velocity_ = {0, 0};

	///< summary>
	///
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初座標</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	///< summary>
	/// 更新
	///  </summary>
	void Update();

	///< summary>
	/// 描画
	///  </summary>
	///  <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;
};
