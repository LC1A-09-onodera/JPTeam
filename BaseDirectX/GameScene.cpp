#include "../VoiceUDPRecive/VoiceUDPRecive.h"
#include"GameScene.h"
#include "BaseDirectX.h"
#include "../WindowsAPI/WinAPI.h"
#include "Input.h"
#include "viewport.h"
#include "../Particle/Particle3D.h"
#include "../imgui/ImguiControl.h"
#include "vec3.h"
#include "../FBXObject/FBXObject.h"
#include "../Shader/ShaderManager.h"
#include "../3DObjectParticle/3DObjectParticle.h"
#include "../Sound/Sound.h"
#include "../LoadStage/LoadStage.h"
#include "../LoadStage/StageObject.h"

GameScene::GameScene()
{
	SceneNum = TITLE;
}

GameScene::~GameScene()
{
	VoiceReciver::EndRecive();
	
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
	//�T�E���h
	Sound::CreateVoice();
	//SRV�̃A�h���X�m��
	BaseDirectX::GetAdress();
	//�J����������
	Cameras::camera.Init();
	Cameras::camera.eye = { 0, 10, -15.0f };
	Cameras::camera.target = { 0, 0, 0 };
	Cameras::camera.Update();

	Cameras::rCamera.Init();
	Cameras::rCamera.eye = { 0, 0, -15.0f };
	Cameras::rCamera.target = { 0, 0, 0 };
	Cameras::rCamera.Update();
	//Imgui�̏�����
	Imgui::Init();
	//���C�g�̏�����
	Light::StaticInitialize(BaseDirectX::dev.Get());
	ShaderManager::LoadShaders();
	// 3D�p�[�e�B�N���ÓI������
	ParticleControl::Init();
	//�C���v�b�g������
	Input::KeySet(WindowsAPI::w, WindowsAPI::hwnd);
	//FBX�n
	FbxLoader::GetInstance()->Initialize(BaseDirectX::dev.Get());
	FBXObject::SetDevice(BaseDirectX::dev.Get());
	FBXObject::CreateGraphicsPipeline();
	//���C�g������
	light.reset(Light::Create());
	//���f�����ׂĂɃ��C�g��K�p
	Model::SetLight(light.get());
	//�|�X�g�G�t�F�N�g�̏�����
	PostEffects::Init();
	ObjectParticles::LoadModels();
	Rewired::KeyCodeString::KeyCodeStringInit();
	
	jumpKey.LoadKey("RewiredTest.txt");
	
	LoadStage::LoadStages("test.txt");
	StageObjects::walls.wallModel.CreateModel("MapWall", ShaderManager::playerShader);
	StageObjects::walls.LoadPosition();
	StageObjects::goals.goalModel.CreateModel("goal", ShaderManager::playerShader);
	StageObjects::goals.LoadPosition();
	seling.LoadModel();
	seling.Init();
	rSeling.LoadModel();
	rSeling.Init();

	VoiceReciver::StartUp();
	//EnemyModels::LoadModels();

	waterFace.LoadModel(ShaderManager::waterShader, PostEffects::postNormal);
	waterFace.Init();
	normalWater.LoadModel(ShaderManager::normalPlaneShader, PostEffects::postNormal);
	normalWater.Init();
	world.CreateModel("SphereW", ShaderManager::playerShader);
	world.each.scale = { 40.0f, 40.0f, 40.0f };
	rWorld.CreateModel("SphereW", ShaderManager::playerShader);
	rWorld.each.scale = { 40.0f, 40.0f, 40.0f };
}

void GameScene::TitleUpdate()
{
	Cameras::camera.Update();
	Cameras::rCamera.eye.v.x = Cameras::camera.eye.v.x;
	Cameras::rCamera.eye.v.y = -Cameras::camera.eye.v.y;
	Cameras::rCamera.eye.v.z = Cameras::camera.eye.v.z;
	Cameras::rCamera.target = Cameras::camera.target;
	Cameras::rCamera.Update();

	seling.Update();
	rSeling.Update();
	waterFace.Update();
	normalWater.Update();

	if (jumpKey.GetKeyDown())
	{
		int a = 0;
	}

	VoiceReciver::VoiceUDPUpdate();
	ObjectParticles::Update();
	LightUpdate();
	Sound::Updete(Imgui::volume);
	light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
	LightUpdate();
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

}

void GameScene::ResultUpdate()
{

}

void GameScene::EndUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE) || Input::directInput->IsButtonPush(DirectInput::ButtonKind::Button01))
	{
		SceneNum = TITLE;
		Cameras::camera.Init();
	}
}

void GameScene::TitleDraw()
{
	//PostEffect��PreDraw
	PostEffects::PreDraw();
	//seling.Draw(true);
	rSeling.seling.each.position = { -seling.seling.each.position.m128_f32[0], -seling.seling.each.position.m128_f32[1], seling.seling.each.position.m128_f32[2], 1.0f };
	rSeling.seling.each.rotation.x = 180;
	rSeling.Draw(true);
	rWorld.each.rotation.x = 180;
	rWorld.Update();
	Draw3DObject(rWorld);
	ObjectParticles::Draw();

	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();

	//PostEffect��Draw
	//PostEffects::Draw();
	seling.Draw(true);
	world.each.rotation.y = 180;
	world.Update();
	StageObjects::Draw();
	Draw3DObject(world);
	ObjectParticles::Draw();
	XMVECTOR sample = { 0, 0, 2.0f, 1.0 };
	if (Imgui::useWaterNum == 0)
	{
		waterFace.Draw(PostEffects::postNormal, sample/*seling.seling.each.position*/);
	}
	else if (Imgui::useWaterNum == 1)
	{
		normalWater.Draw(PostEffects::postNormal, sample/*seling.seling.each.position*/);
	}
	PostEffects::PostDraw();

	Imgui::DrawImGui();
	//�`��R�}���h�����܂�
	BaseDirectX::UpdateBack();
}

void GameScene::SelectDraw()
{
	//PostEffect��PreDraw
	PostEffects::PreDraw();
	Draw3DObject(sample);
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffect��Draw
	PostEffects::Draw();
	PostEffects::PostDraw();

	Imgui::DrawImGui();
	//�`��R�}���h�����܂�
	BaseDirectX::UpdateBack();
}

void GameScene::GameDraw()
{
	//PostEffect��PreDraw
	PostEffects::PreDraw();
	Draw3DObject(sample);
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffect��Draw
	PostEffects::Draw();
	PostEffects::PostDraw();
	Imgui::DrawImGui();

	//�`��R�}���h�����܂�
	BaseDirectX::UpdateBack();
}

void GameScene::ResultDraw()
{
	//PostEffect��PreDraw
	PostEffects::PreDraw();
	Draw3DObject(sample);
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffect��Draw
	PostEffects::Draw();
	PostEffects::PostDraw();
	Imgui::DrawImGui();

	//�`��R�}���h�����܂�
	BaseDirectX::UpdateBack();
}

void GameScene::EndDraw()
{
	//PostEffect��PreDraw
	PostEffects::PreDraw();
	Draw3DObject(sample);
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffect��Draw
	PostEffects::Draw();
	PostEffects::PostDraw();
	Imgui::DrawImGui();

	//�`��R�}���h�����܂�
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
