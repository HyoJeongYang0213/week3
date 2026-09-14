#pragma once

#include "AActor.h"
#include "FontMeshGenerator.h"

class ATextActor : public AActor
{
	DECLARE_CLASS(ATextActor, AActor);

public:
	bool bIsUUIDLabel = false;

	ATextActor()
	{
		SetColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
		Primitive = EPrimitive::None;
	}

	virtual ~ATextActor() = default;

	virtual void Update(float deltaTime) override
	{
		if (bIsUUIDLabel)
		{
			if (!OBJECT.IsValidObject(TargetActor, TargetUUID))
			{
				TargetActor = nullptr;
				DeActive();
				return;
			}
			UpdateLabelTransform();
		}

		// 일반 TextActor든 UUID든 공통:
		SetRotation(CAMERA.GetRotation());
		AActor::Update(deltaTime);
	}

	// 텍스트마다 메쉬 개별 생성
	void SetText(const std::wstring& text)
	{
		if (Text == text && mesh != nullptr) return;
		
		Text = text;
		Geometry = FontMeshGenerator::Generate(Text, GlyphAdvance);

		TArray<FVector> Vertices;
		Vertices.Reserve(Geometry.Indices.Num());
		for (uint32 Index : Geometry.Indices)
		{
			if (Index < Geometry.Vertices.Num())
			{
				const FVertexData& Vertex = Geometry.Vertices[static_cast<size_t>(Index)];
				Vertices.Add(FVector(Vertex.x, Vertex.y, Vertex.z));
			}
		}

		SetMesh(new Mesh(Vertices), true);
	}

	const std::wstring& GetText() const { return Text; }

	void SetTarget(AActor* target)
	{
		TargetActor = target;
		TargetUUID = target ? target->GetID() : 0;
		if (TargetActor) 
		{
			Active();
			bIsUUIDLabel = true;
			UpdateLabelTransform();
		}
		else
		{
			TargetActor = nullptr;
			DeActive();
			OBJECT.ReserveDestroy(this);
		}
	}

	bool bIsPicked(const FRay& Ray, float& OutDistance) override
	{
		if (bIsUUIDLabel) return false;
		return AActor::bIsPicked(Ray, OutDistance);
	}

	FString GetRenderMeshName() const override
	{
		return "Text/" + GetID();
	}

	const FontGeometry& GetGeometry() const { return Geometry; }

private:
	std::wstring Text;

	float GlyphAdvance = 0.6f;
	float LabelPadding = 0.1f;

	AActor* TargetActor = nullptr;
	uint32 TargetUUID = 0;

	FontGeometry Geometry;

	void UpdateLabelTransform()
	{
		if (!TargetActor) return;

		float targetHalfHeight = TargetActor->GetScale().y * 0.5f;
		float textHalfHeight = GetScale().y * 0.5f;

		float offsetY = targetHalfHeight + textHalfHeight + LabelPadding;

		SetLocation(TargetActor->GetLocation() + FVector(0.0f, offsetY, 0.0f));
	}
};