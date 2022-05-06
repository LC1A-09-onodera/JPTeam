#include "OBJHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//�e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv);
	//�V�F�[�f�B���O�ɂ��F
	float4 shadecolor;
	//����x
	const float shininess = 4.0f;
	//���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//���C�g�Ɍ������x�N�g���Ɩ@���̃x�N�g��
	float3 dotlightnormal = dot(lightv, input.normal);
	//���ˌ��x�N�g��
	float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//����
	float3 ambient = m_ambient;
	float3 diffuse = dotlightnormal * m_diffuse;
	//�g�D�[��
    //diffuse.x = step(0.5, diffuse.x);
    //diffuse.y = step(0.5, diffuse.y);
    //diffuse.z = step(0.5, diffuse.z);
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	shadecolor.rgb = (ambient + diffuse + specular) * lightcolor;
	shadecolor.a = m_alpha;
	for (int i = 0; i < POINTLIGHT_NUM; i++)
	{
		if (pointLights[i].active)
		{
			//���C�g�x�N�g��
			float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			//�x�N�g���̒���
			float d = length(lightv);
			//���K��
			lightv = normalize(lightv);
			//���������W��
			float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
			//���ϒl
			float3 dotlightnormal = dot(lightv, input.normal);
			//���ˌ�
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
			float3 diffuse = dotlightnormal * m_diffuse;
			//���ʔ��ˌ�
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//���ׂĂ����Z
			shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
		}
	}
	for (int i = 0; i < SPOTLIGHT_NUM; i++)
	{
		if (spotLights[i].active)
		{
			//���C�g�̕����x�N�g��
			float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);
			//���������W��
			float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));
			//�p�x�W��
			float cos = dot(lightv, spotLights[i].lightv);
			//�����J�n�̓�����1�{�A������O��0�{
			float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
			//��������Z
			atten *= angleatten;
			//���C�g�Ɍ������x�N�g���Ɩ@���̓���
			float3 dotlightnormal = dot(lightv, input.normal);
			//���ˌ��x�N�g��
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
			float3 diffuse = dotlightnormal * m_diffuse;
			//���ʔ��ˌ�
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//�S�Ẳ��Z
			shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
		}
	}
	float4 result;
	//texcolor.a = 0.5f;
	result = shadecolor * texcolor;
	result.a = 1.0f;
	return result;
}