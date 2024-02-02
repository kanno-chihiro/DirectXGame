#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "RailCamera.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "skydome.h"
#include <DebugCamera.h>
#include <Enemy.h>
#include <Player.h>
#include <sstream>
#include "Scene.h"
#include "GameClear.h"

class TitleScene 
{
public:
	TitleScene();

	~TitleScene();

	void Initialize();

	void Update();

	void Draw();
	

	bool TitleSceneEnd_ = false;

	bool IsSceneEnd() { return TitleSceneEnd_; }
	SCENE NextScene() { return SCENE::Reset; }

	bool ClearSceneEnd_ = false;

	void Start() { TitleSceneEnd_ = false; }

 private:

		DirectXCommon* dxCommon_ = nullptr;
	    Input* input_ = nullptr;
	    Audio* audio_ = nullptr;

		// テクスチャハンドル
	    uint32_t titletexture_ = 0;

		Sprite* sprite_ = nullptr;

	    // ワールド変換
	    WorldTransform worldTransform_;

	    // ビュープロジェクション
	    ViewProjection viewProjection_;

		
		
		
};
