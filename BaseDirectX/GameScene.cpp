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
	scoreSprite.CreateSprite(L"Resource/Img/score.png", XMFLOAT3(0, 0, 0));
	timeUp.CreateSprite(L"Resource/Img/time_up.png", XMFLOAT3(0, 0, 0));
	startSprite.CreateSprite(L"Resource/Img/START.png", XMFLOAT3(0, 0, 0));
	gameTime = gameMaxTime;
	Lights::LoadModels();
	OthlloPlayer::Init();
	sky.CreateModel("world", ShaderManager::playerShader, true);
	sky.each.rotation = { 0, 0.0f, 0 };
	sky.each.scale = { 100.0f ,100.0f, 100.f };
	othelloStage.CreateModel("board", ShaderManager::playerShader);
	othelloStage.each.position.m128_f32[0] = -1.0f;
	othelloStage.each.position.m128_f32[1] = 1.0f;
	othelloStage.each.position.m128_f32[2] = 2.5f;
	othelloStage.each.rotation.x = 90;
	othelloStage.each.scale = { 2.0f, 1.0f, 2.0f };
	isPouse = false;
	isTipsButtonDraw = false;
	pouseBack.CreateSprite(L"Resource/Img/title_back_80.png", XMFLOAT3(0, 0, 0));
	pouseBackEach.CreateConstBuff0();
	pouseBackEach.CreateConstBuff1();
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
	reverseObject.CreateModel("reverse", ShaderManager::playerShader);

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
	countDownInfo.CreateConstBuff0();
	countDownInfo.CreateConstBuff1();
	isChanged = false;
	timerCount = 0;
	size_x = MAX_SIZE_X;
	size_y = MAX_SIZE_Y;
	for (int i = 0; i <= 9; i++)
	{
		string name = "number" + to_string(i) + "s";
		countDownNumber[i].CreateModel(name.c_str(), ShaderManager::othelloShader);
	}
	for (int i = 0; i <= 9; i++)
	{
		string name = "number" + to_string(i) + "y";
		timerNumber[i].CreateModel(name.c_str(), ShaderManager::playerShader);
	}
	for (int i = 0; i <= 9; i++)
	{
		string name = "number" + to_string(i);
		numbersObject[i].CreateModel(name.c_str(), ShaderManager::playerShader);
	}
	for (int i = 0; i <= 9; i++)
	{
		string name = "number" + to_string(i) + "s";
		sNumbersObject[i].CreateModel(name.c_str(), ShaderManager::playerShader);
	}

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
	isDrawLogoEnd = false;

	//tips用
	pushButtonGreen.CreateSprite(L"Resource/Img/pushButton/push_button_green.png", XMFLOAT3(0, 0, 0));
	pushButtonBlack[0].CreateSprite(L"Resource/Img/pushButton/push_button_black.png", XMFLOAT3(0, 0, 0));
	pushButtonBlack[1].CreateSprite(L"Resource/Img/pushButton/push_button_black.png", XMFLOAT3(0, 0, 0));
	pushButtonBlack[2].CreateSprite(L"Resource/Img/pushButton/push_button_black.png", XMFLOAT3(0, 0, 0));

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
	tips_name[0].CreateSprite(L"Resource/Img/tips/othello_move.png", XMFLOAT3(NAME_START_X[0], NAME_START_Y, 0));
	tips_name[1].CreateSprite(L"Resource/Img/tips/reverse.png", XMFLOAT3(NAME_START_X[1], NAME_START_Y, 0));
	tips_name[2].CreateSprite(L"Resource/Img/tips/reach.png", XMFLOAT3(NAME_START_X[2], NAME_START_Y, 0));
	tips_name[3].CreateSprite(L"Resource/Img/tips/wall.png", XMFLOAT3(NAME_START_X[3], NAME_START_Y, 0));
	tips_name[4].CreateSprite(L"Resource/Img/tips/connect.png", XMFLOAT3(NAME_START_X[4], NAME_START_Y, 0));
	tips_name[5].CreateSprite(L"Resource/Img/tips/bookend.png", XMFLOAT3(NAME_START_X[5], NAME_START_Y, 0));
	tips_name[6].CreateSprite(L"Resource/Img/tips/waltz.png", XMFLOAT3(NAME_START_X[6], NAME_START_Y, 0));
	tips_name[7].CreateSprite(L"Resource/Img/tips/reversible.png", XMFLOAT3(NAME_START_X[7], NAME_START_Y, 0));

	tips_system[0].CreateSprite(L"Resource/Img/tips/system.png", XMFLOAT3(SYSTEM_START_X, SYSTEM_START_Y, 0));
	tips_system[1].CreateSprite(L"Resource/Img/tips/technic.png", XMFLOAT3(SYSTEM_START_X, SYSTEM_START_Y, 0));

	tips_bar.CreateSprite(L"Resource/Img/tips/tips_bar.png", XMFLOAT3(BAR_START_X, BAR_START_Y, 0));

	tips_text[0].CreateSprite(L"Resource/Img/tips/othello_move_text.png", XMFLOAT3(TEXT_START_X[0] + window_width, TEXT_START_Y, 0));
	tips_text[1].CreateSprite(L"Resource/Img/tips/reverse_text.png", XMFLOAT3(TEXT_START_X[1] + window_width, TEXT_START_Y, 0));
	tips_text[2].CreateSprite(L"Resource/Img/tips/reach_text.png", XMFLOAT3(TEXT_START_X[2] + window_width, TEXT_START_Y, 0));
	tips_text[3].CreateSprite(L"Resource/Img/tips/wall_text.png", XMFLOAT3(TEXT_START_X[3] + window_width, TEXT_START_Y, 0));
	tips_text[4].CreateSprite(L"Resource/Img/tips/connect_text.png", XMFLOAT3(TEXT_START_X[4] + window_width, TEXT_START_Y, 0));
	tips_text[5].CreateSprite(L"Resource/Img/tips/bookend_text.png", XMFLOAT3(TEXT_START_X[5] + window_width, TEXT_START_Y, 0));
	tips_text[6].CreateSprite(L"Resource/Img/tips/waltz_text.png", XMFLOAT3(TEXT_START_X[6] + window_width, TEXT_START_Y, 0));
	tips_text[7].CreateSprite(L"Resource/Img/tips/reversible_text.png", XMFLOAT3(TEXT_START_X[7] + window_width, TEXT_START_Y, 0));
	isInit = false;
	newTips_easeTimer = 0;
	/*----------新tips用----------*/
	tips_cont.CreateSprite(L"Resource/Img/tips/tips_1.png", XMFLOAT3(0, 0, 0));

	tips_ss[0].CreateSprite(L"Resource/Img/tips/in_tips_0.png", XMFLOAT3(START_X, START_Y, 0));
	tips_ss[1].CreateSprite(L"Resource/Img/tips/in_tips_1.png", XMFLOAT3(START_X - SIZE_X, START_Y, 0));
	tips_ss[2].CreateSprite(L"Resource/Img/tips/in_tips_2.png", XMFLOAT3(START_X - SIZE_X, START_Y, 0));

	for (int i = 0; i < 2; i++)
	{
		resultMaxConbo[i].CreateConstBuff0();
		resultMaxConbo[i].CreateConstBuff1();
	}
	for (int i = 0; i < 3; i++)
	{
		resultEraseOthello[i].CreateConstBuff0();
		resultEraseOthello[i].CreateConstBuff1();
	}
	resultScoreKanaModel.CreateModel("score_kana", ShaderManager::playerShader);
	resultMaxConboModel.CreateModel("max_combo", ShaderManager::playerShader);
	resultEraseOthelloModel.CreateModel("delete_block", ShaderManager::playerShader);
	SupportModel.CreateModel("assist", ShaderManager::playerShader);
	BackModel.CreateModel("back", ShaderManager::playerShader);
	TiTleModel.CreateModel("title_text", ShaderManager::playerShader);
	EndModel.CreateModel("end", ShaderManager::playerShader);
	ChackBoxModel.CreateModel("check_box", ShaderManager::playerShader);
	CheckModel.CreateModel("check", ShaderManager::playerShader);
	for (int i = 0; i < 2; i++)
	{
		pouseKakko[i].CreateConstBuff0();
		pouseKakko[i].CreateConstBuff1();
	}
	pouseMinSizeConst = { 0.2f, 0.2f, 0.2f };
	pouseMaxSizeConst = { 0.23f, 0.23f, 0.23f };
	pouseNormalSizeConst = { 0.4f, 0.4f, 0.4f };
	pouseSelectTime1 = 0.0f;
	tipsCounts = 0;
	changeTimerFrame = 0;
	tipsDrawNum = 0;
	tips_easeTimer = 0;
	//green_scale = 0.09f;
	isTipsDrawTrigger = false;
	moveTips = false;
}

void GameScene::TitleUpdate()
{
	static int BGMSoundCount = 0;
	BGMSoundCount++;
	if (BGMSoundCount > 51 * 60)
	{
		BGMSoundCount = 0;
		SoundStopWave(BGMSound);
		SoundPlayOnce(BGMSound);
	}
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
		else { isDrawLogoEnd = true; }
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
				//チュートリアルに飛ぶ
				if ((Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01)) && !isPouse && !isSceneChange && !isPouseToTiTle && isDrawLogoEnd)
				{
					//isGameEnd = true;
					titleSelectNum = 0;
					gameTime = 1;
					isTipsDraw = false;
					ToModeSelect();
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
		if (!isPouse && !isSceneChange && !isResultSceneChange && isDrawLogoEnd && (Input::KeyTrigger(DIK_ESCAPE) || directInput->IsButtonPush(directInput->ButtonPouse)))
		{
			isGameEnd = true;
			/*isPouse = true;
			selectPouse = 0;*/
		}
		//ポーズ中の処理
		else if (isPouse)
		{
			bool W = directInput->IsButtonPush(directInput->UpButton) && directInput->leftStickX() == 0 && directInput->leftStickY() == 0;
			bool S = directInput->IsButtonPush(directInput->DownButton) && directInput->leftStickX() == 0 && directInput->leftStickY() == 0;
			if (Input::KeyTrigger(DIK_W) || W)
			{
				SoundStopWave(selectSound);
				SoundPlayOnce(selectSound);
				if (selectPouse == -1)
				{
					selectPouse = selectMaxPouse;
				}
				else
				{
					selectPouse--;
				}
			}
			else if (Input::KeyTrigger(DIK_S) || S)
			{
				SoundStopWave(selectSound);
				SoundPlayOnce(selectSound);
				if (selectPouse == selectMaxPouse)
				{
					selectPouse = -1;
				}
				else
				{
					selectPouse++;
				}
			}
			if ((Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01)))
			{
				//リザルトに戻る
				if (selectPouse == -1)
				{
					if (isSupport)
					{
						isSupport = false;
					}
					else
					{
						isSupport = true;
					}
				}
				else if (selectPouse == 0)
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
		//if (!isGameEnd)
		if (!isPouse)
		{
			ObjectParticles::Update(othelloManager.GetPressPanellPos(), checkObject.GetCombo());
			ObjectParticles::Update(othelloManager.GetPressPanellPos(), checkObject.GetNameAndCombos());
		}
		else
		{

		}
		light->SetLightDir(XMFLOAT3(Camera::GetTargetDirection()));
		LightUpdate();
	}
	//ゲーム中ポーズ画面からタイトルへ戻す
	if (isPouseToTiTle && sceneChageType != 30 && sceneChageType != 31)
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
	//BGMを無理やり流す--------------------------
	static int BGMSoundCount = 0;
	BGMSoundCount++;
	if (BGMSoundCount > 51 * 60)
	{
		BGMSoundCount = 0;
		SoundStopWave(BGMSound);
		SoundPlayOnce(BGMSound);
	}
	//-------------------------------------------
	light->SetLightDir(XMFLOAT3(Camera::GetTargetDirection()));
	LightUpdate();
	//カメラ変化が行われていない時にゲームを開始する
	if (!isSceneChange && !isResultSceneChange && !isPouse)
	{
		//選択したモードへ移行する
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
			{
				othelloManager.AllDeletePanel();
				gameTime = gameMaxTime;
				isTutorial = false;
				selectMode = true;
				ToGame4(true);
				titleSelectNum = 0;
				selectStageNum = othelloManager.GetEnterNormaStage();
			}
			if (othelloManager.InMode() && othelloManager.GetEnterModeType() == GameMode::Tutorial)
			{
				//Initとシーン遷移を入れる
				othelloManager.AllDeletePanel();
				gameTime = gameMaxTime;
				ToGame4(true, true);
				titleSelectNum = 0;
			}
			if (othelloManager.InMode() && othelloManager.GetEnterModeType() == GameMode::Dojo)
			{
				//Initとシーン遷移を入れる
				othelloManager.AllDeletePanel();
				gameTime = gameMaxTime;
				ToGame4(true, false, true);
				titleSelectNum = 0;
			}
#pragma endregion
		}
		//ゲーム中アップデート
		else if (countDown <= 0)
		{
			//BGM流す---------------------------
			static int BGMSoundCount = 0;
			BGMSoundCount++;
			if (BGMSoundCount > 51 * 60)
			{
				BGMSoundCount = 0;
				SoundStopWave(BGMSound);
				SoundPlayOnce(BGMSound);
			}
			//-----------------------------------
			OthlloPlayer::Update();
			othelloManager.Controll();
			if (isTutorial && !isToDojo)
			{
				//チュートリアルのアップデート
				othelloManager.TutorialUpdate(checkObject.GetCombo());
			}
			else
			{
				//ここでアップデートしてみてほしい
				if (isToDojo)
				{
					int hoge = 0;
				}
				else if (!selectMode)
				{
					//ゲーム中のアップデート？
					othelloManager.Update(checkObject.GetCombo());
				}
				else if (selectMode)
				{
					//ノルマモードのアップデート？
					othelloManager.SetScore(checkObject.GetScore());
					othelloManager.NormaUpdate(checkObject.GetCombo());
				}
				//失敗したとき(過去の遺産)
				if (othelloManager.GetIsNormaFailed())
				{
					isSceneChange = true;
				}
				//クリアしたときの処理
				else if (othelloManager.GetIsNormaClear())
				{
					static int checkTime = 0;
					checkTime++;
					if (checkTime > 60)
					{
						checkTime = 0;
						NormaToModeSelect();
					}
				}
			}
			//判定系の更新
			checkObject.Update(othelloManager.Send(), true);
			//ひっくり返る更新
			othelloManager.Receive(checkObject.GetOthelloDatas(), checkObject.GetCompletePos());
			//リーチ目の更新
			othelloManager.SpawnChances(checkObject.GetReachDatas());
			//ゲーム中のカウントダウン減少
			if (!isTutorial && !selectMode && !isToDojo)
			{
				gameTime--;
			}
			else if (isToDojo)
			{
				//終わった時の処理入れたいかな
			}
			//チュートリアル終わった時の処理
			else if (othelloManager.IsTutorialEnd() && isTutorial)
			{
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
	//スタートの音を鳴らす
	if (countDown == 58)
	{
		SoundStopWave(countdDownSound);
		SoundStopWave(startSound);
		SoundPlayOnce(startSound);
	}
	if (countDown == 0)
	{
		SoundPlayOnce(BGMSound);
	}
	ObjectParticles::Update(othelloManager.GetPressPanellPos(), checkObject.GetCombo());
	ObjectParticles::Update(othelloManager.GetPressPanellPos(), checkObject.GetNameAndCombos());
	//タイトルからgameシーンへ-----基本いじらなくていいかも
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
		else if (sceneChageType == 30)
		{
			NormaToModeSelectUpdate();
		}
		else if (sceneChageType == 31)
		{
			NormaToModeSelectUpdate2();
		}
	}
	//ゲームシーンからリザルトへのトリガー---ゲーム終了時
	if (gameTime <= 0 && !isResultSceneChange)
	{
		ToResult();
	}
	//カメラの動き-----基本いじらなくていいかも
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
				if (!isPouseToTitle && !isTutorial) SceneNum = RESULT;
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
	//ポーズへ行く
	if (!isPouse && !isSceneChange && (Input::KeyTrigger(DIK_ESCAPE) || directInput->IsButtonPush(directInput->ButtonPouse)) && (countDown <= 0 || isModeSelect) && !isTipsDraw)
	{
		isPouse = true;
		selectPouse = 0;
	}
	//ポーズ中の処理
	else if (isPouse)
	{
		bool W = directInput->IsButtonPush(directInput->UpButton) && directInput->leftStickX() == 0 && directInput->leftStickY() == 0;
		bool S = directInput->IsButtonPush(directInput->DownButton) && directInput->leftStickX() == 0 && directInput->leftStickY() == 0;
		if (Input::KeyTrigger(DIK_W) || W)
		{
			SoundStopWave(selectSound);
			SoundPlayOnce(selectSound);
			if (selectPouse == -1)
			{
				selectPouse = selectMaxPouse;
			}
			else
			{
				selectPouse--;
			}
		}
		else if (Input::KeyTrigger(DIK_S) || S)
		{
			SoundStopWave(selectSound);
			SoundPlayOnce(selectSound);
			if (selectPouse == selectMaxPouse)
			{
				selectPouse = -1;
			}
			else
			{
				selectPouse++;
			}
		}
		if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
		{
			//リザルトに戻る
			if (selectPouse == -1)
			{
				if (isSupport)
				{
					isSupport = false;
				}
				else
				{
					isSupport = true;
				}
			}
			else if (selectPouse == 0)
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
				checkObject.Init();
			}
		}
		if (Input::KeyTrigger(DIK_ESCAPE) || directInput->IsButtonPush(directInput->ButtonPouse))
		{
			isPouse = false;
		}
	}
	//竜巻出す
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

	sky.Update();
	othelloStage.Update();
	Lights::Add(checkObject);
	Lights::Update();
	ParticleControl::Update();
	//ゲーム中ポーズ画面からタイトルへ戻す
	if (isPouseToTiTle && sceneChageType != 30 && sceneChageType != 31)
	{
		PouseToTitleUpdate();
	}
	//tips用
	if (isTipsDraw)
	{
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
		//XMFLOAT3 text_Goal = { TEXT_X,TEXT_Y,0 };
		XMFLOAT3 text_Goal = { TEXT_X[tipsCounts],TEXT_Y,0 };

		if (newTips_easeTimer < 1.0f) { newTips_easeTimer += EASE_TIPS_ADD_TIMER; }
		if (newTips_easeTimer > 1.0f) { newTips_easeTimer = 1.0f; }

		float tips_Timers[3];
		tips_Timers[0] = newTips_easeTimer - EASE_DELAY;
		tips_Timers[1] = newTips_easeTimer - EASE_DELAY * 2;
		tips_Timers[2] = newTips_easeTimer - EASE_DELAY * 3;

		name = ShlomonMath::EaseInQuad(name, name_Goal, newTips_easeTimer);
		if (tips_Timers[0] >= 0.0f) { system = ShlomonMath::EaseInQuad(system, system_Goal, tips_Timers[0]); }
		if (tips_Timers[1] >= 0.0f) { bar = ShlomonMath::EaseInQuad(bar, bar_Goal, tips_Timers[1]); }
		if (tips_Timers[2] >= 0.0f) { text = ShlomonMath::EaseInQuad(text, text_Goal, tips_Timers[2]); }
		/*if (text.x - 1 < text_Goal.x - 1)
		{
			isTextMove = true;
		}*/

		tips_name[tipsCounts].position = ConvertXMFLOAT3toXMVECTOR(name);
		if (tipsCounts < 5) { tips_system[0].position = ConvertXMFLOAT3toXMVECTOR(system); }
		else { tips_system[1].position = ConvertXMFLOAT3toXMVECTOR(system); }
		tips_bar.position = ConvertXMFLOAT3toXMVECTOR(bar);
		tips_text[tipsCounts].position = ConvertXMFLOAT3toXMVECTOR(text);

		//中身の移動
		if (!moveTips)
		{
			if (isFirst) { changeTimerFrame -= FIRST_SS_DELAY; isFirst = false; }
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

			if (tips_easeTimer < 1.0f) { tips_easeTimer += EASE_ADD_TIMER; }
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
	if (!isTipsDraw)
	{
		if (isInit)
		{
			moveTips = false;
			isFirst = true;
			changeTimerFrame = 0;
			newTips_easeTimer = 0.0f;
			tips_name[tipsCounts].position = { NAME_START_X[tipsCounts],NAME_START_Y,0,0 };
			if (tipsCounts < 5) { tips_system[0].position = { SYSTEM_START_X,SYSTEM_START_Y,0,0 }; }
			else { tips_system[1].position = { SYSTEM_START_X,SYSTEM_START_Y,0,0 }; }
			tips_bar.position = { BAR_START_X,BAR_START_Y,0,0 };
			//tips_text[tipsCounts].position = { TEXT_START_X,TEXT_START_Y,0,0 };
			tips_text[tipsCounts].position = { TEXT_START_X[tipsCounts] + window_width,TEXT_START_Y,0,0 };

			tipsDrawNum = 0;
			tips_ss[0].position = { START_X,START_Y,0,0 };
			tips_ss[1].position = { START_X - SIZE_X,START_Y,0,0 };
			tips_ss[2].position = { START_X - SIZE_X,START_Y,0,0 };
			isInit = false;
		}
	}
}

void GameScene::ResultUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
	{
		SoundStopWave(enterSound);
		SoundPlayOnce(enterSound);
		OthlloPlayer::SetPosition(XMFLOAT3(0, 0, -2));
		SoundPlayLoop(BGMSound);
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

	if (!isDrawLogoEnd)
	{
		titleLogo[0].SpriteDraw();
		titleLogo[1].SpriteDraw();
		titleLogo[2].SpriteDraw();
		titleLogo[3].SpriteDraw();
	}
	/*-----この中で処理書いてください-----*/
	if (isDrawLogo)
	{
		sky.each.rotation.x = 0.0f;
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
			startModel.each.position = { 0.0f /*+ 6.0f*/, -2.0f, 0, 1.0f };
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
		}
		if (!isPouseToTiTle)
		{
			if (titleSelectNum == 0)
			{
				kakko[1].each.position = { -1 + 5.5f, -2.0f, 0, 1.0f };
				kakko[1].each.rotation.x += 1.0f;
				kakko[1].each.scale = { 1.0f, 1.0f , 1.0f };
				kakko[1].Update();
				Draw3DObject(kakko[1]);
				kakko[0].each.position = { -1 - 3.5f, -2.0f, 0, 1.0f };
				kakko[0].each.rotation.x += 1.0f;
				kakko[0].each.scale = { 1.0f, 1.0f , 1.0f };
				kakko[0].Update();
				Draw3DObject(kakko[0]);
			}
		}
	}
	if (isPouse)
	{
		pouseBackEach.position = { 0, 0, -1.0f, 1.0f };
		pouseBackEach.scale = { 100.0f, 100.f ,1.0f };
		backGround.Update(&pouseBackEach);
		Draw3DObject(backGround);
		if (pouseSelectTime1 < 1.0f)
		{
			pouseSelectTime1 += 0.1f;
			pouseNowSize = Lerp(pouseMinSizeConst, pouseMaxSizeConst, pouseSelectTime1);
		}
		else if (pouseSelectTime2 < 1.0f)
		{
			pouseSelectTime2 += 0.1f;
			pouseNowSize = Lerp(pouseMaxSizeConst, pouseMinSizeConst, pouseSelectTime2);
		}
		else
		{
			pouseSelectTime1 = 0.0f;
			pouseSelectTime2 = 0.0f;
		}
		if (selectPouse == 0)
		{
			SupportModel.each.scale = pouseMinSizeConst;
			BackModel.each.scale = pouseNowSize;
			TiTleModel.each.scale = pouseMinSizeConst;
			EndModel.each.scale = pouseMinSizeConst;
			ChackBoxModel.each.scale = pouseMinSizeConst;

			SupportModel.each.rotation = { 0, 0, 0 };
			BackModel.each.rotation = { 0, 0, 0 };
			TiTleModel.each.rotation = { 0, 0, 0 };
			EndModel.each.rotation = { 0, 0, 0 };
			ChackBoxModel.each.rotation = { 0, 0, 0 };

			pouseKakko[0].position = { -1.0f, 1.0f, -3.0f, 1.0f };
			pouseKakko[1].position = { 3.0f, 1.0f, -3.0f, 1.0f };
			pouseKakko[0].scale = { 0.35f, 0.35f, 0.35f };
			pouseKakko[1].scale = { 0.35f, 0.35f, 0.35f };
		}
		else if (selectPouse == 1)
		{
			SupportModel.each.scale = pouseMinSizeConst;
			BackModel.each.scale = pouseMinSizeConst;
			TiTleModel.each.scale = pouseNowSize;
			EndModel.each.scale = pouseMinSizeConst;
			ChackBoxModel.each.scale = pouseMinSizeConst;

			SupportModel.each.rotation = { 0, 0, 0 };
			BackModel.each.rotation = { 0, 0, 0 };
			TiTleModel.each.rotation = { 0, 0, 0 };
			EndModel.each.rotation = { 0, 0, 0 };
			ChackBoxModel.each.rotation = { 0, 0, 0 };

			pouseKakko[0].position = { -1.0f, -1.0f, -3.0f, 1.0f };
			pouseKakko[1].position = { 3.0f, -1.0f, -3.0f, 1.0f };
			pouseKakko[0].scale = { 0.35f, 0.35f, 0.35f };
			pouseKakko[1].scale = { 0.35f, 0.35f, 0.35f };
		}
		else if (selectPouse == 2)
		{
			SupportModel.each.scale = pouseMinSizeConst;
			BackModel.each.scale = pouseMinSizeConst;
			TiTleModel.each.scale = pouseMinSizeConst;
			EndModel.each.scale = pouseNowSize;
			ChackBoxModel.each.scale = pouseMinSizeConst;

			SupportModel.each.rotation = { 0, 0, 0 };
			BackModel.each.rotation = { 0, 0, 0 };
			TiTleModel.each.rotation = { 0, 0, 0 };
			EndModel.each.rotation = { 0, 0, 0 };
			ChackBoxModel.each.rotation = { 0, 0, 0 };

			pouseKakko[0].position = { -1.0f, -3.0f, -3.0f, 1.0f };
			pouseKakko[1].position = { 3.0f, -3.0f, -3.0f, 1.0f };
			pouseKakko[0].scale = { 0.35f, 0.35f, 0.35f };
			pouseKakko[1].scale = { 0.35f, 0.35f, 0.35f };
		}
		else if (selectPouse == -1)
		{
			SupportModel.each.scale = pouseNowSize;
			BackModel.each.scale = pouseMinSizeConst;
			TiTleModel.each.scale = pouseMinSizeConst;
			EndModel.each.scale = pouseMinSizeConst;
			ChackBoxModel.each.scale = pouseMinSizeConst;

			SupportModel.each.rotation = { 0, 0, 0 };
			BackModel.each.rotation = { 0, 0, 0 };
			TiTleModel.each.rotation = { 0, 0, 0 };
			EndModel.each.rotation = { 0, 0, 0 };
			ChackBoxModel.each.rotation = { 0, 0, 0 };

			pouseKakko[0].position = { -1.0f, 3.0f, -3.0f, 1.0f };
			pouseKakko[1].position = { 7.0f, 3.0f, -3.0f, 1.0f };
			pouseKakko[0].scale = { 0.35f, 0.35f, 0.35f };
			pouseKakko[1].scale = { 0.35f, 0.35f, 0.35f };
		}
		if (isSupport)
		{
			CheckModel.each.position = { -2.8f, 3.6f, -3.0f, 1.0f };
			CheckModel.each.scale = { 0.2f, 0.2f, 0.2f };
			CheckModel.Update();
			Draw3DObject(CheckModel);
		}
		kakko[0].Update(&pouseKakko[0]);
		kakko[1].Update(&pouseKakko[1]);
		Draw3DObject(kakko[0]);
		Draw3DObject(kakko[1]);
		ChackBoxModel.each.scale = ChackBoxModel.each.scale - 0.1f;
		SupportModel.each.position = { 3.0f, 3.0f, -3.0f, 1.0f };
		SupportModel.Update();
		Draw3DObject(SupportModel);
		BackModel.each.position = { 1.0f, 1.0f, -3.0f, 1.0f };
		BackModel.Update();
		Draw3DObject(BackModel);
		TiTleModel.each.position = { 1.0f, -1.0f, -3.0f, 1.0f };
		TiTleModel.Update();
		Draw3DObject(TiTleModel);
		EndModel.each.position = { 1.0f, -3.0f, -3.0f, 1.0f };
		EndModel.Update();
		Draw3DObject(EndModel);
		ChackBoxModel.each.position = { -3.0f, 3.0f, -3.0f, 1.0f };
		ChackBoxModel.Update();
		Draw3DObject(ChackBoxModel);
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
	sky.each.rotation.y += 1.0f;
	if (isSceneChange)
	{
		sky.each.rotation.x = 0.0f;
	}
	else
	{
		sky.each.rotation.x = 80.0f;
	}
	Draw3DObject(sky);
	othelloStage.each.position.m128_f32[2] = 8.0f;
	othelloStage.each.rotation.x = -90;
	if (isStageDisplay)Draw3DObject(othelloStage);
	if ((isSceneChange == true && sceneChageType >= 3 && sceneChageType != 31) || isBackGroundOthello)
	{
		for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
		{
			ObjectParticles::othello2.object.Update(&(*opOthelloItr));
			Draw3DObject(ObjectParticles::othello2.object);
		}
	}
	if (isPouseToTiTle /*&& sceneChageType != 31*/)
	{
		for (auto opOthelloItr = pouseOthellos.begin(); opOthelloItr != pouseOthellos.end(); ++opOthelloItr)
		{
			ObjectParticles::othello2.object.Update(&(*opOthelloItr));
			Draw3DObject(ObjectParticles::othello2.object);
		}
		for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
		{
			ObjectParticles::othello2.object.Update(&(*opOthelloItr));
			Draw3DObject(ObjectParticles::othello2.object);
		}
	}
	if (isSceneChange == true && (sceneChageType == 31 || sceneChageType == 30))
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

			int ichinokurai = gameTime / 60 % 10;
			timerEach[1].position.m128_f32[0] = 0.0f;
			timerEach[1].position.m128_f32[1] = 18.0f;
			timerEach[1].position.m128_f32[2] = -1.0f;
			timerEach[1].scale = { size_x, size_x, size_x };
			timerEach[1].rotation.x = -30.0f;

			if (gameTime / 60 > CHANGE_COLOR_COUNT)
			{
				sNumbersObject[zyunokurai].Update(&timerEach[0]);
				Draw3DObject(sNumbersObject[zyunokurai]);
				sNumbersObject[ichinokurai].Update(&timerEach[1]);
				Draw3DObject(sNumbersObject[ichinokurai]);
			}
			else
			{
				timerNumber[zyunokurai].Update(&timerEach[0]);
				Draw3DObject(timerNumber[zyunokurai]);
				timerNumber[ichinokurai].Update(&timerEach[1]);
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

		if (checkObject.IsAddScore())
		{
			addScore.Init(0, checkObject.GetAddScore());
		}
		addScore.Update();
		GetScoreDraw();
	}
	if (!isSceneChange && !isPouseToTiTle)
	{
		OthlloPlayer::Draw();
	}
	ObjectParticles::Draw();
	ParticleControl::Draw();
	if (!isPouseToTiTle)othelloManager.Draw(isSupport, isSupport);
	if (countDown > 0 && isSceneChange == false && isModeSelect == false)
	{
		XMFLOAT3 scale = { 0.7f, 0.7f, 0.7f };
		if (countDown < 60)
		{
			if (countDown == 59)
			{
				startModel.each.position = { -1, 5.0f, -2.0f, 1.0f, };
				startModel.each.scale = scale;
				startModel.each.rotation.x = -30.0f;
				isSetRand = false;
			}
			else
			{
				const float max_shake = 5.0f;
				if (countDown % 1 == 0)
				{
					if (countDown % 2 == 0) { startModel.each.rotation.z = (max_shake) * ((float)countDown / 60.0f); }
					else { startModel.each.rotation.z = (-max_shake) * ((float)countDown / 60.0f); }
				}
				/*else if (countDown % 10 == 0)
				{
					startModel.each.rotation.z = -max_shake * ((float)countDown / 60.0f);
				}*/
				startModel.each.position.m128_f32[1] -= 0.48f;
				startModel.each.position.m128_f32[2] -= 0.35f;
			}
			startModel.Update();
			Draw3DObject(startModel);
		}
		else
		{
			/*scoreEach[0].position = { -1, 5.0f, -2.0f, 1.0f };
			scoreEach[0].scale = scale;
			scoreEach[0].rotation.x = -30.0f;
			sNumbersObject[countDown / 60].Update(&scoreEach[0]);
			Draw3DObject(sNumbersObject[countDown / 60]);*/

			countDownInfo.position.m128_f32[0] = 0.0f;
			countDownInfo.position.m128_f32[1] = 0.0f;
			countDownInfo.position.m128_f32[2] = -2.0f;
			countDownInfo.scale = { 1.7f, 2.5f, 1.7f };
			countDownInfo.rotation.x = -30.0f;
			int local_countDown = countDown / 60;
			float floatCountDown = countDown / 60.0f - local_countDown;
			if (floatCountDown < 0.02f) { countDownInfo.alpha = 0.2f; }
			else if (floatCountDown < 0.5f) { countDownInfo.alpha -= 0.01f; }
			else { countDownInfo.alpha = 0.5f; }
			if (local_countDown % 2 == 0)
			{
				int random = rand();
				if (!isSetRand)
				{
					countDownInfo.rotation.y = random % 8;
				}
				isSetRand = true;
				countDownInfo.rotation.z = -6.0f;
			}
			else
			{
				int random = rand();
				if (!isSetRand)
				{
					countDownInfo.rotation.y = random % 8 - 8;
				}
				isSetRand = true;
				countDownInfo.rotation.z = 6.0f;
			}
			countDownNumber[local_countDown].Update(&countDownInfo);
			Draw3DObject(countDownNumber[local_countDown]);
		}
	}

	if (gameTime > 0 && countDown <= 0 && !isTutorial)
	{
		if (gameTime / 60 < DRAW_COUNTDOWN_COUNT)
		{
			countDownInfo.position.m128_f32[0] = 0.0f;
			countDownInfo.position.m128_f32[1] = 0.0f;
			countDownInfo.position.m128_f32[2] = -2.0f;
			countDownInfo.scale = { 1.7f, 2.5f, 1.7f };
			countDownInfo.rotation.x = -30.0f;
			int countDown = gameTime / 60;
			float floatCountDown = gameTime / 60.0f - countDown;
			if (floatCountDown < 0.02f) { countDownInfo.alpha = 0.2f; }
			else if (floatCountDown < 0.5f) { countDownInfo.alpha -= 0.01f; }
			else { countDownInfo.alpha = 0.5f; }
			if (countDown % 2 == 0)
			{
				int random = rand();
				if (!isSetRand)
				{
					countDownInfo.rotation.y = random % 8;
				}
				isSetRand = true;
				countDownInfo.rotation.z = -6.0f;
			}
			else
			{
				int random = rand();
				if (!isSetRand)
				{
					countDownInfo.rotation.y = random % 8 - 8;
				}
				isSetRand = true;
				countDownInfo.rotation.z = 6.0f;
			}
			countDownNumber[countDown].Update(&countDownInfo);
			Draw3DObject(countDownNumber[countDown]);
		}
	}
	else { if (countDown < 59) { isSetRand = false; } }

	if (selectMode && !isSceneChange && !isPouseToTiTle)
	{
		othelloManager.NormaTextModelDraw(selectStageNum, true);
	}
	if (isModeSelect && !isSceneChange && !isPouseToTiTle)
	{
		othelloManager.ModeSelectModelDraw(true);
	}
	if (gameTime > 0 && countDown <= 0 && isTutorial)
	{
		othelloManager.TutorialTextDraw();
	}
	if (isToDojo)
	{
		//ここでdojoのdraw

	}
	if (isPouse)
	{
		pouseBackEach.position = { 0, 0, -1.0f, 1.0f };
		pouseBackEach.scale = { 100.0f, 100.f ,1.0f };
		backGround.Update(&pouseBackEach);
		Draw3DObject(backGround);
		if (pouseSelectTime1 < 1.0f)
		{
			pouseSelectTime1 += 0.1f;
			pouseNowSize = Lerp(pouseMinSizeConst, pouseMaxSizeConst, pouseSelectTime1);
		}
		else if (pouseSelectTime2 < 1.0f)
		{
			pouseSelectTime2 += 0.1f;
			pouseNowSize = Lerp(pouseMaxSizeConst, pouseMinSizeConst, pouseSelectTime2);
		}
		else
		{
			pouseSelectTime1 = 0.0f;
			pouseSelectTime2 = 0.0f;
		}
		if (selectPouse == 0)
		{
			SupportModel.each.scale = pouseMinSizeConst;
			SupportModel.each.rotation.x = -35;
			BackModel.each.scale = pouseNowSize;
			BackModel.each.rotation.x = -35;
			TiTleModel.each.scale = pouseMinSizeConst;
			TiTleModel.each.rotation.x = -35;
			EndModel.each.scale = pouseMinSizeConst;
			EndModel.each.rotation.x = -35;
			ChackBoxModel.each.scale = pouseMinSizeConst;
			ChackBoxModel.each.rotation.x = -35;
			pouseKakko[0].position = { -1.1f, -3.0f, -5.0f, 1.0f };
			pouseKakko[1].position = { 3.1f, -3.0f, -5.0f, 1.0f };
			pouseKakko[0].scale = { 0.35f, 0.35f, 0.35f };
			pouseKakko[1].scale = { 0.35f, 0.35f, 0.35f };
			pouseKakko[0].rotation.x = -35;
			pouseKakko[1].rotation.x = -35;
		}
		else if (selectPouse == 1)
		{
			SupportModel.each.scale = pouseMinSizeConst;
			SupportModel.each.rotation.x = -35;
			BackModel.each.scale = pouseMinSizeConst;
			BackModel.each.rotation.x = -35;
			TiTleModel.each.scale = pouseNowSize;
			TiTleModel.each.rotation.x = -35;
			EndModel.each.scale = pouseMinSizeConst;
			EndModel.each.rotation.x = -35;
			ChackBoxModel.each.scale = pouseMinSizeConst;
			ChackBoxModel.each.rotation.x = -35;
			pouseKakko[0].position = { -1.0f,  -4.6f, -5.0f, 1.0f };
			pouseKakko[1].position = { 3.0f,  -4.6f, -5.0f, 1.0f };
			pouseKakko[0].scale = { 0.35f, 0.35f, 0.35f };
			pouseKakko[1].scale = { 0.35f, 0.35f, 0.35f };
		}
		else if (selectPouse == 2)
		{
			SupportModel.each.scale = pouseMinSizeConst;
			SupportModel.each.rotation.x = -35;
			BackModel.each.scale = pouseMinSizeConst;
			BackModel.each.rotation.x = -35;
			TiTleModel.each.scale = pouseMinSizeConst;
			TiTleModel.each.rotation.x = -35;
			EndModel.each.scale = pouseNowSize;
			EndModel.each.rotation.x = -35;
			ChackBoxModel.each.scale = pouseMinSizeConst;
			ChackBoxModel.each.rotation.x = -35;
			pouseKakko[0].position = { -1.0f, -6.2f, -5.0f, 1.0f };
			pouseKakko[1].position = { 2.8f, -6.2f, -5.0f, 1.0f };
			pouseKakko[0].scale = { 0.35f, 0.35f, 0.35f };
			pouseKakko[1].scale = { 0.35f, 0.35f, 0.35f };
		}
		else if (selectPouse == -1)
		{
			SupportModel.each.scale = pouseNowSize;
			SupportModel.each.rotation.x = -35;
			BackModel.each.scale = pouseMinSizeConst;
			BackModel.each.rotation.x = -35;
			TiTleModel.each.scale = pouseMinSizeConst;
			TiTleModel.each.rotation.x = -35;
			EndModel.each.scale = pouseMinSizeConst;
			EndModel.each.rotation.x = -35;
			ChackBoxModel.each.scale = pouseMinSizeConst;
			ChackBoxModel.each.rotation.x = -35;
			pouseKakko[0].position = { -1.0f, -1.0f, -5.0f, 1.0f };
			pouseKakko[1].position = { 7.0f, -1.0f, -5.0f, 1.0f };
			pouseKakko[0].scale = { 0.35f, 0.35f, 0.35f };
			pouseKakko[1].scale = { 0.35f, 0.35f, 0.35f };
		}
		if (isSupport)
		{
			CheckModel.each.position = { -2.7f, -0.5f, -5.0f, 1.0f };
			CheckModel.each.scale = { 0.2f, 0.2f, 0.2f };
			CheckModel.each.rotation.x = -35;
			CheckModel.Update();
			Draw3DObject(CheckModel);
		}
		kakko[0].Update(&pouseKakko[0]);
		kakko[1].Update(&pouseKakko[1]);
		Draw3DObject(kakko[0]);
		Draw3DObject(kakko[1]);
		ChackBoxModel.each.scale = ChackBoxModel.each.scale - 0.1f;
		SupportModel.each.position = { 3.0f, -1.0f, -5.0f, 1.0f };
		SupportModel.Update();
		Draw3DObject(SupportModel);
		BackModel.each.position = { 1.12f, -3.0f, -5.0f, 1.0f };
		BackModel.Update();
		Draw3DObject(BackModel);
		TiTleModel.each.position = { 0.83f, -4.7f, -5.0f, 1.0f };
		TiTleModel.Update();
		Draw3DObject(TiTleModel);
		EndModel.each.position = { 0.72f, -6.2f, -5.0f, 1.0f };
		EndModel.Update();
		Draw3DObject(EndModel);
		ChackBoxModel.each.position = { -2.8f, -1.0f, -5.0f, 1.0f };
		ChackBoxModel.Update();
		Draw3DObject(ChackBoxModel);

	}

	//tips用
	const float max_size = 36;
	const float ander_size = 33;
	const float dist = 60;
	const float green_x = 1180;
	const float green_y = 662;
	const float sub_size = 0.2f;

	if (isTipsDraw)
	{
		isTipsDrawTrigger = true;
		tips_ss[0].SpriteDraw();
		tips_ss[1].SpriteDraw();
		tips_ss[2].SpriteDraw();
		tips_frame.SpriteDraw();
		tips_name[tipsCounts].SpriteDraw();
		if (tipsCounts < 5) { tips_system[0].SpriteDraw(); }
		else { tips_system[1].SpriteDraw(); }
		tips_bar.SpriteDraw();
		tips_text[tipsCounts].SpriteDraw();

		if (isTipsButtonDraw)
		{
			if (size < ander_size - 5) { size = max_size; }
			size -= sub_size;
			pushButtonGreen.ChangeSizeOther(size, size);
			pushButtonBlack[0].ChangeSize(ander_size, ander_size);
			pushButtonBlack[1].ChangeSize(ander_size, ander_size);
			pushButtonBlack[2].ChangeSize(ander_size, ander_size);

			pushButtonGreen.position = { green_x + ander_size / 2, green_y + ander_size / 2,0,0 };
			pushButtonGreen.SpriteDraw();

			pushButtonBlack[0].position = { green_x, green_y - dist,0,0 };
			pushButtonBlack[1].position = { green_x - dist / 2, green_y - dist / 2,0,0 };
			pushButtonBlack[2].position = { green_x + dist / 2, green_y - dist / 2,0,0 };

			pushButtonBlack[0].SpriteDraw();
			pushButtonBlack[1].SpriteDraw();
			pushButtonBlack[2].SpriteDraw();
		}
	}
	else if (!isTipsDraw && isTipsDrawTrigger)
	{
		tipsCounts++;
		isTipsDrawTrigger = false;
		if (tipsCounts >= 8) { tipsCounts = 0; }
	}

	else if (isCameraModed)
	{
		if (!isSceneChange && !isResultSceneChange)
		{
			if (size < ander_size - 5) { size = max_size; }
			size -= sub_size;
			pushButtonGreen.ChangeSizeOther(size, size);
			pushButtonBlack[0].ChangeSize(ander_size, ander_size);
			pushButtonBlack[1].ChangeSize(ander_size, ander_size);
			pushButtonBlack[2].ChangeSize(ander_size, ander_size);

			pushButtonGreen.position = { green_x + ander_size / 2, green_y + ander_size / 2,0,0 };
			pushButtonGreen.SpriteDraw();

			pushButtonBlack[0].position = { green_x, green_y - dist,0,0 };
			pushButtonBlack[1].position = { green_x - dist / 2, green_y - dist / 2,0,0 };
			pushButtonBlack[2].position = { green_x + dist / 2, green_y - dist / 2,0,0 };

			pushButtonBlack[0].SpriteDraw();
			pushButtonBlack[1].SpriteDraw();
			pushButtonBlack[2].SpriteDraw();
		}
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
	sky.each.rotation.x = 80.0f;
	sky.each.rotation.y += 1.0f;
	sky.Update();
	Draw3DObject(sky);
	othelloStage.each.rotation.x = -90;
	Draw3DObject(othelloStage);
	nowScore = checkObject.GetScore();
	float scoreNum = 2.0f;
	scoreEach[0].position = { 1.0f + scoreNum * 6, 19.0f, -12.0f, 1.0f };
	scoreEach[0].scale = { 0.4f, 0.4f, 0.4f };
	scoreEach[0].rotation.x = -70.0f;
	sNumbersObject[nowScore % 10].Update(&scoreEach[0]);
	Draw3DObject(sNumbersObject[nowScore % 10]);
	scoreEach[1].position = { 1.0f + scoreNum * 5, 19.0f, -12.0f, 1.0f };
	scoreEach[1].scale = { 0.4f, 0.4f, 0.4f };
	scoreEach[1].rotation.x = -70.0f;
	sNumbersObject[nowScore / 10 % 10].Update(&scoreEach[1]);
	Draw3DObject(sNumbersObject[nowScore / 10 % 10]);
	scoreEach[2].position = { 1.0f + scoreNum * 4, 19.0f, -12.0f, 1.0f };
	scoreEach[2].scale = { 0.4f, 0.4f, 0.4f };
	scoreEach[2].rotation.x = -70.0f;
	sNumbersObject[nowScore / 100 % 10].Update(&scoreEach[2]);
	Draw3DObject(sNumbersObject[nowScore / 100 % 10]);
	scoreEach[3].position = { 1.0f + scoreNum * 3, 19.0f, -12.0f, 1.0f };
	scoreEach[3].scale = { 0.4f, 0.4f, 0.4f };
	scoreEach[3].rotation.x = -70.0f;
	sNumbersObject[nowScore / 1000 % 10].Update(&scoreEach[3]);
	Draw3DObject(sNumbersObject[nowScore / 1000 % 10]);
	scoreEach[4].position = { 1.0f + scoreNum * 2, 19.0f, -12.0f, 1.0f };
	scoreEach[4].scale = { 0.4f, 0.4f, 0.4f };
	scoreEach[4].rotation.x = -70.0f;
	sNumbersObject[nowScore / 10000 % 10].Update(&scoreEach[4]);
	Draw3DObject(sNumbersObject[nowScore / 10000 % 10]);
	scoreEach[5].position = { 1.0f + scoreNum * 1, 19.0f, -12.0f, 1.0f };
	scoreEach[5].scale = { 0.4f, 0.4f, 0.4f };
	scoreEach[5].rotation.x = -70.0f;
	sNumbersObject[nowScore / 100000 % 10].Update(&scoreEach[5]);
	Draw3DObject(sNumbersObject[nowScore / 100000 % 10]);
	scoreEach[6].position = { 1.0f + scoreNum * 0, 19.0f, -12.0f, 1.0f };
	scoreEach[6].scale = { 0.4f, 0.4f, 0.4f };
	scoreEach[6].rotation.x = -70.0f;
	sNumbersObject[nowScore / 1000000 % 10].Update(&scoreEach[6]);
	Draw3DObject(sNumbersObject[nowScore / 1000000 % 10]);
	resultScoreKanaModel.each.position = { -5.0f, 19.0f, -12.0f, 1.0f };
	resultScoreKanaModel.each.scale = { 0.4f, 0.4f, 0.4f };
	resultScoreKanaModel.each.rotation.x = -70.0f;
	resultScoreKanaModel.Update();
	Draw3DObject(resultScoreKanaModel);

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

	int combo = checkObject.GetMaxCombo();
	int reverse = checkObject.GetTotalReverceCount();
	resultMaxConbo[0].position = { 3.0f, 17.0f, -10.0f, 1.0f };
	resultMaxConbo[0].scale = scale;
	resultMaxConbo[0].rotation.x = -70;
	sNumbersObject[combo % 10].Update(&resultMaxConbo[0]);
	Draw3DObject(sNumbersObject[combo % 10]);
	resultMaxConbo[1].position = { 1.0f, 17.0f, -10.0f, 1.0f };
	resultMaxConbo[1].scale = scale;
	resultMaxConbo[1].rotation.x = -70;
	sNumbersObject[combo / 10 % 10].Update(&resultMaxConbo[1]);
	Draw3DObject(sNumbersObject[combo / 10 % 10]);
	resultMaxConboModel.each.position = { -9.0f, 17.0f, -10.0f, 1.0f };
	resultMaxConboModel.each.scale = scale;
	resultMaxConboModel.each.rotation.x = -70;
	resultMaxConboModel.Update();
	Draw3DObject(resultMaxConboModel);

	resultEraseOthello[0].position = { 5.0f, 15.0f, -8.0f, 1.0f };
	resultEraseOthello[0].scale = scale;
	resultEraseOthello[0].rotation.x = -70;
	sNumbersObject[reverse % 10].Update(&resultEraseOthello[0]);
	Draw3DObject(sNumbersObject[reverse % 10]);
	resultEraseOthello[1].position = { 3.0f, 15.0f, -8.0f, 1.0f };
	resultEraseOthello[1].scale = scale;
	resultEraseOthello[1].rotation.x = -70;
	sNumbersObject[reverse / 10 % 10].Update(&resultEraseOthello[1]);
	Draw3DObject(sNumbersObject[reverse / 10 % 10]);
	resultEraseOthello[2].position = { 1.0f, 15.0f, -8.0f, 1.0f };
	resultEraseOthello[2].scale = scale;
	resultEraseOthello[2].rotation.x = -70;
	sNumbersObject[reverse / 100 % 10].Update(&resultEraseOthello[2]);
	Draw3DObject(sNumbersObject[reverse / 100 % 10]);
	resultEraseOthelloModel.each.position = { -7.0f, 15.0f, -8.0f, 1.0f };
	resultEraseOthelloModel.each.scale = scale;
	resultEraseOthelloModel.each.rotation.x = -70;
	resultEraseOthelloModel.Update();
	Draw3DObject(resultEraseOthelloModel);

	//Imgui::DrawImGui();

	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::GetScoreDraw()
{
	for (auto itr = addScore.scores.begin(); itr != addScore.scores.end(); ++itr)
	{
		for (int i = 0; i < 7; i++)
		{
			itr->scoreEach[i].position = itr->scoreEach[7].position;
			itr->scoreEach[i].position.m128_f32[0] += (7 - i) * 1.0f;
			itr->scoreEach[i].scale = { 0.3f, 0.3f, 0.3f };
		}
		sNumbersObject[itr->score % 10].Update(&itr->scoreEach[0]);
		Draw3DObject(sNumbersObject[itr->score % 10]);
		sNumbersObject[itr->score / 10 % 10].Update(&itr->scoreEach[1]);
		Draw3DObject(sNumbersObject[itr->score / 10 % 10]);
		sNumbersObject[itr->score / 100 % 10].Update(&itr->scoreEach[2]);
		Draw3DObject(sNumbersObject[itr->score / 100 % 10]);
		sNumbersObject[itr->score / 1000 % 10].Update(&itr->scoreEach[3]);
		Draw3DObject(sNumbersObject[itr->score / 1000 % 10]);
		sNumbersObject[itr->score / 10000 % 10].Update(&itr->scoreEach[4]);
		Draw3DObject(sNumbersObject[itr->score / 10000 % 10]);
		sNumbersObject[itr->score / 100000 % 10].Update(&itr->scoreEach[5]);
		Draw3DObject(sNumbersObject[itr->score / 100000 % 10]);
		sNumbersObject[itr->score / 100000 % 10].Update(&itr->scoreEach[6]);
		Draw3DObject(sNumbersObject[itr->score / 1000000 % 10]);
	}
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

void GameScene::ToGame4(bool flags, bool isToTutorial, bool isToDojo)
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
	this->isToTutorial = isToTutorial;
	this->isToDojo = isToDojo;
	isModeSelect = false;
	isTipsButtonDraw = false;
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
			isTipsButtonDraw = true;
			if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
			{
				if (isToTutorial)
				{
					othelloManager.TutorialStart();
				}
				else if (isToDojo)
				{
					othelloManager.DojoStart();
				}
				else if (!selectMode && !isTipsOk)
				{
					othelloManager.StartSetPos();
				}
				else if (flagss && !isTipsOk)
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
			isTipsButtonDraw = true;
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
				//OthlloPlayer::playerReverseObject->PlayAnimation();
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
				othelloManager.TutorialStart();
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
	isTipsButtonDraw = false;
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
		if (pouseToTitleEaseTime1 >= 1.1f)
		{
			pouseToTitleEaseTime1 = 1.1f;
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

void GameScene::NormaToModeSelect()
{
	sceneChageType = 30;
	pouseToTitleEaseTime1 = 0.0f;
	isBackGroundOthello = false;
	isSceneChange = true;
	isTipsButtonDraw = false;
	titleScaleEaseTime = 0.0f;
	ObjectParticles::othelloFrame.DeleteAllParticle();
	ObjectParticles::six.DeleteAllParticle();
	ObjectParticles::frame.DeleteAllParticle();
	othelloManager.AllDeletePanel();
	othelloManager.EraseChanceObject();
}

void GameScene::NormaToModeSelectUpdate()
{

	ObjectParticles::frame.DeleteAllParticle();
	pouseToTitleEaseTime1 += 0.02f;
	if (pouseToTitleEaseTime1 >= 1.0f)
	{
		pouseToTitleEaseTime1 = 1.0f;
		isBackGroundOthello = true;
		isTipsDraw = true;
		isTipsButtonDraw = true;
		Camera::eye.v = { -1.0f, -10.0f, -15.0f };
		Camera::target.v = { -1, 1, 0 };
		if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
		{
			SceneNum = GAME;
			gameTime = 1;
			NormaToModeSelect2();
		}
	}
	for (auto pouseOthelloItr = pouseOthellos.begin(); pouseOthelloItr != pouseOthellos.end(); ++pouseOthelloItr)
	{
		pouseOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(0.01f, 0.01f, 0.01f), XMFLOAT3(1.5f, 1.5f, 1.0f), pouseToTitleEaseTime1);
	}


	/*Camera::target = { -1, 1, 0 };
	Camera::eye = { -1, 1, -15 };*/
	Camera::Update();


	/*Camera::target = { -1, 1, 0 };
	Camera::eye = { -1, 1, -15 };*/
	Camera::Update();
}

void GameScene::NormaToModeSelect2()
{//初期化
	checkObject.Init();
	//シーンが変わることを伝える
	isSceneChange = true;
	//シーンの種類を決める
	sceneChageType = 31;
	isNormaToSelect = true;
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

void GameScene::NormaToModeSelectUpdate2()
{
	eyeEaseTime += 1.0f;
	if (eyeEaseTime >= 1.0f)
	{
		isStageDisplay = true;
		eyeEaseTime = 1.0f;
		sceneChangeDiray2++;

		if (sceneChangeDiray2 >= MaxSceneChangeOk)
		{
			if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
			{
				if (!isTipsOk)othelloManager.ModeSelectStart(selectStageNum + 1);
				SceneNum = GAME;
				isTipsOk = true;
			}
			if (isTipsOk)
			{
				Camera::eye.v = { -1.0f, -10.0f, -15.0f };
				Camera::target.v = { -1, 1, 0 };
				//セレクト画面のアップデート
				othelloManager.ModeSelectUpdate();
				isTipsDraw = false;
				isTipsButtonDraw = true;
				sceneChangeAfterTime += 0.01f;
				for (auto opOthelloItr = pouseOthellos.begin(); opOthelloItr != pouseOthellos.end(); ++opOthelloItr)
				{
					//opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.5f), XMFLOAT3(0.01f, 0.01f, 0.01f), sceneChangeAfterTime);
					opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.5f), XMFLOAT3(0.01f, 0.01f, 0.01f), sceneChangeAfterTime);
				}
			}
		}
		//ゲーム画面」に移行し、カウントダウン開始
		if (sceneChangeAfterTime >= 1.0f)
		{
			sceneChangeAfterTime = 1.0f;
			for (auto opOthelloItr = pouseOthellos.begin(); opOthelloItr != pouseOthellos.end(); ++opOthelloItr)
			{
				opOthelloItr->scale = ShlomonMath::EaseInQuad(XMFLOAT3(1.5f, 1.5f, 1.5f), XMFLOAT3(0.01f, 0.01f, 0.01f), sceneChangeAfterTime);
			}
			isPouseToTiTle = false;
			isSceneChange = false;
			isCameraModed = true;
			OthlloPlayer::isReverse = true;
		}
	}
	Camera::Update();
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

void AddScore::Init(int pos, int GetScore)
{
	isAppeared = false;
	isUpToed = false;
	isActive = true;
	posNum = pos;
	easeTime = 0.0f;
	easeUpTime = 0.0f;
	startPos = { -35.0f, 0, 0 };
	endPos = { -18.0f, 0, 0 };
	score = GetScore;
	for (int i = 0; i < 8; i++)
	{
		scoreEach[i].CreateConstBuff0();
		scoreEach[i].CreateConstBuff1();
	}
}

void AddScore::Update()
{
	if (isActive)
	{
		if (!isAppeared)
		{
			scoreEach[7].position = ConvertXMFLOAT3toXMVECTOR(ShlomonMath::EaseInQuad(startPos, endPos, easeTime));
			easeTime += easeUpCount;
			if (easeTime >= 1.0f)
			{
				easeTime = 1.0f;
				scoreEach[7].position = ConvertXMFLOAT3toXMVECTOR(ShlomonMath::EaseInQuad(startPos, endPos, easeTime));
			}
		}
		else if (isUp)
		{
			UpUpdate();
		}
	}
}

void AddScore::UpInit()
{
	easeUpTime = 0.0f;
	UpToStartPos = ConvertXMVECTORtoXMFLOAT3(scoreEach[7].position);
	UpToEndPos = UpToStartPos;
	UpToEndPos.y += 3.0f;
}

void AddScore::UpUpdate()
{
	easeTime += easeUpCount;
	scoreEach[7].position = ConvertXMFLOAT3toXMVECTOR(ShlomonMath::EaseInQuad(startPos, endPos, easeUpTime));
	if (easeUpTime >= 1.0f)
	{
		easeUpTime = 1.0f;
		scoreEach[7].position = ConvertXMFLOAT3toXMVECTOR(ShlomonMath::EaseInQuad(startPos, endPos, easeUpTime));
		isUpToed = false;
	}
}

void AddScoreManager::Init(int PosNum, int GetScore)
{
	AddScore ex;
	ex.Init(nowDisplayNum, GetScore);
	scores.push_back(ex);
	if (scores.size() > MaxNum)
	{
		auto itr = scores.begin();
		scores.erase(itr);
	}
	if (PosNum < MaxNum)
	{
		PosNum++;
	}
	else
	{
		PosNum = MaxNum;
		auto itr = scores.begin();
		++itr;
		itr->UpInit();
		++itr;
		itr->UpInit();
	}
}

void AddScoreManager::Update()
{
	for (auto itr = scores.begin(); itr != scores.end(); ++itr)
	{
		itr->Update();
	}
}
