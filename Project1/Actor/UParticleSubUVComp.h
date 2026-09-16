#pragma once 

#include "UBillboard.h"

struct ParticleSubUVDesc
{
	int32 ColumnCnt = 1;
	int32 RowCnt = 1;
	int32 FirstIndex = 0;
	int32 LastIndex = 1;

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
	FVector2 GetSubUVScale();
	FVector2 GetSubUVOffset();

private:
	void Initialize();
	void UpdateUVCoordinate();

	ParticleSubUVDesc Desc;
	int32 TotalFrame = 1;
	int32 CurrentFrameIndex = 0;
	int32 ColumnIndex = 0;
	int32 RowIndex = 0;
	float CellSizeU = 1.f;
	float CellSizeV = 1.f;

	float ElapsedTime = 0.f; // second
};