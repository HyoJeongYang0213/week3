#pragma once

#include "GraphicsEnums.h"
#include "Containers.h"
#include <type_traits>

struct Material final
{
	VertexShaderType VertexShader;
	PixelShaderType PixelShader;

	FString Texture; // TODO: FName으로 변경
	Sampler Sampler;

	BlendMode Blend;
	// bool bTwoSided; 필요하다면 추가하고 GraphicsPipeline에 반영

	template <typename T> requires std::is_trivially_copyable_v<T>
	void SetConstants(const T& Constants);

	const void* GetConstantData() const { return ConstantData.GetData(); }
	UINT GetConstantSize() const { return static_cast<UINT>(ConstantData.Num()); }

private:
	TArray<uint8> ConstantData;
};

template <typename T> requires std::is_trivially_copyable_v<T>
void Material::SetConstants(const T& Constants)
{
	ConstantData.Resize(sizeof(T));
	memcpy(ConstantData.GetData(), &Constants, sizeof(T));
}
