#pragma once
class IndexBuffer
{
public:
    IndexBuffer() : buffer(nullptr), count(0) {}
    IndexBuffer(const uint32* indices, UINT count);
    ~IndexBuffer();

    void IASet();

public:
    ID3D11Buffer* buffer = nullptr;
    int count;
};

