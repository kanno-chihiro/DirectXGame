#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Player.h>
#include <Enemy.h>
#include <DebugCamera.h>
#include "skydome.h"
#include "RailCamera.h"
#include <sstream>
#include "Scene.h"
#include"reset.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
	

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
	///  衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void LoadEnemyPopData();

	void UpdateEnemyPopCommands();

	bool GameSceneEnd_ = false;

	bool IsSceneEnd() { return GameSceneEnd_; }
	SCENE NextScene() { return SCENE::Clear; }

	bool ResetSceneEnd_ = false;

	void Start() { GameSceneEnd_ = false; }

	

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	void EnemySpawn(Vector3& position);
	void EnemyObjUpdate();
	void EnemyObjDraw();



	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	
	
	//3Dモデルデータ
	Model* model_ = nullptr;

	//ワールド変換
	WorldTransform worldTransform_;
	
	//ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	Player* player_ = nullptr;
	float inputFloat3[3] = {0, 0, 0};

	bool isDebugCameraActive_ = false;

	DebugCamera* debugCamera_ = nullptr;

	// 敵キャラ
	Enemy* enemy_ = nullptr;

	// tekiテクスチャハンドル
	uint32_t EnemytextureHandle_ = 0;

	skydome* skydome_ = nullptr;
	Model* skydomeModel_ = nullptr;
	RailCamera* railCamera_ = nullptr;

	//敵と弾のリスト
	std::list<EnemyBullet*> enemyBullets_;
	std::list<Enemy*> enemies_;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	bool waitFlag_ = false;
	int32_t waitTime_ = 0;

	


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
