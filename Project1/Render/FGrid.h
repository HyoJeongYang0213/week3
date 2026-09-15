#pragma once

#include "Containers.h"

class FGrid
{
public:
	void AddLines() const;
	void Update(const FVector& CameraLocation);

	float CellSize = 0.3f;

private:
	FVector Location = FVector(0, 0, 0);
	static constexpr int32 GridSize = 2000;
	static constexpr int32 Interval = 10;
};