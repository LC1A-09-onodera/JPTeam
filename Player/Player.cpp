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

	XMFLOAT3 pos = { 0,0,0 };		//�v���C���[�̍��W
	XMFLOAT3 vec3 = { 0,0,0 };		//�����Ă�������i���K���ρj
	int activeCount = 0;			//�s���s�\�J�E���g
	int invincibleCount = 0;		//���G�J�E���g
	float bombForce = 0;			//�{���̗͕ۑ��p
	float enemyForce = 0;			//�G�̗͕ۑ��p
	bool isActive = true;			//�s���ł��邩�ǂ���
	bool isHitBomb = false;			//�{���ɓ������Ĕ�΂���Ă邩�ǂ���
	bool isHitEnemy = false;		//�G�ɓ������Ĕ�΂���Ă邩�ǂ���
	bool isInvincible = false;		//���G���ǂ���
	bool isShoot = false;			//�ˌ������ǂ����i�e�����邩�j
	bool isDetonating = false;		//�N���������ǂ���
}

//�X�V
void Player::Update(bool isBombAlive)
{
	/*�ړ�*/
	CheakIsInput();
	//�s���\���������łȂ����
	CheakIsActive();

	/*���˃g���K�[*/
	CheakShootTrigger(isBombAlive);

	/*�N���g���K�[*/
	CheakDetonatingTrigger(isBombAlive);

	/*���@���G�ɓ����������̔���*/
	CheakHitEnemy();

	/*�s���s�\�Ƃ����G�Ƃ�*/
	CalcActiveCount();

	/*�{���̗͌v�Z*/
	AddBombForce();

	/*�G�̗͌v�Z*/
	AddEnemyForce();

	player.each.position = ConvertXMFLOAT3toXMVECTOR(pos);
	player.Update();
}

//�`��
void Player::Draw()
{
	Draw3DObject(player);
}

void Player::HitBomb(const float& BombForce)
{
	if (isActive)
	{
		//�{���̗͂���
		bombForce = BombForce;

		//�ړ�����
		isActive = false;

		//������ђ��ɂ���
		isHitBomb = true;
	}
}

void Player::CheakIsInput()
{
	/*�ړ�*/
	//���͂Ȃ���΃��Z�b�g
	if (DirectInput::leftStickX() == 0.0f && DirectInput::leftStickY() == 0.0f) { vec3 = { 0,0,0 }; }
	else { vec3.x = DirectInput::leftStickX(); vec3.z = DirectInput::leftStickY(); }
}

void Player::CheakIsActive()
{
	//�s���\���������łȂ����
	if (isActive && !isHitBomb)
	{
		pos.x += vec3.x * MAX_SPEED;
		pos.z += -vec3.z * MAX_SPEED;
	}
}

void Player::CheakShootTrigger(bool isBombAlive)
{
	/*���˃g���K�[*/
	if (DirectInput::IsButtonPush(DirectInput::ButtonKind::Button01) && !isBombAlive && isActive) { isShoot = true; }
	else { isShoot = false; }
}

void Player::CheakDetonatingTrigger(bool isBombAlive)
{
	/*�N���g���K�[*/
	if (DirectInput::IsButtonPush(DirectInput::ButtonKind::Button01) && isBombAlive && isActive) { isDetonating = true; }
	else { isDetonating = false; }
}

void Player::CheakHitEnemy()
{
	//�s���s�\�������G����������ђ��͔��薳��
	if (!isActive || isInvincible || isHitBomb) { return; }

	bool isHit;
	auto itr = Enemys::enemys.begin();
	for (; itr != Enemys::enemys.end(); ++itr)
	{
		XMFLOAT3 enemyPos = itr->GetPosition();

		//2�_�Ԃ̋����Ɣ���i�~�j
		isHit = CheakHit(1, 1, pos, enemyPos);

		//�������ĂȂ��������蒼��
		if (!isHit) { continue; }

		//����������s���s�\�ɂ���
		isActive = false;
		break;
	}

	//�N�Ƃ�������Ȃ������ꍇ�A�I��
	if (isActive) { return; }

	enemyForce = MAX_ENEMY_FORCE;
}

void Player::CalcActiveCount()
{
	/*�s���s�\�Ƃ����G�Ƃ�*/
	//���G���s���\��
	if (isInvincible && isActive)
	{
		if (isInvincible && invincibleCount < MAX_INVICIBLE_COUNT) { invincibleCount++; }
		else { isInvincible = false; }
	}

	//�s���s�\��
	else if (!isActive)
	{
		if (activeCount < MAX_STAN_COUNT) { activeCount++; }
		else
		{
			activeCount = 0;
			isInvincible = true;
			isActive = true;
		}
	}
}

void Player::AddBombForce()
{
	//������ђ������v�Z
	if (!isHitBomb) { return; }

	//�ړ�
	pos.x += bombForce * vec3.x;
	pos.z += bombForce * -vec3.z;

	//���Z
	bombForce -= RESISTANCE_VALUE;

	//���ʂ����������0�ɂ��Ĉړ��ĊJ
	if (bombForce < MINIMUM_FORCE)
	{
		bombForce = 0;
		isActive = true;
		isHitBomb = false;
	}
}

void Player::AddEnemyForce()
{
	//�s���s�\�������v�Z
	if (isActive) { return; }

	//�ړ�
	pos.x += enemyForce * vec3.x;
	pos.z += enemyForce * -vec3.z;

	//���Z
	enemyForce -= RESISTANCE_VALUE;

	//���ʂ����������0�ɂ���
	if (enemyForce < MINIMUM_FORCE) { enemyForce = 0; }
}
