#pragma once

#include "RenderMesh.h"
#include "FMatrix.h"
#include "FLinearColor.h"
#include "ConstantBuffer.h"
#include "Containers.h"
#include "ShaderConstants.h"

struct FGizmoRenderData
{
	const RenderMesh& Mesh;
	FMatrix World;
	FLinearColor Color;
};

class FGizmoRenderer final
{
public:
	FGizmoRenderer() : 
		ObjectBuffer(DEVICE.CreateConstantBuffer(sizeof(ObjectConstants))),
		ColorBuffer(DEVICE.CreateConstantBuffer(sizeof(ColorConstants))) {}

	void Render(const ConstantBuffer& FrameBuffer, const TArray<FGizmoRenderData>& Data);

private:
	ConstantBuffer ObjectBuffer;
	ConstantBuffer ColorBuffer;
};

inline void FGizmoRenderer::Render(const ConstantBuffer& FrameBuffer, const TArray<FGizmoRenderData>& Data)
{
	GraphicsPipelineDesc PipelineDesc{
		.VertexShader = VertexShaderType::Mesh,
		.PixelShader = PixelShaderType::Mesh,
		.VertexLayout = VertexLayout::PositionColorUVNormal,
		.Rasterizer = {
			.FillMode = D3D11_FILL_SOLID,
			.CullMode = D3D11_CULL_BACK,
		},
		.Blend = BlendMode::None,
		.Depth = {
			.bEnable = true,
			.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
			.DepthFunc = D3D11_COMPARISON_LESS_EQUAL,
		},
	};
	auto& Pipeline = RENDER.GetOrCreatePipeline(PipelineDesc);
	CONTEXT.SetPipeline(Pipeline);

	CONTEXT.SetConstantBuffer(0, ObjectBuffer);
	CONTEXT.SetConstantBuffer(1, FrameBuffer);
	CONTEXT.SetConstantBuffer(2, ColorBuffer);

	for (auto& [Mesh, World, Color] : Data)
	{
		CONTEXT.UpdateConstantBuffer(ObjectBuffer, ObjectConstants{ .World = World.Transpose() });
		CONTEXT.UpdateConstantBuffer(ColorBuffer, ColorConstants{ .CustomColor = Color });

		CONTEXT.SetMesh(Mesh);
		if (Mesh.IsIndexed())
		{
			CONTEXT.DrawIndexed(Mesh.GetIndexBuffer()->GetCount());
		}
		else
		{
			CONTEXT.Draw(Mesh.GetVertexBuffer().GetCount());
		}
	}
}
