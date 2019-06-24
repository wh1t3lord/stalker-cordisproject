#pragma once
#include "ImageManager.h"
#include "SDK_ObjectShape.h"
namespace Cordis
{
namespace SDK
{


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
