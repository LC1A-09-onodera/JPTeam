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
	const XMFLOAT3 stageLeftTop{ -cellScale * fieldSize / 2, cellScale * fieldSize / 2, 0 };
	const int animationTimerMax = 30;
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
	XMVECTOR qRotation;
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

	void Controll(const XMFLOAT3 &mousePos);

	//ひっくり返す
	void Revers();
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

private:
	void SetPlayer();
	void Move();
	void RemovePlayer();
	void SetPanel();
private:
	XMFLOAT3 mousePoint;
	static list<Othello> othellos;
	static OthelloModel oserroModel;
	static vector<vector<SendOthelloData>> sendDatas;
};