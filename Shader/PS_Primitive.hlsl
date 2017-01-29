#include "PBRCommon.hlsli"

Texture2D texDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

TextureCube texIrradiance : register(t1);
TextureCube texRadiance : register(t2);

cbuffer CBMatrix : register(b0)
{
	float4x4 matWorld;
	float4x4 matView;
	float4x4 matProjection;
};

cbuffer CBEnvironment : register(b1)
{
	float3 viewPos;
	float tmp_viewPos;
	float3 vecLight;
	float tmp_vecLight;
};

cbuffer CBPbr : register(b2)
{
	float4 baseColor;	 // 便宜上のパラメータ
	float roughness;
	float metallic;
	float2 tmp_pbr;
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD0;
	float3 vecView : TECOORD1;
};

float4 main(VSOutput input) : SV_TARGET
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);

	float3 BaseColor = baseColor.rgb;

	// BaseColors
	const float3 c_diff = lerp(BaseColor, float3(0, 0, 0), metallic);
	const float3 c_spec = lerp(0, BaseColor, metallic);

	const float clamped_roughness = max(0.005f, roughness);		// 0の時にハイライトがなくなるため
	const float alpha = clamped_roughness * clamped_roughness;

	const float3 N = normalize(input.normal);
	const float3 V = normalize(input.vecView);

	const float NdotV = saturate(dot(N, V));

	// ライト0
	{
		float3 LightColor = float3(1, 1, 1);

		const float3 L = normalize(-vecLight);
		const float3 H = normalize(L + V);

		const float NdotL = saturate(dot(N, L));
		const float LdotH = saturate(dot(L, H));
		const float NdotH = saturate(dot(N, H));

		float3 diffuseBRDF = c_diff * DIFFUSE_BRDF(NdotL);
		float3 specularBRDF = SPECULAR_BRDF(alpha, c_spec, NdotV, NdotL, LdotH, NdotH);

		color.rgb += NdotL * LightColor *  (diffuseBRDF + specularBRDF);
	}

	// diffuseIBL
	color.rgb += c_diff * texIrradiance.Sample(samLinear, N).rgb;

	// specularIBL
	float RADIANCE_LOD_MAX = 5;
	float3 R = reflect(-V, N);
	color.rgb += c_spec * texRadiance.SampleLevel(samLinear, R, roughness * RADIANCE_LOD_MAX).rgb;

	return pow(color, 1.0f/2.2f);
	//return color;
}