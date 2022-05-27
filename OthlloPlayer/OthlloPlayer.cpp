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

bool OthlloPlayer::isStay = true;
bool OthlloPlayer::isRunNow = false;
bool OthlloPlayer::isReverse = false;

int OthlloPlayer::runSTime = 0;
int OthlloPlayer::runNTime = 0;
int OthlloPlayer::runETime = 0;
int OthlloPlayer::reverseTime = 0;

const int OthlloPlayer::MaxSTime = 60;
const int OthlloPlayer::MaxNTime = 60;
const int OthlloPlayer::MaxETime = 60;
const int OthlloPlayer::MaxReverseTime = 60;

float OthlloPlayer::easeRun1 = 0.0f;
float OthlloPlayer::easeRun2 = 0.0f;
float OthlloPlayer::easeSpace1 = 0.0f;
float OthlloPlayer::easeSpace2 = 0.0f;
float OthlloPlayer::easeStay1 = 0.0f;
float OthlloPlayer::easeStay2 = 0.0f;

float OthlloPlayer::easeRunAngleStart;
float OthlloPlayer::easeRunAngleEnd;
float OthlloPlayer::easeRunAngleConst = 30.0f;
bool OthlloPlayer::side;

float OthlloPlayer::easeStayStart;
float OthlloPlayer::easeStayEnd;
float OthlloPlayer::easeStayStartConst = -2.0f;
float OthlloPlayer::easeStayEndConst = -2.5f;

float OthlloPlayer::easeSpaceStartConst = -2.0f;
float OthlloPlayer::easeSpaceEndConst = -3.0f;

XMFLOAT3 OthlloPlayer::rotation = { 0, 0, 0 };
void OthlloPlayer::Init()
{
	playerFbx = FbxLoader::GetInstance()->LoadModelFromFile("player_stay");
	player.CreateModel("player", ShaderManager::playerShader);

	playerFbxObj = new FBXObject;
	playerFbxObj->Initialize();
	playerFbxObj->SetModel(playerFbx);
	playerFbxObj->rotation = { -30, 0, 0 };
	playerFbxObj->scale = { 0.008f, 0.008f, 0.008f };
	playerFbxObj->position = { 0, 0, -5 };
	startPos = { 0, 0, -2 };
	//playerFbxObj->PlayAnimation();
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	each.rotation = { 0, -90, 0 };
	each.scale = { 0.5f, 0.5f, 0.5f };
	each.position = { 0, 0, -2 ,1 };
	SoundLoad("Resource/Sound/move_.wav", moveSound);

	isStay = true;
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
	MotionUpdate();
	player.each.position = ConvertXMFLOAT3toXMVECTOR(playerFbxObj->position);
	player.each.position.m128_f32[3] = 1.0f;
	player.each.scale = {0.6f, 0.6f, 0.6f};
	player.Update();
	Draw3DObject(player);
}

void OthlloPlayer::Move()
{
	bool D = Input::KeyTrigger(DIK_D);
	bool A = Input::KeyTrigger(DIK_A);
	bool S = Input::KeyTrigger(DIK_S);
	bool W = Input::KeyTrigger(DIK_W);
	const float deadAngle = 0.0f;
	bool padD = directInput->IsButtonPush(DirectInput::ButtonKind::RightButton) && directInput->leftStickX() >= -deadAngle && directInput->leftStickX() <= deadAngle && directInput->leftStickY() <= deadAngle && directInput->leftStickY() >= -deadAngle;
	bool padS = directInput->IsButtonPush(DirectInput::ButtonKind::DownButton) && directInput->leftStickX() >= -deadAngle && directInput->leftStickX() <= deadAngle && directInput->leftStickY() <= deadAngle && directInput->leftStickY() >= -deadAngle;
	bool padA = directInput->IsButtonPush(DirectInput::ButtonKind::LeftButton) && directInput->leftStickX() >= -deadAngle && directInput->leftStickX() <= deadAngle && directInput->leftStickY() <= deadAngle && directInput->leftStickY() >= -deadAngle;
	bool padW = directInput->IsButtonPush(DirectInput::ButtonKind::UpButton) && directInput->leftStickX() >= -deadAngle && directInput->leftStickX() <= deadAngle && directInput->leftStickY() <= deadAngle && directInput->leftStickY() >= -deadAngle;

	if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(directInput->Button01))
	{
		isReverse = true;
		InitSpace();
	}

	if ((D || A || S || W || padD || padA || padW || padS) && !isEase)
	{
		SoundStopWave(moveSound);
		SoundPlayOnce(moveSound);
		playerFbxObj->PlayAnimation();
		isMoveEnd = false;
		startPos = playerFbxObj->position;
		endPos = startPos;

		InitRun();
		//isRunStart = true;
		//playerRunStartObject->PlayAnimation();
		runSTime = MaxSTime;

		if ((D || padD) && playerFbxObj->position.x < 6.0f)
		{
			endPos.x += MaxMoveAmount;
			side = true;
			rotation.z = -90;
		}
		else if ((A || padA) && playerFbxObj->position.x > -8.0f)
		{
			endPos.x -= MaxMoveAmount;
			rotation.z = 90;
			side = true;
		}
		else if ((S || padS) && playerFbxObj->position.y > -6.0f)
		{
			endPos.y -= MaxMoveAmount;
			rotation.z = 180;
			side = false;
		}
		else if ((W || padW) && playerFbxObj->position.y < 8.0f)
		{
			endPos.y += MaxMoveAmount;
			rotation.z = 0;
			side = false;
		}
		isEase = true;
		easeTime = 0;
	}


	if (isRunNow)
	{
		UpdateRun();
	}
	else if (isStay)
	{
		UpdateStay();
	}
	else if (isReverse)
	{
		UpdateSpace();
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
	XMFLOAT3 easeResult = ShlomonMath::EaseInQuad(startPos, endPos, easeTime);
	playerFbxObj->position.x = easeResult.x;
	playerFbxObj->position.y = easeResult.y;
}

void OthlloPlayer::MotionUpdate()
{

}

void OthlloPlayer::InitRun()
{
	easeRun1 = 0.0f;
	easeRun2 = 0.0f;
	isRunNow = true;
	isStay = false;
	isReverse = false;
	easeRunAngleStart = player.each.rotation.x;
	easeRunAngleEnd = easeRunAngleConst;
}

void OthlloPlayer::UpdateRun()
{
	if (easeRun1 < 1.0f)
	{
		easeRun1 += 0.15f;
		if (side)
		{
			player.each.rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, 0, 0), XMFLOAT3(0, easeRunAngleEnd, 0), easeRun1);
		}
		else
		{
			player.each.rotation = ShlomonMath::EaseInQuad(XMFLOAT3(easeRunAngleStart, 0, 0), XMFLOAT3(easeRunAngleEnd, 0, 0), easeRun1);
		}
	}
	else if (easeRun2 < 1.0f)
	{
		easeRun2 += 0.15f;
		if (side)
		{
			easeRunAngleStart = player.each.rotation.y;
		}
		else
		{
			easeRunAngleStart = player.each.rotation.x;
		}
		easeRunAngleEnd = 0;
		if (side)
		{
			player.each.rotation = ShlomonMath::EaseInQuad(XMFLOAT3(0, easeRunAngleStart, 0), XMFLOAT3(0, 0, 0), easeRun2);
		}
		else
		{
			player.each.rotation = ShlomonMath::EaseInQuad(XMFLOAT3(easeRunAngleStart, 0, 0), XMFLOAT3(easeRunAngleEnd, 0, 0), easeRun2);
		}
	}
	else
	{
		easeRun2 = 1.0f;
		easeRunAngleStart = player.each.rotation.x;
		easeRunAngleEnd = 0;
		player.each.rotation = ShlomonMath::EaseInQuad(XMFLOAT3(easeRunAngleStart, 0, 0), XMFLOAT3(easeRunAngleEnd, 0, 0), 1.0f);
		isRunNow = false;
		InitStay();
	}
}

void OthlloPlayer::InitStay()
{
	isStay = true;
	easeStay1 = 0.0f;
	easeStay2 = 0.0f;
	easeStayStart = player.each.position.m128_f32[2];
	easeStayEnd = easeStayEndConst;
	player.each.rotation = {0.0f, 0.0f, 0.0f};
}

void OthlloPlayer::UpdateStay()
{
	if (easeStay1 < 1.0f)
	{
		easeStay1 += 0.02f;
		playerFbxObj->position = Lerp(XMFLOAT3(0, 0, easeStayStartConst), XMFLOAT3(0, 0, easeStayEndConst), easeStay1);
	}
	else if (easeStay2 < 1.0f)
	{
		easeStay2 += 0.02f;
		playerFbxObj->position = Lerp(XMFLOAT3(0, 0, easeStayEndConst), XMFLOAT3(0, 0, easeStayStartConst), easeStay2);
	}
	else
	{
		InitStay();
	}
	player.each.rotation.z += 1.0f;
}

void OthlloPlayer::InitSpace()
{
	isReverse = true;
	isStay = false;
	isRunNow = false;
	easeSpace1 = 0.0f;
	easeSpace2 = 0.0f;
}

void OthlloPlayer::UpdateSpace()
{
	if (easeSpace1 < 1.0f)
	{
		easeSpace1 += 0.1f;
		playerFbxObj->position = Lerp(XMFLOAT3(0, 0, easeSpaceStartConst), XMFLOAT3(0, 0, easeSpaceEndConst), easeSpace1);
		player.each.rotation.z += 10.0f;
	}
	else if (easeSpace2 < 1.0f)
	{
		easeSpace2 += 0.1f;
		playerFbxObj->position = Lerp(XMFLOAT3(0, 0, easeSpaceEndConst), XMFLOAT3(0, 0, easeSpaceStartConst), easeSpace2);
		player.each.rotation.z += 5.0f;
	}
	else
	{
		isRunNow = false;
		InitStay();
	}
}

void OthlloPlayer::MoveCancel()
{
	isEase = false;
	isMoveEnd = false;
	playerFbxObj->position.x = startPos.x;
	playerFbxObj->position.y = startPos.y;
}