#pragma once
#include "../3DModel/Model.h"
#include "../Shader/ShaderManager.h"

class ThunderEachInfo : public EachInfo
{
public:
	float time;
	void CreateConstBuff0();
};
class ThunderModel : public Model
{
public:
	void Update(ThunderEachInfo* each);
};
class ThunderConstBuffer : public ConstBufferDataB0
{
public:
	float time;
};

class Thunder
{
public:
	ThunderEachInfo each;
	XMFLOAT3 startPos;
	XMFLOAT3 endPos;
	int number = 0;
	const float ThunderMaxTime = 1.0f;
	bool isActive = false;
	void Init(XMFLOAT3& position);
	void Update();
	void Shake();
};

class ThunderModels
{
public:
	static ThunderModel thunder1;
	static ThunderModel thunder2;
	static ThunderModel thunder3;
	static list<Thunder> thunders;
	static list<list<Thunder>::iterator> deleteThunders;
	static void LoadModels();
	static void Init(XMFLOAT3 &position, XMFLOAT3 &rotation);
	static void Update();
	static void Draw();
	static void DeleteList();
};