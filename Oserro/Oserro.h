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
	//配列にぶち込む
	int widthPos = 0;
	int heightPos = 0;


	//専用のタイプが必要かもしれない
	OthelloType type = NONE;


	//表 = 青
	bool isFront = true;

	//特殊な向きが出てきたら
	bool isUp = true;
	bool isRight = true;

	//大賞の向き
	list<bool> FrontActiveAngle;

	//おれか俺じゃないか
	bool isPlayer = false;


	bool isMove = false;


	list<panelPos> panels;
	OthelloData();

	int comboCount = 0;
	bool isReverce = false;

	int animationTimer = 0;
	int waitTimer = 0;
	int JumpTimer = 0;
	bool isJumpUp = false;

	//反転をおこなったか
	bool isVanish = false;

	//消滅しきったか
	bool isDead = false;


	panelPos oldPos = {0, 0};
	int vanishTimer = 0;
};
struct SendOthelloData
{
	OthelloType type = NONE;
	bool isFront = true;
	list<bool> FrontActiveAngle;
	bool isMove = false;
	int comboCount = 0;
};

namespace OthelloConstData
{//定数
	const int fieldSize = 8;
	const float cellScale = 1.0f;
	const XMFLOAT3 stageLeftTop{ -cellScale * fieldSize, cellScale * fieldSize, 0 };
	const int animationTimerMax = 30;
	const int waitTimerMax = 30;
	const int JumpTimerMax = waitTimerMax / 2;
	const int spawnTimerMAx = 20000;
	const int vanishTimerMax = 600;
	const int spawnMoveCount = 2000;
	const int spawnPanelCount = 2;
	const int minPanelCount = 30;
}

class Othello
{
public:

private:
	EachInfo each;
	XMFLOAT3 startPos;
	XMFLOAT3 endPos;
	Model* model;
	float startAngle;
	float endAngle;
	float time;
	bool isEase;
	OthelloData data;
	XMVECTOR qRotation;
public:
	OthelloData* GetGameData() { return &data; }
	bool GetIsEase() { return isEase; }
	XMFLOAT3 GetPosition() { return ConvertXMVECTORtoXMFLOAT3(each.position); }
	void SetIsEase(bool isEase) { this->isEase = isEase; }
	void SetPosition(XMFLOAT3& position) { this->each.position = ConvertXMFLOAT3toXMVECTOR(position); }

public:
	void Init(Model* model);
	void Update();
	void Draw();
	void Finalize();

	void Spawn(OthelloType type, int x, int y, bool isFront = true);

	void Controll(const XMFLOAT3& mousePos, int& moveCount);

	//ひっくり返す
	void Revers();

	void Sink();

	void MakeParticle();
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

	const vector<vector<SendOthelloData>>& Send();
	void Receive(const vector<vector<SendOthelloData>>& data);

	void MinSpawn();
private:
	void SetPlayer();

	void KeySetPlayer();
	void Move(const XMFLOAT3 &MousePos);
	void RemovePlayer();
	void SetPanel();

	void RandumSetPanel();

	void DeadPanel();

	void SpawnPanel();

	void MauseControll();
	void PlayerControll();

	void SartSetPos();

	void playerMoveEnd();

	void playerNotMove();
private:

	panelPos playerPanelPos;
	int spawnTimer = 0;
	int moveCount = 0;
	XMFLOAT3 mousePoint;
	static list<Othello> othellos;
	static OthelloModel oserroModel;
	static vector<vector<SendOthelloData>> sendDatas;

	bool isOnPanel = true;
	bool isOldOnPanel = true;

	bool isPanelMove = false;
	bool isPlayerEnd = true;
};