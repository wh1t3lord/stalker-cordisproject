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
    : inherited(section), m_is_initialized(false), m_is_registered(false), m_population(0),
      m_smart_showed_spot_name(""), m_is_disabled(false), m_is_respawn_point(true), m_base_on_actor_control(nullptr)
{
    Msg("[Scripts/Script_SE_SmartTerrain/ctor(section)] %s", section);
}

Script_SE_SmartTerrain::~Script_SE_SmartTerrain(void)
{
    // Lord: реализовать удаление ini в JobDataExclusive_SubData из gulaggenerator, смотри в this->m_jobs
    {
        xr_string previous_section_name;
        for (JobDataExclusive* it : this->m_jobs.second)
        {
            it->m_job_id.setDeallocationChecker(previous_section_name);

            previous_section_name = it->m_job_id.m_section_name;
            if (it->m_job_id.m_ini_file)
            {
                Msg("[Scripts/Script_SE_SmartTerrain/~dtor()] deleting m_ini_file from %s",
                    it->m_job_id.m_section_name.c_str());
                xr_delete(it);
            }
        }
    }
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
        this->ID, this->m_tGraphID, this->m_tNodeID, this->o_Position.x, this->o_Position.y, this->o_Position.z);

    this->m_smart_alife_task = std::make_unique<CALifeSmartTerrainTask>(this->m_tGraphID, this->m_tNodeID);

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

void Script_SE_SmartTerrain::STATE_Read(NET_Packet& packet, u16 size)
{
    inherited::STATE_Read(packet, size);

    if (FS.IsSDK())
        return;

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, false, "Script_SE_SmartTerrain");
    this->read_params();

    std::uint8_t n = packet.r_u8();

    // Информация о НПС идущих в смарт
    for (std::uint8_t i = 0; i < n; ++i)
    {
        std::uint16_t npc_id = packet.r_u16();
        this->m_arriving_npc[npc_id] = nullptr;
    }

    // ИНформация о НПС в смарте
    n = packet.r_u8();
    for (std::uint16_t i = 0; i < n; ++i)
    {
        std::uint16_t npc_id = packet.r_u16();
        NpcInfo npc_info;
        npc_info.m_job_prioprity = packet.r_u8();
        if (npc_info.m_job_prioprity == Globals::kUnsignedInt8Undefined)
        {
            npc_info.m_job_prioprity = -1;
        }

        npc_info.m_job_id = packet.r_u8();
        if (npc_info.m_job_id == Globals::kUnsignedInt8Undefined)
        {
            npc_info.m_job_id = -1;
        }

        npc_info.m_begin_job = packet.r_u8() ? true : false;
        packet.r_stringZ(npc_info.m_need_job);

        this->m_npc_info[npc_id] = npc_info;
    }

    n = packet.r_u8();

    for (std::uint16_t i = 0; i < n; ++i)
    {
        std::uint8_t job_id = packet.r_u8();
        xrTime time = Globals::Utils::r_CTime(packet);
        this->m_dead_time[job_id] = time;
    }

    this->m_is_need_init_npc = true;

    if (this->m_script_version > 9)
    {
        if (!!packet.r_u8() == true)
        {
            this->m_base_on_actor_control->load(packet);
        }
    }

    bool is_respawn_point = packet.r_u8() ? true : false;

    if (is_respawn_point)
    {
        n = packet.r_u8();

        for (std::uint16_t i = 0; i < n; ++i)
        {
            xr_string id;
            packet.r_stringZ(id);
            std::uint8_t num = packet.r_u8();

            this->m_already_spawned[id] = num;
        }

        if (this->m_script_version > 11)
        {
            bool is_exist = packet.r_u8() ? true : false;

            if (is_exist)
            {
                this->m_last_respawn_update = Globals::Utils::r_CTime(packet);
            }
            else
            {
                this->m_last_respawn_update = xrTime();
                Msg("[Scripts/Script_SE_SmartTerrain/STATE_Read(packet, size)] this->m_last_respawn_update = "
                    "xrTime()!");
            }
        }
    }

    this->m_population = packet.r_u8();

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, true, "Script_SE_SmartTerrain");
}

void Script_SE_SmartTerrain::STATE_Write(NET_Packet& packet)
{
    inherited::STATE_Write(packet);

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, false, "Script_SE_SmartTerrain");

    packet.w_u8(this->m_arriving_npc.size());

    for (const auto& it : this->m_arriving_npc)
        packet.w_u16(it.first);

    packet.w_u8(this->m_npc_info.size());

    for (const auto& it : this->m_npc_info)
    {
        packet.w_u16(it.first);
        packet.w_u8(it.second.m_job_prioprity);
        packet.w_u8(it.second.m_job_id);
        packet.w_u8(it.second.m_begin_job ? true : false);
        packet.w_stringZ(it.second.m_need_job.c_str());
    }

    packet.w_u8(this->m_dead_time.size());

    for (auto& it : this->m_dead_time)
    {
        packet.w_u8(it.first);
        Globals::Utils::w_CTime(packet, it.second);
    }

    if (this->m_base_on_actor_control)
    {
        packet.w_u8(1);
        this->m_base_on_actor_control->save(packet);
    }
    else
    {
        packet.w_u8(0);
    }

    if (this->m_is_respawn_point)
    {
        packet.w_u8(1);
        packet.w_u8(this->m_already_spawned.size());

        for (auto& it : this->m_already_spawned)
        {
            packet.w_stringZ(it.first.c_str());
            packet.w_u8(it.second);
        }

        if (!(this->m_last_respawn_update == xrTime()))
        {
            packet.w_u8(1);
            Globals::Utils::w_CTime(packet, this->m_last_respawn_update);
        }
        else
        {
            packet.w_u8(0);
        }
    }
    else
    {
        packet.w_u8(0);
    }

    if (this->m_population < 0)
    {
        R_ASSERT2(false, "can't be less than zero!");
    }

    packet.w_u8(this->m_population);

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, true, "Script_SE_SmartTerrain");
}

void Script_SE_SmartTerrain::read_params(void)
{
    CInifile& ini = this->spawn_ini();
    if (!ini.section_exist(Globals::kSmartTerrainSMRTSection))
    {
        R_ASSERT2(false, "YOU LOGIC MUST CONTAIN SECTION [smart_terrain]");
        this->m_is_disabled = true;
        return;
    }

    xr_string filename = Globals::Utils::cfg_get_string(&ini, Globals::kSmartTerrainSMRTSection, "cfg");
    if (!filename.empty())
    {
        if (FS.exist("$game_config$", filename.c_str()))
        {
            this->m_ini = std::make_unique<CScriptIniFile>(filename.c_str());
        }
        else
        {
            R_ASSERT2(false, "YOU MUST REGISTRATE CONFIGURATION FILE OF YOUR SMART TERRAIN");
            this->m_is_disabled = true;
            return;
        }
    }

    CScriptIniFile* script_ini = this->getIni();

    if (script_ini)
        this->m_simulation_type_name =
            Globals::Utils::cfg_get_string(script_ini, Globals::kSmartTerrainSMRTSection, "sim_type");
    else
        this->m_simulation_type_name = "default";

    if (this->m_simulation_type_name.empty())
        this->m_simulation_type_name = "default";

    if (Script_GlobalHelper::getInstance().getRegisteredSmartTerrainTerritoryType().find(
            this->m_simulation_type_name) ==
        Script_GlobalHelper::getInstance().getRegisteredSmartTerrainTerritoryType().end())
    {
        Msg("[Scripts/Script_SE_SmartTerrain/read_params()] ERROR: %s", this->m_simulation_type_name.c_str());
        R_ASSERT2(false, "Invalid type of territory check the getRegisteredSmartTerrainTerritoryType()!");
    }

    if (script_ini)
        this->m_squad_id = static_cast<std::uint16_t>(
            Globals::Utils::cfg_get_number(script_ini, Globals::kSmartTerrainSMRTSection, "squad_id"));
    else
        this->m_squad_id = 0;

    xr_string respawn_sector_name;
    if (script_ini)
        respawn_sector_name =
            Globals::Utils::cfg_get_string(script_ini, Globals::kSmartTerrainSMRTSection, "respawn_sector");

    if (script_ini)
        this->m_respawn_radius = static_cast<std::uint16_t>(
            Globals::Utils::cfg_get_number(script_ini, Globals::kSmartTerrainSMRTSection, "respawn_radius"));
    else
        this->m_respawn_radius = 150;

    if (!respawn_sector_name.empty())
    {
        if (respawn_sector_name == "default")
        {
            respawn_sector_name = "all";
        }

        this->m_respawn_sector_condlist = XR_LOGIC::parse_condlist_by_server_object(
            Globals::kSmartTerrainSMRTSection, "respawn_sector", respawn_sector_name);
    }

    this->m_is_mutant_lair = script_ini ?
        Globals::Utils::cfg_get_bool(script_ini, Globals::kSmartTerrainSMRTSection, "mutant_lair") :
        Globals::Utils::cfg_get_bool(&ini, Globals::kSmartTerrainSMRTSection, "mutant_lair");
    this->m_is_no_mutant = script_ini ?
        Globals::Utils::cfg_get_bool(script_ini, Globals::kSmartTerrainSMRTSection, "no_mutant") :
        Globals::Utils::cfg_get_bool(&ini, Globals::kSmartTerrainSMRTSection, "no_mutant");

    if (this->m_is_no_mutant)
        Msg("[Scripts/Script_SE_SmartTerrain/read_params()] Found no mutant point %s", this->name_replace());

    this->m_fobidden_point_name = script_ini ?
        Globals::Utils::cfg_get_string(script_ini, Globals::kSmartTerrainSMRTSection, "forbidden_point") :
        Globals::Utils::cfg_get_string(&ini, Globals::kSmartTerrainSMRTSection, "forbidden_point");

    this->m_defence_restictor = script_ini ?
        Globals::Utils::cfg_get_string(script_ini, Globals::kSmartTerrainSMRTSection, "def_restr") :
        Globals::Utils::cfg_get_string(&ini, Globals::kSmartTerrainSMRTSection, "def_restr");
    this->m_attack_restrictor = script_ini ?
        Globals::Utils::cfg_get_string(script_ini, Globals::kSmartTerrainSMRTSection, "att_restr") :
        Globals::Utils::cfg_get_string(&ini, Globals::kSmartTerrainSMRTSection, "att_restr");
    this->m_safe_restirctor = script_ini ?
        Globals::Utils::cfg_get_string(script_ini, Globals::kSmartTerrainSMRTSection, "safe_restr") :
        Globals::Utils::cfg_get_string(&ini, Globals::kSmartTerrainSMRTSection, "safe_restr");
    this->m_spawn_point_name = script_ini ?
        Globals::Utils::cfg_get_string(script_ini, Globals::kSmartTerrainSMRTSection, "spawn_point") :
        Globals::Utils::cfg_get_string(&ini, Globals::kSmartTerrainSMRTSection, "spawn_point");
    this->m_arrive_distance = static_cast<std::uint32_t>(script_ini ?
            Globals::Utils::cfg_get_number(script_ini, Globals::kSmartTerrainSMRTSection, "arrive_dist") :
            Globals::Utils::cfg_get_number(&ini, Globals::kSmartTerrainSMRTSection, "arrive_dist"));

    if (this->m_arrive_distance < 0)
        this->m_arrive_distance = 30;

    xr_string max_population_name = script_ini ?
        Globals::Utils::cfg_get_string(script_ini, Globals::kSmartTerrainSMRTSection, "max_population") :
        Globals::Utils::cfg_get_string(&ini, Globals::kSmartTerrainSMRTSection, "max_population");
    xr_map<std::uint32_t, CondlistData> parsed_condlist = XR_LOGIC::parse_condlist_by_server_object(
        Globals::kSmartTerrainSMRTSection, "max_population", max_population_name);

    this->m_max_population = atoi(XR_LOGIC::pick_section_from_condlist(
        Globals::get_story_object("actor"), (CSE_ALifeDynamicObject*)(nullptr), parsed_condlist)
                                      .c_str());

    xr_string respawn_params = script_ini ?
        Globals::Utils::cfg_get_string(script_ini, Globals::kSmartTerrainSMRTSection, "respawn_params") :
        Globals::Utils::cfg_get_string(&ini, Globals::kSmartTerrainSMRTSection, "respawn_params");
    this->m_is_respawn_only_smart = script_ini ?
        Globals::Utils::cfg_get_bool(script_ini, Globals::kSmartTerrainSMRTSection, "respawn_only_smart") :
        Globals::Utils::cfg_get_bool(&ini, Globals::kSmartTerrainSMRTSection, "respawn_only_smart");

    xr_string smart_control_section_name = script_ini ?
        Globals::Utils::cfg_get_string(script_ini, Globals::kSmartTerrainSMRTSection, "smart_control") :
        Globals::Utils::cfg_get_string(&ini, Globals::kSmartTerrainSMRTSection, "smart_control");

    if (!(smart_control_section_name.empty()))
    {
        if (this->m_base_on_actor_control)
        {
            R_ASSERT2(false, "something goes wrong. The object must be empty (NULL)!!");
        }

        if (script_ini)
            this->m_base_on_actor_control =
                std::make_unique<Script_SmartTerrainControl>(this, *script_ini, smart_control_section_name);
        else
            this->m_base_on_actor_control =
                std::make_unique<Script_SmartTerrainControl>(this, this->spawn_ini(), smart_control_section_name);
    }

    this->m_is_respawn_point = false;

    if (!(respawn_params.empty()))
    {
        this->check_respawn_params(respawn_params);
    }

    xr_string traveller_actor_name = this->name_replace();
    traveller_actor_name += "_traveller_actor";

    xr_string traveller_squad_name = this->name_replace();
    traveller_squad_name += "_traveller_squad";

    if (Globals::patrol_path_exists(traveller_actor_name.c_str()))
    {
        Msg("[Scripts/Script_SE_SmartTerrain/read_params()] %s has no traveller path traveller_actor_name",
            this->name_replace());
        this->m_traveller_actor_path_name = traveller_actor_name;
    }

    if (Globals::patrol_path_exists(traveller_squad_name.c_str()))
    {
        Msg("[Scripts/Script_SE_SmartTerrain/read_params()] %s has no traveller path traveller_squad_name",
            this->name_replace());
        this->m_traveller_squad_path_name = traveller_squad_name;
    }

    if (!ini_file_locations.section_exist(this->name_replace()))
    {
        Msg("[Scripts/Script_SE_SmartTerrain/read_params()] %s has no terrain_mask section in smart_terrain_masks.ltx!",
            this->name_replace());
    }
}

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

void Script_SE_SmartTerrain::check_respawn_params(xr_string& params)
{
    if (params.empty())
    {
        Msg("[Scripts/Script_SE_SmartTerrain/check_respawn_params(params)] WARNING: params.empty() == true! Return");
        return;
    }

    if (this->getIni())
    {
        if (!this->getIni()->section_exist(params.c_str()))
        {
            R_ASSERT2(false, "INVALID: Wrong smart_terrain respawn_params section!");
            return;
        }
    }
    else
    {
        if (!this->spawn_ini().section_exist(params.c_str()))
        {
            R_ASSERT2(false, "INVALID: Wrong smart_terrain respawn_params section!");
            return;
        }
    }

    std::uint32_t n =
        this->getIni() ? this->getIni()->line_count(params.c_str()) : this->spawn_ini().line_count(params.c_str());

    if (!n)
    {
        R_ASSERT2(false, "Wrong smart_terrain empty params!");
    }

    for (std::uint32_t i = 0; i < n; ++i)
    {
        const char* prop_name;
        const char* prop_condlist;
        bool status = this->getIni() ? this->getIni()->r_line(params.c_str(), i, &prop_name, &prop_condlist) :
                                       this->spawn_ini().r_line(params.c_str(), i, &prop_name, &prop_condlist);

        if (this->getIni())
        {
            if (!this->getIni()->section_exist(prop_name))
            {
                R_ASSERT2(false, "Wrong smart_terrain params no section!");
                return;
            }
        }
        else
        {
            if (!this->spawn_ini().section_exist(prop_name))
            {
                R_ASSERT2(false, "Wrong smart_terrain params no section!");
                return;
            }
        }

        xr_string spawn_squads;
        xr_string spawn_num;
        spawn_squads = this->getIni() ? Globals::Utils::cfg_get_string(this->getIni(), prop_name, "spawn_squads") :
                                        Globals::Utils::cfg_get_string(this->spawn_ini(), prop_name, "spawn_squads");

        spawn_num = this->getIni() ? Globals::Utils::cfg_get_string(this->getIni(), prop_name, "spawn_num") :
                                     Globals::Utils::cfg_get_string(this->spawn_ini(), prop_name, "spawn_num");

        if (spawn_squads.empty())
        {
            R_ASSERT2(false, "Something goes worng!~");
            return;
        }
        else if (spawn_num.empty())
        {
            R_ASSERT2(false, "Something goes wrong!");
            return;
        }

        xr_vector<xr_string> parsed_squads_name = Globals::Utils::parse_names(spawn_squads);
        xr_map<std::uint32_t, CondlistData> parsed_condlist =
            XR_LOGIC::parse_condlist_by_server_object(prop_name, "spawn_num", spawn_num);

        this->m_respawn_params[prop_name] =
            std::pair<xr_vector<xr_string>, xr_map<std::uint32_t, CondlistData>>(parsed_squads_name, parsed_condlist);
        this->m_already_spawned[prop_name] = 0;
    }
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
    this->m_jobs = GulagGenerator::load_job(this);

    auto sort_function = [](const std::pair<std::uint32_t, xr_vector<JobData_SubData>>& a,
                             const std::pair<std::uint32_t, xr_vector<JobData_SubData>>& b) -> bool {
        return (a.first > b.first);
    };

    // @ Sorting
    {
        for (JobData& it : this->m_jobs.first)
        {
            /*
                        for (std::pair<std::uint32_t, xr_vector<JobData_SubData>>& it_sub : it.m_jobs)
                        {
                            std::sort(it_sub.second.begin(), it_sub.second.end(), sort_function);
                        }*/
            std::sort(it.m_jobs.begin(), it.m_jobs.end(), sort_function);
        }
    }
}

} // namespace Scripts
} // namespace Cordis
