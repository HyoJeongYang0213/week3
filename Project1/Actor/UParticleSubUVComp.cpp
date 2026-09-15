#include "pch.h"
#include "UParticleSubUVComp.h"

void UParticleSubUVComp::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	if (!Desc.bPlay || TotalFrame <= 0 || Desc.Duration <= 0.f)
	{
		return;
	}

	ElapsedTime += DeltaTime * Desc.PlayRate;

	if (ElapsedTime >= Desc.Duration && Desc.bIsLoop)
	{
		ElapsedTime -= Desc.Duration;  // loop animation
	}
	if (ElapsedTime >= Desc.Duration) // play only once
	{
		Desc.bPlay = false;
		this->Destroy();
		return;
	}

	CurrentFrameIndex = Desc.FirstIndex + (ElapsedTime / Desc.Duration) * TotalFrame;
	UpdateUVCoordinate();
}

void UParticleSubUVComp::Render()
{
	SetWorldBuffer();
	mesh->IASet(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mesh->indexbuffer->IASet();
	RENDERER.PrepareSubUVShader();
	RENDERER.SetTexture(mesh->GetTexture()); // TODO: 각각의 텍스쳐를 로드하기
	if (RENDERER.SubUVConstantBuffer)
	{
		RENDERER.SubUVConstantBuffer->SetUV(CellSizeU, CellSizeV, CellSizeU * ColumnIndex, CellSizeV * RowIndex);
		RENDERER.SubUVConstantBuffer->SetVSBuffer(3);
	}
	DC->OMSetBlendState(RENDERER.AlphaBlendState, nullptr, 0xffffffff);
	DC->DrawIndexed(mesh->indexbuffer->count, 0u, 0);
	DC->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

void UParticleSubUVComp::Initialize()
{
	TotalFrame = Desc.LastIndex - Desc.FirstIndex;
	CurrentFrameIndex = Desc.FirstIndex;
	CellSizeU = 1.f / Desc.ColumnCnt;
	CellSizeV = 1.f / Desc.RowCnt;
}

void UParticleSubUVComp::UpdateUVCoordinate()
{
	assert(Desc.FirstIndex < Desc.LastIndex);
	const uint32 IndexLength = Desc.LastIndex - Desc.FirstIndex;
	CurrentFrameIndex = Desc.FirstIndex + (CurrentFrameIndex - Desc.LastIndex) % IndexLength;
	ColumnIndex = CurrentFrameIndex % Desc.ColumnCnt;
	RowIndex = CurrentFrameIndex / Desc.ColumnCnt;
}