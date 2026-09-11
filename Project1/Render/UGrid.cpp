#include "pch.h"
#include "UGrid.h"
#include "FVertexSimple.h"
#include "ULineBatch.h"

UGrid::~UGrid() {
  if (WorldBuffer) {
    delete WorldBuffer;
    WorldBuffer = nullptr;
  }
}

void UGrid::Initialize() {
  WorldBuffer = new MatrixBuffer();
}

void UGrid::Render() {
  int HalfSize = GridSize / 2;
  for (int z = -HalfSize; z <= HalfSize; ++z) {
      LINEBATCH.AddLine(
          FVector(Location.x - HalfSize * CellSize, 0.01f, Location.z + z * CellSize),
          FVector(Location.x + HalfSize * CellSize, 0.01f, Location.z + z * CellSize),
          FLinearColor::Gray);
  }
  for (int x = -HalfSize; x <= HalfSize; ++x) {
      LINEBATCH.AddLine(
          FVector(Location.x + x * CellSize, 0.01f, Location.z - HalfSize * CellSize),
          FVector(Location.x + x * CellSize, 0.01f, Location.z + HalfSize * CellSize),
          FLinearColor::Gray);
  }
}

void UGrid::Update(const FVector &CameraLocation) {
  Location.x = floor(CameraLocation.x / CellSize) * CellSize;
  Location.y = 0.0f;
  Location.z = floor(CameraLocation.z / CellSize) * CellSize;
}