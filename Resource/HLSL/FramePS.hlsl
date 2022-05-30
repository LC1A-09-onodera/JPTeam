#include "OBJHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv);
	//シェーディングによる色
	float4 shadecolor;
	//光沢度
	const float shininess = 4.0f;
	//頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//ライトに向かうベクトルと法線のベクトル
	float3 dotlightnormal = dot(lightv, input.normal);
	//反射光ベクトル
	float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//環境光
	float3 ambient = m_ambient;
	float3 diffuse = dotlightnormal * m_diffuse;
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	shadecolor.rgb = (ambient + diffuse + specular) * lightcolor;
	shadecolor.a = m_alpha;
	for (int i = 0; i < POINTLIGHT_NUM; i++)
	{
		if (pointLights[i].active)
		{
			//ライトベクトル
			float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			//ベクトルの長さ
			float d = length(lightv);
			//正規化
			lightv = normalize(lightv);
			//距離減衰係数
			float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
			//内積値
			float3 dotlightnormal = dot(lightv, input.normal);
			//反射光
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			//鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//すべてを加算
			shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
		}
	}
	for (int i = 0; i < SPOTLIGHT_NUM; i++)
	{
		if (spotLights[i].active)
		{
			//ライトの方向ベクトル
			float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);
			//距離減衰係数
			float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));
			//角度係数
			float cos = dot(lightv, spotLights[i].lightv);
			//減衰開始の内側は1倍、それより外は0倍
			float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
			//減衰を乗算
			atten *= angleatten;
			//ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(lightv, input.normal);
			//反射光ベクトル
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			//鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//全ての加算
			shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
		}
	}
	for (int i = 0; i < CIRCLESHADOW_NUM; i++)
	{
		if (circleShadows[i].active)
		{
			float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			float d = dot(casterv, circleShadows[i].dir);
			float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
			atten *= step(0, d);
			float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			float3 lightv = normalize(lightpos - input.worldpos.xyz);
			float cos = dot(lightv, circleShadows[i].dir);
			float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			atten *= angleatten;
			shadecolor.rgb -= atten;
		}
	}
	float4 result;
	//texcolor.a = 0.5f;
	result = shadecolor * texcolor;
	//result.a = 1.0f;
	//result.r += 0.3f;
	//result.g += 0.3f;
	//result.b += 0.3f;
    result.x = 1.0f;
    result.y = 0.99f;
    result.z = 0.6f;
 //   if (colorType == 0)
 //   {
 //       result.x = 0.0f;
 //       result.y = 0.0f;
 //       result.z = 0.0f;
 //   }
	//if (colorType == 1)
	//{
 //       result.x = 0.78f;
 //       result.y = 0.0f;
 //       result.z = 0.04f;
 //       return result;
 //   }
	//if (colorType == 2)
	//{
 //       result.x = 0.82f;
 //       result.y = 0.51f;
 //       result.z = 0.0f;
 //       return result;
 //   }
	//if (colorType == 3)
	//{
 //       result.x = 0.87f;
 //       result.y = 0.82f;
 //       result.z = 0.0f;
 //       return result;
 //   }
 //   if (colorType == 4)
 //   {
 //       result.x = 0.48f;
 //       result.y = 0.67f;
 //       result.z = 0.09f;
 //       return result;
 //   }
	//if (colorType == 5)
 //   {
 //       result.x = 0.0f;
 //       result.y = 0.53f;
 //       result.z = 0.24f;
 //       return result;
 //   }
	//if (colorType == 6)
 //   {
 //       result.x = 0.0f;
 //       result.y = 0.54f;
 //       result.z = 0.51f;
 //       return result;
 //   }
	//if (colorType == 7)
 //   {
 //       result.x = 0.0f;
 //       result.y = 0.55f;
 //       result.z = 0.80f;
 //       return result;
 //   }
 //   if (colorType == 8)
 //   {
 //       result.x = 0.42f;
 //       result.y = 0.61f;
 //       result.z = 0.682f;
 //       return result;
 //   }
	//if (colorType == 9)
 //   {
 //       result.x = 0.47f;
 //       result.y = 0.42f;
 //       result.z = 0.69f;
 //       return result;
 //   }
	//if (colorType == 10)
 //   {
 //       result.x = 0.73f;
 //       result.y = 0.47f;
 //       result.z = 0.69f;
 //       return result;
 //   }
 //   if (colorType == 11)
 //   {
 //       result.x = 0.73f;
 //       result.y = 0.47f;
 //       result.z = 0.69f;
 //       return result;
 //   }
 //   if (colorType == 12)
 //   {
 //       result.x = 0.73f;
 //       result.y = 0.47f;
 //       result.z = 0.69f;
 //       return result;
 //   }
 //   if (colorType == 13)
 //   {
 //       result.x = 0.73f;
 //       result.y = 0.47f;
 //       result.z = 0.69f;
 //       return result;
 //   }
 //   if (colorType == 14)
 //   {
 //       result.x = 0.73f;
 //       result.y = 0.47f;
 //       result.z = 0.69f;
 //       return result;
 //   }
 //   if (colorType == 15)
 //   {
 //       result.x = 0.73f;
 //       result.y = 0.47f;
 //       result.z = 0.69f;
 //       return result;
 //   }
 //   if (colorType == 16)
 //   {
 //       result.x = 0.73f;
 //       result.y = 0.47f;
 //       result.z = 0.69f;
 //       return result;
 //   }
 //   if (colorType == 17)
 //   {
 //       result.x = 0.73f;
 //       result.y = 0.47f;
 //       result.z = 0.69f;
 //       return result;
 //   }
 //   if (colorType == 18)
 //   {
 //       result.x = 0.73f;
 //       result.y = 0.47f;
 //       result.z = 0.69f;
 //       return result;
 //   }
 //   if (colorType == 19)
 //   {
 //       result.x = 0.73f;
 //       result.y = 0.47f;
 //       result.z = 0.69f;
 //       return result;
 //   }
 //   if (colorType == 20)
 //   {
 //       result.x = 0.73f;
 //       result.y = 0.47f;
 //       result.z = 0.69f;
 //       return result;
 //   }
    return result;
}