/*
    [Author]: Lord
    [Date]: 16.02.2019
*/

#pragma once
#include "StdAfx.h"
#include "../xrCore/Imgui/imgui.h"
#include "../xrCore/Imgui/imgui_impl_sdl.h"
class SDK_Connection : public pureRender
{
private:
    SDK_Connection(void) {}

public:
    SDK_Connection(const SDK_Connection&) = delete;
    SDK_Connection& operator=(const SDK_Connection&) = delete;
    SDK_Connection(SDK_Connection&&) = delete;
    SDK_Connection& operator=(SDK_Connection&&) = delete;
    ~SDK_Connection(void) {}

    static SDK_Connection* GetInstance(void)
    {
        if (instance == nullptr)
        {
            instance = new SDK_Connection;
        }

        return instance;
    }

    inline void Connect(void)
    {
        if (!bActivated)
        {
            Device.seqRender.Add(this, REG_PRIORITY_HIGH);
            bActivated = true;
        }
    }

    inline void DisConnect(void)
    {
        if (bActivated)
        {
            Device.seqRender.Remove(this);
            bActivated = false;
        }
    }

    void OnRender(void) override { GEnv.Render->Render(); }

private:
    bool bActivated = false;
    static SDK_Connection* instance;
};
