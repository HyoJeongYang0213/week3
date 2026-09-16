#include "pch.h"
#include "UBillboard.h"

UBillboard::UBillboard()
{
	SetMesh(OBJECT.GetOrCreateMesh("Billboard", quad_vertices));
}

UBillboard::UBillboard(const FString& InTextureName) : UBillboard()
{
	TextureName = InTextureName;
}

void UBillboard::Update(float DeltaTime)
{
	SetRotation(CAMERA.GetRotation());
	AActor::Update(DeltaTime);
}