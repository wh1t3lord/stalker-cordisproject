#include "stdafx.h"
#include "Script_SE_SmartTerrain.h"

CInifile ini_file_locations = CInifile("misc\\smart_terrain_masks.ltx");

namespace Cordis
{
namespace Scripts
{
bool is_job_available_to_npc(const NpcInfo& npc_info, const GulagGenerator::JobData::SubData& job_info,
    const bool& is_monster, Script_SE_SmartTerrain* smart)
{
    if (!smart)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    // @ Lord: убедить что будет выполняться оригинальное условие что объект вообще был инициализирован после default
    // инициализации, то есть xrTime() -> xrTime().setSomeValue();
    if (smart->getDeadTime()[job_info.m_job_index] > 0)
        return false;

    if (is_monster != npc_info.m_is_monster)
        return false;

    if (job_info.m_function)
    {
        if (job_info.m_function(npc_info.m_server_object, smart, job_info.m_function_params, npc_info))
            return false;
    }

    return true;
}

} // namespace Scripts
} // namespace Cordis

namespace Cordis
{
namespace Scripts
{
Script_SE_SmartTerrain::Script_SE_SmartTerrain(LPCSTR section) : inherited(section) {}

Script_SE_SmartTerrain::~Script_SE_SmartTerrain(void)
{
    if (this->m_base_on_actor_control)
        delete this->m_base_on_actor_control;
}
void Script_SE_SmartTerrain::read_params(void) {}
} // namespace Scripts
} // namespace Cordis
