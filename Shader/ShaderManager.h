#pragma once
#include "Shader.h"

class ShaderManager
{
public:
	static HLSLShader playerShader;
	static HLSLShader thunderShader;
	static HLSLShader othelloShader;
	//static HLSLShader translucentShader;
	static void LoadShaders();
};