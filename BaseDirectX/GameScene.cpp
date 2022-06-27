#include "../VoiceUDPRecive/VoiceUDPRecive.h"
#include"GameScene.h"
#include "BaseDirectX.h"
#include "../WindowsAPI/WinAPI.h"
#include "Input.h"
#include "viewport.h"
#include "../Particle/Particle3D.h"
#include "../imgui/ImguiControl.h"
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
	Cameras::camera.Init(XMFLOAT3(0, 10, -15.0f), XMFLOAT3(0, 0, 0));
	Cameras::rCamera.Init(XMFLOAT3(0, -10, -15.0f), XMFLOAT3(0, 0, 0));
	//Imgui�̏�����
	Imgui::Init();
	//���C�g�̏�����
	Light::StaticInitialize(BaseDirectX::dev.Get());
	//�V�F�[�_�[�̃��[�h
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
	Model::SetLight(light);
	//�|�X�g�G�t�F�N�g�̏�����
	PostEffects::Init();
	//3D�I�u�W�F�N�g�̃p�[�e�B�N�����[�h
	ObjectParticles::LoadModels();
	//Rewired�̗v�f������
	Rewired::KeyCodeString::KeyCodeStringInit();
	
	jumpKey.LoadKey("RewiredTest.txt");
	
	//�X�e�[�W���e�L�X�g���烍�[�h
	LoadStage::LoadStages("test.txt");
	StageObjects::walls.wallModel.CreateModel("MapWall", ShaderManager::playerShader);
	StageObjects::walls.LoadPosition();
	StageObjects::goals.goalModel.CreateModel("goal", ShaderManager::playerShader);
	StageObjects::goals.LoadPosition();
	seling.LoadModel();
	seling.Init();
	rSeling.LoadModel();
	rSeling.Init();

	//�{�C�X�R�}���h�̒ʐM��t�X�^�[�g
	//���M���͂܂��Ⴄ�A�v���P�[�V�����ōs��
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
	Cameras::rCamera.isRCamera = true;
}

void GameScene::TitleUpdate()
{
	Cameras::camera.Update();
	Cameras::rCamera.eye.x = Cameras::camera.eye.x;
	Cameras::rCamera.eye.y = -Cameras::camera.eye.y;
	Cameras::rCamera.eye.z = Cameras::camera.eye.z;
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
	Sound::Updete(Imgui::volume);
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
		Cameras::camera.Init(XMFLOAT3(0, 10, -15.0f), XMFLOAT3(0, 0, 0));
	}
}

void GameScene::TitleDraw()
{
	//PostEffect��PreDraw
	PostEffects::PreDraw();

	light->SetLightDir(XMFLOAT3(Cameras::rCamera.GetTargetDirection()));
	LightUpdate();
	rSeling.Draw(true);
	rWorld.Update(&rWorld.each, true);
	Draw3DObject(rWorld);
	StageObjects::Draw(true);
	ObjectParticles::Draw();

	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();

	//PostEffect��Draw
	//PostEffects::Draw();

	light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
	LightUpdate();

	seling.Draw();
	//world.each.rotation.y = 180;
	world.Update(&world.each, false);
	StageObjects::Draw(false);
	Draw3DObject(world);
	ObjectParticles::Draw();

	XMVECTOR sample = { 0, -3.0f, 0.0f, 1.0 };
	if (Imgui::useWaterNum == 0)
	{
		//waterFace.waterModel.each.rotation.y = 180;
		waterFace.Draw(PostEffects::postNormal, sample/*seling.seling.each.position*/);
	}
	else if (Imgui::useWaterNum == 1)
	{
		//normalWater.waterModel.each.rotation.y = 180;
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
