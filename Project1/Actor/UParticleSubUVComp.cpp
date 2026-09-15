#include "pch.h"
#include "UParticleSubUVComp.h"

UParticleSubUVComp::UParticleSubUVComp()
{
	Initialize();
	Primitive = EPrimitive::SubUV;
}

UParticleSubUVComp::UParticleSubUVComp(const FString& TextureName, ParticleSubUVDesc InDesc)
	: UBillboard(TextureName), Desc(InDesc)
{
	Initialize();
	Primitive = EPrimitive::SubUV;
}

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

FVector2D UParticleSubUVComp::GetSubUVScale()
{
	return FVector2D(CellSizeU, CellSizeV);
}

FVector2D UParticleSubUVComp::GetSubUVOffset()
{
	return FVector2D(CellSizeU * ColumnIndex, CellSizeV * RowIndex);
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
	CurrentFrameIndex = Desc.FirstIndex + (CurrentFrameIndex - Desc.FirstIndex) % IndexLength;
	ColumnIndex = CurrentFrameIndex % Desc.ColumnCnt;
	RowIndex = CurrentFrameIndex / Desc.ColumnCnt;
}