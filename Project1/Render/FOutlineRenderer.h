#pragma once

#include "FMatrix.h"
#include "FVector.h"
#include "FLinearColor.h"
#include "GraphicsEnums.h"
#include "ConstantBuffer.h"
#include "RenderMesh.h"
#include "GraphicsPipeline.h"
#include "ShaderConstants.h"

struct FOutlineRenderData
{
	const RenderMesh* Mesh = nullptr;
	FMatrix World;
	float PixelWidth;
	float ViewportWidth;
	float ViewportHeight;
};

class FOutlineRenderer final
{
public:
	void Render(const FMatrix& ViewProjection, const FVector& CameraPosition, const FOutlineRenderData& Data);

	FOutlineRenderer() :
		ObjectBuffer(DEVICEN.CreateConstantBuffer(sizeof(ObjectConstants))),
		FrameBuffer(DEVICEN.CreateConstantBuffer(sizeof(FrameConstants))),
		ColorBuffer(DEVICEN.CreateConstantBuffer(sizeof(ColorConstants))) {}

private:
	// ShaderW0.hlsl
	ConstantBuffer ObjectBuffer;
	ConstantBuffer FrameBuffer;
	ConstantBuffer ColorBuffer;
};

inline void FOutlineRenderer::Render(const FMatrix& ViewProjection, const FVector& CameraPosition, const FOutlineRenderData& Data)
{
	GraphicsPipelineDesc PipelineDesc{
		.VertexShader = VertexShaderType::Outline,
		.PixelShader = PixelShaderType::Object,
		.VertexLayout = VertexLayout::PositionColorUVNormal,
		.Rasterizer = {
			.FillMode = D3D11_FILL_SOLID,
			.CullMode = D3D11_CULL_BACK,
			.bFrontCounterClockWise = false
		},
		.Blend = BlendMode::None,
		.Depth = {
			.bEnable = false,
			.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO,
		},
		.Stencil = StencilMode::TestNotEqual,
	};
	auto& Pipeline = RENDER.GetOrCreatePipeline(PipelineDesc);
	CONTEXT.SetPipeline(Pipeline, 1u);
	CONTEXT.SetConstantBuffer(0, ObjectBuffer);
	CONTEXT.SetConstantBuffer(1, FrameBuffer);
	CONTEXT.SetConstantBuffer(2, ColorBuffer);

	CONTEXT.UpdateConstantBuffer(FrameBuffer, FrameConstants{
		.VP = ViewProjection.Transpose(),
		.CameraPos = CameraPosition });

	CONTEXT.UpdateConstantBuffer(ObjectBuffer, ObjectConstants{
		.World = Data.World.Transpose() });
	CONTEXT.UpdateConstantBuffer(ColorBuffer, ColorConstants{ // 셰이더의 Constant명 수정 필요
		.CustomColor = FLinearColor{
			Data.PixelWidth,
			Data.ViewportWidth,
			Data.ViewportHeight,
		},
		.UseTexture = false });

	CONTEXT.SetMesh(*Data.Mesh);
	CONTEXT.Draw(Data.Mesh->GetVertexBuffer().GetCount());
}
