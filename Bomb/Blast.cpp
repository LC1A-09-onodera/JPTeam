#include "Blast.h"
#include "../DX12operator.h"
#include "../Shader/ShaderManager.h"
#include "../Player/Player.h"
#include "../Hole/Hole.h"
#include "../King/King.h"
#include "Bomb.h"


namespace
{
	const int explosionTimerMax = 180;
	const int safeTimerMax = 10;
	const float baseBlastPower = 0.0f;
	const int bombAliveTimerMax = 60;
	const float blastRadiusMax = 5.0f;
}
Blast::Blast()
{
}

Blast::~Blast()
{
}

void Blast::Init(Model *model)
{
	MeshCopy(model);
	data.blastPower = baseBlastPower;
	each.position = {};
	each.scale = { 1, 1, 1 };
	each.rotation = {};
	each.CreateConstBuff0();
	each.CreateConstBuff1();
}

void Blast::Update()
{
	if (data.isAlive)
	{
		BlastUpdate();
	}
}

void Blast::Finailize()
{
}

void Blast::Draw()
{
	if (data.isAlive)
	{
		blastObject->Update(&each);
		Draw3DObject(*blastObject);
	}
}
bool Blast::EnemyBombCollision(EnemyBase &enemyData)
{
	if (enemyData.GetIsWind())return false;
	//“G‚ÌÀ•W
	XMVECTOR enemyPosition = ConvertXMFLOAT3toXMVECTOR(enemyData.GetPosition());

	//”š’e–{‘Ì‚ÆÚG‚µ‚Ä‚¢‚é‚©‚Ì”»’è
	bool IsBlast = BlastCollision(enemyPosition, 1);

	//”š’e‚ÆÚG‚µ‚Ä‚¢‚½‚ç”š”­‚·‚é
	if (IsBlast)
	{
		//”š’e–{‘Ì‚ÆÚG‚µ‚Ä‚¢‚é‚©‚Ì”»’è
		Hole hole;
		XMFLOAT3 enemyPos = ConvertXMVECTORtoXMFLOAT3(enemyPosition);
		hole.Init(enemyPos);
		Holes::AddHole(hole);
		return true;
	}
	return false;
}

bool Blast::ChainCollision(Bomb &bomb)
{
	//“G‚ÌÀ•W
	XMVECTOR blastPosition = bomb.GetPos();

	//”š’e–{‘Ì‚ÆÚG‚µ‚Ä‚¢‚é‚©‚Ì”»’è
	bool IsBlast = BlastCollision(blastPosition, bomb.GetRadius());

	//”š’e‚ÆÚG‚µ‚Ä‚¢‚½‚ç”š”­‚·‚é
	if (IsBlast)
	{
		bomb.Explosion();
		return true;
	}
	return false;
}
float Blast::PlayerBlastCollision(XMFLOAT3 pos, float radius)
{
	XMVECTOR force;
	XMFLOAT3 tmpForce;
	bool isPlayerHit = false;
	isPlayerHit = BlastCollision(ConvertXMFLOAT3toXMVECTOR(pos), radius, &tmpForce);

	if (isPlayerHit)
	{
		force = ConvertXMFLOAT3toXMVECTOR(tmpForce);

		float power = XMVector3Length(force).m128_f32[0];
		//ƒVƒ“ƒOƒ‹ƒgƒ“‚È‚Ì‚ðˆ«—p‚µ‚Ä‚¢‚Ü‚·
		XMFLOAT3 pos = ConvertXMVECTORtoXMFLOAT3(data.pos);
		Player::GetPlayer()->HitBomb(power, pos);
		return power;
	}
	return 0.0f;
}

void Blast::BlastUpdate()
{
	data.blastTimer++;
	each.position = data.pos;

	each.scale = (XMFLOAT3{ data.blastRadius, data.blastRadius, data.blastRadius });
	if (data.blastTimer >= explosionTimerMax)
	{
		data.isAlive = false;
	}
}


bool Blast::BlastCollision(const XMVECTOR &pos, const float &radius, XMFLOAT3 *blastPower)
{
	//”š”­‚µ‚Ä‚¢‚È‚©‚Á‚½‚ç
	if (!data.isAlive)return false;

	//”š’e‚©‚ç“G‚Ü‚Å‚ÌƒxƒNƒgƒ‹‚ðŒvŽZ
	XMVECTOR distance = (pos - data.pos);
	//“G‚Æ”š’e‚Æ‚Ì‹——£ŒvŽZ
	float bombEnemyLenght = XMVector3Length(distance).m128_f32[0];

	//“G‚Æ“–‚½‚Á‚Ä‚¢‚é‚©‚Ì”»’è
	if (data.blastRadius <= bombEnemyLenght)
	{//”š•—‚ª“G‚ÆÚG‚µ‚Ä‚¢‚È‚©‚Á‚½‚ç‘¬“xƒ[ƒ‚ð•Ô‚·
		return false;
	}


	//”š’e‚©‚ç“G‚ÉŒü‚©‚Á‚Ä‚Ì•ûŒüŒvŽZ
	XMVECTOR blastAngle = XMVector3Normalize(distance);
	//”š”­‚Ì”ÍˆÍ‚Æ”š•—‚Ì”ÍˆÍ‚Ì’·‚³‚ð”äŠr
	float power = 1.0f - (bombEnemyLenght / data.blastRadius);
	//”ä—¦‚É‰ž‚¶‚Ä‰Ÿ‚µ–ß‚µ‚Ì—Í‚Ì‘å‚«‚³‚ð•ÏX
	power *= data.blastPower;
	//Œü‚«‚Æ—Í‚Ì‘å‚«‚³‚ðæŽZ‚µ‚ÄƒxƒNƒgƒ‹‚É‚µ‚ÄŠµ«
	DirectX::XMVECTOR blastVector = blastAngle * power;
	//“–‚½‚Á‚Ä‚¢‚é”»’è
	if (blastPower)
	{
		*blastPower = ConvertXMVECTORtoXMFLOAT3(blastVector);
	}
	return true;
}

void Blast::BlastEasign()
{
	if (isEase)
	{
		float max = blastRadiusMax;
		//EaseOutQuad(XMFLOAT3(0, 0, 0), XMFLOAT3(max, max, max), )
	}
}

void Blast::Spawn(XMFLOAT3 pos)
{
	data.pos = ConvertXMFLOAT3toXMVECTOR(pos);
	data.blastTimer = 0;
	data.isAlive = true;
	each.scale = XMFLOAT3{ 1.0f, 1.0f, 1.0f };
	isEase = true;
}
