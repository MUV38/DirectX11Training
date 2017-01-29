cbuffer CBMatrix : register(b0)
{
	float4x4 matWorld;
	float4x4 matView;
	float4x4 matProjection;
};

cbuffer CBEnvironment : register(b1)
{
	float3 viewPos;
	float3 vecLight;
	float2 tmp_env;
};

cbuffer CBPbr : register(b2)
{
	float roughness;
	float3 tmp_pbr;
};


struct VSInput
{
	float4 pos : SV_Position;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD0;
	float3 vecView : TECOORD1;
};

VSOutput main( VSInput input )
{
	VSOutput output = (VSOutput)0;

	float4x4 mat = mul(mul(matWorld, matView), matProjection);
	output.pos = mul(input.pos, mat);
	output.texcoord = input.texcoord;
	output.normal = mul(input.normal, (float3x3)matWorld);
	output.normal = normalize(output.normal);
	output.color = input.color;

	// éãê¸ÉxÉNÉgÉã
	float4 worldPos = mul(input.pos, matWorld);
	output.vecView = normalize(viewPos - worldPos.xyz);

	return output;
}