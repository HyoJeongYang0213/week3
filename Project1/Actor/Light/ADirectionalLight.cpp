#include "pch.h"
#include "ALight.h"
#include <Global.h>
#include "ADirectionalLight.h"
#include "ULineBatch.h"

ADirectionalLight::ADirectionalLight(const FLinearColor& color) : ALight(color)
{
}

void ADirectionalLight::Render()
{
	Super::Render();

	FVector Apex = GetLocation();
	FVector Forward = transform.Forward(); // = normal
	FVector Up = transform.Up();
	// FVector Right = transform.Right();

	FVector Tip = Apex + Forward * Length;
	FVector HeadBack = Tip - Forward*0.2f;	
	FVector N1 = HeadBack - Up * 0.1f;
	FVector N2 = HeadBack + Up * 0.1f;

	LINEBATCH.AddLine(Apex, Tip, FLinearColor::White, true);
	LINEBATCH.AddLine(N1, N2, FLinearColor::White, true);
	LINEBATCH.AddLine(N1, Tip, FLinearColor::White, true);
	LINEBATCH.AddLine(N2, Tip, FLinearColor::White, true);
}

void ADirectionalLight::ApplyGizmoScale(const FVector& newScale, float moveDist)
{
}
