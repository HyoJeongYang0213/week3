#pragma once

#include "Containers.h"

enum class EPrimitive
{
	Sphere,
	Cube,
	Circle,
	Rectangle,
	Triangle,
	Gizmo,
	None
};

enum class EGizmoAxis
{
	X,
	Y,
	Z,
	None
};

enum class EGizmoMode
{
	Translation,
	Rotation,
	Scale
};

enum class EGridType
{
	Line,
	Triangle
};

enum class EEngineShowFlags : uint32
{
	SF_Primitives = 1ull << 0,
	SF_BillboardText = 1ull << 1,
};