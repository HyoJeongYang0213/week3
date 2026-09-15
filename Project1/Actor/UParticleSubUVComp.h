#pragma once 

#include "UBillboard.h"

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
	UParticleSubUVComp(); 
	UParticleSubUVComp(const FString& TextureName, ParticleSubUVDesc InDesc);
	virtual void Update(float DeltaTime) override;
	const FVector2D& GetSubUVScale() const;
	const FVector2D& GetSubUVOffset() const;

private:
	void Initialize();
	void UpdateUVCoordinate();

	ParticleSubUVDesc Desc;
	uint32 TotalFrame = 1;
	uint32 CurrentFrameIndex = 0;
	uint32 ColumnIndex = 0;
	uint32 RowIndex = 0;
	float CellSizeU = 1.f;
	float CellSizeV = 1.f;

	float ElapsedTime = 0.f; // second
};