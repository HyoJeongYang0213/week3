#pragma once

#include <wrl/client.h>
#include <d3d11.h>

struct Texture final
{
	ID3D11ShaderResourceView& GetSRV() const { return *SRV.Get(); }

private:
	friend class RenderResources;
	Texture() = default;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV;
};
