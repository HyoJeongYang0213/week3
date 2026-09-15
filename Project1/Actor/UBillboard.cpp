#include "pch.h"
#include "UBillboard.h"

UBillboard::UBillboard()
{
	Mesh* BillboardMesh = OBJECT.GetOrCreateMesh("Billboard", quad_vertices);
	if (BillboardMesh->indexbuffer == nullptr){
		BillboardMesh->InitIndexBuffer(quad_indices, sizeof(quad_indices) / sizeof(uint32));
	}
	SetMesh(BillboardMesh);
}

UBillboard::UBillboard(const wstring& TexturePath)
{
	Mesh* BillboardMesh = OBJECT.GetOrCreateMesh("Billboard", quad_vertices);
	if (BillboardMesh->indexbuffer == nullptr){
		BillboardMesh->InitIndexBuffer(quad_indices, sizeof(quad_indices) / sizeof(uint32));
	}
	BillboardMesh->SetTexture(TexturePath);
	SetMesh(BillboardMesh);
}

void UBillboard::Update(float DeltaTime)
{
	SetRotation(CAMERA.GetRotation());
	AActor::Update(DeltaTime);
}

void UBillboard::Render()
{
	DC->OMSetBlendState(RENDERER.AlphaBlendState, nullptr, 0xffffffff);
	AActor::Render();
	DC->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}