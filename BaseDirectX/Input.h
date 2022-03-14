#pragma once
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#include <windef.h>
#include <Windows.h>
#include "BaseDirectX.h"
#include "DirectInput.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

static IDirectInput8 *dinput = nullptr;
static IDirectInputDevice8 *devkeyboard = nullptr;
static BYTE keys[256] = {};
static BYTE oldkeys[256] = {};
static LPDIRECTINPUT8 g_lpDI;
static LPDIRECTINPUTDEVICE8 g_lpDIDevive;
static DIDEVCAPS g_diDevCaps;
static POINT mousePos{};
static bool mouseInput[2] = {false, false};
static bool oldMouseInput[2] = {false,false};
static DirectInput *directInput = DirectInput::GetInstance();/////////////////////

class Input
{
public:
	static void Update();

	static void KeySet(WNDCLASSEX w,HWND hwnd);

	static bool Key(BYTE key);

	static bool KeyTrigger(BYTE key);

	static bool KeyEnd(BYTE key);

	static bool MouseTrigger(int mouseinput);

	static bool Mouse(int mouseinput);

};