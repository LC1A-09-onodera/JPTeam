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

enum class EnemyMoveDirection
{
	RIGHT,
	LEFT,
	UP,
	BOTTOM,
	HATE,
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
	//���f��
	EnemyModel enemy;
	//�X�̍��W
	EachInfo each;

	//�C�[�W���O�X�^�[�g�ʒu
	XMFLOAT3 startPos;
	//�C�[�W���O�̏I���ʒu
	XMFLOAT3 endPos;
	//�C�[�W���O�^�C��
	float easeTime;
	//�C�[�W���O�^�C���̑�����
	float easeAddTime;
	
	//�G�l�~�[�̑���
	EnemyType type;
	//��������
	EnemyMoveDirection moveDireciotnEnum;
	//�̗�
	int hp;
	//����ł���true
	bool isDead;
	//�����󂯂���true
	bool isWind;
	//�������x
	float moveSpeed;
	//�d��
	float weight;

	//�L���O�̕���
	XMFLOAT3 moveDirection;
	//�����̉����o��Power
	XMFLOAT3 windDirection;
	//�v���C���[�Ƃ̉����o��
	XMFLOAT3 playerPushDirection;

	//�w�C�g����
	Target target;

	//�ő�l
	const int MaxHP = 1;
	const float MaxMoveSpeed = 0.01f;
	const float MaxWeight = 1.0f;
	const float resistForce = 1.0f;
	const float resistMinForce = 1.1f;
public:
	void SetMesh(EnemyModel &model) { enemy = model; }
	void SetHP(int HP) { this->hp = HP; }
	void SetIsDead(bool isDead) { this->isDead = isDead; }
	void SetIsWind(bool isWind) { this->isWind = isWind; }
	void SetWeight(float weight) { this->weight = weight; }
	void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	void SetKingDirection(XMFLOAT3 direction) { this->moveDirection = direction; }
	void SetWindDirection(XMFLOAT3 windDirection) { this->windDirection = windDirection; }
	void SetPlayerPushDirection(XMFLOAT3 playerPushDirection) { this->playerPushDirection = playerPushDirection; }
	int GetHP() { return hp; }
	bool GetIsDead() { return isDead; }
	bool GetIsWind() { return isWind; }
	bool GetWeight() { return weight; }
	float GetIsMoveSpeed() { return moveSpeed; }
	XMFLOAT3 GetKingDirection() { return moveDirection; }
	XMFLOAT3 GetWindDirection() { return windDirection; }
	XMFLOAT3 GetPosition() { return ConvertXMVECTORtoXMFLOAT3(each.position); }
	EnemyModel GetModel() { return enemy; }

	void Init(const EnemyMoveDirection &direction);
	void Update(XMFLOAT3 &hate);
	void Draw();
	//�����_���|�W�V�����ɐ���
	void SetRandomPosition();
	//Init����Flag���Z�b�g����
	void SetAlive(const EnemyMoveDirection &direction);
	//�w�C�g�̕������擾����
	void UpdateKingDirection(XMFLOAT3 &kingPos);
	//�g�ɐG��Ă��邩�Ȃǂ̏���
	void HitDethLine();
	//�������󂯂��Ƃ��̏���
	void AddWindForce();
	//�w�C�g�����֓���
	void GoHate(XMFLOAT3 &hatePosition);
	//
	void GoMoveDirection();
	//
	void SampleAddForce();

	bool IsHolesHit(XMFLOAT3& hole);
	float HolesLenght(XMFLOAT3& hole);
	void HolesHit();

	void CheckHitKing(King& kingPos);
	bool IsKingHit(XMFLOAT3& kingPos);
	float KingToLenght(XMFLOAT3& kingPos);
};

class SuperEnemy : public EnemyBase
{
private:
	const int MaxHP = 2;
	const float MaxMoveSpeed = 0.01f;
	const float MaxWeight = 2.0f;
public:
	void Init(const EnemyMoveDirection& direction);
	void SetAlive(const EnemyMoveDirection& direction);
};

class Enemys
{
public:
	static list<EnemyBase> enemys;
	static list<list<EnemyBase>::iterator> deleteEnemys;
	static void AddEnemy(EnemyType type, const EnemyMoveDirection &direction);
	static void DeathEnemy(EnemyBase &enemy);
	static void Update(XMFLOAT3& hate);
	static void Draw();
};