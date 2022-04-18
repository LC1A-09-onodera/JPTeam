#pragma once
#include "../3DModel/Model.h"
#include "../DX12operator.h"
#include <list>


enum OthelloType
{
	NORMAL,
	HARF,
	NONE,
};
namespace
{
	struct panelPos
	{
		int x;
		int y;
	};
}

struct OthelloData
{
	//�z��ɂԂ�����
	int widthPos = 0;
	int heightPos = 0;


	//��p�̃^�C�v���K�v��������Ȃ�
	OthelloType type = NONE;


	//�\ = ��
	bool isFront = true;

	//����Ȍ������o�Ă�����
	bool isUp = true;
	bool isRight = true;

	//��܂̌���
	list<bool> FrontActiveAngle;

	//���ꂩ������Ȃ���
	bool isPlayer = false;


	bool isMove = false;


	list<panelPos> panels;
	OthelloData();

	int comboCount = 0;
	bool isReverce = false;
	bool isSandwich = false;

	int animationTimer = 0;
	int waitTimer = 0;
	int JumpTimer = 0;
	bool isJumpUp = false;

	//���]�������Ȃ�����
	bool isVanish = false;

	//���ł���������
	bool isDead = false;

	bool isHarf = false;

	panelPos oldPos = { 0, 0 };
	int vanishTimer = 0;
	vector<int> SandwichLength;

	int score = 0;

	bool isSpawn = false;
	int spawnTimer = 0;
};
struct SendOthelloData
{
	OthelloType type = NONE;
	bool isFront = true;
	list<bool> FrontActiveAngle;
	bool isMove = false;
	int comboCount = 0;
	bool isSandwich = false;
	vector<int> SandwichLength;
	int score = 0;
};

namespace OthelloConstData
{//�萔
	const int fieldSize = 8;
	const float cellScale = 1.0f;
	const XMFLOAT3 stageLeftTop{ -cellScale * fieldSize, cellScale * fieldSize, 0 };
	const int spawnTimerMAx = 300;
	const int spawnMoveCount = 100;
	const int spawnPanelCount = 2;
	const int minPanelCount = 15;
	//�A�j���[�V����
	const int vanishTimerMax = 600;
	const int animationTimerMax = 30;
	const int waitTimerMax = 30;
	const int JumpTimerMax = waitTimerMax / 2;
	const int SpawnAnimationTimerMax = 180;
}

class Othello
{
public:

private:
	EachInfo each;
	XMFLOAT3 startPos;
	XMFLOAT3 endPos;
	Model *model;
	float startAngle;
	float endAngle;
	float time;
	bool isEase;
	OthelloData data;
public:
	OthelloData *GetGameData() { return &data; }
	bool GetIsEase() { return isEase; }
	XMFLOAT3 GetPosition() { return ConvertXMVECTORtoXMFLOAT3(each.position); }
	void SetIsEase(bool isEase) { this->isEase = isEase; }
	void SetPosition(XMFLOAT3 &position) { this->each.position = ConvertXMFLOAT3toXMVECTOR(position); }

public:
	void Init(Model *model);
	void Update();
	void Draw();
	void Finalize();

	void Spawn(OthelloType type, int x, int y, bool isFront = true);

	void Borne(OthelloType type, int x, int y, bool isFront = true);
	void Controll(const XMFLOAT3 &mousePos, int &moveCount);

	//�Ђ�����Ԃ�
	void Revers();

	void Sandwich();
	void SinkWait();
	void Sink();

	void MakeParticle();

	bool GetIsActive();

	void SpawnUpdate();
private:
	void ReversUpdate();
	void LeftRevers();
};

class OthelloModel : public Model
{
};

class OthelloManager
{
public:
	void Init();
	void Update();
	void Draw();
	void Finalize();
	void Controll();
	void AddPanel();

	const vector<vector<SendOthelloData>> &Send();
	void Receive(const vector<vector<SendOthelloData>> &data);

	void MinSpawn();
	static list<Othello> othellos;
	void DeadPanel();

	void StartSetPos();

	void AllDeadPanel();
	
private:
	void SetPlayer();

	void KeySetPlayer();
	void Move(const XMFLOAT3 &MousePos);
	void RemovePlayer();
	void SetPanel();

	void RandumSetPanel();

	void SpawnPanel(bool isInGame);

	void MauseControll();
	void PlayerControll();


	void playerMoveEnd();

	void playerNotMove();
	
private:

	/// <summary>
	/// ���R�ɓ�������
	/// </summary>
	void TypeA(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y);

	/// <summary>
	/// �����Ă�r���̋�ɂ͏��Ȃ�
	/// </summary>
	void TypeB(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y);

	/// <summary>
	/// �����Ă�r���̋�̑傫�����������Ȃ�ƒʂ�Ȃ��Ȃ�
	/// </summary>
	void TypeC(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y);

	/// <summary>
	/// TypeC�̋t
	/// </summary>
	void TypeD(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y);

	/// <summary>
	/// XI[sai]�̂悤�ɏ��������̋��o���
	/// </summary>
	void TypeXI(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y);
private:

	panelPos playerPanelPos;
	int spawnTimer = 0;
	int moveCount = 0;
	XMFLOAT3 mousePoint;
	
	static OthelloModel oserroModel;
	static vector<vector<SendOthelloData>> sendDatas;

	bool isOnPanel = true;
	bool isOldOnPanel = true;

	bool isPanelMove = false;
	bool isPlayerEnd = true;
};