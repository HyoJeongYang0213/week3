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

	DrawCircle(Center, Forward, Up, Radius);
	DrawCircle(Center, Forward, Right, Radius);
	DrawCircle(Center, Up, Right, Radius);
}

void APointLight::ApplyGizmoScale(const FVector& newScale, float moveDist)
{
	if(DrawStartRadius + moveDist > 0.05f)
		Radius = DrawStartRadius + moveDist;
	else Radius = 0.05f;
}
