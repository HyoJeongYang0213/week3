#pragma once 

#include "UBillboard.h"
#include "GlobalBuffer.h"

struct ParticleSubUVDesc
{
	uint32 ColumnCnt = 1;
	uint32 RowCnt = 1;
	uint32 FirstIndex = 0;
	uint32 LastIndex = 1;

	bool bIsLoop = true;
	bool bPlay = true;
	float PlayRate = 1.f; // ratio
	float Duration = 1.f; // second
};

class UParticleSubUVComp : public UBillboard
{
	DECLARE_CLASS(UParticleSubUVComp, UBillboard);       
public:
	UParticleSubUVComp(){
		Initialize();
		Primitive = EPrimitive::SubUV;
	}
	UParticleSubUVComp(const wstring& TexturePath, ParticleSubUVDesc InDesc)
		: UBillboard(TexturePath), Desc(InDesc){
		Initialize();
		Primitive = EPrimitive::SubUV;
	}
	virtual void Update(float DeltaTime) override;
	virtual void Render() override;

private:
	void Initialize();
	void UpdateUVCoordinate();

	ID3D11ShaderResourceView* TextureSRV; // TODO: 객체마다 각각의 SRV 포인팅하기

	ParticleSubUVDesc Desc;
	uint32 TotalFrame = 1;
	uint32 CurrentFrameIndex = 0;
	uint32 ColumnIndex = 0;
	uint32 RowIndex = 0;
	float CellSizeU = 1.f;
	float CellSizeV = 1.f;

	float ElapsedTime = 0.f; // second
};