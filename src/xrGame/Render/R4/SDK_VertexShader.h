#pragma once
class SDK_VertexShader
{
public:
    SDK_VertexShader(void) = default;
    SDK_VertexShader(const SDK_VertexShader&) = delete;
    SDK_VertexShader& operator=(const SDK_VertexShader&) = delete;
    SDK_VertexShader(SDK_VertexShader&&) = delete;
    SDK_VertexShader& operator=(SDK_VertexShader&&) = delete;

    ~SDK_VertexShader(void);

    bool Initialize(const xr_string&, D3D11_INPUT_ELEMENT_DESC*, UINT);

    inline ID3D10Blob* GetBuffer(void) const { return this->pBuffer; }
    inline ID3D11VertexShader* GetShader(void) const { return this->pShader; }
    inline ID3D11InputLayout* GetLayout(void) const { return this->pLayout; }

private:
    ID3D10Blob* pBuffer;
    ID3D11VertexShader* pShader;
    ID3D11InputLayout* pLayout;
};
