#pragma once
#include "ALight.h"

class ASpotLight : public ALight{
	DECLARE_CLASS(ASpotLight, ALight)

public:
	ASpotLight(const FLinearColor& color = FLinearColor::White);

	float GetAngle() { return Angle; }
	void SetAngle(float angle) { Angle = angle; }

	virtual void Render() override;

private:
	float Length = 3.0f;
	float Angle = 30.0f;
};