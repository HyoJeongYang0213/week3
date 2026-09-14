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
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
};

// 일반 버텍스 셰이더
PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    
    float4 worldPos = mul(float4(input.position, 1.0f), World);
    output.position = mul(worldPos, VP);
    output.worldPosition = worldPos.xyz;
    output.uv = input.uv;
    output.normal = normalize(mul(input.normal, (float3x3) World));
    
    if (CustomColor.a > 0.0f)
    {
        output.color = CustomColor;
    }
    else
    {
        output.color = input.color;
    }
    
    return output;
}
