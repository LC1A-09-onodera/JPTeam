#include "Bomb.h"
#include "../DX12operator.h"
#include "../Shader/ShaderManager.h"
#include "../Player/Player.h"
#include "../Hole/Hole.h"
#include "../King/King.h"
#include "Blast.h"
namespace
{
const int explosionTimerMax = 5;
const int safeTimerMax = 10;
const float baseBlastPower = 0.0f;
const int bombAliveTimerMax = 60;
}

Bomb::Bomb()
{
}

Bomb::~Bomb()
{
}

void Bomb::Init(const Model &model)
{
	MeshCopy(model);
	each.position = {};
	each.scale = {1, 1, 1};
	each.rotation = {};
	each.CreateConstBuff0();
	each.CreateConstBuff1();
}

void Bomb::Update()
{
	if (data.isAlive)
	{
		BombUpdate();
		bombObject.Update(&each);
	}
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
}

bool Bomb::Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos)
{
	//���e�������Ă���Ȃ�
	if (data.isAlive) return false;

	//���e�̏���ǉ�
	data.pos = ConvertXMFLOAT3toXMVECTOR(pos);
	data.isAlive = true;
	data.bombAngle = ConvertXMFLOAT3toXMVECTOR(angle);
	data.bombAliveTimer = 0;
	return true;
}

bool Bomb::EnemyBombCollision(EnemyBase& enemyData)
{
	if (enemyData.GetIsWind())return false;
	//�G�̍��W
	XMVECTOR enemyPosition = ConvertXMFLOAT3toXMVECTOR(enemyData.GetPosition());

	//���e�{�̂ƐڐG���Ă��邩�̔���
	bool IsBlast = BombCollision(enemyPosition, 1);

	//���e�ƐڐG���Ă����甚������
	if (IsBlast)
	{
		Explosion();
		Hole hole;
		XMFLOAT3 enemyPos = ConvertXMVECTORtoXMFLOAT3(enemyPosition);
		hole.Init(enemyPos);
		Holes::AddHole(hole);
		return true;
	}
	return false;
}


void Bomb::Explosion()
{
	data.isAlive = false;
}


void Bomb::BombUpdate()
{
	DirectX::XMVECTOR moveSpeed = (data.bombAngle * data.bombSpeed);
	data.pos += moveSpeed;

	each.position = data.pos;
	data.bombAliveTimer++;

	if (data.bombAliveTimer >= bombAliveTimerMax)
	{
		data.isAlive = false;
	}
}

bool Bomb::BombCollision(const XMVECTOR& pos, const float& radius)
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
