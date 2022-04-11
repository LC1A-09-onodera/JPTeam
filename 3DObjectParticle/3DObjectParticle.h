#pragma once
#include "../3DModel/Model.h"

enum class ParticleType
{
	Exprotion,
	Converge,
	TITLE,
};

class ObjectParticle : public Model
{

};
class ObjectParticle3D
{
	EachInfo each;
	const int Life = 120;
	
	XMFLOAT3 speed;
	XMFLOAT3 acc;
	XMFLOAT3 addRotation;
	XMFLOAT3 startPosition;
	XMFLOAT3 endPosition;
	const float addTime = 0.002f;
	float easeTime;
	bool isSize;
	ParticleType type;
public:
	void Add(XMFLOAT3& emitter, ParticleType type);
	void Update();
	void Draw(ObjectParticle &object);
	void InitExprotion(XMFLOAT3& emitter);
	void InitConverge(XMFLOAT3& emitter);
	void InitTitle();
	void UpdateExprotion();
	void UpdateConverge();
	void UpdateTitle();
	int time;
};

class ObjectParticleInfo
{
	list<ObjectParticle3D> particles;
	list<list<ObjectParticle3D>::iterator> deleteItr;
public:
	ObjectParticle object;
	void Init(XMFLOAT3& emitter, int count, ParticleType type);
	void Update();
	void Draw(ObjectParticle &object);
	void DeleteAllParticle();
	
};

class ObjectParticles
{
public:
	static ObjectParticleInfo triangle;
	static ObjectParticleInfo othello;
	static void LoadModels();
	static void Update();
	static void Draw();
	static void DeleteAllParticles();
};