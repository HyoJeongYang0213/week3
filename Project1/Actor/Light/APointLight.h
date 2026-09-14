#pragma once
#include "ALight.h"

class APointLight : public ALight{
	DECLARE_CLASS(APointLight, ALight)

public:
	APointLight(const FLinearColor& color = FLinearColor::White);

	virtual void Render() override;

private:
	float Attenuation = 3.0f;
};