#pragma once
#include "pch.h"

class ULineBatch {
public:
	ULineBatch();
	~ULineBatch();

	static ULineBatch & GetInstance() {
		static ULineBatch instance;
		return instance;
	}

	void Initialize();

	void AddLine(const FVector & start, const FVector & end, const FLinearColor &Color);
	void Render();


private:
	TArray<FVertexData> Vertices;
	Mesh* LineMesh = nullptr;
	
	ID3D11Buffer* VertexBuffer = nullptr;
	UINT MaxVertexCount = 10000;

	UINT stride = sizeof(FVertexData);
	UINT offset = 0;

	MatrixBuffer* WorldBuffer = nullptr;
};