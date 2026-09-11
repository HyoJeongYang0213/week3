#pragma once

#include "FVertexSimple.h"
#include "RenderMesh.h"

struct FGridRenderData final
{
	const RenderMesh* Mesh;
	FMatrix World;
};

class UGrid
{
public:
	UGrid();

	void Update(const FVector& CameraLocation);
	FGridRenderData GetRenderData() const;

	TArray<FVertexData> CreateVertices();

private:
	float GridSize = 100.0f;
	float CellSize = 1.0f;

	FVector Location = FVector(0, 0, 0);
    float GridSize = 2000.0f;
    float CellSize = 0.3f;
    int interval = 10;

	const RenderMesh* GridMesh = nullptr;
};