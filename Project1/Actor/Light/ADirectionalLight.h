#pragma once
#include "ALight.h"

class ADirectionalLight : public ALight{
	DECLARE_CLASS(ADirectionalLight, ALight)

public:
	ADirectionalLight(const FLinearColor& color = FLinearColor::White);

	// 게터, 세터 제작은 했으나 
	// DirectionalLight는 스케일 늘린다고 화살표 길이 늘어나면 안됨!!! 
	float GetLength() { return Length; }
	void SetLength(float length) { Length = length; }

	virtual void AddLines() override;

	// 아래는 사용할 일 없다!
	virtual void BeginGizmoScale() override { DrawStartRadius = Length; }
	virtual void ApplyGizmoScale(const FVector& newScale, float moveDist) override;

private:
	float Length = 1.0f;
	float DrawStartRadius = 1.0f;
};