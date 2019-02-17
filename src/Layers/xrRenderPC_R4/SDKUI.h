/*
    [Author]: Lord
    [Date]: 16.02.2019
*/

#pragma once

#include "SDKUI_Helpers.h"
#include "../../xrEngine/MonitorManager.hpp"

class SDKUI
{
private:
    SDKUI()
    {
        DisplayX = g_monitors.GetMaximalResolution().first;
        DisplayY = g_monitors.GetMaximalResolution().second;
        bShowMainMenuBar = true;
        // @ Init All stuff
        SDKUI_Log::Widget().Init(GetPosBottomLeft(), 500, 150, ImGuiWindowFlags_NoResize);
        SDKUI_Log::Widget().SetColor(error);
        SDKUI_Log::Widget().AddText("SDK was initialized! %s", "Test");
    } 

public:
    static SDKUI& UI(void)
    {
        static SDKUI instance;
        return instance;
    }

    ~SDKUI() = default;

    void Begin(void);
    void Draw(void);
    void End(void);


    inline void Show(void)
    {
        bShowMainMenuBar = true;
    }

    inline void Hide(void)
    { 
        bShowMainMenuBar = false;
    }

private:
    void DrawMainMenuBar(void);
    void KeyBoardMessages(void);

    inline int GetCenteredX(void) { return DisplayX / 2; }
    inline int GetCenteredY(void) { return DisplayY / 2; }

    inline ImVec2 GetPosCentered(void) { return ImVec2(GetCenteredX(), GetCenteredY()); }
    inline ImVec2 GetPosTopLeft(void) { return ImVec2(0, 0); }
    inline ImVec2 GetPosTopRight(void) { return ImVec2(DisplayX, 0); }
    inline ImVec2 GetPosBottomLeft(void) { return ImVec2(0, DisplayY); }
    inline ImVec2 GetPosBottomRight(void) { return ImVec2(DisplayX, DisplayY); }

private:
    bool bShowMainMenuBar;
    bool bCloseOnce = false;
    int DisplayX;
    int DisplayY;
};
