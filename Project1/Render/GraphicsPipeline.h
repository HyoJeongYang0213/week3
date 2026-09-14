#pragma once

#include "GraphicsEnums.h"
#include <wrl/client.h>
#include <d3d11.h>

struct GraphicsPipeline final
{
public:
	ID3D11VertexShader& GetVertexShader() const
	{
		assert(VertexShader);
		return *VertexShader;
	}
	ID3D11PixelShader* GetPixelShader() const { return PixelShader; }

	ID3D11InputLayout* GetInputLayout() const { return InputLayout.Get(); }
	ID3D11RasterizerState& GetRasterizerState() const { return *RasterizerState.Get(); }
	ID3D11BlendState& GetBlendState() const { return *BlendState.Get(); }
	ID3D11DepthStencilState& GetDepthStencilState() const { return *DepthStencilState.Get(); }

private:
	friend class RenderDevice;
	GraphicsPipeline(ID3D11VertexShader* VertexShader, ID3D11PixelShader* PixelShader)
		: VertexShader(VertexShader), PixelShader(PixelShader) {}

	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> BlendState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState;
};

struct RasterizerDesc final
{
	D3D11_FILL_MODE FillMode = D3D11_FILL_SOLID;
	D3D11_CULL_MODE CullMode = D3D11_CULL_BACK;
	bool bFrontCounterClockWise = false;

	bool operator==(const RasterizerDesc&) const = default;
};

struct DepthStencilDesc final
{
	bool bDepthEnabled = true;
	D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	bool operator==(const DepthStencilDesc&) const = default;
};

struct GraphicsPipelineDesc final
{
	VertexShaderType VertexShader;
	PixelShaderType PixelShader;

	VertexLayout VertexLayout;

	RasterizerDesc Rasterizer;
	BlendMode Blend;
	DepthStencilDesc DepthStencil;

	bool operator==(const GraphicsPipelineDesc&) const = default;
};

struct GraphicsPipelineDescHash
{
	std::size_t operator()(const GraphicsPipelineDesc& Desc) const
	{
		std::size_t Hash = 0;
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.VertexShader)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.PixelShader)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.VertexLayout)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Rasterizer.FillMode)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Rasterizer.CullMode)));
		HashCombine(Hash, std::hash<bool>()(Desc.Rasterizer.bFrontCounterClockWise));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Blend)));
		HashCombine(Hash, std::hash<bool>()(Desc.DepthStencil.bDepthEnabled));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.DepthStencil.DepthWriteMask)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.DepthStencil.DepthFunc)));
		return Hash;
	}

private:
	inline void HashCombine(std::size_t& Seed, std::size_t Value) const
	{
		Seed ^= Value + 0x9e3779b9 + (Seed << 6) + (Seed >> 2);
	}
};
