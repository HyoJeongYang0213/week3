#include "pch.h"
#include "FGrid.h"

#include "FLineBatchRenderer.h"

void FGrid::Update(const FVector& CameraLocation)
{
	Location.x = floor(CameraLocation.x / CellSize) * CellSize;
	Location.y = 0.0f;
	Location.z = floor(CameraLocation.z / CellSize) * CellSize;
}

void FGrid::AddLines() const
{
	static constexpr int32 HalfSize = GridSize / 2;

	for (int32 Z = -HalfSize; Z <= HalfSize; ++Z) 
	{
		FLinearColor LineColor;
		int32 WorldZ = static_cast<int32>(round(Location.z / CellSize)) + Z;
		if (WorldZ == 0) continue;

		if (WorldZ % Interval == 0)
			LineColor = FLinearColor::DarkGray;
		else if (WorldZ % Interval != 0)
			LineColor = FLinearColor::Gray;

		LINEBATCH.AddLine(
			FVector(Location.x - HalfSize * CellSize, 0.0f, Location.z + Z * CellSize),
			FVector(Location.x + HalfSize * CellSize, 0.0f, Location.z + Z * CellSize),
			LineColor);
	}
	for (int32 X = -HalfSize; X <= HalfSize; ++X) {
		FLinearColor lineColor;
		int32 worldX = static_cast<int32>(round(Location.x / CellSize)) + X;

		if (worldX == 0) continue;
		if (worldX % Interval == 0)
			lineColor = FLinearColor::DarkGray;
		else if (worldX % Interval != 0)
			lineColor = FLinearColor::Gray;

		LINEBATCH.AddLine(
			FVector(Location.x + X * CellSize, 0.0f, Location.z - HalfSize * CellSize),
			FVector(Location.x + X * CellSize, 0.0f, Location.z + HalfSize * CellSize),
			lineColor);
	}
}
