#include "pch.h"
#include "UGrid.h"

#include "FVertexSimple.h"
#include "ULineBatch.h"

UGrid::UGrid()
{
	GridMesh = &RESOURCES.CreateMesh("Grid", CreateVertices());
}

void UGrid::Update(const FVector& CameraLocation) {
	Location.x = floor(CameraLocation.x / CellSize) * CellSize;
	Location.y = 0.0f;
	Location.z = floor(CameraLocation.z / CellSize) * CellSize;
}

FGridRenderData UGrid::GetRenderData() const
{
	return FGridRenderData{ GridMesh, FMatrix::Translation(Location) };
}

TArray<FVertexData> UGrid::CreateVertices() {
	const int HalfSize = GridSize / 2;
	TArray<FVertexData> Vertices;

	for (int z = -HalfSize; z < HalfSize; ++z) {
		for (int x = -HalfSize; x < HalfSize; ++x) {
			float x0 = x * CellSize;
			float x1 = (x + 1) * CellSize;

			float z0 = z * CellSize;
			float z1 = (z + 1) * CellSize;

			// 첫번째 삼각형
			Vertices.Add(FVertexData{ x0, 0.0f, z0 });
			Vertices.Add(FVertexData{ x0, 0.0f, z1 });
			Vertices.Add(FVertexData{ x1, 0.0f, z1 });

			// 두번째 삼각형
			Vertices.Add(FVertexData{ x0, 0.0f, z0 });
			Vertices.Add(FVertexData{ x1, 0.0f, z1 });
			Vertices.Add(FVertexData{ x1, 0.0f, z0 });
		}
	}

	return Vertices;
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
          FVector(Location.x - HalfSize * CellSize, 0.01f, Location.z + z * CellSize),
          FVector(Location.x + HalfSize * CellSize, 0.01f, Location.z + z * CellSize),
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
          FVector(Location.x + x * CellSize, 0.01f, Location.z - HalfSize * CellSize),
          FVector(Location.x + x * CellSize, 0.01f, Location.z + HalfSize * CellSize),
          lineColor);
  }
}
