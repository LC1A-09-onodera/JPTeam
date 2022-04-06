#include "Ground.h"
#include "../Shader/ShaderManager.h"
Model Ground::BlackGround;
Model Ground::WhiteGround;
EachInfo Ground::each[8][8];
void Ground::LoadModel()
{
	BlackGround.CreateModel("cubeB", ShaderManager::playerShader);
	WhiteGround.CreateModel("cubeW", ShaderManager::playerShader);
}

void Ground::Init()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			each[i][j].CreateConstBuff0();
			each[i][j].CreateConstBuff1();
			each[i][j].position.m128_f32[0] = -8.0f + (2.0f * i);
			each[i][j].position.m128_f32[1] = -6.0f + (2.0f * j);
			each[i][j].position.m128_f32[2] = 1.0f;
		}
	}
}

void Ground::Update()
{
	
}

void Ground::Draw()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if ((i + j) % 2 == 1)
			{
				BlackGround.Update(&each[i][j]);
				Draw3DObject(BlackGround);
			}
			else
			{
				WhiteGround.Update(&each[i][j]);
				Draw3DObject(WhiteGround);
			}
		}
	}
}
