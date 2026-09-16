#pragma once

#include "FVector.h"
#include "FLinearColor.h"
#include "FVertexSimple.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderConstants.h"

class FLineBatchRenderer {
public:
	static FLineBatchRenderer& GetInstance() {
		static FLineBatchRenderer Instance;
		return Instance;
	}

	void AddLine(const FVector & Start, const FVector& End, const FLinearColor& Color, bool bOverlay = false);
	void Render(const ConstantBuffer& FrameBuffer, bool bDepthEnable = true);

	void AddBox(const FVector Corners[8], const FLinearColor& Color, bool bOverlay = false);

private:
	FLineBatchRenderer() :
		VertexBuffer(DEVICE.CreateVertexBuffer(nullptr, sizeof(FVertexData), 10000)),
		IndexBuffer(DEVICE.CreateIndexBuffer(nullptr, 10000, true)),
		ObjectBuffer(DEVICE.CreateConstantBuffer(sizeof(ObjectConstants))) {};

	VertexBuffer VertexBuffer;
	IndexBuffer IndexBuffer;
	ConstantBuffer ObjectBuffer;

	TArray<FVertexData> Vertices;
	TArray<uint32> Indices;
	UINT MaxVertexCount = 10000;
	UINT MaxIndexCount = 10000;
};