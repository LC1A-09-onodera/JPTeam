#include "Camera.h"
#include "../BaseDirectX/Input.h"
#include "../BaseDirectX/Quaternion.h"
#include "../BaseDirectX/viewport.h"
#include "../imgui/ImguiControl.h"

Camera Cameras::camera;
Camera Cameras::rCamera;

void Camera::CameraTargetRot()
{
	isControl = Imgui::CameraControl;
	if (isControl)
	{
		targetR = Imgui::CameraR;
		rotationXZ = Imgui::CameraRotation;
		rotation.x = ShlomonMath::Cos(rotationXZ);
		rotation.z = ShlomonMath::Sin(rotationXZ);
		rotation.y = Imgui::CameraHigh;
		Normalize(rotation);
		eye.x = target.x + rotation.x * targetR;
		eye.y = target.y + rotation.y * targetR;
		eye.z = target.z + rotation.z * targetR;
	}
}

void Camera::Init(XMFLOAT3& eye, XMFLOAT3& target)
{
	eye = { 0.0f, 0.0f, -100.0f };
	target = { 0.0f, 20.0f, 0.0f };
	up = { 0.0f, 1.0f, 0.0f };

	Update();
}

void Camera::Update()
{
	ShakeUpdate();
	if (!isRCamera)
	{
		CameraTargetRot();
	}
	//�r���[�ϊ��s�����蒼��
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	XMVECTOR upVector = XMLoadFloat3(&up);
	//z��
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//x��
	XMVECTOR cameraAxisX;
	//�O��
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//y��
	XMVECTOR cameraAxisY;
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	//�J������]�s��
	XMMATRIX matCameraRot;
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	matView = XMMatrixTranspose(matCameraRot);
	//���s�ړ��̋t
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	matView.r[3] = translation;
	//�r���{�[�h�Ɏg������̂̌v�Z
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
	//y���r���{�[�h�Ɏg������̂̌v�Z
	XMVECTOR yBillCameraAxisX;
	XMVECTOR yBillCameraAxisY;
	XMVECTOR yBillCameraAxisZ;
	yBillCameraAxisX = cameraAxisX;
	yBillCameraAxisY = XMVector3Normalize(upVector);
	yBillCameraAxisZ = XMVector3Cross(yBillCameraAxisX, yBillCameraAxisY);
	matBillboardY.r[0] = yBillCameraAxisX;
	matBillboardY.r[1] = yBillCameraAxisY;
	matBillboardY.r[2] = yBillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
}

void Camera::EyeAndTargetMove(XMFLOAT3 moveAmount)
{
	eye.x += moveAmount.x;
	eye.y += moveAmount.y;
	eye.z += moveAmount.z;
	target.x += moveAmount.x;
	target.y += moveAmount.y;
	target.z += moveAmount.z;

	Update();
}

void Camera::ShakeUpdate()
{
	if (!isShake) return;
	Camera::eye = Camera::eyeStartPosition;
	Camera::target = Camera::targetStartPosition;
	int random = rand() % 3;
	if (random == 0)
	{
		eye.x += shakePower;
		target.x += shakePower;
	}
	else if (random == 1)
	{
		eye.x += -shakePower;
		target.x += -shakePower;
	}
	random = rand() % 3;
	if (random == 0)
	{
		eye.z += shakePower;
		target.z += shakePower;
	}
	else if (random == 1)
	{
		eye.z += -shakePower;
		target.z += -shakePower;
	}
	shakePower -= 0.5f;
	if (shakePower <= 0)
	{
		shakePower = 0;
		isShake = false;
		Camera::eye = Camera::eyeStartPosition;
		Camera::target = Camera::targetStartPosition;
	}
}

void Camera::SetShake(float shakePower)
{
	Camera::shakePower = shakePower;
	Camera::eyeStartPosition = Camera::eye;
	Camera::targetStartPosition = Camera::target;
	isShake = true;
}

void Camera::SetTarget(XMFLOAT3 &target)
{
	this->target = target;
}

XMFLOAT3 Camera::GetTargetDirection()
{
	XMFLOAT3 dire;
	XMFLOAT3 sub;
	sub.x = target.x - eye.x;
	sub.y = target.y - eye.y;
	sub.z = target.z - eye.z;
	float leg = TargetLength();
	dire.x = sub.x / leg;
	dire.y = sub.y / leg;
	dire.z = sub.z / leg;
	return dire;
}

XMFLOAT3 Camera::GetMousePosition()
{
	//�X�N���[���n
	POINT mouse = WindowsAPI::GetMousePos();
	XMFLOAT2 mouseFloat;
	mouseFloat = XMFLOAT2(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
	//�N���b�v�n
	mouseFloat.x = mouseFloat.x / (float)window_width;
	mouseFloat.y = mouseFloat.y / (float)window_height;
	mouseFloat.x = mouseFloat.x * 2.0f;
	mouseFloat.y = mouseFloat.y * 2.0f;
	mouseFloat.x = mouseFloat.x - 1.0f;
	mouseFloat.y = mouseFloat.y - 1.0f;
	mouseFloat.y = mouseFloat.y * -1.0f;
	//�r���[���W
	XMVECTOR mousePosition;
	XMMATRIX invProj, invView;
	invProj = XMMatrixInverse(nullptr, BaseDirectX::matProjection);
	invView = XMMatrixInverse(nullptr, Camera::matView);
	mousePosition = XMLoadFloat3(&XMFLOAT3(mouseFloat.x, mouseFloat.y, 0));
	mousePosition = XMVector3Transform(mousePosition, invProj);
	mousePosition = XMVector3Transform(mousePosition, invView);
	XMFLOAT3 result;
	result.x = mousePosition.m128_f32[0];
	result.y = mousePosition.m128_f32[1];
	result.z = mousePosition.m128_f32[2];
	return result;
}

XMFLOAT3 Camera::EyeToMouseVec()
{
	XMFLOAT3 mouse;
	mouse = GetMousePosition();
	XMFLOAT3 eyePos;
	eyePos = Camera::eye;
	XMFLOAT3 result;
	result = mouse - eyePos;
	return Normalize(result);
}

XMFLOAT3 Camera::MousePosition(float z)
{
	XMFLOAT3 mouseVec = EyeToMouseVec();
	float count;
	count = z - Camera::eye.z / mouseVec.z;
	float x;
	x = mouseVec.x * count;
	float y;
	y = mouseVec.y * count;
	XMFLOAT3 result = { x, y, z };
	return result;
}

float Camera::TargetLength()
{
	float lenght;
	XMFLOAT3 sub;
	sub.x = target.x - eye.x;
	sub.y = target.y - eye.y;
	sub.z = target.z - eye.z;
	lenght = sqrtf((sub.x * sub.x) + (sub.y * sub.y) + (sub.z * sub.z));
	return lenght;
}
