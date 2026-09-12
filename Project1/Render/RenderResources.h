#pragma once

#include "Pointers.h"
#include "Containers.h"
#include "GraphicsEnums.h"
#include "RenderMesh.h"
#include "VertexShader.h"
#include "Texture.h"
#include <wrl/client.h>
#include <d3d11.h>

class RenderDevice;

// TODO: FString -> FName
class RenderResources final
{
public:
	void RegisterDefaultResources();

	const RenderMesh* GetMesh(const FString& Name);
	const RenderMesh& CreateMesh(
		const FString& Name,
		const void* Vertices, 
		UINT Stride, UINT VertexCount, 
		const void* IndexData = nullptr, 
		UINT IndexCount = 0u, 
		D3D11_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	template <typename TVertex>
	const RenderMesh& CreateMesh(const FString& Name, const TArray<TVertex>& Vertices);
	template <typename TVertex, typename TIndex>
	const RenderMesh& CreateMesh(const FString& Name, const TArray<TVertex>& Vertices, const TArray<TIndex>& Indices);

	const VertexShader& GetVertexShader(VertexShaderType Type) const;
	const VertexShader& CreateVertexShader(VertexShaderType Type, const FWString& FileName, const FString& EntryPoint);
	ID3D11PixelShader& GetPixelShader(PixelShaderType Type) const;
	ID3D11PixelShader& CreatePixelShader(PixelShaderType Type, const FWString& FileName, const FString& EntryPoint);

	const Texture* GetTexture(const FString& Name);
	const Texture& CreateTexture(const FString& Name, const FWString& FileName);

	ID3D11SamplerState& GetSamplerState(Sampler Type) const;

private:
	friend class RenderSystem;
	explicit RenderResources(RenderDevice& Device) : Device(Device) {};

	RenderDevice& Device;

	TMap<FString, TOptional<RenderMesh>> Meshes;
	TArray<TOptional<VertexShader>> VertexShaders; // TODO: TStaticArray로 변경
	TArray<Microsoft::WRL::ComPtr<ID3D11PixelShader>> PixelShaders; // TODO: TStaticArray로 변경
	TMap<FString, TOptional<Texture>> Textures;
	TArray<Microsoft::WRL::ComPtr<ID3D11SamplerState>> SamplerStates; // TODO: TStaticArray로 변경
};

template <typename TVertex>
const RenderMesh& RenderResources::CreateMesh(const FString& Name, const TArray<TVertex>& Vertices)
{
	return CreateMesh(
		Name,
		Vertices.GetData(),
		sizeof(TVertex),
		static_cast<UINT>(Vertices.Num()),
		nullptr,
		0u);
}

template <typename TVertex, typename TIndex>
const RenderMesh& RenderResources::CreateMesh(
	const FString& Name,
	const TArray<TVertex>& Vertices,
	const TArray<TIndex>& Indices)
{
	if (Indices.IsEmpty())
	{
		return CreateMesh(
			Name,
			Vertices.GetData(),
			sizeof(TVertex),
			static_cast<UINT>(Vertices.Num()),
			nullptr,
			0u);
	}

	return CreateMesh(
		Name,
		Vertices.GetData(),
		sizeof(TVertex),
		static_cast<UINT>(Vertices.Num()),
		Indices.GetData(),
		static_cast<UINT>(Indices.Num()));
}
