#pragma once

#include <wrl/client.h>
#include <d3d11.h>

struct VertexShader final
{
public:
	ID3D11VertexShader& GetShader() const { return *Shader.Get(); }
	const TArray<uint8>& GetByteCode() const { return ByteCode; }

private:
	friend class RenderDevice;
	VertexShader() = default;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> Shader;
	TArray<uint8> ByteCode;
};
