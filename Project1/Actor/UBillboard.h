#pragma once
#include "AActor.h"
#include "Sphere.h"

class UBillboard : public AActor // UPrimitiveComponent
{
	DECLARE_CLASS(UBillboard, AActor)
public:
	UBillboard();
	UBillboard(const wstring& TexturePath);

	virtual void Update(float DeltaTime) override;
	virtual void Render() override;
};