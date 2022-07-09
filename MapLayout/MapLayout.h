#pragma once
#include <list>
#include <vector>
#include <string>
#include <array>
#include <DirectXMath.h>
#include "../3DModel/Model.h"

using namespace std;
using namespace DirectX;

enum class MapObjects
{
	NONE,
	WALL,
	GOAL,
	ENEMY,
};

//�I�u�W�F�N�g�̖��O����
class MapObjectNames
{
	static string wall;
	static string goal;
	static string enemy;
};

//�}�b�v�G�f�B�^���ŉ��ςȂǂ��s���N���X
class MapEditorObject
{
public:

	EachInfo piece;
	bool isActive = false;
	bool OnCollisionMouse(float posX, float posY);
	void Init(BaseDirectX &baseDirectX, XMFLOAT3& position);
	string PositionToString();
};

//�O���ۑ��p�N���X
class MapPreservation
{
	XMFLOAT3 position;

};

class MapEditorObjects
{
	static list<MapEditorObject> wall;
	static list<MapEditorObject> goal;
	static list<MapEditorObject> enemy;

	static Model wallModel;
	static Model goalModel;
	static Model enemyModel;

	static XMFLOAT3 lineMousePos;
	static bool isLinePut;

	static list<pair<list<MapEditorObject>::iterator, MapObjects>> andoList;
public:
	static const int MapW;
	static const int MapH;
	static MapObjects activeType;
	static void LoadModels(BaseDirectX &baseDirectX);
	static void LoadFile(string path);
	static void Update(BaseDirectX& baseDirectX, XMFLOAT3& mousePos);
	static void Draw(BaseDirectX& baseDirectX);
	static void SetObject(BaseDirectX& baseDirectX, XMFLOAT3& position);
	static void SetObjectLine(BaseDirectX& baseDirectX, XMFLOAT3& position);
	static bool ObjectCollision(XMFLOAT3& mousePos);
	static void OutputFile(const char *path);
	static void DeleteObjects();
	static void EraseObject();
};