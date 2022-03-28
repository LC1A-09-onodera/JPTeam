#pragma once
#include "../3DModel/Model.h"
#include "../DX12operator.h"

class Oserro
{
private:
	EachInfo each;
	XMFLOAT3 startPos;
	XMFLOAT3 endPos;
	float startAngle;
	float endAngle;
	float time;
	bool isEase;
public:
	bool GetIsEase(){ return isEase; }
	XMFLOAT3 GetPosition() { return ConvertXMVECTORtoXMFLOAT3(each.position); }
	void SetIsEase(bool isEase) { this->isEase = isEase; }
	void SetPosition(XMFLOAT3 &position) { this->each.position = ConvertXMFLOAT3toXMVECTOR(position); }
};

class OserroModel : public Model
{

};

class OserroManager
{
public:
	static list<Oserro> oserros;
	static OserroModel oserroModel;
};