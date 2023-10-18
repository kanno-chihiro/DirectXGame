#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// 自キャラ
/// </summary>

class Player {
public:

	///< summary>
	/// 初期化
	///</summary>
	void Initialize(Model*model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>

	void Draw(ViewProjection &viewProjection_);

private:

	//ワールド変換データ
	WorldTransform worldTransform_;

	ViewProjection viewProjection_;

	//モデル
	Model* model_ = nullptr;

	
	

};