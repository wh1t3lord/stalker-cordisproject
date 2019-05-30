#pragma once

constexpr const char* SECTION_ALL_NAME = "ALL";

class SDKUI_ObjectList
{
private:
    SDKUI_ObjectList(void)
        : m_is_visible(false), m_flag(ImGuiWindowFlags_AlwaysAutoResize),
          m_sections({SECTION_ALL_NAME, SECTION_STATICOBJECTS_NAME, SECTION_LIGHTS_NAME, SECTION_SOUNDSOURCE_NAME,
              SECTION_SOUNDENVIRONMENT_NAME, SECTION_GLOWS_NAME, SECTION_SHAPES_NAME, SECTION_SPAWNELEMENTS_NAME,
              SECTION_WAYPOINTS_NAME, SECTION_SECTORS_NAME, SECTION_PORTALS_NAME, SECTION_GROUPS_NAME,
              SECTION_STATICPARTICLES_NAME, SECTION_DETAILOBJECTS_NAME, SECTION_AIMAP_NAME, SECTION_WALLMARKS_NAME,
              SECTION_FOGVOLUMES_NAME})
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
