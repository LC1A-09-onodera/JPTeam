#include "Bomb.h"
#include "../DX12operator.h"
#include "../Shader/ShaderManager.h"

Bomb::Bomb()
{
}

Bomb::~Bomb()
{
}

void Bomb::Init()
{
	bombObject.CreateModel("Block", ShaderManager::playerShader);
	blastObject.CreateModel("Block", ShaderManager::playerShader);
}

void Bomb::Update()
{
	if (data.isAlive)
	{
		BombUpdate();
	}
	if (data.isBlast)
	{
		BlastUpdate();
	}
}

void Bomb::Finailize()
{
}

void Bomb::Draw()
{
	Draw3DObject(bombObject);
	Draw3DObject(blastObject);
}

bool Bomb::Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos)
{
	//���e�������Ă���Ȃ�
	if (data.isAlive) return false;

	//���e�̏���ǉ�
	data.pos = ConvertXMFLOAT3toXMVECTOR(angle);
	data.isAlive = true;
	data.bombAngle = ConvertXMFLOAT3toXMVECTOR(angle);
	return true;
}

void Bomb::EnemyBombCollision(EnemyBase *enemyData)
{
	//�G�̍��W
	XMVECTOR enemyPosition = enemyData->GetModel().position;

	//���e�{�̂ƐڐG���Ă��邩�̔���
	bool IsBlast = BombCollision(enemyPosition, 0);

	//���e�ƐڐG���Ă����甚������
	if (IsBlast)
	{
		data.isAlive = false;
		data.isBlast = true;
	}


	//�����̃x�N�g��
	XMFLOAT3 blastPower;

	//�����ɓ����������t���O
	bool IsBlastHit = BlastCollision(enemyPosition, 0, &blastPower);


	//�������������e�f�[�^�ɓ����
	if (IsBlastHit)
	{
		enemyData->SetIsWind(IsBlastHit);
		enemyData->SetWindDirection(blastPower);
	}
}

void Bomb::BombUpdate()
{
	DirectX::XMVECTOR moveSpeed = (data.bombAngle * data.bombSpeed);
	data.pos += moveSpeed;

	bombObject.position = data.pos;
	bombObject.active = data.isAlive;
	bombObject.Update();
}

void Bomb::BlastUpdate()
{
	blastObject.position = data.pos;
	blastObject.active = data.isBlast;
	blastObject.Update();
}

bool Bomb::BombCollision(const XMVECTOR &pos, const float &radius)
{
	//���e�����Ă����������瑁�����^�[��
	if (!data.isAlive)return;

	//���e����G�܂ł̃x�N�g�����v�Z
	XMVECTOR distance = (pos - data.pos);
	//�G�Ɣ��e�Ƃ̋����v�Z
	float bombEnemyLenght = XMVector3Length(distance).m128_f32[0];

	//�G�Ɣ��e���������Ă��邩�������Ă��邩�̔���
	if ((data.bombRadius + radius) <= bombEnemyLenght)
	{//���e���ڐG���Ă��Ȃ�����
		return false;
	}
	else
	{
		return true;
	}
}

bool Bomb::BlastCollision(const XMVECTOR &pos, const float &radius, XMFLOAT3 *blastPower)
{
	//�������Ă��Ȃ�������
	if (!data.isBlast)return false;

	//���e����G�܂ł̃x�N�g�����v�Z
	XMVECTOR distance = (pos - data.pos);
	//�G�Ɣ��e�Ƃ̋����v�Z
	float bombEnemyLenght = XMVector3Length(distance).m128_f32[0];

	//�G�Ɠ������Ă��邩�̔���
	if (data.blastRadius <= bombEnemyLenght)
	{//�������G�ƐڐG���Ă��Ȃ������瑬�x�[����Ԃ�
		return false;
	}


	//���e����G�Ɍ������Ă̕����v�Z
	XMVECTOR blastAngle = XMVector3Normalize(distance);
	//�����͈̔͂Ɣ����͈̔͂̒������r
	float power = 1.0f - (bombEnemyLenght / data.blastRadius);
	//�䗦�ɉ����ĉ����߂��̗͂̑傫����ύX
	power *= data.blastPower;
	//�����Ɨ͂̑傫������Z���ăx�N�g���ɂ��Ċ���
	DirectX::XMVECTOR blastVector = blastAngle * power;
	//�������Ă��锻��
	if (blastPower)
	{
		*blastPower = ConvertXMVECTORtoXMFLOAT3(blastVector);
	}
	return true;
}
