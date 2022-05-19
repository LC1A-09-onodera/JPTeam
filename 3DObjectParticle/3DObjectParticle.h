#pragma once
#include "../3DModel/Model.h"

enum class ParticleType
{
	Exprotion,
	Converge,
	TITLE,
	Swell,
	Target,
	Tornado,
	Born,
	BornAndShake,
	Combo,
	ComboNum
};

class ObjectParticle : public Model
{
public:
	void CreateModel(const char* name, HLSLShader& shader, bool smoothing = false, bool addAlpha = true);
	bool InitializeGraphicsPipeline(HLSLShader& shader);
};
class ObjectParticle3D
{

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
	float angle;
public:
	EachInfo each;
	void Add(XMFLOAT3& emitter, ParticleType type);
	void Add(XMFLOAT3& emitter, ParticleType type, XMFLOAT3 &size);
	void Update(list<XMFLOAT3> list);
	void Draw(ObjectParticle& object);
	void InitExprotion(XMFLOAT3& emitter);
	void InitConverge(XMFLOAT3& emitter);
	void InitTitle(XMFLOAT3& emitter);
	void InitSwell(XMFLOAT3& emitter);
	void InitTarget(XMFLOAT3 &emitter);
	void InitTornado(XMFLOAT3 &emitter);
	void InitBorn(XMFLOAT3 &emitter);
	void InitBornAndShake(XMFLOAT3 &emitter);
	void InitConbo(XMFLOAT3 &emitter);
	void InitConboNum(XMFLOAT3& emitter, XMFLOAT3 &size);
	void UpdateExprotion();
	void UpdateConverge();
	void UpdateTitle();
	void UpdateSwell();
	void UpdateTarget();
	void UpdateTornado();
	void UpdateBorn(list<XMFLOAT3> list);
	void UpdateBornAndShake();
	void UpdateCombo(list<XMFLOAT3> list);
	void UpdateConboNum();
	int time;
};

class ObjectParticleInfo
{
public:
	list<ObjectParticle3D> particles;
	list<list<ObjectParticle3D>::iterator> deleteItr;
	ObjectParticle object;
	void Init(XMFLOAT3& emitter, int count, ParticleType type);
	void Init(XMFLOAT3& emitter, int count, ParticleType type, XMFLOAT3 size);
	void Update(list<XMFLOAT3> list);
	void Draw(ObjectParticle& object);
	void DeleteAllParticle();

};

class ObjectParticles
{
public:
	static ObjectParticleInfo triangle;
	static ObjectParticleInfo othello;
	static ObjectParticleInfo frame;
	static ObjectParticleInfo othelloFrame;
	static ObjectParticleInfo six;
	static ObjectParticleInfo othello2;
	static void LoadModels();
	static void Update(list<XMFLOAT3> list);
	static void Draw();
	static void DeleteAllParticles();
};
