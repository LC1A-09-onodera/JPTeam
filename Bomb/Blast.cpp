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
	//�G�̍��W
	XMVECTOR enemyPosition = ConvertXMFLOAT3toXMVECTOR(enemyData.GetPosition());

	//���e�{�̂ƐڐG���Ă��邩�̔���
	bool IsBlast = BlastCollision(enemyPosition, 1);

	//���e�ƐڐG���Ă����甚������
	if (IsBlast)
	{
		//���e�{�̂ƐڐG���Ă��邩�̔���
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
	//�G�̍��W
	XMVECTOR blastPosition = bomb.GetPos();

	//���e�{�̂ƐڐG���Ă��邩�̔���
	bool IsBlast = BlastCollision(blastPosition, bomb.GetRadius());

	//���e�ƐڐG���Ă����甚������
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
		//�V���O���g���Ȃ̂����p���Ă��܂�
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
	//�������Ă��Ȃ�������
	if (!data.isAlive)return false;

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
