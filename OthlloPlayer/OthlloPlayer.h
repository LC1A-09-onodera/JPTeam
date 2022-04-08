#pragma once
#include "../3DModel/Model.h"
#include "../DX12operator.h"

namespace ConstOthlloPlayer
{
	static const float MaxMoveAmount = 2.0f;
}
class OthlloPlayer
{
	static Model player;
	static EachInfo each;
	
	static XMFLOAT3 startPos;
	static XMFLOAT3 endPos;
	static bool isEase;
	static float easeTime;
	static bool isMoveEnd;
	static void Move();
	static void EaseUpdate();
public:
	static XMFLOAT3 GetPosition(){ return ConvertXMVECTORtoXMFLOAT3(each.position); }
	static void SetPosition(XMFLOAT3 position) { each.position = ConvertXMFLOAT3toXMVECTOR(position); }
	static bool GetIsMoveEnd(){return isMoveEnd;}
	static void MoveCancel();
	static void Init();
	static void Update();
	static void Draw();
};