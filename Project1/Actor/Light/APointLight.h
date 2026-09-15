#pragma once
#include "ALight.h"

class APointLight : public ALight{
	DECLARE_CLASS(APointLight, ALight)

public:
	APointLight(const FLinearColor& color = FLinearColor::White);

	float GetRadius() { return Radius; }
	void SetRadius(float angle) { Radius = angle; }

	virtual void AddLines() override;

	virtual void BeginGizmoScale() override { DrawStartRadius = Radius; }
	virtual void ApplyGizmoScale(const FVector & newScale, float moveDist) override; 

private:
	float Radius = 1.0f;
	float DrawStartRadius = 1.0f;
};