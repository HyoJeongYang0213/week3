#include "pch.h"
#include "RenderResources.h"

#include "GraphicsEnums.h"
#include "RenderDevice.h"
#include "RenderMesh.h"
#include "VertexShader.h"
#include "Texture.h"

void RenderResources::RegisterDefaultResources()
{
	VertexShaders.Resize(static_cast<int>(VertexShaderType::Count));
	PixelShaders.Resize(static_cast<int>(PixelShaderType::Count));
	SamplerStates.Resize(static_cast<int>(Sampler::Count));

	CreateVertexShader(VertexShaderType::Object, L"Resources/Shader/ShaderW0.hlsl", "mainVS");
	CreateVertexShader(VertexShaderType::Grid, L"Resources/Shader/GridShader.hlsl", "mainVS_Grid");
	CreatePixelShader(PixelShaderType::Object, L"Resources/Shader/ShaderW0.hlsl", "mainPS");
	CreatePixelShader(PixelShaderType::Grid, L"Resources/Shader/GridShader.hlsl", "mainPS_Grid");
}

const RenderMesh* RenderResources::GetMesh(const FString& Name)
{
	auto It = Meshes.find(Name);
	if (It != Meshes.end())
	{
		return It->second ? &It->second.value() : nullptr;
	}

	return nullptr;
}

const RenderMesh& RenderResources::CreateMesh(const FString& Name, const void* Vertices, UINT Stride, UINT VertexCount,
	const void* IndexData, UINT IndexCount, D3D11_PRIMITIVE_TOPOLOGY Topology)
{
	if (IndexCount != 0u)
	{
		Meshes[Name] = RenderMesh{
			Device.CreateVertexBuffer(Vertices, Stride, VertexCount),
			Device.CreateIndexBuffer(IndexData, IndexCount),
			Topology };
	}
	else
	{
		Meshes[Name] = RenderMesh{
			Device.CreateVertexBuffer(Vertices, Stride, VertexCount),
			Topology };
	}

	return *Meshes[Name];
}

const VertexShader& RenderResources::GetVertexShader(VertexShaderType Type) const
{
	assert(Type != VertexShaderType::None && Type != VertexShaderType::Count);
	return *VertexShaders[static_cast<uint8>(Type)];
}

// TODO: 경로 찾기 수정
const VertexShader& RenderResources::CreateVertexShader(VertexShaderType Type, const FWString& FileName, const FString& EntryPoint)
{
	assert(Type != VertexShaderType::None && Type != VertexShaderType::Count);

	std::filesystem::path Path(FileName);
	if (!std::filesystem::exists(Path)) {
		if (std::filesystem::exists(Path.filename())) {
			Path = Path.filename();
		}
		else if (std::filesystem::exists(std::filesystem::path(L"Resources/Shader") / Path.filename())) {
			Path = std::filesystem::path(L"Resources/Shader") / Path.filename();
		}
		else if (std::filesystem::exists(std::filesystem::path(L"Project1") / FileName)) {
			Path = std::filesystem::path(L"Project1") / FileName;
		}
		else if (std::filesystem::exists(std::filesystem::path(L"../Project1") / FileName)) {
			Path = std::filesystem::path(L"../Project1") / FileName;
		}
	}

	Microsoft::WRL::ComPtr<ID3DBlob> Blob;
	D3DCompileFromFile(Path.c_str(), nullptr, nullptr, EntryPoint.c_str(), "vs_5_0", 0, 0, &Blob, nullptr);

	VertexShaders[static_cast<size_t>(Type)] = Device.CreateVertexShader(Blob->GetBufferPointer(), Blob->GetBufferSize());

	return *VertexShaders[static_cast<size_t>(Type)];
}

ID3D11PixelShader& RenderResources::GetPixelShader(PixelShaderType Type) const
{
	assert(Type != PixelShaderType::None && Type != PixelShaderType::Count);
	return *PixelShaders[static_cast<uint8>(Type)].Get();
}

// TODO: 경로 찾기 수정
ID3D11PixelShader& RenderResources::CreatePixelShader(PixelShaderType Type, const FWString& FileName, const FString& EntryPoint)
{
	assert(Type != PixelShaderType::None && Type != PixelShaderType::Count);

	std::filesystem::path Path(FileName);
	if (!std::filesystem::exists(Path)) {
		if (std::filesystem::exists(Path.filename())) {
			Path = Path.filename();
		}
		else if (std::filesystem::exists(std::filesystem::path(L"Resources/Shader") / Path.filename())) {
			Path = std::filesystem::path(L"Resources/Shader") / Path.filename();
		}
		else if (std::filesystem::exists(std::filesystem::path(L"Project1") / FileName)) {
			Path = std::filesystem::path(L"Project1") / FileName;
		}
		else if (std::filesystem::exists(std::filesystem::path(L"../Project1") / FileName)) {
			Path = std::filesystem::path(L"../Project1") / FileName;
		}
	}

	Microsoft::WRL::ComPtr<ID3DBlob> Blob;
	D3DCompileFromFile(Path.c_str(), nullptr, nullptr, EntryPoint.c_str(), "ps_5_0", 0, 0, &Blob, nullptr);

	PixelShaders[static_cast<size_t>(Type)] = Device.CreatePixelShader(Blob->GetBufferPointer(), Blob->GetBufferSize());

	return *PixelShaders[static_cast<size_t>(Type)].Get();
}

const Texture* RenderResources::GetTexture(const FString& Name)
{
	auto It = Textures.find(Name);
	if (It != Textures.end())
	{
		return It->second ? &It->second.value() : nullptr;
	}

	return nullptr;
}

const Texture& RenderResources::CreateTexture(const FString& Name, const FWString& FileName)
{
	DirectX::ScratchImage Image;
	assert(SUCCEEDED(DirectX::LoadFromWICFile(FileName.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, Image)));

	Textures[Name] = Texture{};
	DirectX::CreateShaderResourceView(&Device.GetNativeDevice(), Image.GetImages(), Image.GetImageCount(), Image.GetMetadata(), &Textures[Name]->SRV);

	return *Textures[Name];
}

ID3D11SamplerState& RenderResources::GetSamplerState(Sampler Type) const
{
	assert(Type != Sampler::None && Type != Sampler::Count);
	return *SamplerStates[static_cast<uint8>(Type)].Get();
}
