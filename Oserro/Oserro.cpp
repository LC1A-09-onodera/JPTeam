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
vector<ChanceObject> OthelloManager::chances;
OthelloModel OthelloManager::oserroModel;
OthelloModel OthelloManager::stopOserroModel;
OthelloModel OthelloManager::wallOserroModel;
OthelloModel OthelloManager::compModel;
TextModel OthelloManager::PanelTextModel;
TextModel OthelloManager::ComboTextModel;
TextModel OthelloManager::ScoreTextModel;
TextModel OthelloManager::ScoreAttackTextModel;
TextModel OthelloManager::NormaModeTextModel;
TextModel OthelloManager::ConnectModel;
TextModel OthelloManager::WaltzModel;
TextModel OthelloManager::BookendModel;
TextModel OthelloManager::TechTrainingModel;
TextModel OthelloManager::UndoModel;
ChanceModel OthelloManager::chanceModelBlue;
ChanceModel OthelloManager::chanceModelOrange;
vector<vector<SendOthelloData>> OthelloManager::sendDatas;
using namespace OthelloConstData;

using namespace OthelloEase;
OthelloData::OthelloData()
{
	for (int i = 0; i < 8; i++)
	{
		FrontActiveAngle.push_back(true);
	}
}

void Othello::Init(OthelloModel *model, OthelloModel *chainModel, OthelloModel *compModel)
{
	this->model = model;
	this->chainModel = chainModel;
	this->compModel = compModel;
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	completeEach.CreateConstBuff0();
	completeEach.CreateConstBuff1();
	float size = 0.1f;
	completeEach.scale = { size,size ,size };
	completeEach.alpha = 1.0f;
	completeEach.rotation.x = 60.0f;

	compEase.Set(0, 120);
}

void Othello::Update(int combo)
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
			ReversUpdate(combo);
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
				completeEach.position = each.position;
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

		if (data.type == STOP && !GetIsActive() && isRockDraw)
		{
			chainModel->Update(&each);
			Draw3DObject(*chainModel);
		}
	}
}

void Othello::CompDraw()
{
	//回転変更
	if (isCompleteDraw)
	{
		float alpha = 0.3f;
		alpha += WaveInOut(compEase, 2) * 0.7;
		completeEach.position.m128_f32[2] = -1.0f;
		completeEach.alpha = alpha;
		compModel->Update(&completeEach);
		Draw3DObject(*compModel);
	}
}
void Othello::Finalize()
{
}

void Othello::Revers()
{
	data.isReverce = true;
	data.isSpawn = false;
	data.isPlayer = false;
	data.isSandwich = true;
	data.animationTimer = 0;
	data.waitTimer = 0;
	data.JumpTimer = 0;
	data.isJumpUp = true;
	data.isFront = !data.isFront;
	XMFLOAT3 pos = ConvertXMVECTORtoXMFLOAT3(each.position);
	pos.z = -0.3f;
	ObjectParticles::six.Init(pos, data.maxComboCount, ParticleType::Exprotion);
	each.scale = { 1.0f,1.0f ,1.0f };
}


void Othello::Sandwich()
{
	data.isPlayer = false;
	data.isSandwich = true;
	data.isSpawn = false;
	data.waitTimer = animationTimerMax;
	//data.isReverce = true;
	data.animationTimer = 0;
	//data.waitTimer = 0;
	//data.JumpTimer = 0;
	//data.isJumpUp = true;
	each.scale = { 1.0f,1.0f ,1.0f };
	XMFLOAT3 pos = ConvertXMVECTORtoXMFLOAT3(each.position);
	ObjectParticles::six.Init(pos, data.maxComboCount, ParticleType::Exprotion);

}

void Othello::SpawnUpdate()
{

	if (data.spawnTimer >= spawnAnimationTimerMax)
	{
		data.isSpawn = false;
	}

	float nowScale = static_cast<float>(data.spawnTimer) / spawnAnimationTimerMax;
	data.isHarf = (nowScale >= 0.5f);
	each.scale = { nowScale , nowScale , 1 };//通常拡縮
	//each.scale = { 1.0f , 1.0f ,  PanelSize * nowScale };
	if (data.spawnDerayTimer <= spawnDerayTimerMax)
	{
		data.spawnDerayTimer++;
	}
	else
	{
		data.spawnTimer++;
	}
}

void Othello::ReversUpdate(int combo)
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


	//float jumpRate = static_cast<float>(data.JumpTimer) / jumpTimerMax;
	//float jumpEaseRate = 0.0f;
	//if (data.isJumpUp)
	//{
	//	jumpEaseRate = ShlomonMath::EaseOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, jumpRate).x;

	//	float hight = pow(jumpEaseRate, count);
	//	each.position.m128_f32[2] = -jumpMax * hight;
	//	if (data.JumpTimer >= jumpTimerMax)
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
		XMFLOAT3 pos = ConvertXMVECTORtoXMFLOAT3(each.position);
		ObjectParticles::othelloFrame.Init(pos, 1, ParticleType::BornAndShake, data.chainName, data.comboCount);
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
		completeEach.position = each.position;
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
		completeEach.position = each.position;
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
	XMFLOAT3 pos = ConvertXMVECTORtoXMFLOAT3(each.position);
	ObjectParticles::othelloFrame.Init(pos, 1, ParticleType::BornAndShake, data.chainName, data.comboCount);
}

void Othello::Sink()
{
	data.vanishTimer++;
	if (data.vanishTimer <= vanishWaitTimer)
	{
		return;
	}
	if (data.vanishTimer >= vanishTimerMax)
	{
		data.isDead = true;
	}
	else
	{
		//沈むアニメーション
		//SinkAnimation();
		BreathAnimation();

	}
}
void Othello::SinkAnimation()
{
	float nowScale = static_cast<float>(data.vanishTimer - vanishWaitTimer) / vanishAnimation;
	data.isHarf = (nowScale >= 0.5f);
	//each.scale = { 1 - nowScale, 1 - nowScale, 1.0f };
	each.scale = { 1.0f, 1.0f, PanelSize * (1 - nowScale) };

}

void Othello::BreathAnimation()
{
	breathTimer++;
	breathLevelTimer++;
	int BreathTimerMax = Breath::breathSpan - (breathLevel * 2);
	if (breathTimer >= BreathTimerMax)
	{
		breathTimer = 0;
		BreathAnimationFlag = !BreathAnimationFlag;
	}
	if (breathLevelTimer >= Breath::breathLevelTimerMax)
	{
		breathLevelTimer = 0;
		breathLevel++;
	}
	float nowScale = static_cast<float>(breathTimer) / BreathTimerMax;

	if (BreathAnimationFlag)
	{
		nowScale = ShlomonMath::EaseInOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, nowScale).x;
	}
	else
	{
		nowScale = ShlomonMath::EaseInOutQuad(XMFLOAT3{ 1, 0, 0 }, XMFLOAT3{}, nowScale).x;
	}
	nowScale *= -0.2f;
	data.isHarf = (nowScale >= 0.5f);
	//each.scale = { 1 - nowScale, 1 - nowScale, 1.0f };
	each.scale = { 1.0f + nowScale, 1.0f + nowScale, PanelSize + nowScale };
}
void Othello::MakeParticle()
{
	XMFLOAT3 sample;
	sample = ConvertXMVECTORtoXMFLOAT3(each.position);
	ObjectParticles::triangle.Init(sample, 5, ParticleType::Exprotion);
	//ObjectParticles::triangle.Init(XMFLOAT3(0, 0, 0), 60, ParticleType::Swell);
}

bool Othello::GetIsActive()
{
	return (data.isVanish || data.isSandwich);
}

void OthelloManager::Init(Tex num[10], Model numModel[10])
{
	oserroModel.CreateModel("newOserro4", ShaderManager::othelloShader);
	stopOserroModel.CreateModel("rock_othello", ShaderManager::othelloShader);
	wallOserroModel.CreateModel("wall", ShaderManager::othelloShader);
	compModel.CreateModel("a_button", ShaderManager::othelloShader);
	chanceModelBlue.CreateModel("chance_1", ShaderManager::othelloShader);
	chanceModelOrange.CreateModel("chance", ShaderManager::othelloShader);

	sendDatas.resize(fieldSize);


	PanelTextModel.CreateModel("all_clear", ShaderManager::playerShader);
	ComboTextModel.CreateModel("combo", ShaderManager::playerShader);
	ScoreTextModel.CreateModel("score_kana", ShaderManager::playerShader);
	ScoreAttackTextModel.CreateModel("score_attack", ShaderManager::playerShader);
	NormaModeTextModel.CreateModel("norma", ShaderManager::playerShader);
	ConnectModel.CreateModel("connect", ShaderManager::playerShader);
	WaltzModel.CreateModel("waltz", ShaderManager::playerShader);
	BookendModel.CreateModel("bookend", ShaderManager::playerShader);
	TechTrainingModel.CreateModel("tech_training", ShaderManager::playerShader);
	UndoModel.CreateModel("back_kana", ShaderManager::playerShader);
	auto itr = sendDatas.begin();
	for (; itr != sendDatas.end(); itr++)
	{
		itr->resize(fieldSize);
	}
	LoadAllNormaStage();
	int fieldTextCount = NormaStartOthellos.size();
	fieldTextCount *= 2;
	fieldTextCount -= 9;
	fieldDrawText.resize(fieldTextCount);
	int stageCount = 0;
	auto stageCountTextItr = fieldDrawText.begin();
	for (int i = 0; i < NormaStartOthellos.size(); i++)
	{
		bool tenOver = stageCount >= 9;
		int x = stageCount % fieldSize;
		int y = stageCount / fieldSize;
		stageCountTextItr->CreateConstBuff0();
		stageCountTextItr->CreateConstBuff1();
		SetModeSelectEachInfo(*stageCountTextItr, panelPos{ x, y });
		//10ステージ以上なら
		if (tenOver)
		{
			float slideLength = 0.4;
			float widthScale = 0.75;
			stageCountTextItr->position.m128_f32[0] += slideLength;
			stageCountTextItr->scale.x *= widthScale;
			stageCountTextItr++;

			stageCountTextItr->CreateConstBuff0();
			stageCountTextItr->CreateConstBuff1();
			SetModeSelectEachInfo(*stageCountTextItr, panelPos{ x, y });
			stageCountTextItr->position.m128_f32[0] -= slideLength;
			stageCountTextItr->scale.x *= widthScale;
		}

		stageCountTextItr++;
		stageCount++;
	}
	TutorialText1.CreateSprite(L"Resource/Img/string_0.png", XMFLOAT3(0, 0, 0));
	TutorialText2.CreateSprite(L"Resource/Img/string_01.png", XMFLOAT3(0, 0, 0));
	TutorialText3.CreateSprite(L"Resource/Img/string_02.png", XMFLOAT3(0, 0, 0));
	TutorialText4.CreateSprite(L"Resource/Img/string_03.png", XMFLOAT3(0, 0, 0));
	TutorialText5.CreateSprite(L"Resource/Img/string_04.png", XMFLOAT3(0, 0, 0));
	TutorialText6.CreateSprite(L"Resource/Img/string_05.png", XMFLOAT3(0, 0, 0));
	CongraturationText.CreateSprite(L"Resource/Img/excellent.png", XMFLOAT3(0, 0, 0));
	exselent.CreateModel("excellent", ShaderManager::playerShader);
	back.CreateSprite(L"Resource/Img/title_back_80.png", XMFLOAT3(0, 0, 0));
	NormaPanelsText.CreateSprite(L"Resource/Img/all_delete_UI.png", XMFLOAT3(0, 0, 0));
	NormaComboText.CreateSprite(L"Resource/Img/combo_UI.png", XMFLOAT3(0, 0, 0));
	NormaScoreText.CreateSprite(L"Resource/Img/score_UI.png", XMFLOAT3(0, 0, 0));

	float changeScale = 0.5f;

	TutorialText1.ChangeSize(1227 * changeScale, 332 * changeScale);
	TutorialText2.ChangeSize(1158 * changeScale, 207 * changeScale);
	TutorialText3.ChangeSize(1158 * changeScale, 207 * changeScale);
	TutorialText4.ChangeSize(971 * changeScale, 82 * changeScale);
	TutorialText5.ChangeSize(910 * changeScale, 207 * changeScale);
	TutorialText6.ChangeSize(1070 * changeScale / 2, 154 * changeScale / 2);
	CongraturationText.ChangeSize(457 * changeScale, 79 * changeScale);
	//TutorialRetryText.ChangeSize(288 * changeScale, 231 * changeScale);
	NormaPanelsText.ChangeSize(444 * changeScale, 72 * changeScale);
	NormaComboText.ChangeSize(219 * changeScale, 72 * changeScale);
	NormaScoreText.ChangeSize(221 * changeScale, 68 * changeScale);

	TutorialText1.position = XMVECTOR{ 640 - (1227 * changeScale / 2), 720 - (332 * changeScale + 30), 0, 0 };
	TutorialText2.position = XMVECTOR{ 640 - (1158 * changeScale / 2), 720 - (207 * changeScale + 30), 0, 0 };
	TutorialText3.position = XMVECTOR{ 640 - (1158 * changeScale / 2) , 720 - (207 * changeScale + 30), 0, 0 };
	TutorialText4.position = XMVECTOR{ 640 - (971 * changeScale / 2) , 720 - (82 * changeScale + 60), 0, 0 };
	TutorialText5.position = XMVECTOR{ 640 - (910 * changeScale / 2) , 720 - (207 * changeScale + 30), 0, 0 };
	TutorialText6.position = XMVECTOR{ 1280 - (1070 * changeScale / 2 + 15) , 720 - (77 * changeScale + 15), 0, 0 };

	CongraturationText.position = XMVECTOR{ 640 - (457 * changeScale / 2), 360 - (79 * changeScale / 2), 0, 0 };

	NormaPanelsText.position = XMVECTOR{ 640 - (444 * changeScale / 2),  (72 * changeScale - 5), 0, 0 };
	NormaComboText.position = XMVECTOR{ 640 - (219 * changeScale / 2),  (72 * changeScale - 5), 0, 0 };
	NormaScoreText.position = XMVECTOR{ 640 - (221 * changeScale / 2),  (68 * changeScale - 5), 0, 0 };

	normaChecker.Init();

	NormaDrawData.position = comboScoreModelPos;
	NormaDrawData.scale = { textBaseScale, textBaseScale, textBaseScale };
	NormaDrawData.rotation.x = -30.0f;
	NormaDrawData.CreateConstBuff0();
	NormaDrawData.CreateConstBuff1();

	ScoreAttackDrawData.position = comboScoreModelPos;
	ScoreAttackDrawData.scale = { textBaseScale, textBaseScale, textBaseScale };
	ScoreAttackDrawData.rotation = { -90, 0, 0 };
	ScoreAttackDrawData.CreateConstBuff0();
	ScoreAttackDrawData.CreateConstBuff1();

	SubNormaDrawData.position = XMVECTOR{ 4.0f, 15.0f, -1.0f ,0 };
	SubNormaDrawData.scale = { textBaseScale, textBaseScale, textBaseScale };
	SubNormaDrawData.rotation.x = -30.0f;
	SubNormaDrawData.CreateConstBuff0();
	SubNormaDrawData.CreateConstBuff1();

	NormaModeTextDrawData.position = XMVECTOR{ 0.0f, 18.0f, -1.0f ,0 };
	NormaModeTextDrawData.scale = { textBaseScale, textBaseScale, textBaseScale };
	NormaModeTextDrawData.rotation.x = -30.0f;
	NormaModeTextDrawData.CreateConstBuff0();
	NormaModeTextDrawData.CreateConstBuff1();

	TechTrainingTextDrawData.scale = { textBaseScale, textBaseScale, textBaseScale };
	TechTrainingTextDrawData.rotation.x = -30.0f;
	TechTrainingTextDrawData.CreateConstBuff0();
	TechTrainingTextDrawData.CreateConstBuff1();

	UndoTextDrawData.rotation.x = -30.0f;
	UndoTextDrawData.scale = { textBaseScale, textBaseScale, textBaseScale };
	UndoTextDrawData.CreateConstBuff0();
	UndoTextDrawData.CreateConstBuff1();

	BookendTextDrawData.rotation.x = -30.0f;
	BookendTextDrawData.scale = { textBaseScale, textBaseScale, textBaseScale };
	BookendTextDrawData.CreateConstBuff0();
	BookendTextDrawData.CreateConstBuff1();

	WaltzTextDrawData.rotation.x = -30.0f;
	WaltzTextDrawData.scale = { textBaseScale, textBaseScale, textBaseScale };
	WaltzTextDrawData.CreateConstBuff0();
	WaltzTextDrawData.CreateConstBuff1();

	ConnectTextDrawData.rotation.x = -30.0f;

	ConnectTextDrawData.scale = { textBaseScale, textBaseScale, textBaseScale };
	ConnectTextDrawData.CreateConstBuff0();
	ConnectTextDrawData.CreateConstBuff1();

	CountDrawData.resize(5);
	numberModel.resize(10);
	for (int i = 0; i < 10; i++)
	{
		numberModel[i] = &numModel[i];
	}
	for (int i = 0; i < 5; i++)
	{
		CountDrawData[i].position = XMVECTOR{};
		CountDrawData[i].scale = { textBaseScale, textBaseScale, textBaseScale };
		CountDrawData[i].rotation.x = -30.0f;
		CountDrawData[i].CreateConstBuff0();
		CountDrawData[i].CreateConstBuff1();

		for (int j = 0; j < 10; j++)
		{
			int count = (i * 10) + j;
			normaCountTex[count].CreateSprite(num[j], XMFLOAT3(window_width / 2 - 10 + (i * 50), 10, 0));
			normaCountTex[count].ChangeSize(48, 64);
		}
	}
	textEase.Set(0, 120);
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			SetChanceObject(x, y, true);
		}
	}


	ConnectText.resize(17);

	ConnectText[0].CreateSprite(L"Resource/Img/connect/connect_text_0.png", XMFLOAT3{});
	ConnectText[1].CreateSprite(L"Resource/Img/connect/connect_text_1.png", XMFLOAT3{});
	ConnectText[2].CreateSprite(L"Resource/Img/connect/connect_text_2.png", XMFLOAT3{});
	ConnectText[3].CreateSprite(L"Resource/Img/connect/connect_text_3.png", XMFLOAT3{});
	ConnectText[4].CreateSprite(L"Resource/Img/connect/connect_text_4.png", XMFLOAT3{});
	ConnectText[5].CreateSprite(L"Resource/Img/connect/connect_text_5.png", XMFLOAT3{});
	ConnectText[6].CreateSprite(L"Resource/Img/connect/connect_text_6.png", XMFLOAT3{});
	ConnectText[7].CreateSprite(L"Resource/Img/connect/connect_text_7.png", XMFLOAT3{});
	ConnectText[8].CreateSprite(L"Resource/Img/connect/connect_text_8.png", XMFLOAT3{});
	ConnectText[9].CreateSprite(L"Resource/Img/connect/connect_text_9.png", XMFLOAT3{});
	ConnectText[10].CreateSprite(L"Resource/Img/connect/connect_text_10.png", XMFLOAT3{});
	ConnectText[11].CreateSprite(L"Resource/Img/connect/connect_text_11.png", XMFLOAT3{});
	ConnectText[12].CreateSprite(L"Resource/Img/connect/connect_text_12.png", XMFLOAT3{});
	ConnectText[13].CreateSprite(L"Resource/Img/connect/connect_text_13.png", XMFLOAT3{});
	ConnectText[14].CreateSprite(L"Resource/Img/connect/connect_text_14.png", XMFLOAT3{});
	ConnectText[15].CreateSprite(L"Resource/Img/connect/connect_text_15.png", XMFLOAT3{});
	ConnectText[16].CreateSprite(L"Resource/Img/connect/connect_text_16.png", XMFLOAT3{});

	for (auto &e : ConnectText)
	{
		e.ChangeSize(1280, 300);
		e.position = XMVECTOR{ 0, 720 - (300 + 30), 0, 0 };
	}
	WaltzText.resize(15);
	WaltzText[0].CreateSprite(L"Resource/Img/waltz_text/waltz_text_0.png", XMFLOAT3{});
	WaltzText[1].CreateSprite(L"Resource/Img/waltz_text/waltz_text_1.png", XMFLOAT3{});
	WaltzText[2].CreateSprite(L"Resource/Img/waltz_text/waltz_text_2.png", XMFLOAT3{});
	WaltzText[3].CreateSprite(L"Resource/Img/waltz_text/waltz_text_3.png", XMFLOAT3{});
	WaltzText[4].CreateSprite(L"Resource/Img/waltz_text/waltz_text_4.png", XMFLOAT3{});
	WaltzText[5].CreateSprite(L"Resource/Img/waltz_text/waltz_text_5.png", XMFLOAT3{});
	WaltzText[6].CreateSprite(L"Resource/Img/waltz_text/waltz_text_6.png", XMFLOAT3{});
	WaltzText[7].CreateSprite(L"Resource/Img/waltz_text/waltz_text_7.png", XMFLOAT3{});
	WaltzText[8].CreateSprite(L"Resource/Img/waltz_text/waltz_text_8.png", XMFLOAT3{});
	WaltzText[9].CreateSprite(L"Resource/Img/waltz_text/waltz_text_9.png", XMFLOAT3{});
	WaltzText[10].CreateSprite(L"Resource/Img/waltz_text/waltz_text_10.png", XMFLOAT3{});
	WaltzText[11].CreateSprite(L"Resource/Img/waltz_text/waltz_text_11.png", XMFLOAT3{});
	WaltzText[12].CreateSprite(L"Resource/Img/waltz_text/waltz_text_12.png", XMFLOAT3{});
	WaltzText[13].CreateSprite(L"Resource/Img/waltz_text/waltz_text_13.png", XMFLOAT3{});
	WaltzText[14].CreateSprite(L"Resource/Img/waltz_text/waltz_text_14.png", XMFLOAT3{});
	for (auto &e : WaltzText)
	{
		e.ChangeSize(1280, 300);
		e.position = XMVECTOR{ 0, 720 - (300 + 30), 0, 0 };
	}
	BookendText.resize(11);
	BookendText[0].CreateSprite(L"Resource/Img/bookend_text/bookend_text_0.png", XMFLOAT3{});
	BookendText[1].CreateSprite(L"Resource/Img/bookend_text/bookend_text_1.png", XMFLOAT3{});
	BookendText[2].CreateSprite(L"Resource/Img/bookend_text/bookend_text_2.png", XMFLOAT3{});
	BookendText[3].CreateSprite(L"Resource/Img/bookend_text/bookend_text_3.png", XMFLOAT3{});
	BookendText[4].CreateSprite(L"Resource/Img/bookend_text/bookend_text_4.png", XMFLOAT3{});
	BookendText[5].CreateSprite(L"Resource/Img/bookend_text/bookend_text_5.png", XMFLOAT3{});
	BookendText[6].CreateSprite(L"Resource/Img/bookend_text/bookend_text_6.png", XMFLOAT3{});
	BookendText[7].CreateSprite(L"Resource/Img/bookend_text/bookend_text_7.png", XMFLOAT3{});
	BookendText[8].CreateSprite(L"Resource/Img/bookend_text/bookend_text_8.png", XMFLOAT3{});
	BookendText[9].CreateSprite(L"Resource/Img/bookend_text/bookend_text_9.png", XMFLOAT3{});
	BookendText[10].CreateSprite(L"Resource/Img/bookend_text/bookend_text_10.png", XMFLOAT3{});
	for (auto &e : BookendText)
	{
		e.ChangeSize(1280, 300);
		e.position = XMVECTOR{ 0, 720 - (300 + 30), 0, 0 };
	}
}

void OthelloManager::Update(int combo)
{
	//死ぬ
	DeadPanel();

	MinSpawn(true);
	//生成
	RandumSetPanel();

	//更新
	auto itr = othellos.begin();

	for (; itr != othellos.end(); ++itr)
	{
		itr->Update(combo);
	}
	if (A())
	{
		isFieldUpdate = true;
	}


	SaveSpawn();
}

void OthelloManager::TutorialUpdate(int combo)
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
		itr->Update(combo);
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
	if (A())
	{
		isFieldUpdate = true;
	}
	bool retry = (Input::KeyTrigger(DIK_R) || directInput->IsButtonPush(directInput->Button03));


	switch (scenes)
	{
	case TutorialSceneFlow::SandwichSpawn:
		whySandwichSpawn();
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
			ObjectParticles::othelloFrame.DeleteAllParticle();
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
			ObjectParticles::othelloFrame.DeleteAllParticle();
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
			ObjectParticles::othelloFrame.DeleteAllParticle();
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
	//TutorialRetryText.SpriteDraw();

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
			//CongraturationText.SpriteDraw();
			exselent.each.position = { -1.0f, 1.00f, -2.0f, 0 };
			exselent.each.scale = { 0.5f, 0.5f, 0.5f };
			exselent.each.rotation.x = -30.0f;
			exselent.Update();
			Draw3DObject(exselent);
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
			//CongraturationText.SpriteDraw();
			exselent.each.position = { -1.0f, 1.00f, -2.0f, 0 };
			exselent.each.scale = { 0.5f, 0.5f, 0.5f };
			exselent.each.rotation.x = -30.0f;
			exselent.Update();
			Draw3DObject(exselent);
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
			//CongraturationText.SpriteDraw();
			exselent.each.position = { -1.0f, 1.00f, -2.0f, 0 };
			exselent.each.scale = { 0.4f, 0.4f, 0.4f };
			exselent.each.rotation.x = -30.0f;
			exselent.Update();
			Draw3DObject(exselent);
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
			if (TutorialEndTextCount >= tutorialEndTextTimer)
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

void OthelloManager::NormaUpdate(int combo)
{
	//死ぬ
	DeadPanel();

	//MinSpawn();
	////生成
	//RandumSetPanel();

	//更新
	auto itr = othellos.begin();
	int nowMaxCombo = 0;
	for (; itr != othellos.end(); ++itr)
	{
		itr->Update(combo);
		if (itr->GetGameData()->maxComboCount >= nowMaxCombo)
		{
			nowMaxCombo = itr->GetGameData()->maxComboCount;
		}
	}
	if (A())
	{
		isFieldUpdate = true;
	}
	if (isNormaMode)
	{
		Undo();
		normaChecker.Update(othellos, nowScore, nowMaxCombo);
	}
}
void OthelloManager::Draw(bool isChanceDraw, bool isCompDraw)
{
	OthelloDraw();
	if (isChanceDraw)
	{
		ChanceDraw();
	}
	if (isCompDraw)
	{
		CompDraw();
	}
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
		if (itr->isDraw)
		{
			itr->Draw();
		}
	}
}


void OthelloManager::CompDraw()
{
	if (othellos.size() == 0) return;

	auto itr = othellos.begin();
	for (; itr != othellos.end(); ++itr)
	{
		itr->CompDraw();
	}
}

void OthelloManager::NormaTextDraw(int stageNum, bool isDraw)
{
	SetTextPos(true);

	if (isDraw)
	{
		list<NormaModeFieldData>::iterator data = GetNormaStage(stageNum);

		int status = data->normaStatus;
		if (data->type == Norma::Combo)
		{
			NormaComboText.SpriteDraw();
		}
		else if (data->type == Norma::Panels)
		{
			NormaPanelsTextSetPos(true);
			NormaPanelsText.SpriteDraw();
			status = data->panels.size();
		}
		else if (data->type == Norma::Score)
		{
			NormaScoreText.SpriteDraw();
		}
		if (data->subNormaFlag)
		{
			NormaPanelsText.SpriteDraw();
		}
		CountTextDraw(status);
		normaChecker.Draw();
	}
}

void OthelloManager::NormaTextModelDraw(int stageNum, bool isDraw)
{
	SetTextPos(true, stageNum);

	if (isDraw)
	{
		list<NormaModeFieldData>::iterator data = GetNormaStage(stageNum);
		//list<NormaModeFieldData>::iterator data = GetNormaStage(GetEnterNormaStage());

		int status = data->normaStatus;

		auto itr = data->panels.begin();

		if (data->type == Norma::Combo)
		{
			ComboTextModel.Update(&NormaDrawData);
			Draw3DObject(ComboTextModel);
			CountModelDraw(status);
		}
		else if (data->type == Norma::Panels)
		{
			status = 0;
			for (; itr != data->panels.end(); itr++)
			{
				if (itr->type != WALL)
				{
					status++;
				}
			}
			PanelTextModel.Update(&NormaDrawData);
			Draw3DObject(PanelTextModel);
		}
		else if (data->type == Norma::Score)
		{
			ScoreTextModel.Update(&NormaDrawData);
			Draw3DObject(ScoreTextModel);
			CountModelDraw(status);
		}
		if (data->subNormaFlag)
		{
			PanelTextModel.Update(&SubNormaDrawData);
			Draw3DObject(PanelTextModel);
		}
		normaChecker.Draw();
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


void OthelloManager::ModeSelectStart(int stageNum, ModeSelectType type)
{

	XMFLOAT3 startPos = OthlloPlayer::GetPosition();
	nowType = type;
	startPos.z = 0.0f;
	XMFLOAT3 stagePos = startPos - stageLeftTop - XMFLOAT3{ -cellScale / 2, cellScale / 2 , 0 };

	//どのマスにマウスがあるのかを確認
	int x = stagePos.x / (cellScale * 2);
	int y = stagePos.y / -(cellScale * 2);

	//プレイヤーの下の駒を設置する処理

		//Othello data;
		//data.Init(&oserroModel, &stopOserroModel);
		//bool randFront = rand() % 2;
		//data.Spawn(NORMAL, x, y, randFront);
		//data.GetGameData()->isMove = false;
		//data.GetGameData()->isPlayer = false;
		//othellos.push_back(data);
	//for (int stageCount = 0; stageCount < 8; stageCount++)
	//{
	//	SetChanceObject(stageCount, stageCount, true);
	//	SetChanceObject(stageCount, 7 - stageCount, false);
	//}
	//MinSpawn(false);
	if (type == DojoSelect)
	{
		SelectDojoPanel();
	}
	else
	{
		SetGameModeSelectPanel(stageNum);
	}
}

void OthelloManager::DojoStart()
{
	TextCount = 0;
	isAct = false;
	//全ての道場が初期化される
	switch (dojoType)
	{
	case Conect:
		ConnectStart();
		break;
	case Waltz:
		WaltzStart();
		break;
	case Bookend:
		BookendStart();
		break;
	default:
		break;
	}
}

void OthelloManager::DojoUpdate(int combo)
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
		itr->Update(combo);
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
	if (A())
	{
		isFieldUpdate = true;
	}
	bool retry = (Input::KeyTrigger(DIK_R) || directInput->IsButtonPush(directInput->Button03));

	switch (dojoType)
	{
	case Conect:
		ConnectUpdate(panelCount, comboMax);
		break;
	case Waltz:
		WaltzUpdate(panelCount, comboMax);
		break;
	case Bookend:
		BookendUpdate(panelCount, comboMax);
		break;
	default:
		break;
	}
	//スイッチ文で道場の内容分岐
}

void OthelloManager::DojoDraw()
{
	if (isAct)
	{
		return;
	}
	//スイッチ文で道場の内容分岐
	switch (dojoType)
	{
	case Conect:
		ConnectDraw();
		break;
	case Waltz:
		WaltzDrwa();
		break;
	case Bookend:
		BookendDrwa();
		break;
	default:
		break;
	}
}

bool OthelloManager::DojoEnd()
{
	bool isEnd = false;
	//スイッチ文で道場の内容分岐
	switch (dojoType)
	{
	case Conect:
		isEnd = nowConnectFlow == ConnectEnd;
		break;
	case Waltz:
		break;
	case Bookend:
		break;
	default:
		break;
	}
	return isEnd;
}

//済
#pragma region Connect
//済
void OthelloManager::ConnectStart()
{
	isAct = false;
	AllDeletePanel();
	nowConnectFlow = Connect1;
	ConnectFirstSpwan();
}
//済
void OthelloManager::ConnectUpdate(int AlivePanel, int ComboCount)
{
	switch (nowConnectFlow)
	{
	case Connect1:
		ConnectFirstUpdate(AlivePanel, ComboCount);
		break;
	case Connect2:
		ConnectSecondUpdate(AlivePanel, ComboCount);
		break;
	case Connect3:
		ConnectThirdUpdate(AlivePanel, ComboCount);
		break;
	case Connect4:
		ConnectForceUpdate(AlivePanel, ComboCount);
		break;
	case Connect5:
		ConnectFifthUpdate(AlivePanel, ComboCount);
		break;
	case ConnectEnd:
		break;
	default:
		break;
	}
}
//済
void OthelloManager::ConnectDraw()
{
	if (TextCount >= ConnectText.size() || nowConnectFlow == ConnectEnd)
	{
		return;
	}
	ConnectText[TextCount].SpriteDraw();
}

//済
void OthelloManager::ConnectFirstSpwan()
{
	SetSpawnPanel(6, 2, false);
	SetSpawnPanel(6, 4, false);
	SetSpawnPanel(6, 5, true);
	SetSpawnPlayer(6, 2);
}
//済
void OthelloManager::ConnectFirstUpdate(int AlivePanel, int ComboCount)
{
	if (!isAct)
	{
		if (A())
		{
			TextCount++;
		}
		//テキストが一定より進んだら
		if (TextCount > 2)
		{
			isAct = true;
		}
	}
	else
	{
		//生きてるパネルがなくなったら
		if (AlivePanel <= 0)
		{
			ConnectSecondSpawn();
		}
	}
}

//済
void OthelloManager::ConnectSecondSpawn()
{
	SetSpawnPanel(4, 5, true);
	SetSpawnPanel(5, 5, false);
	nowConnectFlow = Connect2;
	isAct = false;
}
//済
void OthelloManager::ConnectSecondUpdate(int AlivePanel, int ComboCount)
{
	//生きてるパネルがなくなったら
	if (AlivePanel <= 0)
	{
		isAct = false;
		if (A())
		{
			TextCount++;
		}
		if (TextCount > 11)
		{
			ConnectThirdSpawn();
		}
	}
	else if (!isAct)
	{
		if (A())
		{
			TextCount++;
		}
		//テキストカウントが一定を超えたら
		if (TextCount > 8)
		{
			isAct = true;
		}
	}
}

//済
void OthelloManager::ConnectThirdSpawn()
{
	SetSpawnPanel(4, 4, false);
	SetSpawnPanel(4, 3, true);
	nowConnectFlow = Connect3;
	isAct = true;
}
//済
void OthelloManager::ConnectThirdUpdate(int AlivePanel, int ComboCount)
{
	if (!isAct)
	{
	}
	else
	{
		if (AlivePanel <= 0)
		{
			ConnectForceSpawn();
		}
	}
}

//済
void OthelloManager::ConnectForceSpawn()
{
	SetSpawnPanel(2, 3, true);
	SetSpawnPanel(1, 3, false);
	nowConnectFlow = Connect4;
	isAct = true;
}
//済
void OthelloManager::ConnectForceUpdate(int AlivePanel, int ComboCount)
{
	if (!isAct)
	{
	}
	else
	{
		if (AlivePanel <= 0)
		{
			ConnectFifthSpawn();
		}
	}
}

//済
void OthelloManager::ConnectFifthSpawn()
{
	SetSpawnPanel(2, 0, false);
	SetSpawnPanel(2, 1, true);
	SetSpawnPanel(2, 2, true);
	SetSpawnPanel(2, 4, false);
	nowConnectFlow = Connect5;
	isAct = false;
}
//済
void OthelloManager::ConnectFifthUpdate(int AlivePanel, int ComboCount)
{
	//生きてるパネルがなくなったら
	if (AlivePanel <= 0)
	{
		isAct = false;
		if (A())
		{
			TextCount++;
		}
		if (TextCount > 16)
		{
			nowConnectFlow = ConnectEnd;
		}
	}
	else if (!isAct)
	{
		if (A())
		{
			TextCount++;
		}
		//テキストカウントが一定を超えたら
		if (TextCount > 14)
		{
			isAct = true;
		}
	}
	else
	{

	}
}
#pragma endregion


//未
#pragma region Waltz
//未
void OthelloManager::WaltzStart()
{
	nowWaltzFlow = Waltz1;
	isAct = false;
	AllDeletePanel();
	isAct = false;
	nowWaltzFlow = Waltz1;
	WaltzFirstSpwan();
}
//未
void OthelloManager::WaltzUpdate(int AlivePanel, int ComboCount)
{
	switch (nowWaltzFlow)
	{
	case Waltz1:
		WaltzFirstUpdate(AlivePanel, ComboCount);
		break;
	case Waltz2:
		WaltzSecondUpdate(AlivePanel, ComboCount);
		break;
	case Waltz3:
		WaltzThirdUpdate(AlivePanel, ComboCount);
		break;
	case Waltz4:
		WaltzForceUpdate(AlivePanel, ComboCount);
		break;
	case Waltz5:
		break;
	case Waltz6:
		break;
	case Waltz7:
		break;
	case Waltz8:
		break;
	case WaltzEnd:
		break;
	default:
		break;
	}
}
//未
void OthelloManager::WaltzDrwa()
{
	if (TextCount >= WaltzText.size())
	{
		return;
	}
	WaltzText[TextCount].SpriteDraw();

}

//未
void OthelloManager::WaltzFirstSpwan()
{

}
//未
void OthelloManager::WaltzFirstUpdate(int AlivePanel, int ComboCount)
{
	if (!isAct)
	{

	}
	else
	{
		if (/*クリア*/false)
		{

		}
	}
}

//未
void OthelloManager::WaltzSecondSpawn()
{
	isAct = false;
	nowWaltzFlow = Waltz1;
}
//未
void OthelloManager::WaltzSecondUpdate(int AlivePanel, int ComboCount)
{
	if (!isAct)
	{

	}
	else
	{
		if (/*クリア*/false)
		{

		}
	}
}

//未
void OthelloManager::WaltzThirdSpawn()
{
	isAct = false;
	nowWaltzFlow = Waltz1;
}
//未
void OthelloManager::WaltzThirdUpdate(int AlivePanel, int ComboCount)
{
	if (!isAct)
	{

	}
	else
	{
		if (/*クリア*/false)
		{

		}
	}
}

//未
void OthelloManager::WaltzForceSpawn()
{
	isAct = false;
	nowWaltzFlow = Waltz1;
}
//未
void OthelloManager::WaltzForceUpdate(int AlivePanel, int ComboCount)
{
	if (!isAct)
	{

	}
	else
	{
		if (/*クリア*/false)
		{

		}
	}
}
#pragma endregion

//未
#pragma region Boolend
//未
void OthelloManager::BookendStart()
{
	isAct = false;
	AllDeletePanel();
	BookendFirstSpwan();
	nowBookendFlow = BookEnd1;
}
//済
void OthelloManager::BookendUpdate(int AlivePanel, int ComboCount)
{
	switch (nowBookendFlow)
	{
	case BookEnd1:
		BookendFirstUpdate(AlivePanel, ComboCount);
		break;
	case BookEnd2:
		BookendSecondUpdate(AlivePanel, ComboCount);
		break;
	case BookEnd3:
		BookendThirdUpdate(AlivePanel, ComboCount);
		break;
	case BookEnd4:
		BookendForceUpdate(AlivePanel, ComboCount);
		break;
	case BookEndEnd:
		break;
	default:
		break;
	}

}
//済
void OthelloManager::BookendDrwa()
{
	if (TextCount >= BookendText.size())
	{
		return;
	}
	BookendText[TextCount].SpriteDraw();
}

//済
void OthelloManager::BookendFirstSpwan()
{
	SetSpawnPanel(0, 0, false);
	for (int i = 1; i < 8; i++)
	{
		SetSpawnPanel(i, 0, true);
	}
	SetSpawnPlayer(7, 0);
}
//済
void OthelloManager::BookendFirstUpdate(int AlivePanel, int ComboCount)
{
	if (!isAct)
	{
		if (A())
		{
			TextCount++;
		}
		if (TextCount > 5)
		{
			isAct = true;
		}
	}
	else
	{
		if (AlivePanel <= 1)
		{
			BookendSecondSpawn();
		}
	}
}

//済
void OthelloManager::BookendSecondSpawn()
{
	isAct = false;
	for (int i = 2; i < 6; i++)
	{
		SetSpawnPanel(7, i, false);
	}
	nowBookendFlow = BookEnd2;
}
//済
void OthelloManager::BookendSecondUpdate(int AlivePanel, int ComboCount)
{
	if (!isAct)
	{
		if (A())
		{
			TextCount++;
		}
		if (TextCount > 8)
		{
			isAct = true;
		}
	}
	else
	{
		if (AlivePanel <= 1)
		{
			BookendThirdSpawn();
		}
	}
}

//済
void OthelloManager::BookendThirdSpawn()
{
	isAct = false;
	for (int i = 0; i < 6; i++)
	{
		SetSpawnPanel(i, 5, true);
	}
	nowBookendFlow = BookEnd3;
}
//済
void OthelloManager::BookendThirdUpdate(int AlivePanel, int ComboCount)
{
	if (AlivePanel <= 1)
	{
		isAct = false;

		if (A())
		{
			TextCount++;
		}
		if (TextCount > 10)
		{
		nowBookendFlow = BookEndEnd;
		}
	}
	else if (!isAct)
	{
		if (A())
		{
			TextCount++;
		}
		if (TextCount > 9)
		{
			isAct = true;
		}
	}
}

//未
void OthelloManager::BookendForceSpawn()
{

}
//未
void OthelloManager::BookendForceUpdate(int AlivePanel, int ComboCount)
{
	if (!isAct)
	{

	}
	else
	{
		if (/*クリア*/false)
		{

		}
	}
}
#pragma endregion




















void OthelloManager::ModeSelectUpdate()
{

	isAct = true;
	//死ぬ
	DeadPanel();
	//更新
	auto itr = othellos.begin();

	for (; itr != othellos.end(); ++itr)
	{	//コンボ数を位置として計算
		panelPos nowPos = { itr->GetGameData()->widthPos, itr->GetGameData()->heightPos };

		itr->GetGameData()->isShake = (nowPos == playerPanelPos);

		itr->Update(1);
	}
	if (A())
	{
		switch (nowType)
		{
		case GameModeSelect:
			SelectUpdateGameMode();
			break;
		case DojoSelect:
			SelectUpdateDojo();
			break;
		case PuzzleSelect:
			SelectUpdatePazzle();
			break;
		default:
			break;
		}
		isFieldUpdate = true;
	}



}
void OthelloManager::SelectUpdateGameMode()
{
	if (playerPanelPos == DojoPanel)
	{
		nowType = ModeSelectType::DojoSelect;
		SetDojoModeSelectPanel();
	}
}

void OthelloManager::SelectUpdateDojo()
{
	if (playerPanelPos == ConectPanel)
	{
		dojoType = DojoType::Conect;
	}
	else if (playerPanelPos == WaltzPanel)
	{
		dojoType = DojoType::Waltz;
	}
	else if (playerPanelPos == BookendPanel)
	{
		dojoType = DojoType::Bookend;
	}
	else if (playerPanelPos == UndoPanel)
	{
		nowType = ModeSelectType::GameModeSelect;
		SetGameModeSelectPanel();
	}

}
bool OthelloManager::SelectDojoPanel()
{
	bool isOnpanel = false;
	if (playerPanelPos == ConectPanel)
	{
		isOnpanel = true;
	}
	else if (playerPanelPos == WaltzPanel)
	{
		isOnpanel = true;
	}
	else if (playerPanelPos == BookendPanel)
	{
		isOnpanel = true;
	}


	return isOnpanel;
}

void OthelloManager::SelectUpdatePazzle()
{
	if (playerPanelPos == UndoPanel)
	{
		nowType = ModeSelectType::GameModeSelect;
		SetGameModeSelectPanel();
	}
}

void OthelloManager::ModeSelectControll()
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
}

bool OthelloManager::InMode()
{
	return (A());
}

GameMode OthelloManager::GetEnterModeType()
{
	GameMode now = GameMode::NoneGameMode;

	int stageNum = (playerPanelPos.y * 8) + playerPanelPos.x + 1;
	if (stageNum <= NormaStartOthellos.size() && nowType == PuzzleSelect)
	{
		now = GameMode::NormaMode;
	}
	else if (playerPanelPos == ScoreAttackPanel && nowType == GameModeSelect)
	{
		now = GameMode::ScoreAttack;
	}
	else if (SelectDojoPanel() && nowType == DojoSelect)
	{
		now = GameMode::Dojo;
	}
	else if (playerPanelPos == tutorialPanel && nowType == GameModeSelect)
	{
		now = GameMode::Tutorial;
	}

	return now;
}

int OthelloManager::GetEnterNormaStage()
{
	int stageNum = 1;
	stageNum += playerPanelPos.x;
	stageNum += playerPanelPos.y * 8;
	return stageNum;
}

void OthelloManager::ModeSelectModelDraw(bool isDraw)
{
	textAnimationRate = WaveInOut(textEase, 2);
	SetTextPos(false);
	if (isDraw)
	{
		//int stageNum = playerPanelPos.x + 1;
		//stageNum += playerPanelPos.y * 8;
		//int textCount = 0;
		//for (int i = 1; i <= NormaStartOthellos.size(); i++)
		//{
		//	if (i != stageNum)
		//	{
		//		if (i >= 10)
		//		{
		//			int firstDegit = textCount;
		//			int secondDegit = textCount + 1;
		//			CountModelDraw(i, &fieldDrawText[firstDegit], &fieldDrawText[secondDegit]);
		//		}
		//		else
		//		{
		//			CountModelDraw(i, &fieldDrawText[textCount]);
		//		}
		//	}
		//	if (i >= 10)
		//	{
		//		textCount += 2;
		//	}
		//	else
		//	{
		//		textCount++;
		//	}
		//}

		if (nowType == GameModeSelect)
		{
			ScoreAttackTextModel.Update(&ScoreAttackDrawData);
			Draw3DObject(ScoreAttackTextModel);

			TechTrainingModel.Update(&TechTrainingTextDrawData);
			Draw3DObject(TechTrainingModel);

			if (GetEnterModeType() != GameMode::NormaMode) { return; }

			NormaModeTextDrawData.position = XMVECTOR{ 0.0f, 18.0f, -1.0f ,0 } + (FloatAnimationDistance * textAnimationRate);
			NormaModeTextModel.Update(&NormaModeTextDrawData);
			Draw3DObject(NormaModeTextModel);
			list<NormaModeFieldData>::iterator data = GetNormaStage(GetEnterNormaStage());
			int status = data->normaStatus;
			auto itr = data->panels.begin();

			if (data->type == Norma::Combo)
			{
				ComboTextModel.Update(&NormaDrawData);
				Draw3DObject(ComboTextModel);
				CountModelDraw(status);
			}
			else if (data->type == Norma::Panels)
			{
				status = 0;
				for (; itr != data->panels.end(); itr++)
				{
					if (itr->type != WALL)
					{
						status++;
					}
				}
				PanelTextModel.Update(&NormaDrawData);
				Draw3DObject(PanelTextModel);
			}
			else if (data->type == Norma::Score)
			{
				ScoreTextModel.Update(&NormaDrawData);
				Draw3DObject(ScoreTextModel);
				CountModelDraw(status);
			}
			if (data->subNormaFlag)
			{
				PanelTextModel.Update(&SubNormaDrawData);
				Draw3DObject(PanelTextModel);
			}
		}
		else if (nowType == ModeSelectType::DojoSelect)
		{
			UndoModel.Update(&UndoTextDrawData);
			Draw3DObject(UndoModel);

			ConnectModel.Update(&ConnectTextDrawData);
			Draw3DObject(ConnectModel);

			WaltzModel.Update(&WaltzTextDrawData);
			Draw3DObject(WaltzModel);

			BookendModel.Update(&BookendTextDrawData);
			Draw3DObject(BookendModel);

		}
	}
}

void OthelloManager::ModeSelectDraw(bool isDraw)
{
	SetTextPos(false);

	if (isDraw)
	{
		if (GetEnterModeType() != GameMode::NormaMode) { return; }
		list<NormaModeFieldData>::iterator data = GetNormaStage(GetEnterNormaStage());
		int status = data->normaStatus;

		auto itr = data->panels.begin();

		if (data->type == Norma::Combo)
		{
			NormaComboText.SpriteDraw();
		}
		else if (data->type == Norma::Panels)
		{
			status = 0;
			for (; itr != data->panels.end(); itr++)
			{
				if (itr->type != WALL)
				{
					status++;
				}
			}
			NormaPanelsTextSetPos(false);
			NormaPanelsText.SpriteDraw();
		}
		else if (data->type == Norma::Score)
		{
			NormaScoreText.SpriteDraw();
		}
		if (data->subNormaFlag)
		{
			NormaPanelsText.SpriteDraw();
		}

		CountTextDraw(status);
	}
}

void OthelloManager::CountTextDraw(int count)
{
	vector<int>degit;
	while (count >= 10)
	{
		int tmp = count % 10;
		degit.push_back(tmp);
		count /= 10;
	}
	degit.push_back(count);

	auto itr = degit.begin();
	for (int i = degit.size(); i > 0; i--)
	{
		int num = (i * 10) + *itr;
		normaCountTex[num].SpriteDraw();

		numberModel[*itr]->Update(&CountDrawData[i]);
		itr++;
	}
}

void OthelloManager::CountModelDraw(int count, EachInfo *data, EachInfo *dataB)
{
	vector<int>degit;
	while (count >= 10)
	{
		int tmp = count % 10;
		degit.push_back(tmp);
		count /= 10;
	}
	degit.push_back(count);

	auto itr = degit.begin();
	//二桁のモード選択
	if (dataB != nullptr)
	{

		numberModel[*itr]->Update(data);
		Draw3DObject(*numberModel[*itr]);
		itr++;
		numberModel[*itr]->Update(dataB);
		Draw3DObject(*numberModel[*itr]);

#pragma region testDraw
		//int testCount = 8;
		//numberModel[testCount]->Update(data);
		//Draw3DObject(*numberModel[testCount]);
		//itr++;
		//numberModel[testCount]->Update(dataB);
		//Draw3DObject(*numberModel[testCount]);
#pragma endregion

	}//一桁のモード選択
	else if (data != nullptr)
	{
		numberModel[*itr]->Update(data);
		Draw3DObject(*numberModel[*itr]);
		itr++;
	}//通常の数字の表示
	else
	{
		for (int i = degit.size(); i > 0; i--)
		{
			numberModel[*itr]->Update(&CountDrawData[i]);
			Draw3DObject(*numberModel[*itr]);
			itr++;
		}
	}
}

void OthelloManager::SetGameModeSelectPanel(int startPos)
{
	AllDeadPanel();

	//SetSpawnPanel(normaMode.x, normaMode.y, true, STOP, false);
	SetSpawnPanel(ScoreAttackPanel.x, ScoreAttackPanel.y, true, STOP, false);
	SetSpawnPanel(tutorialPanel.x, tutorialPanel.y, true, STOP, false);
	SetSpawnPanel(DojoPanel.x, DojoPanel.y, true, STOP, false);


	//条件に寄って初期位置が変化する
	bool isDefault = startPos <= 0;
	if (isDefault)
	{
		SetSpawnPlayer(7, 7);
	}
	else
	{
		if (startPos >= NormaStartOthellos.size())
		{
			startPos = NormaStartOthellos.size();
		}
		startPos--;
		int x = startPos % 8;
		int y = startPos / 8;
		SetSpawnPlayer(x, y);
	}
}


void OthelloManager::SetDojoModeSelectPanel(int startPos)
{
	AllDeadPanel();
	bool stageEnd = false;

	//SetSpawnPanel(normaMode.x, normaMode.y, true, STOP, false);
	SetSpawnPanel(ConectPanel.x, ConectPanel.y, true, STOP, false);
	SetSpawnPanel(WaltzPanel.x, WaltzPanel.y, true, STOP, false);
	SetSpawnPanel(BookendPanel.x, BookendPanel.y, true, STOP, false);
	SetSpawnPanel(UndoPanel.x, UndoPanel.y, true, STOP, false);



	//条件に寄って初期位置が変化する
	bool isDefault = startPos <= 0;
	if (isDefault)
	{
		SetSpawnPlayer(7, 7);
	}
	else
	{
		if (startPos >= NormaStartOthellos.size())
		{
			startPos = NormaStartOthellos.size();
		}
		startPos--;
		int x = startPos % 8;
		int y = startPos / 8;
		SetSpawnPlayer(x, y);
	}
}

void OthelloManager::SetPuzzleModeSelectPanel(int startPos)
{
	AllDeadPanel();
	bool stageEnd = false;
	for (int y = 0; y < 7; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			stageEnd = ((y * 8) + x) >= NormaStartOthellos.size();
			if (stageEnd)
			{
				break;
			}
			SetSpawnPanel(x, y, false, STOP, false);
		}
		if (stageEnd)
		{
			break;
		}
	}
	//SetSpawnPanel(normaMode.x, normaMode.y, true, STOP, false);

	//条件に寄って初期位置が変化する
	bool isDefault = startPos <= 0;
	if (isDefault)
	{
		SetSpawnPlayer(7, 7);
	}
	else
	{
		if (startPos >= NormaStartOthellos.size())
		{
			startPos = NormaStartOthellos.size();
		}
		startPos--;
		int x = startPos % 8;
		int y = startPos / 8;
		SetSpawnPlayer(x, y);
	}
}

void OthelloManager::SetCountPos(bool isNormaMode)
{
	XMVECTOR movePos = moveTextPos;
	if (!isNormaMode)
	{
		movePos = FloatAnimationDistance * textAnimationRate;
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			int count = (i * 10) + j;
			normaCountTex[count].position = XMVECTOR{ static_cast<float>(window_width / 2 - 10 + (i * 50) + 20), 10.0f, 0, 0 };
			normaCountTex[count].position += movePos;
		}
	}
}

void OthelloManager::NormaComboTextSetPos(bool isNormaMode)
{
	XMVECTOR movePos = moveTextPos;
	if (!isNormaMode)
	{
		movePos = FloatAnimationDistance * textAnimationRate;
	}
	float changeScale = 0.5f;
	NormaComboText.position = XMVECTOR{ 640 - (219 * changeScale / 2) - 100,  (72 * changeScale - 5), 0, 0 };
	NormaComboText.position += movePos;
}

void OthelloManager::NormaPanelsTextSetPos(bool isNormaMode)
{
	XMVECTOR movePos = moveTextPos;
	if (!isNormaMode)
	{
		movePos = FloatAnimationDistance * textAnimationRate;
	}
	float changeScale = 0.5f;

	NormaPanelsText.position = XMVECTOR{ 640 - (444 * changeScale / 2) - 150,  (72 * changeScale - 5), 0, 0 };
	NormaPanelsText.position += movePos;
}

void OthelloManager::NormaScoreTextSetPos(bool isNormaMode)
{
	XMVECTOR movePos = moveTextPos;
	if (!isNormaMode)
	{
		movePos = FloatAnimationDistance * textAnimationRate;
	}
	float changeScale = 0.5f;

	NormaScoreText.position = XMVECTOR{ 640 - (221 * changeScale / 2) - 100,  (68 * changeScale - 5), 0, 0 };
	NormaScoreText.position += movePos;
}


void OthelloManager::SubNormaTextPos(bool isNormaMode)
{
	XMVECTOR movePos = moveSubTextPos;
	if (!isNormaMode)
	{
		movePos = FloatAnimationDistance * textAnimationRate;
	}
	float changeScale = 0.5f;

	NormaPanelsText.position = XMVECTOR{ 640 - (444 * changeScale / 2) - 150,  (72 * changeScale - 5), 0, 0 };
	NormaPanelsText.position += moveTextPos;
	NormaPanelsText.position += movePos;
}
void OthelloManager::SetTextPos(bool isNormaMode, int stageNum)
{
	NormaComboTextSetPos(isNormaMode);
	NormaScoreTextSetPos(isNormaMode);
	SetCountPos(isNormaMode);
	SubNormaTextPos(isNormaMode);

	if (GetEnterModeType() == GameMode::ScoreAttack)
	{
		SetScoreAttackTextPos();
	}
	else
	{
		SetModeSelectEachInfo(ScoreAttackDrawData, ScoreAttackPanel);
	}

	if (nowType == GameModeSelect && playerPanelPos == DojoPanel)
	{
		//敵固ぴ
		TechTrainingTextDrawData.position = scoreAttackTextPos + (FloatAnimationDistance * textAnimationRate);
		SetPickupModeEachInfo(TechTrainingTextDrawData);
		TechTrainingTextDrawData.scale = { 0.5f,0.5f ,0.5f };
	}
	else
	{
		SetModeSelectEachInfo(TechTrainingTextDrawData, DojoPanel);
	}

	if (nowType == DojoSelect && playerPanelPos == UndoPanel)
	{
		//敵固ぴ
		UndoTextDrawData.position = scoreAttackTextPos + (FloatAnimationDistance * textAnimationRate);
		SetPickupModeEachInfo(UndoTextDrawData);
		UndoTextDrawData.scale = { 0.5f,0.5f ,0.5f };
	}
	else
	{
		SetModeSelectEachInfo(UndoTextDrawData, UndoPanel);
	}

	if (nowType == DojoSelect && playerPanelPos == ConectPanel)
	{
		//敵固ぴ
		ConnectTextDrawData.position = scoreAttackTextPos + (FloatAnimationDistance * textAnimationRate);
		SetPickupModeEachInfo(ConnectTextDrawData);
		ConnectTextDrawData.scale = { 0.5f,0.5f ,0.5f };
	}
	else
	{
		SetModeSelectEachInfo(ConnectTextDrawData, ConectPanel);
	}

	if (nowType == DojoSelect && playerPanelPos == WaltzPanel)
	{
		//敵固ぴ
		WaltzTextDrawData.position = scoreAttackTextPos + (FloatAnimationDistance * textAnimationRate);
		SetPickupModeEachInfo(WaltzTextDrawData);
		WaltzTextDrawData.scale = { 0.5f,0.5f ,0.5f };
	}
	else
	{
		SetModeSelectEachInfo(WaltzTextDrawData, WaltzPanel);
	}

	if (nowType == DojoSelect && playerPanelPos == BookendPanel)
	{
		//敵固ぴ
		BookendTextDrawData.position = scoreAttackTextPos + (FloatAnimationDistance * textAnimationRate);
		SetPickupModeEachInfo(BookendTextDrawData);
		BookendTextDrawData.scale = { 0.5f,0.5f ,0.5f };
	}
	else
	{
		SetModeSelectEachInfo(BookendTextDrawData, BookendPanel);
	}

	list<NormaModeFieldData>::iterator data;
	if (isNormaMode)
	{
		data = GetNormaStage(stageNum);
	}
	else
	{
		data = GetNormaStage(GetEnterNormaStage());
	}
	if (data->type == Norma::Combo)
	{
		NormaComboModelSetPos(isNormaMode);
	}
	else if (data->type == Norma::Panels)
	{
		NormaPanelsModelSetPos(isNormaMode);
	}
	else if (data->type == Norma::Score)
	{
		NormaScoreModelSetPos(isNormaMode);
	}

	SetCountModelPos(isNormaMode, stageNum);
	SubNormaModelPos(isNormaMode);
}

void OthelloManager::NormaComboModelSetPos(bool isNormaMode)
{
	XMVECTOR movePos = moveTextModelPos;
	if (!isNormaMode)
	{
		movePos = FloatAnimationDistance * textAnimationRate;
	}
	float changeScale = 0.5f;

	NormaDrawData.position = comboScoreModelPos;
	NormaDrawData.position += movePos;

}
void OthelloManager::NormaPanelsModelSetPos(bool isNormaMode)
{
	XMVECTOR movePos = moveTextModelPos;
	if (!isNormaMode)
	{
		movePos = FloatAnimationDistance * textAnimationRate;
	}
	float changeScale = 0.5f;
	NormaDrawData.position = allDeleteModelPos;
	NormaDrawData.position += movePos;

}
void OthelloManager::NormaScoreModelSetPos(bool isNormaMode)
{
	XMVECTOR movePos = moveTextModelPos;
	if (!isNormaMode)
	{
		movePos = FloatAnimationDistance * textAnimationRate;
	}
	NormaDrawData.position = comboScoreModelPos;
	NormaDrawData.position += movePos;
}


void OthelloManager::SetScoreAttackTextPos()
{
	ScoreAttackDrawData.position = scoreAttackTextPos + (FloatAnimationDistance * textAnimationRate);
	SetPickupModeEachInfo(ScoreAttackDrawData);
	ScoreAttackDrawData.scale = { 0.5f,0.5f ,0.5f };

}

void OthelloManager::SetModeSelectEachInfo(EachInfo &data, const panelPos &pos)
{
	Othello panelData;
	panelData.Spawn(NORMAL, pos.x, pos.y);
	data.position = ConvertXMFLOAT3toXMVECTOR(panelData.GetPosition());
	data.position.m128_f32[2] += -1.0f;
	//data.position += FloatAnimationDistance * textAnimationRate;
	data.scale = { 0.2,0.2 ,0.2 };
	data.rotation = { -30, 0, 0 };
}

void OthelloManager::SetPickupModeEachInfo(EachInfo &data)
{
	data.rotation = { -30, 0, 0 };
}

void OthelloManager::SetCountModelPos(bool isNormaMode, int stageNum)
{
	XMVECTOR movePos = moveTextModelPos;
	list<NormaModeFieldData>::iterator data = GetNormaStage(stageNum);
	if (!isNormaMode)
	{
		movePos = FloatAnimationDistance * textAnimationRate;
		data = GetNormaStage(GetEnterNormaStage());
	}
	if (GetEnterNormaStage() == GameMode::ScoreAttack)
		moveAllDeleteCountPos;
	XMVECTOR baseCountPos = XMVECTOR{ 1.0f , 13.0f, -1.0f ,0 };

	if (data->type == Norma::Panels)
	{
		baseCountPos = XMVECTOR{ 2.4f , 13.0f, -1.0f ,0 };
	}
	for (int i = 0; i < 5; i++)
	{
		CountDrawData[i].position = baseCountPos + XMVECTOR{ (i * 1.5f), 0.0f, 0.0f ,0 };
		CountDrawData[i].position += movePos;
	}
}
void OthelloManager::SubNormaModelPos(bool isNormaMode)
{
	XMVECTOR movePos = moveSubTextModelPos;
	if (!isNormaMode)
	{
		movePos = FloatAnimationDistance * textAnimationRate;
	}
	float changeScale = 0.5f;

	SubNormaDrawData.position = allDeleteModelPos;
	SubNormaDrawData.position += moveTextModelPos;
	SubNormaDrawData.position += movePos;
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
	if (!isAct)
	{
		return sendDatas;
	}
	//今存在する駒を突っ込んでいく
	auto itr = othellos.begin();
	for (; itr != othellos.end(); itr++)
	{
		OthelloData gameDatas = *itr->GetGameData();
		SendOthelloData data;
		if (gameDatas.isPlayer || gameDatas.type == OthelloType::WALL)
		{
			continue;
		}
		data.FrontActiveAngle = gameDatas.FrontActiveAngle;
		data.isFront = gameDatas.isFront;
		data.type = gameDatas.type;
		data.chainName = gameDatas.chainName;
		data.maxComboCount = gameDatas.maxComboCount;
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

		if (A() && isOnPlayer)
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

void OthelloManager::Receive(const vector<vector<SendOthelloData>> &data, const vector<pair<int, int>> compPos)
{
	sendDatas = data;

	if (othellos.size() == 0 || !isAct)
	{
		return;
	}
	auto itr = othellos.begin();

	int chainName = itr->GetGameData()->chainName;


	list<list<Othello>::iterator> sandOthellos;
	list<list<Othello>::iterator> repairOthellos;
	//生きているやつを挟んだフラグ
	bool isSandFlip = false;
	for (; itr != othellos.end(); itr++)
	{
		OthelloData *gameDatas = itr->GetGameData();

		int x = gameDatas->widthPos;
		int y = gameDatas->heightPos;

		itr->isCompleteDraw = false;
		for (auto &e : compPos)
		{
			bool isComplete = (x == e.second && y == e.first);
			if (isComplete)
			{
				itr->isCompleteDraw = true;
				break;
			}
		}
		gameDatas->score = sendDatas[y][x].score;
		gameDatas->chainName = sendDatas[y][x].chainName;
		gameDatas->maxComboCount = sendDatas[y][x].maxComboCount;
		if (sendDatas[y][x].type == NONE/* || gameDatas->isSpawn*/)
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
		data.Init(&oserroModel, &stopOserroModel, &compModel);
		data.Spawn(NORMAL, x, y, true);
		data.Update(0);
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
	data.Init(&oserroModel, &stopOserroModel, &compModel);

	bool randFront = rand() % 2;
	if (isInGame)
	{
		data.Borne(NORMAL, x, y, randFront);
	}
	else
	{
		data.Spawn(NORMAL, x, y, randFront);
	}
	data.Update(0);
	data.GetGameData()->isMove = false;
	if (x == playerPanelPos.x && y == playerPanelPos.y)
	{
		data.GetGameData()->isPlayer = true;
	}
	othellos.push_back(data);
}

void OthelloManager::MinSpawn(bool inGame)
{
	if (othellos.size() >= minPanelCount)return;

	int minPanelUnderCount = minPanelCount - othellos.size();

	for (int i = 0; i < minPanelUnderCount; i++)
	{
		SpawnPanel(inGame);
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

	//if (Input::MouseTrigger(MouseButton::RBUTTON))
	//{
	//	SetPanel();
	//}
}

void OthelloManager::KeySetPlayer()
{
	isFieldUpdate = false;

	if (pressPos.size() > 0)
	{
		pressPos.clear();
	}
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
	const float deadAngle = 0.0f;
	bool padD = directInput->IsButtonPush(DirectInput::ButtonKind::RightButton) && directInput->leftStickX() >= -deadAngle && directInput->leftStickX() <= deadAngle && directInput->leftStickY() <= deadAngle && directInput->leftStickY() >= -deadAngle;;
	bool padS = directInput->IsButtonPush(DirectInput::ButtonKind::DownButton) && directInput->leftStickX() >= -deadAngle && directInput->leftStickX() <= deadAngle && directInput->leftStickY() <= deadAngle && directInput->leftStickY() >= -deadAngle;
	bool padA = directInput->IsButtonPush(DirectInput::ButtonKind::LeftButton) && directInput->leftStickX() >= -deadAngle && directInput->leftStickX() <= deadAngle && directInput->leftStickY() <= deadAngle && directInput->leftStickY() >= -deadAngle;
	bool padW = directInput->IsButtonPush(DirectInput::ButtonKind::UpButton) && directInput->leftStickX() >= -deadAngle && directInput->leftStickX() <= deadAngle && directInput->leftStickY() <= deadAngle && directInput->leftStickY() >= -deadAngle;

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

	if (x == playerPanelPos.x && y == playerPanelPos.y || !isAct)
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
	data.Init(&oserroModel, &stopOserroModel, &compModel);

	bool randFront = rand() % 2;
	data.Spawn(NORMAL, x, y, randFront);
	data.GetGameData()->isMove = false;
	data.GetGameData()->isPlayer = false;
	othellos.push_back(data);

	playerPanelPos = { x, y };

	MinSpawn(false);
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
			nextStep = 1.0f - (static_cast<float>(nextItr->GetGameData()->spawnTimer) / spawnAnimationTimerMax);
			if (OnPlayer)
			{
				nowStep = 1.0f - static_cast<float>(playerItr->GetGameData()->spawnTimer) / spawnAnimationTimerMax;
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
	bool isNextPanel = nextItr != othellos.end();
	bool isSpawnPanel = isNextPanel;
	if (isNextPanel)
	{
		if (nextItr->GetGameData()->isSpawn)
		{
			int a = nextItr->GetGameData()->spawnTimer;
			float nowScale = static_cast<float>(a) / spawnAnimationTimerMax;
			isSpawnPanel = nowScale >= 0.6f;
		}
	}
	//移動先にパネルがあるか
	if (isNextPanel && isSpawnPanel)
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
				//nowStep = 1.0f - static_cast<float>(playerItr->GetGameData()->vanishTimer) / vanishTimerMax;
				nowStep = 1.0f;
			}
		}
		float stepSize = nextStep - nowStep;
		bool isStepUp = stepSize <= panelWallRate;
		bool isNotMove = (nextItr->GetGameData()->isReverce || !isStepUp || nextItr->GetGameData()->type == OthelloType::WALL);
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
			if (isNextPanel && !playerItr->GetGameData()->isSandwich)
			{
				nextItr->GetGameData()->isDead = true;
				pressPos.push_back(nextItr->GetPosition());
			}
			bool isNotMovePanel = (playerItr->GetGameData()->isVanish || playerItr->GetGameData()->isSpawn || playerItr->GetGameData()->isSandwich);
			bool isNotDown = (downStepCount < downStepCountMax &&isNotMovePanel);
			if (isNotDown)
			{
				DownStep(playerItr);
				return;
			}
			isPanelMove = true;
			if (!playerItr->GetGameData()->isVanish && !playerItr->GetGameData()->isReverce && !playerItr->GetGameData()->isSandwich && !playerItr->GetGameData()->isSpawn && (playerItr->GetGameData()->type == OthelloType::NORMAL))
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

	EraseChanceObject();
}

void OthelloManager::AllDeletePanel()
{
	if (othellos.size() <= 0)
	{
		return;
	}
	othellos.clear();
	EraseChanceObject();

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
			data.Init(&oserroModel, &stopOserroModel, &compModel);
			bool randFront = rand() % 2;
			int x = playerPanelPos.x;
			int y = playerPanelPos.y;
			data.Borne(NORMAL, x, y, randFront);
			data.Update(0);
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

void TextModel::Update(EachInfo *each, XMMATRIX *addRot)
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
		if (addRot != nullptr)
		{
			matRot *= *addRot;
		}
		matTrans = XMMatrixTranslation(this->each.position.m128_f32[0], this->each.position.m128_f32[1], this->each.position.m128_f32[2]);
		matWorld = XMMatrixIdentity();

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

		ConstBufferDataB0 *constMap0 = nullptr;
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

		ConstBufferDataB0 *constMap0 = nullptr;
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
	AllDeletePanel();

	SetPlayerAndPanel(2, 4, false);
	SetSpawnPanel(4, 4, false);
	SetSpawnPanel(5, 4, true);

	scenes = TutorialSceneFlow::SandwichUpdate;
	TutorialTimer = 0;
	isTutorialClear = false;
}

void OthelloManager::whyChainSpawn()
{
	AllDeletePanel();

	SetSpawnPanel(4, 2, true);
	SetSpawnPanel(4, 3, false);
	SetPlayerAndPanel(3, 4, true);
	SetSpawnPanel(4, 4, false);
	SetSpawnPanel(5, 4, true);

	scenes = TutorialSceneFlow::ChainUpdate;
	TutorialTimer = 0;
	isTutorialClear = false;

}

void OthelloManager::EraseChanceObject()
{

	for (auto &e : chances)
	{
		e.isDraw = false;
	}
}

void OthelloManager::SpawnChances(const vector<pair<int, int>> &pos)
{
	EraseChanceObject();
	bool isNothing = (pos.size() <= 0);
	for (auto &e : pos)
	{
		int pos = (e.first * 8) + (e.second);
		chances[pos].isDraw = true;
	}
}

void OthelloManager::TutorialStart()
{
	whySandwichSpawn();
}

void OthelloManager::whyStepSpawn()
{
	AllDeletePanel();

	SetSpawnPlayer(4, 5);
	SetSpawnPanel(3, 4, true);
	SetSpawnPanel(4, 4, false);
	SetSpawnPanel(5, 4, true);

	scenes = TutorialSceneFlow::StepUpdate;
	TutorialTimer = 0;
	TutorialEndTextCount = 0;
	isTutorialClear = false;

}

void OthelloManager::SetSpawnPanel(int x, int y, bool Front, OthelloType type, bool isRockDraw, bool isSpawn)
{
	Othello data;
	if (type == WALL)
	{//タイプが壁だったら
		data.Init(&wallOserroModel, &stopOserroModel, &compModel);
	}
	else if (type == STOP)
	{//タイプが壁だったら
		data.Init(&oserroModel, &stopOserroModel, &compModel);
		data.SetIsRockDraw(isRockDraw);
	}
	else
	{
		data.Init(&oserroModel, &stopOserroModel, &compModel);
	}
	if (isSpawn)
	{
		data.Spawn(type, x, y, Front);
	}
	else
	{
		data.Borne(type, x, y, Front);
	}
	if (type == WALL)
	{//タイプが壁だったら大きくしてる
		data.SetScale(XMFLOAT3(1, 1, 1));
	}

	data.Update(0);
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
	each.scale = { 1.0f , 1.0f ,  0.1f };
	each.position += ConvertXMFLOAT3toXMVECTOR(stageLeftTop);
	each.alpha = 0.5f;
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
	if (Input::KeyTrigger(DIK_Z) || directInput->IsButtonPush(directInput->Button02))
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
			SetSpawnPanel(panelItr->x, panelItr->y, panelItr->isFront, static_cast<OthelloType>(panelItr->type));
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

void OthelloManager::StartNormaMode(int stageNum)
{
	normaChecker.Reset();
	StartNormaField(stageNum);
	isNormaMode = true;
}

list<NormaModeFieldData>::iterator OthelloManager::GetNormaStage(int num)
{
	auto itr = NormaStartOthellos.begin();

	if (NormaStartOthellos.size() > num)
	{
		for (int i = 1; i < num; i++)
		{
			itr++;
		}
	}
	return itr;
}

void OthelloManager::StartNormaField(int stageNum)
{
	//ステージが存在しない
	bool isNoneStage = NormaStartOthellos.size() <= 0;
	//指定されたステージ番号がステージ数を超過している
	bool isStageOver = NormaStartOthellos.size() < (stageNum);

	if (isNoneStage || isStageOver)return;

	//ステージ番号に応じたイテレーターを呼び出す
	auto stageItr = NormaStartOthellos.begin();
	for (int i = 1; i < stageNum; i++)
	{
		stageItr++;
	}

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
		SetSpawnPanel(panelItr->pos.x, panelItr->pos.y, panelItr->isFront, panelItr->type);
	}
	SetSpawnPlayer(stageItr->playerPos.x, stageItr->playerPos.y);
	normaChecker.SetNorma(stageItr->type, stageItr->normaStatus, stageItr->normaMoveCount, stageItr->subNormaFlag, stageItr->subNormaPanels);
}

void OthelloManager::TestStage()
{
	panelData a, b, c;
	NormaModeFieldData testStage;

	a.isFront = true;
	a.pos = { 0, 0 };
	a.type = STOP;

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
	//パネル状況リセット
	normaChecker.Reset();
	//スコア状況もリセット
	nowScore = 0;
}

bool OthelloManager::GetIsNormaClear()
{
	return normaChecker.GetClear();
}

bool OthelloManager::GetIsNormaFailed()
{

	return normaChecker.GetFailed();
}

void OthelloManager::LoadNormaStage(string stage, list<NormaModeFieldData> &stageList)
{
	std::ifstream file;
	const string stageFileName = "Resource/Stage/" + stage + ".txt";

	file.open(stageFileName);

	if (file.fail())
	{
		file.close();
		return;
	}

	string line;
	int yPos = 0;
	NormaModeFieldData NormaField;
	while (getline(file, line))
	{
		//','を' 'に変換
		replace(line.begin(), line.end(), ',', ' ');
		istringstream line_Data(line);

		string key;
		getline(line_Data, key, ' ');

		//情報がノルマだった場合の処理
		if (key == "n")
		{
			string test;
			line_Data >> test;
			if (*test.begin() == 'C' || *test.begin() == 'c')
			{
				NormaField.type = Norma::Combo;
			}
			else if (*test.begin() == 'S' || *test.begin() == 's')
			{
				NormaField.type = Norma::Score;
			}
			else if (*test.begin() == 'P' || *test.begin() == 'p')
			{
				NormaField.type = Norma::Panels;
			}
			else
			{
				NormaField.type = Norma::Panels;
			}

			int norma = 0;
			line_Data >> norma;
			NormaField.normaStatus = norma;

			int hoge = 0;
		}

		//情報が歩数だった場合の処理
		if (key == "w")
		{
			int warkCount = 0;
			line_Data >> warkCount;
			NormaField.normaMoveCount = warkCount;
			int hoge = 0;

		}
		//情報が位置情報だったら
		if (key == "p")
		{
			panelPos playerPos = {};
			line_Data >> playerPos.x;
			line_Data >> playerPos.y;
			NormaField.playerPos = playerPos;

			int hoge = 0;
		}

		//書かなければ問題ないように
		if (key == "d")
		{

			int Count;
			line_Data >> Count;
			NormaField.subNormaPanels = Count;
			NormaField.subNormaFlag = true;
		}
		//情報が盤面に関する情報だった場合
		if (key == "s")
		{
			int panelID = 0;
			int hoge = 0;

			for (int i = 0; i < 8; i++)
			{
				line_Data >> panelID;
				//一文字ずつ確認

				//表
				if (panelID == 1)
				{
					panelData panel;
					panel.pos = { i, yPos };
					panel.isFront = true;
					NormaField.panels.push_back(panel);
				}
				//裏
				else if (panelID == 2)
				{
					panelData panel;
					panel.pos = { i, yPos };
					panel.isFront = false;
					NormaField.panels.push_back(panel);
				}
				//固定表
				else if (panelID == 3)
				{
					panelData panel;
					panel.pos = { i, yPos };
					panel.isFront = true;
					panel.type = STOP;
					NormaField.panels.push_back(panel);
				}
				//固定裏
				else if (panelID == 4)
				{
					panelData panel;
					panel.pos = { i, yPos };
					panel.isFront = false;
					panel.type = STOP;
					NormaField.panels.push_back(panel);
				}
				//壁
				else if (panelID == 5)
				{
					panelData panel;
					panel.pos = { i, yPos };
					panel.isFront = true;
					panel.type = WALL;
					NormaField.panels.push_back(panel);
				}
			}
			yPos++;

		}
	}
	stageList.push_back(NormaField);
}

void OthelloManager::LoadAllNormaStage()
{
	//LoadNormaStage("test");

	string baseName = "stage";
	for (int i = 0; i <= NormaStageCount; i++)
	{
		string count = to_string(i + 1);
		LoadNormaStage(baseName + count, NormaStartOthellos);
	}

	int stageCount = NormaStartOthellos.size();
}

int OthelloManager::GetNormaStagesCount()
{
	return NormaStartOthellos.size();
}

std::list<XMFLOAT3> &OthelloManager::GetPressPanellPos()
{
	return pressPos;
}

bool operator==(const panelPos &a, const panelPos &b)
{
	return (a.x == b.x && a.y == b.y);
}

WaveEase::WaveEase(int timer, int  Max, bool isWaveUp) :
	timer(timer),
	MaxTime(Max),
	isWaveUp(isWaveUp)
{

}

void WaveEase::Set(int timer, int  Max, bool isWaveUp)
{
	this->timer = timer;
	this->MaxTime = Max;
	this->isWaveUp = isWaveUp;
}

float OthelloEase::Wave(WaveEase &obj, int powCount)
{
	if (obj.timer >= obj.MaxTime)
	{
		obj.timer = 0;
		obj.isWaveUp = !obj.isWaveUp;
	}
	else
	{
		obj.timer++;
	}
	float ret = static_cast<float>(obj.timer) / static_cast<float>(obj.MaxTime);

	if (obj.isWaveUp)
	{
		ret = ShlomonMath::EaseInOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, ret).x;
	}
	else
	{
		ret = ShlomonMath::EaseInOutQuad(XMFLOAT3{ 1, 0, 0 }, XMFLOAT3{}, ret).x;
	}
	ret = pow(ret, powCount);

	return ret;
}

float OthelloEase::WaveInOut(WaveEase &obj, int powCount)
{
	if (obj.timer >= obj.MaxTime)
	{
		obj.timer = 0;
		obj.isWaveUp = !obj.isWaveUp;
	}
	else
	{
		obj.timer++;
	}
	float ret = static_cast<float>(obj.timer) / static_cast<float>(obj.MaxTime);

	if (obj.isWaveUp)
	{
		ret = ShlomonMath::EaseOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, ret).x;
	}
	else
	{
		ret = ShlomonMath::EaseInQuad(XMFLOAT3{ 1, 0, 0 }, XMFLOAT3{}, ret).x;
	}
	ret = pow(ret, powCount);

	return ret;

}

bool OthelloManager::A()
{
	return (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01));
}
