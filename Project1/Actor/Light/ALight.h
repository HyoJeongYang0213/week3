#pragma once
#include "AActor.h"

class ALight : public AActor{
	DECLARE_CLASS(ALight, AActor)

public:
	ALight(const FLinearColor& color = FLinearColor::White);

	FString GetRenderMeshName() const override { return "LightBulb"; };

	virtual void AddLines() {};

protected:
	void DrawCircle(const FVector& Center, const FVector& AxisA, const FVector& AxisB, const float Radius);
};