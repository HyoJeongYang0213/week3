#pragma once 

#include "UBillboard.h"
#include "GlobalBuffer.h"

class UParticleSubUVComp : public UBillboard
{
	DECLARE_CLASS(UParticleSubUVComp, UBillboard);
public:
	UParticleSubUVComp() = default;
	UParticleSubUVComp(uint32 InColumnCnt, uint32 InRowCnt, uint32 InTotalFrame, uint32 InCurrentFrameIndex)
		:ColumnCnt(InColumnCnt), RowCnt(InRowCnt), TotalFrame(InTotalFrame), CurrentFrameIndex(InCurrentFrameIndex), CellSizeU(1.f / InColumnCnt), CellSizeV(1.f / InRowCnt), bIsLoop(true), bIsPlaying(true), PlayRate(1.f), Duration(5.f), ElapsedTime(0.f)
	{}

	void UpdateUVCoordinate()
	{
		while (CurrentFrameIndex >= TotalFrame)
		{
			CurrentFrameIndex -= TotalFrame;
		}
		ColumnIndex = CurrentFrameIndex % ColumnCnt;
		RowIndex = CurrentFrameIndex / ColumnCnt;
	}

	void Initialize()
	{
		CellSizeU = 1.f / ColumnCnt;
		CellSizeV = 1.f / ColumnCnt;
	}

	virtual void Update(float DeltaTime) override
	{
		Super::Update(DeltaTime);
		if (!bIsPlaying)
		{
			return;
		}
		ElapsedTime += DeltaTime * PlayRate;
		while (ElapsedTime >= Duration)
		{
			ElapsedTime -= Duration;
		}
		CurrentFrameIndex = (ElapsedTime / Duration) * TotalFrame;
		UpdateUVCoordinate();
	}

	virtual void Render() override
	{
		SetWorldBuffer();
		mesh->IASet(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mesh->indexbuffer->IASet();
		RENDERER.PrepareSubUVShader();
		RENDERER.SetTexture(mesh->GetTexture());
		if (RENDERER.SubUVConstantBuffer)
		{
			RENDERER.SubUVConstantBuffer->SetUV(CellSizeU, CellSizeV, CellSizeU * ColumnIndex, CellSizeV * RowIndex);
			RENDERER.SubUVConstantBuffer->SetVSBuffer(3);
		}
		DC->OMSetBlendState(RENDERER.AlphaBlendState, nullptr, 0xffffffff);
		DC->DrawIndexed(mesh->indexbuffer->count, 0u, 0);
		DC->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

private:
	uint32 ColumnCnt = 6;
	uint32 RowCnt = 6;
	uint32 TotalFrame = 36;
	uint32 CurrentFrameIndex = 0;
	uint32 ColumnIndex = 0;
	uint32 RowIndex = 0;

	float CellSizeU = 1.f / ColumnCnt;
	float CellSizeV = 1.f / RowCnt;

	bool bIsLoop = true;
	bool bIsPlaying = true;
	float PlayRate = 1.f; // ratio
	float Duration = 3.f; // s
	float ElapsedTime = 0.f; // s
};