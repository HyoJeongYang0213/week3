#pragma once
#include "AActor.h"
#include "Material.h"
#include "Camera.h"
#include "RenderSystem.h"

class ASkySphere : public AActor
{
	DECLARE_CLASS(ASkySphere, AActor);

public:
	ASkySphere()
	{
		// TODO: Scene 렌더링 경로에서 스카이 메시와 재질 연결.

		// 카메라를 둘러싸는 기본 크기 설정
		SetScale(FVector(500.0f, 500.0f, 500.0f));
		SetPrimitive(EPrimitive::None);
	}

	virtual ~ASkySphere() override = default;

	virtual void Update(float deltaTime) override
	{
		AActor::Update(deltaTime);
	}

	// 하늘 객체는 마우스 피킹 대상에서 제외
	virtual bool bIsPicked(const FRay& ray, float& outDistance) override
	{
		return false;
	}

	virtual bool IsEditorOnly() const override { return true; }

};
