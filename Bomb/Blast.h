#pragma once

#pragma once
#include <DirectXMath.h>
#include "../sampleObject/sampleObject.h"
#include "../Enemy/Enemy.h"

class Bomb;
class Hole;
struct BlastData
{
	DirectX::XMVECTOR pos = {};//爆弾の座標
	float blastRadius = 5.0f;//爆発の半径
	float blastPower = 0;//爆風の強さ
	int blastTimer = 0;//爆発タイマー(とりあえず使わない)
	bool isAlive = false;//爆が現在生きているかどうか
};
class Blast
{
public:
	Blast();
	~Blast();
	//初期化
	void Init(const Model &model);
	//更新処理
	void Update();
	//終了処理
	void Finailize();
	//描画処理
	void Draw();

	float PlayerBlastCollision(XMFLOAT3 pos, float radius);

	/// <summary>
/// 敵との当たり判定
/// </summary>
/// <param name="enemyData"></param>
	bool EnemyBombCollision(EnemyBase &enemyData);

	bool ChainCollision(Bomb &bomb);

	/// <summary>
	/// 爆風発生
	/// </summary>
	void Spawn(XMFLOAT3 pos);
public://アクセッサ
	bool GetIsAlve() { return data.isAlive; }
	const XMVECTOR &GetPos() { return data.pos; }
	float GetRadius() {return data.blastRadius;}
private:
	//爆発しているときの更新処理
	void BlastUpdate();

	/// <summary>
	/// 爆風との判定
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="radius"></param>
	/// <param name="blastPower"></param>
	/// <returns></returns>
	bool BlastCollision(const XMVECTOR &pos, const float &radius, XMFLOAT3 *blastPower = nullptr);

private:
	void MeshCopy(const Model &model) { this->blastObject = model; }

	BlastData data;
	Model blastObject;
	EachInfo each;
};