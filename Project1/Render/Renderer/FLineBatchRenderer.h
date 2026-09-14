#pragma once

#include "FVector.h"
#include "FLinearColor.h"
#include "FVertexSimple.h"
#include "VertexBuffer.h"
#include "ShaderConstants.h"

class FLineBatchRenderer {
public:
	static FLineBatchRenderer& GetInstance() {
		static FLineBatchRenderer Instance;
		return Instance;
	}

	void AddLine(const FVector & Start, const FVector& End, const FLinearColor& Color, bool bOverlay = false);
	void Render(const ConstantBuffer& FrameBuffer, bool bDepthEnable = true);

private:
	FLineBatchRenderer() :
		VertexBuffer(DEVICE.CreateVertexBuffer(nullptr, sizeof(FVertexData), 10000)),
		ObjectBuffer(DEVICE.CreateConstantBuffer(sizeof(ObjectConstants))) {};

	VertexBuffer VertexBuffer;
	ConstantBuffer ObjectBuffer;

	TArray<FVertexData> Vertices;
	UINT MaxVertexCount = 10000;

};