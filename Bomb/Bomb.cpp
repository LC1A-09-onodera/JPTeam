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
	bombObject.position = data.pos;
	blastObject.position = data.pos;
}

void Bomb::Finailize()
{
}

void Bomb::Draw()
{
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

XMFLOAT3 Bomb::EnemyBombCollision(EnemyBase enemyData)
{
	//敵と当たっているかの判定
	float bombEnemyDistance = 
	//爆弾から敵に向かっての方向計算

	//敵と爆弾との距離計算

	//爆発の範囲と爆風の範囲の長さを比較

	//比率に応じて押し戻しの力の大きさを変更

	//向きと力の大きさを乗算してベクトルにして慣性
	DirectX::XMVECTOR blastVector = {};

	return XMFLOAT3();
}

void Bomb::BombUpdate()
{
	DirectX::XMVECTOR moveSpeed = (data.bombAngle * data.bombSpeed);
	data.pos += moveSpeed;
}

void Bomb::BlastUpdate()
{
}
