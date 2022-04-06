#pragma once
#include "../3DModel/Model.h"

class Ground
{
	static Model BlackGround;
	static Model WhiteGround;
	static EachInfo each[8][8];
public:
	static void LoadModel();
	static void Init();
	static void Update();
	static void Draw();
};