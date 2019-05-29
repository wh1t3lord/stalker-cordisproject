#pragma once

class SDK_IconManager
{
private:
    SDK_IconManager(void) = default;

public:
    inline static SDK_IconManager& GetInstance(void) noexcept
    {
        static SDK_IconManager instance;
        return instance;
    }
    ~SDK_IconManager(void) = default;

    ID3D11ShaderResourceView* LoadImage(const xr_string&, int, int);
    bool LoadImageToolsIcons(const xr_string&, int, int);
    bool LoadImageThmIcons(const xr_string&, const xr_string&, int, int);
    inline size_t GetCountTHMIcons(void) { return this->thm_icons.size(); }
    inline size_t GetCountToolsIcons(void) { return this->tools_icons.size(); }
    inline ID3D11ShaderResourceView* GetImageFromToolsIcons(int idx)
    {
        if (idx > this->tools_icons.size() || !this->tools_icons.size())
            return nullptr;

        return this->tools_icons[idx];
    }

    inline ID3D11ShaderResourceView* GetImageFromTHMIcons(const xr_string& object_name)
    {
        if (!object_name.size() || !this->thm_icons.size())
            return nullptr;

        ID3D11ShaderResourceView* object = nullptr;

        try
        {
            object = this->thm_icons[object_name];
        }
        catch (...)
        {
            SDKUI_Log::Widget().SetColor(warning);
            SDKUI_Log::Widget().AddText("Something goes wrong, because application can't get thm icon for object");
        }

        return object;
    }
    void DestroyData(void);

private:
    xr_vector<ID3D11ShaderResourceView*> tools_icons;
    xr_map<xr_string, ID3D11ShaderResourceView*> thm_icons;
};
