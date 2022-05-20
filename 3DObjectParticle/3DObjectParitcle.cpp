#include "3DObjectParticle.h"
#include "../DX12operator.h"
#include "../Shader/ShaderManager.h"
#include "../WindowsAPI/WinAPI.h"
#include "../Camera/Camera.h"
#include <vector>

ObjectParticleInfo ObjectParticles::triangle;
ObjectParticleInfo ObjectParticles::othello;
ObjectParticleInfo ObjectParticles::frame;
ObjectParticleInfo ObjectParticles::othelloFrame;
ObjectParticleInfo ObjectParticles::six;
ObjectParticleInfo ObjectParticles::othello2;
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
		InitTitle(emitter);
		this->type = type;
	}
	else if (type == ParticleType::Swell)
	{
		InitSwell(emitter);
		this->type = type;
	}
	else if (type == ParticleType::Target)
	{
		InitTarget(emitter);
		this->type = type;
	}
	else if (type == ParticleType::Tornado)
	{
		InitTornado(emitter);
		this->type = type;
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

void ObjectParticle3D::Add(XMFLOAT3& emitter, ParticleType type, int name, int combo)
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
		InitTitle(emitter);
		this->type = type;
	}
	else if (type == ParticleType::Swell)
	{
		InitSwell(emitter);
		this->type = type;
	}
	else if (type == ParticleType::Target)
	{
		InitTarget(emitter);
		this->type = type;
	}
	else if (type == ParticleType::Tornado)
	{
		InitTornado(emitter);
		this->type = type;
	}
	else if (type == ParticleType::Born)
	{
		InitBorn(emitter);
	}
	else if (type == ParticleType::BornAndShake)
	{
		InitBornAndShake(emitter, name, combo);
	}
	else if (type == ParticleType::Combo)
	{
		InitConbo(emitter);
	}
	this->type = type;
}

void ObjectParticle3D::Add(XMFLOAT3& emitter, ParticleType type, XMFLOAT3& size)
{
	if (type == ParticleType::ComboNum)
	{
		InitConboNum(emitter, size);
	}
	this->type = type;
}

void ObjectParticle3D::Update(list<XMFLOAT3> list, int combo)
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
		UpdateBorn(list);
	}
	else if (type == ParticleType::BornAndShake)
	{
		UpdateBornAndShake(combo);
	}
	else if (type == ParticleType::Combo)
	{
		UpdateCombo(list);
	}
	else if (type == ParticleType::ComboNum)
	{
		UpdateConboNum();
	}
}

void ObjectParticle3D::Update(list<XMFLOAT3> list, vector<pair<int, int>> ComboAndname)
{
	if (type == ParticleType::BornAndShake)
	{
		UpdateBornAndShake(ComboAndname);
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
	addRotation = GetRandom(5.0f);
}

void ObjectParticle::CreateModel(const char* name, HLSLShader& shader, bool smoothing, bool addAlpha)
{
	InitializeDescriptorHeap();
	if (addAlpha)
	{
		Model::InitializeGraphicsPipeline(shader);
	}
	else 
	{
		InitializeGraphicsPipeline(shader);
	}
	ifstream file;
	const string modelname = name;
	const string filename = modelname + ".obj";
	const string directoryPath = "Resource/Model/" + modelname + "/";
	file.open(directoryPath + filename);

	if (file.fail())
	{
		assert(0);
	}

	vector<XMFLOAT3> pos;
	vector<XMFLOAT3> normal;
	vector<XMFLOAT2> uv;

	string line;
	while (getline(file, line))
	{
		istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key == "mtllib")
		{   //マテリアル
			string filename;
			line_stream >> filename;
			LoadMaterial(directoryPath, filename);
		}
		if (key == "v")
		{   //座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			pos.emplace_back(position);//追加
		}
		if (key == "vt")
		{
			XMFLOAT2 texcood{};//UV受け取る
			line_stream >> texcood.x;
			line_stream >> texcood.y;

			texcood.y = 1.0f - texcood.y;//ｖを反転
			uv.emplace_back(texcood);

		}
		if (key == "vn")
		{   //法線
			XMFLOAT3 normals{};
			line_stream >> normals.x;
			line_stream >> normals.y;
			line_stream >> normals.z;
			normal.emplace_back(normals);
		}
		if (key == "f")
		{   //インデックス
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//count += 1;
				istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcood;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexTexcood;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				Vertex verte{};
				verte.pos = pos[indexPosition - 1];
				verte.normal = normal[indexNormal - 1];
				verte.uv = uv[indexTexcood - 1];

				mesh.vertices.emplace_back(verte);
				if (smoothing)
				{
					AddAmoothData(indexPosition, (unsigned short)GetVertexCount() - 1);
				}
				mesh.indices.emplace_back((unsigned short)mesh.indices.size());
				/*if (count > 3) {
					const uint16_t index1 = mesh.vertices.size() - 4;
					const uint16_t index2 = mesh.vertices.size() - 2;

					mesh.indices.emplace_back(index1);
					mesh.indices.emplace_back(index2);
				}*/
			}
			//count = 0;
		}
	}

	if (smoothing)
	{
		CalculateSmoothedVertexNormals();
	};
	Init(1);
	file.close();
}

void FrameEach::CreateBuff()
{
	CreateConstBuff0();
	CreateConstBuff1();
}

void FrameEach::CreateConstBuff0()
{
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(FrameConstBuff) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff0));
}

bool ObjectParticle::InitializeGraphicsPipeline(HLSLShader& shader)
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	if (shader.vsBlob == nullptr)
	{
		assert(0);
	}
	if (shader.psBlob == nullptr)
	{
		assert(0);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(shader.vsBlob.Get());
	if (shader.gsBlob != nullptr)
	{
		gpipeline.GS = CD3DX12_SHADER_BYTECODE(shader.gsBlob.Get());
	}
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(shader.psBlob.Get());
	if (shader.hsBlob != nullptr)
	{
		gpipeline.HS = CD3DX12_SHADER_BYTECODE(shader.hsBlob.Get());
	}
	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;

	/*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;*/

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	//CD3DX12_ROOT_PARAMETER rootparams[3];
	CD3DX12_ROOT_PARAMETER rootparams[4];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = BaseDirectX::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	if (FAILED(result)) {
		return result;
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成-------------------------
	result = BaseDirectX::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	if (FAILED(result)) {
		return result;
	}

	return true;
}

void ObjectParticle::Update(FrameEach* each)
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

		FrameConstBuff* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{
			//constMap0->mat = matWorld * Camera::matView * BaseDirectX::matProjection;
			constMap0->viewproj = Camera::matView * BaseDirectX::matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = cameraPos;
			//constMap0->flash = 1;
			constMap0->flash = flash;
			//constMap0->colorType = this->each.colorType;
			constMap0->colorType = each->colorType;
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

		FrameConstBuff* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{
			//constMap0->mat = matWorld * Camera::matView * BaseDirectX::matProjection;
			constMap0->viewproj = Camera::matView * BaseDirectX::matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = cameraPos;
			//constMap0->flash = 1;
			constMap0->flash = flash;
			//constMap0->colorType = this->each.colorType;
			constMap0->colorType = colorType;
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
	each.CreateBuff();
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

void ObjectParticle3D::InitBornAndShake(XMFLOAT3& emitter, int name, int combo)
{
	time = 1;
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	each.position = ConvertXMFLOAT3toXMVECTOR(emitter);

	each.scale = { 1.0f, 1.0f, 6.0f };
	each.colorType = combo;
	othellosName = name;
	easeTime = 1.0f;
	startPosition = emitter;
}

void ObjectParticle3D::InitConbo(XMFLOAT3& emitter)
{
	time = 1;
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	each.position = ConvertXMFLOAT3toXMVECTOR(emitter);
	//each.position.m128_f32[3] = 1;
	each.scale = { 1.0f, 0.3f, 0.5f };
	each.rotation = { 0, 0, 180 };
	easeTime = 1.0f;
	startPosition = emitter;
}

void ObjectParticle3D::InitConboNum(XMFLOAT3& emitter, XMFLOAT3& size)
{
	time = 1;
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	each.position = ConvertXMFLOAT3toXMVECTOR(emitter);
	//each.position.m128_f32[3] = 1;
	each.scale = size;
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

void ObjectParticle3D::UpdateBorn(list<XMFLOAT3> list)
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
	if (list.size() <= 0) return;
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		if (Lenght(each.position, XMFLOAT3(itr->x, itr->y, itr->z)) < 1.0f)
		{
			time = 0;
		}
	}
}

void ObjectParticle3D::UpdateBornAndShake(int combo)
{
	each.position = ConvertXMFLOAT3toXMVECTOR(startPosition);
	each.position.m128_f32[0] += (rand() % (int)(20.0f * easeTime + 1) - (9 * easeTime)) / 400.0f;
	each.position.m128_f32[1] += (rand() % (int)(20.0f * easeTime + 1) - (9 * easeTime)) / 400.0f;
	easeTime -= 1.0f / 600.0f;//vanishTimerMax
	each.colorType = combo;
	if (easeTime <= 0.0f)
	{
		time = 0;
	}
}

void ObjectParticle3D::UpdateBornAndShake(vector<pair<int, int>> ComboAndname)
{
	each.position = ConvertXMFLOAT3toXMVECTOR(startPosition);
	each.position.m128_f32[0] += (rand() % (int)(20.0f * easeTime + 1) - (9 * easeTime)) / 400.0f;
	each.position.m128_f32[1] += (rand() % (int)(20.0f * easeTime + 1) - (9 * easeTime)) / 400.0f;
	easeTime -= 1.0f / 600.0f;//vanishTimerMax
	int othellotypeint = othellosName;
	for (auto itr = ComboAndname.begin(); itr != ComboAndname.end(); ++itr)
	{
		if (itr->first == othellosName)
		{
			each.colorType = itr->second;
		}
	}
	if (easeTime <= 0.0f)
	{
		time = 0;
	}
}

void ObjectParticle3D::UpdateCombo(list<XMFLOAT3> list)
{
	easeTime -= 0.01f;
	each.scale = {1.0f,	0.5f, 0.5f};
	if (easeTime <= 0.0f)
	{
		time = 0;
	}
}

void ObjectParticle3D::UpdateConboNum()
{
	easeTime -= 0.01f;
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

void ObjectParticleInfo::Init(XMFLOAT3& emitter, int count, ParticleType type, int name, int combo)
{
	for (int i = 0; i < count; i++)
	{
		ObjectParticle3D element;
		element.Add(emitter, type, name, combo);
		particles.push_back(element);
	}
}

void ObjectParticleInfo::Init(XMFLOAT3& emitter, int count, ParticleType type, XMFLOAT3 size)
{
	for (int i = 0; i < count; i++)
	{
		ObjectParticle3D element;
		element.Add(emitter, type, size);
		particles.push_back(element);
	}
}

void ObjectParticleInfo::Update(list<XMFLOAT3> list, int combo)
{
	for (auto itr = particles.begin(); itr != particles.end(); ++itr)
	{
		itr->Update(list, combo);
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

void ObjectParticleInfo::Update(list<XMFLOAT3> list, vector<pair<int, int>> ComboAndname)
{
	for (auto itr = particles.begin(); itr != particles.end(); ++itr)
	{
		itr->Update(list, ComboAndname);
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
	six.object.CreateModel("six", ShaderManager::playerShader);
	othello2.object.CreateModel("newOserro3", ShaderManager::playerShader);
}

void ObjectParticles::Update(list<XMFLOAT3> list, int combo)
{
	triangle.Update(list, combo);
	othello.Update(list, combo);
	frame.Update(list, combo);
	six.Update(list, combo);
	othello2.Update(list, combo);
}

void ObjectParticles::Update(list<XMFLOAT3> list, vector<pair<int, int>> ComboAndname)
{
	othelloFrame.Update(list, ComboAndname);
}

void ObjectParticles::Draw()
{
	triangle.Draw(triangle.object);
	othello.Draw(othello.object);
	frame.Draw(frame.object);
	othelloFrame.Draw(othelloFrame.object);
	six.Draw(six.object);
	othello2.Draw(othello2.object);
}

void ObjectParticles::DeleteAllParticles()
{
	triangle.DeleteAllParticle();
	othello.DeleteAllParticle();
	frame.DeleteAllParticle();
	othelloFrame.DeleteAllParticle();
	six.DeleteAllParticle();
	othello2.DeleteAllParticle();
}
