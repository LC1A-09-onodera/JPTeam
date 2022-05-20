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
	Camera::eye.v = gameNowEye;
	Camera::target.v = gameOpTarget;
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
	PostEffects::Init();

	/*model = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	object = new FBXObject;
	object->Initialize();
	object->SetModel(model);
	object->PlayAnimation();*/
	//sample.CreateModel("newOserro", ShaderManager::playerShader);
	//sample.each.rotation.x = 0;


	checkObject.Init();
	ObjectParticles::LoadModels();

	othelloManager.Init();
	othelloManager.AddPanel();

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
	for (int i = 0; i < 30; i += 10)
	{
		numbers[i + 0].CreateSprite(num[0], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		numbers[i + 1].CreateSprite(num[1], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		numbers[i + 2].CreateSprite(num[2], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		numbers[i + 3].CreateSprite(num[3], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		numbers[i + 4].CreateSprite(num[4], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		numbers[i + 5].CreateSprite(num[5], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		numbers[i + 6].CreateSprite(num[6], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		numbers[i + 7].CreateSprite(num[7], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		numbers[i + 8].CreateSprite(num[8], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		numbers[i + 9].CreateSprite(num[9], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	}
	for (int i = 0; i < 60; i += 10)
	{
		scoreNum[i + 0].CreateSprite(num[0], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		scoreNum[i + 1].CreateSprite(num[1], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		scoreNum[i + 2].CreateSprite(num[2], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		scoreNum[i + 3].CreateSprite(num[3], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		scoreNum[i + 4].CreateSprite(num[4], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		scoreNum[i + 5].CreateSprite(num[5], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		scoreNum[i + 6].CreateSprite(num[6], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		scoreNum[i + 7].CreateSprite(num[7], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		scoreNum[i + 8].CreateSprite(num[8], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		scoreNum[i + 9].CreateSprite(num[9], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	}
	for (int i = 0; i < 60; i += 10)
	{
		addScoreNum[i + 0].CreateSprite(num[0], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addScoreNum[i + 1].CreateSprite(num[1], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addScoreNum[i + 2].CreateSprite(num[2], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addScoreNum[i + 3].CreateSprite(num[3], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addScoreNum[i + 4].CreateSprite(num[4], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addScoreNum[i + 5].CreateSprite(num[5], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addScoreNum[i + 6].CreateSprite(num[6], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addScoreNum[i + 7].CreateSprite(num[7], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addScoreNum[i + 8].CreateSprite(num[8], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addScoreNum[i + 9].CreateSprite(num[9], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	}
	for (int i = 0; i < 30; i += 10)
	{
		addConbo[i + 0].CreateSprite(num[0], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addConbo[i + 1].CreateSprite(num[1], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addConbo[i + 2].CreateSprite(num[2], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addConbo[i + 3].CreateSprite(num[3], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addConbo[i + 4].CreateSprite(num[4], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addConbo[i + 5].CreateSprite(num[5], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addConbo[i + 6].CreateSprite(num[6], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addConbo[i + 7].CreateSprite(num[7], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addConbo[i + 8].CreateSprite(num[8], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
		addConbo[i + 9].CreateSprite(num[9], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	}

	//selectStageNumSprite[0].CreateSprite(num[0], XMFLOAT3(window_width / 2 - 30,                            window_height / 2 + 50, 0));
	selectStageNumSprite[1].CreateSprite(num[1], XMFLOAT3(window_width / 2 - 30 + selectNumDistance * 0.0f, window_height / 2 + 50, 0));
	selectStageNumSprite[2].CreateSprite(num[2], XMFLOAT3(window_width / 2 - 30 + selectNumDistance * 1.0f, window_height / 2 + 50, 0));
	selectStageNumSprite[3].CreateSprite(num[3], XMFLOAT3(window_width / 2 - 30 + selectNumDistance * 2.0f, window_height / 2 + 50, 0));
	selectStageNumSprite[4].CreateSprite(num[4], XMFLOAT3(window_width / 2 - 30 + selectNumDistance * 3.0f, window_height / 2 + 50, 0));
	selectStageNumSprite[5].CreateSprite(num[5], XMFLOAT3(window_width / 2 - 30 + selectNumDistance * 4.0f, window_height / 2 + 50, 0));
	selectStageNumSprite[6].CreateSprite(num[6], XMFLOAT3(window_width / 2 - 30 + selectNumDistance * 5.0f, window_height / 2 + 50, 0));
	selectStageNumSprite[7].CreateSprite(num[7], XMFLOAT3(window_width / 2 - 30 + selectNumDistance * 6.0f, window_height / 2 + 50, 0));
	selectStageNumSprite[8].CreateSprite(num[8], XMFLOAT3(window_width / 2 - 30 + selectNumDistance * 7.0f, window_height / 2 + 50, 0));
	selectStageNumSprite[9].CreateSprite(num[9], XMFLOAT3(window_width / 2 - 30 + selectNumDistance * 8.0f, window_height / 2 + 50, 0));
	selectStageNumSprite[10].CreateSprite(L"Resource/Img/number_10.png", XMFLOAT3(window_width / 2 - 30 + selectNumDistance * 9.0f, window_height / 2 + 50, 0));
	addReverse.CreateSprite(L"Resource/Img/combo.png", XMFLOAT3(0, 0, 0));
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
			each.position.m128_f32[1] = -(12 / 2.0f * OthelloR) + (j * OthelloR) - 20;
			each.position.m128_f32[2] = 0;
			each.scale = { 0.01f, 0.01f, 0.01f };
			opOthellos.push_back(each);
		}
	}

	//Timer管理用
	isChanged = false;
	timerCount = 0;
	size_x = MAX_SIZE_X;
	size_y = MAX_SIZE_Y;
}

void GameScene::TitleUpdate()
{
	//SoundPlayLoop(BGMSound);

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
			if ((Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01)) && !isPouse)
			{
				if (titleSelectNum == 1)
				{
					SoundStopWave(enterSound);
					SoundPlayOnce(enterSound);

					ToGame();

					isTutorial = true;
					titleSelectNum = 0;
					othelloManager.whySandwichSpawn();
					gameTime = 60;
				}
				else
				{
					select = true;
					selectStage = false;
					selectMode = false;
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
					selectStageFarstStartPos = ConvertXMVECTORtoXMFLOAT3(selectStageNumSprite[1].position);
					selectStageFarstEndPos = selectStageFarstStartPos;
					selectStageFarstEndPos.x += selectNumDistance;
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
					selectStageFarstStartPos = ConvertXMVECTORtoXMFLOAT3(selectStageNumSprite[1].position);
					selectStageFarstEndPos = selectStageFarstStartPos;
					selectStageFarstEndPos.x -= selectNumDistance;
					selectStageNum++;
				}
			}
			if ((Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01)) && !isPouse)
			{

				SoundStopWave(enterSound);
				SoundPlayOnce(enterSound);

				//スコアアタック
				if (!selectMode)
				{
					gameTime = gameMaxTime;
					isTutorial = false;
					ToGame2();
					othelloManager.StartSetPos();
					titleSelectNum = 0;
				}
				//ノルマモード
				else
				{
					if (selectStage)
					{
						gameTime = gameMaxTime;
						isTutorial = false;
						ToGame2();
						othelloManager.StartNormaMode(selectStageNum);
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
	if (selectEase)
	{
		selectStageNumSprite[1].position = ConvertXMFLOAT3toXMVECTOR(ShlomonMath::EaseInQuad(selectStageFarstStartPos, selectStageFarstEndPos, selectEaseTime));
		for (int i = 2; i < othelloManager.GetNormaStagesCount(); i++)
		{
			selectStageNumSprite[i].position.m128_f32[0] = selectStageNumSprite[1].position.m128_f32[0] + (selectNumDistance * (i - 1));
		}
		selectEaseTime += 0.1f;
		if (selectEaseTime > 1.0f)
		{
			selectStageNumSprite[1].position = ConvertXMFLOAT3toXMVECTOR(ShlomonMath::EaseInQuad(selectStageFarstStartPos, selectStageFarstEndPos, 1.0));
			for (int i = 2; i < othelloManager.GetNormaStagesCount(); i++)
			{
				selectStageNumSprite[i].position.m128_f32[0] = selectStageNumSprite[1].position.m128_f32[0] + (selectNumDistance * (i - 1));
			}
			selectEase = false;
			selectEaseTime = 0;
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
	sky.Update();
	othelloStage.Update();
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
		if (countDown <= 0)
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
					isSceneChange = true;
				}
			}
			if (othelloManager.GetIsSendDataUpdate())
			{
				checkObject.Update(othelloManager.Send(), othelloManager.GetIsSendDataUpdate());
				othelloManager.Receive(checkObject.GetOthelloDatas());
			}
			if (!isTutorial && !selectMode)
			{
				gameTime--;
			}
			else if (othelloManager.IsTutorialEnd() && isTutorial)
			{
				gameTime--;
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
	//タイトルからgameシーンへ
	if (isSceneChange)
	{
		if (sceneChageType == 1)
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
		else if (sceneChageType == 2)
		{
			//回転スタート
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
		else if (sceneChageType == 3)
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
	}
	//ゲームシーンからリザルトへのトリガー
	if (gameTime <= 0 && !isResultSceneChange)
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
		eyeStart = Camera::target.v;
		eyeEnd = { -1, 100, 0 };
		eyeEaseTime = 0;
		resultForTime = 0;
		select = false;
		ObjectParticles::othelloFrame.DeleteAllParticle();
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
				if (isPouseToTitle == true)
				{
					SceneNum = TITLE;
					SoundPlayLoop(BGMSound);
					isPouseToTitle = false;
				}
				else if (isTutorial == true)
				{
					SceneNum = TITLE;
					SoundPlayLoop(BGMSound);
					isTutorial = false;
				}
				else
				{
					SceneNum = RESULT;
					displayScore = checkObject.GetScore();
				}
			}
			Camera::Update();
		}
		else
		{
			resultForTime++;
		}
	}
	if (!isPouse && (Input::KeyTrigger(DIK_ESCAPE) || directInput->IsButtonPush(directInput->ButtonPouse)) && countDown <= 0)
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
				gameTime = 0;
				isPouseToTitle = true;
				isPouse = false;
				select = false;
				//isSceneChange = true;
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
		if (tornadoTime > 20)
		{
			ObjectParticles::triangle.Init(XMFLOAT3(0, 0, 0), checkObject.GetCombo() - 2, ParticleType::Tornado);
			tornadoTime = 0;
		}
	}
	else
	{
		tornadoTime = 0;
	}
	if (!isSceneChange && !isResultSceneChange && !isPouse)
	{
		if (Input::KeyTrigger(DIK_U) || directInput->IsButtonPush(directInput->Button03) && !isTutorial)
		{
			ReStart();
		}
	}

	sky.Update();
	othelloStage.Update();
	Lights::Add(checkObject);
	Lights::Update();
	ParticleControl::Update();
}

void GameScene::ResultUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
	{
		SoundStopWave(enterSound);
		SoundPlayOnce(enterSound);
		OthlloPlayer::SetPosition(XMFLOAT3(0, 0, -2));
		SoundPlayLoop(BGMSound);
		SceneNum = TITLE;
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
	//PostEffectのPreDraw
	//postEffect.PreDraw();

	//Draw3DObject(sample);
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffectのDraw
	//postEffect.Draw();
	Draw3DObject(sky);
	Draw3DObject(othelloStage);
	//if (!isSceneChange) OthlloPlayer::Draw();

	ParticleControl::Draw();
	ObjectParticles::Draw();
	Lights::Draw();
	//スプライトの描画-------------------------
	if (isSceneChange == false)
	{
		titleBack.ChangeSize(1280, 125 * 2);
		titleBack.position.m128_f32[0] = 0;
		titleBack.position.m128_f32[1] = 100;
		titleBack.SpriteDraw();
		title.SpriteDraw();
		spaceBack.position.m128_f32[0] = 0;
		spaceBack.position.m128_f32[1] = 560;
		spaceBack.SpriteDraw();
		space.SpriteDraw();
		float spsize = 1.2f;
		if (!select)
		{
			startSprite.ChangeSize(232 * spsize, 63 * spsize);
			startSprite.position.m128_f32[0] = window_width / 2 + 100;
			startSprite.position.m128_f32[1] = 400;
			startSprite.SpriteDraw();
			tutorialSprite.position.m128_f32[0] = window_width / 2 - 400;
			tutorialSprite.position.m128_f32[1] = 400;
			tutorialSprite.SpriteDraw();
			if (titleSelectNum == 0)
			{
				kagikakkoStartSprite.ChangeSize(30, 70);
				kagikakkoStartSprite.position.m128_f32[0] = window_width / 2 + 60;
				kagikakkoStartSprite.position.m128_f32[1] = 410;
				kagikakkoStartSprite.SpriteDraw();
				kagikakkoEndSprite.ChangeSize(30, 70);
				kagikakkoEndSprite.position.m128_f32[0] = window_width / 2 + 390;
				kagikakkoEndSprite.position.m128_f32[1] = 410;
				kagikakkoEndSprite.SpriteDraw();
			}
			else
			{
				kagikakkoStartSprite.ChangeSize(30, 70);
				kagikakkoStartSprite.position.m128_f32[0] = window_width / 2 - 440;
				kagikakkoStartSprite.position.m128_f32[1] = 410;
				kagikakkoStartSprite.SpriteDraw();
				kagikakkoEndSprite.ChangeSize(30, 70);
				kagikakkoEndSprite.position.m128_f32[0] = window_width / 2 - 10;
				kagikakkoEndSprite.position.m128_f32[1] = 410;
				kagikakkoEndSprite.SpriteDraw();
			}
		}
		else if (!selectStage)
		{
			gameScoreAttackSprite.ChangeSize(232 * spsize, 63 * spsize);
			gameScoreAttackSprite.position.m128_f32[0] = window_width / 2 + 110;
			gameScoreAttackSprite.position.m128_f32[1] = 400;
			gameScoreAttackSprite.SpriteDraw();
			gameNormaSprite.ChangeSize(282 * spsize, 63 * spsize);
			gameNormaSprite.position.m128_f32[0] = window_width / 2 - 400;
			gameNormaSprite.position.m128_f32[1] = 400;
			gameNormaSprite.SpriteDraw();
			if (!selectMode)
			{
				kagikakkoStartSprite.ChangeSize(30, 70);
				kagikakkoStartSprite.position.m128_f32[0] = window_width / 2 + 75;
				kagikakkoStartSprite.position.m128_f32[1] = 410;
				kagikakkoStartSprite.SpriteDraw();
				kagikakkoEndSprite.ChangeSize(30, 70);
				kagikakkoEndSprite.position.m128_f32[0] = window_width / 2 + 400;
				kagikakkoEndSprite.position.m128_f32[1] = 410;
				kagikakkoEndSprite.SpriteDraw();
			}
			else
			{
				kagikakkoStartSprite.ChangeSize(30, 70);
				kagikakkoStartSprite.position.m128_f32[0] = window_width / 2 - 430;
				kagikakkoStartSprite.position.m128_f32[1] = 410;
				kagikakkoStartSprite.SpriteDraw();
				kagikakkoEndSprite.ChangeSize(30, 70);
				kagikakkoEndSprite.position.m128_f32[0] = window_width / 2 - 50;
				kagikakkoEndSprite.position.m128_f32[1] = 410;
				kagikakkoEndSprite.SpriteDraw();
			}
		}
		else
		{
			//selectStageNumSprite[0].SpriteDraw();
			for (int i = 1; i < othelloManager.GetNormaStagesCount(); i++)
			{
				selectStageNumSprite[i].SpriteDraw();

			}

			kagikakkoSelectStartSprite.position.m128_f32[0] = 520;
			kagikakkoSelectStartSprite.position.m128_f32[1] = 410;
			kagikakkoSelectEndSprite.position.m128_f32[0] = 700;
			kagikakkoSelectEndSprite.position.m128_f32[1] = 410;
			kagikakkoSelectStartSprite.SpriteDraw();
			kagikakkoSelectEndSprite.SpriteDraw();
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
	Imgui::DrawImGui();
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
	//PostEffectのPreDraw
	//PostEffects::PreDraw();

	//Draw3DObject(sample);

	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffectのDraw
	//PostEffects::Draw();

	//PostEffectのPostDraw
	//PostEffects::PostDraw();

	Draw3DObject(sky);
	Draw3DObject(othelloStage);
	if (isSceneChange == true && sceneChageType == 3)
	{
		for (auto opOthelloItr = opOthellos.begin(); opOthelloItr != opOthellos.end(); ++opOthelloItr)
		{
			ObjectParticles::othello2.object.Update(&(*opOthelloItr));
			Draw3DObject(ObjectParticles::othello2.object);
		}
	}
	OthlloPlayer::Draw();
	ObjectParticles::Draw();
	ParticleControl::Draw();
	othelloManager.Draw();
	Lights::Draw();

	//スプライトの描画-------------------------
	//titleSprite.SpriteDraw();
	if (countDown > 0 && isSceneChange == false)
	{
		if (countDown < 59)
		{
			startSprite.ChangeSize(232, 63);
			startSprite.position.m128_f32[0] = window_width / 2 - 100;
			startSprite.position.m128_f32[1] = window_height / 2 - 10;
			startSprite.SpriteDraw();
		}
		else
		{
			numbers[countDown / 60].position.m128_f32[0] = window_width / 2 - 10;
			numbers[countDown / 60].position.m128_f32[1] = window_height / 2 - 5;
			numbers[countDown / 60].SpriteDraw();
		}
	}
	if (isSceneChange == true && sceneChageType == 2)
	{
		sceneChangeSprite2.ChangeSize(1280, 720);
		//sceneChangeSprite2.rotation = 0.1f;
		sceneChangeSprite2.SpriteDraw();
	}
	if (gameTime > 0 && countDown <= 0)
	{
		if (gameTime / 60 >= CHANGE_TIMER_SECOND)
		{
			numbers[gameTime / 60 % 10].ChangeSize(48, 64);
			numbers[gameTime / 600 % 10 + 10].ChangeSize(48, 64);
		}
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
					//numbers[gameTime / 60 % 10].ChangeSize(size_x, size_y);
					//numbers[gameTime / 600 % 10].ChangeSize(size_x, size_y);
					timerCount = 0;
					isChanged = false;
				}

				numbers[gameTime / 60 % 10].ChangeSize(size_x, size_y);
				numbers[gameTime / 600 % 10 + 10].ChangeSize(size_x, size_y);

				timerCount++;
			}
		}

		numbers[gameTime / 60 % 10].position.m128_f32[0] = window_width / 2 + 0;
		numbers[gameTime / 60 % 10].position.m128_f32[1] = 10;
		numbers[gameTime / 60 % 10].SpriteDraw();
		numbers[gameTime / 600 % 10 + 10].position.m128_f32[0] = window_width / 2 - 50;
		numbers[gameTime / 600 % 10 + 10].position.m128_f32[1] = 10;
		numbers[gameTime / 600 % 10 + 10].SpriteDraw();

		oldDisplay = nowScore;
		oldScore = nowScore;
		nowScore = checkObject.GetScore();

		//playerの頭上にスコアを出す
		if (checkObject.IsAddScore())
		{
			int addComboint = checkObject.GetCombo();
			if (addComboint < 10)
			{
				addScoreNum[addComboint % 10].position.m128_f32[0] = window_width / 2 - 64;
				addScoreNum[addComboint % 10].position.m128_f32[1] = 74;
				addScoreNum[addComboint % 10].ChangeSize(48, 64);
				addScoreNum[addComboint % 10].SpriteDraw();
				addReverse.position.m128_f32[0] = window_width / 2;
				addReverse.position.m128_f32[1] = 74 + 32;
				addReverse.ChangeSize(192, 32);
				addReverse.SpriteDraw();
			}
			else if (addComboint < 100)
			{
				addScoreNum[addComboint % 10].position.m128_f32[0] = window_width / 2 - 64;
				addScoreNum[addComboint % 10].position.m128_f32[1] = 74;
				addScoreNum[addComboint % 10].ChangeSize(48, 64);
				addScoreNum[addComboint % 10].SpriteDraw();

				addScoreNum[addComboint / 10 % 10 + 10].position.m128_f32[0] = window_width / 2 - 112;
				addScoreNum[addComboint / 10 % 10 + 10].position.m128_f32[1] = 74;
				addScoreNum[addComboint / 10 % 10 + 10].ChangeSize(48, 64);
				addScoreNum[addComboint / 10 % 10 + 10].SpriteDraw();

				addReverse.position.m128_f32[0] = window_width / 2;
				addReverse.position.m128_f32[1] = 74 + 32;
				addReverse.ChangeSize(192, 32);
				addReverse.SpriteDraw();
			}
			else if (addComboint < 1000)
			{
				addScoreNum[addComboint % 10].position.m128_f32[0] = window_width / 2 - 64;
				addScoreNum[addComboint % 10].position.m128_f32[1] = 74;
				addScoreNum[addComboint % 10].ChangeSize(48, 64);
				addScoreNum[addComboint % 10].SpriteDraw();

				addScoreNum[addComboint / 10 % 10 + 10].position.m128_f32[0] = window_width / 2 - 112;
				addScoreNum[addComboint / 10 % 10 + 10].position.m128_f32[1] = 74;
				addScoreNum[addComboint / 10 % 10 + 10].ChangeSize(48, 64);
				addScoreNum[addComboint / 10 % 10 + 10].SpriteDraw();

				addReverse.position.m128_f32[0] = window_width / 2;
				addReverse.position.m128_f32[1] = 74 + 32;
				addReverse.ChangeSize(192, 32);
				addReverse.SpriteDraw();
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
						changeCount = 0;
					}
				}
				else
				{
					changeCount++;
					if (changeCount == changeTime)
					{
						displayScore += sub / 60;
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

		float wid = 35;
		float widPuls = 40;
		float hi = 50;
		scoreSprite.position.m128_f32[0] = 0;
		scoreSprite.position.m128_f32[1] = 15;
		scoreSprite.ChangeSize(150, 50);
		scoreSprite.SpriteDraw();
		scoreNum[displayScore % 10].ChangeSize(wid, hi);
		scoreNum[displayScore % 10].position.m128_f32[0] = widPuls * 9;
		scoreNum[displayScore % 10].position.m128_f32[1] = 10;
		scoreNum[displayScore % 10].SpriteDraw();
		scoreNum[displayScore / 10 % 10 + 10].ChangeSize(wid, hi);
		scoreNum[displayScore / 10 % 10 + 10].position.m128_f32[0] = widPuls * 8;
		scoreNum[displayScore / 10 % 10 + 10].position.m128_f32[1] = 10;
		scoreNum[displayScore / 10 % 10 + 10].SpriteDraw();
		scoreNum[displayScore / 100 % 10 + 20].ChangeSize(wid, hi);
		scoreNum[displayScore / 100 % 10 + 20].position.m128_f32[0] = widPuls * 7;
		scoreNum[displayScore / 100 % 10 + 20].position.m128_f32[1] = 10;
		scoreNum[displayScore / 100 % 10 + 20].SpriteDraw();
		scoreNum[displayScore / 1000 % 10 + 30].ChangeSize(wid, hi);
		scoreNum[displayScore / 1000 % 10 + 30].position.m128_f32[0] = widPuls * 6;
		scoreNum[displayScore / 1000 % 10 + 30].position.m128_f32[1] = 10;
		scoreNum[displayScore / 1000 % 10 + 30].SpriteDraw();
		scoreNum[displayScore / 10000 % 10 + 40].ChangeSize(wid, hi);
		scoreNum[displayScore / 10000 % 10 + 40].position.m128_f32[0] = widPuls * 5;
		scoreNum[displayScore / 10000 % 10 + 40].position.m128_f32[1] = 10;
		scoreNum[displayScore / 10000 % 10 + 40].SpriteDraw();
		scoreNum[displayScore / 100000 % 10 + 50].ChangeSize(wid, hi);
		scoreNum[displayScore / 100000 % 10 + 50].position.m128_f32[0] = widPuls * 4;
		scoreNum[displayScore / 100000 % 10 + 50].position.m128_f32[1] = 10;
		scoreNum[displayScore / 100000 % 10 + 50].SpriteDraw();
		/*moveSprite.position.m128_f32[0] = 950;
		moveSprite.position.m128_f32[1] = 10;
		float sizeSp = 0.6f;
		moveSprite.ChangeSize(382 * sizeSp, 433 * sizeSp);
		moveSprite.SpriteDraw();*/

		//othelloManager.TutorialRetryText.SpriteDraw();

		othelloManager.NormaTextDraw();
		if (isTutorial)
		{
			othelloManager.TutorialTextDraw();
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

	//Imgui::DrawImGui();
	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::ResultDraw()
{
	//PostEffectのPreDraw
	//postEffect.PreDraw();

	//Player::GetPlayer()->Draw();

	//PostEffectのPostDraw
	//postEffect.PostDraw();

	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffectのDraw
	//postEffect.Draw();
	//OthlloPlayer::Draw();
	Draw3DObject(sky);
	Draw3DObject(othelloStage);
	//スプライトの描画-------------------------
	//titleSprite.SpriteDraw();

	titleBack.position.m128_f32[0] = 0;
	titleBack.position.m128_f32[1] = window_height / 2 - 55;
	titleBack.ChangeSize(1280, 100);
	titleBack.SpriteDraw();
	scoreSprite.position.m128_f32[0] = window_width / 2 - 180;
	scoreSprite.position.m128_f32[1] = window_height / 2 - 30;
	scoreSprite.SpriteDraw();
	float wid = 40;
	float widPuls = 45;
	nowScore = checkObject.GetScore();
	scoreNum[nowScore % 10].ChangeSize(wid, 60);
	scoreNum[nowScore % 10].position.m128_f32[0] = window_width / 2 - 180 + widPuls * 9;
	scoreNum[nowScore % 10].position.m128_f32[1] = window_height / 2 - 40;
	scoreNum[nowScore % 10].SpriteDraw();
	scoreNum[nowScore / 10 % 10 + 10].ChangeSize(wid, 60);
	scoreNum[nowScore / 10 % 10 + 10].position.m128_f32[0] = window_width / 2 - 180 + widPuls * 8;
	scoreNum[nowScore / 10 % 10 + 10].position.m128_f32[1] = window_height / 2 - 40;
	scoreNum[nowScore / 10 % 10 + 10].SpriteDraw();
	scoreNum[nowScore / 100 % 10 + 20].ChangeSize(wid, 60);
	scoreNum[nowScore / 100 % 10 + 20].position.m128_f32[0] = window_width / 2 - 180 + widPuls * 7;
	scoreNum[nowScore / 100 % 10 + 20].position.m128_f32[1] = window_height / 2 - 40;
	scoreNum[nowScore / 100 % 10 + 20].SpriteDraw();
	scoreNum[nowScore / 1000 % 10 + 30].ChangeSize(wid, 60);
	scoreNum[nowScore / 1000 % 10 + 30].position.m128_f32[0] = window_width / 2 - 180 + widPuls * 6;
	scoreNum[nowScore / 1000 % 10 + 30].position.m128_f32[1] = window_height / 2 - 40;
	scoreNum[nowScore / 1000 % 10 + 30].SpriteDraw();
	scoreNum[nowScore / 10000 % 10 + 40].ChangeSize(wid, 60);
	scoreNum[nowScore / 10000 % 10 + 40].position.m128_f32[0] = window_width / 2 - 180 + widPuls * 5;
	scoreNum[nowScore / 10000 % 10 + 40].position.m128_f32[1] = window_height / 2 - 40;
	scoreNum[nowScore / 10000 % 10 + 40].SpriteDraw();
	scoreNum[nowScore / 100000 % 10 + 50].ChangeSize(wid, 60);
	scoreNum[nowScore / 100000 % 10 + 50].position.m128_f32[0] = window_width / 2 - 180 + widPuls * 4;
	scoreNum[nowScore / 100000 % 10 + 50].position.m128_f32[1] = window_height / 2 - 40;
	scoreNum[nowScore / 100000 % 10 + 50].SpriteDraw();
	spaceBack.position.m128_f32[0] = 0;
	spaceBack.position.m128_f32[1] = 560;
	spaceBack.SpriteDraw();
	space.SpriteDraw();
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
		ObjectParticles::triangle.Init(pos, 3, ParticleType::Exprotion);
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
	postEffect.Draw();

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
