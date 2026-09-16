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
				TargetUUID = 0;
				DeActive();
				OBJECT.ReserveDestroy(this);
				return;
			}
			UpdateLabelTransform();
		}

		// 일반 TextActor든 UUID든 공통:
		SetRotation(CAMERA.GetRotation());
		AActor::Update(deltaTime);
	}

	// 텍스트마다 메쉬 개별 생성
	void SetText(const FWString& text)
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

	const FWString& GetText() const { return Text; }

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
		return FString("Text/") + std::to_string(GetID());
	}

	const FontGeometry& GetGeometry() const { return Geometry; }

private:
	FWString Text;

	float GlyphAdvance = 0.6f;
	float LabelPadding = 0.1f;

	AActor* TargetActor = nullptr;
	uint32 TargetUUID = 0;

	FontGeometry Geometry;

	void UpdateLabelTransform()
	{
		if (!TargetActor) return;

		// Target Actor의 BoundingBox로 라벨 위치 구하기
		FBoundingBox box = TargetActor->GetWorldBoundingBox();

		FVector labelPos(
			(box.minX + box.maxX) * 0.5f,
			box.maxY + LabelPadding,
			(box.minZ + box.maxZ) * 0.5f
		);

		SetLocation(labelPos);
	}
};