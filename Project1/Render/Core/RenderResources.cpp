#include "pch.h"
#include "RenderResources.h"

#include "GraphicsEnums.h"
#include "RenderDevice.h"
#include "RenderMesh.h"
#include "VertexShader.h"
#include "Texture.h"

void RenderResources::RegisterDefaultResources()
{
	wchar_t Buffer[256];
	GetModuleFileNameW(nullptr, Buffer, 256);
	FWString ExecutableDirectory = filesystem::path(Buffer).parent_path();

	CreateMesh("Cube", cube_vertices);
	CreateMesh("Sphere", CreateSphereVertices(0.5f, 20, 20, false));
	CreateMesh("Triangle", triangle_vertices);
	CreateMesh("Rectangle", rectangle_vertices);
	CreateMesh("Circle", CircleGenerator::MakeCircle(32, 1.0f, 1.0f, 0.0f, 1.0f));
	CreateMesh("GizmoLocation", arrow_vertices);
	CreateMesh("GizmoRotate", rotate_ring_vertices);
	CreateMesh("GizmoScale", scale_axis_vertices);
	CreateMesh("SkySphere", skysphere_vertices);

	VertexShaders.Resize(static_cast<int>(VertexShaderType::Count));
	PixelShaders.Resize(static_cast<int>(PixelShaderType::Count));
	SamplerStates.Resize(static_cast<int>(Sampler::Count));

	CreateVertexShader(VertexShaderType::Mesh, ExecutableDirectory +L"\\Shader\\MeshVS.cso");
	CreateVertexShader(VertexShaderType::Line, ExecutableDirectory + L"\\Shader\\LineVS.cso");
	CreateVertexShader(VertexShaderType::Outline, ExecutableDirectory + L"\\Shader\\OutlineVS.cso");
	CreateVertexShader(VertexShaderType::Sky, ExecutableDirectory + L"\\Shader\\SkyVS.cso");
	CreateVertexShader(VertexShaderType::Font, ExecutableDirectory + L"\\Shader\\FontVS.cso");

	CreatePixelShader(PixelShaderType::Mesh, ExecutableDirectory + L"\\Shader\\MeshPS.cso");
	CreatePixelShader(PixelShaderType::Line, ExecutableDirectory + L"\\Shader\\LinePS.cso");
	CreatePixelShader(PixelShaderType::Sky, ExecutableDirectory + L"\\Shader\\SkyPS.cso");
	CreatePixelShader(PixelShaderType::Font, ExecutableDirectory + L"\\Shader\\FontPS.cso");

	CreateTexture("SkyTexture", L"Resources\\Textures\\Sky.jpg"); // TODO: 릴리즈 시 수정
	CreateTexture("FontAtlas", L"Resources\\Textures\\Pretendard-Regular.dds"); // TODO: 릴리즈 시 수정

	SamplerStates[static_cast<uint8>(Sampler::LinearWrap)] = Device.CreateSamplerState(Sampler::LinearWrap);
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
const VertexShader& RenderResources::CreateVertexShader(VertexShaderType Type, const FWString& Path)
{
	assert(Type != VertexShaderType::None && Type != VertexShaderType::Count);

	Microsoft::WRL::ComPtr<ID3DBlob> Blob;
	D3DReadFileToBlob(Path.c_str(), &Blob);
	VertexShaders[static_cast<size_t>(Type)] = Device.CreateVertexShader(Blob->GetBufferPointer(), Blob->GetBufferSize());

	return *VertexShaders[static_cast<size_t>(Type)];
}

ID3D11PixelShader& RenderResources::GetPixelShader(PixelShaderType Type) const
{
	assert(Type != PixelShaderType::None && Type != PixelShaderType::Count);
	return *PixelShaders[static_cast<uint8>(Type)].Get();
}

// TODO: 경로 찾기 수정
ID3D11PixelShader& RenderResources::CreatePixelShader(PixelShaderType Type, const FWString& Path)
{
	assert(Type != PixelShaderType::None && Type != PixelShaderType::Count);

	Microsoft::WRL::ComPtr<ID3DBlob> Blob;
	D3DReadFileToBlob(Path.c_str(), &Blob);

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
	const bool bDDS = std::filesystem::path(FileName).extension() == L".dds";

	DirectX::ScratchImage Image;
	if (bDDS)
	{
		DirectX::LoadFromDDSFile(FileName.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, Image);
	}
	else
	{
		DirectX::LoadFromWICFile(FileName.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, Image);
	}

	Textures[Name] = Texture{};
	DirectX::CreateShaderResourceView(&Device.GetNativeDevice(), Image.GetImages(), Image.GetImageCount(), Image.GetMetadata(), &Textures[Name]->SRV);

	return *Textures[Name];
}

ID3D11SamplerState& RenderResources::GetSamplerState(Sampler Type) const
{
	assert(Type != Sampler::None && Type != Sampler::Count);
	return *SamplerStates[static_cast<uint8>(Type)].Get();
}
