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
	float4 texColor = MainTexture.Sample(MainSampler, input.uv);
	clip(texColor.g - 0.01f);
	return texColor;
}
