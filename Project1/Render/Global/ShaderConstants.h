#pragma once

struct ObjectConstants
{
	FMatrix World;
};
static_assert(sizeof(ObjectConstants) % 16 == 0);

struct FrameConstants
{
	FMatrix VP;
	FVector CameraPos;
	float Padding;
};
static_assert(sizeof(FrameConstants) % 16 == 0);

struct ColorConstants
{
	FLinearColor CustomColor;
	int32 UseTexture = false;
	float Padding[3];
};
static_assert(sizeof(ColorConstants) % 16 == 0);
