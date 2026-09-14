#pragma once

#include "GraphicsEnums.h"
#include "GraphicsPipeline.h"
#include <wrl/client.h>
#include <d3d11.h>

class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
struct RenderMesh;
struct VertexShader;
struct Texture;
struct GraphicsPipeline;

class RenderDevice final
{
public:
	VertexBuffer CreateVertexBuffer(const void* Data, UINT Stride, UINT Count) const;
	IndexBuffer CreateIndexBuffer(const void* Data, UINT Count) const;
	ConstantBuffer CreateConstantBuffer(UINT Size) const;

	VertexShader CreateVertexShader(const void* ByteCode, SIZE_T ByteCodeSize) const;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> CreatePixelShader(const void* ByteCode, SIZE_T ByteCodeSize) const;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> CreateSamplerState(Sampler Type) const;
	GraphicsPipeline CreateGraphicsPipeline(
		const VertexShader& InVertexShader,
		ID3D11PixelShader& PixelShader,
		VertexLayout Layout,
		RasterizerDesc Rasterizer,
		BlendMode Blend,
		DepthStencilDesc DepthStencil) const;

	// ID3D11Device& GetNativeDevice() const { return *Device.Get(); }
	ID3D11Device& GetNativeDevice() const { return *DEVICE; }

private:
	friend class RenderSystem;

	Microsoft::WRL::ComPtr<ID3D11Device> Device;
};
