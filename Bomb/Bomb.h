#pragma once
#include <DirectXMath.h>
#include "../sampleObject/sampleObject.h"
#include "../Enemy/Enemy.h"

class Hole;
class Blast;
struct BombData
{
	DirectX::XMVECTOR pos = {};//爆弾の座標
	DirectX::XMVECTOR bombAngle = { -1, 0, 0 };
	float bombRadius = 1.0f;//爆弾自体の大きさ
	float bombSpeed = 1.0f;//爆弾の速度
	bool isAlive = false;//爆弾が現在生きているかどうか
	int bombAliveTimer = 0;
};
class Bomb
{
public:
	Bomb();
	~Bomb();
	//初期化
	void Init(const Model &model);
	//更新処理
	void Update();
	//終了処理
	void Finailize();
	//描画処理
	void Draw();

	/// <summary>
	/// 発射
	/// </summary>
	/// <param name="angle">向き</param>
	/// <param name="pos"></param>
	/// <returns></returns>
	bool Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos);

	/// <summary>
	/// 敵との当たり判定
	/// </summary>
	/// <param name="enemyData"></param>
	bool EnemyBombCollision(EnemyBase &enemyData);


	/// <summary>
	/// 爆発処理
	/// </summary>
	void Explosion();
public://アクセッサ
	bool GetIsAlve() {return data.isAlive;}
	const XMVECTOR &GetPos(){return data.pos;}
	float GetRadius() { return data.bombRadius; }
private:
	//爆弾が生きている際の更新処理
	void BombUpdate();


	/// <summary>
	/// 爆弾本体との当たり判定
	/// </summary>
	/// <param name="pos">判定をとる物体の座標</param>
	/// <param name="radius">判定をとる物体の半径</param>
	/// <param name="isHit">当たったか</param>
	bool BombCollision(const XMVECTOR &pos, const float &radius);

	void MeshCopy(const Model &model) { this->bombObject = model;}

private:
	BombData data;
	Model bombObject;
	EachInfo each;
};

