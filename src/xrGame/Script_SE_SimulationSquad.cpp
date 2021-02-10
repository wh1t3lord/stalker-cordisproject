#include "stdafx.h"
#include "Script_SE_SimulationSquad.h"
#include "Script_XR_Logic.h"

CScriptIniFile squad_behavior_ini = CScriptIniFile("misc\\squad_behaviours.ltx");
CScriptIniFile locations_ini = CScriptIniFile("misc\\smart_terrain_masks.ltx");

namespace Cordis
{
    namespace Scripts
    {
		bool can_help_actor(Script_SE_SimulationSquad* const p_squad)
		{
			if (DataBase::Storage::getInstance().getXRCombatIgnoreFightingWithActorNpcs().empty())
				return false;

			if (Globals::Game::get_game_graph()->vertex(p_squad->m_tGraphID)->level_vertex_id() != Globals::Game::get_game_graph()->vertex(ai().alife().graph().actor()->m_tGraphID)->level_vertex_id())
			{
				return false;
			}
			const xr_string& community_name = Script_GlobalHelper::getInstance().getSquadCommunityByBehavior().at(p_squad->getPlayerIDName());

			if (community_name.empty())
			{
				R_ASSERT2(false, "something wrong!");
				return false;
			}

			if (Globals::has_alife_info("sim_duty_help_harder") && community_name == "duty")
			{
				return true;
			}
			else if (Globals::has_alife_info("sim_freedom_help_harder") && community_name == "freedom")
			{
				return true;
			}
			else if (Globals::has_alife_info("sim_stalker_help_harder") && community_name == "stalker")
			{
				return true;
			}

			return false;
		}

		std::uint16_t get_help_target_id(Script_SE_SimulationSquad* const p_squad)
		{
			if (!can_help_actor(p_squad))
				return 0;

			for (const std::pair<std::uint16_t, bool>& it : DataBase::Storage::getInstance().getXRCombatIgnoreFightingWithActorNpcs())
			{
				std::uint16_t enemy_squad_id = ai().alife().objects().object(0)->cast_monster_abstract()->m_group_id;

				if (enemy_squad_id)
				{
					Script_SE_SimulationSquad* const p_target_squad = ai().alife().objects().object(enemy_squad_id)->cast_script_se_simulationsquad();
					if (p_target_squad)
					{
						if (p_squad->position().distance_to_sqr(p_target_squad->position()) < 150.0f * 150.0f && Globals::GameRelations::is_factions_enemies(Script_GlobalHelper::getInstance().getSquadCommunityByBehavior().at(p_squad->getPlayerIDName()), Script_GlobalHelper::getInstance().getSquadCommunityByBehavior().at(p_target_squad->getPlayerIDName())))
						{
							return enemy_squad_id;
						}
					}
				}
			}

			return 0;
		}
    }
}


namespace Cordis
{
namespace Scripts
{
Script_SE_SimulationSquad::Script_SE_SimulationSquad(LPCSTR section)
    : inherited(section), m_assigned_target_id(0), m_current_spot_id(0), m_current_target_id(0), m_smart_terrain_id(0),
      m_settings_id_name(this->name()), m_entered_smart_id(0), m_is_need_free_update(false), m_player_id(0),
      m_next_target_index(0), m_is_need_to_reset_location_masks(false),
      m_sound_manager(Script_SoundManager::getSoundManager((xr_string("squad_").append(this->name()))))
{
    MESSAGE("%s", section);
    this->m_player_id_name =
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "faction");
    this->m_condlist_action = XR_LOGIC::parse_condlist_by_server_object("assign_action", "target_smart",
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "target_smart"));
    this->m_condlist_death = XR_LOGIC::parse_condlist_by_server_object("death_condlist", "on_death",
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "target_smart"));
    this->m_condlist_invulnerability = XR_LOGIC::parse_condlist_by_server_object("invulnerability", "invulnerability",
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "invulnerability"));
    this->m_relationship_name =
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "relationship");
    this->m_sympathy =
        Globals::Utils::cfg_get_number(Globals::get_system_ini(), this->m_settings_id_name, "sympathy", this);
    this->m_condlist_show_spot = XR_LOGIC::parse_condlist_by_server_object("show_spot", "show_spot",
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "show_spot"));
    this->m_is_always_arrived =
        Globals::Utils::cfg_get_bool(Globals::get_system_ini(), this->m_settings_id_name, "always_arrived", this);
    this->m_is_always_walk =
        Globals::Utils::cfg_get_bool(Globals::get_system_ini(), this->m_settings_id_name, "always_walk", this);
    this->set_location_types_section("stalker_terrain");
    this->set_squad_sympathy();
}

Script_SE_SimulationSquad::~Script_SE_SimulationSquad(void) {}

CALifeSmartTerrainTask* Script_SE_SimulationSquad::get_current_task(void)
{
    if (this->m_assigned_target_id && this->m_assigned_target_id != Globals::kUnsignedInt32Undefined)
    {
        CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(this->m_assigned_target_id);
        if (server_object)
        {
            if (server_object->cast_script_se_smartterrain())
                return server_object->cast_script_se_smartterrain()->getAlifeSmartTerrainTask();

            if (server_object->cast_script_se_actor())
                return server_object->cast_script_se_actor()->getAlifeSmartTerrainTask();
        }
    }

    return this->getAlifeSmartTerrainTask();
}

void Script_SE_SimulationSquad::STATE_Read(NET_Packet& packet, u16 size)
{
    inherited::STATE_Read(packet, size);

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, false, "Script_SE_SimulationSquad");

    xr_string current_target_id_data;
    packet.r_stringZ(current_target_id_data);

    if (current_target_id_data == "nil") // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
        this->m_current_target_id = 0;
    else
        this->m_current_target_id = atoi(current_target_id_data.c_str());

    xr_string respawn_point_id_data;
    packet.r_stringZ(respawn_point_id_data);

    if (respawn_point_id_data == "nil") // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
        this->m_respawn_point_id = 0;
    else
        this->m_respawn_point_id = atoi(respawn_point_id_data.c_str());

    packet.r_stringZ(this->m_respawn_point_prop_section_name);
    if (this->m_respawn_point_prop_section_name == "nil") // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
        this->m_respawn_point_prop_section_name.clear();

    xr_string smart_terrain_id_data;
    packet.r_stringZ(smart_terrain_id_data);

    if (smart_terrain_id_data == "nil") // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
        this->m_smart_terrain_id = 0;
    else
        this->m_smart_terrain_id = atoi(smart_terrain_id_data.c_str());

    this->init_squad_on_load();

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, true, "Script_SE_SimulationSquad");
}

void Script_SE_SimulationSquad::STATE_Write(NET_Packet& packet)
{
    inherited::STATE_Write(packet);

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, false, "Script_SE_SimulationSquad");

    packet.w_stringZ(std::to_string(this->m_current_target_id).c_str());
    packet.w_stringZ(std::to_string(this->m_respawn_point_id).c_str());
    packet.w_stringZ(this->m_respawn_point_prop_section_name.c_str());
    packet.w_stringZ(std::to_string(this->m_smart_terrain_id).c_str());

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, true, "Script_SE_SimulationSquad");
}

void Script_SE_SimulationSquad::on_register(void)
{
    inherited::on_register();
    Script_SimulationBoard::getInstance().setSquads(this->ID, this);
    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);
    Script_SimulationObjects::getInstance().registrate(this);
}

void Script_SE_SimulationSquad::on_unregister(void)
{
    Script_StoryObject::getInstance().unregistrate_by_id(this->ID);
    Script_SimulationBoard::getInstance().setSquads(this->ID, nullptr);
    Script_SimulationBoard::getInstance().assigned_squad_to_smart(this);
    inherited::on_unregister();
    Script_SimulationObjects::getInstance().unregistrate(this);

    if (this->m_respawn_point_id)
    {
        CSE_Abstract* const smart = ai().alife().objects().object(this->m_respawn_point_id, true);

        // smart_terrain удаляются раньше чем squadss
        if (smart == nullptr)
            return;

        Script_SE_SmartTerrain* const p_smart = smart->cast_script_se_smartterrain();
        
        if (p_smart == nullptr)
            return;

        std::uint8_t value = p_smart->getAlreadySpawned().at(this->m_respawn_point_prop_section_name);
        --value;
        p_smart->setAlreadySpawned(this->m_respawn_point_prop_section_name, value);
    }
}

void Script_SE_SimulationSquad::update()
{
    inherited::update();
    this->refresh();

    Script_SimulationObjects::getInstance().update_avaliability(this);

    std::uint16_t script_target_id = this->getScriptTarget();

    // Lord: не реализована проверка на бессмертие

    if (script_target_id == 0 || script_target_id == Globals::kUnsignedInt16Undefined)
    {
        this->generic_update();
        if (this->m_is_need_to_reset_location_masks)
        {
            this->set_location_types("");
            this->m_is_need_to_reset_location_masks = false;
        }
        return;
    }

    this->m_sound_manager.update();

    bool is_need_to_find_new_action = false;
    if (this->m_assigned_target_id && this->m_assigned_target_id == script_target_id)
    {
        if (this->m_current_action.getName().empty() == false)
        {
            if (this->m_current_action.getName() == Globals::kSimulationSquadCurrentActionIDStayOnTarget)
            {
                if (this->check_squad_come_to_point())
                {
                    is_need_to_find_new_action = true;
                }
                else
                {
                    is_need_to_find_new_action = this->update_current_action();
                }
            }
            else
            {
                if (this->update_current_action())
                {
                    this->check_squad_come_to_point();
                    is_need_to_find_new_action = true;
                }
            }
        }
        else
        {
            this->check_squad_come_to_point();
            is_need_to_find_new_action = true;
        }
    }
    else
    {
        if (this->m_current_action.getName().empty())
        {
            is_need_to_find_new_action = true;
        }
        else
        {
            is_need_to_find_new_action = true;
        }
    }

    if (is_need_to_find_new_action)
    {
        this->m_assigned_target_id = script_target_id;

        if (this->m_current_action.getName().empty() == false)
        {
            this->m_current_action.Clear();
        }

        this->get_next_action(false);
    }

    if (this->m_is_need_to_reset_location_masks)
    {
        this->set_location_types("");
        this->m_is_need_to_reset_location_masks = false;
    }
}

std::uint16_t Script_SE_SimulationSquad::getScriptTarget(void)
{
    xr_string new_target_name = XR_LOGIC::pick_section_from_condlist(
        DataBase::Storage::getInstance().getActor(), this, this->m_condlist_action);

    if (new_target_name.empty())
        return std::uint16_t(0);

    if (new_target_name != this->m_last_target_name)
    {
        this->m_last_target_name = new_target_name;
        this->m_parsed_targets = Globals::Utils::parse_names(new_target_name);

        if (!this->m_is_need_free_update)
            this->m_next_target_index = 0;
        else
            this->m_is_need_free_update = false;
    }

    if (this->m_next_target_index >= this->m_parsed_targets.size())
        this->m_next_target_index = 0;

    if (this->m_parsed_targets[this->m_next_target_index].empty())
        this->m_next_target_index = 0;

    xr_string _new_target = this->pick_next_target();

    if (_new_target == "nil") // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
        return 0;
    else if (_new_target == "loop")
    {
        this->m_next_target_index = 0;
        _new_target = this->pick_next_target();
    }

    Script_SE_SmartTerrain* smart = Script_SimulationBoard::getInstance().getSmartTerrainsByName().at(_new_target);

    if (!smart)
    {
        MESSAGEE("Incorrect next point [%s] for squad [%s]",
            _new_target.c_str(), std::to_string(this->ID).c_str());
        R_ASSERT(false);
        return std::uint16_t(0);
    }

    return smart->ID;
}

 

void Script_SE_SimulationSquad::assign_squad_member_to_smart(
    const std::uint16_t& object_id, Script_SE_SmartTerrain* smart, const std::uint16_t& old_smart_terrain_id)
{
    CSE_ALifeMonsterAbstract* server_monster = ai().alife().objects().object(object_id)->cast_monster_abstract();

    if (!server_monster)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (server_monster->m_smart_terrain_id == this->m_smart_terrain_id)
        return;

    if (server_monster->m_smart_terrain_id != Globals::kUnsignedInt16Undefined && old_smart_terrain_id &&
        (server_monster->m_smart_terrain_id == old_smart_terrain_id) &&
        (Script_SimulationBoard::getInstance().getSmarts().at(old_smart_terrain_id).getServerSmartTerrain()))
        Script_SimulationBoard::getInstance()
            .getSmarts()
            .at(old_smart_terrain_id)
            .getServerSmartTerrain()
            ->unregister_npc(server_monster);

    if (smart)
        smart->register_npc(server_monster);
}

bool Script_SE_SimulationSquad::assigned_target_available(void)
{
    bool is_target_object = this->m_assigned_target_id && ai().alife().objects().object(this->m_assigned_target_id);
    
    if (is_target_object == false)
        return false;

    CSE_ALifeDynamicObject* const p_server_object = ai().alife().objects().object(this->m_assigned_target_id);
    Script_SE_SmartTerrain* const p_smart = p_server_object->cast_script_se_smartterrain();
    
    if (p_smart)
    {
        return p_smart->target_precondition(this, true);
    }


    return false;
}

bool Script_SE_SimulationSquad::target_precondition(CSE_ALifeObject* squad)
{
    if (squad == nullptr)
    {
        MESSAGEWR("invalid squad passed!");
        return false;
    }

    Script_SE_SimulationSquad* const p_squad = squad->cast_script_se_simulationsquad();

    if (p_squad)
    {
        if (Script_SimulationBoard::getInstance().getSimulationActivities().find(p_squad->getPlayerID()) == Script_SimulationBoard::getInstance().getSimulationActivities().end())
        {
            MESSAGEWR("Can't find simulation activity for %d", p_squad->getPlayerID());
            return false;
        }

        const SimulationActivitiesType& activity = Script_SimulationBoard::getInstance().getSimulationActivities().at(p_squad->getPlayerID());

        if (activity.m_squad.empty())
        {
            MESSAGEWR("m_squad was empty!");
            return false;
        }

        if (activity.m_squad.find(this->m_player_id) == activity.m_squad.end())
        {
            MESSAGEWR("Can't find data in m_squad by ID -> %d", this->m_player_id);
            return false;
        }

        std::function<bool(CSE_ALifeOnlineOfflineGroup*, CSE_ALifeObject*)> precondition = activity.m_squad.at(this->m_player_id);

        if (precondition)
        {
            return precondition(p_squad, this);
        }
        else
        {
            MESSAGEWR("invalid precondition! Can't be check initialization in Script_SimulationBoard!");
            return false;
        }
    }
    
    MESSAGEWR("Can't cast to %s", typeid(p_squad).name());
    return false;
}

float Script_SE_SimulationSquad::evaluate_priority(Script_SE_SimulationSquad* p_squad)
{
    return Script_SimulationObjects::getInstance().evaluate_priority(this, p_squad);
}

void Script_SE_SimulationSquad::set_location_types_section(const xr_string& section)
{
    if (locations_ini.section_exist(section.c_str()))
    {
        const char* N;
        const char* V;

        if (locations_ini.r_line(section.c_str(), 0, &N, &V))
            this->add_location_type(N);
    }
}

void Script_SE_SimulationSquad::set_location_types(const xr_string& new_smart_name)
{
    xr_string default_location_name = "stalker_terrain";
    this->clear_location_types();

    if (ai().alife().objects().object(this->m_assigned_target_id)->script_clsid() ==
        Globals::get_script_clsid(CLSID_SE_SMART_TERRAIN))
    {
        this->set_location_types_section(default_location_name);
        xr_string old_smart_name = "";

        if (this->m_smart_terrain_id)
        {
            CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(this->m_smart_terrain_id);
            if (server_object)
            {
                old_smart_name = server_object->name_replace();
            }
        }

 
        MESSAGE("Old smart terrain name [%s]",
            old_smart_name.c_str());
 

        if (old_smart_name.empty() == false)
            this->set_location_types_section(old_smart_name);

        if (new_smart_name.empty() == false)
        {
			MESSAGE("New smart terrain name [%s]",
				new_smart_name.c_str());
            this->set_location_types_section(new_smart_name);
        }
    }
    else
    {
        MESSAGEI("The target is squad or actor setting "
            "[squad_terrain]!");

        this->set_location_types_section("squad_terrain");

        for (const std::pair<std::uint16_t, CSE_ALifeDynamicObject*>& it :
            Script_SimulationObjects::getInstance().getObjects())
        {
            CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(it.first);
            if (server_object)
            {
                if (server_object->script_clsid() == Globals::get_script_clsid(CLSID_SE_SMART_TERRAIN))
                {
                    xr_string properties_base = server_object->getProperties().at("base");
                    if (properties_base.size())
                        this->set_location_types_section(server_object->name_replace());
                }
            }
        }
    }
}

void Script_SE_SimulationSquad::create_npc(Script_SE_SmartTerrain* spawn_smart)
{
    if (!spawn_smart)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    xr_vector<xr_string> spawn_sections = Globals::Utils::parse_names(
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "npc"));

    xr_string spawn_point_name =
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "spawn_point").size() ?
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "spawn_point") :
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), Globals::kSmartTerrainSMRTSection, "spawn_point");

    xr_map<std::uint32_t, CondlistData> spawn_point_condlist =
        XR_LOGIC::parse_condlist_by_server_object("spawn_point", "spawn_point", spawn_point_name);

    xr_string spawn_point_section_name =
        XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this, spawn_point_condlist);

    MESSAGE("Spawn smart terrain [%s]",
        spawn_smart->name_replace());

    Fvector base_spawn_position = spawn_smart->Position();
    std::uint32_t base_level_vertex_id = spawn_smart->m_tNodeID;
    std::uint16_t base_game_vertex_id = spawn_smart->m_tGraphID;

    if (spawn_point_section_name.size())
    {
        if (spawn_point_section_name != "self")
        {
            base_spawn_position = CPatrolPathParams(spawn_point_section_name.c_str()).point(std::uint32_t(0));
            base_level_vertex_id = CPatrolPathParams(spawn_point_section_name.c_str()).level_vertex_id(0);
            base_game_vertex_id = CPatrolPathParams(spawn_point_section_name.c_str()).game_vertex_id(0);
        }
    }
    else if (spawn_smart->getSpawnPointName().size())
    {
        base_spawn_position = CPatrolPathParams(spawn_smart->getSpawnPointName().c_str()).point(std::uint32_t(0));
        base_level_vertex_id = CPatrolPathParams(spawn_smart->getSpawnPointName().c_str()).level_vertex_id(0);
        base_game_vertex_id = CPatrolPathParams(spawn_smart->getSpawnPointName().c_str()).game_vertex_id(0);
    }

    if (spawn_sections.size())
    {
        for (xr_string& it : spawn_sections)
        {
            this->add_squad_member(it, base_spawn_position, base_level_vertex_id, base_game_vertex_id);
        }
    }

    xr_string random_spawn_name =
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "npc_random");

    if (random_spawn_name.size())
    {
        xr_vector<xr_string> random_spawn_names = Globals::Utils::parse_names(random_spawn_name);
        std::uint32_t count_names = random_spawn_names.size();
        std::uint32_t min_value = Globals::kUnsignedInt32Undefined;
        std::uint32_t max_value = Globals::kUnsignedInt32Undefined;

        Globals::Utils::r_2nums(*const_cast<CInifile*>(Globals::get_system_ini()), this->m_settings_id_name,
            "npc_in_squad", min_value, max_value);

        if (min_value == Globals::kUnsignedInt32Undefined)
        {
            MESSAGEW("can't parse min_value set to 0");
            min_value = 0;
        }

        if (max_value == Globals::kUnsignedInt32Undefined)
        {
            MESSAGEW("can't parse max_value! set to 1");
            max_value = 1;
        }

        if (min_value > max_value)
        {
            R_ASSERT2(false, "It can't be!!!!");
            return;
        }

        std::uint32_t result_value = Globals::Script_RandomInt::getInstance().Generate(min_value, max_value - 1);

        for (std::uint32_t i = 0; i < result_value; ++i)
        {
            // @ Lord: нормально ли будет работать??? Не проверять если count_names > result_value ???
            std::uint32_t id = Globals::Script_RandomInt::getInstance().Generate(std::uint32_t(0), count_names - 1);
            this->add_squad_member(
                random_spawn_names[id], base_spawn_position, base_level_vertex_id, base_game_vertex_id);
        }
    }
    else if (!spawn_sections.size())
    {
        Msg("You are trying to spawn an empty squad [%s] ", this->m_settings_id_name.c_str());
        R_ASSERT(false);
        return;
    }

    this->m_smart_terrain_id = spawn_smart->ID;
    this->refresh();
}

std::uint16_t Script_SE_SimulationSquad::add_squad_member(const xr_string& spawn_section_name,
    const Fvector& spawn_position, const std::uint32_t& level_vertex_id, const std::uint16_t& game_vertex_id)
{
    if (spawn_section_name.empty())
    {
        R_ASSERT2(false, "It can't empty!");
        return Globals::kUnsignedInt16Undefined;
    }

    xr_string custom_data_name =
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), spawn_section_name, "custom_data");

    if (custom_data_name.empty())
    {
		MESSAGEW("INCORRECT npc_spawn_section USED [%s]. You cannot use npc with custom_data in squads",
			spawn_section_name.c_str());
    }


    CSE_Abstract* server_object =
        Globals::Game::alife_create(spawn_section_name, spawn_position, level_vertex_id, game_vertex_id);

    if (!server_object)
    {
        R_ASSERT2(false, "DEFAUQ! It can't be ...");
        return Globals::kUnsignedInt16Undefined;
    }

    this->register_member(server_object->ID);

    this->m_sound_manager.register_npc(server_object->ID);

    CALifeSimulator* alife = const_cast<CALifeSimulator*>(ai().get_alife());

    if (Globals::is_on_the_same_level(server_object->cast_alife_object(), ai().alife().graph().actor()) &&
        spawn_position.distance_to_sqr(alife->graph().actor()->Position()) <=
            (ai().alife().switch_distance() * ai().alife().switch_distance()))
        DataBase::Storage::getInstance().setSpawnedVertexByID(server_object->ID, level_vertex_id);

    return server_object->ID;
}

void Script_SE_SimulationSquad::on_npc_death(CSE_ALifeDynamicObject* server_object)
{
    if (!server_object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    MESSAGE("Squad %d killed member is %d", this->ID,
        server_object->ID);

    this->m_sound_manager.unregister_npc(server_object->ID);
    this->unregister_member(server_object->ID);

    if (!this->npc_count())
    {
        MESSAGE("REMOVING DEAD SQUAD %d", this->ID);

        if (this->m_current_action.getName().empty() == false)
        {
            this->m_current_action.Clear();
        }

        Script_SimulationBoard::getInstance().remove_squad(this);
    }
}

void Script_SE_SimulationSquad::on_reach_target(Script_SE_SimulationSquad* p_squad)
{
    p_squad->set_location_types("");
    Script_SimulationBoard::getInstance().assigned_squad_to_smart(p_squad);
}

void Script_SE_SimulationSquad::remove_squad(void)
{
    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it = this->squad_members().begin();
         it != this->squad_members().end(); ++it)
    {
        CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(it->first);

        if (server_object)
        {
            this->unregister_member(server_object->ID);
            const_cast<CALifeSimulator*>(ai().get_alife())->release(server_object);
        }
    }

    this->hide();
}

void Script_SE_SimulationSquad::set_squad_relation(const xr_string& relation_name)
{
    xr_string new_relation_name = relation_name.size() ? relation_name : this->m_relationship_name;

    if (new_relation_name.size())
    {
        for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
                 this->squad_members().begin();
             it != this->squad_members().end(); ++it)
        {
            CScriptGameObject* npc = nullptr;
            if (DataBase::Storage::getInstance().getStorage().find(it->first) !=
                DataBase::Storage::getInstance().getStorage().end())
                npc = DataBase::Storage::getInstance().getStorage().at(it->first).getClientObject();

            if (npc)
                Globals::GameRelations::set_npcs_relation(
                    npc, DataBase::Storage::getInstance().getActor(), new_relation_name);
            else
                Globals::GameRelations::set_npcs_relation(
                    ai().alife().objects().object(it->first)->ID, ai().alife().graph().actor()->ID, new_relation_name);
        }
    }
}

void Script_SE_SimulationSquad::assign_smart(Script_SE_SmartTerrain* smart)
{
    std::uint16_t old_smart_terrain_id = this->m_smart_terrain_id;

    if (smart)
        this->m_smart_terrain_id = smart->ID;

    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it = this->squad_members().begin();
         it != this->squad_members().end(); ++it)
    {
        this->assign_squad_member_to_smart(it->first, smart, old_smart_terrain_id);
    }
}

void Script_SE_SimulationSquad::set_squad_sympathy(const float& sympathy)
{
    float _sympathy = !(fis_zero(sympathy)) ? sympathy : this->m_sympathy;

    if (_sympathy)
    {
        for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
                 this->squad_members().begin();
             it != this->squad_members().end(); ++it)
        {
            CScriptGameObject* npc = DataBase::Storage::getInstance().getStorage().at(it->first).getClientObject();

            if (npc)
                Globals::GameRelations::set_npc_sympathy(npc, _sympathy);
            else
                DataBase::Storage::getInstance().setGoodwill_Sympathy(it->first, _sympathy);
        }
    }
}

void Script_SE_SimulationSquad::set_squad_behaviour(void)
{
    xr_string behaviour_section_name =
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "behaviour");

    if (!squad_behavior_ini.section_exist(behaviour_section_name.c_str()))
    {
        MESSAGEE("There is no section [%s] in misc\\squad_behaviours.ltx",
            behaviour_section_name.c_str());
        R_ASSERT(false);
        return;
    }

    std::uint32_t count = squad_behavior_ini.line_count(behaviour_section_name.c_str());

    for (std::uint32_t i = 0; i < count; ++i)
    {
        const char* Field = nullptr;
        const char* Value = nullptr;
        if (squad_behavior_ini.r_line(behaviour_section_name.c_str(), i, &Field, &Value))
            this->m_behaviour[Field] = Value;
    }
}

void Script_SE_SimulationSquad::init_squad_on_load(void)
{
    MESSAGE("Init squad %s on load",
        std::to_string(this->ID).c_str());

    this->set_squad_sympathy();
    Script_SimulationBoard::getInstance().assigned_squad_to_smart(this, this->m_smart_terrain_id);

    if (this->m_smart_terrain_id)
        Script_SimulationBoard::getInstance().enter_squad_to_smart(this, this->m_smart_terrain_id);

    this->m_is_need_to_reset_location_masks = true;
}

void Script_SE_SimulationSquad::refresh(void)
{
    if (!this->commander_id())
    {
        this->hide();
        return;
    }

    this->show();
}

void Script_SE_SimulationSquad::show(void)
{
    if (this->m_is_show_disabled)
    {
        this->hide();
        return;
    }

    if (this->m_current_spot_id != this->commander_id())
    {
        this->hide();
        this->m_current_spot_id = this->commander_id();
        this->show();
        return;
    }

    xr_string spot_name;

    if (Script_GlobalHelper::getInstance().getSimulationSquadIsSquadMonster().find(this->m_player_id_name) ==
        Script_GlobalHelper::getInstance().getSimulationSquadIsSquadMonster().end())
    {
        xr_string relation_name = Globals::Game::get_squad_relation_to_actor_by_id(this->ID);

        if (relation_name == Globals::kRelationsTypeFriends)
            spot_name = "alife_presentation_squad_friend";

        if (relation_name == Globals::kRelationsTypeNeutral)
            spot_name = "alife_presentation_squad_neutral";
    }

    if (spot_name.empty() == false)
    {
        if (spot_name == this->m_spot_section_name)
        {
            Globals::Game::level::map_change_spot_hint(this->m_current_spot_id, this->m_spot_section_name.c_str(), "");
            return;
        }

        if (this->m_spot_section_name.empty())
        {
            Globals::Game::level::map_add_object_spot(this->m_current_spot_id, spot_name.c_str(), "");
        }
        else
        {
            Globals::Game::level::map_remove_object_spot(this->m_current_spot_id, this->m_spot_section_name.c_str());
            Globals::Game::level::map_add_object_spot(this->m_current_spot_id, spot_name.c_str(), "");
        }

        this->m_spot_section_name = spot_name;
    }
    else
    {
        if (this->m_spot_section_name.empty() == false)
        {
            Globals::Game::level::map_remove_object_spot(this->m_current_spot_id, this->m_spot_section_name.c_str());
            this->m_spot_section_name.clear();
        }
    }
}

void Script_SE_SimulationSquad::hide(void)
{
    if (!this->m_current_spot_id || !this->m_spot_section_name.size())
        return;

    Globals::Game::level::map_remove_object_spot(this->m_current_spot_id, this->m_spot_section_name.c_str());

    this->m_current_spot_id = 0;
    this->m_spot_section_name = "";
}

bool Script_SE_SimulationSquad::check_squad_come_to_point(void)
{
    if (!this->m_parsed_targets.size())
        return true;

    // @ If we already to come
    std::uint32_t next_target_index = this->m_next_target_index ? this->m_next_target_index : 0;

    if (this->m_assigned_target_id && this->m_smart_terrain_id == this->m_assigned_target_id)
    {
        if (this->m_parsed_targets[next_target_index].size())
        {
            this->m_next_target_index = next_target_index + 1;
            return true;
        }
    }

    return false;
}

// @ Not used in game
void Script_SE_SimulationSquad::check_invulnerability(void)
{
    return;
}

bool Script_SE_SimulationSquad::update_current_action(void)
{
    return this->m_current_action.update(false);
}

void Script_SE_SimulationSquad::get_next_action(const bool is_under_simulation)
{
    Script_SE_SmartTerrain* const p_server_squad_target = ai().alife().objects().object(this->m_assigned_target_id)->cast_script_se_smartterrain();

    if (p_server_squad_target)
    {
        // TODO: удалить потом
		MESSAGE("assigned_target[%s]", p_server_squad_target->name_replace());
    }
    
    if (this->m_current_target_id == 0)
    {
        if (p_server_squad_target == nullptr || p_server_squad_target->am_i_reached(this))
        {
            if (p_server_squad_target)
            {
				p_server_squad_target->on_reach_target(this);
				p_server_squad_target->on_after_reach(this);
            }

            // TODO: удалить потом 
            MESSAGE("stay_on_target");

			this->m_current_action = StayReachOnTarget();
            this->m_current_action.setName(Globals::kSimulationSquadCurrentActionIDStayOnTarget);
			this->m_current_target_id = this->m_assigned_target_id;
			this->m_current_action.make(is_under_simulation);

			return;
        }
    }

    if ((this->m_assigned_target_id == this->m_current_target_id) || this->m_assigned_target_id == 0)
    {
        // TODO: удалить потом
        MESSAGE("stay_on_target");

        this->m_current_action = StayReachOnTarget();
        this->m_current_action.setName(Globals::kSimulationSquadCurrentActionIDStayOnTarget);
        this->m_current_target_id = this->m_assigned_target_id;
        this->m_current_action.make(is_under_simulation);
    }
    else
    {
        // TODO: потом удалить
        MESSAGE("reach_target get action");
        this->m_current_action = StayReachOnTarget(this->ID);
        this->m_current_action.setName(Globals::kSimulationSquadCurrentActionIDReachTarget);
        this->m_current_action.make(is_under_simulation);
    }
}

void Script_SE_SimulationSquad::generic_update(void)
{
    this->m_sound_manager.update();
    this->refresh();

    std::uint16_t help_target_id = get_help_target_id(this);
    if (help_target_id)
    {
        this->m_assigned_target_id = help_target_id;
        this->m_current_action.Clear();
        this->get_next_action(false);
        return;
    }

    if (this->m_assigned_target_id && ai().alife().objects().object(this->m_assigned_target_id) && ai().alife().objects().object(this->m_assigned_target_id)->m_script_clsid != Globals::get_script_clsid(CLSID_SE_ONLINE_OFFLINE_GROUP))
    {
        CSE_ALifeDynamicObject* const p_squad_target = Script_SimulationBoard::getInstance().get_squad_target(this);
        if (p_squad_target == nullptr)
        {
            MESSAGEWR("Invalid object!");
            return;
        }

        if (p_squad_target->m_script_clsid == Globals::get_script_clsid(CLSID_SE_ONLINE_OFFLINE_GROUP))
        {
            MESSAGE("assigned_target[%s][%d]", p_squad_target->name_replace(), p_squad_target->ID);

            this->m_assigned_target_id = p_squad_target->ID;
            this->m_current_action.Clear();
            this->get_next_action(true);
            return;
        }
    }

	if ((this->m_current_action.getName().empty() == false) && this->assigned_target_available())
	{
		bool is_finished = this->m_current_action.update(true);

		if (is_finished)
		{
			if (this->m_current_action.getName() == Globals::kSimulationSquadCurrentActionIDStayOnTarget || !this->m_assigned_target_id)
			{
				CSE_ALifeDynamicObject* p_object = Script_SimulationBoard::getInstance().get_squad_target(this);
				MESSAGE("assigned_target[%s][%d]", p_object->name_replace(), p_object->ID);
				this->m_assigned_target_id = p_object->ID;
			}

			this->m_current_action.Clear();
		}
		else
		{
			return;
		}
	}
	else
	{
		CSE_ALifeDynamicObject* p_object = Script_SimulationBoard::getInstance().get_squad_target(this);
		this->m_current_action.Clear();
		this->m_current_target_id = 0;
		this->m_assigned_target_id = p_object->ID;
		MESSAGE("assigned_target=[%s][%d]", p_object->name_replace(), p_object->ID);
	}

    this->get_next_action(true);
}

bool StayReachOnTarget::update(const bool is_under_simulation)
{
	if (!this->m_name.size())
	{
		R_ASSERT2(false, "You must indentifying your class's ID!");
		return false;
	}

	if (this->m_name == Globals::kSimulationSquadCurrentActionIDStayOnTarget)
	{
		if (!is_under_simulation)
			return false;
		else
			return (Globals::Game::get_game_time().diffSec(this->m_start_time) > this->m_idle_time);
	}
	else if (this->m_name == Globals::kSimulationSquadCurrentActionIDReachTarget)
	{
		Script_SE_SimulationSquad* const p_squad = ai().alife().objects().object(this->m_squad_id)->cast_script_se_simulationsquad();
		CSE_ALifeDynamicObject* p_target = nullptr;

		//		Script_SE_SmartTerrain* p_terrain = Script_SimulationObjects::getInstance().getObjects().at(p_squad->getAssignedTargetID())->cast_script_se_smartterrain();

		if (Script_SimulationObjects::getInstance().getObjects().find(p_squad->getAssignedTargetID()) != Script_SimulationObjects::getInstance().getObjects().end())
		{
			p_target = Script_SimulationObjects::getInstance().getObjects().at(p_squad->getAssignedTargetID());
		}

		if (!is_under_simulation)
			p_target = ai().alife().objects().object(p_squad->getAssignedTargetID())->cast_script_se_smartterrain();

		if (p_target == nullptr)
		{
			p_squad->setAssignedTargetID(0);
			return true;
		}

        if (p_target)
        {
            auto* p_try_1 = p_target->cast_script_se_actor();
            auto* p_try_2 = p_target->cast_script_se_simulationsquad();
            auto* p_try_3 = p_target->cast_script_se_smartterrain();

            if (p_try_1)
            {
                if (p_try_1->am_i_reached())
                {
                    return true;
                }
            }
            else if (p_try_2)
            {
                if (p_try_2->am_i_reached())
                {
                    return true;
                }
            }
            else if (p_try_3)
            {
                if (p_try_3->am_i_reached(p_squad))
                {
                    p_try_3->on_after_reach(p_squad);
                    return true;
                }
            }

        }

	}

	return false;
}

void StayReachOnTarget::make(const bool is_under_simulation)
{
	if (this->m_name.empty())
	{
		MESSAGEWR("You must indentify your class!");
		return;
	}

	if (this->m_name == Globals::kSimulationSquadCurrentActionIDStayOnTarget)
	{
		this->m_start_time = get_time_struct();
	}
	else if (this->m_name == Globals::kSimulationSquadCurrentActionIDReachTarget)
	{
		Script_SE_SimulationSquad* const p_squad = ai().alife().objects().object(this->m_squad_id)->cast_script_se_simulationsquad();

        CSE_ALifeDynamicObject* p_target = nullptr;

//		Script_SE_SmartTerrain* p_terrain = Script_SimulationObjects::getInstance().getObjects().at(p_squad->getAssignedTargetID())->cast_script_se_smartterrain();

        if (Script_SimulationObjects::getInstance().getObjects().find(p_squad->getAssignedTargetID()) != Script_SimulationObjects::getInstance().getObjects().end())
        {
            p_target = Script_SimulationObjects::getInstance().getObjects().at(p_squad->getAssignedTargetID());
        }

		if (!is_under_simulation)
			p_target = ai().alife().objects().object(p_squad->getAssignedTargetID());

        if (p_target)
        {
            //p_terrain->on_reach_target(p_squad);
            auto* p_try_1 = p_target->cast_script_se_actor();
            auto* p_try_2 = p_target->cast_script_se_simulationsquad();
            auto* p_try_3 = p_target->cast_script_se_smartterrain();

            if (p_try_1)
            {
                p_try_1->on_reach_target(p_squad);
            }
            else if (p_try_2)
            {
                p_try_2->on_reach_target(p_squad);
            }
            else if (p_try_3)
            {
                p_try_3->on_reach_target(p_squad);
            }
            else
            {
                R_ASSERT2(false, "can't be you must to cast to valid class which contains on_reach_target method!");
            }
        }


		for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it = p_squad->squad_members().begin(); it != p_squad->squad_members().end(); ++it)
		{
			if (it->first)
			{
				Script_SimulationBoard::getInstance().setup_squad_and_group(it->second);
			}
		}
	}

}

} // namespace Scripts
} // namespace Cordis
