#pragma once

#include "FVector.h"

//struct FBillboardVertex
//{
//	FVector Position;
//	FVector2D UV;
//};
//
//inline const TArray<FBillboardVertex> quad_vertices =
//{
//	{ FVector(-0.5f, -0.5f, 0.0f), FVector2D(0.0f, 1.0f) }, // Bottom-left
//	{ FVector(0.5f, -0.5f, 0.0f), FVector2D(1.0f, 1.0f) }, // Bottom-right
//	{ FVector(-0.5f,  0.5f, 0.0f), FVector2D(0.0f, 0.0f) }, // Top-left 
//	{ FVector(0.5f,  0.5f, 0.0f), FVector2D(1.0f, 0.0f) }, // Top-right 
//};

inline const uint32 quad_indices[] = { 0, 2, 1, 1, 2, 3 };

inline TArray<FVertexData> quad_vertices =
{
	// Triangle A
	{ -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-left (red)
	{ 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f }, // Bottom-right (green)
	{ -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f }, // Top-left (blue)

	// Triangle B
	{ 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f }, // Bottom-right (green)
	{ 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f }, // Top-right (red)
	{ -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f }, // Top-left (blue)
};