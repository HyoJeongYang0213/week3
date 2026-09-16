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

	Mesh(const std::span<const FVector>& InVertices, const std::span<const uint32>& InIndices = {})
	{
		Vertices.Empty();
		for (const auto& Vertex : InVertices)
		{
			Vertices.Add(Vertex);
		}
		Indices.Empty();
		for (const auto& Index : InIndices)
		{
			Indices.Add(Index);
		}

		ComputeLocalBoundingBox();
	}

	Mesh(const std::span<const FVertexData>& InVertices, const std::span<const uint32>& InIndices = {})
	{
		Vertices.Empty();
		for (const auto& Vertex : InVertices)
		{
			Vertices.Add(FVector(Vertex.x, Vertex.y, Vertex.z));
		}
		Indices.Empty();
		for (const auto& Index : InIndices)
		{
			Indices.Add(Index);
		}

		ComputeLocalBoundingBox();
	}

	bool IsPicked(const FRay& worldRay, const Transform& transform, float& outDistance);
	FBoundingBox GetBoundingBox() const { return BoundingBox; };
	void ComputeLocalBoundingBox();

public:
	TArray<FVector> Vertices = {};
	TArray<int32> Indices = {};

private:
	FBoundingBox BoundingBox{};
};


