#include "pch.h"
#include "ALight.h"
#include "ULineBatch.h"
#include "Bulb.h"
#include <Global.h>

ALight::ALight(const FLinearColor& color) : AActor(color)
{
	SetMesh(OBJECT.GetOrCreateMesh("LightBulb", bulbVertices));
	SetPrimitive(EPrimitive::None);
}

void ALight::DrawCircle(const FVector & Center, const FVector &AxisA, const FVector &AxisB, const float Radius)
{
	int Segments = 32;

	for (int i = 1; i <= Segments; i++) {
		float phi0 = 2.0f * Global::PI * (i - 1) / Segments;
		float phi1 = 2.0f * Global::PI * i / Segments;
		
		FVector p0 = Center + AxisA * (Radius * cosf(phi0)) + AxisB * (Radius * sinf(phi0));
		FVector p1 = Center + AxisA * (Radius * cosf(phi1)) + AxisB * (Radius * sinf(phi1));

		LINEBATCH.AddLine(p0, p1, Color, true);
	}
	
}

void ALight::Render()
{
	FVector originScale = transform.GetScale();
	SetScale({0.2f, 0.2f, 0.2f});
	Super::Render();
	SetScale(originScale);
}
