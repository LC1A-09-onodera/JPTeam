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
	//”š’e‚ª¶‚«‚Ä‚¢‚é‚È‚ç
	if (data.isAlive) return false;

	//”š’e‚Ìî•ñ‚ğ’Ç‰Á
	data.pos = ConvertXMFLOAT3toXMVECTOR(pos);
	data.isAlive = true;
	data.bombAngle = ConvertXMFLOAT3toXMVECTOR(angle);
	data.bombAliveTimer = 0;
	return true;
}

bool Bomb::EnemyBombCollision(EnemyBase& enemyData)
{
	if (enemyData.GetIsWind())return false;
	//“G‚ÌÀ•W
	XMVECTOR enemyPosition = ConvertXMFLOAT3toXMVECTOR(enemyData.GetPosition());

	//”š’e–{‘Ì‚ÆÚG‚µ‚Ä‚¢‚é‚©‚Ì”»’è
	bool IsBlast = BombCollision(enemyPosition, 1);

	//”š’e‚ÆÚG‚µ‚Ä‚¢‚½‚ç”š”­‚·‚é
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
	//”š’e‘¶İ‚ª‚µ‚Ä‚¢–³‚©‚Á‚½‚ç‘ŠúƒŠƒ^[ƒ“
	if (!data.isAlive)return false;

	//”š’e‚©‚ç“G‚Ü‚Å‚ÌƒxƒNƒgƒ‹‚ğŒvZ
	XMVECTOR distance = (pos - data.pos);
	//“G‚Æ”š’e‚Æ‚Ì‹——£ŒvZ
	float bombEnemyLenght = XMVector3Length(distance).m128_f32[0];

	if (bombEnemyLenght <= 0)
	{
		return false;
	}
	//“G‚Æ”š’e‚ª“–‚½‚Á‚Ä‚¢‚é‚©“–‚½‚Á‚Ä‚¢‚é‚©‚Ì”»’è
	if ((data.bombRadius + radius) <= bombEnemyLenght)
	{//”š’e‚ªÚG‚µ‚Ä‚¢‚È‚©‚Á‚½
		return false;
	}
	else
	{
		return true;
	}
}
