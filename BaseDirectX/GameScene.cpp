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
	//�T�E���h
	Sound::CreateVoice();
	//SRV�̃A�h���X�m��
	BaseDirectX::GetAdress();
	//�J����������
	Camera::Init();
	Camera::eye = { 0, 0, -20.0 };
	Camera::target = { 0, 0, 0 };
	Camera::Update();
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
	light = Light::Create();
	//���f�����ׂĂɃ��C�g��K�p
	Model::SetLight(light);
	//�v���C���[�̏�����
	Player::GetPlayer()->Init();
	
	//�|�X�g�G�t�F�N�g�̏�����
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
	ObjectParticle3D::LoadObject();

	water.CreateWater();
}

void GameScene::TitleUpdate()
{
	//WindowsAPI::GetMousePos();
	//sample.each.position = ConvertXMFLOAT3toXMVECTOR(Camera::MousePosition(0.0f));
	//sample.Update();

	othelloManager.Controll();
	othelloManager.Update();

	checkObject.Update(othelloManager.Send());
	othelloManager.Receive(checkObject.GetOthelloDatas());

	ObjectParticles::Update();

	/*if (Input::KeyTrigger(DIK_SPACE))
	{
		SceneNum = GAME;
		Camera::eye.v.y = 20;
		Camera::eye.v.z = -10;
	}*/
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
	Camera::eye.v.z = Player::GetPlayer()->GetPos().z - 10.0f;
	GameScene::LightUpdate();
	Camera::target.v = XMFLOAT3(Player::GetPlayer()->GetPos());
	Camera::Update();	spotLightPos[0] = Player::GetPlayer()->GetPos().x;
	spotLightPos[2] = Player::GetPlayer()->GetPos().z;
	spotLightPos[1] = 3;


	//Player::GetPlayer()->Update();
	ParticleControl::Update();

	if (Input::KeyTrigger(DIK_R))
	{
		SceneNum = SELECT;
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
	//PostEffect��PreDraw
	postEffect.PreDraw();
	//Draw3DObject(sample);
	othelloManager.Draw();
	water.Draw();
	ObjectParticles::Draw();
	ParticleControl::Draw();
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffect��Draw
	postEffect.Draw();

	//�X�v���C�g�̕`��-------------------------
	//titleSprite.SpriteDraw();
	Imgui::DrawImGui();
	//�`��R�}���h�����܂�
	BaseDirectX::UpdateBack();
}

void GameScene::SelectDraw()
{
	//PostEffect��PreDraw
	postEffect.PreDraw();

	Player::GetPlayer()->Draw();

	//PostEffect��PostDraw
	postEffect.PostDraw();

	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffect��Draw
	postEffect.Draw();

	//�X�v���C�g�̕`��-------------------------
	//titleSprite.SpriteDraw();

	Imgui::DrawImGui();
	//�`��R�}���h�����܂�
	BaseDirectX::UpdateBack();
}

void GameScene::GameDraw()
{
	//PostEffect��PreDraw
	postEffect.PreDraw();

	Player::GetPlayer()->Draw();

	//PostEffect��PostDraw
	postEffect.PostDraw();

	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffect��Draw
	postEffect.Draw();

	//�X�v���C�g�̕`��-------------------------
	//titleSprite.SpriteDraw();

	Imgui::DrawImGui();
	//�`��R�}���h�����܂�
	BaseDirectX::UpdateBack();
}

void GameScene::ResultDraw()
{
	//PostEffect��PreDraw
	postEffect.PreDraw();

	Player::GetPlayer()->Draw();

	//PostEffect��PostDraw
	postEffect.PostDraw();

	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffect��Draw
	postEffect.Draw();

	//�X�v���C�g�̕`��-------------------------
	//titleSprite.SpriteDraw();

	Imgui::DrawImGui();
	//�`��R�}���h�����܂�
	BaseDirectX::UpdateBack();
}

void GameScene::EndDraw()
{
	//PostEffect��PreDraw
	postEffect.PreDraw();

	Player::GetPlayer()->Draw();

	//PostEffect��PostDraw
	postEffect.PostDraw();

	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffect��Draw
	postEffect.Draw();

	//�X�v���C�g�̕`��-------------------------
	//titleSprite.SpriteDraw();

	Imgui::DrawImGui();
	//�`��R�}���h�����܂�
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
