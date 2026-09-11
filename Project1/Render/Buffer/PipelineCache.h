#pragma once

#include "RenderDevice.h"
#include "RenderResources.h"
#include "GraphicsPipeline.h"

class PipelineCache final
{
public:
	const GraphicsPipeline& GetOrCreate(const GraphicsPipelineDesc& Desc);

private:
	friend class RenderSystem;
	PipelineCache(RenderDevice& Device, RenderResources& Resources)
		: Device(Device), Resources(Resources) {}

	RenderDevice& Device;
	RenderResources& Resources;

	TMap<GraphicsPipelineDesc, TUniquePtr<GraphicsPipeline>, GraphicsPipelineDescHash> Pipelines;
};

inline const GraphicsPipeline& PipelineCache::GetOrCreate(const GraphicsPipelineDesc& Desc)
{
	auto It = Pipelines.find(Desc);
	if (It != Pipelines.end())
	{
		return *It->second;
	}

	Pipelines[Desc] = MakeUnique<GraphicsPipeline>(
		Device.CreateGraphicsPipeline(
			Resources.GetVertexShader(Desc.VertexShader),
			Resources.GetPixelShader(Desc.PixelShader),
			Desc.VertexLayout,
			Desc.Rasterizer,
			Desc.Blend,
			Desc.DepthStencil));

	return *Pipelines[Desc];
}
