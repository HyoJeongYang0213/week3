#include "pch.h"
#include "FGrid.h"

#include "FLineBatchRenderer.h"

void FGrid::Update(const FVector& CameraLocation)
{
	Location.X = floor(CameraLocation.X / CellSize) * CellSize;
	Location.Y = floor(CameraLocation.Y / CellSize) * CellSize;
	Location.Z = 0.0f;
}

void FGrid::AddLines() const
{
	static constexpr int32 HalfSize = GridSize / 2;

	for (int32 X = -HalfSize; X <= HalfSize; ++X) {
		FLinearColor lineColor;
		int32 WorldX = static_cast<int32>(round(Location.X / CellSize)) + X;
		if (WorldX == 0) continue;

		if (WorldX % Interval == 0)
			lineColor = FLinearColor::DarkGray;
		else if (WorldX % Interval != 0)
			lineColor = FLinearColor::Gray;

		LINEBATCH.AddLine(
			FVector(Location.X + X * CellSize, Location.Y - HalfSize * CellSize, 0.0f),
			FVector(Location.X + X * CellSize, Location.Y + HalfSize * CellSize, 0.0f),
			lineColor);
	}

	for (int32 Y = -HalfSize; Y <= HalfSize; ++Y) 
	{
		FLinearColor LineColor;
		int32 WorldY = static_cast<int32>(round(Location.Y / CellSize)) + Y;
		if (WorldY == 0) continue;

		if (WorldY % Interval == 0)
			LineColor = FLinearColor::DarkGray;
		else if (WorldY % Interval != 0)
			LineColor = FLinearColor::Gray;

		LINEBATCH.AddLine(
			FVector(Location.X - HalfSize * CellSize, Location.Y + Y * CellSize, 0.0f),
			FVector(Location.X + HalfSize * CellSize, Location.Y + Y * CellSize, 0.0f),
			LineColor);
	}
}
