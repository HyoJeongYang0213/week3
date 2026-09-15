#pragma once
#include "Containers.h"
#include "Transform.h"
#include "FBoundingBox.h"
#include <span>

struct FRay;

class Mesh
{
public:
	Mesh() = default;

	Mesh(const std::span<const FVector>& InVertices)
	{
		Vertices.clear();
		for (const auto& Vertex : InVertices)
		{
			Vertices.push_back(Vertex);
		}

		ComputeLocalBoundingBox();
	}

	Mesh(const std::span<const FVertexData>& InVertices)
	{
		Vertices.clear();
		for (const auto& Vertex : InVertices)
		{
			Vertices.push_back(FVector(Vertex.x, Vertex.y, Vertex.z));
		}

		ComputeLocalBoundingBox();
	}

	bool IsPicked(const FRay& worldRay, const Transform& transform, float& outDistance);
	FBoundingBox GetBoundingBox() const { return BoundingBox; };
	void ComputeLocalBoundingBox();

public:
	TArray<FVector> Vertices;

private:
	FBoundingBox BoundingBox{};
};


