#pragma once

#include "AActor.h"
#include "FontMeshGenerator.h"

class ATextActor : public AActor
{
	DECLARE_CLASS(ATextActor, AActor);

public:
	inline static bool bShowUUID = true;
	ATextActor() = default;

	ATextActor()
	{
		SetColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
	}

	virtual ~ATextActor() = default;

	virtual void Update(float deltaTime) override
	{
		if (!OBJECT.IsValidObject(TargetActor, TargetUUID))
		{
			TargetActor = nullptr;
			DeActive();
			return;
			UpdateLabelTransform();
		}
		SetLocation(TargetActor->GetLocation() + FVector(0.0f, 1.0f, 0.0f));
		SetRotation(CAMERA.GetRotation());
		AActor::Update(deltaTime);
	}

	virtual void Render() override
	{
		if (!bShowUUID) return;
		AActor::Render();
	}

	// 텍스트마다 메쉬 개별 생성
	void SetText(const std::wstring& text)
	{
		if (Text == text && mesh != nullptr) return;
		Text = text;
		Mesh* newMesh = FontMeshGenerator::Generate(Text, GlyphAdvance);
		SetMesh(newMesh, true);
	}

	const std::wstring& GetText() const { return Text; }

	void SetTarget(AActor* target)
	{
		TargetActor = target;
		TargetUUID = target ? target->GetID() : 0;
		if (TargetActor) 
		{
			Active();
			UpdateLabelTransform();
		}
		else
		{
			TargetActor = nullptr;
			DeActive();
			OBJECT.ReserveDestroy(this);
			return;
		}
	}

	bool bIsPicked(const FRay& Ray, float& OutDistance) override
	{
		if (bIsUUIDLabel) return false;
		return AActor::bIsPicked(Ray, OutDistance);
	}

private:
	std::wstring Text;

	float GlyphAdvance = 0.6f;
	float LabelPadding = 0.1f;

	AActor* TargetActor = nullptr;
	uint32 TargetUUID = 0;

	void UpdateLabelTransform()
	{
		if (!TargetActor) return;

		float targetHalfHeight = TargetActor->GetScale().y * 0.5f;
		float textHalfHeight = GetScale().y * 0.5f;

		float offsetY = targetHalfHeight + textHalfHeight + LabelPadding;

		SetLocation(TargetActor->GetLocation() + FVector(0.0f, offsetY, 0.0f));
	}
};