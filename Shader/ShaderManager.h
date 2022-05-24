#pragma once
#include "Shader.h"

class ShaderManager
{
public:
	static HLSLShader playerShader;
	static HLSLShader thunderShader;
	static HLSLShader othelloShader;
	static HLSLShader othelloFrame;
	static HLSLShader half;
	//static HLSLShader translucentShader;

	static HLSLShader postEffect;
	static void LoadShaders();
};