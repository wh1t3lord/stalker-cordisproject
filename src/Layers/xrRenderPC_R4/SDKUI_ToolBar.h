#pragma once

class SDKUI_ToolBar
{
private:
    SDKUI_ToolBar(void);

public:
    inline static SDKUI_ToolBar& Widget(void) noexcept
    {
        static SDKUI_ToolBar instance;
        return instance;
    }

    SDKUI_ToolBar(const SDKUI_ToolBar&) = delete;
    SDKUI_ToolBar& operator=(const SDKUI_ToolBar&) = delete;
    SDKUI_ToolBar(SDKUI_ToolBar&&) = delete;
    SDKUI_ToolBar& operator=(SDKUI_ToolBar&&) = delete;
    ~SDKUI_ToolBar(void);

    void Draw(void);
    inline void Show(void) noexcept { this->bShow = true; }
    inline void Hide(void) noexcept { this->bShow = false; }
    inline bool IsVisible(void) noexcept { return this->bShow; }
    private:
    bool bShow;
    ImGuiWindowFlags flag;
};
