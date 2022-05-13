#include "../Thunder/Thunder.h"
#include "../Camera/Camera.h"
#include "../DX12operator.h"
ThunderModel ThunderModels::thunder1;
ThunderModel ThunderModels::thunder2;
ThunderModel ThunderModels::thunder3;
list<Thunder> ThunderModels::thunders;
list<list<Thunder>::iterator> ThunderModels::deleteThunders;
void ThunderEachInfo::CreateConstBuff0()
{
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ThunderConstBuffer) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff0));
}

void ThunderModel::Update(ThunderEachInfo* each)
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

		ThunderConstBuffer* constMap0 = nullptr;
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

		ThunderConstBuffer* constMap0 = nullptr;
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

void Thunder::Init(XMFLOAT3& position)
{
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	each.position = ConvertXMFLOAT3toXMVECTOR(position);
	each.time = ThunderMaxTime;
	each.scale = { 0.9f, 0.9f, 0.9f };
	isActive = true;
	each.rotation.x = 90;
	number = rand() % 3;
}

void Thunder::Update()
{
	each.time -= 0.0017f;
	if (each.time <= 0.0f)
	{
		isActive = false;
	}
	Shake();
}

void Thunder::Shake()
{
	each.position = ConvertXMFLOAT3toXMVECTOR(startPos);
	float subX, subY;
	if (rand() % 2)
	{
		subX = each.time;
	}
	else
	{
		subX = -each.time;
	}
	if (rand() % 2)
	{
		subY = each.time;
	}
	else
	{
		subY = -each.time;
	}
	each.position.m128_f32[0] += subX / 50.0f;
	each.position.m128_f32[1] += subY / 50.0f;
}

void ThunderModels::LoadModels()
{
	thunder1.CreateModel("Thunder1", ShaderManager::thunderShader);
	thunder2.CreateModel("Thunder2", ShaderManager::thunderShader);
	thunder3.CreateModel("Thunder3", ShaderManager::thunderShader);
}

void ThunderModels::Init(XMFLOAT3& position, XMFLOAT3& rotation)
{
	int result = rand() % 3;
	Thunder thunder;
	thunder.Init(position);
	thunder.startPos = position;
	thunders.push_back(thunder);
}

void ThunderModels::Update()
{
	for (auto itr = thunders.begin(); itr != thunders.end(); ++itr)
	{
		if (itr->isActive)
		{
			itr->Update();
		}
		else
		{
			deleteThunders.push_back(itr);
		}
	}
	for (auto deleteItr = deleteThunders.begin(); deleteItr != deleteThunders.end(); ++deleteItr)
	{
		thunders.erase(*deleteItr);
	}
	deleteThunders.clear();
}

void ThunderModels::Draw()
{
	for (auto itr = thunders.begin(); itr != thunders.end(); ++itr)
	{
		if (itr->number == 0)
		{
			thunder1.Update(&itr->each);
			Draw3DObject(thunder1);
		}
		else if (itr->number == 1)
		{
			thunder2.Update(&itr->each);
			Draw3DObject(thunder2);
		}
		else
		{
			thunder3.Update(&itr->each);
			Draw3DObject(thunder3);
		}
	}
}

void ThunderModels::DeleteList()
{
	thunders.clear();
}
