cbuffer ObjectConstants : register(b0)
{
    matrix World;
};

cbuffer FrameConstants : register(b1)
{
    matrix VP;
    float3 CameraPos;
    float pad;
    float3 CameraForward;
    float pad2;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 worldPosition : TEXCOORD0;
    float overlay : TEXCOORD1;
};

PS_INPUT mainVS_Line(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    float4 worldPos = mul(float4(input.position, 1.0f), World);
    output.position = mul(worldPos, VP);
    
    if(input.uv.x > 0.5f)
        output.position.z -= 0.0005f * output.position.w;
    
    output.color = input.color;
    output.worldPosition = worldPos.xyz;
    output.overlay = input.uv.x;

    return output;
}

float4 mainPS_Line(PS_INPUT input) : SV_TARGET
{
    if(input.overlay > 0.5f) return input.color;
    
    // 거리기반
    float distance = length(input.worldPosition - CameraPos);
    
    // fov 기반
    float h = abs(CameraPos.y - input.worldPosition.y);
    float fadeEnd = min(10.0f * sqrt(h), 40.0f);
    float fadeStart = fadeEnd * 0.3f;

    float fade = 1.0f - smoothstep(fadeStart, fadeEnd, distance);
    
    clip(fade - 0.01f); // 완전히 투명해야하면 그냥 픽셸 버리기 
    
    float4 finalColor = input.color;
    finalColor.a *= fade;
    
    return finalColor;
}