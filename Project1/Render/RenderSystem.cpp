#include "pch.h"
#include "RenderSystem.h"

#include "RenderDevice.h"
#include "RenderContext.h"
#include "RenderResources.h"

void RenderSystem::Initialize(HWND Window)
{
	constexpr D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	const DXGI_SWAP_CHAIN_DESC SwapChainDesc{
		.BufferDesc = {
			.Width = 0u,
			.Height = 0u,
			.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
		},
		.SampleDesc = {
			.Count = 1u,
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 2u,
		.OutputWindow = Window,
		.Windowed = true,
		.SwapEffect = DXGI_SWAP_EFFECT_DISCARD,
	};

	UINT CreateDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//D3D11CreateDeviceAndSwapChain(
	//	nullptr,
	//	D3D_DRIVER_TYPE_HARDWARE,
	//	nullptr,
	//	CreateDeviceFlags,
	//	FeatureLevels,
	//	ARRAYSIZE(FeatureLevels),
	//	D3D11_SDK_VERSION,
	//	&SwapChainDesc,
	//	&SwapChain,
	//	&Device.Device,
	//	nullptr,
	//	&Context.Context
	//);

	Device.Device = DEVICE;
	Context.Context = DC;
	SwapChain = RENDERER.GetSwapChain();

	RECT ClientRect{};
	GetClientRect(Window, &ClientRect);

	CreateSwapChainResources(
		static_cast<UINT>(ClientRect.right - ClientRect.left),
		static_cast<UINT>(ClientRect.bottom - ClientRect.top)
	);

	Resources.RegisterDefaultResources();
}

void RenderSystem::Shutdown()
{
	Context.Context->ClearState();
	Context.Context->Flush();
}

void RenderSystem::Present(bool bVsync)
{
	SwapChain->Present(bVsync ? 1u : 0u, 0u);
}

void RenderSystem::Resize(UINT Width, UINT Height)
{
	if (Width == 0u || Height == 0u)
	{
		return;
	}

	Context.Context->OMSetRenderTargets(0, nullptr, nullptr);

	BackBufferRTV.Reset();
	DepthStencilBuffer.Reset();
	DepthStencilView.Reset();

	SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0u);

	CreateSwapChainResources(Width, Height);
}

void RenderSystem::CreateSwapChainResources(UINT Width, UINT Height)
{
	auto& NativeDevice = Device.Device;

	Viewport = {
		.TopLeftX = 0.0f,
		.TopLeftY = 0.0f,
		.Width = static_cast<float>(Width),
		.Height = static_cast<float>(Height),
		.MinDepth = 0.0f,
		.MaxDepth = 1.0f,
	};

	Microsoft::WRL::ComPtr<ID3D11Texture2D> BackBuffer;
	SwapChain->GetBuffer(0u, IID_PPV_ARGS(&BackBuffer));
	NativeDevice->CreateRenderTargetView(BackBuffer.Get(), nullptr, &BackBufferRTV);

	const D3D11_TEXTURE2D_DESC DepthStencilDesc{
		.Width = Width,
		.Height = Height,
		.MipLevels = 1u,
		.ArraySize = 1u,
		.Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
		.SampleDesc = {
			.Count = 1u,
		},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_DEPTH_STENCIL,
	};
	NativeDevice->CreateTexture2D(&DepthStencilDesc, nullptr, &DepthStencilBuffer);
	NativeDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), nullptr, &DepthStencilView);
}
