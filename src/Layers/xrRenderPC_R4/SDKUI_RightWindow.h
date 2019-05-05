#pragma once
#include "ImageManager.h"
class SDKUI_RightWindow
{
private:
    SDKUI_RightWindow(void) 
    {
        if (ImGui::BeginMainMenuBar())
        {
            ImVec2 a = ImGui::GetWindowSize();
            this->MainMenuBarY = a.y;
            ImGui::EndMainMenuBar();
        }

        this->flag = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar;
        this->sections.push_back("Objects"); // 0 ..
        this->sections.push_back("Lights");
        this->sections.push_back("Sound Source");
        this->sections.push_back("Sound Environment");
        this->sections.push_back("Glows");
        this->sections.push_back("Shapes");
        this->sections.push_back("Spawn Elements");
        this->sections.push_back("Way Points");
        this->sections.push_back("Sectors");
        this->sections.push_back("Portals");
        this->sections.push_back("Groups");
        this->sections.push_back("Static Particles");
        this->sections.push_back("Detail Objects");
        this->sections.push_back("AI Map");
        this->sections.push_back("Wallmarks");
        this->sections.push_back("Fog Volumes"); // 15
        this->Current_Section = 0;
        this->CurrentSelectedStaticObject = "";
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

    inline void Hide(void) { this->bShow = false; }
    inline void Show(void) { this->bShow = true; }
    inline bool IsVisible(void) const { return this->bShow; }
    inline int GetSection(void) const { return this->Current_Section; }
    inline const xr_string& GetCurrentSelectedStaticObject(void) const noexcept
    {
        return this->CurrentSelectedStaticObject;
    }

private:
    bool bShow = true;
    bool bShowPopup = false;
    bool was_init = false;
    int MainMenuBarY;
    int Current_Section = 0;
    ImGuiWindowFlags flag;
    xr_string CurrentSelectedStaticObject; // @ From list
    xr_vector<xr_string> sections;
};
