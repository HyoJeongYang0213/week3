#pragma once

#include "AActor.h"
#include "Sphere.h"

class UBillboard : public AActor // UPrimitiveComponent
{
	DECLARE_CLASS(UBillboard, AActor)
public:
	UBillboard()
	{
		Mesh* BillboardMesh = OBJECT.GetOrCreateMesh("Billboard", quad_vertices);
		if (BillboardMesh->indexbuffer == nullptr)
		{
			BillboardMesh->InitIndexBuffer(quad_indices, sizeof(quad_indices) / sizeof(uint32));
		}
		if (BillboardMesh->GetTexture() == nullptr)
		{
			BillboardMesh->SetTexture(L"Resources/Textures/Explosion.PNG");
		}
		SetMesh(BillboardMesh);
	}

	virtual void Update(float DeltaTime) override
	{
		SetRotation(CAMERA.GetRotation());
		AActor::Update(DeltaTime);
	}

	virtual void Render() override
	{
		DC->OMSetBlendState(RENDERER.AlphaBlendState, nullptr, 0xffffffff);
		AActor::Render();
		DC->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}


private:
	
};