#include "stdafx.h"
#include "SDK_IconManager.h"

namespace Cordis
{
    namespace SDK
    {
    ID3D11ShaderResourceView* SDK_IconManager::LoadImage(const xr_string& path, int width, int height)
    {
        if (!path.size())
            return false;

        ID3D11ShaderResourceView* shdr_view = nullptr;
        D3DX11_IMAGE_LOAD_INFO info;
        info.Width = D3DX11_DEFAULT;
        info.Height = D3DX11_DEFAULT;
        info.Depth = D3DX11_DEFAULT;
        info.FirstMipLevel = D3DX11_DEFAULT;
        info.MipLevels = D3DX11_DEFAULT;
        info.Usage = (D3D11_USAGE)D3DX11_DEFAULT;
        info.BindFlags = D3DX11_DEFAULT;
        info.CpuAccessFlags = D3DX11_DEFAULT;
        info.MiscFlags = D3DX11_DEFAULT;
        info.Format = DXGI_FORMAT_FROM_FILE;
        info.Filter = D3DX11_DEFAULT;
        info.MipFilter = D3DX11_DEFAULT;
        info.pSrcInfo = NULL;
        HRESULT hr = D3DX11CreateShaderResourceViewFromFileA(HW.pDevice, path.c_str(), &info, NULL, &shdr_view, NULL);
        if (FAILED(hr))
        {
            R_ASSERT2(false, "Can't load image data!");
        }

        return shdr_view;
    }

    bool SDK_IconManager::LoadImageThmIcons(const xr_string& path, const xr_string& object_name, int width, int height)
    {
        ID3D11ShaderResourceView* obj = this->LoadImage(path, width, height);
        if (obj)
        {
            this->m_thm_icons.insert(std::pair<xr_string, ID3D11ShaderResourceView*>(object_name, obj));
            return true;
        }

        return false;
    }

    bool SDK_IconManager::LoadImageToolsIcons(const xr_string& path, int width, int height)
    {
        ID3D11ShaderResourceView* obj = this->LoadImage(path, width, height);
        if (obj)
        {
            this->m_tools_icons.push_back(obj);
            return true;
        }

        return false;
    }

    void SDK_IconManager::DestroyData(void)
    {
#pragma region TOOLS
        for (xr_vector<ID3D11ShaderResourceView*>::iterator it = this->m_tools_icons.begin();
             it != this->m_tools_icons.end(); ++it)
        {
            if ((*it))
            {
                (*it)->Release();
                (*it) = nullptr;
            }
        }

        this->m_tools_icons.clear();
#pragma endregion

#pragma region THM
        for (xr_map<xr_string, ID3D11ShaderResourceView*>::value_type& obj : this->m_thm_icons)
        {
            if (obj.second)
            {
                obj.second->Release();
                obj.second = nullptr;
            }
        }

        this->m_thm_icons.clear();
#pragma endregion
    }

    }
}

