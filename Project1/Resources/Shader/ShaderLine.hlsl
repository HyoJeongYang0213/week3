cbuffer ObjectConstants : register(b0)
{
    matrix World;
};

cbuffer FrameConstants : register(b1)
{
    matrix VP;
    float3 CameraPos;
    float pad;
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
};

PS_INPUT mainVS_Line(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    float4 worldPos = mul(float4(input.position, 1.0f), World);

    output.position = mul(worldPos, VP);
    output.color = input.color;

    return output;
}

float4 mainPS_Line(PS_INPUT input) : SV_TARGET
{
    return input.color;
}