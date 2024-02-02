#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "GameClear.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "TitleScene.h"
#include "WinApp.h"
#include "reset.h"

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
	GameClear* gameclear = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"LE2D_08_カンノ_チヒロ_UNIVERSE");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	//ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	//imguiManager->Initialize(win, dxCommon);

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

	// タイトルシーンの初期化
	titleScene = new TitleScene();
	titleScene->Initialize();

	// 説明シーンの初期化
	resetScene = new reset();
	resetScene->Initialize();

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();

	// ゲームクリアの初期化
	gameclear = new GameClear();
	gameclear->Initialize();

	SCENE sceneNo = SCENE::Title;

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
	//	imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();

		switch (sceneNo) {
		case SCENE::Title:

			titleScene->Update();

			if (titleScene->IsSceneEnd() == true) {
				sceneNo = titleScene->NextScene();
				resetScene->Start();
			}

			break;
		case SCENE::Reset:

			resetScene->Update();

			if (resetScene->IsSceneEnd() == true) {
				sceneNo = resetScene->NextScene();
				gameScene->Start();
				
			}

			break;
		case SCENE::Game:

			gameScene->Update();

			if (gameScene->IsSceneEnd() == true) {
				sceneNo = gameScene->NextScene();
				gameclear->Start();
			}

			break;
		case SCENE::Clear:

			gameclear->Update();
			
			if (gameclear->IsSceneEnd() == true) {
				sceneNo = gameclear->NextScene();
				titleScene->Start();
				gameScene->EnemyRESET();
			}

			break;
		}

		// titleScene->Update();
		// resetScene->Update();
		// gameScene->Update();
		// gameclear->Update();

		// 軸表示の更新
		//axisIndicator->Update();
		// ImGui受付終了
	//	imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();
		switch (sceneNo) {
		case SCENE::Title:
			titleScene->Draw();
			break;

		case SCENE::Reset:
			resetScene->Draw();
			
			break;

		case SCENE::Game:
			gameScene->Draw();
			break;

		case SCENE::Clear:
			gameclear->Draw();
			break;
		}

		/*titleScene->Draw();
		resetScene->Draw();
		gameScene->Draw();
		gameclear->Draw();*/

		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
	//	imguiManager->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	SafeDelete(gameScene);
	audio->Finalize();
	// ImGui解放
//	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}