#include "BaseDirectX/GameScene.h"
#include "../Sound/Sound.h"
#include <dxgidebug.h>
using namespace Microsoft::WRL;
using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "xaudio2.lib")

// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameScene scene;
	scene.Init();

	while (true)  // �Q�[�����[�v
	{
		scene.SceneManageUpdateAndDraw();

		// DirectX���t���[�������@�����܂�
		if (Input::Key(DIK_ESCAPE))
		{
			break;
		}
		if (WindowsAPI::Qite)
		{
			break;
		}
	}
	Sound::xAudio2.Reset();
	// �E�B���h�E�N���X��o�^����
	WindowsAPI::Clear();

	IDXGIDebug* pDxgiDebug = nullptr;
	if (pDxgiDebug == nullptr)
	{
		// �쐬
		typedef HRESULT(__stdcall* fPtr)(const IID&, void**);
		HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
		if (hDll != 0)
		{
			fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

			DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&pDxgiDebug);

			// �o��
			pDxgiDebug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_DETAIL);
		}
	}

	return 0;
}
