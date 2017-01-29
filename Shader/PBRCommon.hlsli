#ifndef _PBR_COMMON_HLSLI_
#define _PBR_COMMON_HLSLI_

#include "Math.hlsli"

#define DIFFUSE_BRDF LambertBRDF
#define SPECULAR_BRDF Specular_BRDF

// Fresnel
float3 F_Schlick(in float3 f0, in float3 f90, in float u)
{
	return f0 + (f90 - f0) * pow(1.0f - u, 5.0f);
}

//----------------------------------------------------------------------------------
//	DiffuseBRDF
//----------------------------------------------------------------------------------
float LambertBRDF(in float NdotL)
{
	return NdotL * (1.0f / PI);
}

//float DisneyBRDF(in float NdotV, in float NdotL, in float LdotH, in float, in float roughtness)
//{
//	float energyBias	= lerp(0.0f, 0.5f, roughness);
//	float energyFactor	= lerp(1.0f, 1.0f/1.5f, roughness);
//	float fd90			= energyBias + 2.0f * LdotH*LdotH * roughness;
//	float3 f0			= float3(1.0f, 1.0f, 1.0f);
//	float lightScatter	= F_Schlick(f0, fd90, NdotL).r;
//	float viewScatter	= F_Schlick(f0, fd90, NdotV).r;
//
//	return lightScatter * viewScatter * energyFactor;
//}

//----------------------------------------------------------------------------------
//	SpecularBRDF
//----------------------------------------------------------------------------------
// GGX NDF SpecularD
float Specular_D_GGX(in float alpha, in float NdotH)
{
	const float alpha2 = alpha * alpha;
	const float lower = (NdotH * NdotH * (alpha2 - 1.0f)) + 1.0f;
	const float result = alpha2 / (PI * lower * lower);

	return result;
}

// Schlick-Smith SpecularG
float G_Schlick_Smith(in float alpha, in float LdotH)
{
	const float k = alpha / 2.0f;
	const float k2 = k * 2.0f;
	const float invk2 = 1.0f - k2;

	return rcp(LdotH * LdotH * invk2 + k2);
}

// SpecularBRDF
float3 Specular_BRDF(in float alpha, in float3 specularColor, in float NdotV, in float NdotL, in float LdotH, in float NdotH)
{
	// D (NDF)
	float D = Specular_D_GGX(alpha, NdotH);

	// F (Fresnel)
	float3 F = F_Schlick(specularColor, 1.0f, LdotH);

	// G (visibility)
	float G = G_Schlick_Smith(alpha, LdotH);

	return (D * F * G);
}

#endif 