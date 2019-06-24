#pragma once
#include "ImageManager.h"
#include "SDK_ObjectShape.h"
namespace Cordis
{
namespace SDK
{
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
          m_is_init(false), m_checked_box_selection(true), m_checked_sphere_selection(false)
    {
		this->m_currentselected_sectionname = SDK_Names::getInstance().getName("st_name_staticgeometry");
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

    inline void Hide(void) noexcept { this->m_is_visible = false; }
    inline void Show(void) noexcept { this->m_is_visible = true; }
    inline bool IsVisible(void) const noexcept { return this->m_is_visible; }
    inline unsigned int GetSection(void) const noexcept { return this->m_id_currentsection; }
    inline const xr_string& GetCurrentSelectedStaticObject(void) const noexcept
    {
        return this->m_currentselected_staticobject_name;
    }
    inline ShapeType GetCurrentShapeType() const noexcept
    {
        if (this->m_id_currentsection == kSection_Shapes)
        {
            if (this->m_checked_box_selection)
                return kShapeType_Box;
            else
                return kShapeType_Sphere;
        }

        return kShapeType_NonSelected;
    }

    inline void ChangeSectionByObject_CLASS(const CurrentSectionType& id)
    {
        this->m_currentselected_sectionname = this->m_sections[id];
        this->m_id_currentsection = id;
    }

    void Draw(void);



private:
    bool m_is_visible;
    bool m_is_show_popup;
    bool m_is_init;
    bool m_checked_sphere_selection;
    bool m_checked_box_selection;
    int m_size_mainmenubar_y;
    int m_id_currentsection;
    ImGuiWindowFlags m_flag;
    xr_string m_currentselected_staticobject_name; // @ From list
    xr_string m_currentselected_sectionname;
    xr_vector<xr_string> m_sections;
};

} // namespace SDK
} // namespace Cordis
