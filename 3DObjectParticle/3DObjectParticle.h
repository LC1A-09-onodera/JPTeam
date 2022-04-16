#pragma once
#include "../3DModel/Model.h"

enum class ParticleType
{
	Exprotion,
	Converge,
};

class ObjectParticle : public Model
{

};
class ObjectParticle3D
{
	static ObjectParticle object;
	EachInfo each;
	const int Life = 120;
	
	XMFLOAT3 speed;
	XMFLOAT3 acc;
	XMFLOAT3 addRotation;
	XMFLOAT3 startPosition;
	XMFLOAT3 endPosition;
	const float addTime = 0.002f;
	float easeTime;

	ParticleType type;
public:
	static void LoadObject();
	void Add(XMFLOAT3& emitter, ParticleType type);
	void Update();
	void Draw();
	void InitExprotion(XMFLOAT3& emitter);
	void InitConverge(XMFLOAT3& emitter);
	void UpdateExprotion();
	void UpdateConverge();
	int time;
};
class ObjectParticles
{
	static list<ObjectParticle3D> particles;
	static list<list<ObjectParticle3D>::iterator> deleteItr;
public:
	static void Init(XMFLOAT3& emitter, int count, ParticleType type);
	static void Update();
	static void Draw();
};