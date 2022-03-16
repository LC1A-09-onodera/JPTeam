#include "Enemy.h"
#include "../Player/Player.h"
#include "../Particle/Particle3D.h"
#include "../Sound/Sound.h"
#include "../WindowsAPI/WinAPI.h"
#include "../King/King.h"
#include "../Shader/ShaderManager.h"

EnemyModel EnemyModels::baseEnemy;
EnemyModel EnemyModels::superEnemy;

list<EnemyBase> Enemys::enemys;
void EnemyBase::Init()
{
	//�G���G��Mesh���R�s�[
	SetMesh(EnemyModels::baseEnemy);
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	SetAlive();
}
void EnemyBase::Update(King& king)
{
	//�������󂯂Ă��Ȃ�
	if (!isWind)
	{
		kingDirection = king.GetPosition() - GetPosition();
		each.position += ConvertXMFLOAT3toXMVECTOR(Normalize(kingDirection) * moveSpeed);

	}
	else
	{

	}
}
void EnemyBase::Draw()
{
	enemy.Update(&each);
	Draw3DObject(enemy);
}
void EnemyBase::SetAlive()
{
	//�|�W�V�����������_���ɐ���
	SetRandomPosition();
	//�ȉ��������ɃZ�b�g���郂�m
	this->hp = MaxHP;
	this->moveSpeed = MaxMoveSpeed;
	this->weight = MaxWeight;
	isDead = false;
	isWind = false;
	kingDirection = { 0.1f, 0, 0 };
	windDirection = { 0, 0, 0 };
}
void EnemyBase::UpdateKingDirection(XMFLOAT3& kingPos)
{
	//�L���O�������擾���Ă���
	XMFLOAT3 result;
	result = kingPos - GetPosition();
	kingDirection = Normalize(result);
}
void EnemyBase::SetRandomPosition()
{
	//���E����o�Ă���
	if (rand() % 2 == 0)
	{
		if (rand() % 2)
		{
			each.position.m128_f32[0] = rand() % 10 + 30;
		}
		else
		{
			each.position.m128_f32[0] = -(rand() % 10) - 30;
		}
		//�㉺�̒���
		each.position.m128_f32[2] = rand() % 30 - 15;
	}
}

void Enemys::AddEnemy(EnemyType type)
{
	if (type == EnemyType::NONE)
	{
		//�G���G
		EnemyBase enemy;
		enemy.Init();
		enemys.push_back(enemy);
	}
	else
	{
		//�G���G
		SuperEnemy enemy;
		enemy.Init();
		enemys.push_back(enemy);
	}
}

void Enemys::Update(King& king)
{
	auto itr = enemys.begin();
	for (; itr != enemys.end(); ++itr)
	{
		itr->Update(king);
	}
}

void Enemys::Draw()
{
	auto itr = enemys.begin();
	for (; itr != enemys.end(); ++itr)
	{
		itr->Draw();
	}
}

void EnemyModels::LoadModels()
{
	baseEnemy.CreateModel("maru", ShaderManager::playerShader);
	superEnemy.CreateModel("Block", ShaderManager::playerShader);
}

void SuperEnemy::Init()
{
	SetMesh(EnemyModels::superEnemy);
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	SetAlive();
}

void SuperEnemy::SetAlive()
{
	//
	SetRandomPosition();
	this->hp = MaxHP;
	this->moveSpeed = MaxMoveSpeed;
	this->weight = MaxWeight;
	isDead = false;
	isWind = false;
	kingDirection = { 0.1f, 0, 0 };
	windDirection = { 0, 0, 0 };
}
