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

}
Blast::Blast()
{
}

Blast::~Blast()
{
}

void Blast::Init(const Model &model)
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

	blastObject.Update(&each);
}

void Blast::Finailize()
{
}

void Blast::Draw()
{
	if (data.isAlive)
	{
		Draw3DObject(blastObject);
	}
}
bool Blast::EnemyBombCollision(EnemyBase &enemyData)
{
	if (enemyData.GetIsWind())return false;
	//敵の座標
	XMVECTOR enemyPosition = ConvertXMFLOAT3toXMVECTOR(enemyData.GetPosition());

	//爆弾本体と接触しているかの判定
	bool IsBlast = BlastCollision(enemyPosition, 1);

	//爆弾と接触していたら爆発する
	if (IsBlast)
	{
		enemyData.SetIsDead(true);

		//爆弾本体と接触しているかの判定
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
	//敵の座標
	XMVECTOR blastPosition = bomb.GetPos();

	//爆弾本体と接触しているかの判定
	bool IsBlast = BlastCollision(blastPosition, bomb.GetRadius());

	//爆弾と接触していたら爆発する
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
		//シングルトンなのを悪用しています
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
	//爆発していなかったら
	if (!data.isAlive)return false;

	//爆弾から敵までのベクトルを計算
	XMVECTOR distance = (pos - data.pos);
	//敵と爆弾との距離計算
	float bombEnemyLenght = XMVector3Length(distance).m128_f32[0];

	//敵と当たっているかの判定
	if (data.blastRadius <= bombEnemyLenght)
	{//爆風が敵と接触していなかったら速度ゼロを返す
		return false;
	}


	//爆弾から敵に向かっての方向計算
	XMVECTOR blastAngle = XMVector3Normalize(distance);
	//爆発の範囲と爆風の範囲の長さを比較
	float power = 1.0f - (bombEnemyLenght / data.blastRadius);
	//比率に応じて押し戻しの力の大きさを変更
	power *= data.blastPower;
	//向きと力の大きさを乗算してベクトルにして慣性
	DirectX::XMVECTOR blastVector = blastAngle * power;
	//当たっている判定
	if (blastPower)
	{
		*blastPower = ConvertXMVECTORtoXMFLOAT3(blastVector);
	}
	return true;
}

void Blast::Spawn(XMFLOAT3 pos)
{
	data.pos = ConvertXMFLOAT3toXMVECTOR(pos);
	data.blastTimer = 0;
	data.isAlive = true;
	blastObject.each.scale = XMFLOAT3{ 1.0f, 1.0f, 1.0f };
}
