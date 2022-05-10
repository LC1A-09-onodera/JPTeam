#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>
using namespace Microsoft::WRL;
using namespace DirectX;

class Imgui
{
private:
    static ComPtr<ID3D12DescriptorHeap> imguiDescHeap;//imguiï€éùópÉÅÉìÉo
    static ComPtr<ID3D12DescriptorHeap> heapForImgui;
public:
    
    static ComPtr<ID3D12DescriptorHeap> CreateDescrriptorHeapForImgui();
    static ComPtr<ID3D12DescriptorHeap> GetHeapForImgui();
    static void DrawImGui();
    static void Init();
    static void GetAndDrawInfo(const int &intOriginal, int& imguiInfo);
    static void GetAndDrawInfo(const float &floatOriginal, float& imguiInfo);
    static void GetAndDrawInfo(const XMFLOAT3 &xmfloat3Original, XMFLOAT3& imguiInfo);
    static void ChangeInfo(int &intOriginal, int& imguiInfo);
    static void ChangeInfo(float &floatOriginal, float& imguiInfo);
    static void ChangeInfo(XMFLOAT3 &xmfloat3Original, XMFLOAT3& imguiInfo);

    enum ImguiTab
    {
        Status,
        PostEffect,
        Camera,
        Debug,
    };

    static bool spotLight1;
    static float lightColor[3];

    static int spritePos1[2];
    static int spritePos2[2];
    static int spritePos3[2];
    static int spritePos4[2];
    static int spritePos5[2];
    static int spritePos6[2];
    static int spritePos7[2];

    //System
    static float fps;
    static float eyePos[3];
    static float target[3];

    //imguiÇÃëÆê´
    //static ImguiTab tab;

    static int combo;

    static int score;

    static int sample;
    static float dethLine;
};
