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


FBXModel* OthlloPlayer::playerStay;
FBXObject* OthlloPlayer::playerStayObject;
FBXModel* OthlloPlayer::playerRunStart;
FBXObject* OthlloPlayer::playerRunStartObject;
FBXModel* OthlloPlayer::playerRunNow;
FBXObject* OthlloPlayer::playerRunNowObject;
FBXModel* OthlloPlayer::playerRunEnd;
FBXObject* OthlloPlayer::playerRunEndObject;
FBXModel* OthlloPlayer::playerReverse;
FBXObject* OthlloPlayer::playerReverseObject;

bool OthlloPlayer::isStay = true;
bool OthlloPlayer::isRunStart = false;
bool OthlloPlayer::isRunNow = false;
bool OthlloPlayer::isRunEnd = false;
bool OthlloPlayer::isReverse = false;

int OthlloPlayer::runSTime = 0;
int OthlloPlayer::runNTime = 0;
int OthlloPlayer::runETime = 0;
int OthlloPlayer::reverseTime = 0;

const int OthlloPlayer::MaxSTime = 60;
const int OthlloPlayer::MaxNTime = 60;
const int OthlloPlayer::MaxETime = 60;
const int OthlloPlayer::MaxReverseTime = 60;

XMFLOAT3 OthlloPlayer::rotation = {0, 0, 0};
void OthlloPlayer::Init()
{
	player.CreateModel("player", ShaderManager::playerShader);
	playerFbx = FbxLoader::GetInstance()->LoadModelFromFile("player_stay");

	playerStay = FbxLoader::GetInstance()->LoadModelFromFile("player_stay");
	playerRunStart = FbxLoader::GetInstance()->LoadModelFromFile("player_move_start");
	playerRunNow = FbxLoader::GetInstance()->LoadModelFromFile("player_move_now");
	playerRunEnd = FbxLoader::GetInstance()->LoadModelFromFile("player_move_end");
	playerReverse = FbxLoader::GetInstance()->LoadModelFromFile("player_reverse");

	playerStayObject = new FBXObject;
	playerRunStartObject = new FBXObject;
	playerRunNowObject = new FBXObject;
	playerRunEndObject = new FBXObject;
	playerReverseObject = new FBXObject;

	playerStayObject->Initialize();
	playerRunStartObject->Initialize();
	playerRunNowObject->Initialize(2);
	playerRunEndObject->Initialize();
	playerReverseObject->Initialize(2);

	playerStayObject->SetModel(playerStay);
	playerRunStartObject->SetModel(playerRunStart);
	playerRunNowObject->SetModel(playerRunNow);
	playerRunEndObject->SetModel(playerRunEnd);
	playerReverseObject->SetModel(playerReverse);

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

	if (isRunStart)
	{
		playerRunStartObject->Draw(BaseDirectX::cmdList.Get());
	}
	else if (isRunNow)
	{
		playerRunNowObject->Draw(BaseDirectX::cmdList.Get());
	}
	else if (isRunEnd)
	{
		playerRunEndObject->Draw(BaseDirectX::cmdList.Get());
	}
	else if (isReverse)
	{
		playerReverseObject->Draw(BaseDirectX::cmdList.Get());
	}
	else
	{
		playerStayObject->Draw(BaseDirectX::cmdList.Get());
	}
	//playerFbxObj->Update();
	//playerFbxObj->Draw(BaseDirectX::cmdList.Get());
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
	
	if (Input::KeyTrigger(DIK_SPACE))
	{
		isReverse = true;
		isRunStart = false;
		isRunEnd = false;
		isRunNow = false;
		isStay = false;
		playerReverseObject->PlayAnimation();
	}

	if ((D || A || S || W || padD || padA || padW || padS) && !isEase)
	{
		SoundStopWave(moveSound);
		SoundPlayOnce(moveSound);
		playerFbxObj->PlayAnimation();
		isMoveEnd = false;
		startPos = playerFbxObj->position;
		endPos = startPos;

		isRunNow = true;
		//isRunStart = true;
		//playerRunStartObject->PlayAnimation();
		playerRunNowObject->PlayAnimation();
		runSTime = MaxSTime;

		if ((D || padD) && each.position.m128_f32[0] < 6.0f)
		{
			endPos.x += MaxMoveAmount;
			rotation.z = -90;
		}
		else if ((A || padA) && each.position.m128_f32[0] > -8.0f)
		{
			endPos.x -= MaxMoveAmount;
			rotation.z = 90;
		}
		else if ((S || padS) && each.position.m128_f32[1] > -6.0f)
		{
			endPos.y -= MaxMoveAmount;
			rotation.z = 180;
		}
		else if ((W || padW) && each.position.m128_f32[1] < 8.0f)
		{
			endPos.y += MaxMoveAmount;
			rotation.z = 0;
		}
		isEase = true;
		easeTime = 0;
	}

	if (isRunStart)
	{
		isRunNow = false;
		isRunEnd = false;
		if (!playerRunStartObject->GetPlay())
		{
			isRunStart = false;
			isRunNow = true;
			runNTime = MaxNTime;
			playerRunNowObject->PlayAnimation();
		}
	}
	else if (isRunNow)
	{
		isRunStart = false;
		isRunEnd = false;
		if (!playerRunNowObject->GetPlay())
		{
			//isRunEnd = true;
			isStay = true;
			isRunNow = false;
			runETime = MaxETime;
			//playerRunEndObject->PlayAnimation();
			playerStayObject->PlayAnimation();
		}
	}
	else if (isRunEnd)
	{
		isRunStart = false;
		isRunNow = false;
		if (!playerRunEndObject->GetPlay())
		{
			isRunEnd = false;
			isStay = true;
		}
	}
	else if (isStay)
	{
		if (!playerStayObject->GetPlay())
		{
			playerStayObject->PlayAnimation();
		}
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
	//playerFbxObj->rotation.y = 90;
}

void OthlloPlayer::MotionUpdate()
{
	playerRunStartObject->position = playerFbxObj->position;
	//playerRunStartObject->rotation = playerFbxObj->rotation;
	playerRunStartObject->rotation = rotation;
	playerRunStartObject->scale = playerFbxObj->scale;
	playerRunStartObject->Update();
	//player.Update(&each);


	playerRunNowObject->position = playerFbxObj->position;
	//playerRunNowObject->rotation = playerFbxObj->rotation;
	playerRunNowObject->rotation = rotation;
	playerRunNowObject->scale = playerFbxObj->scale;
	playerRunNowObject->Update();

	playerRunEndObject->position = playerFbxObj->position;
	//playerRunEndObject->rotation = playerFbxObj->rotation;
	playerRunEndObject->rotation = rotation;
	playerRunEndObject->scale = playerFbxObj->scale;
	playerRunEndObject->Update();

	playerReverseObject->position = playerFbxObj->position;
	//playerReverseObject->rotation = playerFbxObj->rotation;
	playerReverseObject->rotation = rotation;
	playerReverseObject->scale = playerFbxObj->scale;
	playerReverseObject->Update();

	playerStayObject->position = playerFbxObj->position;
	//playerStayObject->rotation = playerFbxObj->rotation;
	playerStayObject->rotation = rotation;
	playerStayObject->scale = playerFbxObj->scale;
	if (!playerStayObject->GetPlay())
	{
		playerStayObject->PlayAnimation();
	}
	playerStayObject->Update();
}

void OthlloPlayer::MoveCancel()
{
	isEase = false;
	isMoveEnd = false;
	playerFbxObj->position = startPos;
}