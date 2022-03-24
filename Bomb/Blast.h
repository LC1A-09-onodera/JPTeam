#pragma once

#pragma once
#include <DirectXMath.h>
#include "../sampleObject/sampleObject.h"
#include "../Enemy/Enemy.h"

class Bomb;
class Hole;
struct BlastData
{
	DirectX::XMVECTOR pos = {};//���e�̍��W
	float blastRadius = 5.0f;//�����̔��a
	float blastPower = 0;//�����̋���
	int blastTimer = 0;//�����^�C�}�[(�Ƃ肠�����g��Ȃ�)
	bool isAlive = false;//�������ݐ����Ă��邩�ǂ���
};
class Blast
{
public:
	Blast();
	~Blast();
	//������
	void Init(const Model &model);
	//�X�V����
	void Update();
	//�I������
	void Finailize();
	//�`�揈��
	void Draw();

	float PlayerBlastCollision(XMFLOAT3 pos, float radius);

	/// <summary>
/// �G�Ƃ̓����蔻��
/// </summary>
/// <param name="enemyData"></param>
	bool EnemyBombCollision(EnemyBase &enemyData);

	bool ChainCollision(Bomb &bomb);

	/// <summary>
	/// ��������
	/// </summary>
	void Spawn(XMFLOAT3 pos);
public://�A�N�Z�b�T
	bool GetIsAlve() { return data.isAlive; }
	const XMVECTOR &GetPos() { return data.pos; }
	float GetRadius() {return data.blastRadius;}
private:
	//�������Ă���Ƃ��̍X�V����
	void BlastUpdate();

	/// <summary>
	/// �����Ƃ̔���
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="radius"></param>
	/// <param name="blastPower"></param>
	/// <returns></returns>
	bool BlastCollision(const XMVECTOR &pos, const float &radius, XMFLOAT3 *blastPower = nullptr);

private:
	void MeshCopy(const Model &model) { this->blastObject = model; }

	BlastData data;
	Model blastObject;
	EachInfo each;
};