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
	
	XMFLOAT3 speed;
	XMFLOAT3 acc;
	XMFLOAT3 addRotation;
public:
	static void LoadObject();
	void Add(XMFLOAT3& emitter);
	void Update();
	void Draw();
	int time;
};
class ObjectParticles
{
	static list<ObjectParticle3D> particles;
	static list<list<ObjectParticle3D>::iterator> deleteItr;
public:
	static void Init(XMFLOAT3& emitter, int count);
	static void Update();
	static void Draw();
};