cbuffer ObjectConstants : register(b0) // FConstants
{
    matrix World;
};
cbuffer FrameConstants : register(b1) // FFrameConstants
{
    matrix VP;
    float3 CameraPos;
    float pad;
};
cbuffer ColorConstants : register(b2)
{
    float4 CustomColor;
    int UseTexture;
    float3 ColorPad;
};

// 텍스처 및 샘플러 레지스터
Texture2D MainTexture : register(t0);
SamplerState MainSampler : register(s0);

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

// 일반 픽셀 셰이더
float4 mainPS(PS_INPUT input) : SV_TARGET
{
    float coverage = MainTexture.Sample(MainSampler, input.uv).r;
    
    // 투명한 영역은 픽셀 폐기
    clip(coverage - 0.01f);
    
    return float4(input.color.rgb, coverage * input.color.a);
}