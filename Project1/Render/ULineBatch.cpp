#include "pch.h"
#include "ULineBatch.h"

ULineBatch::ULineBatch()
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(FVertexData) * MaxVertexCount;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	DEVICE->CreateBuffer(&desc, nullptr, &VertexBuffer);
}

ULineBatch::~ULineBatch()
{
	if (VertexBuffer) {
		VertexBuffer->Release();
		VertexBuffer = nullptr;
	}

	if (WorldBuffer) {
		delete WorldBuffer;
		WorldBuffer = nullptr;
	}
}

void ULineBatch::Initialize()
{
	WorldBuffer = new MatrixBuffer();
}

void ULineBatch::AddLine(const FVector& start, const FVector& end, const FLinearColor& Color)
{
	if (Vertices.size() + 2 > MaxVertexCount) return;

	FVertexData v0;
	v0.x = start.x; v0.y = start.y; v0.z = start.z;
	v0.r = Color.r; v0.g = Color.g; v0.b = Color.b; v0.a = Color.a;

	FVertexData v1;
	v1.x = end.x; v1.y = end.y; v1.z = end.z;
	v1.r = Color.r; v1.g = Color.g; v1.b = Color.b; v1.a = Color.a;

	Vertices.Add(v0);
	Vertices.Add(v1);

}

void ULineBatch::Render()
{
	if (Vertices.size() > MaxVertexCount) {
		VertexBuffer->Release();
		MaxVertexCount = Vertices.size() * 2;

		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = sizeof(FVertexData) * MaxVertexCount;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		DEVICE->CreateBuffer(&desc, nullptr, &VertexBuffer);
	}

	FMatrix World = FMatrix::Identity();
	WorldBuffer->SetMat(World);
	WorldBuffer->SetVSBuffer(0);

	D3D11_MAPPED_SUBRESOURCE mapped;
	DC->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, Vertices.data(), sizeof(FVertexData) * Vertices.size());
	DC->Unmap(VertexBuffer, 0);

	RENDERER.PrepareLineShader();
	DC->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	

	DC->Draw(Vertices.size(), 0);

	Vertices.clear();
}
