#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <d3d11.h>
#include <optional>

struct RenderMesh final
{
	const VertexBuffer& GetVertexBuffer() const { return VertexBuffer; }
	const IndexBuffer* GetIndexBuffer() const { return IndexBuffer ? &*IndexBuffer : nullptr; }
	D3D11_PRIMITIVE_TOPOLOGY GetTopology() const { return Topology; }

	bool IsIndexed() const { return IndexBuffer.has_value(); }

private:
	friend class RenderResources;
	RenderMesh(
		VertexBuffer InVertexBuffer,
		D3D11_PRIMITIVE_TOPOLOGY InTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	) :
		VertexBuffer(std::move(InVertexBuffer)),
		Topology(InTopology)
	{}

	RenderMesh(
		VertexBuffer InVertexBuffer,
		IndexBuffer InIndexBuffer,
		D3D11_PRIMITIVE_TOPOLOGY InTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	) :
		VertexBuffer(std::move(InVertexBuffer)),
		IndexBuffer(std::move(InIndexBuffer)),
		Topology(InTopology)
	{}

	VertexBuffer VertexBuffer;
	std::optional<IndexBuffer> IndexBuffer;

	D3D11_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};
