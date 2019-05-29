#pragma once

class SDKUI_ObjectList
{
private:
    SDKUI_ObjectList(void) : bShow(false), flag(ImGuiWindowFlags_AlwaysAutoResize) {}

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

    inline void Hide(void) noexcept { this->bShow = false; } 
    inline void Show(void) noexcept { this->bShow = true; }
    inline bool IsVisible(void) noexcept { return this->bShow; }
    void Draw(void);
private:
    bool bShow;
    ImGuiWindowFlags flag;
};
