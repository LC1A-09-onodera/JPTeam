#pragma once
#include "Bomb.h"
#include "Blast.h"
#include <vector>
#include <list>
class BombManager
{
public:
	BombManager();
	~BombManager();
	void Init();
	void Update();
	void Finalize();
	void Draw();
	void Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos);

	bool GetBombAlive();
	int GetBombAliveCount();


	/// <summary>
	/// ���e�ƓG &�����ƓG�̃R���W������Z�߂�����
	/// </summary>
	/// <param name="data"></param>
	void enemyCollision(std::list<EnemyBase> &data);
	/// <summary>
	/// �G�Ɣ��e�̐ڐG����
	/// </summary>
	/// <param name="data"></param>
	void enemyBombCollision(std::list<EnemyBase> &data);

	/// <summary>
	/// �G�Ɣ����̐ڐG����
	/// </summary>
	/// <param name="data"></param>
	void enemyBlastCollision(std::list<EnemyBase> &data);

	/// <summary>
	/// �����ƃv���C���[�̐ڐG����
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="radius"></param>
	void PlayerCollision(XMFLOAT3 pos, float radius);

	/// <summary>
	/// ������
	/// </summary>
	void Explosion();

	/// <summary>
	/// �����Ɣ��e�̐ڐG����
	/// </summary>
	void BlastBombCollision();
	void BlastDelete();

	void BlastSpawn(const XMFLOAT3 &pos);

private:
	void AddBlast(Blast &blast);
private:
	vector<Bomb> bombs;
	list<Blast> blasts;
};

class BombMesh
{
public:
	static Model bombMesh;
	static Model blastMesh;
	static void LoadModel();

};