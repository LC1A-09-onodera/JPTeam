#pragma once
#include "../3DModel/Model.h"
#include "../DX12operator.h"
#include "../FBXObject/FBXObject.h"
#include "../FbxLoader/FbxLoader.h"
#include "../Sound/Sound.h"
namespace ConstOthlloPlayer
{
	static const float MaxMoveAmount = 2.0f;
}
class OthlloPlayer
{
	
	static Model player;
	static EachInfo each;
	static SoundData moveSound;
	
	static bool isMoveEnd;
	static void Move();
	static void EaseUpdate();
	static void MotionUpdate();
public:
	static XMFLOAT3 GetPosition(){ return playerFbxObj->position; }
	static void SetPosition(XMFLOAT3 position) { playerFbxObj->position = position; }
	static FBXModel* playerFbx;
	static FBXObject* playerFbxObj;

	static FBXModel* playerStay;
	static FBXObject* playerStayObject;
	static FBXModel* playerRunStart;
	static FBXObject* playerRunStartObject;
	static FBXModel* playerRunNow;
	static FBXObject* playerRunNowObject;
	static FBXModel* playerRunEnd;
	static FBXObject* playerRunEndObject;
	static FBXModel* playerReverse;
	static FBXObject* playerReverseObject;

	static XMFLOAT3 startPos;
	static XMFLOAT3 endPos;
	static bool isEase;
	static float easeTime;

	static bool isStay;
	static bool isRunStart;
	static bool isRunNow;
	static bool isRunEnd;
	static bool isReverse;

	static int runSTime;
	static int runNTime;
	static int runETime;
	static int reverseTime;

	static const int MaxSTime;
	static const int MaxNTime;
	static const int MaxETime;
	static const int MaxReverseTime;

	static XMFLOAT3 rotation;

	static bool GetIsMoveEnd(){return isMoveEnd;}
	static void MoveCancel();
	static void Init();
	static void Update();
	static void Draw();
};