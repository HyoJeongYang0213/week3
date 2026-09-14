#include "pch.h"
#include "FLineBatchRenderer.h"

void FLineBatchRenderer::AddLine(const FVector& Start, const FVector& End, const FLinearColor& Color, bool bOverlay)
{
	if (Vertices.size() + 2 > MaxVertexCount) return;

	FVertexData v0;
	v0.x = Start.x; v0.y = Start.y; v0.z = Start.z;
	v0.r = Color.r; v0.g = Color.g; v0.b = Color.b; v0.a = Color.a;

	FVertexData v1;
	v1.x = End.x; v1.y = End.y; v1.z = End.z;
	v1.r = Color.r; v1.g = Color.g; v1.b = Color.b; v1.a = Color.a;

	float overlayFlag = bOverlay ? 1.0f : 0.0f;
	v0.u = overlayFlag;
	v1.u = overlayFlag;

	Vertices.Add(v0);
	Vertices.Add(v1);
}

void FLineBatchRenderer::Render(const ConstantBuffer& FrameBuffer)
{
	bool bResized = false;
	while (Vertices.size() > MaxVertexCount) {
		MaxVertexCount = Vertices.size() * 2;
		bResized = true;
	}
	if (bResized) {
		VertexBuffer = DEVICEN.CreateVertexBuffer(nullptr, sizeof(FVertexData), MaxVertexCount);
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
		.Depth = {
			.bEnable = true,
			.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
			.DepthFunc = D3D11_COMPARISON_LESS_EQUAL
		}
	};
	auto& Pipeline = RENDER.GetOrCreatePipeline(PipelineDesc);
	CONTEXT.SetPipeline(Pipeline);
	CONTEXT.SetConstantBuffer(0, ObjectBuffer);
	CONTEXT.SetConstantBuffer(1, FrameBuffer);

	CONTEXT.UpdateVertexBuffer(VertexBuffer, Vertices.data(), sizeof(FVertexData) * Vertices.size());

	FMatrix World = FMatrix::Identity();
	CONTEXT.UpdateConstantBuffer(ObjectBuffer, &World, sizeof(FMatrix));

	CONTEXT.SetVertexBuffer(VertexBuffer);
	CONTEXT.SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	
	CONTEXT.Draw(Vertices.size());
	Vertices.clear();
}
