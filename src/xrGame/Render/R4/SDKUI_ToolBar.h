#pragma once

namespace Cordis
{
    namespace SDK
    {
    class SDKUI_ToolBar
    {
    private:
        SDKUI_ToolBar(void) noexcept
            : m_is_visible(true), m_flag(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize)
        {
        }

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
        inline void Show(void) noexcept { this->m_is_visible = true; }
        inline void Hide(void) noexcept { this->m_is_visible = false; }
        inline bool IsVisible(void) const noexcept { return this->m_is_visible; }

    private:
        bool m_is_visible;
        ImGuiWindowFlags m_flag;
    };
    }
}

