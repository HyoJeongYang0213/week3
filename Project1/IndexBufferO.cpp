#include "pch.h"
#include "IndexBuffer.h"

IndexBufferO::IndexBufferO(const uint32* indices, UINT count) : count(count)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(uint32) * count;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA subData = {};
    subData.pSysMem  = indices;

    DEVICE->CreateBuffer(&bufferDesc, &subData, &buffer);
}

IndexBufferO::~IndexBufferO()
{
    if (buffer)
    {
        buffer->Release();
        buffer = nullptr;
    }
}

void IndexBufferO::IASet()
{
    DC->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
}
