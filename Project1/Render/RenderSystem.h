#pragma once

#include "RenderDevice.h"
#include "RenderContext.h"
#include "RenderResources.h"
#include "PipelineCache.h"
#include <wrl/client.h>
#include <d3d11.h>

class RenderSystem final
{
public:
	static RenderSystem& GetInstance()
	{
		static RenderSystem Instance;
		return Instance;
	}

	void Initialize(HWND Window);
	void Shutdown();

	void Present(bool bVsync = true);

	void Resize(UINT Width, UINT Height);

	const RenderDevice& GetDevice() const { return Device; }
	RenderContext& GetContext() { return Context; }
	RenderResources& GetResources() { return Resources; }
	PipelineCache& GetPipelines() { return Pipelines; }
	const GraphicsPipeline& GetOrCreatePipeline(const GraphicsPipelineDesc& Desc) { return Pipelines.GetOrCreate(Desc); }

	ID3D11RenderTargetView& GetBackBufferRTV() const { return *BackBufferRTV.Get(); }
	ID3D11DepthStencilView& GetDepthStencilView() const { return *DepthStencilView.Get(); }

	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;
	RenderSystem(RenderSystem&&) = delete;
	RenderSystem& operator=(RenderSystem&&) = delete;

private:
	void CreateSwapChainResources(UINT Width, UINT Height);

	RenderDevice Device;
	RenderContext Context;
	RenderResources Resources;
	PipelineCache Pipelines;

	// 창이 여러개가 되면 분리 대상
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
	D3D11_VIEWPORT Viewport{};
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> BackBufferRTV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencilBuffer;

	RenderSystem() : Resources(Device), Pipelines(Device, Resources) {}
	~RenderSystem() = default;
};
