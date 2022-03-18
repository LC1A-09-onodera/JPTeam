#include "Enemy.h"
#include "../Player/Player.h"
#include "../Particle/Particle3D.h"
#include "../Sound/Sound.h"
#include "../WindowsAPI/WinAPI.h"
#include "../King/King.h"
#include "../Shader/ShaderManager.h"
#include "../imgui/ImguiControl.h"
#include "../BaseDirectX/Input.h"

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
		//�w�C�g�̕����ɓ����Ă���
		GoHate(king.GetPosition());
	}
	else
	{
		//�����̉e�����󂯂Ă���
		AddWindForce();
	}
	SampleAddForce();
	//�O���ɓ���������_���[�W���󂯂�
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
void EnemyBase::AddWindForce()
{
	each.position += ConvertXMFLOAT3toXMVECTOR(windDirection);
	//�����̗͂����������Ă���
	if (windDirection.x > 0.1f)
	{
		windDirection.x -= resistForce;
	}
	else if (windDirection.x < 0.1f)
	{
		windDirection.x += resistForce;
	}
	if (windDirection.z > 0.1f)
	{
		windDirection.z -= resistForce;
	}
	else if(windDirection.z < 0.1f)
	{
		windDirection.z += resistForce;
	}
	//�ݒ�l�ȉ��Ȃ�0��
	if (windDirection.x < resistMinForce && windDirection.x < -resistMinForce)
	{
		windDirection.x = 0.0f;
	}
	if (windDirection.z < resistMinForce && windDirection.z < -resistMinForce)
	{
		windDirection.z = 0.0f;
	}
	//
	if (windDirection.x == 0.0f && windDirection.z == 0.0f)
	{
		isWind = false;
	}
}
void EnemyBase::GoHate(XMFLOAT3 &hatePosition)
{
	kingDirection = hatePosition - GetPosition();
	each.position += ConvertXMFLOAT3toXMVECTOR(Normalize(kingDirection) * moveSpeed);
}
void EnemyBase::SampleAddForce()
{
	if (Input::KeyTrigger(DIK_V))
	{
		isWind = true;
		windDirection = {4.0f, 0, 0};
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
		each.position.m128_f32[2] = rand() % 49 - 24;
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
		each.position.m128_f32[2] = rand() % 49 - 24;
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
