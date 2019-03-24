#include "stdafx.h"
#include "SDK_VertexShader.h"

SDK_VertexShader::~SDK_VertexShader()
{
    if (this->pShader)
    {
        this->pShader->Release();
        this->pShader = nullptr;
    }

    if (this->pLayout)
    {
        this->pLayout->Release();
        this->pLayout = nullptr;
    }

    if (this->pBuffer)
    {
        this->pBuffer->Release();
        this->pBuffer = nullptr;
    }





}

bool SDK_VertexShader::Initialize(const xr_string& filename, D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements)
{
    if (!desc)
    {
        ASSERT(TEXT("desc was null"));
        return false;
    }

    if (filename.empty())
    {
        ASSERT(TEXT("The string can't be empty"));
        return false;
    }

    if (!numElements)
    {
        ASSERT(TEXT("numElemnets was null!"));
        return false;
    }

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &filename[0], filename.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &filename[0], filename.size(), &wstrTo[0], size_needed);

    HRESULT hr = D3DReadFileToBlob(wstrTo.c_str(), &this->pBuffer);
    if (FAILED(hr))
    {
        ASSERT(TEXT("Can't ReadFileToBlob!"));
        return false;
    }

    hr = HW.pDevice->CreateVertexShader(
        this->pBuffer->GetBufferPointer(), this->pBuffer->GetBufferSize(), NULL, &this->pShader);
    if (FAILED(hr))
    {
        ASSERT(TEXT("Can't create a vertex shader!"));
        return false;
    }

    hr = HW.pDevice->CreateInputLayout(
        desc, numElements, this->pBuffer->GetBufferPointer(), this->pBuffer->GetBufferSize(), &this->pLayout);
    if (FAILED(hr))
    {
        ASSERT(TEXT("Can't create a layout for vertex shader!"));
        return false;
    }

    return true;
}
