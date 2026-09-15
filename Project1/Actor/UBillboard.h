#pragma once
#include "AActor.h"
#include "Sphere.h"

class UBillboard : public AActor // UPrimitiveComponent
{
	DECLARE_CLASS(UBillboard, AActor)
public:
	UBillboard() = default;
	UBillboard(const FString& TextureName);

	virtual void Update(float DeltaTime) override;

	FString GetRenderMeshName() const override { return "Billboard"; }
	const FString& GetTextureName() const { return TextureName; }

private:
	FString TextureName;
};
