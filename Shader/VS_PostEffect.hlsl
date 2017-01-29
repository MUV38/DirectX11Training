
//--------------------------------------------------------------------------------------
//		コンスタントバッファ
//--------------------------------------------------------------------------------------
cbuffer cbTransformMatrix : register( c0 )
{
	matrix	matWorld;
	matrix	matProjection;
};

//--------------------------------------------------------------------------------------
//		入出力レイアウト
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : SV_Position;
	float2 Tex : TEXCOORD;
	float4 Color : COLOR;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float4 Color : COLOR0;
};

//--------------------------------------------------------------------------------------
//		頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT	main( VS_INPUT In )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	Out.Pos = mul( In.Pos, matWorld );
	Out.Pos = mul( Out.Pos, matProjection );
	Out.Tex = In.Tex;
	Out.Color = In.Color;

	return Out;
}
