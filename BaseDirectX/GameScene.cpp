#include "../VoiceUDPRecive/VoiceUDPRecive.h"
#include"GameScene.h"
#include "../WindowsAPI/WinAPI.h"
#include "Input.h"
#include "../Sound/Sound.h"
#include "viewport.h"
#include "../Particle/Particle3D.h"
#include "../imgui/ImguiControl.h"
#include "vec3.h"
#include "../FBXObject/FBXObject.h"
#include "../Shader/ShaderManager.h"
#include "../3DObjectParticle/3DObjectParticle.h"
#include "../OthlloPlayer/OthlloPlayer.h"
#include "../Thunder/Thunder.h"
#include "../LightObject/LightObject.h"

GameScene::GameScene()
{
	SceneNum = TITLE;
}

GameScene::~GameScene()
{
	VoiceReciver::EndRecive();
	delete(light);
}

void GameScene::SceneManageUpdateAndDraw()
{
	Input::Update();
	WindowsAPI::CheckMsg();
	light->Update();
	switch (SceneNum)
	{
	case TITLE:
		TitleUpdate();
		TitleDraw();
		break;
	case SELECT:
		SelectUpdate();
		SelectDraw();
		break;
	case GAME:
		GameUpdate();
		GameDraw();
		break;
	case RESULT:
		ResultUpdate();
		ResultDraw();
		break;
	case END:
		EndUpdate();
		EndDraw();
		break;
	default:
		break;
	}
}

void GameScene::Init()
{
	BaseDirectX::Set();
	//サウンド
	Sound::CreateVoice();
	//SRVのアドレス確保
	BaseDirectX::GetAdress();
	//カメラ初期化
	Camera::Init();
	//Camera::eye.v = gameNowEye;
	Camera::eye.v = { -1, 1, -15 };
	//Camera::target.v = gameOpTarget;
	Camera::target.v = { -1, 1, 0 };
	Camera::Update();
	//Imguiの初期化
	Imgui::Init();
	//ライトの初期化
	Light::StaticInitialize(BaseDirectX::dev.Get());
	ShaderManager::LoadShaders();
	// 3Dパーティクル静的初期化
	ParticleControl::Init();
	//インプット初期化
	Input::KeySet(WindowsAPI::w, WindowsAPI::hwnd);
	//FBX系
	FbxLoader::GetInstance()->Initialize(BaseDirectX::dev.Get());
	FBXObject::SetDevice(BaseDirectX::dev.Get());
	FBXObject::CreateGraphicsPipeline();
	//ライト初期化
	light = Light::Create();
	//モデルすべてにライトを適用
	Model::SetLight(light);

	//ポストエフェクトの初期化
	//PostEffects::Init();

	/*model = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	object = new FBXObject;
	object->Initialize();
	object->SetModel(model);
	object->PlayAnimation();*/
	//sample.CreateModel("newOserro", ShaderManager::playerShader);
	//sample.each.rotation.x = 0;


	checkObject.Init();
	ObjectParticles::LoadModels();

	title.CreateSprite(L"Resource/Img/titel.png", XMFLOAT3(70, 60 + 50, 0));
	titleBack.CreateSprite(L"Resource/Img/title_back.png", XMFLOAT3(0, 46 + 50, 0));
	spaceBack.CreateSprite(L"Resource/Img/title_back.png", XMFLOAT3(0, 0, 0));
	space.CreateSprite(L"Resource/Img/push_space.png", XMFLOAT3(650, 550, 0));
	sceneChage.CreateSprite(L"Resource/Img/SceneChange.png", XMFLOAT3(window_width / 2, window_height / 2, 0));
	num[0].LoadGraph(L"Resource/Img/number_0.png");
	num[1].LoadGraph(L"Resource/Img/number_1.png");
	num[2].LoadGraph(L"Resource/Img/number_2.png");
	num[3].LoadGraph(L"Resource/Img/number_3.png");
	num[4].LoadGraph(L"Resource/Img/number_4.png");
	num[5].LoadGraph(L"Resource/Img/number_5.png");
	num[6].LoadGraph(L"Resource/Img/number_6.png");
	num[7].LoadGraph(L"Resource/Img/number_7.png");
	num[8].LoadGraph(L"Resource/Img/number_8.png");
	num[9].LoadGraph(L"Resource/Img/number_9.png");
	//addReverse.CreateSprite(L"Resource/Img/combo.png", XMFLOAT3(0, 0, 0));
	scoreSprite.CreateSprite(L"Resource/Img/score.png", XMFLOAT3(0, 0, 0));
	timeUp.CreateSprite(L"Resource/Img/time_up.png", XMFLOAT3(0, 0, 0));
	startSprite.CreateSprite(L"Resource/Img/START.png", XMFLOAT3(0, 0, 0));
	gameTime = gameMaxTime;
	Lights::LoadModels();
	OthlloPlayer::Init();
	sky.CreateModel("world", ShaderManager::playerShader, true);
	sky.each.rotation = { 70, 0.0f, 0 };
	sky.each.scale = { 100.0f ,100.0f, 100.f };
	othelloStage.CreateModel("board", ShaderManager::playerShader);
	othelloStage.each.position.m128_f32[0] = -1.0f;
	othelloStage.each.position.m128_f32[1] = 1.0f;
	othelloStage.each.position.m128_f32[2] = 2.5f;
	othelloStage.each.rotation.x = 90;
	othelloStage.each.scale = { 2.0f, 1.0f, 2.0f };
	isPouse = false;
	pouseBack.CreateSprite(L"Resource/Img/title_back_80.png", XMFLOAT3(0, 0, 0));
	isGameEnd = false;
	moveSprite.CreateSprite(L"Resource/Img/control.png", XMFLOAT3(0, 0, 0));
	reverseSprtie.CreateSprite(L"Resource/Img/reverse.png", XMFLOAT3(0, 0, 0));
	backSprite.CreateSprite(L"Resource/Img/back.png", XMFLOAT3(0, 0, 0));
	endSprite.CreateSprite(L"Resource/Img/end.png", XMFLOAT3(0, 0, 0));
	titleSprite.CreateSprite(L"Resource/Img/go_title.png", XMFLOAT3(0, 0, 0));
	kagikakkoStartSprite.CreateSprite(L"Resource/Img/select_big_1.png", XMFLOAT3(0, 0, 0));
	kagikakkoEndSprite.CreateSprite(L"Resource/Img/select_big_0.png", XMFLOAT3(0, 0, 0));
	kagikakkoSelectStartSprite.CreateSprite(L"Resource/Img/select_0.png", XMFLOAT3(0, 0, 0));
	kagikakkoSelectEndSprite.CreateSprite(L"Resource/Img/select_1.png", XMFLOAT3(0, 0, 0));
	tutorialSprite.CreateSprite(L"Resource/Img/tutorial.png", XMFLOAT3(0, 0, 0));
	gameScoreAttackSprite.CreateSprite(L"Resource/Img/score_attack.png", XMFLOAT3(0, 0, 0));
	gameNormaSprite.CreateSprite(L"Resource/Img/quota.png", XMFLOAT3(0, 0, 0));
	titleSelectNum = 0;
	SoundLoad("Resource/Sound/select_.wav", selectSound);
	SoundLoad("Resource/Sound/time_up_.wav", timeUpSound);
	SoundLoad("Resource/Sound/enter_.wav", enterSound);
	SoundLoad("Resource/Sound/bgm_.wav", BGMSound);
	SoundLoad("Resource/Sound/start_.wav", startSound);
	SoundLoad("Resource/Sound/count_down_.wav", countdDownSound);
	sceneChangeSprite2.CreateSprite(L"Resource/Img/SceneChange.png", XMFLOAT3(-1280, -720, 0));
	checkObject.SoundInit();
	selectGameTypeActive = false;
	selectGameType = 1;
	SoundPlayLoop(BGMSound);


	const float OthelloR = 1.8f;
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			FrameEach each;
			each.CreateConstBuff0();
			each.CreateConstBuff1();
			each.rotation.x = 0;
			each.position.m128_f32[0] = -(20 / 2.0f * OthelloR) + (i * OthelloR);
			each.position.m128_f32[1] = -(12 / 2.0f * OthelloR) + (j * OthelloR);
			each.position.m128_f32[2] = -0.5f;
			each.scale = { 0.01f, 0.01f, 0.01f };
			opOthellos.push_back(each);
		}
	}
	const float OthelloR2 = 1.5f;
	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			FrameEach each;
			each.CreateConstBuff0();
			each.CreateConstBuff1();
			each.rotation.x = 0;
			each.position.m128_f32[0] = -(20 / 2.0f * OthelloR) + (i * OthelloR);
			each.position.m128_f32[1] = -(12 / 2.0f * OthelloR) + (j * OthelloR);
			each.position.m128_f32[2] = 2.0f - j * 1.0f;
			each.scale = { 0.01f, 0.01f, 0.01f };
			each.rotation.x = -33.0f;
			pouseOthellos.push_back(each);
		}
	}

	//Timer管理用
	isChanged = false;
	timerCount = 0;
	size_x = MAX_SIZE_X;
	size_y = MAX_SIZE_Y;
	timerNumber[0].CreateModel("number0y", ShaderManager::playerShader);
	timerNumber[1].CreateModel("number1y", ShaderManager::playerShader);
	timerNumber[2].CreateModel("number2y", ShaderManager::playerShader);
	timerNumber[3].CreateModel("number3y", ShaderManager::playerShader);
	timerNumber[4].CreateModel("number4y", ShaderManager::playerShader);
	timerNumber[5].CreateModel("number5y", ShaderManager::playerShader);
	timerNumber[6].CreateModel("number6y", ShaderManager::playerShader);
	timerNumber[7].CreateModel("number7y", ShaderManager::playerShader);
	timerNumber[8].CreateModel("number8y", ShaderManager::playerShader);
	timerNumber[9].CreateModel("number9y", ShaderManager::playerShader);
	numbersObject[0].CreateModel("number0", ShaderManager::playerShader);
	numbersObject[1].CreateModel("number1", ShaderManager::playerShader);
	numbersObject[2].CreateModel("number2", ShaderManager::playerShader);
	numbersObject[3].CreateModel("number3", ShaderManager::playerShader);
	numbersObject[4].CreateModel("number4", ShaderManager::playerShader);
	numbersObject[5].CreateModel("number5", ShaderManager::playerShader);
	numbersObject[6].CreateModel("number6", ShaderManager::playerShader);
	numbersObject[7].CreateModel("number7", ShaderManager::playerShader);
	numbersObject[8].CreateModel("number8", ShaderManager::playerShader);
	numbersObject[9].CreateModel("number9", ShaderManager::playerShader);
	sNumbersObject[0].CreateModel("number0s", ShaderManager::playerShader);
	sNumbersObject[1].CreateModel("number1s", ShaderManager::playerShader);
	sNumbersObject[2].CreateModel("number2s", ShaderManager::playerShader);
	sNumbersObject[3].CreateModel("number3s", ShaderManager::playerShader);
	sNumbersObject[4].CreateModel("number4s", ShaderManager::playerShader);
	sNumbersObject[5].CreateModel("number5s", ShaderManager::playerShader);
	sNumbersObject[6].CreateModel("number6s", ShaderManager::playerShader);
	sNumbersObject[7].CreateModel("number7s", ShaderManager::playerShader);
	sNumbersObject[8].CreateModel("number8s", ShaderManager::playerShader);
	sNumbersObject[9].CreateModel("number9s", ShaderManager::playerShader);
	reverseObject.CreateModel("reverse", ShaderManager::playerShader);

	scoreObject.CreateModel("score", ShaderManager::playerShader);

	for (int i = 0; i < 2; i++)
	{
		reversEach[i].CreateConstBuff0();
		reversEach[i].CreateConstBuff1();
		timerEach[i].CreateConstBuff0();
		timerEach[i].CreateConstBuff1();
	}
	for (int i = 0; i < 7; i++)
	{
		scoreEach[i].CreateConstBuff0();
		scoreEach[i].CreateConstBuff1();
	}
	tipsSprite[0].CreateSprite(L"Resource/Img/tips/ex1.png", XMFLOAT3(0, 0, 0));
	tipsSprite[1].CreateSprite(L"Resource/Img/tips/ex2.png", XMFLOAT3(0, 0, 0));
	tipsSprite[2].CreateSprite(L"Resource/Img/tips/ex3.png", XMFLOAT3(0, 0, 0));
	tipsSprite[3].CreateSprite(L"Resource/Img/tips/ex4.png", XMFLOAT3(0, 0, 0));

	startModel.CreateModel("start", ShaderManager::playerShader);
	startModel.each.scale = { 0.3,0.3,0.3 };
	startModel.Update();
	AButton.CreateModel("A", ShaderManager::playerShader);
	pushA.CreateModel("pushA", ShaderManager::playerShader);
	tutorialObject.CreateModel("tutorial", ShaderManager::playerShader);
	tutorialObject.each.scale = { 0.3,0.3,0.3 };
	tutorialObject.Update();
	pushSpace.CreateModel("push_space", ShaderManager::playerShader);
	titleObject.CreateModel("title", ShaderManager::playerShader);
	backGround.CreateModel("background", ShaderManager::half);
	kakko[0].CreateModel("arrow_0", ShaderManager::playerShader);
	kakko[1].CreateModel("arrow_1", ShaderManager::playerShader);
	for (int i = 0; i < 2; i++)
	{
		backGroundEach[i].CreateConstBuff0();
		backGroundEach[i].CreateConstBuff1();
	}

	othelloManager.Init(num, sNumbersObject);
	tipsNumAndTexCount.push_back({ 0, 4 });

	//タイトル演出用
	titleLogo[0].CreateSprite(L"Resource/Img/title_logo/logo_leftUp.png", XMFLOAT3(-640, 0, 0));
	titleLogo[1].CreateSprite(L"Resource/Img/title_logo/logo_rightUp.png", XMFLOAT3(640, -360, 0));
	titleLogo[3].CreateSprite(L"Resource/Img/title_logo/logo_leftDown.png", XMFLOAT3(0, 720, 0));
	titleLogo[2].CreateSprite(L"Resource/Img/title_logo/logo_rightDown.png", XMFLOAT3(1280, 360, 0));
	logoSize = { 640,360,0 };
	logoNum = 0;
	easeTimer = 0;
	isDrawLogo = false;

	//tips用
	pushButton_black.CreateModel("push_button_black", ShaderManager::playerShader);
	pushButton_green.CreateModel("push_button_green", ShaderManager::playerShader);

	pushButton[0].CreateConstBuff0();
	pushButton[0].CreateConstBuff1();
	pushButton[1].CreateConstBuff0();
	pushButton[1].CreateConstBuff1();
	pushButton[2].CreateConstBuff0();
	pushButton[2].CreateConstBuff1();

	tips[0].CreateSprite(L"Resource/Img/tips/tips_0.png", XMFLOAT3(0, 0, 0));
	tips[1].CreateSprite(L"Resource/Img/tips/tips_2.png", XMFLOAT3(0, 0, 0));
	tips[2].CreateSprite(L"Resource/Img/tips/tips_3.png", XMFLOAT3(0, 0, 0));
	tips[3].CreateSprite(L"Resource/Img/tips/tips_4.png", XMFLOAT3(0, 0, 0));
	tips[4].CreateSprite(L"Resource/Img/tips/tips_5.png", XMFLOAT3(0, 0, 0));
	tips[5].CreateSprite(L"Resource/Img/tips/tips_6.png", XMFLOAT3(0, 0, 0));
	tips[6].CreateSprite(L"Resource/Img/tips/tips_7.png", XMFLOAT3(0, 0, 0));
	tips[7].CreateSprite(L"Resource/Img/tips/tips_8.png", XMFLOAT3(0, 0, 0));

	tips_frame.CreateSprite(L"Resource/Img/tips/tips_frame.png", XMFLOAT3(0, 0, 0));

	/*----------新tips用----------*/
	tips_name[0].CreateSprite(L"Resource/Img/tips/othello_move.png", XMFLOAT3(NAME_START_X, NAME_START_Y, 0));
	tips_name[1].CreateSprite(L"Resource/Img/tips/othello_move.png", XMFLOAT3(NAME_START_X, NAME_START_Y, 0));
	tips_name[2].CreateSprite(L"Resource/Img/tips/othello_move.png", XMFLOAT3(NAME_START_X, NAME_START_Y, 0));
	tips_name[3].CreateSprite(L"Resource/Img/tips/othello_move.png", XMFLOAT3(NAME_START_X, NAME_START_Y, 0));
	tips_name[4].CreateSprite(L"Resource/Img/tips/othello_move.png", XMFLOAT3(NAME_START_X, NAME_START_Y, 0));
	tips_name[5].CreateSprite(L"Resource/Img/tips/othello_move.png", XMFLOAT3(NAME_START_X, NAME_START_Y, 0));
	tips_name[6].CreateSprite(L"Resource/Img/tips/othello_move.png", XMFLOAT3(NAME_START_X, NAME_START_Y, 0));
	tips_name[7].CreateSprite(L"Resource/Img/tips/othello_move.png", XMFLOAT3(NAME_START_X, NAME_START_Y, 0));

	tips_system[0].CreateSprite(L"Resource/Img/tips/system.png", XMFLOAT3(SYSTEM_START_X, SYSTEM_START_Y, 0));
	tips_system[1].CreateSprite(L"Resource/Img/tips/system.png", XMFLOAT3(SYSTEM_START_X, SYSTEM_START_Y, 0));

	tips_bar.CreateSprite(L"Resource/Img/tips/tips_bar.png", XMFLOAT3(BAR_START_X, BAR_START_Y, 0));

	tips_text[0].CreateSprite(L"Resource/Img/tips/othello_move_text.png", XMFLOAT3(TEXT_START_X, TEXT_START_Y, 0));
	tips_text[1].CreateSprite(L"Resource/Img/tips/othello_move_text.png", XMFLOAT3(TEXT_START_X, TEXT_START_Y, 0));
	tips_text[2].CreateSprite(L"Resource/Img/tips/othello_move_text.png", XMFLOAT3(TEXT_START_X, TEXT_START_Y, 0));
	tips_text[3].CreateSprite(L"Resource/Img/tips/othello_move_text.png", XMFLOAT3(TEXT_START_X, TEXT_START_Y, 0));
	tips_text[4].CreateSprite(L"Resource/Img/tips/othello_move_text.png", XMFLOAT3(TEXT_START_X, TEXT_START_Y, 0));
	tips_text[5].CreateSprite(L"Resource/Img/tips/othello_move_text.png", XMFLOAT3(TEXT_START_X, TEXT_START_Y, 0));
	tips_text[6].CreateSprite(L"Resource/Img/tips/othello_move_text.png", XMFLOAT3(TEXT_START_X, TEXT_START_Y, 0));
	tips_text[7].CreateSprite(L"Resource/Img/tips/othello_move_text.png", XMFLOAT3(TEXT_START_X, TEXT_START_Y, 0));
	isInit = false;
	newTips_easeTimer = 0;
	/*----------新tips用----------*/


	tips_cont.CreateSprite(L"Resource/Img/tips/tips_1.png", XMFLOAT3(0, 0, 0));

	tips_ss[0].CreateSprite(L"Resource/Img/tips/tips_ss1.png", XMFLOAT3(START_X, START_Y, 0));
	tips_ss[1].CreateSprite(L"Resource/Img/tips/tips_ss2.png", XMFLOAT3(START_X - SIZE_X, START_Y, 0));
	tips_ss[2].CreateSprite(L"Resource/Img/tips/tips_ss3.png", XMFLOAT3(START_X - SIZE_X, START_Y, 0));
	tips_ss[3].CreateSprite(L"Resource/Img/tips/tips_ss4.png", XMFLOAT3(START_X - SIZE_X, START_Y, 0));

	tipsCounts = 0;
	changeTimerFrame = 0;
	tipsDrawNum = 0;
	tips_easeTimer = 0;
	green_scale = 0.09f;
	isTipsDrawTrigger = false;
	moveTips = false;
}

void GameScene::TitleUpdate()
{
	//SoundPlayLoop(BGMSound);
	if (!isDrawLogo)
	{
		if (logoNum == 0)		//左上
		{
			XMFLOAT3 titleLogoPosGoal = { 0,0,0 };
			XMFLOAT3 titleLogoPos = ConvertXMVECTORtoXMFLOAT3(titleLogo[logoNum].position);
			if (easeTimer < 1.0f) { easeTimer += EASE_TIMER_COUNT; }
			if (easeTimer > 1.0f) { easeTimer = 1.0f; }
			titleLogoPos = ShlomonMath::EaseInQuad(titleLogoPos, titleLogoPosGoal, easeTimer);
			titleLogo[logoNum].position = ConvertXMFLOAT3toXMVECTOR(titleLogoPos);
		}
		else if (logoNum == 1)	//右上
		{
			XMFLOAT3 titleLogoPosGoal = { 640,0,0 };
			XMFLOAT3 titleLogoPos = ConvertXMVECTORtoXMFLOAT3(titleLogo[logoNum].position);
			if (easeTimer < 1.0f) { easeTimer += EASE_TIMER_COUNT; }
			if (easeTimer > 1.0f) { easeTimer = 1.0f; }
			titleLogoPos = ShlomonMath::EaseInQuad(titleLogoPos, titleLogoPosGoal, easeTimer);
			titleLogo[logoNum].position = ConvertXMFLOAT3toXMVECTOR(titleLogoPos);
		}
		else if (logoNum == 2)	//右下
		{
			XMFLOAT3 titleLogoPosGoal = { 640,360,0 };
			XMFLOAT3 titleLogoPos = ConvertXMVECTORtoXMFLOAT3(titleLogo[logoNum].position);
			if (easeTimer < 1.0f) { easeTimer += EASE_TIMER_COUNT; }
			if (easeTimer > 1.0f) { easeTimer = 1.0f; }
			titleLogoPos = ShlomonMath::EaseInQuad(titleLogoPos, titleLogoPosGoal, easeTimer);
			titleLogo[logoNum].position = ConvertXMFLOAT3toXMVECTOR(titleLogoPos);
		}
		else if (logoNum == 3)	//左下
		{
			XMFLOAT3 titleLogoPosGoal = { 0,360,0 };
			XMFLOAT3 titleLogoPos = ConvertXMVECTORtoXMFLOAT3(titleLogo[logoNum].position);
			if (easeTimer < 1.0f) { easeTimer += EASE_TIMER_COUNT; }
			if (easeTimer > 1.0f) { easeTimer = 1.0f; }
			titleLogoPos = ShlomonMath::EaseInQuad(titleLogoPos, titleLogoPosGoal, easeTimer);
			titleLogo[logoNum].position = ConvertXMFLOAT3toXMVECTOR(titleLogoPos);
		}
		else
		{
			easeTimer += EASE_TIMER_COUNT;
			if (easeTimer > 1.0f) { easeTimer = 1.0f; }
			isDrawLogo = true;
		}

		if (easeTimer == 1.0f) { easeTimer = 0; logoNum++; }
	}

#pragma region 俺以外の処理
	if (isDrawLogo && !isPouseToTiTle)
	{
		if (easeTimer < 1.0f)
		{
			XMFLOAT3 titleLogoSizeGoal = { 0,0,0 };
			easeTimer += EASE_TIMER_COUNT / 5.0f;
			if (easeTimer > 1.0f) { easeTimer = 1.0f; }
			logoSize = ShlomonMath::EaseInQuad(logoSize, titleLogoSizeGoal, easeTimer);
			titleLogo[0].ChangeSizeOther(logoSize.x, logoSize.y);
			titleLogo[1].ChangeSizeOther(logoSize.x, logoSize.y);
			titleLogo[2].ChangeSizeOther(logoSize.x, logoSize.y);
			titleLogo[3].ChangeSizeOther(logoSize.x, logoSize.y);

			titleLogo[0].position = ConvertXMFLOAT3toXMVECTOR(XMFLOAT3(320, 180, 0));
			titleLogo[1].position = ConvertXMFLOAT3toXMVECTOR(XMFLOAT3(960, 180, 0));
			titleLogo[2].position = ConvertXMFLOAT3toXMVECTOR(XMFLOAT3(960, 540, 0));
			titleLogo[3].position = ConvertXMFLOAT3toXMVECTOR(XMFLOAT3(320, 540, 0));
		}
		//オセロのパーティクルを出していく
		static int particleTime = 0;
		//シーンチェンジ開始前
		if (!isSceneChange)
		{
			//パーティクルをだす
			particleTime++;
			if (particleTime % 5 == 4)
			{
				ObjectParticles::othello.Init(XMFLOAT3(0, 0, -15), 1, ParticleType::TITLE);
				particleTime = 0;
			}
			if (!select)
			{
				//チュートリアルかどうかを選択する
				if (Input::KeyTrigger(DIK_A) || directInput->IsButtonPush(directInput->LeftButton))
				{
					SoundStopWave(selectSound);
					SoundPlayOnce(selectSound);
					if (titleSelectNum == 0)
					{
						titleSelectNum = 1;
					}
				}
				if (Input::KeyTrigger(DIK_D) || directInput->IsButtonPush(directInput->RightButton))
				{
					SoundStopWave(selectSound);
					SoundPlayOnce(selectSound);
					if (titleSelectNum == 1)
					{
						titleSelectNum = 0;
					}
				}
				//チュートリアルに飛ぶ
				if ((Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01)) && !isPouse && !isSceneChange && !isPouseToTiTle)
				{
					if (titleSelectNum == 1)
					{
						SoundStopWave(enterSound);
						SoundPlayOnce(enterSound);

						ToTutorial();

						isTutorial = true;
						isTipsDraw = false;
						othelloManager.AllDeletePanel();
						othelloManager.EraseChanceObject();
						titleSelectNum = 0;
						//othelloManager.whySandwichSpawn();
						gameTime = 60;
					}
					//ここでモード選択に飛ぶ
					else
					{
						gameTime = 1;
						isTipsDraw = false;
						ToModeSelect();
					}
				}
			}
			else if (select)
			{
				//スコアアタックかどうかの選択を行う
				if (Input::KeyTrigger(DIK_A) || directInput->IsButtonPush(directInput->LeftButton))
				{
					SoundStopWave(selectSound);
					SoundPlayOnce(selectSound);
					if (!selectStage)
					{
						selectMode = true;
					}
					else if (selectStage && 1 < selectStageNum && !selectEase)
					{
						selectEaseDirection = true;
						selectEase = true;
						selectStageNum--;
					}
				}
				if (Input::KeyTrigger(DIK_D) || directInput->IsButtonPush(directInput->RightButton))
				{
					SoundStopWave(selectSound);
					SoundPlayOnce(selectSound);
					if (!selectStage)
					{
						selectMode = false;
					}
					else if (selectStageNum < othelloManager.GetNormaStagesCount() && selectStage && !selectEase)
					{
						selectEaseDirection = false;
						selectEase = true;
						selectStageNum++;
					}
				}
				if ((Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01)) && !isPouse && !isPouseToTiTle)
				{

					SoundStopWave(enterSound);
					SoundPlayOnce(enterSound);
					othelloManager.AllDeletePanel();
					othelloManager.EraseChanceObject();
					//スコアアタック
					if (!selectMode)
					{
						gameTime = gameMaxTime;
						isTutorial = false;
						ToGame4();
						titleSelectNum = 0;
					}
					//ノルマモード
					else
					{
						if (selectStage)
						{
							gameTime = gameMaxTime;
							isTutorial = false;
							ToGame4(true);
							titleSelectNum = 0;
						}
						else
						{
							selectStage = true;
						}
					}
				}
			}
		}
		if (isBackGroundOthello)
		{
			for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
			{
				ObjectParticles::othello2.object.Update(&(*opOthelloItr));
				Draw3DObject(ObjectParticles::othello2.object);
			}
		}
		//ポーズ画面に移行する
		if (!isPouse && (Input::KeyTrigger(DIK_ESCAPE) || directInput->IsButtonPush(directInput->ButtonPouse)))
		{
			isPouse = true;
			selectPouse = 0;
		}
		//ポーズ中の処理
		else if (isPouse)
		{
			if (Input::KeyTrigger(DIK_W) || directInput->IsButtonPush(directInput->UpButton))
			{
				SoundStopWave(selectSound);
				SoundPlayOnce(selectSound);
				if (selectPouse == 0)
				{
					selectPouse = selectMaxPouse;
				}
				else
				{
					selectPouse--;
				}
			}
			else if (Input::KeyTrigger(DIK_S) || directInput->IsButtonPush(directInput->DownButton))
			{
				SoundStopWave(selectSound);
				SoundPlayOnce(selectSound);
				if (selectPouse == selectMaxPouse)
				{
					selectPouse = 0;
				}
				else
				{
					selectPouse++;
				}
			}
			if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
			{
				//リザルトに戻る
				if (selectPouse == 0)
				{
					isPouse = false;
				}
				else if (selectPouse == 1)
				{
					gameTime = 0;
					isPouse = false;
					select = false;
					checkObject.Init();
				}
				else if (selectPouse == 2)
				{
					isGameEnd = true;
					checkObject.Init();
				}
			}
			if (Input::KeyTrigger(DIK_ESCAPE) || directInput->IsButtonPush(directInput->ButtonPouse))
			{
				isPouse = false;
			}
		}
		ObjectParticles::Update(othelloManager.GetPressPanellPos(), checkObject.GetCombo());
		light->SetLightDir(XMFLOAT3(Camera::GetTargetDirection()));
		LightUpdate();
		sky.each.rotation.y += 1.0f;
		sky.Update();
	}
	if (isPouseToTiTle)
	{
		PouseToTitleUpdate();
	}
#pragma endregion
}

void GameScene::SelectUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE))
	{
		SceneNum = GAME;
	}
}

void GameScene::GameUpdate()
{
	light->SetLightDir(XMFLOAT3(Camera::GetTargetDirection()));
	LightUpdate();
	//カメラ変化が行われていない時にゲームを開始する
	if (!isSceneChange && !isResultSceneChange && !isPouse)
	{
		if (isModeSelect)
		{
			OthlloPlayer::Update();
			othelloManager.Controll();
			//セレクト画面のアップデート
			othelloManager.ModeSelectUpdate();
			//使用例
#pragma region example
			//スコアアタックを選択した
			if (othelloManager.InMode() && othelloManager.GetEnterModeType() == GameMode::ScoreAttack)
			{
				isTutorial = false;
				ToGame4();
				othelloManager.AllDeletePanel();
			}
			//ノルマモードを選択した
			if (othelloManager.InMode() && othelloManager.GetEnterModeType() == GameMode::NormaMode)
			{//内部でステージ数を検索(中身は適当です)
				othelloManager.AllDeletePanel();
				gameTime = gameMaxTime;
				isTutorial = false;
				selectMode = true;
				ToGame4(true);
				//othelloManager.StartNormaMode(othelloManager.GetEnterNormaStage());
				titleSelectNum = 0;
				selectStageNum = othelloManager.GetEnterNormaStage();
			}
#pragma endregion
		}
		else if (countDown <= 0)
		{
			SoundPlayOnce(BGMSound);
			OthlloPlayer::Update();

			othelloManager.Controll();
			if (isTutorial)
			{
				othelloManager.TutorialUpdate(checkObject.GetCombo());
			}
			else
			{
				if (!selectMode)
				{
					othelloManager.Update(checkObject.GetCombo());
				}
				else if (selectMode)
				{
					othelloManager.SetScore(checkObject.GetScore());
					othelloManager.NormaUpdate(checkObject.GetCombo());
				}
				if (othelloManager.GetIsNormaFailed())
				{
					isSceneChange = true;
				}
				else if (othelloManager.GetIsNormaClear())
				{
					static int checkTime = 0;
					checkTime++;
					if (checkTime > 60)
					{
						checkTime = 0;
						PouseToTitle();
					}
					//PouseToTitle();
				}
			}
			checkObject.Update(othelloManager.Send(), othelloManager.GetIsSendDataUpdate());
			othelloManager.Receive(checkObject.GetOthelloDatas(), checkObject.GetCompletePos());
			othelloManager.SpawnChances(checkObject.GetReachDatas());

			if (othelloManager.GetIsSendDataUpdate())
			{

				//vector<pair<int, int>> testPos;
				//for (int i = 0; i < 8; i++)
				//{
				//	pair<int, int> tmp(i, i);
				//	testPos.push_back(tmp);
				//}
				//othelloManager.SpawnChances(testPos);
			}
			if (!isTutorial && !selectMode)
			{
				gameTime--;
			}
			else if (othelloManager.IsTutorialEnd() && isTutorial)
			{
				//gameTime--;
				PouseToTitle();
			}
			countDown--;
		}
		else
		{
			SoundStopWave(BGMSound);
			countDown--;
		}
	}
	if (countDown == 45)
	{
		SoundStopWave(countdDownSound);
		SoundStopWave(startSound);
		SoundPlayOnce(startSound);
	}
	ObjectParticles::Update(othelloManager.GetPressPanellPos(), checkObject.GetCombo());
	ObjectParticles::Update(othelloManager.GetPressPanellPos(), checkObject.GetNameAndCombos());
	//タイトルからgameシーンへ
	if (isSceneChange)
	{
		if (sceneChageType == 1)
		{
			ToGame1Update();
		}
		else if (sceneChageType == 2)
		{
			ToGame2Update();
		}
		else if (sceneChageType == 3)
		{
			ToGame3Update();
		}
		else if (sceneChageType == 4)
		{
			ToGame4Update();
		}
		else if (sceneChageType == 5)
		{
			ToModeSelectUpdate();
		}
		else if (sceneChageType == 6)
		{
			ToTutorialUpdate();
		}
	}
	//ゲームシーンからリザルトへのトリガー
	if (gameTime <= 0 && !isResultSceneChange)
	{
		ToResult();
	}
	//カメラの動き
	if (isResultSceneChange)
	{
		isPouse = false;
		if (resultForTime >= 60)
		{
			Camera::target.v = ShlomonMath::EaseInQuad(eyeStart, eyeEnd, eyeEaseTime);
			eyeEaseTime += 0.02f;
			if (eyeEaseTime > 1.0f)
			{
				Camera::target.v = ShlomonMath::EaseInQuad(eyeStart, eyeEnd, 1.0f);
				resultForTime = 0;
				isResultSceneChange = false;
				/*if (isPouseToTitle == true)
				{
					SceneNum = TITLE;
					SoundPlayLoop(BGMSound);
					isPouseToTitle = false;
				}*/
				/*if (isTutorial == true)
				{
					SceneNum = TITLE;
					SoundPlayLoop(BGMSound);
					isTutorial = false;
				}*/
				if (!isPouseToTitle && !isTutorial)
					SceneNum = RESULT;
				resultForTime = 0;
				isResultSceneChange = false;
				displayScore = checkObject.GetScore();

			}
			Camera::Update();
		}
		else
		{
			resultForTime++;
		}
	}
	//if (!isPouse && (Input::KeyTrigger(DIK_ESCAPE) || directInput->IsButtonPush(directInput->ButtonPouse)) && )
	if (!isPouse && (Input::KeyTrigger(DIK_ESCAPE) || directInput->IsButtonPush(directInput->ButtonPouse)) && (countDown <= 0 || isModeSelect))
	{
		isPouse = true;
		selectPouse = 0;
	}
	//ポーズ中の処理
	else if (isPouse)
	{
		if (Input::KeyTrigger(DIK_W) || directInput->IsButtonPush(directInput->UpButton))
		{
			SoundStopWave(selectSound);
			SoundPlayOnce(selectSound);
			if (selectPouse == 0)
			{
				selectPouse = selectMaxPouse;
			}
			else
			{
				selectPouse--;
			}
		}
		else if (Input::KeyTrigger(DIK_S) || directInput->IsButtonPush(directInput->DownButton))
		{
			SoundStopWave(selectSound);
			SoundPlayOnce(selectSound);
			if (selectPouse == selectMaxPouse)
			{
				selectPouse = 0;
			}
			else
			{
				selectPouse++;
			}
		}
		if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
		{
			SoundStopWave(enterSound);
			SoundPlayOnce(enterSound);
			//
			if (selectPouse == 0)
			{
				isPouse = false;
			}
			else if (selectPouse == 1)
			{
				PouseToTitle();
				isCameraModed = false;
			}
			else if (selectPouse == 2)
			{
				isGameEnd = true;
				select = false;
			}
		}
		if (Input::KeyTrigger(DIK_ESCAPE) || directInput->IsButtonPush(directInput->ButtonPouse))
		{
			isPouse = false;
		}
	}
	if (checkObject.GetCombo() >= 3)
	{
		tornadoTime++;
		if (tornadoTime > 40)
		{
			ObjectParticles::triangle.Init(XMFLOAT3(0, 0, 0), checkObject.GetCombo() - 2, ParticleType::Tornado);
			tornadoTime = 0;
		}
	}
	else
	{
		tornadoTime = 0;
	}
	/*if (!isSceneChange && !isResultSceneChange && !isPouse)
	{
		if (Input::KeyTrigger(DIK_U) || directInput->IsButtonPush(directInput->Button03) && !isTutorial)
		{
			ReStart();
		}
	}*/

	sky.Update();
	othelloStage.Update();
	Lights::Add(checkObject);
	Lights::Update();
	ParticleControl::Update();
	if (isPouseToTiTle)
	{
		PouseToTitleUpdate();
	}

	//tips用
	if (isTipsDraw)
	{
		//ButtonUpdate
		//pushButton_black.each.position = { 5.0f, -4.0f, -5.0f, 1.0 };
		//pushButton_black.each.scale = { 0.1f, 0.1f, 0.1f };

		const float black_scale = 0.085f;
		if (green_scale < 0.075f) { green_scale = 0.09f; }
		green_scale -= 0.0005f;

		pushButton[0].position = { 8.2f, -3.33f, -5.0f, 1.0 };
		pushButton[0].scale = { black_scale, black_scale, black_scale };

		pushButton[1].position = { 7.7f, -3.83f, -5.0f, 1.0 };
		pushButton[1].scale = { black_scale, black_scale, black_scale };

		pushButton[2].position = { 8.7f, -3.83f, -5.0f, 1.0 };
		pushButton[2].scale = { black_scale, black_scale, black_scale };

		pushButton_green.each.position = { 8.2f, -4.33f, -5.0f, 1.0 };
		pushButton_green.each.scale = { green_scale, green_scale, green_scale };

		//pushButton_black.Update();
		//pushButton_green.Update();

		//外の情報の移動
		isInit = true;
		XMFLOAT3 name = ConvertXMVECTORtoXMFLOAT3(tips_name[tipsCounts].position);
		XMFLOAT3 system = { 0,0,0 };
		if (tipsCounts < 5) { system = ConvertXMVECTORtoXMFLOAT3(tips_system[0].position); }
		else { system = ConvertXMVECTORtoXMFLOAT3(tips_system[1].position); }
		XMFLOAT3 bar = ConvertXMVECTORtoXMFLOAT3(tips_bar.position);
		XMFLOAT3 text = ConvertXMVECTORtoXMFLOAT3(tips_text[tipsCounts].position);

		XMFLOAT3 name_Goal = { NAME_X,NAME_Y,0 };
		XMFLOAT3 system_Goal = { SYSTEM_X,SYSTEM_Y,0 };
		XMFLOAT3 bar_Goal = { BAR_X,BAR_Y,0 };
		XMFLOAT3 text_Goal = { TEXT_X,TEXT_Y,0 };

		if (newTips_easeTimer < 1.0f) { newTips_easeTimer += 0.01; }
		if (newTips_easeTimer > 1.0f) { newTips_easeTimer = 1.0f; }

		float tips_Timers[3];
		tips_Timers[0] = newTips_easeTimer - EASE_DELAY;
		tips_Timers[1] = newTips_easeTimer - EASE_DELAY * 2;
		tips_Timers[2] = newTips_easeTimer - EASE_DELAY * 3;

		name = ShlomonMath::EaseInQuad(name, name_Goal, newTips_easeTimer);
		if (tips_Timers[0] >= 0.0f) { system = ShlomonMath::EaseInQuad(system, system_Goal, tips_Timers[0]); }
		if (tips_Timers[1] >= 0.0f) { bar = ShlomonMath::EaseInQuad(bar, bar_Goal, tips_Timers[1]); }
		if (tips_Timers[2] >= 0.0f) { text = ShlomonMath::EaseInQuad(text, text_Goal, tips_Timers[2]); }

		tips_name[tipsCounts].position = ConvertXMFLOAT3toXMVECTOR(name);
		if (tipsCounts < 5) { tips_system[0].position = ConvertXMFLOAT3toXMVECTOR(system); }
		else { tips_system[1].position = ConvertXMFLOAT3toXMVECTOR(system); }
		tips_bar.position = ConvertXMFLOAT3toXMVECTOR(bar);
		tips_text[tipsCounts].position = ConvertXMFLOAT3toXMVECTOR(text);

		//中身の移動
		if (!moveTips)
		{
			if (changeTimerFrame < CHANGE_TIMER_FRAME) { changeTimerFrame++; }
			else { changeTimerFrame = 0; moveTips = true; }
		}
		else
		{
			//現在描画されてるtips
			XMFLOAT3 ssPos0 = ConvertXMVECTORtoXMFLOAT3(tips_ss[tipsDrawNum].position);
			//これから描画されるtips
			XMFLOAT3 ssPos1 = { 0,0,0 };
			if (tipsDrawNum >= USE_TEX_0 - 1) { ssPos1 = ConvertXMVECTORtoXMFLOAT3(tips_ss[0].position); }
			else { ssPos1 = ConvertXMVECTORtoXMFLOAT3(tips_ss[tipsDrawNum + 1].position); }

			XMFLOAT3 ssPos0_Goal = { START_X + SIZE_X,START_Y,0 };
			XMFLOAT3 ssPos1_Goal = { START_X,START_Y,0 };

			if (tips_easeTimer < 1.0f) { tips_easeTimer += 0.01; }
			if (tips_easeTimer > 1.0f) { tips_easeTimer = 1.0f; }

			ssPos0 = ShlomonMath::EaseInQuad(ssPos0, ssPos0_Goal, tips_easeTimer);
			ssPos1 = ShlomonMath::EaseInQuad(ssPos1, ssPos1_Goal, tips_easeTimer);

			tips_ss[tipsDrawNum].position = ConvertXMFLOAT3toXMVECTOR(ssPos0);
			if (tipsDrawNum >= USE_TEX_0 - 1) { tips_ss[0].position = ConvertXMFLOAT3toXMVECTOR(ssPos1); }
			else { tips_ss[tipsDrawNum + 1].position = ConvertXMFLOAT3toXMVECTOR(ssPos1); }

			if (tips_easeTimer == 1.0f)
			{
				tips_ss[tipsDrawNum].position = ConvertXMFLOAT3toXMVECTOR(
					XMFLOAT3(START_X - SIZE_X, START_Y, 0)
				);

				tipsDrawNum++;
				if (tipsDrawNum > USE_TEX_0 - 1) { tipsDrawNum = 0; }
				tips_easeTimer = 0;
				moveTips = false;
			}
		}
	}

	else if (isCameraModed)
	{
		const float black_scale = 0.085f;
		if (green_scale < 0.075f) { green_scale = 0.09f; }
		green_scale -= 0.0005f;

		//pushButton_green.each.position = { 8.2f, -4.33f, -5.0f, 1.0 };
		pushButton_green.each.scale = { green_scale, green_scale, green_scale };
	}

	if (!isTipsDraw)
	{
		if (isInit)
		{
			newTips_easeTimer = 0.0f;
			tips_name[tipsCounts].position = { NAME_START_X,NAME_START_Y,0,0 };
			if (tipsCounts < 5) { tips_system[0].position = { SYSTEM_START_X,SYSTEM_START_Y,0,0 }; }
			else { tips_system[1].position = { SYSTEM_START_X,SYSTEM_START_Y,0,0 }; }
			tips_bar.position = { BAR_START_X,BAR_START_Y,0,0 };
			tips_text[tipsCounts].position = { TEXT_START_X,TEXT_START_Y,0,0 };

			isInit = false;
		}
	}

	pushButton_green.Update();
}

void GameScene::ResultUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
	{
		SoundStopWave(enterSound);
		SoundPlayOnce(enterSound);
		OthlloPlayer::SetPosition(XMFLOAT3(0, 0, -2));
		SoundPlayLoop(BGMSound);
		//SceneNum = TITLE;
		//isSceneChange = false;
		ToTitle();
	}
	if (isResultStart)
	{
		ToTitleUpdate();
	}
}

void GameScene::EndUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(DirectInput::ButtonKind::Button01))
	{
		SceneNum = TITLE;
		Camera::Init();
	}
}

void GameScene::TitleDraw()
{
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();

	titleLogo[0].SpriteDraw();
	titleLogo[1].SpriteDraw();
	titleLogo[2].SpriteDraw();
	titleLogo[3].SpriteDraw();

	/*-----この中で処理書いてください-----*/
	if (isDrawLogo)
	{
		Draw3DObject(sky);
		//Draw3DObject(othelloStage);
		if (isPouseToTiTle)
		{
			for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
			{
				ObjectParticles::othello2.object.Update(&(*opOthelloItr));
				Draw3DObject(ObjectParticles::othello2.object);
			}
		}
		ParticleControl::Draw();
		ObjectParticles::Draw();
		Lights::Draw();
		//スプライトの描画-------------------------
		if (isSceneChange == false)
		{
			XMFLOAT3 scale = { 0.4f, 0.4f, 0.4f };
			titleObject.each.position = { -1, 3, 0, 1.0f };
			titleObject.each.scale = scale;
			titleObject.each.rotation = { 0, 0, 0 };
			titleObject.Update();
			Draw3DObject(titleObject);
			backGroundEach[0].position = { -1, 4.5f, 4.0f, 1.0f };
			backGroundEach[0].scale = { 20.0f, 4.0f, 1.0f };
			backGroundEach[0].rotation = { 0, 0, 0 };
			backGround.Update(&backGroundEach[0]);
			Draw3DObject(backGround);
			backGroundEach[1].position = { -1, -6.5f, 4.0f, 1.0f };
			backGroundEach[1].scale = { 23.0f, 1.5f, 1.0f };
			backGroundEach[1].rotation = { 0, 0, 0 };
			backGround.Update(&backGroundEach[1]);
			Draw3DObject(backGround);
			XMFLOAT3 scale2 = { 0.3f, 0.3f, 0.3f };
			startModel.each.position = { -1.0f + 6.0f, -2.0f, 0, 1.0f };
			if (titleSelectNum == 0)
			{
				XMFLOAT3 scale = startModel.each.scale;
				if (scale.x > 0.3f) { scale = { 0.3f, 0.3f, 0.3f }; }
				scale.x -= 0.001f;
				scale.y -= 0.001f;
				scale.z -= 0.001f;
				if (scale.x < 0.276f) { scale = { 0.3f, 0.3f, 0.3f }; }
				startModel.each.scale = scale;
			}
			else { startModel.each.scale = scale2; }
			startModel.each.rotation = { 0, 0, 0 };
			startModel.Update();
			Draw3DObject(startModel);
			XMFLOAT3 scale3 = { 0.3f, 0.3f, 0.3f };
			tutorialObject.each.position = { -1 - 6.0f, -2.0f, 0, 1.0f };
			if (titleSelectNum == 1)
			{
				XMFLOAT3 scale = tutorialObject.each.scale;
				if (scale.x > 0.3f) { scale = { 0.3f, 0.3f, 0.3f }; }
				scale.x -= 0.001f;
				scale.y -= 0.001f;
				scale.z -= 0.001f;
				if (scale.x < 0.276f) { scale = { 0.3f, 0.3f, 0.3f }; }
				tutorialObject.each.scale = scale;
			}
			else { tutorialObject.each.scale = scale3; }
			tutorialObject.each.rotation = { 0, 0, 0 };
			tutorialObject.Update();
			Draw3DObject(tutorialObject);
			XMFLOAT3 scale4 = { 0.3f, 0.3f, 0.3f };
			pushSpace.each.position = { -1 + 6.0f, -5.0f, 0, 1.0f };
			pushSpace.each.scale = scale4;
			pushSpace.each.rotation = { 0, 0, 0 };
			pushSpace.Update();
			pushA.each.position = { -1 + 6.0f, -5.0f, 0, 1.0f };
			pushA.each.scale = scale4;
			pushA.each.rotation = { 0, 0, 0 };
			pushA.Update();
			Draw3DObject(pushA);
			//Draw3DObject(pushSpace);
		}
		if (!isPouseToTiTle)
		{
			if (titleSelectNum == 0)
			{
				kakko[1].each.position = { -1 + 10.5f, -2.0f, 0, 1.0f };
				kakko[1].each.rotation.x += 1.0f;
				kakko[1].Update();
				Draw3DObject(kakko[1]);
				kakko[0].each.position = { -1 + 1.5f, -2.0f, 0, 1.0f };
				kakko[0].each.rotation.x += 1.0f;
				kakko[0].Update();
				Draw3DObject(kakko[0]);
			}
			else
			{
				kakko[1].each.position = { -1 - 0.5f, -2.0f, 0, 1.0f };
				kakko[1].each.rotation.x += 1.0f;
				kakko[1].Update();
				Draw3DObject(kakko[1]);
				kakko[0].each.position = { -1 - 11.5f, -2.0f, 0, 1.0f };
				kakko[0].each.rotation.x += 1.0f;
				kakko[0].Update();
				Draw3DObject(kakko[0]);
			}
		}
	}
	if (isPouse)
	{
		pouseBack.ChangeSize(1280, 720);
		pouseBack.position.m128_f32[0] = 0;
		pouseBack.position.m128_f32[1] = 0;
		pouseBack.SpriteDraw();
		backSprite.position.m128_f32[0] = window_width / 2 - 100;
		backSprite.position.m128_f32[1] = window_height / 2;
		backSprite.SpriteDraw();
		titleSprite.position.m128_f32[0] = window_width / 2 - 100;
		titleSprite.position.m128_f32[1] = window_height / 2 + 80;
		titleSprite.SpriteDraw();
		endSprite.position.m128_f32[0] = window_width / 2 - 100;
		endSprite.position.m128_f32[1] = window_height / 2 + 170;
		endSprite.SpriteDraw();
		if (selectPouse == 0)
		{
			kagikakkoStartSprite.ChangeSize(30, 70);
			kagikakkoStartSprite.position.m128_f32[0] = window_width / 2 - 140;
			kagikakkoStartSprite.position.m128_f32[1] = window_height / 2 + 5;
			kagikakkoStartSprite.SpriteDraw();
			kagikakkoEndSprite.ChangeSize(30, 70);
			kagikakkoEndSprite.position.m128_f32[0] = window_width / 2 + 120;
			kagikakkoEndSprite.position.m128_f32[1] = window_height / 2 + 5;
			kagikakkoEndSprite.SpriteDraw();
		}
		else if (selectPouse == 1)
		{
			kagikakkoStartSprite.ChangeSize(30, 70);
			kagikakkoStartSprite.position.m128_f32[0] = window_width / 2 - 140;
			kagikakkoStartSprite.position.m128_f32[1] = window_height / 2 + 85;
			kagikakkoStartSprite.SpriteDraw();
			kagikakkoEndSprite.ChangeSize(30, 70);
			kagikakkoEndSprite.position.m128_f32[0] = window_width / 2 + 120;
			kagikakkoEndSprite.position.m128_f32[1] = window_height / 2 + 85;
			kagikakkoEndSprite.SpriteDraw();
		}
		else if (selectPouse == 2)
		{
			kagikakkoStartSprite.ChangeSize(30, 70);
			kagikakkoStartSprite.position.m128_f32[0] = window_width / 2 - 140;
			kagikakkoStartSprite.position.m128_f32[1] = window_height / 2 + 165;
			kagikakkoStartSprite.SpriteDraw();
			kagikakkoEndSprite.ChangeSize(30, 70);
			kagikakkoEndSprite.position.m128_f32[0] = window_width / 2 + 70;
			kagikakkoEndSprite.position.m128_f32[1] = window_height / 2 + 165;
			kagikakkoEndSprite.SpriteDraw();
		}
	}
	//Imgui::DrawImGui();
	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::SelectDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw();
	Draw3DObject(sample);
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffectのDraw
	PostEffects::Draw();
	PostEffects::PostDraw();

	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::GameDraw()
{
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffectのDraw
	//PostEffects::Draw();

	//PostEffectのPostDraw
	//PostEffects::PostDraw();
	sky.each.rotation.y += 1.0f;
	Draw3DObject(sky);
	othelloStage.each.position.m128_f32[2] = 8.0f;
	if (isStageDisplay)Draw3DObject(othelloStage);
	if ((isSceneChange == true && sceneChageType >= 3) || isBackGroundOthello)
	{
		for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
		{
			ObjectParticles::othello2.object.Update(&(*opOthelloItr));
			Draw3DObject(ObjectParticles::othello2.object);
		}
	}
	if (isPouseToTiTle && !isBackGroundOthello)
	{
		for (auto opOthelloItr = pouseOthellos.begin(); opOthelloItr != pouseOthellos.end(); ++opOthelloItr)
		{
			ObjectParticles::othello2.object.Update(&(*opOthelloItr));
			Draw3DObject(ObjectParticles::othello2.object);
		}
	}
	float sizeX = size_x;
	float sizeY = size_y;
	if (gameTime > 0 && countDown <= 0)
	{
		if (gameTime / 60 <= CHANGE_TIMER_SECOND)
		{
			if (gameTime % 60 == 0)
			{
				isChanged = true;
			}
			if (isChanged)
			{
				if (timerCount < MAX_COUNT / 2)
				{
					size_x += ADD_SIZE;
					size_y += ADD_SIZE;
				}
				else if (timerCount < MAX_COUNT)
				{
					size_x -= ADD_SIZE;
					size_y -= ADD_SIZE;
				}

				if (timerCount > 10)
				{
					size_x = MAX_SIZE_X;
					size_y = MAX_SIZE_Y;
					timerCount = 0;
					isChanged = false;
				}
				timerCount++;
			}
		}
		if (!isResultSceneChange)
		{
			int zyunokurai = gameTime / 600 % 10;
			timerEach[0].position.m128_f32[0] = -2.0f;
			timerEach[0].position.m128_f32[1] = 18.0f;
			timerEach[0].position.m128_f32[2] = -1.0f;
			timerEach[0].scale = { size_x, size_x, size_x };
			timerEach[0].rotation.x = -30.0f;
			if (gameTime / 60 > CHANGE_COLOR_COUNT)
			{
				sNumbersObject[zyunokurai].Update(&timerEach[0]);
				Draw3DObject(sNumbersObject[zyunokurai]);
			}
			else
			{
				//numbersObject[zyunokurai].Update(&timerEach[0]);
				timerNumber[zyunokurai].Update(&timerEach[0]);
				//Draw3DObject(numbersObject[zyunokurai]);
				Draw3DObject(timerNumber[zyunokurai]);
			}


			int ichinokurai = gameTime / 60 % 10;
			timerEach[1].position.m128_f32[0] = 0.0f;
			timerEach[1].position.m128_f32[1] = 18.0f;
			timerEach[1].position.m128_f32[2] = -1.0f;
			timerEach[1].scale = { size_x, size_x, size_x };
			timerEach[1].rotation.x = -30.0f;
			if (gameTime / 60 > CHANGE_COLOR_COUNT)
			{
				sNumbersObject[ichinokurai].Update(&timerEach[1]);
				Draw3DObject(sNumbersObject[ichinokurai]);
			}
			else
			{
				//numbersObject[ichinokurai].Update(&timerEach[1]);
				timerNumber[ichinokurai].Update(&timerEach[1]);
				//Draw3DObject(numbersObject[ichinokurai]);
				Draw3DObject(timerNumber[ichinokurai]);
			}
		}
		oldDisplay = nowScore;
		oldScore = nowScore;
		nowScore = checkObject.GetScore();

		//playerの頭上にスコアを出す
		if (checkObject.IsAddScore() && !isTutorial && !isPouseToTiTle && !isResultSceneChange)
		{
			int addComboint = checkObject.GetCombo();
			if (addComboint < 10 && addComboint >= 1)
			{
				reversEach[0].position = ConvertXMFLOAT3toXMVECTOR(OthlloPlayer::GetPosition());
				reversEach[0].position.m128_f32[0] -= 0.0f;
				reversEach[0].position.m128_f32[1] += 2.0f;
				reversEach[0].scale = { 0.2f, 0.2f, 0.2f };
				reversEach[0].rotation.x = -30.0f;
				numbersObject[addComboint % 10].Update(&reversEach[0]);
				Draw3DObject(numbersObject[addComboint % 10]);
				reverseObject.each.position = ConvertXMFLOAT3toXMVECTOR(OthlloPlayer::GetPosition());
				reverseObject.each.position.m128_f32[0] += 2.0f;
				reverseObject.each.position.m128_f32[1] += 2.0f;
				reverseObject.each.scale = { 0.1f, 0.1f, 0.1f };
				reverseObject.each.rotation.x = -30.0f;
				reverseObject.Update();
				Draw3DObject(reverseObject);
			}
			else if (addComboint < 100)
			{
				reversEach[0].position = ConvertXMFLOAT3toXMVECTOR(OthlloPlayer::GetPosition());
				reversEach[0].position.m128_f32[0] -= 0.0f;
				reversEach[0].position.m128_f32[1] += 1.5f;
				reversEach[0].scale = { 0.2f, 0.2f, 0.2f };
				reversEach[0].rotation.x = -30.0f;
				numbersObject[addComboint % 10].Update(&reversEach[0]);
				Draw3DObject(numbersObject[addComboint % 10]);
				reversEach[1].position = ConvertXMFLOAT3toXMVECTOR(OthlloPlayer::GetPosition());
				reversEach[1].position.m128_f32[0] -= 1.0f;
				reversEach[1].position.m128_f32[1] += 1.5f;
				reversEach[1].scale = { 0.2f, 0.2f, 0.2f };
				reversEach[1].rotation.x = -30.0f;
				numbersObject[addComboint / 10 % 10].Update(&reversEach[1]);
				Draw3DObject(numbersObject[addComboint / 10 % 10]);
				reverseObject.each.position = ConvertXMFLOAT3toXMVECTOR(OthlloPlayer::GetPosition());
				reverseObject.each.position.m128_f32[0] += 2.0f;
				reverseObject.each.position.m128_f32[1] += 2.0f;
				reverseObject.each.scale = { 0.1f, 0.1f, 0.1f };
				reverseObject.each.rotation.x = -30.0f;
				reverseObject.Update();
				Draw3DObject(reverseObject);
			}
		}

		if (oldDisplay != nowScore)
		{
			scoreChange = true;
			changeTime = 15 - abs(nowScore - displayScore);
			if (changeTime < 0)
			{
				changeTime = 1;
			}
			changeCount = 0;
		}
		if (scoreChange)
		{
			if (nowScore >= displayScore)
			{
				int sub = abs(nowScore - displayScore);
				if (sub < 100)
				{
					changeCount++;
					if (changeCount == changeTime)
					{
						displayScore++;
						if (displayScore >= nowScore)
						{
							displayScore = nowScore;
						}
						changeCount = 0;
					}
				}
				else
				{
					changeCount++;
					if (changeCount == changeTime)
					{
						displayScore += sub / 60;
						if (displayScore >= nowScore)
						{
							displayScore = nowScore;
						}
						changeCount = 0;
					}
				}
			}
			else
			{
				displayScore = nowScore;
				scoreChange = false;
				changeCount = 0;
				changeTime = 0;
			}
		}

		if (!isResultSceneChange)
		{
			XMFLOAT3 scoreScale = { 0.4f, 0.4f, 0.4f };
			float scoreDirection = 2.0f;
			float scoreBaseX = -6.0f;
			scoreEach[0].position = { scoreBaseX - scoreDirection * 0, 18.0f, -1.0f, 1.0f };
			scoreEach[0].scale = scoreScale;
			scoreEach[0].rotation.x = -30.0f;
			sNumbersObject[displayScore % 10].Update(&scoreEach[0]);
			Draw3DObject(sNumbersObject[displayScore % 10]);

			scoreEach[1].position = { scoreBaseX - scoreDirection * 1, 18.0f, -1.0f, 1.0f };
			scoreEach[1].scale = scoreScale;
			scoreEach[1].rotation.x = -30.0f;
			sNumbersObject[displayScore / 10 % 10].Update(&scoreEach[1]);
			Draw3DObject(sNumbersObject[displayScore / 10 % 10]);

			scoreEach[2].position = { scoreBaseX - scoreDirection * 2, 18.0f, -1.0f, 1.0f };
			scoreEach[2].scale = scoreScale;
			scoreEach[2].rotation.x = -30.0f;
			sNumbersObject[displayScore / 100 % 10].Update(&scoreEach[2]);
			Draw3DObject(sNumbersObject[displayScore / 100 % 10]);

			scoreEach[3].position = { scoreBaseX - scoreDirection * 3, 18.0f, -1.0f, 1.0f };
			scoreEach[3].scale = scoreScale;
			scoreEach[3].rotation.x = -30.0f;
			sNumbersObject[displayScore / 1000 % 10].Update(&scoreEach[3]);
			Draw3DObject(sNumbersObject[displayScore / 1000 % 10]);

			scoreEach[4].position = { scoreBaseX - scoreDirection * 4, 18.0f, -1.0f, 1.0f };
			scoreEach[4].scale = scoreScale;
			scoreEach[4].rotation.x = -30.0f;
			sNumbersObject[displayScore / 10000 % 10].Update(&scoreEach[4]);
			Draw3DObject(sNumbersObject[displayScore / 10000 % 10]);

			scoreEach[5].position = { scoreBaseX - scoreDirection * 5, 18.0f, -1.0f, 1.0f };
			scoreEach[5].scale = scoreScale;
			scoreEach[5].rotation.x = -30.0f;
			sNumbersObject[displayScore / 100000 % 10].Update(&scoreEach[5]);
			Draw3DObject(sNumbersObject[displayScore / 100000 % 10]);

			scoreEach[6].position = { scoreBaseX - scoreDirection * 6, 18.0f, -1.0f, 1.0f };
			scoreEach[6].scale = scoreScale;
			scoreEach[6].rotation.x = -30.0f;
			sNumbersObject[displayScore / 1000000 % 10].Update(&scoreEach[6]);
			Draw3DObject(sNumbersObject[displayScore / 1000000 % 10]);

			scoreObject.each.position = { scoreBaseX - 2.0f - scoreDirection * 8, 18.0f, -1.0f, 1.0f };
			scoreObject.each.scale = scoreScale;
			scoreObject.each.rotation.x = -30.0f;
			scoreObject.Update();
			Draw3DObject(scoreObject);
		}
		if (isTutorial)
		{
			othelloManager.TutorialTextDraw();
		}
	}
	if (countDown > 0 && isSceneChange == false && isModeSelect == false)
	{
		XMFLOAT3 scale = { 0.7f, 0.7f, 0.7f };
		if (countDown < 59)
		{
			startModel.each.position = { -1, 5.0f, -2.0f, 1.0f, };
			startModel.each.scale = scale;
			startModel.each.rotation.x = -30.0f;
			startModel.Update();
			Draw3DObject(startModel);
		}
		else
		{
			scoreEach[0].position = { -1, 5.0f, -2.0f, 1.0f };
			scoreEach[0].scale = scale;
			scoreEach[0].rotation.x = -30.0f;
			sNumbersObject[countDown / 60].Update(&scoreEach[0]);
			Draw3DObject(sNumbersObject[countDown / 60]);
		}
	}
	if (!isSceneChange && !isPouseToTiTle)
	{
		OthlloPlayer::Draw();
	}
	ObjectParticles::Draw();
	ParticleControl::Draw();
	if (!isPouseToTiTle)othelloManager.Draw();
	if (selectMode && !isSceneChange && !isPouseToTiTle)
	{
		othelloManager.NormaTextModelDraw(selectStageNum, true);
	}
	if (isModeSelect && !isSceneChange && !isPouseToTiTle)
	{
		othelloManager.ModeSelectModelDraw(true);
	}
	if (isPouse)
	{
		pouseBack.ChangeSize(1280, 720);
		pouseBack.position.m128_f32[0] = 0;
		pouseBack.position.m128_f32[1] = 0;
		pouseBack.SpriteDraw();
		backSprite.position.m128_f32[0] = window_width / 2 - 100;
		backSprite.position.m128_f32[1] = window_height / 2;
		backSprite.SpriteDraw();
		titleSprite.position.m128_f32[0] = window_width / 2 - 100;
		titleSprite.position.m128_f32[1] = window_height / 2 + 80;
		titleSprite.SpriteDraw();
		endSprite.position.m128_f32[0] = window_width / 2 - 100;
		endSprite.position.m128_f32[1] = window_height / 2 + 170;
		endSprite.SpriteDraw();
		if (selectPouse == 0)
		{
			kagikakkoStartSprite.ChangeSize(30, 70);
			kagikakkoStartSprite.position.m128_f32[0] = window_width / 2 - 140;
			kagikakkoStartSprite.position.m128_f32[1] = window_height / 2 + 5;
			kagikakkoStartSprite.SpriteDraw();
			kagikakkoEndSprite.ChangeSize(30, 70);
			kagikakkoEndSprite.position.m128_f32[0] = window_width / 2 + 125;
			kagikakkoEndSprite.position.m128_f32[1] = window_height / 2 + 5;
			kagikakkoEndSprite.SpriteDraw();
		}
		else if (selectPouse == 1)
		{
			kagikakkoStartSprite.ChangeSize(30, 70);
			kagikakkoStartSprite.position.m128_f32[0] = window_width / 2 - 140;
			kagikakkoStartSprite.position.m128_f32[1] = window_height / 2 + 80;
			kagikakkoStartSprite.SpriteDraw();
			kagikakkoEndSprite.ChangeSize(30, 70);
			kagikakkoEndSprite.position.m128_f32[0] = window_width / 2 + 125;
			kagikakkoEndSprite.position.m128_f32[1] = window_height / 2 + 80;
			kagikakkoEndSprite.SpriteDraw();
		}
		else if (selectPouse == 2)
		{
			kagikakkoStartSprite.ChangeSize(30, 70);
			kagikakkoStartSprite.position.m128_f32[0] = window_width / 2 - 140;
			kagikakkoStartSprite.position.m128_f32[1] = window_height / 2 + 170;
			kagikakkoStartSprite.SpriteDraw();
			kagikakkoEndSprite.ChangeSize(30, 70);
			kagikakkoEndSprite.position.m128_f32[0] = window_width / 2 + 75;
			kagikakkoEndSprite.position.m128_f32[1] = window_height / 2 + 170;
			kagikakkoEndSprite.SpriteDraw();
		}
	}

	//tips用
	if (isTipsDraw)
	{
		isTipsDrawTrigger = true;

		//pushButton_green.Update();
		Draw3DObject(pushButton_green);

		pushButton_black.Update(&pushButton[0]);
		Draw3DObject(pushButton_black);
		pushButton_black.Update(&pushButton[1]);
		Draw3DObject(pushButton_black);
		pushButton_black.Update(&pushButton[2]);
		Draw3DObject(pushButton_black);

		tips_ss[0].SpriteDraw();
		tips_ss[1].SpriteDraw();
		tips_ss[2].SpriteDraw();
		tips_ss[3].SpriteDraw();
		//tips[tipsCounts].SpriteDraw();

		tips_frame.SpriteDraw();
		tips_name[tipsCounts].SpriteDraw();
		if (tipsCounts < 5) { tips_system[0].SpriteDraw(); }
		else { tips_system[1].SpriteDraw(); }
		tips_bar.SpriteDraw();
		tips_text[tipsCounts].SpriteDraw();
	}
	else if (!isTipsDraw && isTipsDrawTrigger)
	{
		tipsCounts++;
		isTipsDrawTrigger = false;
		if (tipsCounts > 8) { tipsCounts = 0; }
	}

	else if (isCameraModed)
	{
		Draw3DObject(pushButton_green);

		//XMFLOAT3 rot = pushButton_black.each.rotation;
		//XMFLOAT3 rot2 = pushButton_green.each.rotation;

		pushButton_black.Update(&pushButton[0]);
		Draw3DObject(pushButton_black);
		pushButton_black.Update(&pushButton[1]);
		Draw3DObject(pushButton_black);
		pushButton_black.Update(&pushButton[2]);
		Draw3DObject(pushButton_black);
	}
	//Imgui::DrawImGui();
	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::ResultDraw()
{
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	sky.each.rotation.y += 1.0f;
	sky.Update();
	Draw3DObject(sky);
	Draw3DObject(othelloStage);
	nowScore = checkObject.GetScore();
	float scoreNum = 2.0f;
	scoreEach[0].position = { 1.0f + scoreNum * 6, 20.0f, -12.0f, 1.0f };
	scoreEach[0].scale = { 0.4f, 0.4f, 0.4f };
	scoreEach[0].rotation.x = -70.0f;
	sNumbersObject[nowScore % 10].Update(&scoreEach[0]);
	Draw3DObject(sNumbersObject[nowScore % 10]);
	scoreEach[1].position = { 1.0f + scoreNum * 5, 20.0f, -12.0f, 1.0f };
	scoreEach[1].scale = { 0.4f, 0.4f, 0.4f };
	scoreEach[1].rotation.x = -70.0f;
	sNumbersObject[nowScore / 10 % 10].Update(&scoreEach[1]);
	Draw3DObject(sNumbersObject[nowScore / 10 % 10]);
	scoreEach[2].position = { 1.0f + scoreNum * 4, 20.0f, -12.0f, 1.0f };
	scoreEach[2].scale = { 0.4f, 0.4f, 0.4f };
	scoreEach[2].rotation.x = -70.0f;
	sNumbersObject[nowScore / 100 % 10].Update(&scoreEach[2]);
	Draw3DObject(sNumbersObject[nowScore / 100 % 10]);
	scoreEach[3].position = { 1.0f + scoreNum * 3, 20.0f, -12.0f, 1.0f };
	scoreEach[3].scale = { 0.4f, 0.4f, 0.4f };
	scoreEach[3].rotation.x = -70.0f;
	sNumbersObject[nowScore / 1000 % 10].Update(&scoreEach[3]);
	Draw3DObject(sNumbersObject[nowScore / 1000 % 10]);
	scoreEach[4].position = { 1.0f + scoreNum * 2, 20.0f, -12.0f, 1.0f };
	scoreEach[4].scale = { 0.4f, 0.4f, 0.4f };
	scoreEach[4].rotation.x = -70.0f;
	sNumbersObject[nowScore / 10000 % 10].Update(&scoreEach[4]);
	Draw3DObject(sNumbersObject[nowScore / 10000 % 10]);
	scoreEach[5].position = { 1.0f + scoreNum * 1, 20.0f, -12.0f, 1.0f };
	scoreEach[5].scale = { 0.4f, 0.4f, 0.4f };
	scoreEach[5].rotation.x = -70.0f;
	sNumbersObject[nowScore / 100000 % 10].Update(&scoreEach[5]);
	Draw3DObject(sNumbersObject[nowScore / 100000 % 10]);
	scoreEach[6].position = { 1.0f + scoreNum * 0, 20.0f, -12.0f, 1.0f };
	scoreEach[6].scale = { 0.4f, 0.4f, 0.4f };
	scoreEach[6].rotation.x = -70.0f;
	sNumbersObject[nowScore / 1000000 % 10].Update(&scoreEach[6]);
	Draw3DObject(sNumbersObject[nowScore / 1000000 % 10]);
	scoreObject.each.position = { -5.0f, 20.0f, -12.0f, 1.0f };
	scoreObject.each.scale = { 0.4f, 0.4f, 0.4f };
	scoreObject.each.rotation.x = -70.0f;
	scoreObject.Update();
	Draw3DObject(scoreObject);

	XMFLOAT3 scale = { 0.4f, 0.4f, 0.4f };
	titleObject.each.position = { -1, 3, 0, 1.0f };
	titleObject.each.scale = scale;
	titleObject.Update();
	Draw3DObject(titleObject);
	backGroundEach[0].position = { -1, 4.5f, 4.0f, 1.0f };
	backGroundEach[0].scale = { 20.0f, 4.0f, 1.0f };
	backGround.Update(&backGroundEach[0]);
	Draw3DObject(backGround);
	backGroundEach[1].position = { -1, -6.5f, 4.0f, 1.0f };
	backGroundEach[1].scale = { 23.0f, 1.5f, 1.0f };
	backGround.Update(&backGroundEach[1]);
	Draw3DObject(backGround);
	XMFLOAT3 scale2 = { 0.3f, 0.3f, 0.3f };
	startModel.each.position = { -1 + 5.0f, -2.0f, 0, 1.0f };
	startModel.each.scale = scale2;
	startModel.Update();
	Draw3DObject(startModel);
	XMFLOAT3 scale3 = { 0.3f, 0.3f, 0.3f };
	tutorialObject.each.position = { -1 - 5.0f, -2.0f, 0, 1.0f };
	tutorialObject.each.scale = scale2;
	tutorialObject.Update();
	Draw3DObject(tutorialObject);
	XMFLOAT3 scale4 = { 0.3f, 0.3f, 0.3f };
	pushSpace.each.position = { -1 + 6.0f, -5.0f, 0, 1.0f };
	pushSpace.each.scale = scale4;
	pushSpace.Update();
	Draw3DObject(pushSpace);

	//Imgui::DrawImGui();

	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::ReStart()
{
	//SoundStopWave(timeUpSound);
	//SoundPlayOnce(timeUpSound);
	for (auto triangleItr = OthelloManager::othellos.begin(); triangleItr != OthelloManager::othellos.end(); ++triangleItr)
	{
		XMFLOAT3 pos = triangleItr->GetPosition();
		ObjectParticles::triangle.Init(pos, 2, ParticleType::Exprotion);
		triangleItr->GetGameData()->isDead = true;
	}
	othelloManager.DeadPanel();
	eyeStart = Camera::target.v;
	eyeEnd = { 0, 100, 0 };
	nowScore = 0;
	displayScore = 0;
	oldDisplay = 0;
	countDown = 239;
	gameTime = gameMaxTime;
	checkObject.SetScore(0);
	OthlloPlayer::SetPosition(XMFLOAT3(0, 0, -2));
	OthlloPlayer::startPos = { 0, 0, -2 };
	OthlloPlayer::endPos = { 0, 0, -2 };
	OthlloPlayer::easeTime = 0.0f;
	OthlloPlayer::isEase = false;
	othelloManager.StartSetPos();
}

void GameScene::ToGame()
{
	checkObject.Init();
	//オセロを爆散させてカメラの動きを開始させる
	for (auto triangleItr = ObjectParticles::othello.particles.begin(); triangleItr != ObjectParticles::othello.particles.end(); ++triangleItr)
	{
		XMFLOAT3 pos = ConvertXMVECTORtoXMFLOAT3(triangleItr->each.position);
		ObjectParticles::triangle.Init(pos, 2, ParticleType::Exprotion);
		triangleItr->time = 1;
		countDown = countMax;
	}
	isSceneChange = true;
	sceneChageType = 1;
	isPouse = false;
	eyeStart = Camera::target.v;
	eyeEnd = { 0.0f, 0.0f, 0.0f };
	eyeEaseTime = 0;
	SceneNum = GAME;
	nowScore = 0;
	displayScore = 0;
	oldDisplay = 0;
	countDown = 239;
	checkObject.SetScore(0);
	OthlloPlayer::SetPosition(XMFLOAT3(0, 0, -2));
	OthlloPlayer::isEase = false;
}

void GameScene::ToGame2()
{
	checkObject.Init();
	isSceneChange = true;
	sceneChageType = 2;
	isPouse = false;
	SceneNum = GAME;

	sceneChangeSpriteStartPos = { 0, 0, 0 };
	sceneChangeSpriteEndPos = { 0, 0, 0 };
	//sceneChangeSprite2.position = ConvertXMFLOAT3toXMVECTOR(sceneChangeSpriteStartPos);
	sceneChangeSprite2.position = { 0, 0, 0, 1 };
	sceneChangeSprite2.rotation = -2.0f;
	isSceneChangeRady = false;

	eyeStart = Camera::target.v;
	eyeEnd = { -1.0f, 0.0f, 0.0f };
	eyeEaseTime = 0.0f;
	sceneChangeAfterTime = 0.0f;

	sceneChangeDiray2 = 0;

	nowScore = 0;
	displayScore = 0;
	oldDisplay = 0;
	countDown = 239;
	checkObject.SetScore(0);
	OthlloPlayer::SetPosition(XMFLOAT3(0, 0, -2));
	OthlloPlayer::isEase = false;
}

void GameScene::ToGame3()
{
	//初期化
	checkObject.Init();
	//シーンが変わることを伝える
	isSceneChange = true;
	//シーンの種類を決める
	sceneChageType = 3;

	isPouse = false;
	SceneNum = GAME;
	//カメラの終点を決める
	eyeStart = Camera::target.v;
	eyeEnd = gameNowEye;
	eyeEaseTime = 0.0f;
	sceneChangeAfterTime = 0.0f;

	sceneChangeDiray2 = 0;
	sceneChangeAfterTime = 0.0f;
	//なんやかんやの初期化
	nowScore = 0;
	displayScore = 0;
	oldDisplay = 0;
	countDown = 239;
	checkObject.SetScore(0);
	OthlloPlayer::SetPosition(XMFLOAT3(0, 0, -2));
	OthlloPlayer::isEase = false;
}

void GameScene::ToGame4(bool flags)
{
	//初期化
	checkObject.Init();
	//シーンが変わることを伝える
	isSceneChange = true;
	//シーンの種類を決める
	sceneChageType = 4;
	isTipsDraw = false;
	isPouse = false;
	SceneNum = GAME;
	//カメラの終点を決める
	eyeStart = XMFLOAT3(-1, 1, -15.0f);
	eyeEnd = gameNowEye;
	camTargetStart = { -1, 1, 0 };
	camTargetEnd = { -1, 0, 0 };

	eyeEaseTime = 0.0f;
	sceneChangeAfterTime = 0.0f;

	isModeSelect = false;

	gameTime = gameMaxTime;

	isTipsOk = false;
	isCameraModed = false;
	isStageDisplay = false;
	goToGameTime = 0.0f;
	sceneChangeDiray2 = 0;
	//なんやかんやの初期化
	nowScore = 0;
	displayScore = 0;
	oldDisplay = 0;
	countDown = 239;
	checkObject.SetScore(0);
	othelloManager.normaChecker.isClear = false;
	OthlloPlayer::SetPosition(XMFLOAT3(0, 0, -2));
	OthlloPlayer::isEase = false;

	ObjectParticles::othello.DeleteAllParticle();
	flagss = flags;

	tipsNumber = rand() % GameScene::tipsCount;
}

void GameScene::ToTutorial()
{//初期化
	checkObject.Init();
	//シーンが変わることを伝える
	isSceneChange = true;
	//シーンの種類を決める
	sceneChageType = 6;
	isTipsDraw = false;
	isPouse = false;
	SceneNum = GAME;
	//カメラの終点を決める
	eyeStart = XMFLOAT3(-1, 1, -15.0f);
	eyeEnd = gameNowEye;
	camTargetStart = { -1, 1, 0 };
	camTargetEnd = { -1, 0, 0 };

	eyeEaseTime = 0.0f;
	sceneChangeAfterTime = 0.0f;

	isModeSelect = false;

	gameTime = gameMaxTime;

	isTipsOk = false;
	isCameraModed = false;
	isStageDisplay = false;
	goToGameTime = 0.0f;
	sceneChangeDiray2 = 0;
	//なんやかんやの初期化
	nowScore = 0;
	displayScore = 0;
	oldDisplay = 0;
	countDown = 239;
	checkObject.SetScore(0);
	othelloManager.normaChecker.isClear = false;
	OthlloPlayer::SetPosition(XMFLOAT3(0, 0, -2));
	OthlloPlayer::isEase = false;

	ObjectParticles::othello.DeleteAllParticle();
	//flagss = flags;

	tipsNumber = rand() % GameScene::tipsCount;
}

void GameScene::ToGame1Update()
{
	Camera::target.v = ShlomonMath::EaseInQuad(eyeStart, eyeEnd, eyeEaseTime);
	eyeEaseTime += 0.02f;
	if (eyeEaseTime > 1.0f)
	{
		Camera::target.v = ShlomonMath::EaseInQuad(eyeStart, eyeEnd, 1.0f);
		isSceneChange = false;
		if (isTutorial)
		{
			gameTime = 1;
		}
		else
		{
			gameTime = gameMaxTime;
			SoundStopWave(countdDownSound);
			SoundPlayOnce(countdDownSound);
		}
	}
	Camera::Update();
}

void GameScene::ToGame2Update()
{//回転スタート
	sceneChangeSprite2.rotation += 0.05f;
	//はじめの回転の時間
	eyeEaseTime += 0.02f;
	//画面を覆ったらいったん止めて
	if (sceneChangeSprite2.rotation > 0.0f)
	{
		eyeEaseTime = 1.0f;
		//一定時間立つまでまつ
		if (!isSceneChangeRady)sceneChangeSprite2.rotation = 0.0f;
		//カメラをゲーム画面に固定
		Camera::target.v = ShlomonMath::EaseInQuad(eyeStart, eyeEnd, 1.0f);
		Camera::eye.v = gameNowEye;
		sceneChangeDiray2++;
		//一定時間たったら
		if (sceneChangeDiray2 > 120)
		{
			isSceneChangeRady = true;
			if (sceneChangeSprite2.rotation < 2.0f)sceneChangeSprite2.rotation += 0.05f;
			sceneChangeAfterTime += 0.02f;
			//ゲーム画面」に移行し、カウントダウン開始
			if (sceneChangeAfterTime >= 1.0f)
			{
				isSceneChange = false;
				if (isTutorial)
				{
					gameTime = 1;
				}
				else
				{
					gameTime = gameMaxTime;
					SoundStopWave(countdDownSound);
					SoundPlayOnce(countdDownSound);
				}
			}
		}
	}
	Camera::Update();
}

void GameScene::ToGame3Update()
{
	Camera::eye.v = { 0, -20, -15 };
	Camera::target.v = { 0 ,-20 ,0 };
	//はじめの回転の時間
	eyeEaseTime += 0.02f;
	for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
	{
		opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(0.01f, 0.01f, 0.01f), XMFLOAT3(1.5f, 1.5f, 1.0f), eyeEaseTime);
		opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 90), eyeEaseTime);
		opOthelloItr->rotation.y = 180;
	}
	if (eyeEaseTime >= 1.0f)
	{
		sceneChangeDiray2++;
		eyeEaseTime = 1.0f;
		for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
		{
			opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(0.01f, 0.01f, 0.01f), XMFLOAT3(1.5f, 1.5f, 1.0f), eyeEaseTime);
			opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 90), eyeEaseTime);
			opOthelloItr->rotation.y = 180;
		}
		if (sceneChangeDiray2 > 120)
		{
			sceneChangeAfterTime += 0.02f;
			for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
			{
				opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.0f), XMFLOAT3(0.01f, 0.01f, 1.0f), sceneChangeAfterTime);
				opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 90), XMFLOAT3(0, 0, 0), sceneChangeAfterTime);
				opOthelloItr->rotation.y = 180;
			}
			//カメラをゲーム画面に固定
			//Camera::target.v = gameNowTarget;
			//Camera::eye.v = gameNowEye;
			if (sceneChangeAfterTime >= 1.0f)
			{
				Camera::target.v = gameNowTarget;
				Camera::eye.v = gameNowEye;
				sceneChangeAfterTime = 1.0f;
				for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
				{
					opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.0f), XMFLOAT3(0.01f, 0.01f, 1.0f), sceneChangeAfterTime);
					opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 90), XMFLOAT3(0, 0, 0), sceneChangeAfterTime);
					opOthelloItr->rotation.y = 180;
				}
				isSceneChange = false;
				if (isTutorial)
				{
					gameTime = 1;
				}
				else
				{
					gameTime = gameMaxTime;
					SoundStopWave(countdDownSound);
					SoundPlayOnce(countdDownSound);
				}
			}

		}
	}
	Camera::Update();
}

void GameScene::ToGame4Update()
{
	Camera::eye.v = eyeStart;
	Camera::target.v = camTargetStart;
	eyeEaseTime += 0.02f;
	for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
	{
		opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(0.01f, 0.01f, 0.01f), XMFLOAT3(1.5f, 1.5f, 1.0f), eyeEaseTime);
		opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 90), eyeEaseTime);
	}
	if (eyeEaseTime >= 1.0f)
	{
		isStageDisplay = true;
		isTipsDraw = true;
		eyeEaseTime = 1.0f;
		sceneChangeDiray2++;
		for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
		{
			opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(0.01f, 0.01f, 0.01f), XMFLOAT3(1.5f, 1.5f, 1.0f), eyeEaseTime);
			opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 90), eyeEaseTime);
		}
		if (sceneChangeDiray2 >= MaxSceneChangeOk)
		{

			if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
			{
				if (!selectMode && !isTipsOk)
				{
					othelloManager.StartSetPos();
				}
				if (flagss && !isTipsOk)
				{
					othelloManager.StartNormaMode(selectStageNum);
				}
				isTipsOk = true;
			}
			if (isTipsOk)
			{
				isTipsDraw = false;
				sceneChangeAfterTime += 0.01f;
				for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
				{
					opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.5f), XMFLOAT3(0.01f, 0.01f, 0.01f), sceneChangeAfterTime);
					opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 90), XMFLOAT3(0, 0, 0), sceneChangeAfterTime);
				}
			}
		}
		if (sceneChangeAfterTime >= 0.8f && sceneChangeAfterTime < 1.0f)
		{
			for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
			{
				opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.5f), XMFLOAT3(0.01f, 0.01f, 0.01f), sceneChangeAfterTime);
				opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 90), XMFLOAT3(0, 0, 0), sceneChangeAfterTime);
			}
			goToGameTime += 0.02f;
			Camera::eye.v = ShlomonMath::EaseInQuad(eyeStart, eyeEnd, goToGameTime);
			if (goToGameTime >= 1.0f)
			{
				isSceneChange = false;
				isCameraModed = true;
				if (isTutorial)
				{
					gameTime = 1;
				}
				else
				{
					gameTime = gameMaxTime;
					SoundStopWave(countdDownSound);
					SoundPlayOnce(countdDownSound);
				}
			}
		}
		//ゲーム画面」に移行し、カウントダウン開始
		if (sceneChangeAfterTime >= 1.0f)
		{
			sceneChangeAfterTime = 1.0f;
			for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
			{
				opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.5f), XMFLOAT3(0.01f, 0.01f, 0.01f), sceneChangeAfterTime);
				opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 90), XMFLOAT3(0, 0, 0), sceneChangeAfterTime);
			}
			goToGameTime += 0.02f;
			Camera::eye.v = ShlomonMath::EaseInQuad(eyeStart, eyeEnd, goToGameTime);
			if (goToGameTime >= 1.0f)
			{
				isSceneChange = false;
				isCameraModed = true;
				if (isTutorial)
				{
					gameTime = 1;
				}
				else
				{
					gameTime = gameMaxTime;
					SoundStopWave(countdDownSound);
					SoundPlayOnce(countdDownSound);
				}
			}
		}
	}
	Camera::Update();
}

void GameScene::ToModeSelectUpdate()
{
	Camera::eye.v = eyeStart;
	Camera::target.v = camTargetStart;
	eyeEaseTime += 0.02f;
	for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
	{
		opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(0.01f, 0.01f, 0.01f), XMFLOAT3(1.5f, 1.5f, 1.0f), eyeEaseTime);
		opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 90), eyeEaseTime);
	}
	if (eyeEaseTime >= 1.0f)
	{
		isTipsDraw = true;
		isStageDisplay = true;
		eyeEaseTime = 1.0f;
		sceneChangeDiray2++;
		for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
		{
			opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(0.01f, 0.01f, 0.01f), XMFLOAT3(1.5f, 1.5f, 1.0f), eyeEaseTime);
			opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 90), eyeEaseTime);
		}
		if (sceneChangeDiray2 >= MaxSceneChangeOk)
		{

			if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
			{
				if (!isTipsOk)othelloManager.ModeSelectStart();
				isTipsOk = true;
			}
			if (isTipsOk)
			{
				isTipsDraw = false;
				sceneChangeAfterTime += 0.01f;
				for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
				{
					opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.5f), XMFLOAT3(0.01f, 0.01f, 0.01f), sceneChangeAfterTime);
					opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 90), XMFLOAT3(0, 0, 0), sceneChangeAfterTime);
				}
			}
		}
		if (sceneChangeAfterTime >= 0.8f && sceneChangeAfterTime < 1.0f)
		{
			for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
			{
				opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.5f), XMFLOAT3(0.01f, 0.01f, 0.01f), sceneChangeAfterTime);
				opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 90), XMFLOAT3(0, 0, 0), sceneChangeAfterTime);
			}
			goToGameTime += 0.02f;
			Camera::eye.v = ShlomonMath::EaseInQuad(eyeStart, eyeEnd, goToGameTime);
			if (goToGameTime >= 1.0f)
			{
				isSceneChange = false;
				isCameraModed = true;
			}
		}
		//ゲーム画面」に移行し、カウントダウン開始
		else if (sceneChangeAfterTime >= 1.0f)
		{
			sceneChangeAfterTime = 1.0f;
			for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
			{
				opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.5f), XMFLOAT3(0.01f, 0.01f, 0.01f), sceneChangeAfterTime);
				opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 90), XMFLOAT3(0, 0, 0), sceneChangeAfterTime);
			}
			goToGameTime += 0.02f;
			Camera::eye.v = ShlomonMath::EaseInQuad(eyeStart, eyeEnd, goToGameTime);
			if (goToGameTime >= 1.0f)
			{
				isSceneChange = false;
				isCameraModed = true;
				OthlloPlayer::isReverse = true;
				OthlloPlayer::playerReverseObject->PlayAnimation();
			}
		}
	}
	Camera::Update();
}

void GameScene::ToTutorialUpdate()
{
	Camera::eye.v = eyeStart;
	Camera::target.v = camTargetStart;
	eyeEaseTime += 0.02f;
	for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
	{
		opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(0.01f, 0.01f, 0.01f), XMFLOAT3(1.5f, 1.5f, 1.0f), eyeEaseTime);
		opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 90), eyeEaseTime);
	}
	if (eyeEaseTime >= 1.0f)
	{
		isStageDisplay = true;
		isTipsDraw = true;
		eyeEaseTime = 1.0f;
		sceneChangeDiray2++;
		for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
		{
			opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(0.01f, 0.01f, 0.01f), XMFLOAT3(1.5f, 1.5f, 1.0f), eyeEaseTime);
			opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 90), eyeEaseTime);
		}
		if (sceneChangeDiray2 >= MaxSceneChangeOk)
		{
			if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
			{
				isTutorial = true;
				titleSelectNum = 0;
				othelloManager.whySandwichSpawn();
				gameTime = 60;
				isTipsOk = true;
			}
			if (isTipsOk)
			{
				isTipsDraw = false;
				sceneChangeAfterTime += 0.01f;
				for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
				{
					opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.5f), XMFLOAT3(0.01f, 0.01f, 0.01f), sceneChangeAfterTime);
					opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 90), XMFLOAT3(0, 0, 0), sceneChangeAfterTime);
				}
			}
		}
		if (sceneChangeAfterTime >= 0.8f && sceneChangeAfterTime < 1.0f)
		{
			for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
			{
				opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.5f), XMFLOAT3(0.01f, 0.01f, 0.01f), sceneChangeAfterTime);
				opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 90), XMFLOAT3(0, 0, 0), sceneChangeAfterTime);
			}
			goToGameTime += 0.02f;
			Camera::eye.v = ShlomonMath::EaseInQuad(eyeStart, eyeEnd, goToGameTime);
			if (goToGameTime >= 1.0f)
			{
				isSceneChange = false;
				isCameraModed = true;
				if (isTutorial)
				{
					gameTime = 1;
					SoundStopWave(countdDownSound);
					SoundPlayOnce(countdDownSound);
				}
				else
				{
					gameTime = gameMaxTime;
					SoundStopWave(countdDownSound);
					SoundPlayOnce(countdDownSound);
				}
			}
		}
		//ゲーム画面」に移行し、カウントダウン開始
		if (sceneChangeAfterTime >= 1.0f)
		{
			sceneChangeAfterTime = 1.0f;
			for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
			{
				opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.5f), XMFLOAT3(0.01f, 0.01f, 0.01f), sceneChangeAfterTime);
				opOthelloItr->rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 90), XMFLOAT3(0, 0, 0), sceneChangeAfterTime);
			}
			goToGameTime += 0.02f;
			Camera::eye.v = ShlomonMath::EaseInQuad(eyeStart, eyeEnd, goToGameTime);
			if (goToGameTime >= 1.0f)
			{
				isSceneChange = false;
				isCameraModed = true;
				if (isTutorial)
				{
					gameTime = 1;
					SoundStopWave(countdDownSound);
					SoundPlayOnce(countdDownSound);
				}
				else
				{
					gameTime = gameMaxTime;
					SoundStopWave(countdDownSound);
					SoundPlayOnce(countdDownSound);
				}
			}
		}
	}
	Camera::Update();
}

void GameScene::ToModeSelect()
{
	//初期化
	checkObject.Init();
	//シーンが変わることを伝える
	isSceneChange = true;
	//シーンの種類を決める
	sceneChageType = 5;

	isPouse = false;
	SceneNum = GAME;
	//カメラの終点を決める
	eyeStart = XMFLOAT3(-1, 1, -15.0f);
	eyeEnd = gameNowEye;
	camTargetStart = { -1, 1, 0 };
	camTargetEnd = { -1, 0, 0 };

	eyeEaseTime = 0.0f;
	sceneChangeAfterTime = 0.0f;

	//
	isModeSelect = true;

	isTipsOk = false;
	isStageDisplay = false;
	goToGameTime = 0.0f;
	sceneChangeDiray2 = 0;
	//なんやかんやの初期化
	nowScore = 0;
	displayScore = 0;
	oldDisplay = 0;
	countDown = 239;
	checkObject.SetScore(0);
	OthlloPlayer::SetPosition(XMFLOAT3(0, 0, -2));
	OthlloPlayer::isEase = false;

	ObjectParticles::othello.DeleteAllParticle();

	select = true;
	selectStage = false;
	selectMode = false;

	othelloManager.AllDeletePanel();
	othelloManager.EraseChanceObject();

	tipsNumber = rand() % GameScene::tipsCount;

	isCameraModed = false;
}

void GameScene::ToTitle()
{
	//シーンの種類を決める
	sceneChageType = 10;
	//カメラの終点を決める
	eyeStart = Camera::target.v;
	eyeEnd = { -1, 1, 0 };
	camTargetStart = Camera::eye.v;
	camTargetEnd = { -1, 1, -15 };
	eyeEaseTime = 0.0f;
	isResultStart = true;
}

void GameScene::ToTitleUpdate()
{
	eyeEaseTime += 0.02f;
	if (eyeEaseTime >= 1.0f)
	{
		eyeEaseTime = 1.0f;
		SceneNum = TITLE;
		isResultStart = false;
	}
	light->SetLightDir(XMFLOAT3(Camera::GetTargetDirection()));
	LightUpdate();
	Camera::target.v = ShlomonMath::EaseInQuad(eyeStart, eyeEnd, eyeEaseTime);
	Camera::eye.v = ShlomonMath::EaseInQuad(camTargetStart, camTargetEnd, eyeEaseTime);
	Camera::Update();
}

void GameScene::ToResult()
{
	SoundStopWave(BGMSound);
	SoundStopWave(timeUpSound);
	SoundPlayOnce(timeUpSound);
	for (auto triangleItr = OthelloManager::othellos.begin(); triangleItr != OthelloManager::othellos.end(); ++triangleItr)
	{
		XMFLOAT3 pos = triangleItr->GetPosition();
		ObjectParticles::triangle.Init(pos, 2, ParticleType::Exprotion);
		triangleItr->GetGameData()->isDead = true;
	}
	othelloManager.DeadPanel();
	isResultSceneChange = true;
	isSceneChange = false;
	eyeStart = Camera::target.v;
	eyeEnd = { -1, 100, 0 };
	eyeEaseTime = 0;
	resultForTime = 0;
	select = false;
	sceneChageType = 0;
	ObjectParticles::othelloFrame.DeleteAllParticle();
}

void GameScene::PouseToTitle()
{
	//gameTime = 0;
	sceneChageType = 20;
	isPouseToTiTle = true;
	isPouse = false;
	isTutorial = false;
	select = false;
	pouseToTitleEaseTime1 = 0.0f;
	isBackGroundOthello = false;
	isSceneChange = true;
	titleScaleEaseTime = 0.0f;
	ObjectParticles::othelloFrame.DeleteAllParticle();
	ObjectParticles::six.DeleteAllParticle();
	ObjectParticles::frame.DeleteAllParticle();
	othelloManager.AllDeletePanel();
	othelloManager.EraseChanceObject();
}

void GameScene::PouseToTitleUpdate()
{
	if (isPouseToTiTle)
	{
		ObjectParticles::frame.DeleteAllParticle();
		pouseToTitleEaseTime1 += 0.02f;
		if (pouseToTitleEaseTime1 >= 1.0f)
		{
			pouseToTitleEaseTime1 = 1.0f;
			isBackGroundOthello = true;
			isTipsDraw = true;
		}
		if (!isBackGroundOthello)
		{
			for (auto pouseOthelloItr = pouseOthellos.begin(); pouseOthelloItr != pouseOthellos.end(); ++pouseOthelloItr)
			{
				pouseOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(0.01f, 0.01f, 0.01f), XMFLOAT3(1.5f, 1.5f, 1.0f), pouseToTitleEaseTime1);
			}
		}
		else
		{
			if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
			{
				SceneNum = TITLE;
				isTipsDraw = false;
				isSceneChange = false;
			}
			Camera::target = { -1, 1, 0 };
			Camera::eye = { -1, 1, -15 };
			Camera::Update();
			if (SceneNum != TITLE)
			{
				for (auto pouseOthelloItr = opOthellos.begin(); pouseOthelloItr != opOthellos.end(); ++pouseOthelloItr)
				{
					pouseOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.0f), XMFLOAT3(0.01f, 0.01f, 0.01f), 0.0f);
				}
			}
			else
			{
				titleScaleEaseTime += 0.02f;
				if (titleScaleEaseTime >= 1.0f)
				{
					titleScaleEaseTime = 1.0f;
					isPouseToTiTle = false;
				}
				for (auto pouseOthelloItr = opOthellos.begin(); pouseOthelloItr != opOthellos.end(); ++pouseOthelloItr)
				{
					pouseOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.0f), XMFLOAT3(0.01f, 0.01f, 0.01f), titleScaleEaseTime);
				}
			}
		}
	}
}

void GameScene::EndDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw();
	Draw3DObject(sample);
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffectのDraw
	//postEffect.Draw();

	//スプライトの描画-------------------------
	//titleSprite.SpriteDraw();
	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::LightUpdate()
{
	light->SetPointLightActive(0, false);
	light->SetSpotLightActive(0, false);
	//light->SetCircleShadowActive(0, false);
	light->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	light->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));
	light->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	light->SetSpotLightDir(0, XMVECTOR({ spotLightDir[0], spotLightDir[1], spotLightDir[2], 0 }));
	light->SetSpotLightPos(0, XMFLOAT3(spotLightPos));
	light->SetSpotLightColor(0, XMFLOAT3(spotLightColor));
	light->SetSpotLightAtten(0, XMFLOAT3(spotLightAtten));
	light->SetSpotLightAngle(0, XMFLOAT2(spotLightAngle));
}
