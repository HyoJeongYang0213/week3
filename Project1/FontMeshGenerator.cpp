#include "pch.h"
#include "FontMeshGenerator.h"
#include "Mesh.h"
#include "FVertexSImple.h"
#include "Sphere.h"

uint32 FontMeshGenerator::GetAtlasIndex(wchar_t ch)
{
	// 1. ASCII 범위인가?
	if (ch >= 0x20 && ch <= 0x7E)
	{
		return ch - 0x20;
	}
	// 2. 한글 자모 범위인가?
	else if (ch >= 0x3131 && ch <= 0x3163)
	{
		return ch - 0x3131 + 95;
	}
	// 3. 완성형 한글 범위인가?
	else if (ch >= 0xAC00 && ch <= 0xD7A3)
	{
		return ch - 0xAC00 + 146;
	}
	// 4. 지원 안 하는 문자: '?'
	return 31;
}

Mesh* FontMeshGenerator::Generate(const std::wstring& text, float glyphAdvance)
{
	std::vector<FVertexData> vertices;
	std::vector<uint32> indices;

	for (size_t i = 0; i < text.size(); i++)
	{
		wchar_t ch = text[i];

		// 1. ch로 atlasIndex / UV 계산
		uint32 atlasIndex = GetAtlasIndex(ch);

		// 2. font_quad_vertices 4개를 복사
		FVertexData quad[4] =
		{
			font_quad_vertices[0],
			font_quad_vertices[1],
			font_quad_vertices[2],
			font_quad_vertices[3]
		};

		// 3. i번째 글자만큼 각 정점의 x를 이동
		uint32 col = atlasIndex % 128;
		uint32 row = atlasIndex / 128;
		float u0 = col / 128.0f;
		float u1 = (col + 1) / 128.0f;

		float v0 = row / 128.0f;
		float v1 = (row + 1) / 128.0f;

		float xOffset = static_cast<float>(i) * glyphAdvance;

		for (int j = 0; j < 4; ++j)
		{
			quad[j].x += xOffset;
		}

		// 4. UV 적용
		quad[0].u = u0;
		quad[1].u = u1;
		quad[2].u = u0;
		quad[3].u = u1;

		quad[0].v = v0;
		quad[1].v = v0;
		quad[2].v = v1;
		quad[3].v = v1;

		// 5. vertices에 정점 4개 추가
		vertices.push_back(quad[0]);
		vertices.push_back(quad[1]);
		vertices.push_back(quad[2]);
		vertices.push_back(quad[3]);

		// 6. indices에 인덱스 6개 추가
		uint32 baseVertex = static_cast<uint32>(i * 4);
		for (int j = 0; j < 6; ++j)
		{
			indices.push_back(font_quad_indices[j] + baseVertex);
		}
	}


	Mesh* mesh = new Mesh(vertices);

	mesh->InitIndexBuffer(
		indices.data(),
		static_cast<UINT>(indices.size())
	);

	mesh->SetTexture(L"Resources/Textures/Pretendard-Regular.dds");
	mesh->bIsFont = true;

	return mesh;
}