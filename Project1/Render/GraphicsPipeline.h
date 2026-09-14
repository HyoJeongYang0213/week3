#pragma once

#include "GraphicsEnums.h"
#include <wrl/client.h>
#include <d3d11.h>

struct GraphicsPipeline
{
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

struct RasterizerDesc
{
	D3D11_FILL_MODE FillMode = D3D11_FILL_SOLID;
	D3D11_CULL_MODE CullMode = D3D11_CULL_BACK;
	bool bFrontCounterClockWise = false;

	bool operator==(const RasterizerDesc&) const = default;
};

struct DepthDesc
{
	bool bEnable = true;
	D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	bool operator==(const DepthDesc&) const = default;
};

struct StencilFaceDesc
{
	D3D11_STENCIL_OP FailOp = D3D11_STENCIL_OP_KEEP;
	D3D11_STENCIL_OP DepthFailOp = D3D11_STENCIL_OP_KEEP;
	D3D11_STENCIL_OP PassOp = D3D11_STENCIL_OP_KEEP;
	D3D11_COMPARISON_FUNC Func = D3D11_COMPARISON_ALWAYS;

	bool operator==(const StencilFaceDesc&) const = default;
};

struct StencilDesc
{
	bool bEnable = false;
	UINT8 ReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	UINT8 WriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	StencilFaceDesc FrontFace;
	StencilFaceDesc BackFace;

	bool operator==(const StencilDesc&) const = default;
};

struct GraphicsPipelineDesc
{
	VertexShaderType VertexShader;
	PixelShaderType PixelShader;

	VertexLayout VertexLayout;

	RasterizerDesc Rasterizer;
	BlendMode Blend;
	DepthDesc Depth;
	StencilDesc Stencil;

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
		HashCombine(Hash, std::hash<bool>()(Desc.Depth.bEnable));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Depth.DepthWriteMask)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Depth.DepthFunc)));
		HashCombine(Hash, std::hash<bool>()(Desc.Stencil.bEnable));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Stencil.ReadMask)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Stencil.WriteMask)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Stencil.FrontFace.FailOp)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Stencil.FrontFace.DepthFailOp)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Stencil.FrontFace.PassOp)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Stencil.FrontFace.Func)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Stencil.BackFace.FailOp)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Stencil.BackFace.DepthFailOp)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Stencil.BackFace.PassOp)));
		HashCombine(Hash, std::hash<uint8>()(static_cast<uint8>(Desc.Stencil.BackFace.Func)));
		return Hash;
	}

private:
	void HashCombine(std::size_t& Seed, std::size_t Value) const
	{
		Seed ^= Value + 0x9e3779b9 + (Seed << 6) + (Seed >> 2);
	}
};

namespace StencilMode
{
	constexpr StencilDesc Disabled{
		.bEnable = false
	};

	constexpr StencilDesc TestNotEqual{
		.bEnable = true,
		.ReadMask = 0xFF,
		.WriteMask = 0xFF,
		.FrontFace = {
			.FailOp = D3D11_STENCIL_OP_KEEP,
			.DepthFailOp = D3D11_STENCIL_OP_KEEP,
			.PassOp = D3D11_STENCIL_OP_KEEP,
			.Func = D3D11_COMPARISON_NOT_EQUAL
		},
		.BackFace = {
			.FailOp = D3D11_STENCIL_OP_KEEP,
			.DepthFailOp = D3D11_STENCIL_OP_KEEP,
			.PassOp = D3D11_STENCIL_OP_KEEP,
			.Func = D3D11_COMPARISON_NOT_EQUAL
		}
	};
}

