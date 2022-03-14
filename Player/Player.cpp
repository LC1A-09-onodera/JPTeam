#include "Player.h"
#include "../BaseDirectX/Input.h"
#include "../Enemy/Enemy.h"
#include "../Camera/Camera.h"
#include "../Sound/Sound.h"
#include "../Shader/ShaderManager.h"

Player::Player()
{
	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT3 vec3 = { 0,0,0 };
	int activeCount = 0;
	int invincibleCount = 0;
	bool isActive = true;
	bool isInvincible = false;
	bool isShoot = false;
	bool isDetonating = false;
}

Player::~Player()
{
}

Player* Player::GetPlayer()
{
	static Player player;
	return &player;
}

//������
void Player::Init()
{
	player.CreateModel("Block", ShaderManager::playerShader);
	//water.CreateWater();

	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT3 vec3 = { 0,0,0 };
	int activeCount = 0;
	int invincibleCount = 0;
	bool isActive = true;
	bool isInvincible = false;
	bool isShoot = false;
	bool isDetonating = false;
}

//�X�V
void Player::Update(bool isBombAlive, bool isHit)
{
	/*�ړ�*/
	if (DirectInput::leftStickX() == 0.0f && DirectInput::leftStickY() == 0.0f) { vec3 = { 0,0,0 }; }
	else { vec3.x = DirectInput::leftStickX(); vec3.z = DirectInput::leftStickY(); }

	pos.x += vec3.x * MAX_SPEED;
	pos.z += -vec3.z * MAX_SPEED;

	/*�ˌ��A�e�֌W*/
	if (DirectInput::IsButtonPush(DirectInput::ButtonKind::Button01)) { isShoot = true; }
	else if (!isBombAlive) { isShoot = false; }

	/*�N��*/
	if (isShoot && DirectInput::IsButtonPush(DirectInput::ButtonKind::Button01)) { isDetonating = true; }
	else { isDetonating = false; }

	/*���@���G�ɓ����������̔���*/
	if (isHit && !isInvincible) { isActive = false; }

	/*�s���s�\�Ƃ����G�Ƃ�*/
	//�s���\��
	if (isActive)
	{
		if (isInvincible && invincibleCount < MAX_INVICIBLE_COUNT) { invincibleCount++; }
		else { isInvincible = false; }
	}

	//�s���s�\��
	else
	{
		if (activeCount < MAX_ACTIVE_COUNT) { activeCount++; }
		else
		{
			activeCount = 0;
			isInvincible = true;
			isActive = true;
		}
	}

	player.position = ConvertXMFLOAT3toXMVECTOR(pos);
	player.Update();
}

//�`��
void Player::Draw()
{
	Draw3DObject(player);
}
