#include "pch.h"
#include "FLineBatchRenderer.h"
#include "VertexBuffer.h"

void FLineBatchRenderer::AddLine(const FVector& Start, const FVector& End, const FLinearColor& Color, bool bOverlay)
{
	// 들어온 Vertices의 시작점, 끝점의 인덱스 넣기
	Indices.Add(static_cast<uint32>(Vertices.size()));
	Indices.Add(static_cast<uint32>(Vertices.size() + 1));

	FVertexData v0;
	v0.x = Start.X; v0.y = Start.Y; v0.z = Start.Z;
	v0.r = Color.r; v0.g = Color.g; v0.b = Color.b; v0.a = Color.a;

	FVertexData v1;
	v1.x = End.X; v1.y = End.Y; v1.z = End.Z;
	v1.r = Color.r; v1.g = Color.g; v1.b = Color.b; v1.a = Color.a;

	float overlayFlag = bOverlay ? 1.0f : 0.0f;
	v0.u = overlayFlag;
	v1.u = overlayFlag;

	Vertices.Add(v0);
	Vertices.Add(v1);
}

void FLineBatchRenderer::Render(const ConstantBuffer& FrameBuffer, bool bDepthEnable)
{
	bool bResized = false;
	while (Vertices.size() > MaxVertexCount) {
		MaxVertexCount = Vertices.size() * 2;
		bResized = true;
	}
	if (bResized) {
		VertexBuffer = DEVICE.CreateVertexBuffer(nullptr, sizeof(FVertexData), MaxVertexCount);
	}

	bResized = false;
	while (Indices.size() > MaxIndexCount) {
		MaxIndexCount = Indices.size() * 2;
		bResized = true;
	}
	if (bResized) {
		IndexBuffer = DEVICE.CreateIndexBuffer(nullptr, MaxIndexCount, true);
	}


	GraphicsPipelineDesc PipelineDesc{
		.VertexShader = VertexShaderType::Line,
		.PixelShader = PixelShaderType::Line,
		.VertexLayout = VertexLayout::PositionColorUVNormal,
		.Rasterizer = {
			.FillMode = D3D11_FILL_SOLID,
			.CullMode = D3D11_CULL_NONE,
		},
		.Blend = BlendMode::AlphaBlend,
		.Depth = bDepthEnable ? DepthDesc{} : DepthDesc{.bEnable = false },
	};
	auto& Pipeline = RENDER.GetOrCreatePipeline(PipelineDesc);
	CONTEXT.SetPipeline(Pipeline);
	CONTEXT.SetConstantBuffer(0, ObjectBuffer);
	CONTEXT.SetConstantBuffer(1, FrameBuffer);

	CONTEXT.UpdateVertexBuffer(VertexBuffer, Vertices.data(), sizeof(FVertexData) * Vertices.size());
	CONTEXT.UpdateIndexBuffer(IndexBuffer, Indices.data(), IndexBuffer::Size * Indices.size());

	FMatrix World = FMatrix::Identity();
	CONTEXT.UpdateConstantBuffer(ObjectBuffer, &World, sizeof(FMatrix));

	CONTEXT.SetVertexBuffer(VertexBuffer);
	CONTEXT.SetIndexBuffer(IndexBuffer);
	CONTEXT.SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	
	CONTEXT.DrawIndexed(Indices.size());
	Vertices.clear();
	Indices.clear();
}

void FLineBatchRenderer::AddBox(const FVector Corners[8], const FLinearColor& Color, bool bOverlay)
{
	uint32 BaseSize = static_cast<uint32>(Vertices.size());

	// 점 8개부터 넣기
	for (int i = 0; i < 8; i++) {
		FVertexData v;
		v.x = Corners[i].X; v.y = Corners[i].Y; v.z = Corners[i].Z;
		v.r = Color.r; v.g = Color.g; v.b = Color.b; v.a = Color.a;
		v.u = (bOverlay ? 1.0f : 0.0f);
		Vertices.Add(v);
	}

	// 인덱스 추가
	static const uint32 BoxEdges[24] = {
		0, 1, 2, 3, 4, 5, 6, 7 ,
		0, 2 ,1, 3 ,4, 6 ,5, 7 ,
		0, 4, 1, 5 ,2, 6 ,3, 7 
	};

	// 시작위치 + 박스 인덱스
	for (int i = 0; i < 24; i++) {
		Indices.Add(BaseSize + BoxEdges[i]);
	}
}
