#include "King.h"
#include "../Shader/ShaderManager.h"
#include "../BaseDirectX/Input.h"
void King::LoadModel()
{
	king.CreateModel("King", ShaderManager::playerShader);
}

void King::SetPosition(XMFLOAT3 &position)
{
	king.each.position = ConvertXMFLOAT3toXMVECTOR(position);
}

void King::Init(int HP)
{
	this->HP = HP;
	isWind = false;
	LoadModel();
	king.each.position = {0, 0, 5};
}

void King::Update()
{
	//爆風を受けている時の処理
	if (isWind)
	{
		AddWindForce();
	}
	if (isEnemyHit)
	{
		AddEnemyForce();
	}
	if (Input::Key(DIK_D))
	{
		king.each.position.m128_f32[0] += 0.5f;
	}
	if (Input::Key(DIK_A))
	{
		king.each.position.m128_f32[0] -= 0.5f;
	}
	if (Input::Key(DIK_W))
	{
		king.each.position.m128_f32[2] += 0.5f;
	}
	if (Input::Key(DIK_S))
	{
		king.each.position.m128_f32[2] -= 0.5f;
	}
}

void King::Draw()
{
	king.Update();
	Draw3DObject(king);
}

void King::AddWindForce()
{
	if (!isWind) return;
	king.each.position += ConvertXMFLOAT3toXMVECTOR(windDirection);
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

void King::AddEnemyForce()
{
	if (!isEnemyHit) return;
	king.each.position += ConvertXMFLOAT3toXMVECTOR(enemyDirection);
	//爆風の力を減衰させていく
	if (enemyDirection.x > 0.1f)
	{
		enemyDirection.x -= resistForce;
	}
	else if (enemyDirection.x < 0.1f)
	{
		enemyDirection.x += resistForce;
	}
	if (enemyDirection.z > 0.1f)
	{
		enemyDirection.z -= resistForce;
	}
	else if (enemyDirection.z < 0.1f)
	{
		enemyDirection.z += resistForce;
	}
	//設定値以下なら0に
	if (enemyDirection.x < resistMinForce && enemyDirection.x > -resistMinForce)
	{
		enemyDirection.x = 0.0f;
	}
	if (enemyDirection.z < resistMinForce && enemyDirection.z > -resistMinForce)
	{
		enemyDirection.z = 0.0f;
	}
	//
	if (enemyDirection.x == 0.0f && enemyDirection.z == 0.0f)
	{
		isEnemyHit = false;
	}
}
