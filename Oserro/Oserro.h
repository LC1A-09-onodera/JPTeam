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
struct OthelloData
{
	int widthPos = 0;
	int heightPos = 0;
	//専用のタイプが必要かもしれない
	OthelloType type = NORMAL;
	//表 = 白
	bool IsFront = true;
	bool isUp = true;
	bool isRight = true;
	list<bool> FrontActiveAngle;

	bool isPlayer = false;
	bool isMove = false;
	OthelloData();
};
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

	void Spawn(OthelloType type, int x, int y);

	void Controll(const XMFLOAT3 &mousePos);

	//ひっくり返す
	void Revers();
private:
		void RightRevers();
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
private:
	void SetPlayer();
	void Move();
	void RemovePlayer();
private:
	XMFLOAT3 mousePoint;
	static list<Othello> othellos;
	static OthelloModel oserroModel;
};