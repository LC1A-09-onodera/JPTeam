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

	static XMFLOAT3 startPos;
	static XMFLOAT3 endPos;
	static bool isEase;
	static float easeTime;

	static bool isStay;
	static bool isRunNow;
	static bool isReverse;

	static float easeRun1;
	static float easeRun2;
	static float easeSpace1;
	static float easeSpace2;
	static float easeStay1;
	static float easeStay2;

	static float easeRunAngleStart;
	static float easeRunAngleEnd;
	static float easeRunAngleConst;
	static bool side;

	static float easeStayStart;
	static float easeStayEnd;
	static float easeStayStartConst;
	static float easeStayEndConst;

	static float easeSpaceStartConst;
	static float easeSpaceEndConst;

	static void InitRun();
	static void InitSpace();
	static void InitStay();
	static void UpdateRun();
	static void UpdateSpace();
	static void UpdateStay();

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