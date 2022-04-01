#include"ShaderManager.h"

HLSLShader ShaderManager::playerShader;
//HLSLShader ShaderManager::translucentShader;
void ShaderManager::LoadShaders()
{
	playerShader.vsBlob = LoadShader(L"Resource/HLSL/OBJVertexShader.hlsl", "vs_5_0");
	playerShader.psBlob = LoadShader(L"Resource/HLSL/OBJPixelShader.hlsl", "ps_5_0");
	/*translucentShader.vsBlob = LoadShader(L"Resource/HLSL/OBJVertexShader.hlsl", "vs_5_0");
	translucentShader.psBlob = LoadShader(L"Resource/HLSL/transluentPixelShader.hlsl", "ps_5_0");*/
}
