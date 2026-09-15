#pragma once

#include "FMatrix.h"
#include "FLinearColor.h"
#include "Containers.h"
#include "GraphicsEnums.h"
#include "ConstantBuffer.h"
#include "RenderMesh.h"
#include "Material.h"
#include "GraphicsPipeline.h"
#include "RenderSystem.h"
#include "ShaderConstants.h"

struct FMeshRenderData
{
	const RenderMesh& Mesh;
	const Material* Material = nullptr;
	FMatrix World;
	FLinearColor Color;
	bool bSelected = false;
	bool bWireFrame = false;
};

class FMeshRenderer final
{
public:
	void Render(const ConstantBuffer& FrameBuffer, const TArray<FMeshRenderData>& RenderData, bool bDepthEnable = true);
	
	FMeshRenderer() :
		ObjectBuffer(DEVICE.CreateConstantBuffer(sizeof(ObjectConstants))),
		ColorBuffer(DEVICE.CreateConstantBuffer(sizeof(ColorConstants))) {}

private:
	// ShaderW0.hlsl
	ConstantBuffer ObjectBuffer;
	ConstantBuffer ColorBuffer;
};

inline void FMeshRenderer::Render(const ConstantBuffer& FrameBuffer, const TArray<FMeshRenderData>& RenderData, bool bDepthEnable)
{
	CONTEXT.SetConstantBuffer(0, ObjectBuffer);
	CONTEXT.SetConstantBuffer(1, FrameBuffer);
	CONTEXT.SetConstantBuffer(2, ColorBuffer);

	for (const auto& [Mesh, Material, World, Color, bSelected, bWireFrame] : RenderData)
	{
		GraphicsPipelineDesc PipelineDesc{
			.VertexShader = Material ? Material->VertexShader : VertexShaderType::Mesh,
			.PixelShader = Material ? Material->PixelShader : PixelShaderType::Mesh,
			.VertexLayout = Material ? Material->Layout : VertexLayout::PositionColorUVNormal,
			.Rasterizer = {
				.FillMode = bWireFrame ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID,
				.CullMode = D3D11_CULL_BACK,
			},
			.Blend = Material ? Material->Blend : BlendMode::None,
			.Depth = bDepthEnable ? DepthDesc{} : DepthDesc{ .bEnable = false },
			.Stencil = bSelected ? StencilDesc{
				.bEnable = true,
				.FrontFace = {
					.DepthFailOp = D3D11_STENCIL_OP_REPLACE,
					.PassOp = D3D11_STENCIL_OP_REPLACE,
				},
				.BackFace = {
					.DepthFailOp = D3D11_STENCIL_OP_REPLACE,
					.PassOp = D3D11_STENCIL_OP_REPLACE,
				},
			} : StencilMode::Disabled,
		};
		auto& Pipeline = RENDER.GetOrCreatePipeline(PipelineDesc);
		CONTEXT.SetPipeline(Pipeline, bSelected ? 1u : 0u);

		CONTEXT.UpdateConstantBuffer(ObjectBuffer, ObjectConstants{
			.World = World.Transpose(),
		});
		CONTEXT.UpdateConstantBuffer(ColorBuffer, ColorConstants{
			.CustomColor = Color,
			.UseTexture = false,
		});

		CONTEXT.SetMesh(Mesh);
		if (Material)
		{
			CONTEXT.SetTexture(0, *RESOURCES.GetTexture(Material->Texture));
			CONTEXT.SetSampler(0, RESOURCES.GetSamplerState(Material->Sampler));
		}

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
