#pragma once
#include "AActor.h"
#include "Sphere.h"

class UBillboard : public AActor // UPrimitiveComponent
{
	DECLARE_CLASS(UBillboard, AActor)
public:
	UBillboard();
	UBillboard(const FString& InTextureName);

	virtual void Update(float DeltaTime) override;

	void SetTextureName(const FString& name) { TextureName = name; }
	FString GetRenderMeshName() const override { return "Billboard"; }
	const FString& GetTextureName() const { return TextureName; }

private:
	FString TextureName;
};
