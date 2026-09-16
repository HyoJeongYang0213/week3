#include "pch.h"
#include "UBillboard.h"
#include "PrimitiveVertex.h"

UBillboard::UBillboard()
{
	SetMesh(OBJECT.GetOrCreateMesh("Billboard", quad_vertices));
}

UBillboard::UBillboard(const FString& InTextureName) : UBillboard()
{
	TextureName = InTextureName;
	SetMesh(OBJECT.GetOrCreateMesh("Billboard", quad_vertices, quad_indices));
}

void UBillboard::Update(float DeltaTime)
{
	SetRotation(CAMERA.GetRotation());
	AActor::Update(DeltaTime);
}