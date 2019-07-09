#pragma once
class SDK_PixelShader
{
public:
    SDK_PixelShader(void) = default;
    SDK_PixelShader(const SDK_PixelShader&) = delete;
    SDK_PixelShader& operator=(const SDK_PixelShader&) = delete;
    SDK_PixelShader(SDK_PixelShader&&) = delete;
    SDK_PixelShader& operator=(SDK_PixelShader&&) = delete;


    bool Initialize(const xr_string&);
    inline ID3D10Blob* GetBuffer(void) const { return this->pBuffer; }
    inline ID3D11PixelShader* GetShader(void) const { return this->pShader; }

    ~SDK_PixelShader(void);

private:
    ID3D10Blob* pBuffer;
    ID3D11PixelShader* pShader;
};
