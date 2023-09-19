#include "MatrixTrans.h"

class EnemyBullet {
public:
	// ���x
	Vector3 velocity_ = {0, 0};

	//������
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	
	//�X�V
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);

	private:

	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	// ���f��
	Model* model__ = nullptr;

	// �e�N�X�`���n���h��
	uint32_t textureHandle__ = 0u;
};
