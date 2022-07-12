#pragma once
#include "../BaseDirectX/Input.h"
#include "../Sprite/Sprite.h"
#include <tuple>

namespace Rewired
{
	class KeyCodeString
	{
	public:
		static void KeyCodeStringInit(BaseDirectX &baseDirectX);
		//Keyborad
		static list<tuple<string, KeyCode, Sprite>> keyboardKeys;
		//XBoxPad
		static list<tuple<string, PadKeyCode, Sprite>> padKeys;
	};

	class RewiredKeys
	{
		string fileName;
	public:
		string newKeyName;
		bool GetKey();
		bool GetKeyDown();
		bool GetKeyUp();
		void AddKey(KeyCode key);
		void AddKey(PadKeyCode key);
		void Subkey(KeyCode key);
		void SubKey(PadKeyCode key);
		void LoadKey(const char* name, bool isAdd = true);
		void SaveKey();
		string GetFileName(){ return fileName; }
		list<KeyCode> keys;
		list<PadKeyCode> padKeys;
	};

	class RewiredContainer
	{
	public:
		static vector<std::string> files;
		static list<RewiredKeys *> rewireds;
		static list<RewiredKeys> rewiredsC;
		static void AddRewired(RewiredKeys &rewired);
		//ImGui�Ńt�@�C���쐬����
		static void CreateRewired(string rewiredName);
		//�t�@�C�����͊����̂��g���̂ŏ��������ɂ͎g���Ȃ�
		static void ReloadRewired();
		static void LoadAllRewired();
		static void GetFilesName();
	};
}