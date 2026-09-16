#pragma once

#include <wrl/client.h>
#include <d3d11.h>

class ConstantBuffer final
{
public:
	ID3D11Buffer& GetNativeBuffer() const { return *Buffer.Get(); }
	UINT GetSize() const { return Size; }

private:
	friend class RenderDevice;

	ConstantBuffer(UINT InSize) : Size(InSize) {};

	Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer;
	UINT Size = 0u;
};
