#pragma once

namespace Cordis
{
    namespace SDK
    {
    constexpr const char* SECTION_ALL_NAME = "ALL";

    class SDKUI_ObjectList
    {
    private:
        SDKUI_ObjectList(void) noexcept
            : m_is_visible(false), m_flag(ImGuiWindowFlags_AlwaysAutoResize),
              m_sections({SDK_Names::getInstance().getName("st_log_all"), SDK_Names::getInstance().getName("st_name_staticgeometry"), SDK_Names::getInstance().getName("st_name_light"), SDK_Names::getInstance().getName("st_name_sound_sources"),
                  SDK_Names::getInstance().getName("st_name_sound_environments"), SECTION_GLOWS_NAME, SDK_Names::getInstance().getName("st_name_shapes"), SDK_Names::getInstance().getName("st_name_spawnelements"),
                  SDK_Names::getInstance().getName("st_name_waypoints"), SDK_Names::getInstance().getName("st_name_sectors"), SDK_Names::getInstance().getName("st_name_portals"), SDK_Names::getInstance().getName("st_name_groups"),
                  SDK_Names::getInstance().getName("st_name_staticparticles"), SDK_Names::getInstance().getName("st_name_detail_objects"), SDK_Names::getInstance().getName("st_name_aimap"), SDK_Names::getInstance().getName("st_name_wallmarks"),
                  SDK_Names::getInstance().getName("st_name_fogvolumes")})
        {
        }

    public:
        inline static SDKUI_ObjectList& Widget(void) noexcept
        {
            static SDKUI_ObjectList instance;
            return instance;
        }

        SDKUI_ObjectList(const SDKUI_ObjectList&) = delete;
        SDKUI_ObjectList& operator=(const SDKUI_ObjectList&) = delete;
        SDKUI_ObjectList(SDKUI_ObjectList&&) = delete;
        SDKUI_ObjectList& operator=(SDKUI_ObjectList&&) = delete;
        ~SDKUI_ObjectList(void);

        inline void Hide(void) noexcept { this->m_is_visible = false; }
        inline void Show(void) noexcept { this->m_is_visible = true; }
        inline bool IsVisible(void) const noexcept { return this->m_is_visible; }
        void Draw(void);

    private:
        bool m_is_visible;
        ImGuiWindowFlags m_flag;
        unsigned int m_index_currentsection;
        xr_vector<xr_string> m_sections;
    };

    }
}
