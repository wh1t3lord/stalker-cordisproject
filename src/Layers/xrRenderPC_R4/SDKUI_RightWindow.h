#pragma once
#include "ImageManager.h"

constexpr const char* SECTION_STATICOBJECTS_NAME = "Objects";
constexpr const char* SECTION_LIGHTS_NAME = "Lights";
constexpr const char* SECTION_SOUNDSOURCE_NAME = "Sound Source";
constexpr const char* SECTION_SOUNDENVIRONMENT_NAME = "Sound Environment";
constexpr const char* SECTION_GLOWS_NAME = "Glows";
constexpr const char* SECTION_SHAPES_NAME = "Shapes";
constexpr const char* SECTION_SPAWNELEMENTS_NAME = "Spawn Elements";
constexpr const char* SECTION_WAYPOINTS_NAME = "Way Points";
constexpr const char* SECTION_SECTORS_NAME = "Sectors";
constexpr const char* SECTION_PORTALS_NAME = "Portals";
constexpr const char* SECTION_GROUPS_NAME = "Groups";
constexpr const char* SECTION_STATICPARTICLES_NAME = "Static Particles";
constexpr const char* SECTION_DETAILOBJECTS_NAME = "Detail Objects";
constexpr const char* SECTION_AIMAP_NAME = "AI Map";
constexpr const char* SECTION_WALLMARKS_NAME = "Wallmarks";
constexpr const char* SECTION_FOGVOLUMES_NAME = "Fog Volumes";

class SDKUI_RightWindow
{
private:
    SDKUI_RightWindow(void)
        : m_flag(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
              ImGuiWindowFlags_HorizontalScrollbar),
          m_id_currentsection(0), m_currentselected_staticobject_name(""), m_is_visible(true), m_is_show_popup(false),
          m_is_init(false),
          m_sections({SECTION_STATICOBJECTS_NAME, SECTION_LIGHTS_NAME, SECTION_SOUNDSOURCE_NAME,
              SECTION_SOUNDENVIRONMENT_NAME, SECTION_GLOWS_NAME, SECTION_SHAPES_NAME, SECTION_SPAWNELEMENTS_NAME,
              SECTION_WAYPOINTS_NAME, SECTION_SECTORS_NAME, SECTION_PORTALS_NAME, SECTION_GROUPS_NAME,
              SECTION_STATICPARTICLES_NAME, SECTION_DETAILOBJECTS_NAME, SECTION_AIMAP_NAME, SECTION_WALLMARKS_NAME,
              SECTION_FOGVOLUMES_NAME})

    {
        if (ImGui::BeginMainMenuBar())
        {
            ImVec2 a = ImGui::GetWindowSize();
            this->m_size_mainmenubar_y = a.y;
            ImGui::EndMainMenuBar();
        }
    }

public:
    inline static SDKUI_RightWindow& Widget(void) noexcept
    {
        static SDKUI_RightWindow instance;
        return instance;
    }
    SDKUI_RightWindow(const SDKUI_RightWindow&) = delete;
    SDKUI_RightWindow& operator=(const SDKUI_RightWindow&) = delete;
    SDKUI_RightWindow(SDKUI_RightWindow&&) = delete;
    SDKUI_RightWindow& operator=(SDKUI_RightWindow&&) = delete;

    ~SDKUI_RightWindow(void);

    void Draw(void);

    inline void Hide(void) { this->m_is_visible = false; }
    inline void Show(void) { this->m_is_visible = true; }
    inline bool IsVisible(void) const noexcept { return this->m_is_visible; }
    inline int GetSection(void) const noexcept { return this->m_id_currentsection; }
    inline const xr_string& GetCurrentSelectedStaticObject(void) const noexcept
    {
        return this->m_currentselected_staticobject_name;
    }

private:
    bool m_is_visible;
    bool m_is_show_popup;
    bool m_is_init;
    int m_size_mainmenubar_y;
    int m_id_currentsection;
    ImGuiWindowFlags m_flag;
    xr_string m_currentselected_staticobject_name; // @ From list
    xr_vector<xr_string> m_sections;
};
