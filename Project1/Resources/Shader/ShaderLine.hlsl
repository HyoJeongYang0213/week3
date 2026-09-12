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
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 worldPosition : TEXCOORD0;
};

PS_INPUT mainVS_Line(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    float4 worldPos = mul(float4(input.position, 1.0f), World);

    output.position = mul(worldPos, VP);
    output.color = input.color;
    output.worldPosition = worldPos.xyz;

    return output;
}

float4 mainPS_Line(PS_INPUT input) : SV_TARGET
{
    // 거리기반
    float distance = length(input.worldPosition - CameraPos);
    
    // fov 기반
    float grazing = abs(dot(normalize(CameraForward), float3(0, 1, 0)));
    float fadeStart = 1.0f;
    float fadeEnd = 30.0f;
    float finalFadeEnd = lerp(5.0f, fadeEnd, grazing);
   
    float fade = 1.0f - smoothstep(fadeStart, finalFadeEnd, distance);
    
    clip(fade - 0.01f); // 완전히 투명해야하면 그냥 픽셸 버리기 
    
    float4 finalColor = input.color;
    finalColor.a *= fade;
    
    return finalColor;
}