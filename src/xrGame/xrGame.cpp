////////////////////////////////////////////////////////////////////////////
//	Module 		: xrGame.cpp
//	Created 	: 07.01.2001
//  Modified 	: 27.05.2004
//	Author		: Aleksandr Maksimchuk and Oles' Shyshkovtsov
//	Description : Defines the entry point for the DLL application.
////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "object_factory.h"
#include "xrUICore/XML/xrUIXmlParser.h"
#include "xr_level_controller.h"
#include "xrEngine/profiler.h"

extern void FillUIStyleToken();
extern void CleanupUIStyleToken();

extern "C" {
DLL_API IFactoryObject* __cdecl xrFactory_Create(CLASS_ID clsid)
{
    IFactoryObject* object = object_factory().client_object(clsid);
#ifdef DEBUG
    if (!object)
        return (0);
#endif
    // XXX nitrocaster XRFACTORY: set clsid during factory initialization
    object->GetClassId() = clsid;
    return (object);
}

DLL_API void __cdecl xrFactory_Destroy(IFactoryObject* O) { xr_delete(O); }

__declspec(dllexport) CSE_Abstract* __cdecl xrServer_Create(LPCSTR section, CSE_Motion*& motion, CSE_Visual*& visual)
{
    IServerEntity* obj = object_factory().server_object(pSettings->r_clsid(section, "class"), section);
    motion = obj->motion();
    visual = obj->visual();
    return (CSE_Abstract*)obj;
}

__declspec(dllexport) void __cdecl xrServer_Destroy(IServerEntity*& entity)
{
    auto object = smart_cast<CSE_Abstract*>(entity);
    xr_delete(entity);
    entity = nullptr;
}

__declspec(dllexport) CSE_Motion* __cdecl xrServer_GetMotion(CSE_Abstract* object)
{
    if (!object)
        return nullptr;

    return object->motion();
}

__declspec(dllexport) CSE_Visual* __cdecl xrServer_GetVisual(CSE_Abstract* object)
{
    if (!object)
        return nullptr;

    return object->visual();
}
};

void CCC_RegisterCommands();

#ifdef LINUX
__attribute__((constructor))
#endif
static void
load(int argc, char** argv, char** envp)
{
    // Fill ui style token
    FillUIStyleToken();
    // register console commands
    CCC_RegisterCommands();
    // keyboard binding
    CCC_RegisterInput();
#ifdef DEBUG
// XXX nitrocaster PROFILER: temporarily disabled due to linkage issues
// g_profiler			= new CProfiler();
#endif
    gStringTable = new CStringTable();
    StringTable().Init();
}

#ifdef LINUX
__attribute__((destructor))
#endif
static void
unload()
{
    CleanupUIStyleToken();
    xr_delete(gStringTable);
}

#ifdef WINDOWS
BOOL APIENTRY DllMain(HANDLE hModule, u32 ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        load(0, nullptr, nullptr);
        break;
    }

    case DLL_PROCESS_DETACH:
    {
        unload();
        break;
    }
    }
    return (TRUE);
}
#endif
