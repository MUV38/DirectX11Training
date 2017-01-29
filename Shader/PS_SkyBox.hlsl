#include "PBRCommon.hlsli"

Texture2D texDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

TextureCube		texCubeMap	: register(t1);
SamplerState	sampCubeMap
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

cbuffer CBMatrix : register(b0)
{
	float4x4 matWorld;
	float4x4 matView;
	float4x4 matProjection;
};

// 使用しない
cbuffer CBEnvironment : register(b1)
{
	float3 viewPos;
	float tmp_viewPos;
	float3 vecLight;
	float tmp_vecLight;
};

// 使用しない
cbuffer CBPbr : register(b2)
{
	float4 baseColor;	 // 便宜上のパラメータ
	float roughness;
	float metallic;
	float2 tmp_pbr;
};

struct VSOutput
{
	float4 pos		: SV_POSITION;
	float4 color	: COLOR0;
	float3 normal	: NORMAL;
	float2 texcoord : TEXCOORD0;
	float3 posLocal	: TEXCOORD1;
};

float4 main(VSOutput input) : SV_TARGET
{
	return texCubeMap.SampleLevel(sampCubeMap, input.posLocal, -1);
}