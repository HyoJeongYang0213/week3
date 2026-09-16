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
	const TOptional<Material> Material = Nullopt;
	FMatrix World;
	FLinearColor Color;
	bool bSelected = false;
	bool bWireFrame = false;
	FVector2 UVScale;
	FVector2 UVOffset;
};

class FMeshRenderer final
{
public:
	void Render(const ConstantBuffer& FrameBuffer, const TArray<FMeshRenderData>& RenderData, bool bDepthEnable = true);
	
	FMeshRenderer() :
		ObjectBuffer(DEVICE.CreateConstantBuffer(sizeof(ObjectConstants))),
		ColorBuffer(DEVICE.CreateConstantBuffer(sizeof(ColorConstants))),
		SubUVBuffer(DEVICE.CreateConstantBuffer(sizeof(SubUVConstants))){}

private:
	// ShaderW0.hlsl
	ConstantBuffer ObjectBuffer;
	ConstantBuffer ColorBuffer;
	ConstantBuffer SubUVBuffer;
};

inline void FMeshRenderer::Render(const ConstantBuffer& FrameBuffer, const TArray<FMeshRenderData>& RenderData, bool bDepthEnable)
{
	CONTEXT.SetConstantBuffer(0, ObjectBuffer);
	CONTEXT.SetConstantBuffer(1, FrameBuffer);
	CONTEXT.SetConstantBuffer(2, ColorBuffer);
	CONTEXT.SetConstantBuffer(3, SubUVBuffer);

	for (const auto& [Mesh, Material, World, Color, bSelected, bWireFrame, UVScale, UVOffset] : RenderData)
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
		CONTEXT.UpdateConstantBuffer(SubUVBuffer, SubUVConstants{
			.UVScale = UVScale,
			.UVOffset = UVOffset,
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
