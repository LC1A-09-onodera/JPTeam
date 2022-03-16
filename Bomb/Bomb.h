#pragma once
#include <DirectXMath.h>
#include "../sampleObject/sampleObject.h"
#include "../Enemy/Enemy.h"
struct BombData
{
	DirectX::XMVECTOR pos = {};//���e�̍��W
	DirectX::XMVECTOR bombAngle = { -1, 0, 0 };
	float bombRadius = 0.0f;//���e���̂̑傫��
	float bombSpeed = 1.0f;//���e�̑��x
	float blastRadius = 10;//�����̔��a
	float blastPower = 10;//�����̋���
	int blastTimer = 30;//�����^�C�}�[(�Ƃ肠�����g��Ȃ�)
	bool isAlive = false;//���e�����ݐ����Ă��邩�ǂ���
	bool isExplosion = true;//�������������Ă��邩�ǂ���
};
class Bomb
{
public:
	Bomb();
	~Bomb();
	//������
	void Init();
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
	void EnemyBombCollision(EnemyBase *enemyData);
public:
	bool GetIsAlve() {return data.isAlive;}
	bool GetIsExplosion(){return data.isExplosion;}
private:
	//���e�������Ă���ۂ̍X�V����
	void BombUpdate();
	//�������Ă���Ƃ��̍X�V����
	void BlastUpdate();

	/// <summary>
	/// ���e�{�̂Ƃ̓����蔻��
	/// </summary>
	/// <param name="pos">������Ƃ镨�̂̍��W</param>
	/// <param name="radius">������Ƃ镨�̂̔��a</param>
	/// <param name="isHit">����������</param>
	bool BombCollision(const XMVECTOR &pos, const float &radius);

	/// <summary>
	/// �����Ƃ̔���
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="radius"></param>
	/// <param name="blastPower"></param>
	/// <returns></returns>
	bool BlastCollision(const XMVECTOR &pos, const float &radius, XMFLOAT3 *blastPower = nullptr);

	/// <summary>
	/// ��������
	/// </summary>
	void Explosion();
private:
	BombData data;
	SampleObject bombObject;
	SampleObject blastObject;
};