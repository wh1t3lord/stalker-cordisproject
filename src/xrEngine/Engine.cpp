// Engine.cpp: implementation of the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Engine.h"

CEngine Engine;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEngine::CEngine() {}
CEngine::~CEngine() {}
extern void msCreate(LPCSTR name);

void CEngine::Initialize(void)
{
    Cordis::TaskManager::getInstance().getCore()->run([&]() {    Engine.Sheduler.Initialize(); });

#ifdef DEBUG
    Cordis::TaskManager::getInstance().getCore()->run([&]() {msCreate("game"); });
#endif
}

void CEngine::Destroy()
{
    Engine.Sheduler.Destroy();
    Engine.Scheduler.Destroy();
    Engine.External.Destroy();
}
