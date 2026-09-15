#include "pch.h"
#include "RenderContext.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "RenderMesh.h"
#include "Texture.h"
#include "GraphicsPipeline.h"

void RenderContext::SetRenderTarget(ID3D11RenderTargetView& RenderTarget, ID3D11DepthStencilView* DepthStencilView)
{
	ID3D11RenderTargetView* RenderTargets[] = { &RenderTarget };
	Context->OMSetRenderTargets(1u, RenderTargets, DepthStencilView);
}

void RenderContext::SetViewport(const D3D11_VIEWPORT& Viewport)
{
	const D3D11_VIEWPORT Viewports[] = { Viewport };
	Context->RSSetViewports(1u, Viewports);
}

void RenderContext::ClearRenderTarget(ID3D11RenderTargetView& RenderTarget, const float* Color)
{
	constexpr float DefaultColor[] = { 1.0f, 0.0f, 1.0f, 1.0f };
	Context->ClearRenderTargetView(&RenderTarget, Color ? Color : DefaultColor);
}

void RenderContext::ClearDepthStencil(ID3D11DepthStencilView& DepthStencilView, float Depth, UINT8 Stencil)
{
	Context->ClearDepthStencilView(&DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, Depth, Stencil);
}

void RenderContext::SetVertexBuffer(const VertexBuffer& InVertexBuffer)
{
	UINT Strides[] = { InVertexBuffer.GetStride() };
	UINT Offsets[] = { InVertexBuffer.GetOffset() };
	ID3D11Buffer* Buffers[] = { &InVertexBuffer.GetNativeBuffer() };
	Context->IASetVertexBuffers(0u, 1u, Buffers, Strides, Offsets);
}

void RenderContext::UpdateVertexBuffer(const VertexBuffer& InVertexBuffer, const void* Data, UINT Size)
{
	if (Size > InVertexBuffer.GetCount() * InVertexBuffer.GetStride())
	{
		assert(false);
		return;
	}

	ID3D11Buffer* NativeBuffer = &InVertexBuffer.GetNativeBuffer();

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	Context->Map(NativeBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &MappedResource);
	memcpy(MappedResource.pData, Data, Size);
	Context->Unmap(NativeBuffer, 0u);
}

void RenderContext::SetIndexBuffer(const IndexBuffer& InIndexBuffer)
{
	Context->IASetIndexBuffer(&InIndexBuffer.GetNativeBuffer(), InIndexBuffer.GetFormat(), InIndexBuffer.GetOffset());
}

void RenderContext::UpdateIndexBuffer(const IndexBuffer& InIndexBuffer, const void* Data, UINT Size)
{
	if (Size > InIndexBuffer.GetCount() * IndexBuffer::Size)
	{
		assert(false);
		return;
	}

	ID3D11Buffer* NativeBuffer = &InIndexBuffer.GetNativeBuffer();

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	Context->Map(NativeBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &MappedResource);
	memcpy(MappedResource.pData, Data, Size);
	Context->Unmap(NativeBuffer, 0u);
}

void RenderContext::SetTopology(D3D11_PRIMITIVE_TOPOLOGY Topology)
{
	Context->IASetPrimitiveTopology(Topology);
}

// TODO: 슬롯별로 캐싱해서 다시 바인드하지 않도록 하면 좋을 듯
void RenderContext::SetConstantBuffer(UINT Slot, const ConstantBuffer& InConstantBuffer)
{
	ID3D11Buffer* Buffers[] = { &InConstantBuffer.GetNativeBuffer() };

	Context->VSSetConstantBuffers(Slot, 1u, Buffers);
	Context->PSSetConstantBuffers(Slot, 1u, Buffers);
}

void RenderContext::UpdateConstantBuffer(ConstantBuffer& InConstantBuffer, const void* Data, UINT Size)
{
	if (Size > InConstantBuffer.GetSize())
	{
		assert(false);
		return;
	}

	ID3D11Buffer* NativeBuffer = &InConstantBuffer.GetNativeBuffer();

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	Context->Map(NativeBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &MappedResource);
	memcpy(MappedResource.pData, Data, Size);
	Context->Unmap(NativeBuffer, 0u);
}

void RenderContext::SetMesh(const RenderMesh& Mesh)
{
	SetVertexBuffer(Mesh.GetVertexBuffer());
	if (Mesh.IsIndexed())
	{
		SetIndexBuffer(*Mesh.GetIndexBuffer());
	}
	Context->IASetPrimitiveTopology(Mesh.GetTopology());
}

void RenderContext::SetTexture(UINT Slot, const Texture& InTexture)
{
	ID3D11ShaderResourceView* SRVs[] = { &InTexture.GetSRV() };
	Context->PSSetShaderResources(Slot, 1u, SRVs);
}

void RenderContext::SetSampler(UINT Slot, ID3D11SamplerState& SamplerState)
{
	ID3D11SamplerState* Samplers[] = { &SamplerState };
	Context->PSSetSamplers(Slot, 1u, Samplers);
}

void RenderContext::SetPipeline(const GraphicsPipeline& Pipeline, UINT8 StencilRef)
{
	if (BoundPipeline == &Pipeline && StencilRef == BoundStencilRef)
	{
		return;
	}
	BoundPipeline = &Pipeline;
	BoundStencilRef = StencilRef;

	Context->IASetInputLayout(Pipeline.GetInputLayout());
	Context->VSSetShader(&Pipeline.GetVertexShader(), nullptr, 0u);
	Context->PSSetShader(Pipeline.GetPixelShader(), nullptr, 0u);
	Context->RSSetState(&Pipeline.GetRasterizerState());
	Context->OMSetBlendState(&Pipeline.GetBlendState(), nullptr, 0xFFFFFFFF);
	Context->OMSetDepthStencilState(&Pipeline.GetDepthStencilState(), StencilRef);
}

void RenderContext::Draw(UINT VertexCount)
{
	Context->Draw(VertexCount, 0u);
}

void RenderContext::DrawIndexed(UINT IndexCount)
{
	Context->DrawIndexed(IndexCount, 0u, 0u);
}
