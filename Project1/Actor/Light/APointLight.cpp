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
	FVector Forward = transform.Forward();
	FVector Up = transform.Up();
	FVector Right = transform.Right();

	DrawCircle(Center, Forward, Up, Attenuation);
	DrawCircle(Center, Forward, Right, Attenuation);
	DrawCircle(Center, Up, Right, Attenuation);
}
