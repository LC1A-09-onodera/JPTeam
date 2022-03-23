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
	//モデル
	EnemyModel enemy;
	//個々の座標
	EachInfo each;

	//イージングスタート位置
	XMFLOAT3 startPos;
	//イージングの終了位置
	XMFLOAT3 endPos;
	//イージングタイム
	float easeTime;
	//イージングタイムの増加率
	float easeAddTime;
	
	//エネミーの属性
	EnemyType type;
	//動く方向
	EnemyMoveDirection moveDireciotnEnum;
	//体力
	int hp;
	//死んでたらtrue
	bool isDead;
	//爆風受けたらtrue
	bool isWind;
	//動く速度
	float moveSpeed;
	//重さ
	float weight;

	//キングの方向
	XMFLOAT3 moveDirection;
	//爆風の押し出しPower
	XMFLOAT3 windDirection;
	//プレイヤーとの押し出し
	XMFLOAT3 playerPushDirection;

	//ヘイト方向
	Target target;

	//最大値
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
	//ランダムポジションに生成
	void SetRandomPosition();
	//Init時にFlagをセットする
	void SetAlive(const EnemyMoveDirection &direction);
	//ヘイトの方向を取得する
	void UpdateKingDirection(XMFLOAT3 &kingPos);
	//枠に触れているかなどの処理
	void HitDethLine();
	//爆風を受けたときの処理
	void AddWindForce();
	//ヘイト方向へ動く
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