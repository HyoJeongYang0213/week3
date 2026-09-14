#pragma once

#include <wrl/client.h>
#include <d3d11.h>

class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
struct RenderMesh;
struct Material;
struct Texture;
struct GraphicsPipeline;

class RenderContext final
{
public:
	void SetRenderTarget(ID3D11RenderTargetView& RenderTarget, ID3D11DepthStencilView* DepthStencilView = nullptr);
	void SetViewport(const D3D11_VIEWPORT& Viewport);
	// void SetScissorRect()
	void ClearRenderTarget(ID3D11RenderTargetView& RenderTarget, const float* Color = nullptr);
	void ClearDepthStencil(ID3D11DepthStencilView& DepthStencilView, float Depth = 1.0f, UINT8 Stencil = 0u);

	void SetVertexBuffer(const VertexBuffer& InVertexBuffer);
	void UpdateVertexBuffer(const VertexBuffer& InVertexBuffer, const void* Data, UINT Size);
	template <typename T> void UpdateVertexBuffer(const VertexBuffer& InVertexBuffer, const T& Value);

	void SetIndexBuffer(const IndexBuffer& InIndexBuffer);

	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);
	
	void SetConstantBuffer(UINT Slot, const ConstantBuffer& InConstantBuffer);
	void UpdateConstantBuffer(ConstantBuffer& InConstantBuffer, const void* Data, UINT Size);
	template <typename T> void UpdateConstantBuffer(ConstantBuffer& InConstantBuffer, const T& Value);

	void SetMesh(const RenderMesh& Mesh);
	void SetTexture(UINT Slot, const Texture& InTexture);
	void SetSampler(UINT Slot, ID3D11SamplerState& SamplerState);

	void SetPipeline(const GraphicsPipeline& Pipeline, UINT8 StencilRef = 0u);

	void Draw(UINT VertexCount);
	void DrawIndexed(UINT IndexCount);
	
	ID3D11DeviceContext& GetNativeContext() const { return *Context.Get(); }

private:
	friend class RenderSystem;

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context;
};

template <typename T>
void RenderContext::UpdateVertexBuffer(const VertexBuffer& InVertexBuffer, const T& Value)
{
	UpdateVertexBuffer(InVertexBuffer, &Value, sizeof(T));
}

template <typename T>
void RenderContext::UpdateConstantBuffer(ConstantBuffer& InConstantBuffer, const T& Value)
{
	UpdateConstantBuffer(InConstantBuffer, &Value, sizeof(T));
}
