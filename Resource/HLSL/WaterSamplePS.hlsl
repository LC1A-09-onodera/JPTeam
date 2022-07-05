#include "OBJHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

#define  Iterations  8

float4 main(VSOutput input) : SV_TARGET
{
    //���ʂۂ���
    float2 windowSize;
    windowSize.x = 1280;
    windowSize.y = 720;
    float speed = 0.8f;
    //�g�ׂ̍���
    float ripple = 0.6f;
    float2 uv = input.uv;
    //���݂̃t���[�����擾
    uint frameCount = flash;
    float time = frameCount * 0.01;
    float2 pos = input.svpos.xy / windowSize * 12.0 - 20.0;
    float2 tmp = pos;
    float speed2 = speed * 1.0;
    //�g�̃f�J��
    float inten = 0.020f;
    float col = 0;
    //�c�݂̃f�J��
    float distortion = 0.1f;
    for (int i = 0; i < Iterations; ++i)
    {
        float t = time * (1.0 - (3.2 / (float(i) + speed)));
        tmp = pos + float2(
		cos(t - tmp.x * ripple) + sin(t + tmp.y * ripple),
		sin(t - tmp.y * ripple) + cos(t + tmp.x * ripple));
        tmp += time;
        col += 1.0 / length(float2(pos.x / (sin(tmp.x + t * speed2) / inten), pos.y / (cos(tmp.y + t * speed2) / inten)));
    }
    col /= float(Iterations);
    col = saturate(1.5 - sqrt(col));
    uv += col * distortion;
    return tex.Sample(smp, uv) + (col * 0.6f);
}