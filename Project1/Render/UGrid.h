#pragma once

#include "FVertexSimple.h"

class UGrid
{
public:
	void Render();
	void Update(const FVector& CameraLocation);

    void SetCellSize(float inSize) { CellSize = inSize; }
    float GetCellSize() { return CellSize; }

private:
	FVector Location = FVector(0, 0, 0);
    float GridSize = 2000.0f;
    float CellSize = 0.3f;
    int interval = 10;
};