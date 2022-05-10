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
list<NormaModeFieldData> OthelloManager::NormaStartOthellos;
list<ChanceObject> OthelloManager::chances;
OthelloModel OthelloManager::oserroModel;
ChanceModel OthelloManager::chanceModelBlue;
ChanceModel OthelloManager::chanceModelOrange;
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
		float nowScale = each.scale.z;
		if (nowScale > PanelSize * panelWallRate || !data.isVanish)
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
	data.isFront = !data.isFront;
	XMFLOAT3 pos = ConvertXMVECTORtoXMFLOAT3(each.position);
	pos.z = -0.3f;
}


void Othello::Sandwich()
{
	data.isPlayer = false;
	data.isSandwich = true;
	data.waitTimer = animationTimerMax;
	//data.isReverce = true;
	data.animationTimer = 0;
	//data.waitTimer = 0;
	//data.JumpTimer = 0;
	//data.isJumpUp = true;
	XMFLOAT3 pos = ConvertXMVECTORtoXMFLOAT3(each.position);
	pos.z = -0.3f;

}

void Othello::SpawnUpdate()
{

	if (data.spawnTimer >= SpawnAnimationTimerMax)
	{
		data.isSpawn = false;
	}

	float nowScale = static_cast<float>(data.spawnTimer) / SpawnAnimationTimerMax;
	data.isHarf = (nowScale >= 0.5f);
	each.scale = { nowScale , nowScale , 1 };//通常拡縮
	//each.scale = { 1.0f , 1.0f ,  PanelSize * nowScale };
	if (data.spawnDerayTimer <= SpawnDerayTimerMax)
	{
		data.spawnDerayTimer++;
	}
	else
	{
		data.spawnTimer++;
	}
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
	float easeRate = ShlomonMath::EaseInOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, rate).x;
	if (data.isFront)
	{
		each.rotation.z = 3600.0f * easeRate;
		each.rotation.x = 0;
	}
	else
	{
		each.rotation.z = 3600.0f * easeRate + 180.0f;
		each.rotation.x = 0;
	}

	each.scale.z = easeRate * PanelSize;
	data.JumpTimer++;

	int count = data.comboCount;
	if (count >= 4)
	{
		count = 4;
	}
	const float jumpMax = 3.0f + (1 * count);


	//float jumpRate = static_cast<float>(data.JumpTimer) / JumpTimerMax;
	//float jumpEaseRate = 0.0f;
	//if (data.isJumpUp)
	//{
	//	jumpEaseRate = ShlomonMath::EaseOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, jumpRate).x;

	//	float hight = pow(jumpEaseRate, count);
	//	each.position.m128_f32[2] = -jumpMax * hight;
	//	if (data.JumpTimer >= JumpTimerMax)
	//	{
	//		data.isJumpUp = false;
	//		data.JumpTimer = 0;
	//	}
	//}
	//else
	//{
	//	jumpEaseRate = ShlomonMath::EaseInQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, jumpRate).x;
	//	float hight = 1 - (pow(jumpEaseRate, count));
	//	each.position.m128_f32[2] = -jumpMax * hight;
	//}

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
	if (rate >= 1.0f)
	{
		//回転フラグoff
		//data.isFront = !data.isFront;
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
	float easeRate = ShlomonMath::EaseInOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, rate).x;
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
	XMVECTOR PlayerPos = { data.widthPos * cellScale,data.heightPos * -cellScale , 0, 0 };

	PlayerPos += ConvertXMFLOAT3toXMVECTOR(stageLeftTop);
	XMVECTOR dist = ConvertXMFLOAT3toXMVECTOR(mousePos - PlayerPos);
	XMVECTOR angle = XMVector3Normalize(dist);


	bool isOver = false;
	while (true)
	{

		PlayerPos = { data.widthPos * cellScale * 2,data.heightPos * -cellScale * 2, 0, 0 };
		PlayerPos += ConvertXMFLOAT3toXMVECTOR(stageLeftTop);

		dist = ConvertXMFLOAT3toXMVECTOR(mousePos - PlayerPos);
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
			if (fabs(angle.m128_f32[0]) > fabs(angle.m128_f32[1]))
			{//横のほうが長かったら
				//左右どちらか
				float length = dist.m128_f32[0];
				if (fabs(length) >= 1.0f)
				{
					length /= fabs(length);
				}
				PlayerPos.m128_f32[0] += length;
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
				PlayerPos.m128_f32[1] += length;
				//上下どちらか
				each.rotation.x = length * 90;
				each.rotation.y = 0;
			}
		}
		else
		{
			if (fabs(angle.m128_f32[0]) > fabs(angle.m128_f32[1]))
			{//横のほうが長かったら
				//左右どちらか
				float length = dist.m128_f32[0];
				if (fabs(length) >= 1.0f)
				{
					length /= fabs(length);
				}
				PlayerPos.m128_f32[0] += length;
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
				PlayerPos.m128_f32[1] += length;
				//上下どちらか
				each.rotation.x = length * 90 + 180;
				each.rotation.y = 0;
			}

		}
	}
	each.position = PlayerPos;
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
		//each.scale = { 1.0f , 1.0f ,  PanelSize };
		each.scale = { 1.0f , 1.0f , 1.0f };
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
		XMFLOAT3 pos = ConvertXMVECTORtoXMFLOAT3(each.position);
		ObjectParticles::frame.Init(pos, 1, ParticleType::Born);
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
		data.animationTimer++;
		float rate = static_cast<float>(data.animationTimer) / animationTimerMax;
		float easeRate = ShlomonMath::EaseInOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, rate).x;
		if (data.isFront)
		{
			each.rotation.z = 3600.0f * easeRate;
			each.rotation.x = 0;
		}
		else
		{
			each.rotation.z = 3600.0f * easeRate + 180.0f;
			each.rotation.x = 0;
		}

		each.scale.z = easeRate * PanelSize;
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
		//each.scale = { 1 - nowScale, 1 - nowScale, 1.0f };
		each.scale = { 1.0f, 1.0f, PanelSize * (1 - nowScale) };
	}
}

void Othello::MakeParticle()
{
	XMFLOAT3 sample;
	sample = ConvertXMVECTORtoXMFLOAT3(each.position);
	ObjectParticles::triangle.Init(sample, 10, ParticleType::Exprotion);
	//ObjectParticles::triangle.Init(XMFLOAT3(0, 0, 0), 60, ParticleType::Swell);
}

bool Othello::GetIsActive()
{
	return (data.isVanish || data.isSandwich);
}

void OthelloManager::Init()
{
	oserroModel.CreateModel("newOserro", ShaderManager::othelloShader);
	chanceModelBlue.CreateModel("chance_1", ShaderManager::othelloShader);
	chanceModelOrange.CreateModel("chance", ShaderManager::othelloShader);
	sendDatas.resize(fieldSize);

	auto itr = sendDatas.begin();
	for (; itr != sendDatas.end(); itr++)
	{
		itr->resize(fieldSize);
	}

	TutorialText1.CreateSprite(L"Resource/Img/string_0.png", XMFLOAT3(0, 0, 0));
	TutorialText2.CreateSprite(L"Resource/Img/string_01.png", XMFLOAT3(0, 0, 0));
	TutorialText3.CreateSprite(L"Resource/Img/string_02.png", XMFLOAT3(0, 0, 0));
	TutorialText4.CreateSprite(L"Resource/Img/string_03.png", XMFLOAT3(0, 0, 0));
	TutorialText5.CreateSprite(L"Resource/Img/string_04.png", XMFLOAT3(0, 0, 0));
	TutorialText6.CreateSprite(L"Resource/Img/string_05.png", XMFLOAT3(0, 0, 0));
	CongraturationText.CreateSprite(L"Resource/Img/excellent.png", XMFLOAT3(0, 0, 0));
	TutorialRetryText.CreateSprite(L"Resource/Img/reset.png", XMFLOAT3(0, 0, 0));
	back.CreateSprite(L"Resource/Img/title_back_80.png", XMFLOAT3(0, 0, 0));
	float changeScale = 0.5f;

	TutorialText1.ChangeSize(1227 * changeScale, 332 * changeScale);
	TutorialText2.ChangeSize(1158 * changeScale, 207 * changeScale);
	TutorialText3.ChangeSize(1158 * changeScale, 207 * changeScale);
	TutorialText4.ChangeSize(971 * changeScale, 82 * changeScale);
	TutorialText5.ChangeSize(910 * changeScale, 207 * changeScale);
	TutorialText6.ChangeSize(1070 * changeScale / 2, 154 * changeScale / 2);
	CongraturationText.ChangeSize(457 * changeScale, 79 * changeScale);
	TutorialRetryText.ChangeSize(288 * changeScale, 231 * changeScale);

	TutorialText1.position = XMVECTOR{ 640 - (1227 * changeScale / 2), 720 - (332 * changeScale + 30), 0, 0 };
	TutorialText2.position = XMVECTOR{ 640 - (1158 * changeScale / 2), 720 - (207 * changeScale + 30), 0, 0 };
	TutorialText3.position = XMVECTOR{ 640 - (1158 * changeScale / 2) , 720 - (207 * changeScale + 30), 0, 0 };
	TutorialText4.position = XMVECTOR{ 640 - (971 * changeScale / 2) , 720 - (82 * changeScale + 60), 0, 0 };
	TutorialText5.position = XMVECTOR{ 640 - (910 * changeScale / 2) , 720 - (207 * changeScale + 30), 0, 0 };
	TutorialText6.position = XMVECTOR{ 1280 - (1070 * changeScale / 2 + 15) , 720 - (77 * changeScale + 15), 0, 0 };

	CongraturationText.position = XMVECTOR{ 640 - (457 * changeScale / 2), 360 - (79 * changeScale / 2), 0, 0 };

	TutorialRetryText.position = XMVECTOR{ 990, 300, 0, 0 };
	normaChecker.Init();
	TestStage();
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
	if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
	{
		isFieldUpdate = true;
	}


	SaveSpawn();

}

void OthelloManager::TutorialUpdate()
{
	//死ぬ
	DeadPanel();

	//更新
	auto itr = othellos.begin();
	int panelCount = 0;
	int comboMax = 0;
	bool tutorialOnPlayer = false;
	for (; itr != othellos.end(); ++itr)
	{
		itr->Update();
		if (!itr->GetIsActive())
		{
			panelCount++;
		}
		if (itr->GetGameData()->comboCount > comboMax)
		{
			comboMax = itr->GetGameData()->comboCount;
		}

		if (!tutorialOnPlayer)
		{
			tutorialOnPlayer = (itr->GetGameData()->widthPos == playerPanelPos.x && itr->GetGameData()->heightPos == playerPanelPos.y);
		}
	}
	if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
	{
		isFieldUpdate = true;
	}
	bool retry = (Input::KeyTrigger(DIK_R) || directInput->IsButtonPush(directInput->Button03));


	switch (scenes)
	{
	case TutorialSceneFlow::SandwichSpawn:
		break;

	case TutorialSceneFlow::SandwichUpdate:
		//全部消したらチュートリアル１クリア
		if (panelCount <= 0)
		{
			isTutorialClear = true;
		}

		//チュートリアルクリアしたらタイマースタート
		if (isTutorialClear)
		{
			TutorialTimer++;
		}

		//タイマー分経過したら
		if (TutorialTimer >= tutorialTimerLimit)
		{
			scenes = TutorialSceneFlow::ChainSpawn;
		}
		if (retry)
		{
			whySandwichSpawn();
		}
		break;


	case TutorialSceneFlow::ChainSpawn:
		whyChainSpawn();
		break;

	case TutorialSceneFlow::ChainUpdate:
		if (comboMax >= 2)
		{
			isTutorialClear = true;
		}

		//チュートリアルクリアしたらタイマースタート
		if (isTutorialClear)
		{
			TutorialTimer++;
		}

		//タイマー分経過したら
		if (TutorialTimer >= tutorialTimerLimit)
		{
			scenes = TutorialSceneFlow::StepSpawn;
		}
		if (retry)
		{
			whyChainSpawn();
		}
		break;

	case TutorialSceneFlow::StepSpawn:
		whyStepSpawn();
		break;

	case TutorialSceneFlow::StepUpdate:
		RandumSetPanel();
		//SaveSpawn();

		TutorialEndTextCount++;
		if (tutorialOnPlayer)
		{
			//isTutorialClear = true;
		}

		if (isTutorialClear)
		{
			TutorialTimer++;
		}
		//チュートリアルクリアしたらタイマースタート
		if (TutorialTimer >= tutorialTimerLimit)
		{
			//scenes = TutorialSceneFlow::TutorialEnd;
		}

		if (retry)
		{
			whyStepSpawn();
		}
		break;
	case TutorialSceneFlow::TutorialEnd:
		break;
	default:
		break;
	}
}

void OthelloManager::TutorialTextDraw()
{

	if (textChangeTimer >= 300)
	{
		textChangeTimer = 0;
		textChange = !textChange;
	}
	//やり直しテキスト描画
	TutorialRetryText.SpriteDraw();

	switch (scenes)
	{
	case TutorialSceneFlow::SandwichSpawn:
		break;
	case TutorialSceneFlow::SandwichUpdate:
		back.position.m128_f32[1] = 500;
		back.ChangeSize(1280, 280);
		back.SpriteDraw();
		if (isTutorialClear)
		{
			CongraturationText.SpriteDraw();
		}
		else
		{
			TutorialText1.SpriteDraw();
		}

		//テキストの描画
		break;
	case TutorialSceneFlow::ChainSpawn:
		break;
	case TutorialSceneFlow::ChainUpdate:
		//テキストの描画
		back.position.m128_f32[1] = 550;
		back.ChangeSize(1280, 280);
		back.SpriteDraw();
		if (isTutorialClear)
		{
			CongraturationText.SpriteDraw();
		}
		else
		{
			textChangeTimer++;
			if (textChange)
			{
				TutorialText3.SpriteDraw();
			}
			else
			{
				TutorialText2.SpriteDraw();
			}
		}

		break;
	case TutorialSceneFlow::StepSpawn:
		break;
	case TutorialSceneFlow::StepUpdate:
		back.position.m128_f32[1] = 550;
		back.SpriteDraw();
		if (isTutorialClear)
		{
			CongraturationText.SpriteDraw();
		}
		else
		{
			textChangeTimer++;
			if (textChange)
			{
				TutorialText5.SpriteDraw();
			}
			else
			{
				TutorialText4.SpriteDraw();
			}
			if (TutorialEndTextCount >= TutorialEndTextTimer)
			{
				TutorialText6.SpriteDraw();
			}
		}

		//テキストの描画
		break;
	case TutorialSceneFlow::TutorialEnd:
		break;
	default:
		break;
	}
}

void OthelloManager::NormaUpdate()
{
	//死ぬ
	DeadPanel();

	//MinSpawn();
	////生成
	//RandumSetPanel();

	//更新
	auto itr = othellos.begin();

	for (; itr != othellos.end(); ++itr)
	{
		itr->Update();
	}
	if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
	{
		isFieldUpdate = true;
	}
	Undo();
	normaChecker.Update(othellos);
}
void OthelloManager::Draw()
{
	OthelloDraw();
	//ChanceDraw();
}

void OthelloManager::OthelloDraw()
{
	if (othellos.size() == 0) return;

	auto itr = othellos.begin();
	for (; itr != othellos.end(); ++itr)
	{
		itr->Draw();
	}
}

void OthelloManager::ChanceDraw()
{
	if (chances.size() == 0) return;

	auto itr = chances.begin();
	for (; itr != chances.end(); ++itr)
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

		if ((Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01)) && isOnPlayer)
		{
			data.isMove = true;
			SetNormaMove();
		}

		data.comboCount = gameDatas.comboCount;
		data.SandwichLength = gameDatas.SandwichLength;
		data.score = gameDatas.score;
		data.chainName = gameDatas.chainName;
		data.maxComboCount = gameDatas.maxComboCount;
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

		gameDatas->score = sendDatas[y][x].score;

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
		int x = repaierOthelloDataItr->GetGameData()->widthPos;
		int y = repaierOthelloDataItr->GetGameData()->heightPos;
		if (isSandFlip)
		{
			int timer = repaierOthelloDataItr->GetGameData()->vanishTimer;

			if (timer >= chainRepairTime)
			{
				timer -= chainRepairTime;
				repaierOthelloDataItr->GetGameData()->comboCount = sendDatas[y][x].comboCount;
				repaierOthelloDataItr->GetGameData()->score = sendDatas[y][x].score;
				repaierOthelloDataItr->GetGameData()->maxComboCount = sendDatas[y][x].maxComboCount;
				repaierOthelloDataItr->GetGameData()->chainName = sendDatas[y][x].chainName;

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
	bool padD = directInput->IsButtonPush(DirectInput::ButtonKind::RightButton);
	bool padS = directInput->IsButtonPush(DirectInput::ButtonKind::DownButton);
	bool padA = directInput->IsButtonPush(DirectInput::ButtonKind::LeftButton);
	bool padW = directInput->IsButtonPush(DirectInput::ButtonKind::UpButton);

	if (D || padD)
	{
		x++;
		if (x >= fieldSize)
		{
			x = fieldSize - 1;
		}
	}
	else if (A || padA)
	{
		x--;
		if (x < 0)
		{
			x = 0;
		}
	}
	else if (S || padS)
	{
		y++;
		if (y >= fieldSize)
		{
			y = fieldSize - 1;
		}
	}
	else if (W || padW)
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
		//TypeXI(playerItr, nextItr, x, y);
		TypeUp(playerItr, nextItr, x, y);
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

	for (int i = 0; i < 8; i++)
	{
		SetChanceObject(i, i, true);
		SetChanceObject(i, 7 - i, false);
	}
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
		bool isStepUp = stepSize <= 0.7f;
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
			bool isNotMovePanel = (playerItr->GetGameData()->isVanish || playerItr->GetGameData()->isSpawn || playerItr->GetGameData()->isSandwich);
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

void OthelloManager::TypeUp(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y)
{
	bool OnPlayer = playerItr != othellos.end();
	//移動先にパネルがあるか
	if (nextItr != othellos.end())
	{
		float nextStep = 0.0f;
		float nowStep = 0.0f;
		if (OnPlayer)
		{
			nowStep = 0.0f;
		}

		//if (nextItr->GetGameData()->isSpawn)
		//{
		//	nextStep = 1.0f - (static_cast<float>(nextItr->GetGameData()->spawnTimer) / SpawnAnimationTimerMax);
		//	if (OnPlayer)
		//	{
		//		nowStep = 1.0f - static_cast<float>(playerItr->GetGameData()->spawnTimer) / SpawnAnimationTimerMax;
		//	}
		//}
		if (nextItr->GetGameData()->isVanish)
		{
			nextStep = 1.0f - (static_cast<float>(nextItr->GetGameData()->vanishTimer) / vanishTimerMax);
			if (OnPlayer && playerItr->GetIsActive())
			{
				nowStep = 1.0f - static_cast<float>(playerItr->GetGameData()->vanishTimer) / vanishTimerMax;
			}
		}
		float stepSize = nextStep - nowStep;
		bool isStepUp = stepSize <= panelWallRate;
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
			bool isNotMovePanel = (playerItr->GetGameData()->isVanish || playerItr->GetGameData()->isSpawn || playerItr->GetGameData()->isSandwich);
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
				SetNormaMove();
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

void OthelloManager::whySandwichSpawn()
{
	AllDeadPanel();

	SetPlayerAndPanel(2, 4, false);
	SetSpawnPanel(4, 4, false);
	SetSpawnPanel(5, 4, true);

	scenes = TutorialSceneFlow::SandwichUpdate;
	TutorialTimer = 0;
	isTutorialClear = false;
}

void OthelloManager::whyChainSpawn()
{
	AllDeadPanel();

	SetSpawnPanel(4, 2, true);
	SetSpawnPanel(4, 3, false);
	SetPlayerAndPanel(3, 4, true);
	SetSpawnPanel(4, 4, false);
	SetSpawnPanel(5, 4, true);

	scenes = TutorialSceneFlow::ChainUpdate;
	TutorialTimer = 0;
	isTutorialClear = false;

}

void OthelloManager::whyStepSpawn()
{
	AllDeadPanel();

	SetSpawnPlayer(4, 5);
	SetSpawnPanel(3, 4, true);
	SetSpawnPanel(4, 4, false);
	SetSpawnPanel(5, 4, true);

	scenes = TutorialSceneFlow::StepUpdate;
	TutorialTimer = 0;
	TutorialEndTextCount = 0;
	isTutorialClear = false;

}

void OthelloManager::SetSpawnPanel(int x, int y, bool Front)
{
	Othello data;
	data.Init(&oserroModel);

	data.Spawn(NORMAL, x, y, Front);

	data.Update();
	othellos.push_back(data);
}

void OthelloManager::SetSpawnPlayer(int x, int y)
{
	OthlloPlayer::MoveCancel();
	XMFLOAT3 tmpPlayerPos = OthlloPlayer::GetPosition();

	float tmpx = (x * cellScale * 2);
	float tmpy = -(y * cellScale * 2);
	tmpPlayerPos.x = tmpx;
	tmpPlayerPos.y = tmpy;
	tmpPlayerPos = tmpPlayerPos + stageLeftTop;

	playerPanelPos = { x, y };
	OthlloPlayer::SetPosition(tmpPlayerPos);
}

void OthelloManager::SetPlayerAndPanel(int x, int y, bool Front)
{
	SetSpawnPanel(x, y, Front);
	SetSpawnPlayer(x, y);
}

bool OthelloManager::IsTutorialEnd()
{
	return (scenes == TutorialSceneFlow::TutorialEnd);
}

void OthelloManager::SetChanceObject(int x, int y, bool Front)
{
	ChanceObject data;
	data.Init(&chanceModelBlue);
	if (Front)
	{
		data.SetModel(&chanceModelBlue);
	}
	else
	{
		data.SetModel(&chanceModelOrange);
	}
	data.Spawn(x, y, Front);
	chances.push_back(data);
}

void ChanceObject::Init(ChanceModel *model)
{
	SetModel(model);
	each.CreateConstBuff0();
	each.CreateConstBuff1();
}
void ChanceObject::Update()
{
	//アニメーションの内容による何かしらを記述
}
void ChanceObject::Draw()
{
	if (model == nullptr) { return; }

	model->Update(&each);
	Draw3DObject(*model);
}
void ChanceObject::Finalize()
{

}
void ChanceObject::Spawn(int x, int y, bool isFront)
{

	float posX = (x * cellScale * 2);
	float posY = -(y * cellScale * 2);

	each.position = XMVECTOR{ posX, posY ,0, 0 };
	each.scale = { 1.0f , 1.0f ,  1.0f };
	each.position += ConvertXMFLOAT3toXMVECTOR(stageLeftTop);
	each.alpha = 0.4f;
	if (isFront)
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

void OthelloManager::Undo()
{
	if (Input::KeyTrigger(DIK_Z) || directInput->IsButtonPush(directInput->Button01))
	{
		Norma::FieldStatus data = normaChecker.Undo();

		//送られてきた情報がこれ以上戻ることができない場合盤面をいじらず戻る
		if (data.isFirst)
		{
			return;
		}
		//パネルを全て消去してから
		othellos.clear();
		auto panelItr = data.othellos.begin();
		for (; panelItr != data.othellos.end(); panelItr++)
		{
			SetSpawnPanel(panelItr->x, panelItr->y, panelItr->isFront);
		}

		SetSpawnPlayer(data.playerPos.x, data.playerPos.y);
		//Update();
	}
}

void OthelloManager::SetNormaMove()
{
	if (!isNormaMode) return;
	normaChecker.SetMove(othellos, playerPanelPos.x, playerPanelPos.y);
}

void OthelloManager::StartNormaMode(Norma::NormaType normaType, int normaStatus, int normaMoveCount)
{
	normaChecker.Reset();
	StartNormaField();
	isNormaMode = true;
}

void OthelloManager::StartNormaField()
{
	if (NormaStartOthellos.size() <= 0)return;

	//ノルマ番号orステージ番号を決定してその盤面を呼び出す処理(Todo)
	{}

	//ステージ番号に応じたイテレーターを呼び出す
	auto stageItr = NormaStartOthellos.begin();

	//パネルが一つもなかったら早期リターン
	if (stageItr->panels.size() <= 0)
	{
		//とりあえず速攻終了する条件ノルマモードを終わらせる
		normaChecker.SetNorma(Norma::Panels, 0);
		SetSpawnPlayer(4, 4);
		return;
	}
	auto panelItr = stageItr->panels.begin();
	for (; panelItr != stageItr->panels.end(); panelItr++)
	{
		SetSpawnPanel(panelItr->pos.x, panelItr->pos.y, panelItr->isFront);
	}
	SetSpawnPlayer(stageItr->playerPos.x, stageItr->playerPos.y);
	normaChecker.SetNorma(stageItr->type, stageItr->normaStatus, stageItr->normaMoveCount);

}

void OthelloManager::TestStage()
{
	panelData a, b, c;
	NormaModeFieldData testStage;

	a.isFront = true;
	a.pos = { 0, 0 };

	testStage.panels.push_back(a);
	b.isFront = false;
	b.pos = { 1, 1 };
	testStage.panels.push_back(b);

	c.isFront = true;
	c.pos = { 2, 2 };
	testStage.panels.push_back(c);

	testStage.playerPos = { 0, 0 };
	testStage.normaMoveCount = 0;
	NormaStartOthellos.push_back(testStage);
}

void OthelloManager::EndNormaMode()
{
	isNormaMode = false;
	normaChecker.Reset();
}

void OthelloManager::RestartNorma()
{
	normaChecker.Reset();
}

bool OthelloManager::GetIsNormaClear()
{
	return normaChecker.GetClear();
}

bool OthelloManager::GetIsNormaFailed()
{
	return normaChecker.GetFailed();
}
