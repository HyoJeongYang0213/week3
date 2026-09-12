#pragma once
#include "FVertexSimple.h"

class UGrid
{
public:
    UGrid() = default;
    ~UGrid();

    void Initialize();
    void Update(const FVector& CameraLocation);
    void Render();

    void SetCellSize(float inSize) { CellSize = inSize; }
    float GetCellSize() { return CellSize; }

    void CreateVertices();

private:
    float GridSize = 2000.0f;
    float CellSize = 0.3f;
    int interval = 10;

    FVector Location = FVector(0, 0, 0);
    MatrixBuffer* WorldBuffer = nullptr;
};