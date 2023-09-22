#include <Enemy.h>
#include "MatrixTrans.h"
#include <cassert>

Enemy::~Enemy() {
	// bullet_の解放
	for (EnemyBullet* Enemybullet : Enemybullets_)
	{
		delete Enemybullet_;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	
	// NULLポインタチェック
	 assert(model);
	
	
	model_ = model;
	EnemytextureHandle_ = textureHandle;
	worldTransform_.Initialize();

	worldTransform_.translation_.z = 60.0f;
	worldTransform_.translation_.x = 30.0f;

	phase_ = Phase::Approach;

	Phase_();

};


void Enemy::Fire(Vector3& position) {
	
		////弾があれば解放する
		// if (bullet__) {
		//	delete bullet__;
		//	bullet__ = nullptr;
		// }

		// 弾の速度
		const float kBulletSpeed = -2.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, position, velocity);

		// 弾を登録する
		// bullet_ = newBullet;
		Enemybullets_.push_back(newBullet);
}
//接近フェ-ズ
void Enemy::Phase_()
{
	//発射タイマーを初期化
	    BulletTimer = kFireinterval;
}

void Enemy::Update(){

	// 行列を定数バッファに転送

	Vector3 move = {0.0, 0.0, 0.0f};

	const float kCharacterSpeed = 0.2f;
	

	switch (phase_) {
	case Phase::Approach:
	default:
		// 移動(ベクトルを加算)
		move.z -= kCharacterSpeed;

		//発射タイマーのカウントダウン
		BulletTimer--;


		if (BulletTimer <= 0)
		{
			//弾の発射
			Fire(worldTransform_.translation_);

			
			for (EnemyBullet* bullet : Enemybullets_)
			{
				bullet->Update();
			}
			//発射タイマーを初期化
			BulletTimer = kFireinterval;
		}

		// 規定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:

			//移動(ベクトルを加算)
		move.x -= kCharacterSpeed;
		move.y += kCharacterSpeed;
		move.z -= kCharacterSpeed;
		break;
	}
	//デスフラグガ立った弾を削除
	Enemybullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//敵弾発射
	for (EnemyBullet* bullet : Enemybullets_)
	{
		bullet->Update();
	}

	// 足し算(
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	
	worldTransform_.TransferMatrix();
};

void Enemy::Draw(ViewProjection& viewProjection){
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, EnemytextureHandle_);

	//弾の描画
	for (EnemyBullet* Enemybullet : Enemybullets_)
	{
		Enemybullet->Draw(viewProjection);
	}
	if (Enemybullet_)
	{
		Enemybullet_->Draw(viewProjection);
	}

};

