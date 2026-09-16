#include "pch.h"
#include "Mesh.h"
#include "Intersection.h"

bool Mesh::IsPicked(const FRay& worldRay, const Transform& transform, float& outDistance)
{
	if (Vertices.size() < 3) return false;

	bool bSuccess = false;
	FMatrix invWorld = transform.WorldMat.Inverse(&bSuccess);
	if (!bSuccess)
	{
		return false;
	}

	FVector localOrigin = TransformPoint(worldRay.Origin, invWorld);
	FVector localDir = TransformDirection(worldRay.Direction, invWorld);

	float closestDist = FLT_MAX;
	bool bHit = false;

	FVector VertexA;
	FVector VertexB;
	FVector VertexC;
	uint32 LoopCount;

	if (Indices.IsEmpty())
	{
		LoopCount = Vertices.Num();
	}
	else
	{
		LoopCount = Indices.Num();
	}

	// 삼각형 충돌 검사
	for (size_t i = 0; i + 2 < LoopCount; i += 3)
	{
		if (Indices.IsEmpty())
		{
			VertexA = Vertices[i];
			VertexB = Vertices[i + 1];
			VertexC = Vertices[i + 2];
		}
		else
		{
			VertexA = Vertices[Indices[i]];
			VertexB = Vertices[Indices[i + 1]];
			VertexC = Vertices[Indices[i + 2]];
		}

		float dist = 0.0f;
		if (RayIntersectTriangle(localOrigin, localDir, VertexA, VertexB, VertexC, dist))
		{
			if (dist > 0.0f && dist < closestDist)
			{
				closestDist = dist;
				bHit = true;
			}
		}
	}


	if (bHit)
	{
		// localDir을 정규화하지 않았으므로 closestDist가 곧바로 정확한 월드 거리
		outDistance = closestDist;


		return true;
	}
	return false;
}

void Mesh::ComputeLocalBoundingBox()
{
	if (Vertices.empty()) return;

	BoundingBox.minX = BoundingBox.maxX = Vertices[0].x;
	BoundingBox.minY = BoundingBox.maxY = Vertices[0].y;
	BoundingBox.minZ = BoundingBox.maxZ = Vertices[0].z;

	for (auto vertice : Vertices) {
		BoundingBox.minX = min(BoundingBox.minX, vertice.x);
		BoundingBox.maxX = max(BoundingBox.maxX, vertice.x);
		BoundingBox.minY = min(BoundingBox.minY, vertice.y);
		BoundingBox.maxY = max(BoundingBox.maxY, vertice.y);
		BoundingBox.minZ = min(BoundingBox.minZ, vertice.z);
		BoundingBox.maxZ = max(BoundingBox.maxZ, vertice.z);
	}
}
