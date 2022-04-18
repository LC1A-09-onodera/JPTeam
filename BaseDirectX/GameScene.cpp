#include"GameScene.h"
#include "BaseDirectX.h"
#include "../WindowsAPI/WinAPI.h"
#include "Input.h"
#include "../Sound/Sound.h"
#include "viewport.h"
#include "../Particle/Particle3D.h"
#include "../imgui/ImguiControl.h"
#include "../Sound/Sound.h"
#include "../Light/Light.h"
#include "vec3.h"
#include "../FBXObject/FBXObject.h"
#include "../Shader/ShaderManager.h"
#include"../Enemy/Enemy.h"
#include "../Hole/Hole.h"
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
	delete(light);
	/*delete(object);
	delete(model);*/
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
	Camera::eye = { 0, -10, -15.0 };
	Camera::target = { 0, 100, 0 };
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
	//プレイヤーの初期化
	Player::GetPlayer()->Init();

	//ポストエフェクトの初期化
	postEffect.Initialize();


	/*model = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	object = new FBXObject;
	object->Initialize();
	object->SetModel(model);
	object->PlayAnimation();*/
	//sample.CreateModel("newOserro", ShaderManager::playerShader);
	//sample.each.rotation.x = 0;

	othelloManager.Init();
	othelloManager.AddPanel();

	checkObject.Init();
	ObjectParticles::LoadModels();

	water.CreateWater();
	OthlloPlayer::Init();

	ThunderModels::LoadModels();
	title.CreateSprite(L"Resource/Img/titel.png", XMFLOAT3(70, 60 + 50, 0));
	titleBack.CreateSprite(L"Resource/Img/title_back.png", XMFLOAT3(0, 46 + 50, 0));
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
	numbers[0].CreateSprite(num[0], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[1].CreateSprite(num[1], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[2].CreateSprite(num[2], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[3].CreateSprite(num[3], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[4].CreateSprite(num[4], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[5].CreateSprite(num[5], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[6].CreateSprite(num[6], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[7].CreateSprite(num[7], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[8].CreateSprite(num[8], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[9].CreateSprite(num[9], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
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
	scoreSprite.CreateSprite(L"Resource/Img/score.png", XMFLOAT3(0, 0, 0));
	timeUp.CreateSprite(L"Resource/Img/time_up.png", XMFLOAT3(0, 0, 0));
	startSprite.CreateSprite(L"Resource/Img/START.png", XMFLOAT3(0, 0, 0));
	gameTime = gameMaxTime;

	numbers[0].CreateSprite(num[0], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[1].CreateSprite(num[1], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[2].CreateSprite(num[2], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[3].CreateSprite(num[3], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[4].CreateSprite(num[4], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[5].CreateSprite(num[5], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[6].CreateSprite(num[6], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[7].CreateSprite(num[7], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[8].CreateSprite(num[8], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	numbers[9].CreateSprite(num[9], XMFLOAT3(window_width / 2 - 10, window_height / 2 - 10, 0));
	Lights::LoadModels();
	sky.CreateModel("world", ShaderManager::playerShader, true);
	sky.each.rotation = { 70, 0.0f, 0 };
	sky.each.scale = { 100.0f ,100.0f, 100.f };
	othelloStage.CreateModel("board", ShaderManager::playerShader);
	othelloStage.each.position.m128_f32[0] = -1.0f;
	othelloStage.each.position.m128_f32[1] = 1.0f;
	othelloStage.each.position.m128_f32[2] = 2.5f;
	othelloStage.each.rotation.x = 90;
	othelloStage.each.scale = { 2.0f, 1.0f, 2.0f };
}

void GameScene::TitleUpdate()
{
	//オセロのパーティクルを出していく
	static int particleTime = 0;
	if (!isSceneChange)
	{
		particleTime++;
		if (particleTime % 5 == 4)
		{
			ObjectParticles::othello.Init(XMFLOAT3(0, 0, -15), 1, ParticleType::TITLE);
		}
	}

	ObjectParticles::Update();
	if (Input::KeyTrigger(DIK_SPACE))
	{
		//オセロを爆散させてカメラの動きを開始させる
		for (auto triangleItr = ObjectParticles::othello.particles.begin(); triangleItr != ObjectParticles::othello.particles.end(); ++triangleItr)
		{
			XMFLOAT3 pos = ConvertXMVECTORtoXMFLOAT3(triangleItr->each.position);
			ObjectParticles::triangle.Init(pos, 10, ParticleType::Exprotion);
			triangleItr->time = 1;
			countDown = countMax;
		}
		isSceneChange = true;
		eyeStart = Camera::target.v;
		eyeEnd = { 0.0f, 0.0f, 0.0f };
		eyeEaseTime = 0;
		gameTime = gameMaxTime;
		SceneNum = GAME;
		countDown = 239;
	}
	sky.Update();
	othelloStage.Update();
}

void GameScene::SelectUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE))
	{
		Player::GetPlayer()->Init();

		SceneNum = GAME;
	}
}

void GameScene::GameUpdate()
{
	//カメラ変化が行われていない時にゲームを開始する
	if (!isSceneChange && !isResultSceneChange)
	{
		if (countDown <= 0)
		{
			OthlloPlayer::Update();
			ThunderModels::Update();
			othelloManager.Controll();
			othelloManager.Update();
			checkObject.Update(othelloManager.Send());
			othelloManager.Receive(checkObject.GetOthelloDatas());

			//gameTime--;
		}
		else
		{
			countDown--;
		}
	}
	ObjectParticles::Update();
	//タイトルからgameシーンへ
	if (isSceneChange)
	{
		Camera::target.v = EaseInQuad(eyeStart, eyeEnd, eyeEaseTime);
		eyeEaseTime += 0.02f;
		if (eyeEaseTime > 1.0f)
		{
			Camera::target.v = EaseInQuad(eyeStart, eyeEnd, 1.0f);
			isSceneChange = false;
			gameTime = gameMaxTime;
		}
		Camera::Update();
	}
	//ゲームシーンからリザルトへのトリガー
	if (gameTime <= 0 && !isResultSceneChange)
	{
		for (auto triangleItr = OthelloManager::othellos.begin(); triangleItr != OthelloManager::othellos.end(); ++triangleItr)
		{
			XMFLOAT3 pos = triangleItr->GetPosition();
			ObjectParticles::triangle.Init(pos, 10, ParticleType::Exprotion);
			triangleItr->GetGameData()->isDead = true;
			ThunderModels::DeleteList();
		}
		othelloManager.DeadPanel();
		isResultSceneChange = true;
		eyeStart = Camera::target.v;
		eyeEnd = { 0, 100, 0 };
		eyeEaseTime = 0;
		resultForTime = 0;
	}
	//カメラの動き
	if (isResultSceneChange)
	{
		if (resultForTime >= 60)
		{
			Camera::target.v = EaseInQuad(eyeStart, eyeEnd, eyeEaseTime);
			eyeEaseTime += 0.02f;
			if (eyeEaseTime > 1.0f)
			{
				Camera::target.v = EaseInQuad(eyeStart, eyeEnd, 1.0f);
				resultForTime = 0;
				isResultSceneChange = false;
				SceneNum = RESULT;
			}
			Camera::Update();
		}
		else
		{
			resultForTime++;
		}
	}
	sky.Update();
	othelloStage.Update();
	Lights::Add(checkObject);
	Lights::Update();
}

void GameScene::ResultUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE))
	{
		SceneNum = TITLE;
	}
}

void GameScene::EndUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(DirectInput::ButtonKind::Button01))
	{
		SceneNum = TITLE;
		Camera::Init();
		Player::GetPlayer()->Init();
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
	ObjectParticles::Draw();
	Lights::Draw();
	//スプライトの描画-------------------------
	if (isSceneChange == false)
	{
		titleBack.ChangeSize(1280, 125);
		titleBack.position.m128_f32[0] = Imgui::spritePos1[0];
		titleBack.position.m128_f32[1] = Imgui::spritePos1[1];
		titleBack.SpriteDraw();
		title.SpriteDraw();
		space.SpriteDraw();
	}
	Imgui::DrawImGui();
	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::SelectDraw()
{
	//PostEffectのPreDraw
	postEffect.PreDraw();

	Player::GetPlayer()->Draw();

	//PostEffectのPostDraw
	postEffect.PostDraw();

	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffectのDraw
	postEffect.Draw();

	//スプライトの描画-------------------------
	//titleSprite.SpriteDraw();

	Imgui::DrawImGui();
	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::GameDraw()
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
	othelloManager.Draw();
	Draw3DObject(sky);
	Draw3DObject(othelloStage);
	ThunderModels::Draw();
	OthlloPlayer::Draw();
	ObjectParticles::Draw();
	ParticleControl::Draw();
	Lights::Draw();
	//スプライトの描画-------------------------
	//titleSprite.SpriteDraw();
	if (countDown > 0 && isSceneChange == false)
	{
		numbers[countDown / 60].position.m128_f32[0] = window_width / 2 - 10;
		numbers[countDown / 60].position.m128_f32[1] = window_height / 2 - 10;
		numbers[countDown / 60].SpriteDraw();
	}

	if (gameTime > 0 && countDown <= 0)
	{
		numbers[gameTime / 60].position.m128_f32[0] = window_width / 2 - 10;
		numbers[gameTime / 60].position.m128_f32[1] = 30;
		numbers[gameTime / 60].SpriteDraw();
	}
	Imgui::DrawImGui();
	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::ResultDraw()
{
	//PostEffectのPreDraw
	postEffect.PreDraw();

	Player::GetPlayer()->Draw();

	//PostEffectのPostDraw
	postEffect.PostDraw();

	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffectのDraw
	postEffect.Draw();

	//スプライトの描画-------------------------
	//titleSprite.SpriteDraw();
	scoreSprite.SpriteDraw();
	space.SpriteDraw();
	Imgui::DrawImGui();
	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::EndDraw()
{
	//PostEffectのPreDraw
	postEffect.PreDraw();

	Player::GetPlayer()->Draw();

	//PostEffectのPostDraw
	postEffect.PostDraw();

	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffectのDraw
	postEffect.Draw();

	//スプライトの描画-------------------------
	//titleSprite.SpriteDraw();

	Imgui::DrawImGui();
	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::LightUpdate()
{
	light->SetPointLightActive(0, false);
	light->SetSpotLightActive(0, Imgui::spotLight1);
	//light->SetCircleShadowActive(0, false);
	light->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	light->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));
	light->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	light->SetSpotLightDir(0, XMVECTOR({ spotLightDir[0], spotLightDir[1], spotLightDir[2], 0 }));
	light->SetSpotLightPos(0, XMFLOAT3(spotLightPos));
	light->SetSpotLightColor(0, XMFLOAT3(Imgui::lightColor[0], Imgui::lightColor[1], Imgui::lightColor[2]));
	light->SetSpotLightAtten(0, XMFLOAT3(spotLightAtten));
	light->SetSpotLightAngle(0, XMFLOAT2(spotLightAngle));
}
