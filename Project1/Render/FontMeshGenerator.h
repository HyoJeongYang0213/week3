#pragma once

class Mesh;

struct FontGeometry
{
	TArray<FVertexData> Vertices;
	TArray<uint32> Indices;
};

namespace FontMeshGenerator
{
	FontGeometry Generate(const std::wstring& text, float glyphAdvance);
};
