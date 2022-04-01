#pragma once
#include "../3DModel/Model.h"

class ObjectParticle : public Model
{

};
class ObjectParticle3D
{
	static ObjectParticle object;
	EachInfo each;
	const int Life = 120;
	int time;
	XMFLOAT3 speed;
	XMFLOAT3 acc;
	XMFLOAT3 addRotation;
public:
	static void LoadObject();
	void Add(XMFLOAT3& emitter);
	void Update();
	void Draw();
};
class ObjectParticles
{
	static list<ObjectParticle3D> particles;
public:
	static void Init(XMFLOAT3& emitter, int count);
	static void Update();
	static void Draw();
};