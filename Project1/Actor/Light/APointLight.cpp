#include "pch.h"
#include "ALight.h"
#include <Global.h>
#include "APointLight.h"

APointLight::APointLight(const FLinearColor& color) : ALight(color)
{
}

void APointLight::Render()
{
	Super::Render();

	FVector Center = GetLocation();
	DrawCircle(Center, FVector(1, 0, 0), FVector(0, 1, 0), Attenuation);
}
