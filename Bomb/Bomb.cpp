#include "Bomb.h"
#include "../DX12operator.h"
#include "../Shader/ShaderManager.h"

namespace
{
	const int explosionTimerMax = 30;
}
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
	if (data.isExplosion)
	{
		BlastUpdate();
	}

	bombObject.Update();
	blastObject.Update();
}

void Bomb::Finailize()
{
}

void Bomb::Draw()
{
	if (data.isAlive)
	{
		Draw3DObject(bombObject);
	}
	if (data.isExplosion)
	{
		Draw3DObject(blastObject);
	}
}

bool Bomb::Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos)
{
	//���e�������Ă���Ȃ�
	if (data.isAlive) return false;

	//���e�̏���ǉ�
	data.pos = ConvertXMFLOAT3toXMVECTOR(pos);
	data.isAlive = true;
	data.bombAngle = ConvertXMFLOAT3toXMVECTOR(angle);
	return true;
}

void Bomb::EnemyBombCollision(EnemyBase &enemyData)
{
	//�G�̍��W
	XMVECTOR enemyPosition = ConvertXMFLOAT3toXMVECTOR(enemyData.GetPosition());

	//���e�{�̂ƐڐG���Ă��邩�̔���
	bool IsBlast = BombCollision(enemyPosition, 1);

	//���e�ƐڐG���Ă����甚������
	if (IsBlast)
	{
	//���e�{�̂ƐڐG���Ă��邩�̔���
		BombCollision(enemyPosition, 0);
		Explosion();
	}


	//�����̃x�N�g��
	XMFLOAT3 blastPower;

	//�����ɓ����������t���O
	bool IsBlastHit = BlastCollision(enemyPosition, 0, &blastPower);


	//�������������e�f�[�^�ɓ����
	if (IsBlastHit)
	{
		enemyData.SetIsWind(IsBlastHit);
		enemyData.SetWindDirection(blastPower);
	}
}



void Bomb::BombUpdate()
{
	DirectX::XMVECTOR moveSpeed = (data.bombAngle * data.bombSpeed);
	data.pos += moveSpeed;

	bombObject.each.position = data.pos;
}

void Bomb::BlastUpdate()
{
	data.blastTimer++;
	blastObject.each.position = data.pos;

	blastObject.each.scale = (blastObject.each.scale + XMFLOAT3{ 1.0f, 1.0f, 1.0f });
	if (data.blastTimer >= explosionTimerMax)
	{
		data.isExplosion = false;
	}
}

bool Bomb::BombCollision(const XMVECTOR &pos, const float &radius)
{
	//���e���݂����Ă����������瑁�����^�[��
	if (!data.isAlive)return false;

	//���e����G�܂ł̃x�N�g�����v�Z
	XMVECTOR distance = (pos - data.pos);
	//�G�Ɣ��e�Ƃ̋����v�Z
	float bombEnemyLenght = XMVector3Length(distance).m128_f32[0];

	if (bombEnemyLenght <= 0)
	{
		return false;
	}
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
	if (!data.isExplosion)return false;

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

void Bomb::Explosion()
{
	data.blastTimer = 0;
	data.isAlive = false;
	data.isExplosion = true;
}
