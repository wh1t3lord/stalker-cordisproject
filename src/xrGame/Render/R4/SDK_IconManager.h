#pragma once

namespace Cordis
{
    namespace SDK
    {
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
        inline size_t GetCountTHMIcons(void) const noexcept { return this->m_thm_icons.size(); }
        inline size_t GetCountToolsIcons(void) const noexcept { return this->m_tools_icons.size(); }
        inline ID3D11ShaderResourceView* GetImageFromToolsIcons(int idx) const
        {
            if (idx > this->m_tools_icons.size() || !this->m_tools_icons.size())
                return nullptr;

            return this->m_tools_icons[idx];
        }

        inline ID3D11ShaderResourceView* GetImageFromTHMIcons(const xr_string& object_name)
        {
            if (!object_name.size() || !this->m_thm_icons.size())
                return nullptr;

            ID3D11ShaderResourceView* object = nullptr;

            try
            {
                object = this->m_thm_icons[object_name];
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
        xr_vector<ID3D11ShaderResourceView*> m_tools_icons;
        xr_map<xr_string, ID3D11ShaderResourceView*> m_thm_icons;
    };

    }
}
