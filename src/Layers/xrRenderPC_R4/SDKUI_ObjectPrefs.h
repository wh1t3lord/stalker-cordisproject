#pragma once

class SDK_ObjectStaticGeometry;

class SDKUI_StaticGeometryPref
{
private:
    SDKUI_StaticGeometryPref(void) : m_is_visible(false) {}

public:
    inline static SDKUI_StaticGeometryPref& Widget(void)
    {
        static SDKUI_StaticGeometryPref instance;
        return instance;
    }

    SDKUI_StaticGeometryPref(const SDKUI_StaticGeometryPref&) = delete;
    SDKUI_StaticGeometryPref& operator=(const SDKUI_StaticGeometryPref&) = delete;
    SDKUI_StaticGeometryPref(SDKUI_StaticGeometryPref&&) = delete;
    SDKUI_StaticGeometryPref& operator=(SDKUI_StaticGeometryPref&&) = delete;
    ~SDKUI_StaticGeometryPref(void) noexcept = default;

    inline void Show(void) { this->m_is_visible = true; }
    inline void Hide(void) { this->m_is_visible = false; }
    inline bool IsVisible(void) const noexcept { return this->m_is_visible; }
    void Draw(SDK_ObjectStaticGeometry*);

private:
    bool m_is_visible;
};
