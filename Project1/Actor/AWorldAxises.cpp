#include "pch.h"
#include "AWorldAxises.h"
#include "FVertexSimple.h"
#include "Ulinebatch.h"

AWorldAxises::AWorldAxises()
{
	// 오브젝트 매니저를 통해 축선 메시 캐시 사용
	SetMesh(OBJECT.GetOrCreateMesh("WorldAxises", worldAxisVertices));
	SetPrimitive(EPrimitive::None);
}

void AWorldAxises::Render()
{
	UObject::Render();

	LINEBATCH.AddLine(FVector(-INF_DIST, 0, 0), FVector(INF_DIST, 0, 0), FLinearColor::Red);
	LINEBATCH.AddLine(FVector(0, -INF_DIST, 0), FVector(0, INF_DIST, 0), FLinearColor::Green);
	LINEBATCH.AddLine(FVector(0, 0, -INF_DIST), FVector(0, 0, INF_DIST), FLinearColor::Blue);
}



