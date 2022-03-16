#pragma once
#include "../3DModel/Model.h"
#include <DirectXMath.h>

using namespace DirectX;
class King;

enum EnemyType
{
	NONE,
};

class EnemyModel : public Model
{

};

class EnemyBase
{
private:
	//
	EnemyModel enemy;
	EnemyType type;
	int hp;
	//����ł���true
	bool isDead;
	//�����󂯂���true
	bool isWind;
	//�������x
	float moveSpeed;
	//�L���O�̕���
	XMFLOAT3 kingDirection;
	//�����̉����o��Power
	XMFLOAT3 windDirection;
public:
	EnemyModel SetModel(EnemyModel model) { enemy = model; }
	void SetHP(int HP) { this->hp = HP; }
	void SetIsDead(bool isDead) { this->isDead = isDead; }
	void SetIsWind(bool isWind) { this->isWind = isWind; }
	void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	void SetKingDirection(XMFLOAT3 direction) { this->kingDirection = direction; }
	void SetWindDirection(XMFLOAT3 windDirection) { this->windDirection = windDirection; }
	int GetHP() { return hp; }
	bool GetIsDead() { return isDead; }
	bool GetIsWind() { return isWind; }
	float GetIsMoveSpeed() { return moveSpeed; }
	XMFLOAT3 GetKingDirection() { return kingDirection; }
	XMFLOAT3 GetWindDirection() { return windDirection; }
	EnemyModel GetModel() { return enemy; }

	void Init();
	void Init(XMFLOAT3 position = { 0, 0, 0 }, EnemyType enemyType = EnemyType::NONE, int hp = 10, float moveSpeed = 2);
	void Update();
	void Draw();

	void SetRandomPosition();
	void SetAlive(int hp = 10, float moveSpeed = 1.0f);
};