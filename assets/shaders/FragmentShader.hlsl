#define HLSL
#include "CommonRT.hlsl"

struct VSOut {
	float4 position : SV_POSITION;
	float3 normal 	: NORMAL0;
	float2 texCoord : TEXCOORD0;
	float3 worldPos : WORLDPOS;
};

Texture2DArray tex : register(MERGE(t, TEX_REG_DIFFUSE_SLOT));
SamplerState ss : register(s0);
SamplerState static_ss : register(s1);

cbuffer CB_DiffuseTint : register(MERGE(b, CB_REG_DIFFUSE_TINT)) {
	float4 diffuseTint;
}

float4 PSMain(VSOut input) : SV_TARGET0 {
	UINT width;
	UINT height;
	UINT numElements;
	UINT numberOfLevels;
	float3 lightDir = normalize(input.worldPos - float3(100, 500, 100));
	float s = dot(input.normal, lightDir);
	float3 lightDir2 = normalize(input.worldPos - float3(0, 500, 0));
	float s2 = dot(input.normal, lightDir);
	s = s + 0.3;
	s = clamp(s, 0.7, 1.0);
	tex.GetDimensions(0, width, height, numElements, numberOfLevels);
    float4 color = tex.Sample(static_ss, float3(input.texCoord, numElements - 1));
    //color = float4(input.texCoord, 0.0, 0.0);
	return color * s * float4(diffuseTint.rgb, 1.0);
}