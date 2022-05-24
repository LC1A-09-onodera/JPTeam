#include "SpriteShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 result = tex.Sample(smp, input.uv);
	//result.w = 0.5f;
	return result;
}