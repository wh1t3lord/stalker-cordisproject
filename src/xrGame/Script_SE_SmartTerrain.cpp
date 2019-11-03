#include "stdafx.h"
#include "Script_SE_SmartTerrain.h"
#include "Script_GulagGenerator.h"

CScriptIniFile ini_file_locations = CScriptIniFile("misc\\smart_terrain_masks.ltx");

namespace Cordis
{
namespace Scripts
{
bool is_job_available_to_npc(
    const NpcInfo& npc_info, const JobData_SubData& job_info, const bool& is_monster, Script_SE_SmartTerrain* smart)
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

inline void job_iterator(
    JobData& jobs, NpcInfo& npc_data, std::uint32_t selected_job_priority, Script_SE_SmartTerrain* smart)
{
    if (!smart)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }
}

} // namespace Scripts
} // namespace Cordis

namespace Cordis
{
namespace Scripts
{
Script_SE_SmartTerrain::Script_SE_SmartTerrain(LPCSTR section)
    : inherited(section), m_is_initialized(false), m_is_registered(false), m_population(0), m_smart_showed_spot_name("")
{
    Msg("[Scripts/Script_SE_SmartTerrain/ctor(section)] %s", section);
}

Script_SE_SmartTerrain::~Script_SE_SmartTerrain(void)
{
 //   if (this->m_base_on_actor_control) Lord: точно не должно удаляться?
  //      delete this->m_base_on_actor_control;
}

void Script_SE_SmartTerrain::on_before_register(void)
{
    inherited::on_before_register();
    Script_SimulationBoard::getInstance().register_smart(this);
    this->m_smart_level =
        *(ai().game_graph().header().level(ai().game_graph().vertex(this->m_tGraphID)->level_id()).name());
}

void Script_SE_SmartTerrain::on_register(void)
{
    inherited::on_register();

    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);
    Script_SimulationObjects::getInstance().registrate(this);

    Msg("[Scripts/Script_SE_SmartTerrain/on_register()] register smart %s", this->name_replace());

    Msg("[Scripts/Script_SE_SmartTerrain/on_register()] Returning alife task for object [%d] game_vertex [%d] "
        "level_vertex [%d] position %f %f %f",
        this->ID, this->m_tGraphID, this->m_tNodeID, this->o_Position.x, this->o_Position.y,
        this->o_Position.z);

    this->m_smart_alife_task =
        std::make_unique<CALifeSmartTerrainTask>(this->m_tGraphID, this->m_tNodeID);

    Script_GlobalHelper::getInstance().setGameRegisteredServerSmartTerrainsByName(this->name_replace(), this);
    this->m_is_registered = true;

    this->load_jobs();

    Script_SimulationBoard::getInstance().init_smart(this);

    if (this->m_is_need_init_npc)
    {
        this->m_is_need_init_npc = false;
        // Lord: реализовать метод
        // this->init_npc_after_load();
    }
}

void Script_SE_SmartTerrain::on_unregister(void) {}

void Script_SE_SmartTerrain::read_params(void) {}
void Script_SE_SmartTerrain::on_after_reach(Script_SE_SimulationSquad* squad)
{
    if (!squad)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
             squad->squad_members().begin();
         it != squad->squad_members().end(); ++it)
    {
        if ((*it).second)
        {
            Script_SimulationBoard::getInstance().setup_squad_and_group((*it).second->cast_alife_dynamic_object());
        }
    }

    squad->setCurrentTargetID(this->ID);
}

void Script_SE_SmartTerrain::on_reach_target(Script_SE_SimulationSquad* squad)
{
    if (!squad)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    squad->set_location_types(this->name_replace());

    Script_SimulationBoard::getInstance().assigned_squad_to_smart(squad, this->ID);

    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
             squad->squad_members().begin();
         it != squad->squad_members().end(); ++it)
    {
        if (DataBase::Storage::getInstance().getOfflineObjects().at(it->first).second.size())
        {
            DataBase::Storage::getInstance().setOfflineObjects(it->first, Globals::kUnsignedInt16Undefined, "");
        }
    }
}

void Script_SE_SmartTerrain::clear_dead(CSE_ALifeDynamicObject* server_object)
{
    if (!server_object)
    {
        R_ASSERT2(false, "object was nullptr!");
        return;
    }

    if (this->m_npc_info[server_object->ID].m_job_link.m_job_index != Globals::kUnsignedInt32Undefined)
    {
        this->m_dead_time[this->m_npc_info[server_object->ID].m_job_link.m_job_index] = Globals::Game::get_game_time();
        this->m_npc_info[server_object->ID].clear();
        CSE_ALifeMonsterAbstract* object = server_object->cast_monster_abstract();

        if (!object)
        {
            R_ASSERT2(false, "bad cast!");
            return;
        }

        object->m_smart_terrain_id = 0xffff;
        return;
    }
    else
    {
        R_ASSERT2(false, "something went wrong! Can't be here that this->m_npc_info = unInitialize");
        return;
    }

    if (this->m_arriving_npc[server_object->ID])
    {
        this->m_arriving_npc[server_object->ID] = nullptr;
        CSE_ALifeMonsterAbstract* object = server_object->cast_monster_abstract();

        if (!object)
        {
            R_ASSERT2(false, "bad cast!");
            return;
        }

        object->m_smart_terrain_id = 0xffff;
        //   return;
    }
}

void Script_SE_SmartTerrain::hide(void)
{
    if (!this->m_smart_showed_spot_name.size())
        return;

    xr_string spot_type_name = "alife_presentation_smart_";
    spot_type_name += this->m_simulation_type_name;
    spot_type_name += "_";
    spot_type_name += this->m_smart_showed_spot_name;

    Globals::Game::level::map_remove_object_spot(this->ID, spot_type_name.c_str());
}

void Script_SE_SmartTerrain::show(void)
{
    std::uint32_t time = Device.dwTimeGlobal;

    if (this->m_show_time && this->m_show_time + 200 >= time)
        return;

    xr_string player = this->m_player_name;
    xr_string spot_name = Globals::kRelationsTypeNeutral;

    if (!this->IsSimulationAvailable() &&
        XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this,
            this->getSimulationAvail()) == XR_LOGIC::kXRLogicReturnTypeSuccessfulName)
        spot_name = "friend";
    else
        spot_name = "enemy";

    if (this->m_smart_showed_spot_name == spot_name)
    {
        xr_string spot_type_name = "alife_presentation_smart_";
        spot_type_name += this->m_simulation_type_name;
        spot_type_name += "_";
        spot_type_name += this->m_smart_showed_spot_name;
        Globals::Game::level::map_change_spot_hint(this->ID, spot_type_name.c_str(), "");
        return;
    }

    xr_string spot_type_name = "alife_presentation_smart_";
    spot_type_name += this->m_simulation_type_name;
    spot_type_name += "_";
    spot_type_name += this->m_smart_showed_spot_name;

    if (this->m_smart_showed_spot_name.size() &&
        Globals::Game::level::map_has_object_spot(this->ID, spot_type_name.c_str()))
    {
        xr_string spot_type_name2 = "alife_presentation_smart_base_";
        spot_type_name2 += this->m_smart_showed_spot_name;
        Globals::Game::level::map_remove_object_spot(this->ID, spot_type_name2.c_str());
    }
}

void Script_SE_SmartTerrain::load_jobs(void)
{
    std::pair<xr_vector<JobData>, xr_vector<JobDataExclusive>> jobs = GulagGenerator::load_job(this);
}

} // namespace Scripts
} // namespace Cordis
