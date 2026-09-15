#pragma once
#include "AActor.h"

class ALight : public AActor{
	DECLARE_CLASS(ALight, AActor)

public:
	ALight(const FLinearColor& color = FLinearColor::White);

	void DrawCircle(const FVector& Center, const FVector& AxisA, const FVector& AxisB, const float Radius);

	virtual void Render() override;
private:

};