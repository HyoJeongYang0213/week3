#include "pch.h"
#include "ALight.h"
#include <Global.h>
#include "ASpotLight.h"
#include "ULineBatch.h"

ASpotLight::ASpotLight(const FLinearColor& color) : ALight(color)
{
}

void ASpotLight::Render()
{
	Super::Render();

	FVector Apex = GetLocation();
	FVector Forward = GetTransform().Forward();
	FVector Up = GetTransform().Up();
	FVector Right = GetTransform().Right();

	float AngleRad = Angle * Global::PI / 180.0f;

	FVector BaseCenter = Apex + Forward * Length;	// 밑면 센터
	float Radius = Length * tanf(AngleRad);
	
	DrawCircle(BaseCenter, Right, Up, Radius);

	int Segments = 16;
	for (int i = 0; i < Segments; i++) {
		float phi = 2 * Global::PI * i / Segments;
		FVector Q = BaseCenter + Right * (Radius * cosf(phi)) + Up * (Radius * sinf(phi));
		LINEBATCH.AddLine(Apex, Q, Color, true);
	}
}

void ASpotLight::ApplyGizmoScale(const FVector& newScale, float moveDist)
{
	if (DrawStartRadius + moveDist > 0.05f)
		Length = DrawStartRadius + moveDist;
	else Length = 0.05f;
}
