#pragma once
#include "ALight.h"

class APointLight : public ALight{
	DECLARE_CLASS(APointLight, ALight)

public:
	APointLight(const FLinearColor& color = FLinearColor::White);

	float GetRadius() { return Radius; }
	void SetRadius(float angle) { Radius = angle; }

	virtual void Render() override;

private:
	float Radius = 1.0f;
};