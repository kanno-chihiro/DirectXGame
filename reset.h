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

class reset {
public:
	reset();

	~reset();

	void Initialize();

	void Update();

	void Draw();

	bool ResetSceneEnd_ = false;

	bool IsSceneEnd() { return ResetSceneEnd_; }
	SCENE NextScene() { return SCENE::Game; }

	bool TitleSceneEnd_ = false;

	void Start() { ResetSceneEnd_ = false; }


	private:

		DirectXCommon* dxCommon_ = nullptr;
	    Input* input_ = nullptr;
	    Audio* audio_ = nullptr;

		// テクスチャハンドル
	    uint32_t resettexture_ = 0;

	    Sprite* sprite_ = nullptr;

	    // ワールド変換
	    WorldTransform worldTransform_;

	    // ビュープロジェクション
	    ViewProjection viewProjection_;

	   
		
};
