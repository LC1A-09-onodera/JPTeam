#include "Oserro.h"
#include "../Shader/ShaderManager.h"
#include "../Camera/Camera.h"
#include "../BaseDirectX/Input.h"

list<Othello> OthelloManager::othellos;
OthelloModel OthelloManager::oserroModel;

namespace
{
	const int fieldSize = 8;
	const float cellScale = 1.0f;
	const XMFLOAT3 stageLeftTop{ 0, 0, 0 };
}

OthelloData::OthelloData()
{
	for (int i = 0; i < 8; i++)
	{
		FrontActiveAngle.push_back(true);
	}
}

void Othello::Init(Model *model)
{
	this->model = model;
	each.CreateConstBuff0();
	each.CreateConstBuff1();
}

void Othello::Update()
{
	//タイプがNONE出なかったら生きている
	bool isAlive = data.type != NONE;
	if (!isAlive) return;
	float x = (data.widthPos * cellScale) + (cellScale / 2);
	float y = (data.heightPos * cellScale) + (cellScale / 2);

	if (!data.isPlayer)
	{
		each.position = { x, y ,0 };
	}
}

void Othello::Draw()
{
	if (data.type != NONE)
	{
		//each.rotation = {90, 0,0 };
		//each.scale = {1, 1, 1};
		//each.rotation.y ++;
		model->Update(&each);
		Draw3DObject(*model);
	}
}

void Othello::Finalize()
{
}

void Othello::RightRevers()
{
	float rate = 0;
	float easeRate = EaseInOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, rate).x;

	if (data.IsFront)
	{
		each.rotation.y = 180.0f * easeRate;
	}
	else
	{
		each.rotation.y = 180.0f * easeRate + 180.0f;
	}


	if (rate >= 1.0f)
	{
		//回転フラグoff

		//表裏の変更
		data.IsFront = !data.IsFront;
		if (data.IsFront)
		{
			each.rotation.y = 0;
		}
		else
		{
			each.rotation.y = 180;
		}

	}
}
void Othello::LeftRevers()
{
	float rate = 0;
	float easeRate = EaseInOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, rate).x;
	if (data.IsFront)
	{
		each.rotation.y = -180.0f * easeRate;
	}
	else
	{
		each.rotation.y = -180.0f * easeRate + 180.0f;
	}

	if (rate >= 1.0f)
	{
		//回転フラグoff

		//表裏の変更
		data.IsFront = !data.IsFront;

		if (data.IsFront)
		{
			each.rotation.y = 0;
		}
		else
		{
			each.rotation.y = 180;
		}
	}

}
void Othello::Controll(const XMFLOAT3 &mousePos)
{
	//data.widthPos = data.heightPos = 1;
	XMVECTOR playerPos = { data.widthPos * cellScale,data.heightPos * -cellScale , 0, 0 };
	//XMVECTOR playerPos = each.position;

	//XMFLOAT3 tmp = { 8.56402779, 6.03010273, 0 };

	playerPos += ConvertXMFLOAT3toXMVECTOR(stageLeftTop);
	XMVECTOR dist = ConvertXMFLOAT3toXMVECTOR(mousePos - playerPos);
	XMVECTOR angle = XMVector3Normalize(dist);


	while (true)
	{
		if (Input::KeyTrigger(DIK_1))
		{
			int hoge = 0;
		}


		playerPos = { data.widthPos * cellScale * 2,data.heightPos * -cellScale * 2, 0, 0 };
		playerPos += ConvertXMFLOAT3toXMVECTOR(stageLeftTop);

		dist = ConvertXMFLOAT3toXMVECTOR(mousePos - playerPos);
		angle = XMVector3Normalize(dist);
		float length = XMVector3Length(dist).m128_f32[0];
		//距離がマスの大きさより長かった場合

			//縦横どちらが長いか
		if (fabs(angle.m128_f32[0]) > fabs(angle.m128_f32[1]))
		{//横のほうが長かったら
			float length = fabs(dist.m128_f32[0]);
			if (length >= cellScale)
			{
				//左右どちらか
				if (angle.m128_f32[0] > 0)
				{//右に行く
					data.widthPos++;
				}
				else
				{//左に行く
					data.widthPos--;
				}
			}
			else
			{
				break;
			}
		}
		else
		{//縦のほうが長かったら
			float length = fabs(dist.m128_f32[1]);
			if (length >= cellScale)
			{
				//上下どちらか
				if (angle.m128_f32[1] < 0)
				{//下に行く
					data.heightPos++;
				}
				else
				{//上に行く
					data.heightPos--;
				}

			}
			else
			{
				break;
			}
		}

		data.IsFront = !data.IsFront;

	}


	//傾かせる
	//縦横どちらが長いか
	if (data.IsFront)
	{
		if (Input::KeyTrigger(DIK_1))
		{
			int hoge = 0;
		}
		if (fabs(angle.m128_f32[0]) > fabs(angle.m128_f32[1]))
		{//横のほうが長かったら
			//左右どちらか
			playerPos.m128_f32[0] += dist.m128_f32[0];
			each.rotation.y = dist.m128_f32[0] * -90;
			each.rotation.x = 0;
		}
		else
		{//縦のほうが長かったら

			playerPos.m128_f32[1] += dist.m128_f32[1];
			//上下どちらか
			each.rotation.x = dist.m128_f32[1] * 90;
			each.rotation.y = 0;
		}
	}
	else
	{
		if (fabs(angle.m128_f32[0]) > fabs(angle.m128_f32[1]))
		{//横のほうが長かったら
			//左右どちらか
			playerPos.m128_f32[0] += dist.m128_f32[0];
			each.rotation.y = dist.m128_f32[0] * -90 + 180;
			each.rotation.x = 0;
		}
		else
		{//縦のほうが長かったら
			playerPos.m128_f32[1] += dist.m128_f32[1];
			//上下どちらか
			each.rotation.x = dist.m128_f32[1] * 90 + 180;
			each.rotation.y = 0;
		}

	}
	each.position = playerPos;
}

void Othello::Spawn(OthelloType type, int x, int y)
{
	if (data.type == NONE)
	{
		data.widthPos = x;
		data.heightPos = y;
		data.type = type;
	}
}

void OthelloManager::Init()
{
	oserroModel.CreateModel("newOserro", ShaderManager::playerShader);
	Othello player;
	player.Spawn(NORMAL, 4, 4);
	player.Init(&oserroModel);
	OthelloData *playerData = player.GetGameData();
	playerData->isPlayer = true;
	othellos.push_back(player);
}

void OthelloManager::Update()
{
	auto itr = othellos.begin();
	for (; itr != othellos.end(); ++itr)
	{
		itr->Update();
	}
}

void OthelloManager::Draw()
{
	if (othellos.size() == 0) return;

	auto itr = othellos.begin();
	for (; itr != othellos.end(); ++itr)
	{
		itr->Draw();
	}

}

void OthelloManager::Finalize()
{
	auto itr = othellos.begin();
	for (; itr != othellos.end(); ++itr)
	{
		itr->Finalize();
	}

}

void OthelloManager::Controll()
{
	//マウスのクリックがされた瞬間
	if (false)
	{
		SetPlayer();
	}
	//マウスがドラッグ中
	if (true)
	{
		Move();
	}

	//マウスをドロップしたら
	if (false)
	{
		RemovePlayer();
	}


}

void OthelloManager::SetPlayer()
{
	XMFLOAT3 MousePos = Camera::MousePosition(0.0f);
	XMFLOAT3 stagePos = MousePos - stageLeftTop;
	//どのマスにマウスがあるのかを確認
	int x = stagePos.x / cellScale;
	int y = stagePos.y / cellScale;
	auto itr = othellos.begin();
	for (; itr != othellos.end(); itr++)
	{
		if (itr->GetGameData()->widthPos == x && itr->GetGameData()->heightPos == y)
		{
			itr->GetGameData()->isPlayer = true;
			break;
		}
	}
}

void OthelloManager::Move()
{
	XMFLOAT3 MousePos = Camera::MousePosition(0.0f);
	XMFLOAT3 stagePos = MousePos - stageLeftTop;
	//どのマスにマウスがあるのかを確認
	int x = stagePos.x / cellScale;
	int y = stagePos.y / cellScale;

	auto itr = othellos.begin();

	//プレイヤーを探索
	for (; itr != othellos.end(); ++itr)
	{
		OthelloData *data = itr->GetGameData();
		if (data->isPlayer)
		{
			break;
		}
	}
	itr->Controll(MousePos);
	//（まうすの座標がプレイヤーのマスの座標と同一だったら）または、
	//（移動先のマスに駒があったら）
}

void OthelloManager::RemovePlayer()
{
	auto itr = othellos.begin();

	//プレイヤーを探索
	for (; itr != othellos.end(); ++itr)
	{
		OthelloData *data = itr->GetGameData();
		if (data->isPlayer)
		{
			break;
		}
	}
	itr->GetGameData()->isPlayer = false;
	int x = itr->GetGameData()->widthPos;
	int y = itr->GetGameData()->heightPos;
}
