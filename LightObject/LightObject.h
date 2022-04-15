#pragma once
#include "../3DModel/Model.h"
#include "../CheakOthello/CheakOthello.h"

class LightObjectEachInfo : public EachInfo
{
public:
	float time;
	void CreateConstBuff0();
};
class LightObjectConstBuffer : public ConstBufferDataB0
{
public:
	float time;
};

class LightObjectModel : public Model
{
public:
	void Update(LightObjectEachInfo* each);
};

class LightObjectModels
{
public:
	LightObjectEachInfo each1;
	LightObjectEachInfo each2;
	LightObjectEachInfo each3;
	const float alpha1 = 0.2f;
	const float alpha2 = 0.2f;
	const float alpha3 = 0.2f;
	void Init(const pair<int, int>& start,const pair<int, int>& end);
	void Update();
};


class Lights
{
public:
	static list<LightObjectModels> eachs;
	static LightObjectModel light1;
	static LightObjectModel light2;
	static LightObjectModel light3;
	static void LoadModels();
	static void Add(CheakOthello& othellos);
	static void Update();
	static void Draw();
};