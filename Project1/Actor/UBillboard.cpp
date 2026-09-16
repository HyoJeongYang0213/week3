#include "pch.h"
#include "UBillboard.h"

UBillboard::UBillboard(const FString& InTextureName)
{
	TextureName = InTextureName;
	SetMesh(OBJECT.GetOrCreateMesh("Billboard", quad_vertices, quad_indices));
}

void UBillboard::Update(float DeltaTime)
{
	SetRotation(CAMERA.GetRotation());
	AActor::Update(DeltaTime);
}