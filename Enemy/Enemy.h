#pragma once
#include "../3DModel/Model.h"
#include <DirectXMath.h>
#include "../DX12operator.h"

using namespace DirectX;
class King;

enum class EnemyType
{
	NONE,
	SUPER,
};

enum class Target
{
	KING,
	PLAYER,
	CROWN,
};

class EnemyModel : public Model
{

};

class EnemyModels
{
public:
	static EnemyModel baseEnemy;
	static EnemyModel superEnemy;
	static void LoadModels();
};

class EnemyBase
{
//private:
protected:
	//
	EnemyModel enemy;
	//�C�[�W���O�X�^�[�g�ʒu
	XMFLOAT3 startPos;
	//�C�[�W���O�̏I���ʒu
	XMFLOAT3 endPos;
	//�C�[�W���O�^�C��
	float easeTime;
	//�C�[�W���O�^�C���̑�����
	float easeAddTime;
	//
	EachInfo each;

	EnemyType type;
	int hp;
	//����ł���true
	bool isDead;
	//�����󂯂���true
	bool isWind;
	//�������x
	float moveSpeed;
	//
	float weight;
	//�L���O�̕���
	XMFLOAT3 kingDirection;
	//�����̉����o��Power
	XMFLOAT3 windDirection;
	//�v���C���[�Ƃ̉����o��
	XMFLOAT3 playerPushDirection;
	//
	Target target;
	const int MaxHP = 1;
	const float MaxMoveSpeed = 0.1f;
	const float MaxWeight = 1.0f;
public:
	void SetMesh(EnemyModel &model) { enemy = model; }
	void SetHP(int HP) { this->hp = HP; }
	void SetIsDead(bool isDead) { this->isDead = isDead; }
	void SetIsWind(bool isWind) { this->isWind = isWind; }
	void SetWeight(float weight) { this->weight = weight; }
	void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	void SetKingDirection(XMFLOAT3 direction) { this->kingDirection = direction; }
	void SetWindDirection(XMFLOAT3 windDirection) { this->windDirection = windDirection; }
	void SetPlayerPushDirection(XMFLOAT3 playerPushDirection) { this->playerPushDirection = playerPushDirection; }
	int GetHP() { return hp; }
	bool GetIsDead() { return isDead; }
	bool GetIsWind() { return isWind; }
	bool GetWeight() { return weight; }
	float GetIsMoveSpeed() { return moveSpeed; }
	XMFLOAT3 GetKingDirection() { return kingDirection; }
	XMFLOAT3 GetWindDirection() { return windDirection; }
	XMFLOAT3 GetPosition() { return ConvertXMVECTORtoXMFLOAT3(each.position); }
	EnemyModel GetModel() { return enemy; }

	void Init();
	void Update(King &king);
	void Draw();
	//�����_���|�W�V�����ɐ���
	void SetRandomPosition();
	//Init����Flag���Z�b�g����
	void SetAlive();
	//�w�C�g�̕������擾����
	void UpdateKingDirection(XMFLOAT3 &kingPos);
	void HitDethLine();
};

class SuperEnemy : public EnemyBase
{
private:
	const int MaxHP = 2;
	const float MaxMoveSpeed = 0.5f;
	const float MaxWeight = 2.0f;
	//static const int dethLine = 25;
public:
	void Init();
	void SetAlive();
};

class Enemys
{
public:
	static list<EnemyBase> enemys;
	static list<list<EnemyBase>::iterator> deleteEnemys;
	static void AddEnemy(EnemyType type);
	static void DeathEnemy(EnemyBase &enemy);
	static void Update(King &king);
	static void Draw();
};