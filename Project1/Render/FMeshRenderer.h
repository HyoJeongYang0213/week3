#pragma once

#include "FMatrix.h"
#include "FVector.h"
#include "FLinearColor.h"
#include "Containers.h"
#include "GraphicsEnums.h"
#include "ConstantBuffer.h"
#include "RenderMesh.h"
#include "Material.h"
#include "GraphicsPipeline.h"
#include "ShaderConstants.h"

struct FMeshRenderData
{
	const RenderMesh* Mesh = nullptr;
	const Material* Material = nullptr;
	FMatrix World;
	FLinearColor Color;
};

class FMeshRenderer final
{
public:
	void Render(const FMatrix& ViewProjection, const FVector& CameraPosition, TArray<FMeshRenderData>& RenderData, bool bIsWireframe = false);
	
	FMeshRenderer() :
		ObjectBuffer(DEVICEN.CreateConstantBuffer(sizeof(ObjectConstants))),
		FrameBuffer(DEVICEN.CreateConstantBuffer(sizeof(FrameConstants))),
		ColorBuffer(DEVICEN.CreateConstantBuffer(sizeof(ColorConstants))) {}

private:
	// ShaderW0.hlsl
	ConstantBuffer ObjectBuffer;
	ConstantBuffer FrameBuffer;
	ConstantBuffer ColorBuffer;
};

inline void FMeshRenderer::Render(const FMatrix& ViewProjection, const FVector& CameraPosition, TArray<FMeshRenderData>& RenderData, bool bIsWireframe)
{
	GraphicsPipelineDesc PipelineDesc{
		.VertexShader = VertexShaderType::Object,
		.PixelShader = PixelShaderType::Object,
		.VertexLayout = VertexLayout::PositionColorUVNormal,
		.Rasterizer = {
			.FillMode = bIsWireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID,
			.CullMode = D3D11_CULL_BACK,
		},
		.Blend = BlendMode::None,
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
	CONTEXT.SetConstantBuffer(2, ColorBuffer);

	CONTEXT.UpdateConstantBuffer(FrameBuffer, FrameConstants{
		.VP = ViewProjection.Transpose(),
		.CameraPos = CameraPosition,
	});

	for (const auto& Data : RenderData)
	{
		CONTEXT.UpdateConstantBuffer(ObjectBuffer, ObjectConstants{
			.World = Data.World.Transpose(),
		});
		CONTEXT.UpdateConstantBuffer(ColorBuffer, ColorConstants{
			.CustomColor = Data.Color,
			.UseTexture = false,
		});

		CONTEXT.SetMesh(*Data.Mesh);
		CONTEXT.Draw(Data.Mesh->GetVertexBuffer().GetCount());

		// TODO: Material 사용
	}
}
