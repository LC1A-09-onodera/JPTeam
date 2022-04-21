#include "OthlloPlayer.h"
#include "../BaseDirectX/Input.h"
#include "../Shader/ShaderManager.h"
using namespace ConstOthlloPlayer;

Model OthlloPlayer::player;
EachInfo OthlloPlayer::each;
SoundData OthlloPlayer::moveSound;

XMFLOAT3 OthlloPlayer::startPos;
XMFLOAT3 OthlloPlayer::endPos;
bool OthlloPlayer::isEase;
bool OthlloPlayer::isMoveEnd;
float OthlloPlayer::easeTime;
FBXModel* OthlloPlayer::playerFbx;
FBXObject* OthlloPlayer::playerFbxObj;
void OthlloPlayer::Init()
{
	player.CreateModel("player", ShaderManager::playerShader);
	playerFbx = FbxLoader::GetInstance()->LoadModelFromFile("player");
	playerFbxObj = new FBXObject;
	playerFbxObj->Initialize();
	playerFbxObj->SetModel(playerFbx);
	playerFbxObj->rotation = { 0, 180, -90 };
	playerFbxObj->scale = { 0.5f, 0.5f, 0.5f };
	playerFbxObj->position = { 0, 0, -2 };
	startPos = { 0, 0, -2 };
	//playerFbxObj->PlayAnimation();
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	each.rotation = { 0, -90, 90 };
	each.scale = { 0.5f, 0.5f, 0.5f };
	each.position = { 0, 0, -2 ,1 };
	SoundLoad("Resource/Sound/playerMoveSE_.wav", moveSound);
}

void OthlloPlayer::Update()
{
	Move();
	if (isEase)
	{
		EaseUpdate();
	}
	else
	{
		isMoveEnd = false;
	}
}

void OthlloPlayer::Draw()
{
	//player.Update(&each);
	playerFbxObj->Update();
	playerFbxObj->Draw(BaseDirectX::cmdList.Get());
	//Draw3DObject(player);
}

void OthlloPlayer::Move()
{
	bool D = Input::KeyTrigger(DIK_D);
	bool A = Input::KeyTrigger(DIK_A);
	bool S = Input::KeyTrigger(DIK_S);
	bool W = Input::KeyTrigger(DIK_W);
	bool padD = directInput->IsButtonPush(DirectInput::ButtonKind::RightButton);
	bool padS = directInput->IsButtonPush(DirectInput::ButtonKind::DownButton);
	bool padA = directInput->IsButtonPush(DirectInput::ButtonKind::LeftButton);
	bool padW = directInput->IsButtonPush(DirectInput::ButtonKind::UpButton);

	if ((D || A || S || W || padD || padA || padW || padS) && !isEase)
	{
		SoundPlayOnce(moveSound);
		isMoveEnd = false;
		startPos = playerFbxObj->position;
		endPos = startPos;
		if ((D || padD) && each.position.m128_f32[0] < 6.0f)
		{
			endPos.x += MaxMoveAmount;
		}
		else if ((A || padA) && each.position.m128_f32[0] > -8.0f)
		{
			endPos.x -= MaxMoveAmount;
		}
		else if ((S || padS) && each.position.m128_f32[1] > -6.0f)
		{
			endPos.y -= MaxMoveAmount;
		}
		else if ((W || padW) && each.position.m128_f32[1] < 8.0f)
		{
			endPos.y += MaxMoveAmount;
		}
		isEase = true;
		easeTime = 0;
	}
}

void OthlloPlayer::EaseUpdate()
{
	easeTime += 0.15f;
	if (easeTime >= 1.0f)
	{
		easeTime = 1.0f;
		isEase = false;
		isMoveEnd = true;
	}
	each.position = ConvertXMFLOAT3toXMVECTOR(ShlomonMath::EaseInQuad(startPos, endPos, easeTime));
	playerFbxObj->position = ShlomonMath::EaseInQuad(startPos, endPos, easeTime);
}

void OthlloPlayer::MoveCancel()
{
	isEase = false;
	isMoveEnd = false;
	playerFbxObj->position = startPos;
}