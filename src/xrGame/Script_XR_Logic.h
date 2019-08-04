#pragma once

#include "xrCore/xr_ini.h"
#include "xrServer_Objects_ALife.h"

namespace Cordis
{
namespace Scripts
{
namespace XR_LOGIC
{
inline CInifile configure_schemes(CSE_ALifeObject* npc, const CInifile& ini, const xr_string& ini_filename,
    unsigned int stype, const xr_string& section_logic, const xr_string& gulag_name)
{
    if (!npc)
    {
        R_ASSERT2(false, "Object was null!");
        return CInifile("system.ltx");
    }

    std::uint16_t npc_id = npc->ID;

    // Lord: доделать!
    return CInifile("а это убрать и написать нормальный аргумент.ltx");
}

// @ Lord: doesn't have any differences between parse_condlist_by_script_object, but first argument is script object :D
inline xr_map<std::uint32_t, CondlistData> parse_condlist_by_server_object(
    CSE_ALifeObject* npc, const xr_string& section, const xr_string& field, const xr_string& source)
{
    // Lord: реализовать парсинг и доделать!
    if (!npc)
    {
        //   R_ASSERT2(false, "object is null!");
        // Lord: without any assertion cuz first argument may be nullptr
        Msg("[Script/XR_LOGIC/parse_condlist]: Warning 'npc' was nullptr!");
        /*        return xr_map<std::uint32_t, CondlistData>();*/
    }


    xr_map<std::uint32_t, CondlistData> result;





    return xr_map<std::uint32_t, CondlistData>();
}

// @ Lord: doesn't have any differences between parse_condlist_by_server_object, but first argument is CSE object :D
inline xr_map<std::uint32_t, CondlistData> parse_condlist_by_script_object(
    CScriptGameObject* npc, const xr_string& section, const xr_string& field, const xr_string& source)
{
    return xr_map<std::uint32_t, CondlistData>();
}

inline xr_string pick_section_from_condlist(
    CScriptGameObject* actor, CSE_ALifeObject* npc, const xr_map<std::uint32_t, CondlistData>& condlist)
{
    // Lord: доделать
    if (!actor)
    {
        R_ASSERT2(false, "object is null!");
        return xr_string("");
    }

    if (!npc)
    {
        R_ASSERT2(false, "object is null!");
        return xr_string("");
    }

    return xr_string("");
}

inline xr_string pick_section_from_condlist(
    CSE_ALifeCreatureActor* actor, CSE_ALifeObject* npc, const xr_map<std::uint32_t, CondlistData>& condlist)
{
    // Lord: доделать
    if (!actor)
    {
        R_ASSERT2(false, "object is null!");
        return xr_string("");
    }

    if (!npc)
    {
        R_ASSERT2(false, "object is null!");
        return xr_string("");
    }

    return xr_string("");
}
 
inline xr_string pick_section_from_condlist(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_map<std::uint32_t, CondlistData>& condlist)
{
    // Lord: доделать!
    if (!actor)
    {
        R_ASSERT2(false, "object is null!");
        return xr_string("");
    }

    if (!npc)
    {
        R_ASSERT2(false, "object is null!");
        return xr_string("");
    }

    return xr_string("");
}

} // namespace XR_LOGIC
} // namespace Scripts
} // namespace Cordis
