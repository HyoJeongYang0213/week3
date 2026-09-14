#pragma once
class IndexBufferO
{
public:
    IndexBufferO() : buffer(nullptr), count(0) {}
    IndexBufferO(const uint32* indices, UINT count);
    ~IndexBufferO();

    void IASet();

public:
    ID3D11Buffer* buffer = nullptr;
    int count;
};

