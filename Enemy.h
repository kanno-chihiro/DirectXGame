#pragma once
#include "Input.h"
#include "Model.h"
#include <list>
#include "Matrix/MatrixTransform.h"
#include"Vector/VectorTransform.h"

class Enemy {
public:
	~Enemy();

	//’e
	EnemyBullet* bullet_ = nullptr;
	std::list<EnemyBullet*> bullets_;

	void 

};
