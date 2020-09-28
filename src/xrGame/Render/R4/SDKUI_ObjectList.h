#pragma once

namespace Cordis
{
    namespace SDK
    {
    class SDKUI_ObjectList
    {
    private:
        SDKUI_ObjectList(void) noexcept : m_is_visible(false), m_flag(ImGuiWindowFlags_AlwaysAutoResize), m_index_currentsection(0)
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
