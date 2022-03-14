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
	bombObject.position = data.pos;
	blastObject.position = data.pos;
}

void Bomb::Finailize()
{
}

void Bomb::Draw()
{
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

XMFLOAT3 Bomb::EnemyBombCollision(EnemyBase enemyData)
{
	//�G�Ɠ������Ă��邩�̔���
	float bombEnemyDistance = 
	//���e����G�Ɍ������Ă̕����v�Z

	//�G�Ɣ��e�Ƃ̋����v�Z

	//�����͈̔͂Ɣ����͈̔͂̒������r

	//�䗦�ɉ����ĉ����߂��̗͂̑傫����ύX

	//�����Ɨ͂̑傫������Z���ăx�N�g���ɂ��Ċ���
	DirectX::XMVECTOR blastVector = {};

	return XMFLOAT3();
}

void Bomb::BombUpdate()
{
	DirectX::XMVECTOR moveSpeed = (data.bombAngle * data.bombSpeed);
	data.pos += moveSpeed;
}

void Bomb::BlastUpdate()
{
}
