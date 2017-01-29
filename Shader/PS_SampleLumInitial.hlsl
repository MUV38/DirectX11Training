//--------------------------------------------------------------------------------------
//		テクスチャ
//--------------------------------------------------------------------------------------
Texture2D	decaleTex : register( t0 );
SamplerState	sampLinear : register( s0 );

//--------------------------------------------------------------------------------------
//		定数
//--------------------------------------------------------------------------------------
static const int	MAX_SAMPLE = 9;
static const float3 LUMINANCE_VECTOR = float3(0.2125f, 0.7154f, 0.0721f);

//--------------------------------------------------------------------------------------
//		コンスタントバッファ
//--------------------------------------------------------------------------------------
cbuffer CBSampleLumInitial : register(b0)
{
	float2	sampleOffset[MAX_SAMPLE];
	float2	tmp;
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

	float3 color = 0;
	float logLumSum = 0;

	for (int i = 0; i < MAX_SAMPLE; i++) {
		color = decaleTex.Sample(sampLinear, Tex + sampleOffset[i]).rgb;
		logLumSum += log(dot(color, LUMINANCE_VECTOR) + 0.0001f);
	}

	logLumSum /= 9.0f;

	return float4(logLumSum, logLumSum, logLumSum, 1.0f);
}