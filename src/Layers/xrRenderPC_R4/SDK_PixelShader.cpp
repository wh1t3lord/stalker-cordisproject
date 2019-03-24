#include "stdafx.h"
#include "SDK_PixelShader.h"

SDK_PixelShader::~SDK_PixelShader(void)
{
    if (this->pShader)
    {
        this->pShader->Release();
        this->pShader = nullptr;
    }

    if (this->pBuffer)
    {
        this->pBuffer->Release();
        this->pBuffer = nullptr;
    }


}

bool SDK_PixelShader::Initialize(const xr_string& filename)
{
    if (filename.empty())
    {
        ASSERT(TEXT("was null"));
        return false;
    }
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &filename[0], filename.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &filename[0], filename.size(), &wstrTo[0], size_needed);

    HRESULT hr = D3DReadFileToBlob(wstrTo.c_str(), &this->pBuffer);

    if (FAILED(hr))
    {
        ASSERT(TEXT("Can't read file"));
        return false;
    }

    hr = HW.pDevice->CreatePixelShader(this->pBuffer->GetBufferPointer(), this->pBuffer->GetBufferSize(), 0, &this->pShader);
    if (FAILED(hr))
    {
        ASSERT(TEXT("Can't create"));
        return false;
    }

    return true;
}
