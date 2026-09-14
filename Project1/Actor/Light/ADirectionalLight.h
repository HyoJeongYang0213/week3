#pragma once
#include "ALight.h"

class ADirectionalLight : public ALight{
	DECLARE_CLASS(ADirectionalLight, ALight)

public:
	ADirectionalLight(const FLinearColor& color = FLinearColor::White);

	virtual void Render() override;

private:
	float Length = 3.0f;
};