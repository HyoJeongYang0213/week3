// 텍스처 및 샘플러 레지스터
Texture2D MainTexture : register(t0);
SamplerState MainSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
};

float4 mainPS(PS_INPUT input) : SV_TARGET
{
    float coverage = MainTexture.Sample(MainSampler, input.uv).r;
    
    // 투명한 영역은 픽셀 폐기
    clip(coverage - 0.01f);
    
    return float4(input.color.rgb, coverage * input.color.a);
}
