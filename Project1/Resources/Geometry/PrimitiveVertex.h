#pragma once

#include "Containers.h"
#include "FVertexSimple.h"



inline const TArray<FVertexData> triangle_vertices =
{
	{ 0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top vertex (red)
	{ -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f },  // Bottom-left vertex (blue)
	{ 1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right vertex (green)
};

inline const TArray<uint32> trianlge_indices = { 0, 1, 2, 2, 1, 0 };

inline const TArray<FVertexData> rectangle_vertices =
{
	// Triangle A
	{ -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }, // Bottom-left (red)
	{ 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right (green)
	{ -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // Top-left (blue)
	{ 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }, // Top-right (red)
};

inline const TArray<uint32> rectangle_indices = { 0, 1, 2, 1, 3, 2, 2, 1, 0, 2, 3, 1 };


inline const TArray<FVertexData> cube_vertices =
{
	{ -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f }, // 0
	{ 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f }, // 1
	{ 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f }, // 2
	{ -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // 3
	{ -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // 4
	{ 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f }, // 5
	{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f }, // 6
	{ -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f }, // 7
};

inline const TArray<uint32> cube_indices =
{
	7, 5, 6,   7, 4, 5,   // Front  (Z+)
	2, 0, 3,   2, 1, 0,   // Back   (Z-)
	3, 4, 7,   3, 0, 4,   // Left   (X-)
	6, 1, 2,   6, 5, 1,   // Right  (X+)
	7, 6, 2,   7, 2, 3,   // Top    (Y+)
	0, 1, 5,   0, 5, 4,   // Bottom (Y-)
};

inline const TArray<FVertexData> quad_vertices =
{
	{ -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-left (red)
	{ 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f }, // Bottom-right (green)
	{ -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f }, // Top-left (blue)
	{ 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f }, // Top-right (red)
};

inline const TArray<uint32> quad_indices = { 0, 2, 1, 1, 2, 3 };

inline const FVertexData font_quad_vertices[] =
{
	// Position              Color          UV                   Normal
	{ -0.5f,  0.5f, 0,      1,1,1,1,       33.0f / 128, 0,         -1,  1, 0 }, // 좌상
	{ 0.5f,  0.5f, 0,      1,1,1,1,       34.0f / 128, 0,          1,  1, 0 }, // 우상
	{ -0.5f, -0.5f, 0,      1,1,1,1,       33.0f / 128, 1.0f / 128,  -1, -1, 0 }, // 좌하
	{ 0.5f, -0.5f, 0,      1,1,1,1,       34.0f / 128, 1.0f / 128,   1, -1, 0 }, // 우하
};

inline const uint32 font_quad_indices[] = { 0, 1, 2, 2, 1, 3 };

const float INF_DIST = 10000.0f; // 우주 끝까지 거리
inline TArray<FVertexData> worldAxisVertices =
{
	// X축 (빨간색 선: -10000 ~ +10000)
	{ 0, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f },
	{ INF_DIST, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f },
	// Y축 (초록색 선: -10000 ~ +10000)
	{ 0.0f, 0, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f,  INF_DIST, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f },
	// Z축 (파란색 선: -10000 ~ +10000)
	{ 0.0f, 0.0f, 0,  0.0f, 0.0f, 1.0f, 1.0f },
	{ 0.0f, 0.0f,  INF_DIST,  0.0f, 0.0f, 1.0f, 1.0f }
};
