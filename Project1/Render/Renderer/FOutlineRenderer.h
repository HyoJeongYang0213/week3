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
	void Render(const ConstantBuffer& FrameBuffer, const FOutlineRenderData& Data);

	FOutlineRenderer() :
		ObjectBuffer(DEVICE.CreateConstantBuffer(sizeof(ObjectConstants))),
		ColorBuffer(DEVICE.CreateConstantBuffer(sizeof(ColorConstants))) {}

private:
	// ShaderW0.hlsl
	ConstantBuffer ObjectBuffer;
	ConstantBuffer ColorBuffer;
};

inline void FOutlineRenderer::Render(const ConstantBuffer& FrameBuffer, const FOutlineRenderData& Data)
{
	GraphicsPipelineDesc PipelineDesc{
		.VertexShader = VertexShaderType::Outline,
		.PixelShader = PixelShaderType::Mesh,
		.VertexLayout = VertexLayout::PositionColorUVNormal,
		.Rasterizer = {
			.FillMode = D3D11_FILL_SOLID,
			.CullMode = D3D11_CULL_BACK,
			.bFrontCounterClockWise = false
		},
		.Blend = BlendMode::None,
		.Depth = {
			.bEnable = true,
			.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
			.DepthFunc = D3D11_COMPARISON_ALWAYS,
		},
		.Stencil = StencilMode::TestNotEqual,
	};
	auto& Pipeline = RENDER.GetOrCreatePipeline(PipelineDesc);
	CONTEXT.SetPipeline(Pipeline, 1u);
	CONTEXT.SetConstantBuffer(0, ObjectBuffer);
	CONTEXT.SetConstantBuffer(1, FrameBuffer);
	CONTEXT.SetConstantBuffer(2, ColorBuffer);

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
