cbuffer ObjectConstants : register(b0)
{
    matrix World;
}

cbuffer FrameConstants : register(b1)
{
    matrix VP;
    float3 CameraPos;
    float pad;
}

cbuffer ColorConstants : register(b2)
{
    float4 CustomColor;
    int UseTexture;
    float3 ColorPad;
}
