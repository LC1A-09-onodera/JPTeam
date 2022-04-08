#include "OthlloPlayer.h"
#include "../BaseDirectX/Input.h"
#include "../Shader/ShaderManager.h"
using namespace ConstOthlloPlayer;

Model OthlloPlayer::player;
EachInfo OthlloPlayer::each;

XMFLOAT3 OthlloPlayer::startPos;
XMFLOAT3 OthlloPlayer::endPos;
bool OthlloPlayer::isEase;
bool OthlloPlayer::isMoveEnd;
float OthlloPlayer::easeTime;

void OthlloPlayer::Init()
{
	player.CreateModel("maru", ShaderManager::playerShader);
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	each.position = {0, 0, 0 ,1};
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
	player.Update(&each);
	Draw3DObject(player);
}

void OthlloPlayer::Move()
{
	bool D = Input::KeyTrigger(DIK_D);
	bool A = Input::KeyTrigger(DIK_A);
	bool S = Input::KeyTrigger(DIK_S);
	bool W = Input::KeyTrigger(DIK_W);
	if ((D || A || S || W) && !isEase)
	{
		isMoveEnd = false;
		startPos = ConvertXMVECTORtoXMFLOAT3(each.position);
		endPos = startPos;
		if (D && each.position.m128_f32[0] < 6.0f)
		{
			endPos.x += MaxMoveAmount;
		}
		else if (A && each.position.m128_f32[0] > -8.0f)
		{
			endPos.x -= MaxMoveAmount;
		}
		else if (S && each.position.m128_f32[1] > -6.0f)
		{
			endPos.y -= MaxMoveAmount;
		}
		else if (W && each.position.m128_f32[1] < 8.0f)
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
	each.position = ConvertXMFLOAT3toXMVECTOR(EaseInQuad(startPos, endPos, easeTime));
}


void OthlloPlayer::MoveCancel()
{
	isEase = false;
	isMoveEnd = false;
}