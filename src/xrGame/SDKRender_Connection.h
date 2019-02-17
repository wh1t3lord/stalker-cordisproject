/*
    [Author]: Lord
    [Date]: 16.02.2019
*/

#pragma once
#include "stdafx.h"

class SDK_Connection : public pureRender, public pureFrame
{
private:
    SDK_Connection(void) { }

public:
    SDK_Connection(SDK_Connection&) = delete;
    SDK_Connection& operator=(SDK_Connection const&) = delete;
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
    void OnFrame(void) override { ; }

private:
    bool bActivated = false;
    static SDK_Connection* instance;
};
