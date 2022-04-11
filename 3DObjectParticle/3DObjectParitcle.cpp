#include "3DObjectParticle.h"
#include "../DX12operator.h"
#include "../Shader/ShaderManager.h"
#include "../WindowsAPI/WinAPI.h"
ObjectParticle ObjectParticle3D::object;
list<ObjectParticle3D> ObjectParticles::particles;
list<list<ObjectParticle3D>::iterator> ObjectParticles::deleteItr;

void ObjectParticle3D::LoadObject()
{
	object.CreateModel("Triangle", ShaderManager::playerShader);
}

void ObjectParticle3D::Add(XMFLOAT3& emitter, ParticleType type)
{
	if (type == ParticleType::Exprotion)
	{
		InitExprotion(emitter);
		this->type = type;
	}
	else if (type == ParticleType::Converge)
	{
		InitConverge(emitter);
		this->type = type;
	}
	else if (type == ParticleType::TITLE)
	{
		InitTitle();
		this->type = type;
	}
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
}

void ObjectParticle3D::Draw()
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
	each.scale = {0.2f, 0.2f, 0.2f};
	endPosition = emitter;
	easeTime = 0;
	time = 1;
}

void ObjectParticle3D::InitTitle()
{
	int xSub = (rand() % 10 + 5.0f);
	if (rand() % 2 == 0)
	{
		xSub = -xSub;
	}
	each.position.m128_f32[0] = rand() % 40 - 20;
	int ySub = (rand() % 5 + 5.0f);
	if (rand() % 2 == 0)
	{
		ySub = -ySub;
	}
	each.position.m128_f32[1] = rand() % 20 - 10;
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

void ObjectParticle3D::UpdateExprotion()
{
	XMFLOAT3 nowPosition = ConvertXMVECTORtoXMFLOAT3(each.position);
	nowPosition = nowPosition + speed;
	speed = speed - acc;
	each.position = ConvertXMFLOAT3toXMVECTOR(nowPosition);
	each.rotation = each.rotation + addRotation;
	each.scale.x -= 0.01f;
	each.scale.y -= 0.01f;
	each.scale.z -= 0.01f;
	time--;
}

void ObjectParticle3D::UpdateConverge()
{
	each.position = ConvertXMFLOAT3toXMVECTOR(EaseOutQuad(startPosition, endPosition, easeTime));
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
	else if(!isSize)
	{
		isSize = true;
	}
	else if (each.scale.x > 0.0f)
	{
		each.scale.x -= 0.01f;
		each.scale.y -= 0.01f;
		each.scale.z -= 0.01f;
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

void ObjectParticles::Init(XMFLOAT3& emitter, int count, ParticleType type)
{
	for (int i = 0; i < count; i++)
	{
		ObjectParticle3D element;
		element.Add(emitter, type);
		particles.push_back(element);
	}
}

void ObjectParticles::Update()
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

void ObjectParticles::Draw()
{
	for (auto itr = particles.begin(); itr != particles.end(); ++itr)
	{
		itr->Draw();
	}
}

void ObjectParticles::DeleteAllParticle()
{
	particles.clear();
	deleteItr.clear();
}
