#include "3DObjectParticle.h"
#include "../DX12operator.h"
#include "../Shader/ShaderManager.h"
ObjectParticle ObjectParticle3D::object;
list<ObjectParticle3D> ObjectParticles::particles;
void ObjectParticle3D::LoadObject()
{
	object.CreateModel("Triangle", ShaderManager::playerShader);
}

void ObjectParticle3D::Add(XMFLOAT3& emitter)
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

void ObjectParticle3D::Update()
{
	XMFLOAT3 nowPosition = ConvertXMVECTORtoXMFLOAT3(each.position);
	nowPosition = nowPosition + speed;
	speed = speed - acc;
	each.position = ConvertXMFLOAT3toXMVECTOR(nowPosition);
	each.rotation = each.rotation + addRotation;
	time--;
}

void ObjectParticle3D::Draw()
{
	object.Update(&each);
	Draw3DObject(object);
}

void ObjectParticles::Init(XMFLOAT3& emitter, int count)
{
	for (int i = 0; i < count; i++)
	{
		ObjectParticle3D element;
		element.Add(emitter);
		particles.push_back(element);
	}
}

void ObjectParticles::Update()
{
	for (auto itr = particles.begin(); itr != particles.end(); ++itr)
	{
		itr->Update();
	}
}

void ObjectParticles::Draw()
{
	for (auto itr = particles.begin(); itr != particles.end(); ++itr)
	{
		itr->Draw();
	}
}
