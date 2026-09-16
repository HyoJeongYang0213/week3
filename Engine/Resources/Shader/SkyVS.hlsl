#include "Constants.hlsli"

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 localPos : TEXCOORD0;
};

PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    
    float4 clipPos = mul(mul(float4(input.position, 1.0f), World), VP);
    output.position = clipPos.xyww;
    output.localPos = input.position;
    
    return output;
}
