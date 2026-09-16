struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 localPos : TEXCOORD0;
};

// 텍스처 및 샘플러 레지스터
Texture2D MainTexture : register(t0);
SamplerState MainSampler : register(s0);

float4 mainPS(PS_INPUT input) : SV_TARGET
{
    float3 dir = normalize(input.localPos);
    float u = 0.5f + atan2(dir.x, dir.y) / 6.2831853f;
    float v = 0.5f - asin(clamp(dir.z, -1.0f, 1.0f)) / 3.1415926f;
    
    return MainTexture.Sample(MainSampler, float2(u, v));
}
