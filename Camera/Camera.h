#include <DirectXMath.h>
#include "../BaseDirectX/DX12operator.h"
using namespace DirectX;

class Camera
{
	float shakePower;
	bool isShake;
	float rotationXZ = 0.0f;
	XMFLOAT3 rotation = {0, 0.2f, -1.0f};
	float targetR = 10.0f;

public:
	//�ʒu
	XMFLOAT3 eye;
	XMFLOAT3 eyeStartPosition;
	XMFLOAT3 eyeGoalPosition;
	
	//�ǂ��𒍎��_�Ƃ��邩
	XMFLOAT3 target;
	XMFLOAT3 targetStartPosition;
	XMFLOAT3 targetGoalPosition;
	//�J�����̌����H
	XMFLOAT3 up;
	//�r���[�s��̍쐬
	XMMATRIX matView;
	//�r���{�[�h
	XMMATRIX matBillboard;
	//Y���r���{�[�h
	XMMATRIX matBillboardY;
	//Imgui����̑�����󂯕t����
	bool isControl = true;
	//���Ε����̃J�������ǂ���
	bool isRCamera = false;
	
	void CameraTargetRot();
	void Init(XMFLOAT3 &eye, XMFLOAT3 &target);
	void Update();
	void EyeAndTargetMove(XMFLOAT3 moveAmount);
	float TargetLength();
	void ShakeUpdate();
	void SetShake(float shakePower);
	void SetTarget(XMFLOAT3 &target);
	XMFLOAT3 GetTargetDirection();
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