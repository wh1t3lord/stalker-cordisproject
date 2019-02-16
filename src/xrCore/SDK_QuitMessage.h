/*
    [Author]: Lord
    [Date]: 16.02.2019
*/
#pragma once
#include "stdafx.h"

class XRCORE_API SDK_QuitMessage
{
private:
    SDK_QuitMessage() = delete;
    ~SDK_QuitMessage() = delete;

public:
    static void TellAppToClose(void) {if (!bQuit) bQuit = true; }
    static bool GetState(void) { return bQuit; }

private:
    static bool bQuit;
};
