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
    SDKUI_Log(void) noexcept : bShow(true), bSysCall(false), bShowErrors(true), bShowWarnings(true), bShowDefault(true), bShowSpecial(true),
          bShowUn(true), bShowGood(true), CurrentSizeX(0), CurrentSizeY(0), CurrentPosX(0), CurrentPosY(0), currentcolor(default)
    {
        this->Clear();
    }

public:
    inline static SDKUI_Log& Widget(void) noexcept
    {
        static SDKUI_Log instance;
        return instance;
    }
    SDKUI_Log(const SDKUI_Log&) = delete;
    SDKUI_Log& operator=(const SDKUI_Log&) = delete;
    SDKUI_Log(SDKUI_Log&&) = delete;
    SDKUI_Log& operator=(SDKUI_Log&&) = delete;
    ~SDKUI_Log(void) noexcept = default;

    void Draw(void);

    inline void Show(void) noexcept { this->bShow = true; }
    inline void Hide(void) noexcept { this->bShow = false; }
    inline bool GetVisible(void) const noexcept { return this->bShow; }
    inline void Init(int x, int y, int SizeX, int SizeY, ImGuiWindowFlags flag = NULL)
    {
        this->CurrentSizeX = SizeX;
        this->CurrentSizeY = SizeY;
        this->CurrentPosX = x;

        if (this->CurrentPosX - (this->CurrentSizeX) < 0)
            this->CurrentPosX = 0;
        else
            this->CurrentPosX -= this->CurrentSizeX;

        this->CurrentPosY = y;

        if (this->CurrentPosY - (this->CurrentSizeY) < 0)
            this->CurrentPosY = 0;
        else
            this->CurrentPosY -= this->CurrentSizeY;

        this->wndFlags = flag;
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
        this->buf.appendfv(fmt, args);
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
        this->buf.clear();
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
    bool bShow;
    bool bSysCall;

    bool bShowErrors;
    bool bShowWarnings;
    bool bShowDefault;
    bool bShowSpecial;
    bool bShowUn; // @ unimportant
    bool bShowGood;

    int CurrentSizeX;
    int CurrentSizeY;
    int CurrentPosX;
    int CurrentPosY;
    SDKErrorType currentcolor;
    ImGuiWindowFlags wndFlags;
    ImGuiTextBuffer buf;
    xr_vector<xr_string> buff;

    ImVector<int> LineOffSet;
};

class SDKUI_Overlay
{
private:
    SDKUI_Overlay(void) noexcept
        : bShow(true), bShowOnlySDKInfo(true), bShowOnlySysInfo(true), bShowOnlyProjectInfo(true),
          flag(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
              ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize |
              ImGuiWindowFlags_NoBackground),
          iGridType(0), vSavedPosition({0, 0, 0})
    {
    }

public:
    inline static SDKUI_Overlay& Widget(void) noexcept
    {
        static SDKUI_Overlay instance;
        return instance;
    }

    ~SDKUI_Overlay(void) noexcept = default;
    SDKUI_Overlay(SDKUI_Overlay&) = delete;
    SDKUI_Overlay& operator=(const SDKUI_Overlay&) = delete;
    SDKUI_Overlay(SDKUI_Overlay&&) = delete;
    SDKUI_Overlay& operator=(SDKUI_Overlay&&) = delete;
    void Draw(void);
    inline void Show(void) noexcept { this->bShow = true; }
    inline void Hide(void) noexcept { this->bShow = false; }
    inline bool GetVisible(void) const noexcept { return this->bShow; }

private:
    bool bShow;
    bool bShowOnlySDKInfo;
    bool bShowOnlySysInfo;
    bool bShowOnlyProjectInfo;
    int iGridType;
    ImGuiWindowFlags flag;
    Fvector vSavedPosition;
};

class SDKUI_CameraHelper
{
private:
    SDKUI_CameraHelper(void) noexcept : bShow(false)
    {
        
    }

public:
    inline static SDKUI_CameraHelper& Widget(void) noexcept
    {
        static SDKUI_CameraHelper instance;
        return instance;
    }
    SDKUI_CameraHelper(const SDKUI_CameraHelper&) = delete;
    SDKUI_CameraHelper& operator=(const SDKUI_CameraHelper&) = delete;
    SDKUI_CameraHelper(SDKUI_CameraHelper&&) = delete;
    SDKUI_CameraHelper& operator=(SDKUI_CameraHelper&&) = delete;
    ~SDKUI_CameraHelper(void) noexcept = default;

    void Draw(void);
    inline void Show(void) noexcept { this->bShow = true; }
    inline void Hide(void) noexcept { this->bShow = false; }
    inline bool GetVisible(void) const noexcept { return this->bShow; }

private:
    bool bShow;
};
