#include<player.h>
#include "ImGuiManager.h"

Player::~Player() {
	// bullet_の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet_;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	input_ = Input::GetInstance();
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.Initialize();
	
};



void Player::Attack(Vector3& position) { 
	if (input_->TriggerKey(DIK_SPACE))
	{
		////弾があれば解放する
		//if (bullet_) {
		//	delete bullet_;
		//	bullet_ = nullptr;
		//}

		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_,position);

		//弾を登録する
		//bullet_ = newBullet;
		bullets_.push_back(newBullet);
	}
}

void Player::Update() 
{
	// 行列を定数バッファに転送

	const float kClientVeloctiy = 0.2f;
	Vector3 move = {0, 0, 0};

	// 回転速さ{ラジアン/frame}
	// const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		// worldTransformのY軸まわり角度を回転速さ分減算する
		worldTransform_.translation_.y;
	} else if (input_->PushKey(DIK_D)) {
		// worldTransformのY軸まわり角度を回転速さ分加算する
		worldTransform_.translation_.y;
	}

	ImGui::Begin("Debug");
	float playerPos[] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("PlayerPos", playerPos, 0.0f, 1.0f);
	// ↑処理のままだとSliderFlaot3でplayerPosの値を変えているので実際の座標(translation)が
	// 変わっていないのでここで変更する
	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.y = playerPos[1];
	worldTransform_.translation_.z = playerPos[2];
	ImGui::End();

	// 限界座標
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

	//	範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);


	if (input_->PushKey(DIK_UP))
	{
		move.y += kClientVeloctiy;
	}
	if (input_->PushKey(DIK_DOWN)) {

		move.y -= kClientVeloctiy;
	}
	if (input_->PushKey(DIK_RIGHT)) {

		move.x += kClientVeloctiy;
	}
	if (input_->PushKey(DIK_LEFT)) {

		move.x -= kClientVeloctiy;
	}

	// 攻撃処理
	Attack(worldTransform_.translation_);

	//弾の更新
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Update();
	}

	/*if (bullet_) {
		bullet_->Update();
	}*/

	worldTransform_.translation_ = 
		vectorTransform_->Add(worldTransform_.translation_, move);


	worldTransform_.matWorld_=
	MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	
	
	

	//行列の更新
	worldTransform_.TransferMatrix();
	


};


void Player::Draw(ViewProjection &viewProjection) {
	//3Dモデルを描画
	model_->Draw(worldTransform_,viewProjection, textureHandle_);

	//弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	/*if (bullet_) {
		bullet_->Draw(viewProjection);
	}*/

};
