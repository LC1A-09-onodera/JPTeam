#include "Camera.h"
#include "../BaseDirectX/Input.h"
#include "../BaseDirectX/Quaternion.h"
#include "../BaseDirectX/viewport.h"

Vec3 Camera::eye;
Vec3 Camera::target;
Vec3 Camera::up;
XMFLOAT3 Camera::direction;
XMMATRIX Camera::matBillboard;
XMMATRIX Camera::matBillboardY;
XMMATRIX Camera::matView;
float Camera::cameraAngleHorizonal;
float Camera::cameraAngleVertical;
XMMATRIX Camera::rotM;
XMVECTOR Camera::vv0;
Vec3 Camera::eyePos;
Vec3 Camera::cameraUp;
float Camera::cameraRightAngle = 0.0f;
float Camera::cameraUpAngle = 0.0f;
float Camera::rotX = 0;
float Camera::rotY = 0;
float Camera::rotZ = 0;
Vec3 Camera::eyeStartPosition;
Vec3 Camera::eyeGoalPosition;
float Camera::shakePower;
bool Camera::isShake;
Vec3 Camera::targetStartPosition;
Vec3 Camera::targetGoalPosition;

void Camera::Init()
{
	eye.v = { 0.0f, 0.0f, -100.0f };
	target = { 0.0f, 20.0f, 0.0f };
	up = { 0.0f, 1.0f, 0.0f };
	eyePos = eye;
	cameraUp = up;

	rotM = XMMatrixIdentity();

	vv0 = { 0, 0, -1, 0 };

	cameraAngleHorizonal = 0.0f;
	cameraAngleVertical = 0.0f;

	Update();
}

void Camera::Update()
{
	ShakeUpdate();
	//ビュー変換行列を作り直す
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	XMVECTOR eyePosition = XMLoadFloat3(&eye.v);
	XMVECTOR targetPosition = XMLoadFloat3(&target.v);
	XMVECTOR upVector = XMLoadFloat3(&up.v);
	//z軸
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//x軸
	XMVECTOR cameraAxisX;
	//外積
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//y軸
	XMVECTOR cameraAxisY;
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	//カメラ回転行列
	XMMATRIX matCameraRot;
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	matView = XMMatrixTranspose(matCameraRot);
	//平行移動の逆
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	matView.r[3] = translation;
	//ビルボードに使えるものの計算
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
	//y軸ビルボードに使えるものの計算
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

void Camera::EyeMove(Vec3 moveAmount)
{
	Update();
}

void Camera::EyeAndTargetMove(XMFLOAT3 moveAmount)
{
	eye.v.x += moveAmount.x;
	eye.v.y += moveAmount.y;
	eye.v.z += moveAmount.z;
	target.v.x += moveAmount.x;
	target.v.y += moveAmount.y;
	target.v.z += moveAmount.z;

	Update();
}

void Camera::TargetMove(XMFLOAT3 moveAmount)
{
	target.v.x += moveAmount.x;
	target.v.y += moveAmount.y;
	target.v.z += moveAmount.z;
	Update();
}

void Camera::FPSTargetMove(XMFLOAT3 moveAmount, float R)
{

}

void Camera::setEyePositionTPS(XMFLOAT3 Amount)
{
	if (cameraAngleHorizonal >= 360) {
		cameraAngleHorizonal = 360;
	}
	else if (cameraAngleHorizonal <= 0) {
		cameraAngleHorizonal = 0;
	}

	if (cameraAngleVertical >= 90) {
		cameraAngleVertical = 90;
	}
	else if (cameraAngleVertical <= -90) {
		cameraAngleVertical = -90;
	}
	eye.v.x = cos(
		cameraAngleHorizonal) * cos(cameraAngleVertical) * Amount.x;
	eye.v.z = sin(cameraAngleHorizonal) * cos(cameraAngleVertical) * Amount.y;
	eye.v.y = sin(cameraAngleVertical) * Amount.z;
	Update();
}

void Camera::setEyeTargetTPS(XMFLOAT3 Amount)
{
	target.v = Amount;
	target.v.y = Amount.y + 8;
	Update();
}

void Camera::eyeMoveCircleHorizonal(XMFLOAT3 Amount)
{
	target.v = Amount;
	target.v.y = Amount.y + 8;

	rotM = XMMatrixRotationY(cameraAngleHorizonal);
	vv0 = XMVector3TransformNormal(vv0, rotM);

	eye.v.x = target.v.x + vv0.m128_f32[0];
	eye.v.y = target.v.y + vv0.m128_f32[1];
	eye.v.z = target.v.z + vv0.m128_f32[2];
	Update();
}

void Camera::eyeMoveCircleVertical(XMFLOAT3 Amount)
{
	target.v = Amount;
	target.v.y = Amount.y + 8;

	rotM = XMMatrixRotationX(cameraAngleVertical);
	vv0 = XMVector3TransformNormal(vv0, rotM);

	eye.v.x = target.v.x + vv0.m128_f32[0];
	eye.v.y = target.v.y + vv0.m128_f32[1];
	eye.v.z = target.v.z + vv0.m128_f32[2];
	Update();
}

void Camera::SetTarget(const XMFLOAT3& target)
{
	Camera::target.v = target;
}

void Camera::QuaternionRotation(const float& RightAngle, const float& UpAngle)
{
	const float ROT_UNIT = 0.02f;
	cameraUpAngle = 0.0f;
	cameraRightAngle = 0.0f;
	if (RightAngle != NULL)
	{
		cameraRightAngle += RightAngle;
	}
	if (UpAngle != NULL)
	{
		cameraRightAngle += UpAngle;
	}

	if (directInput->rightStickX() > 0 || Input::Key(DIK_RIGHT))
	{
		cameraRightAngle = -ROT_UNIT;
	}

	if (directInput->rightStickX() < 0 || Input::Key(DIK_LEFT))
	{
		cameraRightAngle = ROT_UNIT;
	}

	/*if (directInput->rightStickY() < 0 || Input::Key(DIK_DOWN))
	{
		cameraUpAngle = -ROT_UNIT;
	}

	if (directInput->rightStickY() > 0 || Input::Key(DIK_UP))
	{
		cameraUpAngle = ROT_UNIT;
	}*/
	Quaternion qCameraRghtAngle = quaternion(cameraUp, cameraRightAngle);
	Vec3 cameraSide = cameraUp.cross(target - eyePos).nomalize();
	Quaternion qCameraUpAngle = quaternion(cameraSide, cameraUpAngle);

	Quaternion q = qCameraRghtAngle * qCameraUpAngle;

	Quaternion qCameraPosition = quaternion(eyePos.v.x, eyePos.v.y, eyePos.v.z, 0.0f);

	Quaternion qq = Conjugate(q);

	qCameraPosition = q * qCameraPosition * qq;

	eyePos = { qCameraPosition.x,qCameraPosition.y,qCameraPosition.z };

	//上方向ベクトル
	Quaternion qCameraUp = quaternion(cameraUp.v.x, cameraUp.v.y, cameraUp.v.z, 0.0f);
	qCameraUp = q * qCameraUp * qq;

	cameraUp = GetAxis(qCameraUp);

	eye.v.x += ((eyePos.v.x + target.v.x) - eye.v.x) / 1;
	eye.v.y += ((eyePos.v.y + target.v.y) - eye.v.y) / 1;
	eye.v.z += ((eyePos.v.z + target.v.z) - eye.v.z) / 1;
	up = cameraUp;
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
		eye.v.x += shakePower;
		target.v.x += shakePower;
	}
	else if (random == 1)
	{
		eye.v.x += -shakePower;
		target.v.x += -shakePower;
	}
	random = rand() % 3;
	if (random == 0)
	{
		eye.v.z += shakePower;
		target.v.z += shakePower;
	}
	else if (random == 1)
	{
		eye.v.z += -shakePower;
		target.v.z += -shakePower;
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

XMFLOAT3 Camera::GetMousePosition()
{
	//スクリーン系
	POINT mouse = WindowsAPI::GetMousePos();
	XMFLOAT2 mouseFloat;
	mouseFloat = XMFLOAT2(mouse.x, mouse.y);
	//クリップ系
	mouseFloat.x = mouseFloat.x / (float)window_width;
	mouseFloat.y = mouseFloat.y / (float)window_height;
	mouseFloat.x = mouseFloat.x * 2.0f;
	mouseFloat.y = mouseFloat.y * 2.0f;
	mouseFloat.x = mouseFloat.x - 1.0f;
	mouseFloat.y = mouseFloat.y - 1.0f;
	mouseFloat.y = mouseFloat.y * -1.0f;
	//ビュー座標
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
	eyePos = Camera::eye.v;
	XMFLOAT3 result;
	result = mouse - eyePos;
	return Normalize(result);
}

XMFLOAT3 Camera::MousePosition(float z)
{
	XMFLOAT3 mouseVec = EyeToMouseVec();
	int count;
	count = z - Camera::eye.v.z / mouseVec.z;
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
	sub.x = target.v.x - eye.v.x;
	sub.y = target.v.y - eye.v.y;
	sub.z = target.v.z - eye.v.z;
	lenght = sqrtf((sub.x * sub.x) + (sub.y * sub.y) + (sub.z * sub.z));
	return lenght;
}

XMFLOAT3 Camera::GetTargetDirection()
{
	XMFLOAT3 dire;
	XMFLOAT3 sub;
	sub.x = target.v.x - eye.v.x;
	sub.y = target.v.y - eye.v.y;
	sub.z = target.v.z - eye.v.z;
	float leg = TargetLength();
	dire.x = sub.x / leg;
	dire.y = sub.y / leg;
	dire.z = sub.z / leg;
	direction = dire;
	return direction;
}

XMFLOAT3 Camera::GetTargetDirectionSideR()
{
	XMFLOAT3 result;
	result.x = sinf(90) * GetTargetDirection().x;
	result.z = cosf(90) * GetTargetDirection().z;
	return result;
}

XMFLOAT3 Camera::GetTargetDirectionSideL()
{
	XMFLOAT3 result;
	result.x = sinf(-90.0f * 3.141592f / 180.0f) * GetTargetDirection().x;
	result.z = cosf(-90.0f * 3.141592f / 180.0f) * GetTargetDirection().z;
	return result;
}
