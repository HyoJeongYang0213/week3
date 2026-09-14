#include "pch.h"
#include "AWorldAxes.h"
#include "FVertexSimple.h"
#include "FLineBatchRenderer.h"

AWorldAxes::AWorldAxes()
{
	SetPrimitive(EPrimitive::None);
}

void AWorldAxes::AddLines()
{
	LINEBATCH.AddLine(FVector(-INF_DIST, 0, 0), FVector(INF_DIST, 0, 0), FLinearColor::Red);
	LINEBATCH.AddLine(FVector(0, -INF_DIST, 0), FVector(0, INF_DIST, 0), FLinearColor::Green);
	LINEBATCH.AddLine(FVector(0, 0, -INF_DIST), FVector(0, 0, INF_DIST), FLinearColor::Blue);
}



