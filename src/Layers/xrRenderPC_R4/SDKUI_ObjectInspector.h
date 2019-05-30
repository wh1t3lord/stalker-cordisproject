#pragma once

class SDKUI_ObjectInspector
{
private:
    SDKUI_ObjectInspector(void) : m_is_visible(true), m_flag(ImGuiWindowFlags_AlwaysAutoResize) // @ Lord bShow(false)!
    {
    
    }

public:
    inline static SDKUI_ObjectInspector& Widget(void) noexcept
    {
        static SDKUI_ObjectInspector instance;
        return instance;
    }

    SDKUI_ObjectInspector(const SDKUI_ObjectInspector&) = delete;
    SDKUI_ObjectInspector& operator=(const SDKUI_ObjectInspector&) = delete;
    SDKUI_ObjectInspector(SDKUI_ObjectInspector&&) = delete;
    SDKUI_ObjectInspector& operator=(SDKUI_ObjectInspector&&) = delete;
    ~SDKUI_ObjectInspector(void);

    inline void Show(void) noexcept { this->m_is_visible = true; }
    inline void Hide(void) noexcept { this->m_is_visible = false; }
    inline bool IsVisible(void) const noexcept { return this->m_is_visible; }

    void Draw(void);

private:
    bool m_is_visible;
    ImGuiWindowFlags m_flag;
};
