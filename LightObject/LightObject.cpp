#include "../LightObject/LightObject.h"
#include "../Shader/ShaderManager.h"
#include "../Camera/Camera.h"
#include "../CheakOthello/CheakOthello.h"
list<LightObjectModels> Lights::eachs;
list<list<LightObjectModels>::iterator> Lights::deleteItr;
LightObjectModel Lights::light1;
LightObjectModel Lights::light2;
LightObjectModel Lights::light3;
void LightObjectEachInfo::CreateConstBuff0()
{
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(LightObjectConstBuffer) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff0));
}

void LightObjectModels::Init(const pair<int, int>& start, const pair<int, int>& end)
{
	float startY = start.first;
	float startX = start.second;
	float endY = end.first;
	float endX = end.second;
	bool hol = startY == endY;
	bool ver = startX == endX;
	float centerX;
	float centerY;
	//斜め処理
	if (!hol && !ver)
	{
		//左上方向
		if ((startX > endX && startY > endY) || (startX < endX && startY < endY))
		{
			each1.rotation.z = 45;
			each2.rotation.z = 45;
			each3.rotation.z = 45;
		}
		else
		{
			each1.rotation.z = -45;
			each2.rotation.z = -45;
			each3.rotation.z = -45;
		}
		float subX = abs(startX - endX);
		float subY = abs(startY - endY);
		if (startX > endX)
		{
			centerX = startX - subX / 2.0f;
		}
		else
		{
			centerX = startX + subX / 2.0f;
		}
		if (startY > endY)
		{
			centerY = startY - subY / 2.0f;
		}
		else
		{
			centerY = startY + subY / 2.0f;
		}
		centerX = centerX * 2.0f - 8.0f;
		centerY = centerY * 2.0f - 8.0f;
		//each1.scale.y = subX;
		startScale = { 0.1f, 0.1f, 0.1f };
		endScale = { 0.85f, subX, 0.5f };
		time = 0;
	}
	else if (!hol || !ver)
	{
		//横方向
		if (!hol)
		{
			//差分
			float sub = abs(startY - endY);
			if (startY > endY)
			{
				centerY = startY - sub / 2.0f;
			}
			else
			{
				centerY = startY + sub / 2.0f;
			}
			centerX = startX;
			startScale = {0.1f, 0.1f, 0.1f};
			//endScale = {1.9f, sub, 0.8f};
			endScale = { 1.0f, sub, 0.5f };
			time = 0;
			
		}
		//縦方向
		else
		{
			//差分
			float sub = abs(startX - endX);
			if (startX > endX)
			{
				centerX = startX - sub / 2.0f;
			}
			else
			{
				centerX = startX + sub / 2.0f;
			}
			centerY = startY;
			each1.rotation.z = 90;
			each2.rotation.z = 90;
			each3.rotation.z = 90;
			startScale = { 0.1f, 0.1f, 0.1f };
			//endScale = { 1.9f, sub, 0.8f };
			endScale = { 0.85f, sub, 0.5f };
			time = 0;
		}
		centerX = centerX * 2.0f - 8.0f;
		centerY = centerY * 2.0f - 8.0f;
	}
	each1.position.m128_f32[0] = centerX;
	each1.position.m128_f32[1] = -centerY;
	each1.position.m128_f32[2] = 0;
	each2.position.m128_f32[0] = centerX;
	each2.position.m128_f32[1] = -centerY;
	each2.position.m128_f32[2] = 0;
	each3.position.m128_f32[0] = centerX;
	each3.position.m128_f32[1] = -centerY;
	each3.position.m128_f32[2] = 0;
	each1.time = alpha1;
	each2.time = alpha2;
	each3.time = alpha3;
	//each2.scale = each1.scale * 0.8f;
	//each3.scale = each1.scale * 0.6f;
	each2.scale = each1.scale * 0.9f;
	each3.scale = each1.scale * 0.9f;
	each1.CreateConstBuff0();
	each1.CreateConstBuff1();
	each2.CreateConstBuff0();
	each2.CreateConstBuff1();
	each3.CreateConstBuff0();
	each3.CreateConstBuff1();
}

void LightObjectModels::Update()
{
	if (time > 1.0f)
	{
		time += 0.05f;
	}
	else
	{
		each1.scale = ShlomonMath::EaseOutQuad(startScale, endScale, time);
		each2.scale = each1.scale * 0.9f;
		each3.scale = each2.scale * 0.85f;
		time += 0.05f;
	}
}

void LightObjectModel::Update(LightObjectEachInfo* each)
{
	if (each != nullptr)
	{
		this->each = *each;
		XMMATRIX matScale, matRot, matTrans;
		const XMFLOAT3& cameraPos = Camera::eye.v;
		matScale = XMMatrixScaling(this->each.scale.x, this->each.scale.y, this->each.scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(this->each.rotation.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(this->each.rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(this->each.rotation.y));
		matTrans = XMMatrixTranslation(this->each.position.m128_f32[0], this->each.position.m128_f32[1], this->each.position.m128_f32[2]);
		matWorld = XMMatrixIdentity();

		//ビルボード
		//if (billboard)
		//{
		//    matWorld *= BaseDirectX::matBillboard;//ビルボードをかける
		//}
		//ビルボードY
		//if (billboard)
		//{
		//    matWorld *= Camera::matBillboardY;//ビルボードをかける
		//}
		matWorld *= matScale;
		matWorld *= matRot;
		matWorld *= matTrans;

		Vertex* vertMap = nullptr;
		BaseDirectX::result = mesh.vertBuff->Map(0, nullptr, (void**)&vertMap);
		if (SUCCEEDED(BaseDirectX::result))
		{
			copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
			mesh.vertBuff->Unmap(0, nullptr);    // マップを解除
		}

		LightObjectConstBuffer* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{
			//constMap0->mat = matWorld * Camera::matView * BaseDirectX::matProjection;
			constMap0->viewproj = Camera::matView * BaseDirectX::matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = cameraPos;
			constMap0->time = each->time;
			this->each.constBuff0->Unmap(0, nullptr);
		}

		ConstBufferDataB1* constMap1 = nullptr;
		BaseDirectX::result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);

	}
	else
	{
		XMMATRIX matScale, matRot, matTrans;
		const XMFLOAT3& cameraPos = Camera::eye.v;
		matScale = XMMatrixScaling(this->each.scale.x, this->each.scale.y, this->each.scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(this->each.rotation.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(this->each.rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(this->each.rotation.y));
		matTrans = XMMatrixTranslation(this->each.position.m128_f32[0], this->each.position.m128_f32[1], this->each.position.m128_f32[2]);
		matWorld = XMMatrixIdentity();

		//ビルボード
		//if (billboard)
		//{
		//    matWorld *= BaseDirectX::matBillboard;//ビルボードをかける
		//}
		//ビルボードY
		//if (billboard)
		//{
		//    matWorld *= Camera::matBillboardY;//ビルボードをかける
		//}
		matWorld *= matScale;
		matWorld *= matRot;
		matWorld *= matTrans;

		Vertex* vertMap = nullptr;
		BaseDirectX::result = mesh.vertBuff->Map(0, nullptr, (void**)&vertMap);
		if (SUCCEEDED(BaseDirectX::result))
		{
			copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
			mesh.vertBuff->Unmap(0, nullptr);    // マップを解除
		}

		LightObjectConstBuffer* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{
			//constMap0->mat = matWorld * Camera::matView * BaseDirectX::matProjection;
			constMap0->viewproj = Camera::matView * BaseDirectX::matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = cameraPos;
			this->each.constBuff0->Unmap(0, nullptr);
		}

		ConstBufferDataB1* constMap1 = nullptr;
		BaseDirectX::result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);
	}
}

void Lights::LoadModels()
{
	light1.CreateModel("LightObj3", ShaderManager::thunderShader);
	light2.CreateModel("LightObj3", ShaderManager::thunderShader);
	light3.CreateModel("LightObj3", ShaderManager::thunderShader);
}

void Lights::Add(CheakOthello& othellos)
{
	for (int i = 0; i < othellos.GetStartAndEndArrayDatas().size(); i += 2)
	{
		LightObjectModels each;
		each.Init(othellos.GetStartAndEndArrayDatas()[i], othellos.GetStartAndEndArrayDatas()[i + (int)1]);
		eachs.push_back(each);
	}
	othellos.ResetStartAndEndArrayDatas();
}

void Lights::Update()
{
	for (auto itr = eachs.begin(); itr != eachs.end(); ++itr)
	{
		itr->Update();
		if (itr->time > 3.0f)
		{
			deleteItr.push_back(itr);
		}
	}
	for (auto itr = deleteItr.begin(); itr != deleteItr.end(); ++itr)
	{
		eachs.erase(*itr);
	}
	deleteItr.clear();
}

void Lights::Draw()
{
	for (auto itr = eachs.begin(); itr != eachs.end(); ++itr)
	{
		light3.Update(&itr->each3);
		Draw3DObject(light3);
		light2.Update(&itr->each2);
		Draw3DObject(light2);
		light1.Update(&itr->each1);
		Draw3DObject(light1);
	}
}
