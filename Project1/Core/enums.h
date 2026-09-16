#pragma once

#include "Containers.h"

enum class EPrimitive
{
	Sphere,
	Cube,
	Circle,
	Rectangle,
	Triangle,
	PointLight,
	SpotLight,
	DirectionalLight,
	Gizmo,
	SubUV,
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
	SF_None = 0u,
	SF_Primitives = 1u << 0,
	SF_BillboardText = 1u << 1,
};

constexpr EEngineShowFlags operator|(EEngineShowFlags a, EEngineShowFlags b)
{
	return static_cast<EEngineShowFlags>(static_cast<uint32>(a) | static_cast<uint32>(b));
}

constexpr EEngineShowFlags operator&(EEngineShowFlags a, EEngineShowFlags b)
{
	return static_cast<EEngineShowFlags>(static_cast<uint32>(a) & static_cast<uint32>(b));
}

constexpr EEngineShowFlags operator^(EEngineShowFlags a, EEngineShowFlags b)
{
	return static_cast<EEngineShowFlags>(static_cast<uint32>(a) ^ static_cast<uint32>(b));
}
