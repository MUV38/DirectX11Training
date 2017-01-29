//--------------------------------------------------------------------------------------
//		テクスチャ
//--------------------------------------------------------------------------------------
Texture2D	decaleTex : register( t0 );
SamplerState	sampLinear : register( s0 );

SamplerState	sampPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

//--------------------------------------------------------------------------------------
//		入出力レイアウト
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float4 Color : COLOR0;
};


//--------------------------------------------------------------------------------------
//		ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4	main( VS_OUTPUT In ) : SV_TARGET
{
	float2 Tex = float2(In.Tex.y, In.Tex.x);

	// tonemap reinhard
	float4 SceneColor = decaleTex.Sample(sampPoint, Tex);
	//SceneColor.rgb = SceneColor.rgb / (1.0f+SceneColor.rgb);

	// gamma
	//SceneColor.rgb = pow(SceneColor.rgb, 1.0f/2.2f);

	return SceneColor;
}