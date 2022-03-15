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
}

void Bomb::Finailize()
{
}

void Bomb::Draw()
{
	Draw3DObject(bombObject);
	Draw3DObject(blastObject);
}

bool Bomb::Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos)
{
	//爆弾が生きているなら
	if (data.isAlive) return false;

	//爆弾の情報を追加
	data.pos = ConvertXMFLOAT3toXMVECTOR(angle);
	data.isAlive = true;
	data.bombAngle = ConvertXMFLOAT3toXMVECTOR(angle);
	return true;
}

void Bomb::EnemyBombCollision(EnemyBase *enemyData)
{
	//敵の座標
	XMVECTOR enemyPosition = enemyData->GetModel().position;

	//爆弾本体と接触しているかの判定
	bool IsBlast = BombCollision(enemyPosition, 0);

	//爆弾と接触していたら爆発する
	if (IsBlast)
	{
		data.isAlive = false;
		data.isBlast = true;
	}


	//爆風のベクトル
	XMFLOAT3 blastPower;

	//爆風に当たったかフラグ
	bool IsBlastHit = BlastCollision(enemyPosition, 0, &blastPower);


	//当たった情報を各データに入れる
	if (IsBlastHit)
	{
		enemyData->SetIsWind(IsBlastHit);
		enemyData->SetWindDirection(blastPower);
	}
}

void Bomb::BombUpdate()
{
	DirectX::XMVECTOR moveSpeed = (data.bombAngle * data.bombSpeed);
	data.pos += moveSpeed;

	bombObject.position = data.pos;
	bombObject.active = data.isAlive;
	bombObject.Update();
}

void Bomb::BlastUpdate()
{
	blastObject.position = data.pos;
	blastObject.active = data.isBlast;
	blastObject.Update();
}

bool Bomb::BombCollision(const XMVECTOR &pos, const float &radius)
{
	//爆弾がしてい無かったら早期リターン
	if (!data.isAlive)return;

	//爆弾から敵までのベクトルを計算
	XMVECTOR distance = (pos - data.pos);
	//敵と爆弾との距離計算
	float bombEnemyLenght = XMVector3Length(distance).m128_f32[0];

	//敵と爆弾が当たっているか当たっているかの判定
	if ((data.bombRadius + radius) <= bombEnemyLenght)
	{//爆弾が接触していなかった
		return false;
	}
	else
	{
		return true;
	}
}

bool Bomb::BlastCollision(const XMVECTOR &pos, const float &radius, XMFLOAT3 *blastPower)
{
	//爆発していなかったら
	if (!data.isBlast)return false;

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
