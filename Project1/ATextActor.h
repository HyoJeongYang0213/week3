#pragma once

#include "AActor.h"
#include "FontMeshGenerator.h"

class ATextActor : public AActor
{
	DECLARE_CLASS(ATextActor, AActor);

public:
	inline static bool bShowUUID = true;
	ATextActor() = default;
	virtual ~ATextActor() = default;

	virtual void Update(float deltaTime) override
	{
		if (!OBJECT.IsValidObject(TargetActor, TargetUUID))
		{
			TargetActor = nullptr;
			DeActive();
			return;
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
		Text = text;

		Mesh* newMesh = FontMeshGenerator::Generate(Text, 0.8);
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
		}
		else
		{
			TargetActor = nullptr;
			DeActive();
			OBJECT.ReserveDestroy(this);
			return;
		}
	}

private:
	std::wstring Text;
	AActor* TargetActor = nullptr;
	uint32 TargetUUID = 0;
};