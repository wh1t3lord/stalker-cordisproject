#pragma once

template <class T>
class SDK_ConstantBuffer
{
public:
    SDK_ConstantBuffer(void) = default;
    SDK_ConstantBuffer(const SDK_ConstantBuffer<T>&) = delete;
    SDK_ConstantBuffer& operator=(const SDK_ConstantBuffer<T>&) = delete;
    SDK_ConstantBuffer(SDK_ConstantBuffer<T>&&) = delete;
    SDK_ConstantBuffer& operator=(SDK_ConstantBuffer<T>&&) = delete;
    ~SDK_ConstantBuffer(void)
    {
        if (this->pBuffer)
        {
            this->pBuffer->Release();
            this->pBuffer = nullptr;
        }
    }

    inline ID3D11Buffer* GetBuffer(void) const { return this->pBuffer; }
    inline ID3D11Buffer* const* GetAddressOf(void) const { return &this->pBuffer; }
    inline T GetData(void) const { return this->data; }

    bool Initialize(void)
    {
        if (!HW.pDevice)
        {
            ASSERT(TEXT("Device was null"));
            return;
        }

        if (!HW.pContext)
        {
            ASSERT(TEXT("Context was null"));
            return;
        }

        D3D11_BUFFER_DESC Desc;
        Desc.Usage = D3D11_USAGE_DYNAMIC;
        Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        Desc.MiscFlags = 0;
        Desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
        Desc.StructureByteStride = 0;
        
        HRESULT hr = HW.pContext->CreateBuffer(&Desc, 0, &this->pBuffer);
        if (FAILED(hr))
        {
            ASSERT(TEXT("Can't Create a Constant Buffer!"));
            return false;
        }

        return true;
    }

    void ApplyChanges(void) 
    { 
        if (!HW.pContext)
        {
            ASSERT(TEXT("Context was null"));
            return;
        }

        D3D11_MAPPED_SUBRESOURCE Resource;

        if (!this->pBuffer)
        {
            ASSERT(TEXT("Buffer was null"));
            return;
        }

        HRESULT hr = HW.pContext->Map(this->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, &Resource);
        if (FAILED(hr))
        {
            ASSERT(TEXT("Can't Map"));
            return;
        }

        if (!Resource.pData)
        {
            ASSERT(TEXT("Data was null"));
            return;
        }

        CopyMemory(Resource.pData, &this->data, sizeof(T));
        HW.pContext->Unmap(this->pBuffer, 0);
    }

private:
    T data;
    ID3D11Buffer* pBuffer;
};
