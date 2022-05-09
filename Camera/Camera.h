#include <DirectXMath.h>
#include "../BaseDirectX/vec3.h"
#include "../DX12operator.h"
using namespace DirectX;

class Camera
{
public:
	//�ʒu
	Vec3 eye;
	Vec3 eyePos;
	Vec3 eyeStartPosition;
	Vec3 eyeGoalPosition;
	float shakePower;
	bool isShake;
	//�ǂ��𒍎��_�Ƃ��邩
	Vec3 target;
	Vec3 targetStartPosition;
	Vec3 targetGoalPosition;
	//�J�����̌����H
	Vec3 up;
	Vec3 cameraUp;
	//�r���[�s��̍쐬
	XMMATRIX matView;
	//�r���{�[�h
	XMMATRIX matBillboard;
	//Y���r���{�[�h
	XMMATRIX matBillboardY;

	XMMATRIX rotM;
	XMVECTOR vv0;

	float cameraRightAngle;
	float cameraUpAngle;


	float rotX;
	float rotY;
	float rotZ;

	float cameraAngleHorizonal;
	float cameraAngleVertical;

	void Init();
	void Update();
	void EyeMove(Vec3 moveAmount);
	void EyeAndTargetMove(XMFLOAT3 moveAmount);
	void TargetMove(XMFLOAT3 moveAmount);
	void FPSTargetMove(XMFLOAT3 moveAmount, float R = 100.0f);
	float TargetLength();
	XMFLOAT3 GetTargetDirectionSideR();
	XMFLOAT3 GetTargetDirectionSideL();
	void eyeMoveCircleHorizonal(XMFLOAT3 Amount);
	void eyeMoveCircleVertical(XMFLOAT3 Amount);
	void setEye(XMFLOAT3 Amount);
	void SetTarget(const XMFLOAT3& target);
	void QuaternionRotation(const float &RightAngle = NULL, const float &UpAngle = NULL);
	void ShakeUpdate();
	void SetShake(float shakePower);

	XMFLOAT3 GetMousePosition();
	XMFLOAT3 EyeToMouseVec();
	XMFLOAT3 MousePosition(float z);
};

class Cameras
{
public:
	static Camera camera;
	static Camera rCamera;
};