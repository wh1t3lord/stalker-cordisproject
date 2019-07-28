#pragma once

#include "xrCore/xr_ini.h"

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

struct CondlistData
{
    struct CondlistValues
    {
        bool m_required;
        bool m_expected;
        xr_string m_section_name;
        xr_string m_function_name;
    };


    xr_map<std::uint32_t, xr_map<xr_string, bool>> m_infop_check;
    xr_map<std::uint32_t, xr_map<xr_string, bool>> m_infop_set;


};

inline xr_map<std::uint32_t, CondlistData> parse_condlist(
    CSE_ALifeObject* npc, const xr_string& section, const xr_string& field, const xr_string& source)
{
}

inline xr_map<std::uint32_t, CondlistData> parse_condlist(
    CScriptGameObject* npc, const xr_string& section, const xr_string& field, const xr_string& source)
{
}

} // namespace XR_LOGIC
} // namespace Scripts
} // namespace Cordis
