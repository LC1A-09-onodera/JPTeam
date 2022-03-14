#pragma once
#include "../3DModel/Model.h"
#include <DirectXMath.h>

using namespace DirectX;

struct Bullet;

enum EnemyAction
{
	ARMATTACK, FOOTATTACK, MOVE, DOWN, STAY
};
//struct Bullet
//{
//	Model bullet;
//	XMFLOAT3 pos;
//	XMFLOAT3 direction;
//	bool isActive;
//	int life;
//};

class Enemy
{
	//���f���f�[�^
	static const int stayMotionAmount = 5;
	static int stayMotionCount;
	static Model stay[stayMotionAmount];
	static const int armAttackMotionAmount = 5;
	static int armAttackMotionCount;
	static Model armAttack[armAttackMotionAmount];
	static const int footAttackMotionAmount = 5;
	static int footAttackMotionCount;
	static Model footAttack[footAttackMotionAmount];
	//��{�I�Ȉʒu
	static XMFLOAT3 position;
	//�e���ʂ̃|�W�V����
	static XMFLOAT3 armRPosition;
	static XMFLOAT3 armLPosition;
	static XMFLOAT3 footRPosition;
	static XMFLOAT3 footLPosition;
	static XMFLOAT3 headPosition;
	//�����Ă����
	static XMFLOAT3 direction;
	//�s�����Ă邩�̃t���O
	static bool isAction ;
	//�G�l�~�[�̗̑�
	static int HP;
	//�U�����ɗ\���G�t�F�N�g�����邽�߂̃t���O
	static bool isAttackEffect;
	//�s�����Ă��鎞��
	static int actionTime;
	//�s�������Ԗڂ�
	static int actionNum;
	//�m���̎��ɍU���I�ɂȂ�
	static const int redHP = 20;
	//���U�����̎���
	static const int footActionTime = 120;
	//�G�t�F�N�g������^�C�~���O
	static const int footActionEfectionTime = 30;
	//�r�U�����̎���
	static const int armActionTime = 120;
	//�G�t�F�N�g������^�C�~���O
	static const int armActionEffectTime = 30;
	static XMFLOAT3 attackEffectEmitter;
	//��������
	static const int moveActionTime = 120;
	//�_�E�����Ă���Ƃ��Ɏ���
	static const int downActionTime = 120;
	//�҂�����
	static const int stayActionTime = 120;
	//�e��
	static const int bulletAmount = 50;
	//�ʂ̎���
	static const int bulletLife = 90;
	//�G�̍ő�HP
	static const int enemyMaxHP = 100;
	//��
	static Bullet bullet[bulletAmount];
	//�o��ʂ̃C���f�b�N�X
	static int bulletIndex;
	//
	static const int bulletInterval = 5;
	//
	static int bulletTime;
public:
	//������
	static void Init();
	//�ʂ̈ꊇ������
	static void BulletInit();
	//�X�V
	static void Update();
	//�`��
	static void Draw();
	//�r�U��
	static void ArmAttack();
	//���U��
	static void FootAttack();
	//�_�E�����ɍs���s�\�ɂȂ�悤�ȏ���
	static void Down();
	//�����Ƃ��Ɏg��
	static void Move();
	//�҂�����
	static void Stay();
	//�s�������߂�
	static void SelectAction();
	//�s����Ɍ��ɖ߂�
	static void ReturnNone();
	//�ʂ̓���
	static void BulletUpdate();
	//�ʈ���˂��邽�߂̂�[��
	static void BulletOneInit();
	//�ʂ̕������߂�
	static XMFLOAT3 BulletDirection();
	//�G�t�F�N�g�̔�����ꏊ���̕ύX
	static void EffectControl();
	static void ForPlayerDirection();
	static void ReSet();
};