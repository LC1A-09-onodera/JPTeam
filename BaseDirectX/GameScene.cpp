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
#include "../Sound/Sound.h"
#include "../LoadStage/LoadStage.h"
#include "../LoadStage/StageObject.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	VoiceReciver::EndRecive();
}

void GameScene::SceneManageUpdateAndDraw()
{
	Input::Update(baseDirectX);
	WindowsAPI::CheckMsg();
	light->Update();
	switch (SceneNum)
	{
	case OP:
		OPUpdate();
		OPDraw();
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
	SceneNum = TITLE;
	baseDirectX.Set();
	//�T�E���h
	Sound::CreateVoice(baseDirectX);
	//SRV�̃A�h���X�m��
	baseDirectX.GetAdress();
	//�J����������
	Cameras::camera.Init(XMFLOAT3(0, 10, -15.0f), XMFLOAT3(0, 0, 0));
	Cameras::rCamera.Init(XMFLOAT3(0, -10, -15.0f), XMFLOAT3(0, 0, 0));
	Cameras::rCamera.isRCamera = true;
	//Imgui�̏�����
	Imgui::Init(baseDirectX);
	//���C�g�̏�����
	Light::StaticInitialize(baseDirectX.dev.Get());
	//�V�F�[�_�[�̃��[�h
	ShaderManager::LoadShaders();
	// 3D�p�[�e�B�N���ÓI������
	ParticleControl::Init(baseDirectX);
	//�C���v�b�g������
	Input::KeySet(baseDirectX, WindowsAPI::w, WindowsAPI::hwnd);
	//FBX�n
	/*FbxLoader::GetInstance()->Initialize(baseDirectX.dev.Get());
	FBXObject::SetDevice(baseDirectX.dev.Get());
	FBXObject::CreateGraphicsPipeline();*/
	//���C�g������
	light.reset(Light::Create());
	//���f�����ׂĂɃ��C�g��K�p
	Model::SetLight(light);
	//�|�X�g�G�t�F�N�g�̏�����
	PostEffects::Init(baseDirectX);
	//Rewired�̗v�f������
	Rewired::KeyCodeString::KeyCodeStringInit();
	//�X�e�[�W���e�L�X�g���烍�[�h
	LoadStage::LoadStages("Resource/TextData/Stage/stage1.txt");

	StageObjects::LoadModel(baseDirectX);

	seling.LoadModel(baseDirectX);
	rSeling.LoadModel(baseDirectX);

	//�{�C�X�R�}���h�̒ʐM��t�X�^�[�g
	//���M���͂܂��Ⴄ�A�v���P�[�V�����ōs��
	VoiceReciver::StartUp();

	waterFace.LoadModel(baseDirectX, ShaderManager::waterShader, PostEffects::postNormal);
	normalWater.LoadModel(baseDirectX, ShaderManager::normalPlaneShader, PostEffects::postNormal);
	const float worldSize = 40.0f;
	world.CreateModel(baseDirectX, "SphereW", ShaderManager::playerShader);
	world.each.scale = { worldSize, worldSize, worldSize };
	rWorld.CreateModel(baseDirectX, "SphereW", ShaderManager::playerShader);
	rWorld.each.scale = { worldSize, worldSize, worldSize };
	skyDome.CreateModel(baseDirectX, "sky", ShaderManager::playerShader, true);
	const float skyDomeSize = 60.0f;
	skyDome.each.scale = { skyDomeSize , skyDomeSize , skyDomeSize };

	spaceSp.CreateSprite(baseDirectX, L"Resource/Image/space.png", XMFLOAT3(window_width / 2.0f - 100.0f, 600.0f, 0));

	for (int i = 0; i < 10; i++)
	{
		string path = "Resource/Image/Numbers/Num" + to_string(i) + ".png";
		numTex[i].LoadGraph(baseDirectX, ConvertStringToWchaer(path));
	}
	for (int i = 0; i < 10; i++)
	{
		resultNumSprite[i].CreateSprite(baseDirectX, numTex[i], XMFLOAT3(window_width / 2.0f - 100.0f, 600.0f, 0));
		resultNumSprite[i + 10].CreateSprite(baseDirectX, numTex[i], XMFLOAT3(window_width / 2.0f - 100.0f, 600.0f, 0));
		resultNumSprite[i + 20].CreateSprite(baseDirectX, numTex[i], XMFLOAT3(window_width / 2.0f - 100.0f, 600.0f, 0));
		resultNumSprite[i + 30].CreateSprite(baseDirectX, numTex[i], XMFLOAT3(window_width / 2.0f - 100.0f, 600.0f, 0));
	}
}

void GameScene::TitleUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE))
	{
		SceneNum = GAME;
	}
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
	seling.Update();
	rSeling.Update();
	waterFace.Update();
	normalWater.Update();
	
	Cameras::camera.target = ConvertXMVECTORtoXMFLOAT3(seling.seling.each.position);
	Cameras::camera.Update();
	Cameras::rCamera.eye.x = 0;
	Cameras::rCamera.eye.y = -60.0f;
	Cameras::rCamera.eye.z = -5.0f;
	Cameras::rCamera.up = { 0, 1, 0 };
	Cameras::rCamera.Update();

	if (seling.GetIsGoal())
	{
		SceneNum = RESULT;
	}

	VoiceReciver::VoiceUDPUpdate(baseDirectX);
	Sound::Updete(Imgui::volume);
}

void GameScene::ResultUpdate()
{
	seling.Update();
	rSeling.Update();
	waterFace.Update();
	normalWater.Update();

	//�J�����̃C�[�W���O���s���@
	Imgui::CameraRotation = ShlomonMath::EaseInOutQuad(XMFLOAT3(Imgui::CameraRotation, 0 ,0) , XMFLOAT3(350.0f, 0, 0), 0.1f).x;
	Cameras::camera.target = ConvertXMVECTORtoXMFLOAT3(seling.seling.each.position);
	Cameras::camera.Update();
	Cameras::rCamera.eye.x = 0;
	Cameras::rCamera.eye.y = -60.0f;
	Cameras::rCamera.eye.z = -5.0f;
	Cameras::rCamera.up = { 0, 1, 0 };
	Cameras::rCamera.Update();

	if (Input::KeyTrigger(DIK_SPACE))
	{
		SceneNum = TITLE;
		seling.Init();
		rSeling.Init();
		Imgui::CameraRotation = 270.0f;
	}

	VoiceReciver::VoiceUDPUpdate(baseDirectX);
	Sound::Updete(Imgui::volume);
}

void GameScene::OPUpdate()
{
	if (opAnimationTime > MaxOPAnimationTime)
	{
		SceneNum = TITLE;
	}
	opAnimationTime++;
}

void GameScene::EndUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE) || Input::directInput->IsButtonPush(DirectInput::ButtonKind::Button01))
	{
		SceneNum = TITLE;
		Cameras::camera.Init(XMFLOAT3(0, 10, -15.0f), XMFLOAT3(0, 0, 0));
	}
}

void GameScene::PreWaterFaceDraw()
{
	bool isRDraw;
	if (Imgui::effectType < 0)
	{
		isRDraw = true;
	}
	else
	{
		isRDraw = false;
	}
	light->SetLightDir(XMFLOAT3(Cameras::rCamera.GetTargetDirection()));
	LightUpdate();
	rSeling.Draw(baseDirectX, isRDraw);
	rWorld.Update(baseDirectX, &rWorld.each, isRDraw);
	Draw3DObject(baseDirectX, rWorld);
	StageObjects::Draw(baseDirectX, isRDraw);

}

void GameScene::PostWaterFaceDraw()
{
	light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
	LightUpdate();
	seling.Draw(baseDirectX);
	world.Update(baseDirectX, &world.each, false);
	Draw3DObject(baseDirectX, world);
	skyDome.Update(baseDirectX, &skyDome.each);
	Draw3DObject(baseDirectX, skyDome);
	StageObjects::Draw(baseDirectX, false);
}

void GameScene::DrawSprites()
{
	switch (SceneNum)
	{
	case OP:
		break;
	case TITLE:
		spaceSp.SpriteDraw(baseDirectX);
		break;

	}
	
}

void GameScene::TitleDraw()
{
	baseDirectX.UpdateFront();
	

	DrawSprites();
	Imgui::DrawImGui(baseDirectX);
	//�`��R�}���h�����܂�
	baseDirectX.UpdateBack();
}

void GameScene::SelectDraw()
{
	baseDirectX.UpdateFront();
	
	DrawSprites();
	Imgui::DrawImGui(baseDirectX);
	//�`��R�}���h�����܂�
	baseDirectX.UpdateBack();
}

void GameScene::GameDraw()
{
	//PostEffect��PreDraw
	PostEffects::PreDraw(baseDirectX);

	PreWaterFaceDraw();

	baseDirectX.UpdateFront();

	PostWaterFaceDraw();

	XMVECTOR waterFacePosition = { 0, -0.5f, 0.0f, 1.0 };
	PostEffect waterFaceTarget;
	if (PostEffects::type == PostEffects::PostEffectType::Normal)
	{
		waterFaceTarget = PostEffects::postNormal;

	}
	else if (PostEffects::type == PostEffects::PostEffectType::Water)
	{
		waterFaceTarget = PostEffects::postWater;
	}
	else if (PostEffects::type == PostEffects::PostEffectType::Mosaic)
	{
		waterFaceTarget = PostEffects::postMosaic;
	}
	else if (PostEffects::type == PostEffects::PostEffectType::Blur)
	{
		waterFaceTarget = PostEffects::postBlur;
	}
	else
	{
		waterFaceTarget = PostEffects::postNormal;
	}
	if (Imgui::useWaterNum == 0)
	{
		waterFace.Draw(baseDirectX, waterFaceTarget, waterFacePosition);
	}
	else if (Imgui::useWaterNum == 1)
	{
		normalWater.Draw(baseDirectX, waterFaceTarget, waterFacePosition);
	}

	PostEffects::Draw(baseDirectX);

	DrawSprites();

	PostEffects::PostDraw(baseDirectX);
	Imgui::DrawImGui(baseDirectX);
	//�`��R�}���h�����܂�
	baseDirectX.UpdateBack();
}

void GameScene::ResultDraw()
{
	//PostEffect��PreDraw
	PostEffects::PreDraw(baseDirectX);

	PreWaterFaceDraw();

	baseDirectX.UpdateFront();

	PostWaterFaceDraw();

	XMVECTOR waterFacePosition = { 0, -0.5f, 0.0f, 1.0 };
	PostEffect waterFaceTarget;
	if (PostEffects::type == PostEffects::PostEffectType::Normal)
	{
		waterFaceTarget = PostEffects::postNormal;

	}
	else if (PostEffects::type == PostEffects::PostEffectType::Water)
	{
		waterFaceTarget = PostEffects::postWater;
	}
	else if (PostEffects::type == PostEffects::PostEffectType::Mosaic)
	{
		waterFaceTarget = PostEffects::postMosaic;
	}
	else if (PostEffects::type == PostEffects::PostEffectType::Blur)
	{
		waterFaceTarget = PostEffects::postBlur;
	}
	else
	{
		waterFaceTarget = PostEffects::postNormal;
	}
	if (Imgui::useWaterNum == 0)
	{
		waterFace.Draw(baseDirectX, waterFaceTarget, waterFacePosition);
	}
	else if (Imgui::useWaterNum == 1)
	{
		normalWater.Draw(baseDirectX, waterFaceTarget, waterFacePosition);
	}

	PostEffects::Draw(baseDirectX);

	DrawSprites();

	PostEffects::PostDraw(baseDirectX);
	Imgui::DrawImGui(baseDirectX);
	//�`��R�}���h�����܂�
	baseDirectX.UpdateBack();
}

void GameScene::OPDraw()
{
}

void GameScene::EndDraw()
{
	//PostEffect��PreDraw
	PostEffects::PreDraw(baseDirectX);

	baseDirectX.UpdateFront();
	//PostEffect��Draw
	PostEffects::Draw(baseDirectX);
	PostEffects::PostDraw(baseDirectX);
	Imgui::DrawImGui(baseDirectX);

	//�`��R�}���h�����܂�
	baseDirectX.UpdateBack();
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
