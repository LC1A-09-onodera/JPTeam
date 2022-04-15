#include "Oserro.h"
#include "../Shader/ShaderManager.h"
#include "../Camera/Camera.h"
#include "../BaseDirectX/Input.h"
#include "../3DObjectParticle/3DObjectParticle.h"
#include "../OthlloPlayer/OthlloPlayer.h"
#include "../Thunder/Thunder.h"
#include "../imgui/ImguiControl.h"
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
	float x = (data.widthPos * cellScale * 2);
	float y = -(data.heightPos * cellScale * 2);



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
		}
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

void Othello::Revers()
{
	data.isReverce = true;
	data.isPlayer = false;
	data.isSandwich = true;
	data.animationTimer = 0;
	data.waitTimer = waitTimerMax * (data.comboCount - 1);
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
	const float jumpMax = 3.0f + (1 * count);

	if (count >= 5)
	{
		count = 5;
	}
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
		data.isMove = true;

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
		float a = static_cast<float>(data.vanishTimer) / vanishTimerMax;
		data.isHarf = (a >= 0.5f);
		each.scale = { 1 - a, 1 - a, 1.0f };
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
	return (data.isVanish ||data.isSandwich);
}

void OthelloManager::Init()
{
	oserroModel.CreateModel("newOserro", ShaderManager::playerShader);


	sendDatas.resize(fieldSize);

	StartSetPos();
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
		if (gameDatas.isPlayer || gameDatas.isReverce)
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


			data.isMove = itr->GetIsActive();

		bool isOnPlayer = (gameDatas.widthPos == playerPanelPos.x && gameDatas.heightPos == playerPanelPos.y);
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
			gameDatas->isMove = true;
			gameDatas->score = sendDatas[y][x].score;
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

	for (int i = 0; i < spawnPanelCount; i++)
	{
		//フィールドがパネルで満たされているか
		bool isPanelMax = othellos.size() >= 64;
		if (isPanelMax)
		{
			return;
		}


		SpawnPanel();
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


void OthelloManager::SpawnPanel()
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
	data.Spawn(NORMAL, x, y, randFront);
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
		SpawnPanel();
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
		TypeA(playerItr, nextItr, x, y);
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
		TypeXI(playerItr, nextItr, x, y);
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
	data.GetGameData()->isFront = true;
	othellos.push_back(data);

	playerPanelPos = { x, y };

	SpawnPanel();
}


void OthelloManager::playerMoveEnd()
{
	if (OthlloPlayer::GetIsMoveEnd())
	{
		isPlayerEnd = true;
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
			if (!playerItr->GetGameData()->isVanish && !playerItr->GetGameData()->isReverce && !playerItr->GetGameData()->isSandwich)
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
			if (!playerItr->GetGameData()->isVanish && !playerItr->GetGameData()->isReverce && !playerItr->GetGameData()->isSandwich)
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
			if (!playerItr->GetGameData()->isVanish && !playerItr->GetGameData()->isReverce && !playerItr->GetGameData()->isSandwich)
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
		float nextStep = 1.0f - (static_cast<float>(nextItr->GetGameData()->vanishTimer) / vanishTimerMax);
		float nowStep = 1.0f;
		if (OnPlayer)
		{
			nowStep = 1.0f - static_cast<float>(playerItr->GetGameData()->vanishTimer) / vanishTimerMax;
		}
		else
		{
			nowStep = 0.0f;
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
			if (!playerItr->GetGameData()->isVanish && !playerItr->GetGameData()->isReverce && !playerItr->GetGameData()->isSandwich)
			{
				playerItr->GetGameData()->isPlayer = true;
			}
		}
		playerPanelPos = { x, y };
	}
	isPlayerEnd = false;
}
