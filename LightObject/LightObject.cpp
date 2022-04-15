#include "../LightObject/LightObject.h"
#include "../Shader/ShaderManager.h"
#include "../Camera/Camera.h"
#include "../CheakOthello/CheakOthello.h"
list<LightObjectModels> Lights::eachs;
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
	int startY = start.first;
	int startX = start.second;
	int endY = end.first;
	int endX = end.second;
	XMFLOAT3 resultStart;
	resultStart.x = startX * 2.0f - 8.0f;
	resultStart.y = startY * 2.0f + 8.0f;
	XMFLOAT3 resultEnd;
	resultEnd.x = endX * 2.0f - 8.0f;
	resultEnd.y = endY * 2.0f + 8.0f;
	if (resultStart.x > resultEnd.x)
	{
		resultStart.x -= (resultStart.x - resultEnd.x) / 2;
	}
	else if (resultStart.x < resultEnd.x)
	{
		resultStart.x += (resultStart.x - resultEnd.x) / 2;
	}
	if (resultStart.y > resultEnd.y)
	{
		resultStart.y += (resultStart.y - resultEnd.y) / 2;
	}
	else if (resultStart.y < resultEnd.y)
	{
		resultStart.y -= (resultStart.y - resultEnd.y) / 2;
	}
	resultStart.z = 0;
	each1.position = ConvertXMFLOAT3toXMVECTOR(resultStart);
	each1.time = alpha1;
	if (resultStart.x !=  resultEnd.x)
	{
		each1.scale.x = abs(resultStart.x - resultEnd.x) / 2.0f;
	}
	else
	{
		each1.scale.x = 0.8f;
	}
	if (resultStart.y != resultEnd.y)
	{
		each1.scale.y = abs(resultStart.y - resultEnd.y) / 2.0f;
	}
	else
	{
		each1.scale.y = 0.8f;
	}
	each1.scale.z = 0.1f;
	each2.position = ConvertXMFLOAT3toXMVECTOR(resultStart);
	//each2.position.m128_f32[0] += 2.0f;
	each2.time = alpha2;
	each2.scale = {each1.scale.x * 0.8f, each1.scale.y * 0.8f ,each1.scale.z * 0.8f };
	each3.position = ConvertXMFLOAT3toXMVECTOR(resultStart);
	//each3.position.m128_f32[0] += 4.0f;
	each3.time = alpha3;
	each3.scale = { each1.scale.x * 0.5f, each1.scale.y * 0.5f ,each1.scale.z * 0.5f };
	each1.CreateConstBuff0();
	each1.CreateConstBuff1();
	each2.CreateConstBuff0();
	each2.CreateConstBuff1();
	each3.CreateConstBuff0();
	each3.CreateConstBuff1();
}

void LightObjectModels::Update()
{
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
	light1.CreateModel("LightBox1", ShaderManager::thunderShader);
	light2.CreateModel("LightBox1", ShaderManager::thunderShader);
	light3.CreateModel("LightBox1", ShaderManager::thunderShader);
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
	}
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
