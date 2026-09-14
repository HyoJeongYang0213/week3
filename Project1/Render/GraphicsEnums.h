#pragma once

#include "Containers.h"
#include <d3d11.h>

enum class VertexShaderType : uint8
{
	Object,
	Grid,

	Count,
	None = 0xFF,
};

enum class PixelShaderType : uint8
{
	Object,
	Grid,

	Count,
	None = 0xFF,
};

enum class Sampler : uint8
{
	LinearWrap,

	Count,
	None = 0xFF,
};

// === Blend Modes ===
enum class BlendMode : uint8
{
	None,
	AlphaBlend,
};

inline D3D11_BLEND_DESC GetBlendDesc(BlendMode Blend)
{
	switch (Blend)
	{
	case BlendMode::None:
		return {
			.RenderTarget = {{
				.BlendEnable = false,
				.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL
			}},
		};

	case BlendMode::AlphaBlend:
		return {
			.RenderTarget = {{
				.BlendEnable = true,
				.SrcBlend = D3D11_BLEND_SRC_ALPHA,
				.DestBlend = D3D11_BLEND_INV_SRC_ALPHA,
				.BlendOp = D3D11_BLEND_OP_ADD,
				.SrcBlendAlpha = D3D11_BLEND_ONE,
				.DestBlendAlpha = D3D11_BLEND_ZERO,
				.BlendOpAlpha = D3D11_BLEND_OP_ADD,
				.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL
			}},
		};
	}

	assert(false);
	return {
		.RenderTarget = {{
			.BlendEnable = false,
			.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL
		}},
	};
}


// === Vertex Layouts ===
enum class VertexLayout : uint8
{
	None,
	PositionColor,
	PositionColorUVNormal,
};

inline TArray<D3D11_INPUT_ELEMENT_DESC> GetVertexLayoutDesc(VertexLayout Layout)
{
	switch (Layout)
	{
	case VertexLayout::None:
		return {};

	case VertexLayout::PositionColor:
		return {
			{
				"POSITION", 0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0, 0,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"COLOR", 0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
		};

	case VertexLayout::PositionColorUVNormal:
		return {
			{
				"POSITION", 0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0, 0,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"COLOR", 0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"TEXCOORD", 0,
				DXGI_FORMAT_R32G32_FLOAT,
				0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"NORMAL", 0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
		};
	}

	assert(false);
	return {};
}
