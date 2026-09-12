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
      FLinearColor lineColor;
      int worldZ = (int)round((Location.z + z * CellSize) / CellSize);

      if (worldZ % interval == 0)
          lineColor = FLinearColor::DarkGray;
      else if (worldZ % interval != 0)
          lineColor = FLinearColor::Gray;
      
      LINEBATCH.AddLine(
          FVector(Location.x - HalfSize * CellSize, 0.0f, Location.z + z * CellSize),
          FVector(Location.x + HalfSize * CellSize, 0.0f, Location.z + z * CellSize),
          lineColor);
  }
  for (int x = -HalfSize; x <= HalfSize; ++x) {
      FLinearColor lineColor;
      int worldX = (int)round((Location.x + x * CellSize) / CellSize);

      if (worldX % interval == 0)
          lineColor = FLinearColor::DarkGray;
      else if (worldX % interval != 0)
          lineColor = FLinearColor::Gray;

      LINEBATCH.AddLine(
          FVector(Location.x + x * CellSize, 0.0f, Location.z - HalfSize * CellSize),
          FVector(Location.x + x * CellSize, 0.0f, Location.z + HalfSize * CellSize),
          lineColor);
  }
}

void UGrid::Update(const FVector &CameraLocation) {
  Location.x = floor(CameraLocation.x / CellSize) * CellSize;
  Location.y = 0.0f;
  Location.z = floor(CameraLocation.z / CellSize) * CellSize;
}