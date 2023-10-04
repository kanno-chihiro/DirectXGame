#pragma once
#include "Audio.h"
#include <DirectXCommon.h>
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class TitleScene 
{
public:
	TitleScene();

	~TitleScene();

	void Initialize();

	void Update();

	void Draw();

	private:

		DirectXCommon* dxCommon_ = nullptr;
	    Input* input_ = nullptr;
	    Audio* audio_ = nullptr;

};
