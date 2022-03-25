#include "BombManager.h"
#include "../Shader/ShaderManager.h"
namespace
{
	const int bombsMaxCount = 10;
}

Model BombMesh::bombMesh;
Model BombMesh::blastMesh;
BombManager::BombManager()
{
	bombs.resize(bombsMaxCount);
}

BombManager::~BombManager()
{
}

void BombManager::Init()
{
	for (int i = 0; i < bombs.size(); i++)
	{
		bombs[i].Init(BombMesh::bombMesh);
	}
	if (blasts.size() <= 0)return;
	blasts.clear();
}

void BombManager::Update()
{
	for (int i = 0; i < bombs.size(); i++)
	{
		bombs[i].Update();
	}

	if (blasts.size() <= 0) return;

	auto itr = blasts.begin();
	for (; itr != blasts.end(); itr++)
	{
		itr->Update();
	}
	BlastDelete();
}

void BombManager::Finalize()
{
	for (int i = 0; i < bombs.size(); i++)
	{
		bombs[i].Finailize();
	}
}

void BombManager::Draw()
{
	for (int i = 0; i < bombs.size(); i++)
	{
		bombs[i].Draw();
	}
	auto itr = blasts.begin();
	for (; itr != blasts.end(); itr++)
	{
		itr->Draw();
	}
}

void BombManager::Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos)
{
	bool isShot = false;
	const int Nothing = 100;
	int bombNumber = Nothing;
	for (int i = 0; i < bombs.size(); i++)
	{
		if (bombs[i].GetIsAlve())
		{
			return;
		}

		bombNumber = i;

	}
	if (bombNumber != Nothing)
	{
		bombs[bombNumber].Shot(angle, pos);
	}
}

bool BombManager::GetBombAlive()
{
	for (int i = 0; i < bombs.size(); i++)
	{
		if (bombs[i].GetIsAlve())
		{
			return true;
		}
	}
	return false;
}

void BombManager::enemyCollision(std::list<EnemyBase> &data)
{
	enemyBombCollision(data);
	enemyBlastCollision(data);
}

void BombManager::enemyBombCollision(std::list<EnemyBase> &data)
{
	auto itr = data.begin();
	for (; itr != Enemys::enemys.end(); ++itr)
	{
		if (itr->GetIsDead() == true)continue;
		for (int i = 0; i < bombs.size(); i++)
		{
			if (bombs[i].EnemyBombCollision(*itr))
			{
				BlastSpawn(ConvertXMVECTORtoXMFLOAT3(bombs[i].GetPos()));
				BlastSpawn(itr->GetPosition());
			}
		}
	}

}

void BombManager::enemyBlastCollision(std::list<EnemyBase> &data)
{
	auto enemyItr = data.begin();
	auto blastItr = blasts.begin();
	for (; blastItr != blasts.end(); blastItr++)
	{
		for (; enemyItr != Enemys::enemys.end(); ++enemyItr)
		{
			if (blastItr->EnemyBombCollision(*enemyItr) && !enemyItr->GetIsDead())
			{
				enemyItr->SetIsDead(true);

				BlastSpawn(enemyItr->GetPosition());
				break;
			}
		}
	}
}

void BombManager::PlayerCollision(XMFLOAT3 pos, float radius)
{
	if (blasts.size() <= 0)return;

	float power = 0.0f;
	auto itr = blasts.begin();
	for (; itr != blasts.end(); itr++)
	{
		itr->PlayerBlastCollision(pos, radius);
	}
}

void BombManager::Explosion()
{
	//”š’e‚Ì”š”j
	for (int i = 0; i < bombs.size(); i++)
	{
		if (!bombs[i].GetIsAlve() && bombs[i].GetIsAlve())
		{
			BlastSpawn(ConvertXMVECTORtoXMFLOAT3(bombs[i].GetPos()));
		}
	}
}

void BombManager::BlastBombCollision()
{
	if (blasts.size() <= 0)return;
	for (int i = 0; i < bombs.size(); i++)
	{
		if (bombs[i].GetIsAlve())
		{
			auto itr = blasts.begin();
			for (; itr != blasts.end(); itr++)
			{
				if (itr->ChainCollision(bombs[i]))
				{
					bombs[i].Explosion();
					BlastSpawn(ConvertXMVECTORtoXMFLOAT3(bombs[i].GetPos()));
					break;
				}
			}
		}
	}
}

void BombManager::BlastSpawn(const XMFLOAT3 &pos)
{
	Blast blast;
	blast.Init( &BombMesh::blastMesh);
	blast.Spawn(pos);
	AddBlast(blast);
}

void BombManager::AddBlast(Blast &blast)
{
	blasts.push_back(blast);
}

void BombManager::BlastDelete()
{
	if (blasts.size() <= 0) return;
	auto itr = blasts.begin();
	list<list<Blast>::iterator> deleteBlasts;
	for (; itr != blasts.end(); ++itr)
	{
		if (itr->GetIsAlve() <= 0)
		{
			deleteBlasts.push_back(itr);
		}
	}
	if (deleteBlasts.size() <= 0) return;
	auto deleteItr = deleteBlasts.begin();
	for (; deleteItr != deleteBlasts.end(); ++deleteItr)
	{
		blasts.erase(*deleteItr);
	}
	deleteBlasts.clear();
}

void BombMesh::LoadModel()
{
	bombMesh.CreateModel("Block", ShaderManager::playerShader);
	blastMesh.CreateModel("Block", ShaderManager::playerShader);
}
