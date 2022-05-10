#include "ImguiControl.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx12.h"
#include "../imgui/imgui_impl_win32.h"
#include "../BaseDirectX/BaseDirectX.h"
#include "../Camera/Camera.h"
//#include "../BaseDirectX/GameScene.h"

ComPtr<ID3D12DescriptorHeap> Imgui::imguiDescHeap;
ComPtr<ID3D12DescriptorHeap> Imgui::heapForImgui;
bool Imgui::spotLight1;
float Imgui::lightColor[3];
float Imgui::dethLine = 25.0f;
int Imgui::sample = 0;
int Imgui::spritePos1[2];
int Imgui::spritePos2[2];
int Imgui::spritePos3[2];
int Imgui::spritePos4[2];
int Imgui::spritePos5[2];
int Imgui::spritePos6[2];
int Imgui::spritePos7[2];
int Imgui::score;
float Imgui::fps;
//ImguiTab Imgui::tab;
float Imgui::eyePos[3];
float Imgui::target[3];

ComPtr<ID3D12DescriptorHeap> Imgui::CreateDescrriptorHeapForImgui()
{
    ComPtr<ID3D12DescriptorHeap> ret;
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NodeMask = 0;
    desc.NumDescriptors = 1;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    BaseDirectX::dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(ret.ReleaseAndGetAddressOf()));
    return ret;
}

ComPtr<ID3D12DescriptorHeap> Imgui::GetHeapForImgui()
{
    return imguiDescHeap;
}

void Imgui::DrawImGui()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Test", nullptr, ImGuiWindowFlags_MenuBar);//ウィンドウの名前
    ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
    //imguiここにいろいろ書く
    /*ImGui::Checkbox("spotLight1", &spotLight1);
    ImGui::SliderFloat3("ligthColor", lightColor, 0.0f, 1.0f);*/
    fps = WindowsAPI::rate;

    /*if (ImGui::BeginMenuBar())
    {
        if (ImGui::MenuItem("Status"))
        {
            tab = ImguiTab::Status;
        }
        if (ImGui::MenuItem("PostEffect"))
        {
            tab = ImguiTab::PostEffect;

        }
        if (ImGui::MenuItem("Camera"))
        {
            tab = ImguiTab::Camera;

        }
        if (ImGui::MenuItem("Debug"))
        {
            tab = ImguiTab::Debug;
        }
        ImGui::EndMenuBar();
    }
    if (tab == ImguiTab::Status)
    {
        ImGui::Text("fps:%.2f", fps);
    }
    else if (tab == ImguiTab::PostEffect)
    {

    }
    else if (tab == ImguiTab::Camera)
    {
        eyePos[0] = Camera::eye.v.x;
        eyePos[1] = Camera::eye.v.y;
        eyePos[2] = Camera::eye.v.z;
        target[0] = Camera::target.v.x;
        target[1] != Camera::target.v.y;
        target[2] = Camera::target.v.z;
        ImGui::SliderFloat3("eyePosition", eyePos, -200, 200);
        ImGui::SliderFloat3("Target", target, -200, 200);
    }
    else if (tab == ImguiTab::Debug)
    {

    }*/

    ImGui::End();
    ImGui::Render();
    BaseDirectX::cmdList->SetDescriptorHeaps(1, GetHeapForImgui().GetAddressOf());
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), BaseDirectX::cmdList.Get());
}

void Imgui::Init()
{
    //imgui
    imguiDescHeap = CreateDescrriptorHeapForImgui();
    if (imguiDescHeap == nullptr)
    {
        return;
    }
    if (ImGui::CreateContext() == nullptr)
    {
        assert(0);
        return;
    }
    //[ImGui::]
    bool blnResult = ImGui_ImplWin32_Init(WindowsAPI::hwnd);
    if (!blnResult)
    {
        assert(0);
        return;
    }
    blnResult = ImGui_ImplDX12_Init(BaseDirectX::dev.Get(), 3, DXGI_FORMAT_R8G8B8A8_UNORM, GetHeapForImgui().Get(), GetHeapForImgui()->GetCPUDescriptorHandleForHeapStart(), GetHeapForImgui()->GetGPUDescriptorHandleForHeapStart());
}

void Imgui::ChangeInfo(int& intOriginal, int& imguiInfo)
{
    intOriginal = imguiInfo;
    return;
}

void Imgui::ChangeInfo(float& floatOriginal, float& imguiInfo)
{
    floatOriginal = imguiInfo;
    return;
}

void Imgui::ChangeInfo(XMFLOAT3& xmfloat3Original, XMFLOAT3& imguiInfo)
{
    xmfloat3Original = imguiInfo;
    return;
}

void Imgui::GetAndDrawInfo(const int& intOriginal, int& imguiInfo)
{
    imguiInfo = intOriginal;
    return;
}

void Imgui::GetAndDrawInfo(const float& floatOriginal, float& imguiInfo)
{
    imguiInfo = floatOriginal;
    return;
}

void Imgui::GetAndDrawInfo(const XMFLOAT3& xmfloat3Original, XMFLOAT3& imguiInfo)
{
    imguiInfo = xmfloat3Original;
    return;
}
