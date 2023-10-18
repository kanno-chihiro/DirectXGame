#pragma once

#include <memory>
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <player.h>
#include "skydome.h"
#include "Ground.h"
#include "DebugCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	
	//3Dモデルデータ
	std::unique_ptr<Model>model_;
	// 3Dモデルデータ
	std::unique_ptr<Model> model__;
	std::unique_ptr<Model> GroundModel_;

	// ワールド変換
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//自キャラ
	std::unique_ptr<Player> player_;

	std::unique_ptr<skydome> skydome_;

	std::unique_ptr<Ground> Ground_;


	

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
