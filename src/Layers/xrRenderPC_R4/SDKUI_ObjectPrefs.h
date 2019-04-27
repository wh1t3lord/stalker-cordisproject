#pragma once

class SDK_ObjectStaticGeometry;

class SDKUI_StaticGeometryPref
{
private:
    SDKUI_StaticGeometryPref(void) { this->bShow = false; }

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

    inline void Show(void) { this->bShow = true; }
    inline void Hide(void) { this->bShow = false; }
    inline bool IsVisible(void) const { return this->bShow; }
    void Draw(SDK_ObjectStaticGeometry*);

private:
    bool bShow;
};
