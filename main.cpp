#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "TitleScene.h"
#include <reset.h>

enum SCENE { 
	scene0, 
	scene1, 
	scene2, 
	scene3, 
	scene4 };

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	GameScene* gameScene = nullptr;
	TitleScene* titleScene = nullptr;
	reset* resetScene = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"LE2C_09_カンノ_チヒロ");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	int sceneNo = scene0;

	
	//タイトルシーンの初期化
	titleScene = new TitleScene();
	titleScene->Initialize();

	//説明シーンの初期化
	resetScene = new reset();
	resetScene->Initialize();

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		
		switch (sceneNo) {
		case scene0:
			if (input->PushKey(DIK_SPACE)) {
				sceneNo = scene1;
			}
			break;
		case scene1:
			if (input->PushKey(DIK_SPACE)) {
				sceneNo = scene2;
			}
			break;
		case scene2:



			break;
		case scene3:
			if (input->PushKey(DIK_SPACE))
			{
				sceneNo = scene0;
			}
			break;
		case scene4:
			if (input->PushKey(DIK_SPACE))
			{
				sceneNo = scene0;
			}
			break;
		}


		// タイトルシーンの毎フレーム処理
		titleScene->Update();
		//説明シーンの毎フレーム処理
		resetScene->Update();
		// ゲームシーンの毎フレーム処理
		gameScene->Update();


		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();



		// 描画開始
		dxCommon->PreDraw();

		switch (sceneNo) {
		case scene0:
			// タイトルシーンの描画
			titleScene->Draw();
			break;
		case scene1:
			// タイトルシーンの描画
			resetScene->Draw();
			break;
		case scene2:
			// ゲームシーンの描画
			gameScene->Draw();
			break;
		case scene3:
			//ゲームクリアの描画
			break;
		case scene4:
			//ゲームオーバーの描画
			break;
		}


		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	SafeDelete(gameScene);
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}