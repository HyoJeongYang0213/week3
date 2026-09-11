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
	// void SetViewport()
	// void SetScissorRect()
	// void ClearRenderTarget()
	// void ClearDepthStencil()

	void SetVertexBuffer(const VertexBuffer& InVertexBuffer);
	void SetIndexBuffer(const IndexBuffer& InIndexBuffer);
	
	void SetConstantBuffer(UINT Slot, const ConstantBuffer& InConstantBuffer);
	void UpdateConstantBuffer(ConstantBuffer& InConstantBuffer, const void* Data, UINT Size);
	template <typename T> void UpdateConstantBuffer(ConstantBuffer& InConstantBuffer, const T& Value);

	void SetMesh(const RenderMesh& Mesh);
	void SetTexture(UINT Slot, const Texture& InTexture);
	void SetSampler(UINT Slot, ID3D11SamplerState& SamplerState);

	void SetPipeline(const GraphicsPipeline& Pipeline);

	void Draw(UINT VertexCount);
	void DrawIndexed(UINT IndexCount);
	
	// ID3D11DeviceContext& GetNativeContext() const { return *Context.Get(); }
	ID3D11DeviceContext& GetNativeContext() const { return *DC; }

private:
	friend class RenderSystem;

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context;
};

template <typename T>
void RenderContext::UpdateConstantBuffer(ConstantBuffer& InConstantBuffer, const T& Value)
{
	UpdateConstantBuffer(InConstantBuffer, &Value, sizeof(T));
}
