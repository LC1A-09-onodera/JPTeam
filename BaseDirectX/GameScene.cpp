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
	title.CreateSprite(L"Resource/Img/titel.png", XMFLOAT3(60, 60, 0));
	space.CreateSprite(L"Resource/Img/PushSpace.png", XMFLOAT3(600, 600, 0));
	sceneChage.CreateSprite(L"Resource/Img/SceneChange.png", XMFLOAT3(window_width / 2, window_height / 2, 0));
	gameTime = gameMaxTime;
}

void GameScene::TitleUpdate()
{
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
		for (auto triangleItr = ObjectParticles::othello.particles.begin(); triangleItr != ObjectParticles::othello.particles.end(); ++triangleItr)
		{
			XMFLOAT3 pos = ConvertXMVECTORtoXMFLOAT3(triangleItr->each.position);
			ObjectParticles::triangle.Init(pos, 10, ParticleType::Exprotion);
			triangleItr->time = 1;
		}
		isSceneChange = true;
		eyeStart = Camera::target.v;
		eyeEnd = { 0.0f, 0.0f, 0.0f };
		eyeEaseTime = 0;
		gameTime = gameMaxTime;
		SceneNum = GAME;
	}
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
	if (!isSceneChange && !isResultSceneChange)
	{
		OthlloPlayer::Update();
		ThunderModels::Update();
		othelloManager.Controll();
		othelloManager.Update();
		checkObject.Update(othelloManager.Send());
		othelloManager.Receive(checkObject.GetOthelloDatas());

		gameTime--;
	}
	ObjectParticles::Update();
	if (isSceneChange)
	{
		Camera::target.v = EaseInQuad(eyeStart, eyeEnd, eyeEaseTime);
		eyeEaseTime += 0.02f;
		if (eyeEaseTime > 1.0f)
		{
			Camera::target.v = EaseInQuad(eyeStart, eyeEnd, 1.0f);
			//ObjectParticles::triangle.Init(XMFLOAT3(0, 0, 10), 100, ParticleType::Exprotion);
			isSceneChange = false;
			gameTime = gameMaxTime;
		}
		Camera::Update();
	}
	if (gameTime <= 0 && !isResultSceneChange)
	{
		for (auto triangleItr = OthelloManager::othellos.begin(); triangleItr != OthelloManager::othellos.end(); ++triangleItr)
		{
			XMFLOAT3 pos = triangleItr->GetPosition();
			ObjectParticles::triangle.Init(pos, 10, ParticleType::Exprotion);
			triangleItr->GetGameData()->isDead = true;

		}
		othelloManager.DeadPanel();
		isResultSceneChange = true;
		eyeStart = Camera::target.v;
		eyeEnd = { 0, 100, 0 };
		eyeEaseTime = 0;
		resultForTime = 0;
	}
	if (isResultSceneChange)
	{
		if (resultForTime >= 60)
		{
			Camera::target.v = EaseInQuad(eyeStart, eyeEnd, eyeEaseTime);
			eyeEaseTime += 0.02f;
			if (eyeEaseTime > 1.0f)
			{
				Camera::target.v = EaseInQuad(eyeStart, eyeEnd, 1.0f);
				//ObjectParticles::triangle.Init(XMFLOAT3(0, 0, 10), 100, ParticleType::Exprotion);
				//isSceneChange = false;
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
	postEffect.PreDraw();
	ObjectParticles::Draw();
	//Draw3DObject(sample);
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffectのDraw
	postEffect.Draw();

	//スプライトの描画-------------------------
	if (isSceneChange == false)
	{
		title.SpriteDraw();
		space.SpriteDraw();
	}Imgui::DrawImGui();
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
	postEffect.PreDraw();
	//Draw3DObject(sample);
	othelloManager.Draw();
	water.Draw();
	ThunderModels::Draw();
	OthlloPlayer::Draw();
	ObjectParticles::Draw();
	ParticleControl::Draw();

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
