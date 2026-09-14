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
// TODO:
//  나중에 용도를 Immutable, Default, Dynamic으로 구분할 수 있습니다. 프레임당 Constant는 Default가 적합한 편입니다.
//  이 경우에는 RenderContext::UpdateConstantBuffer()도 용도별로 방식을 바꾸도록 수정이 필요합니다.
