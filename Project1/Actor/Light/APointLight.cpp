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

	float maxScale = (std::max)({ transform.GetScale().x, transform.GetScale().y, transform.GetScale().z });
	float Scale = Radius * maxScale;

	DrawCircle(Center, Forward, Up, Scale);
	DrawCircle(Center, Forward, Right, Scale);
	DrawCircle(Center, Up, Right, Scale);
}
