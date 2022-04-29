#include "3DObjectParticle.h"
#include "../DX12operator.h"
#include "../Shader/ShaderManager.h"
#include "../WindowsAPI/WinAPI.h"

ObjectParticleInfo ObjectParticles::triangle;
ObjectParticleInfo ObjectParticles::othello;
ObjectParticleInfo ObjectParticles::frame;
ObjectParticleInfo ObjectParticles::othelloFrame;
ObjectParticleInfo ObjectParticles::combo;
void ObjectParticle3D::Add(XMFLOAT3& emitter, ParticleType type)
{
	if (type == ParticleType::Exprotion)
	{
		InitExprotion(emitter);
	}
	else if (type == ParticleType::Converge)
	{
		InitConverge(emitter);
	}
	else if (type == ParticleType::TITLE)
	{
		InitTitle(emitter);
	}
	else if (type == ParticleType::Swell)
	{
		InitSwell(emitter);
	}
	else if (type == ParticleType::Target)
	{
		InitTarget(emitter);
	}
	else if (type == ParticleType::Tornado)
	{
		InitTornado(emitter);
	}
	else if (type == ParticleType::Born)
	{
		InitBorn(emitter);
	}
	else if (type == ParticleType::BornAndShake)
	{
		InitBornAndShake(emitter);
	}
	else if (type == ParticleType::Combo)
	{
		InitConbo(emitter);
	}
	this->type = type;
}

void ObjectParticle3D::Update()
{
	if (this->type == ParticleType::Exprotion)
	{
		UpdateExprotion();
	}
	else if (this->type == ParticleType::Converge)
	{
		UpdateConverge();
	}
	else if (this->type == ParticleType::TITLE)
	{
		UpdateTitle();
	}
	else if (type == ParticleType::Swell)
	{
		UpdateSwell();
	}
	else if (type == ParticleType::Target)
	{
		UpdateTarget();
	}
	else if (type == ParticleType::Tornado)
	{
		UpdateTornado();
	}
	else if (type == ParticleType::Born)
	{
		UpdateBorn();
	}
	else if (type == ParticleType::BornAndShake)
	{
		UpdateBornAndShake();
	}
	else if (type == ParticleType::Combo)
	{
		UpdateCombo();
	}
}

void ObjectParticle3D::Draw(ObjectParticle& object)
{
	object.Update(&each);
	Draw3DObject(object);
}

void ObjectParticle3D::InitExprotion(XMFLOAT3& emitter)
{
	time = Life;
	each.position = ConvertXMFLOAT3toXMVECTOR(emitter);
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	speed = GetRandom(2.0f);
	acc = GetRandom(1.0f);
	speed.x = speed.x / 10.0f;
	speed.y = speed.y / 10.0f;
	speed.z = speed.z / 10.0f;
	acc.x = acc.x / 400.0f;
	acc.y = acc.y / 400.0f;
	acc.z = acc.z / 400.0f;
	addRotation = GetRandom(2.0f);
}

void ObjectParticle3D::InitConverge(XMFLOAT3& emitter)
{
	int xSub = (rand() % 10 + 13.0f);
	if (rand() % 2 == 0)
	{
		xSub = -xSub;
	}
	each.position.m128_f32[0] = emitter.x + xSub;
	int ySub = (rand() % 5 + 13.0f);
	if (rand() % 2 == 0)
	{
		ySub = -ySub;
	}
	each.position.m128_f32[1] = emitter.x + ySub;
	int zSub = (rand() % 10 + 13.0f);
	if (rand() % 2 == 0)
	{
		zSub = -zSub;
	}
	each.position.m128_f32[2] = emitter.x + zSub;
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	startPosition = ConvertXMVECTORtoXMFLOAT3(each.position);
	each.scale = { 0.2f, 0.2f, 0.2f };
	endPosition = emitter;
	easeTime = 0;
	time = 1;
}

void ObjectParticle3D::InitTitle(XMFLOAT3& emitter)
{
	each.position = ConvertXMFLOAT3toXMVECTOR(emitter);
	each.rotation = GetRandom(90);
	int xSub = (rand() % 10 + 5.0f);
	if (rand() % 2 == 0)
	{
		xSub = -xSub;
	}
	each.position.m128_f32[0] += rand() % 40 - 20;
	int ySub = (rand() % 5 + 5.0f);
	if (rand() % 2 == 0)
	{
		ySub = -ySub;
	}
	each.position.m128_f32[1] += rand() % 20 - 10;
	int zSub = (rand() % 10 + 5.0f);
	if (rand() % 2 == 0)
	{
		zSub = -zSub;
	}
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	speed = GetRandom(2.0f);
	acc = GetRandom(1.0f);
	startPosition = ConvertXMVECTORtoXMFLOAT3(each.position);
	speed.x = speed.x / 150.0f;
	speed.y = speed.y / 150.0f;
	speed.z = speed.z / 150.0f;
	acc.x = acc.x / 500.0f;
	acc.y = acc.y / 500.0f;
	acc.z = acc.z / 500.0f;
	each.scale = { 0.1f, 0.1f, 0.1f };
	easeTime = 0;
	isSize = false;
	time = 600;
}

void ObjectParticle3D::InitSwell(XMFLOAT3& emitter)
{
	time = Life * 2.0f;
	each.position = ConvertXMFLOAT3toXMVECTOR(emitter);
	each.position.m128_f32[2] = 30.0f;
	each.position.m128_f32[0] = rand() % 50 - 24;
	each.position.m128_f32[1] = rand() % 50 - 24;
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	speed.z = -rand() % 5 + 1;
	acc.z = -rand() % 5 + 1;
	speed.z = speed.z / 10.0f;
	acc.z = acc.z / 400.0f;
	addRotation = GetRandom(2.0f);
}

void ObjectParticle3D::InitTarget(XMFLOAT3& emitter)
{
	time = 1;
	each.position = ConvertXMFLOAT3toXMVECTOR(emitter);
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	endPosition = XMFLOAT3(-5, -5, 3);
	speed.x = (rand() % 3 + 1) / 10.0f;
	speed.y = (rand() % 3 + 1) / 10.0f;
	speed.z = (rand() % 3 + 1) / 10.0f;
	easeTime = 1.0f;
}

void ObjectParticle3D::InitTornado(XMFLOAT3& emitter)
{
	time = 1;
	//each.position = ConvertXMFLOAT3toXMVECTOR(emitter);
	startPosition = { 0, 0, 10 };
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	each.position.m128_f32[2] = 10;
	speed.z = rand() % 10 + 5;
	speed.z = -speed.z / 100.0f;
	angle = rand() % 360;
	easeTime = 1.0f;
}

void ObjectParticle3D::InitBorn(XMFLOAT3& emitter)
{
	time = 1;
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	each.position = ConvertXMFLOAT3toXMVECTOR(emitter);

	each.scale = { 0.1f, 0.1f, 0.1f };
	easeTime = 1.0f;
}

void ObjectParticle3D::InitBornAndShake(XMFLOAT3& emitter)
{
	time = 1;
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	each.position = ConvertXMFLOAT3toXMVECTOR(emitter);

	each.scale = { 1.0f, 1.0f, 6.0f };
	easeTime = 1.0f;
	startPosition = emitter;
}

void ObjectParticle3D::InitConbo(XMFLOAT3& emitter)
{
	time = 1;
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	each.position = ConvertXMFLOAT3toXMVECTOR(emitter);
	each.position.m128_f32[3] = 1;
	each.scale = { 1.0f, 1.0f, 1.0f };
	each.rotation = { 0, 0, 180 };
	easeTime = 1.0f;
	startPosition = emitter;
}

void ObjectParticle3D::UpdateExprotion()
{
	XMFLOAT3 nowPosition = ConvertXMVECTORtoXMFLOAT3(each.position);
	nowPosition = nowPosition + speed;
	speed = speed - acc;
	each.position = ConvertXMFLOAT3toXMVECTOR(nowPosition);
	each.rotation = each.rotation + addRotation;
	if (each.scale.x > 0)
	{
		each.scale.x -= 0.01f;
		each.scale.y -= 0.01f;
		each.scale.z -= 0.01f;
	}
	time--;
}

void ObjectParticle3D::UpdateConverge()
{
	each.position = ConvertXMFLOAT3toXMVECTOR(ShlomonMath::EaseOutQuad(startPosition, endPosition, easeTime));
	each.rotation.x += 3.0f;
	each.rotation.z += 2.0f;
	easeTime += addTime;
	if (easeTime >= 1.0f)
	{
		time = 0;
	}
}

void ObjectParticle3D::UpdateTitle()
{
	if (!isSize && each.scale.x <= 1.0f)
	{
		each.scale.x += 0.01f;
		each.scale.y += 0.01f;
		each.scale.z += 0.01f;
	}
	else if (!isSize)
	{
		isSize = true;
	}
	else if (each.scale.x > 0.0f)
	{
		each.scale.x -= 0.001f;
		each.scale.y -= 0.001f;
		each.scale.z -= 0.001f;
	}
	else
	{
		time = 0;
	}
	XMFLOAT3 nowPosition = ConvertXMVECTORtoXMFLOAT3(each.position);
	nowPosition = nowPosition + speed;
	speed = speed - acc;
	each.rotation.x += 1.0f;
	each.rotation.y += 1.0f;
	each.position = ConvertXMFLOAT3toXMVECTOR(nowPosition);
	time--;
}

void ObjectParticle3D::UpdateSwell()
{
	XMFLOAT3 nowPosition = ConvertXMVECTORtoXMFLOAT3(each.position);
	nowPosition = nowPosition + speed;
	speed = speed + acc;
	each.position = ConvertXMFLOAT3toXMVECTOR(nowPosition);
	each.rotation = each.rotation + addRotation;
	time--;
	if (each.scale.x > 0)
	{
		each.scale.x -= 0.002f;
		each.scale.y -= 0.002f;
		each.scale.z -= 0.002f;
	}
}

void ObjectParticle3D::UpdateTarget()
{
	easeTime -= 0.01f;
	XMFLOAT3 homing = ShlomonMath::Homing(ConvertXMVECTORtoXMFLOAT3(each.position), endPosition, speed);
	speed = homing / 1.0f;
	each.position = ConvertXMFLOAT3toXMVECTOR(ConvertXMVECTORtoXMFLOAT3(each.position) + speed);

	if (easeTime <= 0.0f)
	{
		time = 0;
	}
}

void ObjectParticle3D::UpdateTornado()
{
	startPosition.z += speed.z;
	if (angle >= 360)
	{
		angle = 0;
	}
	angle++;
	float R = 20.0f;
	each.position.m128_f32[0] = ShlomonMath::Cos(angle) * R;
	each.position.m128_f32[1] = ShlomonMath::Sin(angle) * R;
	each.position.m128_f32[2] = startPosition.z;
	each.rotation.x += 2.0f;
	each.rotation.z += 2.0f;
	if (each.position.m128_f32[2] < -10.0f)
	{
		time = 0;
	}
}

void ObjectParticle3D::UpdateBorn()
{

	if (each.scale.x >= 1.0f)
	{
		easeTime -= 0.02f;
		if (easeTime <= 0.0f)
		{
			time = 0;
		}
	}
	else
	{
		each.rotation.z += 1.0f;
		each.scale = each.scale + 0.01f;
	}
}

void ObjectParticle3D::UpdateBornAndShake()
{
	each.position = ConvertXMFLOAT3toXMVECTOR(startPosition);
	each.position.m128_f32[0] += (rand() % (int)(20.0f * easeTime + 1) - (9 * easeTime)) / 400.0f;
	each.position.m128_f32[1] += (rand() % (int)(20.0f * easeTime + 1) - (9 * easeTime)) / 400.0f;
	easeTime -= 0.01f;
	if (easeTime <= 0.0f)
	{
		time = 0;
	}
}

void ObjectParticle3D::UpdateCombo()
{
	easeTime -= 0.01f;
	each.scale = {1.0f,	1.0f, 1.0f};
	if (easeTime <= 0.0f)
	{
		time = 0;
	}
}

void ObjectParticleInfo::Init(XMFLOAT3& emitter, int count, ParticleType type)
{
	for (int i = 0; i < count; i++)
	{
		ObjectParticle3D element;
		element.Add(emitter, type);
		particles.push_back(element);
	}
}

void ObjectParticleInfo::Update()
{
	for (auto itr = particles.begin(); itr != particles.end(); ++itr)
	{
		itr->Update();
		if (itr->time <= 0)
		{
			deleteItr.push_back(itr);
		}
	}
	for (auto deleteitr = deleteItr.begin(); deleteitr != deleteItr.end(); ++deleteitr)
	{
		particles.erase(*deleteitr);
	}
	deleteItr.clear();
}

void ObjectParticleInfo::Draw(ObjectParticle& object)
{
	for (auto itr = particles.begin(); itr != particles.end(); ++itr)
	{
		itr->Draw(object);
	}
}

void ObjectParticleInfo::DeleteAllParticle()
{
	particles.clear();
	deleteItr.clear();
}

void ObjectParticles::LoadModels()
{
	triangle.object.CreateModel("Triangle", ShaderManager::playerShader);
	othello.object.CreateModel("newOserro", ShaderManager::playerShader);
	frame.object.CreateModel("Frame", ShaderManager::playerShader);
	othelloFrame.object.CreateModel("OthelloFrame", ShaderManager::othelloFrame);
	combo.object.CreateModel("plane", ShaderManager::playerShader);
}
                             
void ObjectParticles::Update()
{
	triangle.Update();
	othello.Update();
	frame.Update();
	othelloFrame.Update();
	combo.Update();
}

void ObjectParticles::Draw()
{
	triangle.Draw(triangle.object);
	othello.Draw(othello.object);
	frame.Draw(frame.object);
	othelloFrame.Draw(othelloFrame.object);
	combo.Draw(combo.object);
}

void ObjectParticles::DeleteAllParticles()
{
	triangle.DeleteAllParticle();
	othello.DeleteAllParticle();
	frame.DeleteAllParticle();
	othelloFrame.DeleteAllParticle();
	combo.DeleteAllParticle();
}
