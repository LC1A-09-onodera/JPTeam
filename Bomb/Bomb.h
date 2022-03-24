#pragma once
#include <DirectXMath.h>
#include "../sampleObject/sampleObject.h"
#include "../Enemy/Enemy.h"

class Hole;
class Blast;
struct BombData
{
	DirectX::XMVECTOR pos = {};//���e�̍��W
	DirectX::XMVECTOR bombAngle = { -1, 0, 0 };
	float bombRadius = 1.0f;//���e���̂̑傫��
	float bombSpeed = 1.0f;//���e�̑��x
	bool isAlive = false;//���e�����ݐ����Ă��邩�ǂ���
	int bombAliveTimer = 0;
};
class Bomb
{
public:
	Bomb();
	~Bomb();
	//������
	void Init(const Model &model);
	//�X�V����
	void Update();
	//�I������
	void Finailize();
	//�`�揈��
	void Draw();

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="angle">����</param>
	/// <param name="pos"></param>
	/// <returns></returns>
	bool Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos);

	/// <summary>
	/// �G�Ƃ̓����蔻��
	/// </summary>
	/// <param name="enemyData"></param>
	bool EnemyBombCollision(EnemyBase &enemyData);


	/// <summary>
	/// ��������
	/// </summary>
	void Explosion();
public://�A�N�Z�b�T
	bool GetIsAlve() {return data.isAlive;}
	const XMVECTOR &GetPos(){return data.pos;}
	float GetRadius() { return data.bombRadius; }
private:
	//���e�������Ă���ۂ̍X�V����
	void BombUpdate();


	/// <summary>
	/// ���e�{�̂Ƃ̓����蔻��
	/// </summary>
	/// <param name="pos">������Ƃ镨�̂̍��W</param>
	/// <param name="radius">������Ƃ镨�̂̔��a</param>
	/// <param name="isHit">����������</param>
	bool BombCollision(const XMVECTOR &pos, const float &radius);

	void MeshCopy(const Model &model) { this->bombObject = model;}

private:
	BombData data;
	Model bombObject;
	EachInfo each;
};

