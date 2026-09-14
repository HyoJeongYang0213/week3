#pragma once 

#include "UBillboard.h"
#include "GlobalBuffer.h"

class UParticleSubUVComp : public UBillboard
{
	DECLARE_CLASS(UParticleSubUVComp, UBillboard);
public:
	UParticleSubUVComp()
	{
		CellSizeU = 1.f / ColumnCnt;
		CellSizeV = 1.f / RowCnt;
		Initialize();
	}
	UParticleSubUVComp(uint32 ColumnCnt, uint32 RowCnt, uint32 TotalFrame, uint32 CurrentFrameIndex)
		:ColumnCnt(ColumnCnt), RowCnt(RowCnt), TotalFrame(TotalFrame), CurrentFrameIndex(CurrentFrameIndex)
	{
		CellSizeU = 1.f / ColumnCnt;
		CellSizeV = 1.f / RowCnt;
		Initialize();
	}

	void UpdateUVCoordinate()
	{
		if (CurrentFrameIndex >= TotalFrame)
		{
			CurrentFrameIndex -= TotalFrame;
		}
		ColumnIndex = CurrentFrameIndex % ColumnCnt;
		RowIndex = CurrentFrameIndex / ColumnCnt;
	}

	void Initialize()
	{
		SubUVConstantBuffer = RENDERER.SubUVConstantBuffer;
	}

	virtual void Render() override
	{
		SetWorldBuffer();
		mesh->IASet(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mesh->indexbuffer->IASet();
		RENDERER.PrepareSubUVShader();
		RENDERER.SetTexture(mesh->GetTexture());
		if (SubUVConstantBuffer)
		{
			SubUVConstantBuffer->SetUV(CellSizeU, CellSizeV, CellSizeU * 2, CellSizeV * 1);
			//SubUVConstantBuffer->SetUV(1, 1, 0, 0);
			SubUVConstantBuffer->SetVSBuffer(3);
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

	float CellSizeU = 0.f;
	float CellSizeV = 0.f;

	SubUVBuffer* SubUVConstantBuffer = nullptr;
};