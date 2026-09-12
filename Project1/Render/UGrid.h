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
	void Render();
	void Update(const FVector& CameraLocation);

    void Initialize();

    void SetCellSize(float inSize) { CellSize = inSize; }
    float GetCellSize() { return CellSize; }

    void CreateVertices();

private:
	FVector Location = FVector(0, 0, 0);
    float GridSize = 2000.0f;
    float CellSize = 0.3f;
    int interval = 10;
};