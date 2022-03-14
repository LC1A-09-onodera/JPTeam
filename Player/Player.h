#pragma once
//#include "../3DModel/Model.h"
#include "../sampleObject/sampleObject.h"
#include "../Collition/CollisionPrimitive.h"
#include "../Sound/Sound.h"

using namespace DirectX;

const int MAX_ACTIVE_COUNT = 300;
const int MAX_INVICIBLE_COUNT = 120;
const float MAX_SPEED = 5.0f;

static XMFLOAT3 VecNormaliz(XMFLOAT3& vector)
{
	float leg = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	vector.x = vector.x / leg;
	vector.y = vector.y / leg;
	vector.z = vector.z / leg;
	return vector;
}

class Player final
{
private:
	Player();
	~Player();

	XMFLOAT3 pos;			//�v���C���[�̍��W
	XMFLOAT3 vec3;			//�����Ă�������i���K���ρj
	int activeCount;		//�s���s�\�J�E���g
	int invincibleCount;	//���G�J�E���g
	bool isActive;			//�s���ł��邩�ǂ���
	bool isInvincible;		//���G���ǂ���
	bool isShoot;			//�ˌ������ǂ����i�e�����邩�j
	bool isDetonating;		//�N���������ǂ���

public:
	Player(const Player& obj) = delete;
	Player& operator=(const Player& obj) = delete;
	static Player* GetPlayer();
	//������
	void Init();
	//�X�V
	void Update(bool isBombAlive,bool isHit);
	//�`��
	void Draw();

	XMFLOAT3 GetPos() { return pos; }				//�v���C���[�̍��W
	XMFLOAT3 GetVec3() { return vec3; }				//�����Ă�������i���K���ρj 
	bool IsActive() { return isActive; }			//�s���ł��邩�ǂ���
	bool IsInvincible() { return isInvincible; }	//���G���ǂ���
	bool IsShoot() { return isShoot; }				//�ˌ������ǂ���
	//void EndShoot() { isShoot = false; }			//�e����ʊO���N�����ꂽ���G�Ɠ��������ꍇ�ɌĂяo��
	bool IsDetonating() { return isDetonating; }	//�N���������ǂ���


	XMVECTOR oldPlayerPos;
	Box2D playerCollision;
	SampleObject player;
	int particleTime;
	bool isParticle;
};