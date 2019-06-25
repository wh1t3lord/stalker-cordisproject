////////////////////////////////////////////////////////////////////////////
//  Module      : xrSE_Factory.cpp
//  Created     : 18.06.2004
//  Modified    : 18.06.2004
//  Author      : Dmitriy Iassenev
//  Description : Precompiled header creatore
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xrSE_Factory.h"
#include "ai_space.h"
#include "xrCore/ModuleLookup.hpp"
#include "xrScriptEngine/script_engine.hpp"
#include "xrServerEntities/object_factory.h"
#include "xrServerEntities/character_info.h"
#include "xrServerEntities/specific_character.h"
#include "xrSE_Factory_import_export.h"
#include "script_properties_list_helper.h"

extern CSE_Abstract* F_entity_Create(LPCSTR section);

extern CScriptPropertiesListHelper* g_property_list_helper;
extern XRay::Module prop_helper_module;

extern "C" {
FACTORY_API IServerEntity* __stdcall create_entity(LPCSTR section) { return F_entity_Create(section); }
FACTORY_API void __stdcall destroy_entity(IServerEntity*& abstract)
{
    auto object = smart_cast<CSE_Abstract*>(abstract);
    F_entity_Destroy(object);
    abstract = 0;
}
};
static bool was_initialized_dll = false; // Lord-Remark: это создано для обхода повторной инициализации, потому что мы всё же лоадим xrGame.dll
static bool was_deinitialized_dll = false; // Lord-Remark: Аналогично выше
BOOL APIENTRY DllMain(HANDLE module_handle, DWORD call_reason, LPVOID reserved)
{
    switch (call_reason)
    {
    case DLL_PROCESS_ATTACH:
    {
        //xrDebug::Initialize();
        //Core.Initialize("xrSE_Factory", nullptr, true, "fsfactory.ltx");
		if (!was_initialized_dll)
		{
			string_path SYSTEM_LTX;
			FS.update_path(SYSTEM_LTX, "$game_config$", "system.ltx");
			pSettings = new CInifile(SYSTEM_LTX);

			CCharacterInfo::InitInternal();
			CSpecificCharacter::InitInternal();
			was_initialized_dll = true;
		}
        break;
    }
    case DLL_PROCESS_DETACH:
    {
		if (!was_deinitialized_dll)
		{
			CCharacterInfo::DeleteSharedData();
			CCharacterInfo::DeleteIdToIndexData();
			CSpecificCharacter::DeleteSharedData();
			CSpecificCharacter::DeleteIdToIndexData();

			auto s = (CInifile**)&pSettings;
			xr_delete(*s);
			xr_delete(g_property_list_helper);
			xr_delete(g_ai_space);
			xr_delete(g_object_factory);
			prop_helper_module = nullptr;
			was_deinitialized_dll = true;
		}

        //Core._destroy();
        break;
    }
    }
    return (TRUE);
}

void _destroy_item_data_vector_cont(T_VECTOR* vec)
{
    auto it = vec->begin();
    auto it_e = vec->end();

    xr_vector<CUIXml*> _tmp;
    for (; it != it_e; ++it)
    {
        auto it_f = std::find(_tmp.begin(), _tmp.end(), (*it)._xml);
        if (it_f == _tmp.end())
            _tmp.push_back((*it)._xml);
    }
    delete_data(_tmp);
}
