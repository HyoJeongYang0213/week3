#pragma once

#include <wrl/client.h>
#include <d3d11.h>

class IndexBuffer final
{
public:
	ID3D11Buffer& GetNativeBuffer() const { return *Buffer.Get(); }
	UINT GetCount() const { return Count; }
	DXGI_FORMAT GetFormat() const { return DXGI_FORMAT_R32_UINT; }
	UINT GetOffset() const { return 0u; }

	static constexpr UINT Size = sizeof(uint32); // 인덱스는 uint32만 쓰는걸로 가정

private:
	friend class RenderDevice;
	IndexBuffer(UINT Count) : Count(Count) {};

	Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer;
	UINT Count = 0u;
};
