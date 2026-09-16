#pragma once

#include <wrl/client.h>
#include <d3d11.h>

class VertexBuffer
{
public:
	ID3D11Buffer& GetNativeBuffer() const { return *Buffer.Get(); }
	UINT GetStride() const { return Stride; }
	UINT GetOffset() const { return 0u; }
	UINT GetCount() const { return Count; }

private:
	friend class RenderDevice;
	VertexBuffer(UINT Stride, UINT Count) : Stride(Stride), Count(Count) {};

	Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer;
	UINT Stride = 0u;
	UINT Count = 0u;
};
