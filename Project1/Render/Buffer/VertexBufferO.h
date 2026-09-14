#pragma once

#pragma once
class VertexBufferO
{
public:
    VertexBufferO() : buffer(nullptr), stride(0), offset(0) {}
    VertexBufferO(const void* data, UINT stride, UINT count);
    ~VertexBufferO();

    void IASet(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

public:
    ID3D11Buffer* buffer = nullptr;
    UINT stride = 0;
    UINT offset = 0;
};

inline VertexBufferO::VertexBufferO(const void* data, UINT stride, UINT count)
    : stride(stride), offset(0)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = stride * count;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA subData = {};
    subData.pSysMem = data;

    DEVICE->CreateBuffer(&bufferDesc, &subData, &buffer);
}

inline VertexBufferO::~VertexBufferO()
{
    if (buffer)
    {
        buffer->Release();
        buffer = nullptr;
    }
}

inline void VertexBufferO::IASet(D3D11_PRIMITIVE_TOPOLOGY type)
{
    DC->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
    DC->IASetPrimitiveTopology(type);
}
