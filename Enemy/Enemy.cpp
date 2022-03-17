#include "Enemy.h"
#include "../Player/Player.h"
#include "../Particle/Particle3D.h"
#include "../Sound/Sound.h"
#include "../WindowsAPI/WinAPI.h"
#include "../King/King.h"
#include "../Shader/ShaderManager.h"
#include "../imgui/ImguiControl.h"

EnemyModel EnemyModels::baseEnemy;
EnemyModel EnemyModels::superEnemy;

list<EnemyBase> Enemys::enemys;
list<list<EnemyBase>::iterator> Enemys::deleteEnemys;
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
		//�����̉e�����󂯂Ă���
		each.position += ConvertXMFLOAT3toXMVECTOR(windDirection);
		if (windDirection.x > 0.5f && windDirection.x < -0.5f)
		{
			windDirection.x = windDirection.x * 0.5f;
		}
		else
		{
			windDirection.x = 0.0f;
		}
		if (windDirection.z > 0.5f && windDirection.z < -0.5f)
		{
			windDirection.z = windDirection.z * 0.5f;
		}
		else
		{
			windDirection.z = 0.0f;
		}
		if (windDirection.x == 0.0f && windDirection.z == 0.0f)
		{
			isWind = false;
		}
	}

	HitDethLine();
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
void EnemyBase::HitDethLine()
{
	bool resultR = each.position.m128_f32[0] > Imgui::dethLine;
	bool resultL = each.position.m128_f32[0] < -Imgui::dethLine;
	bool resultU = each.position.m128_f32[2] > Imgui::dethLine;
	bool resultB = each.position.m128_f32[2] < -Imgui::dethLine;
	if (resultR || resultL || resultU || resultB)
	{
		hp -= 1;
	}
}
void EnemyBase::SetRandomPosition()
{
	//���E����o�Ă���
	if (rand() % 2 == 0)
	{
		if (rand() % 2)
		{
			each.position.m128_f32[0] = 10 + 15;
		}
		else
		{
			each.position.m128_f32[0] = -10 - 15;
		}
		//�㉺�̒���
		each.position.m128_f32[2] = 25;
	}
	else
	{
		if (rand() % 2)
		{
			each.position.m128_f32[0] = 10 + 15;
		}
		else
		{
			each.position.m128_f32[0] = -10 - 15;
		}
		//�㉺�̒���
		each.position.m128_f32[2] = 25;
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

void Enemys::DeathEnemy(EnemyBase& enemy)
{
	
}

void Enemys::Update(King& king)
{
	auto itr = enemys.begin();
	for (; itr != enemys.end(); ++itr)
	{
		itr->Update(king);
		if (itr->GetHP() <= 0)
		{
			//enemys.erase(itr);
			deleteEnemys.push_back(itr);
		}
	}
	auto deleteItr = deleteEnemys.begin();
	for (; deleteItr != deleteEnemys.end(); ++deleteItr)
	{
		enemys.erase(*deleteItr);
	}
	deleteEnemys.clear();
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
