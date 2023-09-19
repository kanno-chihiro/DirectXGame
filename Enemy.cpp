#include <Enemy.h>
#include "ImGuiManager.h"
#include <cassert>

Enemy::~Enemy() {
	// bullet_の解放
	for (EnemyBullet* bul9let : bullets__) {
		delete bullet__;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	input_ = Input::GetInstance();
	// NULLポインタチェック
	//初期化
	assert(model);
	model__ = model;
	textureHandle__ = textureHandle;
	worldTransform_.translation_ = {0.0f, 3.0f, 50.0f};
	worldTransform_.Initialize();
};


void Enemy::Fire(Vector3& position) {
	const int kBulletspeed =1.0;
	
		////弾があれば解放する
		// if (bullet__) {
		//	delete bullet__;
		//	bullet__ = nullptr;
		// }

		

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model__, position, velocity);

		// 弾を登録する
		// bullet_ = newBullet;
		bullets__.push_back(newBullet);
}


void Enemy::Update(){

	// 行列を定数バッファに転送

	const float kClientVelocity = 0.2f;
	Vector3 Enemymove = {0, 0, 0};

	// 回転速さ{ラジアン/frame}
	const float kEnemySpeed = 0.2f;

	

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:

		//移動(ベクトルを加算)
		worldTransform_.translation_.z -= kEnemySpeed;

		BulletTimer--;

		if (BulletTimer <= 0)
		{
			Fire(worldTransform_.translation_);
			BulletTimer = kFireinterval;
			for (EnemyBullet* bullet : bullets__)
			{
				bullet->Update();
			}
		}

		
		// 規定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Enemy::Phase::Leave;
		}
		break;
	case Enemy::Phase::Leave:

			//移動(ベクトルを加算)
		worldTransform_.translation_.x -= kEnemySpeed;
		worldTransform_.translation_.y += kEnemySpeed;
		worldTransform_.translation_.z -= kEnemySpeed;
		break;
	}


	// 行列の更新
	worldTransform_.UpdateMatrix();
};

void Enemy::Draw(ViewProjection& viewProjection){
	// 3Dモデルを描画
	model__->Draw(worldTransform_, viewProjection, textureHandle__);
};

