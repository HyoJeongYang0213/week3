#pragma once

class Mesh;

class FontMeshGenerator
{
public:
    static Mesh* Generate(const std::wstring& text, float glyphAdvance);
private:
    static uint32 GetAtlasIndex(wchar_t ch);
};