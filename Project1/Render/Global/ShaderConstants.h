#pragma once

#include "FVector.h"

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

struct SubUVConstants
{
	FVector2D UVScale{ 1.0f, 1.0f };
	FVector2D UVOffset{ 0.0f, 0.0f };
};
static_assert(sizeof(SubUVConstants) % 16 == 0);
