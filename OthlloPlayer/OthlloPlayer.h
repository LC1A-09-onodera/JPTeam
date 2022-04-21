#pragma once
#include "../3DModel/Model.h"
#include "../DX12operator.h"
#include "../FBXObject/FBXObject.h"
#include "../FbxLoader/FbxLoader.h"
namespace ConstOthlloPlayer
{
	static const float MaxMoveAmount = 2.0f;
}
class OthlloPlayer
{
	
	static Model player;
	static EachInfo each;
	
	
	
	static bool isMoveEnd;
	static void Move();
	static void EaseUpdate();
public:
	static XMFLOAT3 GetPosition(){ return playerFbxObj->position; }
	static void SetPosition(XMFLOAT3 position) { playerFbxObj->position = position; }
	static FBXModel* playerFbx;
	static FBXObject* playerFbxObj;
	static XMFLOAT3 startPos;
	static XMFLOAT3 endPos;
	static bool isEase;
	static float easeTime;
	static bool GetIsMoveEnd(){return isMoveEnd;}
	static void MoveCancel();
	static void Init();
	static void Update();
	static void Draw();
};