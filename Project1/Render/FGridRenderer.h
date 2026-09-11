#pragma once

#include "FMatrix.h"
#include "FVector.h"
#include "ConstantBuffer.h"
#include "UGrid.h"

struct FSceneView;

class FGridRenderer final
{
public:
	FGridRenderer() : // GridShader.hlsl
		ObjectBuffer(DEVICEN.CreateConstantBuffer(sizeof(ObjectConstants))),
		FrameBuffer(DEVICEN.CreateConstantBuffer(sizeof(FrameConstants)))
	{}

	void Render(const FSceneView& View, const FGridRenderData& GridData);

private:
	ConstantBuffer ObjectBuffer;
	ConstantBuffer FrameBuffer;

	struct alignas(16) FrameConstants
	{
		FMatrix VP;
		FVector CameraPos;
		float Padding = 0.0f;
	};

	struct alignas(16) ObjectConstants
	{
		FMatrix World;
	};
};

struct FSceneView final
{
	FMatrix ViewProjection;
	FVector CameraPosition;
	float ViewportWidth;
	float ViewportHeight;
};

inline void FGridRenderer::Render(const FSceneView& View, const FGridRenderData& GridData)
{
	const GraphicsPipelineDesc Desc{
		.VertexShader = VertexShaderType::Grid,
		.PixelShader = PixelShaderType::Grid,
		.VertexLayout = VertexLayout::PositionColorUVNormal,
		.Rasterizer = { D3D11_FILL_SOLID, D3D11_CULL_BACK, false },
	};
	auto& Pipeline = RENDER.GetPipelines().GetOrCreate(Desc);
	CONTEXT.SetPipeline(Pipeline);

	const ObjectConstants ObjectConst{
		.World = GridData.World.Transpose(),
	};
	const FrameConstants FrameConst{
		.VP = View.ViewProjection.Transpose(),
		.CameraPos = View.CameraPosition,
	};

	CONTEXT.UpdateConstantBuffer(ObjectBuffer, &ObjectConst, sizeof(ObjectConstants));
	CONTEXT.SetConstantBuffer(0, ObjectBuffer);

	CONTEXT.UpdateConstantBuffer(FrameBuffer, &FrameConst, sizeof(FrameConstants));
	CONTEXT.SetConstantBuffer(1, FrameBuffer);

	CONTEXT.SetMesh(*GridData.Mesh);
	CONTEXT.Draw(GridData.Mesh->GetVertexBuffer().GetCount());
}
