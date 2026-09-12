#include "pch.h"
#include "RenderDevice.h"

#include "GraphicsEnums.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "GraphicsPipeline.h"

VertexBuffer RenderDevice::CreateVertexBuffer(const void* Data, UINT Stride, UINT Count) const
{
	VertexBuffer Result{ Stride, Count };

	const D3D11_BUFFER_DESC Desc{
		.ByteWidth = Stride * Count,
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER,
	};

	const D3D11_SUBRESOURCE_DATA InitData{
		.pSysMem = Data,
	};

	Device->CreateBuffer(&Desc, &InitData, &Result.Buffer);

	return Result;
}

IndexBuffer RenderDevice::CreateIndexBuffer(const void* Data, UINT Count) const
{
	IndexBuffer Result{ Count };

	const D3D11_BUFFER_DESC Desc{
		.ByteWidth = IndexBuffer::Size * Count,
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_INDEX_BUFFER,
	};

	const D3D11_SUBRESOURCE_DATA InitData{
		.pSysMem = Data,
	};

	Device->CreateBuffer(&Desc, &InitData, &Result.Buffer);

	return Result;
}

ConstantBuffer RenderDevice::CreateConstantBuffer(UINT Size) const
{
	Size = (Size + 15u) & ~15u; // 16바이트 단위로 정렬

	ConstantBuffer Result{ Size };

	const D3D11_BUFFER_DESC Desc{
		.ByteWidth = Size,
		.Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
	};

	Device->CreateBuffer(&Desc, nullptr, &Result.Buffer);

	return Result;
}

VertexShader RenderDevice::CreateVertexShader(const void* ByteCode, SIZE_T ByteCodeSize) const
{
	VertexShader Result;

	Device->CreateVertexShader(ByteCode, ByteCodeSize, nullptr, &Result.Shader);
	Result.ByteCode.Resize(ByteCodeSize);
	std::memcpy(Result.ByteCode.GetData(), ByteCode, ByteCodeSize);

	return Result;
}

Microsoft::WRL::ComPtr<ID3D11PixelShader> RenderDevice::CreatePixelShader(const void* ByteCode, SIZE_T ByteCodeSize) const
{
	Microsoft::WRL::ComPtr<ID3D11PixelShader> Result;

	Device->CreatePixelShader(ByteCode, ByteCodeSize, nullptr, &Result);

	return Result;
}

Microsoft::WRL::ComPtr<ID3D11SamplerState> RenderDevice::CreateSamplerState(Sampler Type) const
{
	assert(Type != Sampler::None && Type != Sampler::Count);

	Microsoft::WRL::ComPtr<ID3D11SamplerState> Result;

	D3D11_SAMPLER_DESC Desc;
	switch (Type)
	{
	case Sampler::LinearWrap:
		Desc = {
			.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			.AddressU = D3D11_TEXTURE_ADDRESS_WRAP,
			.AddressV = D3D11_TEXTURE_ADDRESS_WRAP,
			.AddressW = D3D11_TEXTURE_ADDRESS_WRAP,
			.ComparisonFunc = D3D11_COMPARISON_NEVER,
			.MinLOD = 0.0f,
			.MaxLOD = D3D11_FLOAT32_MAX
		};
		break;
	}

	Device->CreateSamplerState(&Desc, &Result);

	return Result;
}

GraphicsPipeline RenderDevice::CreateGraphicsPipeline(
	const VertexShader& InVertexShader,
	ID3D11PixelShader& PixelShader,
	VertexLayout Layout, 
	RasterizerDesc Rasterizer,
	BlendMode Blend,
	DepthStencilDesc DepthStencil) const
{
	GraphicsPipeline Result{ &InVertexShader.GetShader(), &PixelShader };

	const auto InputLayouts = GetVertexLayoutDesc(Layout);
	Device->CreateInputLayout(
		InputLayouts.GetData(),
		InputLayouts.Num(), 
		InVertexShader.GetByteCode().GetData(),
		InVertexShader.GetByteCode().Num(), &Result.InputLayout);

	const D3D11_RASTERIZER_DESC RasterizerDesc{
		.FillMode = Rasterizer.FillMode,
		.CullMode = Rasterizer.CullMode,
		.FrontCounterClockwise = Rasterizer.bFrontCounterClockWise,
	};
	Device->CreateRasterizerState(&RasterizerDesc, &Result.RasterizerState);

	const D3D11_BLEND_DESC BlendDesc = GetBlendDesc(Blend);
	Device->CreateBlendState(&BlendDesc, &Result.BlendState);

	const D3D11_DEPTH_STENCIL_DESC DepthDesc{
		.DepthEnable = DepthStencil.bDepthEnabled,
		.DepthWriteMask = DepthStencil.DepthWriteMask,
		.DepthFunc = DepthStencil.DepthFunc,
	};
	Device->CreateDepthStencilState(&DepthDesc, &Result.DepthStencilState);

	return Result;
}
