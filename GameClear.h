#pragma once
#include "Audio.h"
#include <DirectXCommon.h>
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Scene.h"
#include "GameScene.h"

class GameClear {
public:
	GameClear();

	~GameClear();

	void Initialize();

	void Update();

	void Draw();

	bool ClearSceneEnd_ = false;

	bool IsSceneEnd() { return ClearSceneEnd_; }
	SCENE NextScene() { return SCENE::Title; }

	bool GameSceneEnd_ = false;

	void Start() { ClearSceneEnd_ = false; }


private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t gamecleartexture_ = 0;

	Sprite* sprite_ = nullptr;

	// ワールド変換
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	

};
