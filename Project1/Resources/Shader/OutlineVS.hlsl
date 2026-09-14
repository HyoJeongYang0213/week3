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

PS_INPUT mainVS(VS_INPUT input)
{
	PS_INPUT output;
	
	float outlinePixels = CustomColor.x;
	float screenWidth = CustomColor.y;
	float screenHeight = CustomColor.z;
	
	float3 norm = length(input.normal) > 0.001f ? input.normal : input.position;
	float len = length(norm);
	norm = (len > 0.0001f) ? (norm / len) : float3(0.0f, 1.0f, 0.0f);

	float4 clipPos = mul(mul(float4(input.position, 1.0f), World), VP);
	float4 clipNormal = mul(mul(float4(norm, 0.0f), World), VP);
	
	float2 offsetDir = normalize(clipNormal.xy + 0.00001f);
	float2 pixelToNdc = float2(outlinePixels * 2.0f / screenWidth, outlinePixels * 2.0f / screenHeight);
	
	clipPos.xy += offsetDir * pixelToNdc * clipPos.w;
	
	float4 worldPos = mul(float4(input.position, 1.0f), World);

	output.position = clipPos;
	output.worldPosition = worldPos.xyz;
	output.color = float4(1.0f, 1.0f, 0.0f, 1.0f);
	output.uv = input.uv;
	output.normal = norm;
	return output;
}
