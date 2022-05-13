#include "Enemy.h"
#include "../Player/Player.h"
#include "../Particle/Particle3D.h"
#include "../Sound/Sound.h"
#include "../WindowsAPI/WinAPI.h"
#include "../King/King.h"
#include "../Shader/ShaderManager.h"
#include "../imgui/ImguiControl.h"
#include "../BaseDirectX/Input.h"
#include "../Hole/Hole.h"

EnemyModel EnemyModels::baseEnemy;
EnemyModel EnemyModels::superEnemy;

list<EnemyBase> Enemys::enemys;
list<list<EnemyBase>::iterator> Enemys::deleteEnemys;

void EnemyBase::Init(const EnemyMoveDirection &direction)
{
	//雑魚敵のMeshをコピー
	SetMesh(EnemyModels::baseEnemy);
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	SetAlive(direction);
}
void EnemyBase::Update(XMFLOAT3& hate)
{
	//爆風を受けていない
	if (!isWind)
	{
		if (moveDireciotnEnum != EnemyMoveDirection::HATE)
		{
			GoMoveDirection();
		}
		else
		{
			//ヘイトの方向に動いていく
			GoHate(hate);
		}
	}
	else
	{
		//爆風の影響を受けている
		AddWindForce();
	}
	//CheckHitKing(king);
	//SampleAddForce();
	//外側に当たったらダメージを受ける
	//HitDethLine();
	//穴に当たる
	//HolesHit();
}
void EnemyBase::Draw()
{
	enemy.Update(&each);
	Draw3DObject(enemy);
}
void EnemyBase::SetAlive(const EnemyMoveDirection &direction)
{
	//ポジションをランダムに生成
	SetRandomPosition();
	//以下復活時にセットするモノ
	this->hp = MaxHP;
	this->moveSpeed = MaxMoveSpeed;
	this->weight = MaxWeight;
	isDead = false;
	isWind = false;
	if (direction == EnemyMoveDirection::LEFT)
	{
		moveDirection = { 0.1f, 0, 0 };
	}
	else if (direction == EnemyMoveDirection::RIGHT)
	{
		moveDirection = {-0.1f, 0, 0 };
	}
	else if (direction == EnemyMoveDirection::UP)
	{
		moveDirection = {0, 0, 0.1f};
	}
	else if (direction == EnemyMoveDirection::BOTTOM)
	{
		moveDirection = {0, 0, -0.1f};
	}
	else
	{
		moveDirection = {0, 0, 0};
	}
	moveDireciotnEnum = direction;
	windDirection = { 0, 0, 0 };
}
void EnemyBase::UpdateKingDirection(XMFLOAT3& kingPos)
{
	//キング方向を取得していく
	XMFLOAT3 result;
	result = kingPos - GetPosition();
	moveDirection = Normalize(result);
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
	//爆風の力を減衰させていく
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
	else if (windDirection.z < 0.1f)
	{
		windDirection.z += resistForce;
	}
	//設定値以下なら0に
	if (windDirection.x < resistMinForce && windDirection.x > -resistMinForce)
	{
		windDirection.x = 0.0f;
	}
	if (windDirection.z < resistMinForce && windDirection.z > -resistMinForce)
	{
		windDirection.z = 0.0f;
	}
	//
	if (windDirection.x == 0.0f && windDirection.z == 0.0f)
	{
		isWind = false;
	}
}
void EnemyBase::GoHate(XMFLOAT3& hatePosition)
{
	moveDirection = hatePosition - GetPosition();
	each.position += ConvertXMFLOAT3toXMVECTOR(Normalize(moveDirection) * moveSpeed);
}
void EnemyBase::GoMoveDirection()
{
	each.position += ConvertXMFLOAT3toXMVECTOR(Normalize(moveDirection) * moveSpeed);
}
void EnemyBase::SampleAddForce()
{
	if (Input::KeyTrigger(DIK_V))
	{
		isWind = true;
		windDirection = { 4.0f, 0, 0 };
	}
}
bool EnemyBase::IsHolesHit(XMFLOAT3& hole)
{
	if (HolesLenght(hole) <= 2.0f)
	{
		return true;
	}
	return false;
}
float EnemyBase::HolesLenght(XMFLOAT3 &hole)
{
	return Lenght(hole,ConvertXMVECTORtoXMFLOAT3(each.position));
}
void EnemyBase::HolesHit()
{
	if (Holes::GetHoleList().size() <= 0) return;
	auto itr = Holes::holes.begin();
	for (; itr != Holes::holes.end(); ++itr)
	{
		if (IsHolesHit(itr->GetPosition()))
		{
			hp--;
		}
	}
}
void EnemyBase::CheckHitKing(King& king)
{
	if (king.GetIsEnemyhit())return;
	if (!IsKingHit(king.GetPosition())) return;
	king.SetIsEnemyHit(true);
	XMFLOAT3 kingPos = king.GetPosition();
	XMFLOAT3 vec = kingPos - GetPosition();
	const float enemyForce = 5.0f;
	XMFLOAT3 result = Normalize(vec) * enemyForce;
	king.SetEnemyDirection(result);
}
bool EnemyBase::IsKingHit(XMFLOAT3& kingPos)
{
	if (KingToLenght(kingPos) > 2.0f)return false;
	return true;
}
float EnemyBase::KingToLenght(XMFLOAT3 &kingPos)
{
	float leng;
	leng = Lenght(kingPos, ConvertXMVECTORtoXMFLOAT3(each.position));
	return leng;
}
void EnemyBase::SetRandomPosition()
{
	//左右から出てくる
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
		//上下の調整
		each.position.m128_f32[2] = rand() % 49 - 24;
	}
	else
	{
		if (rand() % 2)
		{
			each.position.m128_f32[2] = 10 + 15;
		}
		else
		{
			each.position.m128_f32[2] = -10 - 15;
		}
		//上下の調整
		each.position.m128_f32[0] = rand() % 49 - 24;
	}
}

void Enemys::AddEnemy(EnemyType type, const EnemyMoveDirection &direction)
{
	if (type == EnemyType::NONE)
	{
		//雑魚敵
		EnemyBase enemy;
		enemy.Init(direction);
		enemys.push_back(enemy);
	}
	else
	{
		//雑魚敵
		SuperEnemy enemy;
		enemy.Init(direction);
		enemys.push_back(enemy);
	}
}

void Enemys::DeathEnemy(EnemyBase& enemy)
{

}

void Enemys::Update(XMFLOAT3& hate)
{
	auto itr = enemys.begin();
	for (; itr != enemys.end(); ++itr)
	{
		itr->Update(hate);
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

void SuperEnemy::Init(const EnemyMoveDirection &direction)
{
	SetMesh(EnemyModels::superEnemy);
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	SetAlive(direction);
}

void SuperEnemy::SetAlive(const EnemyMoveDirection& direction)
{
	//
	SetRandomPosition();
	this->hp = MaxHP;
	this->moveSpeed = MaxMoveSpeed;
	this->weight = MaxWeight;
	isDead = false;
	isWind = false;
	if (direction == EnemyMoveDirection::LEFT)
	{
		moveDirection = { 0.1f, 0, 0 };
	}
	else if (direction == EnemyMoveDirection::RIGHT)
	{
		moveDirection = { -0.1f, 0, 0 };
	}
	else if (direction == EnemyMoveDirection::UP)
	{
		moveDirection = { 0, 0, 0.1f };
	}
	else if (direction == EnemyMoveDirection::BOTTOM)
	{
		moveDirection = { 0, 0, -0.1f };
	}
	else
	{
		moveDirection = { 0, 0, 0 };
	}
	moveDireciotnEnum = direction;
	windDirection = { 0, 0, 0 };
}
