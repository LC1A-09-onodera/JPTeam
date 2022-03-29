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

XMFLOAT3 GameScene::UpdateMousePosition()
{
	//スクリーン系
	POINT mouse = WindowsAPI::GetMousePos();
	XMFLOAT2 mouseFloat;
	mouseFloat = XMFLOAT2(mouse.x, mouse.y);
	//クリップ系
	mouseFloat.x = mouseFloat.x / (float)window_width;
	mouseFloat.y = mouseFloat.y / (float)window_height;
	mouseFloat.x = mouseFloat.x * 2.0f;
	mouseFloat.y = mouseFloat.y * 2.0f;
	mouseFloat.x = mouseFloat.x - 1.0f;
	mouseFloat.y = mouseFloat.y - 1.0f;
	mouseFloat.y = mouseFloat.y * -1.0f;
	//ビュー座標
	XMVECTOR mousePosition;
	XMMATRIX invProj, invView;
	invProj = XMMatrixInverse(nullptr, BaseDirectX::matProjection);
	invView = XMMatrixInverse(nullptr, Camera::matView);
	mousePosition = XMLoadFloat3(&XMFLOAT3(mouseFloat.x, mouseFloat.y, 0));
	mousePosition = XMVector3Transform(mousePosition, invProj);
	mousePosition = XMVector3Transform(mousePosition, invView);
	XMFLOAT3 result;
	result.x = mousePosition.m128_f32[0];
	result.y = mousePosition.m128_f32[1];
	result.z = mousePosition.m128_f32[2];
	//result.z = 0.0f;
	return result;
}

XMFLOAT3 GameScene::EyeToMouseVec()
{
	XMFLOAT3 mouse;
	mouse = UpdateMousePosition();
	XMFLOAT3 eyePos;
	eyePos = Camera::eye.v;
	XMFLOAT3 result;
	result = mouse - eyePos;
	return Normalize(result);
}

XMFLOAT3 GameScene::MousePosition(float z)
{
	XMFLOAT3 mouseVec = EyeToMouseVec();
	int count;
	count = z - Camera::eye.v.z / mouseVec.z;
	float x;
	x = mouseVec.x * count;
	float y;
	y = mouseVec.y * count;
	XMFLOAT3 result = { x, y, z };
	return result;
}

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
	Camera::eye = { 0, 0, -20.0 };
	Camera::target = { 0, 0, 0 };
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
	sample.CreateModel("oserro", ShaderManager::playerShader);
	sample.each.rotation.x = 90;
}

void GameScene::TitleUpdate()
{
	//WindowsAPI::GetMousePos();
	sample.each.position = ConvertXMFLOAT3toXMVECTOR(MousePosition(0.0f));
	sample.Update();
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
	//PostEffectのPreDraw
	postEffect.PreDraw();
	Draw3DObject(sample);
	
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
