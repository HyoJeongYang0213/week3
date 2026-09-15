#include "pch.h"
#include "UBillboard.h"

UBillboard::UBillboard(const FString& TextureName) : TextureName(TextureName) {}

void UBillboard::Update(float DeltaTime)
{
	SetRotation(CAMERA.GetRotation());
	AActor::Update(DeltaTime);
}
