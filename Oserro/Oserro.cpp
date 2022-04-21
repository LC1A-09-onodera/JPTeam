#include "Oserro.h"
#include "../Shader/ShaderManager.h"
#include "../Camera/Camera.h"
#include "../BaseDirectX/Input.h"
#include "../3DObjectParticle/3DObjectParticle.h"
#include "../OthlloPlayer/OthlloPlayer.h"
#include "../Thunder/Thunder.h"
#include "../imgui/ImguiControl.h"
#include <random>
#include <limits>
list<Othello> OthelloManager::othellos;
OthelloModel OthelloManager::oserroModel;
vector<vector<SendOthelloData>> OthelloManager::sendDatas;
using namespace OthelloConstData;

OthelloData::OthelloData()
{
	for (int i = 0; i < 8; i++)
	{
		FrontActiveAngle.push_back(true);
	}
}

void Othello::Init(OthelloModel *model)
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
	float x = (data.widthPos * cellScale * 2);
	float y = -(data.heightPos * cellScale * 2);

	if (data.isSpawn)
	{
		SpawnUpdate();
	}
	else
	{
		if (data.isReverce)
		{
			ReversUpdate();
		}
		else if (data.isSandwich)
		{
			SinkWait();
		}
		else if (data.isVanish)
		{
			Sink();
		}
		else
		{
			if (!data.isPlayer)
			{
				each.position = XMVECTOR{ x, y ,0, 0 };
				each.position += ConvertXMFLOAT3toXMVECTOR(stageLeftTop);
				if (data.isFront)
				{
					each.rotation.y = 0;
					each.rotation.x = 0;
				}
				else
				{
					each.rotation.y = 180;
					each.rotation.x = 0;
				}
				if (data.isShake)
				{
					Shake();
				}
			}
		}
	}
}


void Othello::Shake()
{
	std::random_device seed;
	std::mt19937 randum(seed());

	int x = randum();
	int y = randum();
	float powerX = static_cast<float>(rand()) / RAND_MAX;
	float powerY = static_cast<float>(rand()) / RAND_MAX;
	powerX += -0.5f;
	powerY += -0.5f;
	float shakepower = 0.1;
	powerX *= shakepower;
	powerY *= shakepower;
	each.position.m128_f32[0] += powerX;
	each.position.m128_f32[1] += powerY;

}

void Othello::Draw()
{
	if (data.type != NONE)
	{
		//each.rotation = {90, 0,0 };
		//each.scale = {1, 1, 1};
		//each.rotation.y ++;
		if (each.scale.x > 0.7)
		{
			each.alpha = 1.0f;
		}
		else
		{
			each.alpha = 0.5f;
		}
		model->Update(&each);
		Draw3DObject(*model);
	}
}

void Othello::Finalize()
{
}

void Othello::Revers()
{
	data.isReverce = true;
	data.isPlayer = false;
	data.isSandwich = true;
	data.animationTimer = 0;
	data.waitTimer = 0;
	data.JumpTimer = 0;
	data.isJumpUp = true;
	XMFLOAT3 pos = ConvertXMVECTORtoXMFLOAT3(each.position);
	pos.z = -0.3f;
	ThunderModels::Init(pos, XMFLOAT3(0, 0, 0));
}


void Othello::Sandwich()
{
	data.isPlayer = false;
	data.isSandwich = true;
	data.waitTimer = animationTimerMax;
	XMFLOAT3 pos = ConvertXMVECTORtoXMFLOAT3(each.position);
	pos.z = -0.3f;
	ThunderModels::Init(pos, XMFLOAT3(0, 0, 0));

}

void Othello::SpawnUpdate()
{
	if (data.spawnTimer >= SpawnAnimationTimerMax)
	{
		data.isSpawn = false;
	}

	float nowScale = static_cast<float>(data.spawnTimer) / SpawnAnimationTimerMax;
	data.isHarf = (nowScale >= 0.5f);
	each.scale = { nowScale , nowScale , 1 };
	data.spawnTimer++;
}

void Othello::ReversUpdate()
{
	if (data.waitTimer > 0)
	{
		data.waitTimer--;
		return;
	}

	if (data.animationTimer == 0)
	{
		//MakeParticle();
	}
	data.animationTimer++;
	float rate = static_cast<float>(data.animationTimer) / animationTimerMax;
	float easeRate = EaseInOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, rate).x;
	if (data.isFront)
	{
		each.rotation.y = 180.0f * easeRate;
		each.rotation.x = 0;
	}
	else
	{
		each.rotation.y = 180.0f * easeRate + 180.0f;
		each.rotation.x = 0;
	}

	data.JumpTimer++;

	int count = data.comboCount;
	if (count >= 4)
	{
		count = 4;
	}
	const float jumpMax = 3.0f + (1 * count);


	float jumpRate = static_cast<float>(data.JumpTimer) / JumpTimerMax;
	float jumpEaseRate = 0.0f;
	if (data.isJumpUp)
	{
		jumpEaseRate = EaseOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, jumpRate).x;

		float hight = pow(jumpEaseRate, count);
		each.position.m128_f32[2] = -jumpMax * hight;
		if (data.JumpTimer >= JumpTimerMax)
		{
			data.isJumpUp = false;
			data.JumpTimer = 0;
		}
	}
	else
	{
		jumpEaseRate = EaseInQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, jumpRate).x;
		float hight = 1 - (pow(jumpEaseRate, count));
		each.position.m128_f32[2] = -jumpMax * hight;
	}


	if (rate >= 1.0f)
	{
		//回転フラグoff
		data.isFront = !data.isFront;
		data.isReverce = false;
		//data.comboCount = 0;

		data.isVanish = true;

		////ひっくり返ったら起動フラグをオンにする
		//data.isMove = true;

	}
}
void Othello::LeftRevers()
{
	float rate = 0;
	float easeRate = EaseInOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, rate).x;
	if (data.isFront)
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
		data.isFront = !data.isFront;

		if (data.isFront)
		{
			each.rotation.y = 0;
		}
		else
		{
			each.rotation.y = 180;
		}
	}

}

void Othello::Controll(const XMFLOAT3 &mousePos, int &moveCount)
{
	XMVECTOR playerPos = { data.widthPos * cellScale,data.heightPos * -cellScale , 0, 0 };

	playerPos += ConvertXMFLOAT3toXMVECTOR(stageLeftTop);
	XMVECTOR dist = ConvertXMFLOAT3toXMVECTOR(mousePos - playerPos);
	XMVECTOR angle = XMVector3Normalize(dist);


	bool isOver = false;
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

		panelPos tmpPos = { data.widthPos,data.heightPos };
		//縦横どちらが長いか
		if (fabs(angle.m128_f32[0]) > fabs(angle.m128_f32[1]))
		{//横のほうが長かったら
			float length = fabs(dist.m128_f32[0]);
			if (length >= cellScale)
			{
				//左右どちらか
				if (angle.m128_f32[0] > 0)
				{//右に行く
					tmpPos.x++;
				}
				else
				{//左に行く
					tmpPos.x--;
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
					tmpPos.y++;
				}
				else
				{//上に行く
					tmpPos.y--;
				}

			}
			else
			{
				break;
			}
		}

		bool moveEnd = false;
		auto itr = data.panels.begin();
		for (; itr != data.panels.end(); itr++)
		{
			if (tmpPos.x == itr->x && tmpPos.y == itr->y)
			{
				moveEnd = true;
				break;
			}
		}
		if (moveEnd)
		{
			break;
		}
		data.widthPos = tmpPos.x;
		data.heightPos = tmpPos.y;
		//範囲を超過したか

		isOver = (data.widthPos < 0 ||
			data.widthPos >= fieldSize ||
			data.heightPos < 0 ||
			data.heightPos >= fieldSize);
		if (isOver)
		{
			if (data.widthPos < 0)
			{
				data.widthPos = 0;
			}
			if (data.widthPos >= fieldSize)
			{
				data.widthPos = fieldSize - 1;
			}
			if (data.heightPos < 0)
			{
				data.heightPos = 0;
			}
			if (data.heightPos >= fieldSize)
			{
				data.heightPos = fieldSize - 1;
			}

			break;
		}
		//ひっくり返る
		data.isFront = !data.isFront;
		moveCount++;
	}


	//傾かせる
	//縦横どちらが長いか

	if (!isOver)
	{
		if (data.isFront)
		{
			if (Input::KeyTrigger(DIK_1))
			{
				int hoge = 0;
			}
			if (fabs(angle.m128_f32[0]) > fabs(angle.m128_f32[1]))
			{//横のほうが長かったら
				//左右どちらか
				float length = dist.m128_f32[0];
				if (fabs(length) >= 1.0f)
				{
					length /= fabs(length);
				}
				playerPos.m128_f32[0] += length;
				each.rotation.y = length * -90;
				each.rotation.x = 0;
			}
			else
			{//縦のほうが長かったら
				float length = dist.m128_f32[1];
				if (fabs(length) >= 1.0f)
				{
					length /= fabs(length);
				}
				playerPos.m128_f32[1] += length;
				//上下どちらか
				each.rotation.x = length * 90;
				each.rotation.y = 0;
			}
		}
		else
		{
			if (Input::KeyTrigger(DIK_1))
			{
				int hoge = 0;
			}
			if (fabs(angle.m128_f32[0]) > fabs(angle.m128_f32[1]))
			{//横のほうが長かったら
				//左右どちらか
				float length = dist.m128_f32[0];
				if (fabs(length) >= 1.0f)
				{
					length /= fabs(length);
				}
				playerPos.m128_f32[0] += length;
				each.rotation.y = length * -90 + 180;
				each.rotation.x = 0;
			}
			else
			{//縦のほうが長かったら
				float length = dist.m128_f32[1];
				if (fabs(length) >= 1.0f)
				{
					length /= fabs(length);
				}
				playerPos.m128_f32[1] += length;
				//上下どちらか
				each.rotation.x = length * 90 + 180;
				each.rotation.y = 0;
			}

		}
	}
	each.position = playerPos;
}

void Othello::Spawn(OthelloType type, int x, int y, bool isFront)
{
	if (data.type == NONE)
	{
		data.widthPos = x;
		data.heightPos = y;
		data.type = type;
		data.isFront = isFront;

		float x = (data.widthPos * cellScale * 2);
		float y = -(data.heightPos * cellScale * 2);

		each.position = XMVECTOR{ x, y ,0, 0 };
		each.position += ConvertXMFLOAT3toXMVECTOR(stageLeftTop);
		if (data.isFront)
		{
			each.rotation.y = 0;
			each.rotation.x = 0;
		}
		else
		{
			each.rotation.y = 180;
			each.rotation.x = 0;
		}
	}
}

void Othello::Borne(OthelloType type, int x, int y, bool isFront)
{
	if (data.type == NONE)
	{
		data.widthPos = x;
		data.heightPos = y;
		data.type = type;
		data.isFront = isFront;
		data.isSpawn = true;

		float x = (data.widthPos * cellScale * 2);
		float y = -(data.heightPos * cellScale * 2);

		each.position = XMVECTOR{ x, y ,0, 0 };
		each.position += ConvertXMFLOAT3toXMVECTOR(stageLeftTop);
		if (data.isFront)
		{
			each.rotation.y = 0;
			each.rotation.x = 0;
		}
		else
		{
			each.rotation.y = 180;
			each.rotation.x = 0;
		}
	}
}
void Othello::SinkWait()
{
	if (data.waitTimer > 0)
	{
		data.waitTimer--;
		return;
	}
	data.isSandwich = false;
	data.isVanish = true;
}

void Othello::Sink()
{
	data.vanishTimer++;
	if (data.vanishTimer >= vanishTimerMax)
	{
		data.isDead = true;
	}
	else
	{
		float nowScale = static_cast<float>(data.vanishTimer) / vanishTimerMax;
		data.isHarf = (nowScale >= 0.5f);
		each.scale = { 1 - nowScale, 1 - nowScale, 1.0f };
	}
}

void Othello::MakeParticle()
{
	XMFLOAT3 sample;
	sample = ConvertXMVECTORtoXMFLOAT3(each.position);
	//ObjectParticles::Init(sample, 10, ParticleType::Exprotion);
}

bool Othello::GetIsActive()
{
	return (data.isVanish || data.isSandwich);
}

void OthelloManager::Init()
{
	oserroModel.CreateModel("newOserro", ShaderManager::othelloShader);


	sendDatas.resize(fieldSize);

	auto itr = sendDatas.begin();
	for (; itr != sendDatas.end(); itr++)
	{
		itr->resize(fieldSize);
	}
}

void OthelloManager::Update()
{
	//死ぬ
	DeadPanel();

	MinSpawn();
	//生成
	RandumSetPanel();

	//更新
	auto itr = othellos.begin();

	for (; itr != othellos.end(); ++itr)
	{
		itr->Update();
	}
	if (Input::KeyTrigger(DIK_SPACE))
	{
		isFieldUpdate = true;
	}

	SaveSpawn();
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
	//MauseControll();
	PlayerControll();
}

void OthelloManager::SetPlayer()
{
	XMFLOAT3 MousePos = Camera::MousePosition(0.0f);
	XMFLOAT3 stagePos = MousePos - stageLeftTop - XMFLOAT3{ -cellScale / 2, cellScale / 2 , 0 };

	if (othellos.size() <= 0)
	{
		return;
	}
	//どのマスにマウスがあるのかを確認
	int x = stagePos.x / (cellScale * 2);
	int y = stagePos.y / -(cellScale * 2);
	auto itr = othellos.begin();
	auto playerItr = othellos.begin();
	for (; itr != othellos.end(); itr++)
	{
		if (itr->GetGameData()->widthPos == x && itr->GetGameData()->heightPos == y)
		{
			itr->GetGameData()->isPlayer = true;
			playerItr = itr;
			itr->GetGameData()->oldPos = { x, y };
			break;
		}
	}

	itr = othellos.begin();

	list<panelPos> tmpPanels;
	for (; itr != othellos.end(); itr++)
	{
		itr->GetGameData()->isMove = false;
		if (itr != playerItr)
		{
			int x = itr->GetGameData()->widthPos;
			int y = itr->GetGameData()->heightPos;
			panelPos data = { x, y };

			tmpPanels.push_back(data);
		}
	}
	playerItr->GetGameData()->panels = tmpPanels;
}

void OthelloManager::Move(const XMFLOAT3 &MousePos)
{
	XMFLOAT3 stagePos = MousePos - stageLeftTop;
	//どのマスにマウスがあるのかを確認
	int x = stagePos.x / (cellScale * 2);
	int y = stagePos.y / (cellScale * 2);

	auto playerItr = othellos.begin();

	//プレイヤーを探索
	for (; playerItr != othellos.end(); ++playerItr)
	{
		OthelloData *data = playerItr->GetGameData();
		if (data->isPlayer)
		{
			break;
		}
	}
	if (playerItr == othellos.end())
	{
		return;
	}

	auto itr = othellos.begin();

	list<panelPos> tmpPanels;
	for (; itr != othellos.end(); itr++)
	{
		//itr->GetGameData()->isMove = false;
		if (itr != playerItr)
		{
			int x = itr->GetGameData()->widthPos;
			int y = itr->GetGameData()->heightPos;
			panelPos data = { x, y };

			tmpPanels.push_back(data);
		}
	}
	playerItr->GetGameData()->panels = tmpPanels;



	playerItr->Controll(MousePos, moveCount);

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
	if (itr == othellos.end())
	{
		return;
	}


	itr->GetGameData()->isPlayer = false;
	int x = itr->GetGameData()->widthPos;
	int y = itr->GetGameData()->heightPos;

	panelPos tmpPos = itr->GetGameData()->oldPos;
	bool notMove = (tmpPos.x == x && tmpPos.y == y);
	if (!notMove)
	{
		itr->GetGameData()->isMove = true;
	}
}

void OthelloManager::AddPanel()
{
	//Othello panelA, panelB, panelC, panelD;
	//panelA.Spawn(NORMAL, 0, 1, true);
	//panelA.Init(&oserroModel);
	//othellos.push_back(panelA);

	//panelB.Spawn(NORMAL, 1, 1, false);
	//panelB.Init(&oserroModel);
	//othellos.push_back(panelB);

	//panelC.Spawn(NORMAL, 2, 0, true);
	//panelC.Init(&oserroModel);
	//othellos.push_back(panelC);

	//panelD.Spawn(NORMAL, 2, 1, false);
	//panelD.Init(&oserroModel);
	//othellos.push_back(panelD);
}

const vector<vector<SendOthelloData>> &OthelloManager::Send()
{
	if (Input::KeyTrigger(DIK_1))
	{
		int hoge = 0;
	}
	SendOthelloData empty;
	//空を作る
	empty.isFront = false;
	empty.isMove = false;
	empty.type = NONE;
	for (int i = 0; i < 8; i++)
	{
		empty.FrontActiveAngle.push_back(false);
	}

	//一回全体を空の駒で埋める
	for (int i = 0; i < fieldSize; i++)
	{
		for (int j = 0; j < fieldSize; j++)
		{
			sendDatas[i][j] = empty;
		}
	}

	//今存在する駒を突っ込んでいく
	auto itr = othellos.begin();
	for (; itr != othellos.end(); itr++)
	{
		OthelloData gameDatas = *itr->GetGameData();
		SendOthelloData data;
		if (gameDatas.isPlayer || gameDatas.isReverce || gameDatas.isSpawn)
		{
			continue;
		}
		data.FrontActiveAngle = gameDatas.FrontActiveAngle;
		data.isFront = gameDatas.isFront;
		data.type = gameDatas.type;

		if (itr->GetIsActive())
		{
			data.isSandwich = true;
		}


		data.isMove = false;

		bool isOnPlayer = (gameDatas.widthPos == playerPanelPos.x && gameDatas.heightPos == playerPanelPos.y);
		if (isOnPlayer)
		{
			data.isOnPlayer = true;
		}
		if (Input::KeyTrigger(DIK_SPACE) && isOnPlayer)
		{
			data.isMove = true;
		}

		data.comboCount = gameDatas.comboCount;
		data.SandwichLength = gameDatas.SandwichLength;
		data.score = gameDatas.score;
		sendDatas[gameDatas.heightPos][gameDatas.widthPos] = data;
	}

	return sendDatas;
}

void OthelloManager::Receive(const vector<vector<SendOthelloData>> &data)
{
	sendDatas = data;

	//
	auto itr = othellos.begin();
	list<list<Othello>::iterator> sandOthellos;
	list<list<Othello>::iterator> repairOthellos;
	//生きているやつを挟んだフラグ
	bool isSandFlip = false;
	for (; itr != othellos.end(); itr++)
	{
		OthelloData *gameDatas = itr->GetGameData();

		int x = gameDatas->widthPos;
		int y = gameDatas->heightPos;
		if (sendDatas[y][x].type == NONE)
		{
			continue;
		}

		if (sendDatas[y][x].isCheckEnd)
		{
			gameDatas->isShake = true;
		}
		else
		{
			gameDatas->isShake = false;
		}
		//if (sendDatas[y][x].comboCount >= 1 && !gameDatas->isVanish)
		//{
		//	gameDatas->comboCount = sendDatas[y][x].comboCount;
		//	itr->Revers();
		//}
		if (sendDatas[y][x].isFront != gameDatas->isFront && !gameDatas->isVanish && !gameDatas->isSandwich && !gameDatas->isReverce)
		{
			gameDatas->comboCount = sendDatas[y][x].comboCount;
			itr->Revers();
			//生きているやつを挟んだ
			isSandFlip = true;
			gameDatas->score = sendDatas[y][x].score;

		}
		else if (sendDatas[y][x].isSandwich && !gameDatas->isVanish && !gameDatas->isSandwich && !gameDatas->isReverce)
		{
			gameDatas->comboCount = sendDatas[y][x].comboCount;
			sandOthellos.push_back(itr);
			//gameDatas->isMove = true;
			gameDatas->score = sendDatas[y][x].score;
		}

		bool isAddVanish = (sendDatas[y][x].isSandwich && gameDatas->isVanish);
		if (isAddVanish)
		{
			repairOthellos.push_back(itr);
		}
		//gameDatas->isMove = false;
	}

	auto SandItr = sandOthellos.begin();
	for (; SandItr != sandOthellos.end(); SandItr++)
	{
		auto SandOthelloDataItr = *SandItr;
		if (isSandFlip)
		{
			SandOthelloDataItr->Sandwich();
		}
		else
		{
			SandOthelloDataItr->GetGameData()->comboCount = 0;
		}
	}
	auto repairItr = repairOthellos.begin();
	for (; repairItr != repairOthellos.end(); repairItr++)
	{
		auto repaierOthelloDataItr = *repairItr;
		if (isSandFlip)
		{
			int timer = repaierOthelloDataItr->GetGameData()->vanishTimer;

			if (timer >= chainRepairTime)
			{
				timer -= chainRepairTime;
			}
			else
			{
				timer = 0;
			}
			repaierOthelloDataItr->GetGameData()->vanishTimer = timer;
		}
	}

}

void OthelloManager::SetPanel()
{
	XMFLOAT3 MousePos = Camera::MousePosition(0.0f);
	XMFLOAT3 stagePos = MousePos - stageLeftTop - XMFLOAT3{ -cellScale / 2, cellScale / 2 , 0 };
	//どのマスにマウスがあるのかを確認
	int x = stagePos.x / (cellScale * 2);
	int y = stagePos.y / -(cellScale * 2);


	bool isOutSide = (x < 0 || x >= fieldSize || y < 0 || y >= fieldSize);
	if (isOutSide)
	{
		return;
	}
	auto itr = othellos.begin();
	for (; itr != othellos.end(); itr++)
	{
		if (itr->GetGameData()->widthPos == x && itr->GetGameData()->heightPos == y)
		{
			if (itr->GetGameData()->type == NORMAL)
			{
				if (itr->GetGameData()->isFront)
				{
					itr->GetGameData()->isFront = false;
				}
				else
				{
					othellos.erase(itr);
				}
				return;
			}
		}
	}

	if (itr == othellos.end())
	{
		Othello data;
		data.Init(&oserroModel);
		data.Spawn(NORMAL, x, y, true);
		data.Update();
		data.GetGameData()->isMove = false;
		if (x == playerPanelPos.x && y == playerPanelPos.y)
		{
			data.GetGameData()->isPlayer = true;
		}
		othellos.push_back(data);
	}
}

void OthelloManager::RandumSetPanel()
{
	spawnTimer++;

	bool notSpawn = (spawnTimer < spawnTimerMAx &&moveCount < spawnMoveCount);
	if (notSpawn)
	{
		return;
	}

	moveCount = 0;
	spawnTimer = 0;
	isFieldUpdate = true;
	for (int i = 0; i < spawnPanelCount; i++)
	{
		//フィールドがパネルで満たされているか
		bool isPanelMax = othellos.size() >= 64;
		if (isPanelMax)
		{
			return;
		}


		SpawnPanel(true);
	}
}

void OthelloManager::DeadPanel()
{
	auto itr = othellos.begin();
	list<list<Othello>::iterator> deadOthellos;

	for (; itr != othellos.end(); itr++)
	{
		if (itr->GetGameData()->isDead)
		{
			deadOthellos.push_back(itr);
			itr->MakeParticle();
		}
	}

	if (deadOthellos.size() <= 0)return;


	auto deadItr = deadOthellos.begin();
	for (; deadItr != deadOthellos.end(); deadItr++)
	{
		othellos.erase(*deadItr);
	}
}


void OthelloManager::SpawnPanel(bool isInGame)
{
	int x = rand() % 8;
	int y = rand() % 8;

	auto itr = othellos.begin();
	bool isSpawn = false;

	while (true)
	{
		itr = othellos.begin();
		for (; itr != othellos.end(); itr++)
		{
			if (itr->GetGameData()->widthPos == x && itr->GetGameData()->heightPos == y)
			{
				if (itr->GetGameData()->type == NORMAL)
				{
					x++;
					if (x >= fieldSize)
					{
						x = 0;
						y++;
						if (y >= fieldSize)
						{
							y = 0;
						}
					}
					break;
				}
			}
		}

		if (itr == othellos.end())
		{
			break;
		}

	}
	Othello data;
	data.Init(&oserroModel);

	bool randFront = rand() % 2;
	if (isInGame)
	{
		data.Borne(NORMAL, x, y, randFront);
	}
	else
	{
		data.Spawn(NORMAL, x, y, randFront);
	}
	data.Update();
	data.GetGameData()->isMove = false;
	if (x == playerPanelPos.x && y == playerPanelPos.y)
	{
		data.GetGameData()->isPlayer = true;
	}
	othellos.push_back(data);
}

void OthelloManager::MinSpawn()
{
	if (othellos.size() >= minPanelCount)return;

	int minPanelUnderCount = minPanelCount - othellos.size();

	for (int i = 0; i < minPanelUnderCount; i++)
	{
		SpawnPanel(true);
	}
}

void OthelloManager::MauseControll()
{
	//マウスのクリックがされた瞬間
	if (Input::MouseTrigger(MouseButton::LBUTTON))
	{
		SetPlayer();
	}
	//マウスがドラッグ中
	if (Input::Mouse(MouseButton::LBUTTON))
	{
		Move(Camera::MousePosition(0.0f));
	}

	//マウスをドロップしたら
	if (!Input::Mouse(MouseButton::LBUTTON))
	{
		RemovePlayer();
	}
	if (Input::MouseTrigger(MouseButton::RBUTTON))
	{
		SetPanel();
	}
}
void OthelloManager::PlayerControll()
{
	//マウスのクリックがされた瞬間
	if (isPlayerEnd)
	{
		KeySetPlayer();
	}
	//マウスがドラッグ中
	if (isPanelMove)
	{
		Move(OthlloPlayer::GetPosition());
	}

	playerMoveEnd();

	if (Input::MouseTrigger(MouseButton::RBUTTON))
	{
		SetPanel();
	}
}

void OthelloManager::KeySetPlayer()
{
	isFieldUpdate = false;

	if (othellos.size() <= 0)
	{
		return;
	}
	//どのマスにマウスがあるのかを確認
	int x = playerPanelPos.x;
	int y = playerPanelPos.y;


	bool D = Input::KeyTrigger(DIK_D);
	bool A = Input::KeyTrigger(DIK_A);
	bool S = Input::KeyTrigger(DIK_S);
	bool W = Input::KeyTrigger(DIK_W);

	if (D)
	{
		x++;
		if (x >= fieldSize)
		{
			x = fieldSize - 1;
		}
	}
	else if (A)
	{
		x--;
		if (x < 0)
		{
			x = 0;
		}
	}
	else if (S)
	{
		y++;
		if (y >= fieldSize)
		{
			y = fieldSize - 1;
		}
	}
	else if (W)
	{
		y--;
		if (y < 0)
		{
			y = 0;
		}
	}
	//動いていない
	if (x == playerPanelPos.x && y == playerPanelPos.y)
	{
		playerNotMove();
		return;
	}
	auto itr = othellos.begin();
	auto playerItr = othellos.end();
	auto nextItr = othellos.end();
	//移動先にパネルがあるかを検索
	for (; itr != othellos.end(); itr++)
	{
		int ax = itr->GetGameData()->widthPos;
		int ay = itr->GetGameData()->heightPos;
		if (itr->GetGameData()->widthPos == x && itr->GetGameData()->heightPos == y)
		{
			nextItr = itr;
		}
		if (itr->GetGameData()->widthPos == playerPanelPos.x && itr->GetGameData()->heightPos == playerPanelPos.y)
		{
			playerItr = itr;
		}
	}

	if (Imgui::sample == 0)
	{
		TypeXI(playerItr, nextItr, x, y);
	}
	else if (Imgui::sample == 1)
	{
		TypeB(playerItr, nextItr, x, y);
	}
	else if (Imgui::sample == 2)
	{
		TypeC(playerItr, nextItr, x, y);
	}
	else if (Imgui::sample == 3)
	{
		TypeD(playerItr, nextItr, x, y);
	}
	else
	{
		TypeA(playerItr, nextItr, x, y);
	}
}

void OthelloManager::StartSetPos()
{

	XMFLOAT3 startPos = OthlloPlayer::GetPosition();

	startPos.z = 0.0f;
	XMFLOAT3 stagePos = startPos - stageLeftTop - XMFLOAT3{ -cellScale / 2, cellScale / 2 , 0 };

	//どのマスにマウスがあるのかを確認
	int x = stagePos.x / (cellScale * 2);
	int y = stagePos.y / -(cellScale * 2);

	Othello data;
	data.Init(&oserroModel);

	bool randFront = rand() % 2;
	data.Spawn(NORMAL, x, y, randFront);
	data.GetGameData()->isMove = false;
	data.GetGameData()->isPlayer = false;
	othellos.push_back(data);

	playerPanelPos = { x, y };

	MinSpawn();
}


void OthelloManager::playerMoveEnd()
{
	if (OthlloPlayer::GetIsMoveEnd())
	{
		isPlayerEnd = true;
		isFieldUpdate = true;
	}
	bool isMove = isPanelMove && OthlloPlayer::GetIsMoveEnd();
	if (isMove)
	{
		isPanelMove = false;
	}
	auto itr = othellos.begin();

	//プレイヤーを探索
	for (; itr != othellos.end(); ++itr)
	{
		OthelloData *data = itr->GetGameData();
		if (data->isPlayer)
		{

			//itr->GetGameData()->isMove = isMove;
			if (OthlloPlayer::GetIsMoveEnd())
			{
				itr->GetGameData()->isPlayer = false;
				isFieldUpdate = true;
			}

			return;
		}
	}
	if (itr == othellos.end())
	{
		return;
	}

}

void OthelloManager::playerNotMove()
{
	OthlloPlayer::MoveCancel();
	XMFLOAT3 tmpPlayerPos = OthlloPlayer::GetPosition();

	float tmpx = (playerPanelPos.x * cellScale * 2);
	float tmpy = -(playerPanelPos.y * cellScale * 2);
	tmpPlayerPos.x = tmpx;
	tmpPlayerPos.y = tmpy;
	tmpPlayerPos = tmpPlayerPos + stageLeftTop;
	OthlloPlayer::SetPosition(tmpPlayerPos);
}

void OthelloManager::TypeA(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y)
{
	bool OnPlayer = playerItr != othellos.end();
	//移動先にパネルがあるか
	if (nextItr != othellos.end())
	{
		bool isNotMove = nextItr->GetGameData()->isReverce;
		if (isNotMove)
		{
			playerNotMove();
			return;
		}

		bool isOnVanish = nextItr->GetGameData()->isVanish;

		if (isOnVanish)
		{
			nextItr->GetGameData()->isPlayer = false;
		}
		else
		{
			nextItr->GetGameData()->isPlayer = true;
		}
		isPanelMove = false;
		nextItr->GetGameData()->isMove = false;

		if (OnPlayer)
		{
			playerItr->GetGameData()->isPlayer = false;
		}
		playerPanelPos = { x, y };
	}
	else
	{
		if (OnPlayer)
		{

			isPanelMove = true;
			if (!playerItr->GetGameData()->isVanish && !playerItr->GetGameData()->isReverce && !playerItr->GetGameData()->isSandwich && !playerItr->GetGameData()->isSpawn)
			{
				playerItr->GetGameData()->isPlayer = true;
			}
		}
		playerPanelPos = { x, y };
	}
	isPlayerEnd = false;
}

void OthelloManager::TypeB(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y)
{
	bool OnPlayer = playerItr != othellos.end();
	//移動先にパネルがあるか
	if (nextItr != othellos.end())
	{
		bool isNotMove = nextItr->GetGameData()->isReverce || nextItr->GetGameData()->isVanish;
		if (isNotMove)
		{
			playerNotMove();
			return;
		}

		isPanelMove = false;
		nextItr->GetGameData()->isMove = false;
		nextItr->GetGameData()->isPlayer = true;
		if (OnPlayer)
		{
			playerItr->GetGameData()->isPlayer = false;
		}
		playerPanelPos = { x, y };
	}
	else
	{
		if (OnPlayer)
		{
			isPanelMove = true;
			if (!playerItr->GetGameData()->isVanish && !playerItr->GetGameData()->isReverce && !playerItr->GetGameData()->isSandwich && !playerItr->GetGameData()->isSpawn)
			{
				playerItr->GetGameData()->isPlayer = true;
			}
		}
		playerPanelPos = { x, y };
	}
	isPlayerEnd = false;
}

void OthelloManager::TypeC(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y)
{
	bool OnPlayer = playerItr != othellos.end();
	//移動先にパネルがあるか
	if (nextItr != othellos.end())
	{
		bool isNotMove = (nextItr->GetGameData()->isReverce || nextItr->GetGameData()->isHarf);
		if (isNotMove)
		{
			playerNotMove();
			return;
		}

		bool isOnVanish = nextItr->GetGameData()->isVanish;

		if (isOnVanish)
		{
			nextItr->GetGameData()->isPlayer = false;
		}
		else
		{
			nextItr->GetGameData()->isPlayer = true;
		}
		isPanelMove = false;
		nextItr->GetGameData()->isMove = false;

		if (OnPlayer)
		{
			playerItr->GetGameData()->isPlayer = false;
		}
		playerPanelPos = { x, y };
	}
	else
	{
		if (OnPlayer)
		{
			isPanelMove = true;
			if (!playerItr->GetGameData()->isVanish && !playerItr->GetGameData()->isReverce && !playerItr->GetGameData()->isSandwich && !playerItr->GetGameData()->isSpawn)
			{
				playerItr->GetGameData()->isPlayer = true;
			}
		}
		playerPanelPos = { x, y };
	}
	isPlayerEnd = false;
}

void OthelloManager::TypeD(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y)
{
	nextItr->GetGameData()->isHarf = !nextItr->GetGameData()->isHarf;
	TypeC(playerItr, nextItr, x, y);
}

void OthelloManager::TypeXI(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y)
{
	bool OnPlayer = playerItr != othellos.end();
	//移動先にパネルがあるか
	if (nextItr != othellos.end())
	{
		float nextStep = 1.0f;
		float nowStep = 0.0f;
		if (OnPlayer)
		{
			nowStep = 1.0f;
		}
		if (nextItr->GetGameData()->isSpawn)
		{
			nextStep = 1.0f - (static_cast<float>(nextItr->GetGameData()->spawnTimer) / SpawnAnimationTimerMax);
			if (OnPlayer)
			{
				nowStep = 1.0f - static_cast<float>(playerItr->GetGameData()->spawnTimer) / SpawnAnimationTimerMax;
			}
		}
		if (nextItr->GetGameData()->isVanish)
		{
			nextStep = 1.0f - (static_cast<float>(nextItr->GetGameData()->vanishTimer) / vanishTimerMax);
			if (OnPlayer)
			{
				nowStep = 1.0f - static_cast<float>(playerItr->GetGameData()->vanishTimer) / vanishTimerMax;
			}
		}
		float stepSize = nextStep - nowStep;
		bool isStepUp = stepSize <= 0.7;
		bool isNotMove = (nextItr->GetGameData()->isReverce || !isStepUp);
		if (isNotMove)
		{
			playerNotMove();
			return;
		}

		bool isOnVanish = nextItr->GetGameData()->isVanish;

		isPanelMove = false;
		nextItr->GetGameData()->isMove = false;

		DownStepReset();
		playerPanelPos = { x, y };
	}
	else
	{
		if (OnPlayer)
		{
			bool isNotMovePanel = (playerItr->GetGameData()->isVanish || playerItr->GetGameData()->isSpawn);
			bool isNotDown = (downStepCount < downStepCountMax &&isNotMovePanel);
			if (isNotDown)
			{
				DownStep(playerItr);
				return;
			}
			isPanelMove = true;
			if (!playerItr->GetGameData()->isVanish && !playerItr->GetGameData()->isReverce && !playerItr->GetGameData()->isSandwich && !playerItr->GetGameData()->isSpawn)
			{
				playerItr->GetGameData()->isPlayer = true;
			}
		}
		DownStepReset();
		playerPanelPos = { x, y };
	}
	isPlayerEnd = false;
}

void OthelloManager::AllDeadPanel()
{
	auto itr = othellos.begin();

	//プレイヤーを探索
	for (; itr != othellos.end(); ++itr)
	{
		itr->GetGameData()->isDead = true;
	}

	DeadPanel();
}

void OthelloManager::DownStep(list<Othello>::iterator playerItr)
{
	playerNotMove();
	downStepCount++;
}

bool OthelloManager::GetIsSendDataUpdate()
{
	return isFieldUpdate;
}


void OthelloManager::SaveSpawn()
{
	auto itr = othellos.begin();
	auto playerItr = othellos.end();

	for (; itr != othellos.end(); itr++)
	{
		int ax = itr->GetGameData()->widthPos;
		int ay = itr->GetGameData()->heightPos;
		if (itr->GetGameData()->widthPos == playerPanelPos.x && itr->GetGameData()->heightPos == playerPanelPos.y)
		{
			playerItr = itr;
		}
	}
	bool isNotOnPlayer = (playerItr == othellos.end());
	if (isNotOnPlayer)
	{
		if (saveTimer >= saveTimerLimit)
		{
			Othello data;
			data.Init(&oserroModel);
			bool randFront = rand() % 2;
			int x = playerPanelPos.x;
			int y = playerPanelPos.y;
			data.Borne(NORMAL, x, y, randFront);
			data.Update();
			othellos.push_back(data);
			saveTimer = 0;
			isFieldUpdate = true;
		}
		else
		{
			saveTimer++;
		}
	}
	else
	{
		saveTimer = 0;
	}
}

void OthelloEachInfo::CreateConstBuff0()
{
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(OthelloConstBuffer) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff0));
}

void OthelloModel::Update(OthelloEachInfo *each)
{
	if (each != nullptr)
	{
		this->each = *each;
		XMMATRIX matScale, matRot, matTrans;
		const XMFLOAT3 &cameraPos = Camera::eye.v;
		matScale = XMMatrixScaling(this->each.scale.x, this->each.scale.y, this->each.scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(this->each.rotation.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(this->each.rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(this->each.rotation.y));
		matTrans = XMMatrixTranslation(this->each.position.m128_f32[0], this->each.position.m128_f32[1], this->each.position.m128_f32[2]);
		matWorld = XMMatrixIdentity();

		//ビルボード
		//if (billboard)
		//{
		//    matWorld *= BaseDirectX::matBillboard;//ビルボードをかける
		//}
		//ビルボードY
		//if (billboard)
		//{
		//    matWorld *= Camera::matBillboardY;//ビルボードをかける
		//}
		matWorld *= matScale;
		matWorld *= matRot;
		matWorld *= matTrans;

		Vertex *vertMap = nullptr;
		BaseDirectX::result = mesh.vertBuff->Map(0, nullptr, (void **)&vertMap);
		if (SUCCEEDED(BaseDirectX::result))
		{
			copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
			mesh.vertBuff->Unmap(0, nullptr);    // マップを解除
		}

		OthelloConstBuffer *constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void **)&constMap0)))
		{
			//constMap0->mat = matWorld * Camera::matView * BaseDirectX::matProjection;
			constMap0->viewproj = Camera::matView * BaseDirectX::matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = cameraPos;
			constMap0->flash = each->alpha;
			this->each.constBuff0->Unmap(0, nullptr);
		}

		ConstBufferDataB1 *constMap1 = nullptr;
		BaseDirectX::result = this->each.constBuff1->Map(0, nullptr, (void **)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);

	}
	else
	{
		XMMATRIX matScale, matRot, matTrans;
		const XMFLOAT3 &cameraPos = Camera::eye.v;
		matScale = XMMatrixScaling(this->each.scale.x, this->each.scale.y, this->each.scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(this->each.rotation.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(this->each.rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(this->each.rotation.y));
		matTrans = XMMatrixTranslation(this->each.position.m128_f32[0], this->each.position.m128_f32[1], this->each.position.m128_f32[2]);
		matWorld = XMMatrixIdentity();

		//ビルボード
		//if (billboard)
		//{
		//    matWorld *= BaseDirectX::matBillboard;//ビルボードをかける
		//}
		//ビルボードY
		//if (billboard)
		//{
		//    matWorld *= Camera::matBillboardY;//ビルボードをかける
		//}
		matWorld *= matScale;
		matWorld *= matRot;
		matWorld *= matTrans;

		Vertex *vertMap = nullptr;
		BaseDirectX::result = mesh.vertBuff->Map(0, nullptr, (void **)&vertMap);
		if (SUCCEEDED(BaseDirectX::result))
		{
			copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
			mesh.vertBuff->Unmap(0, nullptr);    // マップを解除
		}

		OthelloConstBuffer *constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void **)&constMap0)))
		{
			//constMap0->mat = matWorld * Camera::matView * BaseDirectX::matProjection;
			constMap0->viewproj = Camera::matView * BaseDirectX::matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = cameraPos;
			this->each.constBuff0->Unmap(0, nullptr);
		}

		ConstBufferDataB1 *constMap1 = nullptr;
		BaseDirectX::result = this->each.constBuff1->Map(0, nullptr, (void **)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);
	}
}
