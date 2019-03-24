#pragma once
#include "stdafx.h"

#include "../../xrCore/Imgui/imgui.h"
#include "../../xrCore/Imgui/imgui_impl_sdl.h"
#include "../../xrCore/Imgui/imgui_impl_dx11.h"

enum SDKErrorType
{
    default, // white
    warning, // yellow
    error, // red
    good, // green
    special, // dark-blue
    unimportant // gray
};

class SDKUI_Log
{
private:
    SDKUI_Log()
    {
        Clear();
        currentcolor = default;
    }

public:
    static SDKUI_Log& Widget()
    {
        static SDKUI_Log instance;
        return instance;
    }
    SDKUI_Log(SDKUI_Log&) = delete;
    SDKUI_Log& operator=(const SDKUI_Log&) = delete;
    ~SDKUI_Log() = default;

    void Draw(void);

    inline void Show(void) { bShow = true; }
    inline void Hide(void) { bShow = false; }
    inline bool GetVisible(void) { return bShow; }
    inline void Init(int x, int y, int SizeX, int SizeY, ImGuiWindowFlags flag = NULL)
    {
        CurrentSizeX = SizeX;
        CurrentSizeY = SizeY;
        CurrentPosX = x;

        if (CurrentPosX - (CurrentSizeX) < 0)
            CurrentPosX = 0;
        else
            CurrentPosX -= CurrentSizeX;

        CurrentPosY = y;

        if (CurrentPosY - (CurrentSizeY) < 0)
            CurrentPosY = 0;
        else
            CurrentPosY -= CurrentSizeY;

        wndFlags = flag;
    }

    inline void Init(const ImVec2& pos, const ImVec2& size, ImGuiWindowFlags flag = NULL)
    {
        Init(pos.x, pos.y, size.x, size.y, flag);
    }

    inline void Init(const ImVec2& pos, int SizeX, int SizeY, ImGuiWindowFlags flag = NULL)
    {
        Init(pos.x, pos.y, SizeX, SizeY, flag);
    }

    inline void Init(int x, int y, const ImVec2& size, ImGuiWindowFlags flag = NULL)
    {
        Init(x, y, size.x, size.y, flag);
    }

    // @ Use it before call AddText method
    inline void SetColor(SDKErrorType type)
    {
        currentcolor = type;
        bSysCall = true;
    }

    inline void AddText(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        buf.appendfv(fmt, args);
        xr_string sas = "[";
        sas += __TIME__;

        if (bSysCall == false)
            currentcolor = default;

        if (currentcolor == SDKErrorType::error)
            sas += "|ERROR";
        else if (currentcolor == SDKErrorType::warning)
            sas += "|WARNING";
        else if (currentcolor == SDKErrorType::special)
            sas += "|INFO";
        else if (currentcolor == SDKErrorType::unimportant)
            sas += "|OLD";
        else if (currentcolor == SDKErrorType::good)
            sas += "|SUCCESSFUL";

        sas += "] ";
        sas += buf.c_str();

        if (sas[sas.size() - 1] != '\n')
            sas += '\n';

        va_end(args);
        buff.push_back(sas);
        buf.clear();
        bSysCall = false;
    }

    inline void Clear(void)
    {
        buf.clear();
        buff.clear();
        LineOffSet.clear();
        LineOffSet.push_back(0);
    }

private:
    bool bShow = true;
    bool bSysCall = false;

    bool bShowErrors = true;
    bool bShowWarnings = true;
    bool bShowDefault = true;
    bool bShowSpecial = true;
    bool bShowUn = true; // @ unimportant
    bool bShowGood = true;

    int CurrentSizeX = 0;
    int CurrentSizeY = 0;
    int CurrentPosX = 0;
    int CurrentPosY = 0;
    SDKErrorType currentcolor;
    ImGuiWindowFlags wndFlags;
    ImGuiTextBuffer buf;
    xr_vector<xr_string> buff;

    ImVector<int> LineOffSet;
};

class SDKUI_Overlay
{
private:
    SDKUI_Overlay() = default;

public:
    static SDKUI_Overlay& Widget(void)
    {
        static SDKUI_Overlay instance;
        return instance;
    }

    ~SDKUI_Overlay() = default;
    SDKUI_Overlay(SDKUI_Overlay&) = delete;
    SDKUI_Overlay& operator=(const SDKUI_Overlay&) = delete;

    void Draw(void);
    inline void Show(void) { bShow = true; }
    inline void Hide(void) { bShow = false; }
    inline bool GetVisible(void) { return bShow; }

private:
    bool bShow = true;
    bool bShowOnlySDKInfo = true;
    bool bShowOnlySysInfo = true;
    bool bShowOnlyProjectInfo = true;
    int iGridType = 0;
};

class SDKUI_CameraHelper
{
private:
    SDKUI_CameraHelper(void) = default;

public:
    inline static SDKUI_CameraHelper& Widget(void)
    { 
        static SDKUI_CameraHelper instance;
        return instance;
    }
    SDKUI_CameraHelper(const SDKUI_CameraHelper&) = delete;
    SDKUI_CameraHelper& operator=(const SDKUI_CameraHelper&) = delete;
    SDKUI_CameraHelper(SDKUI_CameraHelper&&) = delete;
    SDKUI_CameraHelper& operator=(SDKUI_CameraHelper&&) = delete;
    ~SDKUI_CameraHelper(void) = default;

    void Draw(void);
    inline void Show(void) { this->bShow = true; }
    inline void Hide(void) { this->bShow = false; }
    inline bool GetVisible(void) const { return this->bShow; }

private:
    bool bShow = true;
};
