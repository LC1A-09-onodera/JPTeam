#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <wrl.h>
#include <d3dx12.h>
#include <vector>
#include <xaudio2.h>
#include <fstream>
#include "../WindowsAPI/WinAPI.h"

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "xaudio2.lib")

class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequires) {};
	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
	STDMETHOD_(void, OnStreamEnd) (THIS) {};
	STDMETHOD_(void, OnBufferStart) (THIS_ void *pBufferContext) {};
	STDMETHOD_(void, OnBufferEnd) (THIS_ void *pBufferContext) { delete[] pBufferContext; };
	STDMETHOD_(void, OnLoopEnd) (THIS_ void *pBufferContext) {};
	STDMETHOD_(void, OnVoiceError) (THIS_ void *pBufferCountext, HRESULT Error) {};
};

struct PeraVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class BaseDirectX
{
public:
	BaseDirectX();
	~BaseDirectX();
	XMFLOAT4 backColor = {0.0f, 0.0f, 0.0f , 0.0f};
	HRESULT result;
	ComPtr<ID3D12Device> dev = nullptr;
	ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
	ComPtr<IDXGISwapChain4> swapchain = nullptr;
	ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;
	ComPtr<ID3D12CommandQueue> cmdQueue = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;
	vector<IDXGIAdapter1 *> adapters;
	D3D_FEATURE_LEVEL levels[4] = {} ;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL();
	IDXGIAdapter1 *tmpAdapter = nullptr;
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = D3D12_COMMAND_QUEUE_DESC();
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = DXGI_SWAP_CHAIN_DESC1();
	ComPtr<IDXGISwapChain1> swapchain1 = nullptr;
	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;
	vector<ComPtr<ID3D12Resource>> backBuffers;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = D3D12_DESCRIPTOR_HEAP_DESC();
	ComPtr<ID3D12Resource> depthBuffer = nullptr;
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC();
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = D3D12_DESCRIPTOR_HEAP_DESC();
	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = D3D12_DEPTH_STENCIL_VIEW_DESC();
	//static const int ObjectNum = 200;
	static const int TexBufferNum = 100;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV[BaseDirectX::TexBufferNum] = {};
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV[BaseDirectX::TexBufferNum] = {};
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = D3D12_CPU_DESCRIPTOR_HANDLE();
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = D3D12_GRAPHICS_PIPELINE_STATE_DESC();
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineMeth = D3D12_GRAPHICS_PIPELINE_STATE_DESC();
	D3D12_RENDER_TARGET_BLEND_DESC &blenddesc = D3D12_RENDER_TARGET_BLEND_DESC();
	ComPtr<ID3DBlob> vsBlob = nullptr;
	ComPtr<ID3DBlob> psBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[3];
	CD3DX12_DESCRIPTOR_RANGE descRangeCRV = CD3DX12_DESCRIPTOR_RANGE();
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV = CD3DX12_DESCRIPTOR_RANGE();
	CD3DX12_ROOT_PARAMETER rootparam[2];
	ComPtr<ID3D12PipelineState> pipelinestate = nullptr;
	ComPtr<ID3D12PipelineState> pipelinestateMeth = nullptr;
	ComPtr<ID3D12RootSignature> rootsignature = nullptr;
	//テクスチャサンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc = D3D12_STATIC_SAMPLER_DESC();
	//定数バッファデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> basicDescHeap = nullptr;
	XMMATRIX matProjection = XMMatrixIdentity();
	//ビュー行列の作成
	//static XMMATRIX matView;
	float angle = 0;
	float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	void DebugLayerOn();
	void Set();
	void UpdateFront();
	void UpdateBack();
	void BackGroundFront();
	void GetAdress();
};