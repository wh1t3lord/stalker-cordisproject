#include "stdafx.h"
#include "Script_SE_SimulationSquad.h"
#include "Script_XR_Logic.h"

CInifile squad_behavior_ini = CInifile("misc\\squad_behaviours.ltx");
CInifile locations_ini = CInifile("misc\\smart_terrain_masks.ltx");

namespace Cordis
{
namespace Scripts
{
Script_SE_SimulationSquad::Script_SE_SimulationSquad(LPCSTR section)
    : inherited(section), m_assigned_target_id(0), m_current_spot_id(0), m_current_target_id(0), m_smart_terrain_id(0),
      m_settings_id_name(this->name()),
      m_sound_manager(Script_SoundManager::getSoundManager((xr_string("squad_").append(this->name())))),
      m_player_id_name(Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "faction")),
      m_condlist_action(XR_LOGIC::parse_condlist_by_server_object("assign_action", "target_smart",
          Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "target_smart"))),
      m_condlist_death(XR_LOGIC::parse_condlist_by_server_object("death_condlist", "on_death",
          Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "target_smart"))),
      m_condlist_invulnerability(XR_LOGIC::parse_condlist_by_server_object("invulnerability", "invulnerability",
          Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "invulnerability"))),
      m_relationship_name(
          Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "relationship")),
      m_sympathy(Globals::Utils::cfg_get_number(Globals::get_system_ini(), this->m_settings_id_name, "sympathy", this)),
      m_condlist_show_spot(XR_LOGIC::parse_condlist_by_server_object("show_spot", "show_spot",
          Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "show_spot"))),
      m_is_always_arrived(
          Globals::Utils::cfg_get_bool(Globals::get_system_ini(), this->m_settings_id_name, "always_arrived", this)),
      m_is_always_walk(
          Globals::Utils::cfg_get_bool(Globals::get_system_ini(), this->m_settings_id_name, "always_walk", this)),
      m_is_need_to_reset_location_masks(false), m_is_need_free_update(false)
{
    this->set_location_types_section("stalker_terrain");
    this->set_squad_sympathy();
}

Script_SE_SimulationSquad::~Script_SE_SimulationSquad(void) {}

inline std::uint16_t Script_SE_SimulationSquad::getScriptTarget(void)
{
    xr_string new_target_name = XR_LOGIC::pick_section_from_condlist(
        DataBase::Storage::getInstance().getActor(), this, this->m_condlist_action);

    if (!new_target_name.size())
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

    if (!this->m_parsed_targets[this->m_next_target_index].size())
        this->m_next_target_index = 0;

    xr_string _new_target = this->pick_next_target();

    if (_new_target == "nil")
        return 0;
    else if (_new_target == "loop")
    {
        this->m_next_target_index = 0;
        _new_target = this->pick_next_target();
    }

    Script_SE_SmartTerrain* smart = Script_SimulationBoard::getInstance().getSmartTerrainsByName()[_new_target];

    if (!smart)
    {
        Msg("[Scripts/Script_SE_SimulationSquad/getScriptTarget()] Incorrect next point [%s] for squad [%s]",
            _new_target.c_str(), std::to_string(this->ID).c_str());
        R_ASSERT(false);
        return std::uint16_t(0);
    }

    return smart->ID;
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

    if (ai().alife().objects().object(this->m_assigned_target_id)->script_clsid() == CLSID_SE_SMART_TERRAIN)
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

        Msg("[Scripts/Script_SE_SmartTerrain/set_location_types(new_smart_name)] Old smart terrain name [%s]",
            old_smart_name.c_str());

        if (old_smart_name.size())
            this->set_location_types_section(old_smart_name);

        Msg("[Scripts/Script_SE_SmartTerrain/set_location_types(new_smart_name)] New smart terrain name [%s]",
            new_smart_name.c_str());

        if (new_smart_name.c_str())
            this->set_location_types_section(new_smart_name);
    }
    else
    {
        Msg("[Scripts/Script_SE_SmartTerrain/set_location_types(new_smart_name)] The target is squad or actor setting "
            "[squad_terrain]!");

        this->set_location_types_section("squad_terrain");

        for (std::pair<const std::uint16_t, CSE_ALifeDynamicObject*>& it :
            Script_SimulationObjects::getInstance().getObjects())
        {
            CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(it.first);
            if (server_object)
            {
                if (server_object->script_clsid() == CLSID_SE_SMART_TERRAIN)
                {
                    xr_string properties_base = server_object->getProperties()["base"];
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

    Msg("[Scripts/Script_SE_SimulationSquad/create_npc(spawn_smart)] Spawn smart terrain [%s]",
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

    if (!spawn_sections.size())
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
        std::uint32_t min_value = 1;
        std::uint32_t max_value = 2;

        Globals::Utils::r_2nums(*const_cast<CInifile*>(Globals::get_system_ini()), this->m_settings_id_name,
            "npc_in_squad", min_value, max_value);

        if (min_value > max_value)
        {
            R_ASSERT2(false, "It can't be!!!!");
            return;
        }

        std::uint32_t result_value = Globals::Script_RandomInt::getInstance().Generate(min_value, max_value);

        for (std::uint32_t i = 0; i < result_value; ++i)
        {
            // @ Lord: нормально ли будет работать??? Не проверять если count_names > result_value ???
            std::uint32_t id = Globals::Script_RandomInt::getInstance().Generate(std::uint32_t(0), count_names);
            this->add_squad_member(
                random_spawn_names[id], base_spawn_position, base_level_vertex_id, base_game_vertex_id);
        }
    }

    if (!spawn_sections.size())
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
    if (!spawn_section_name.size())
    {
        R_ASSERT2(false, "It can't empty!");
        return Globals::kUnsignedInt16Undefined;
    }

    xr_string custom_data_name =
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), spawn_section_name, "custom_data");

    if (!custom_data_name.size())
        Msg("[Scripts/Script_SE_SimulationSquad/add_squad_member(spawn_section_name, spawn_position, level_vertex_id, "
            "game_vertex_id)] INCORRECT npc_spawn_section USED [%s]. You cannot use npc with custom_data in squads",
            spawn_section_name.c_str());

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
        DataBase::Storage::getInstance().getSpawnedVertexByID()[server_object->ID] = level_vertex_id;

    return server_object->ID;
}

void Script_SE_SimulationSquad::on_npc_death(CSE_ALifeDynamicObject* server_object)
{
    if (!server_object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    Msg("[Scripts/Script_SE_SimulationSquad/on_npc_death(server_object)] Squad %d killed member is %d", this->ID,
        server_object->ID);

    this->m_sound_manager.unregister_npc(server_object->ID);
    this->unregister_member(server_object->ID);

    if (!this->npc_count())
    {
        Msg("[Scripts/Script_SE_SimulationSquad/on_npc_death(server_object)] REMOVING DEAD SQUAD %d", this->ID);

        if (this->m_current_action.m_name.size())
        {
            this->m_current_action.Clear();
        }

        Script_SimulationBoard::getInstance().remove_squad(this);
    }
}

void Script_SE_SimulationSquad::remove_squad(void)
{
    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it = this->squad_members().begin();
         it != this->squad_members().end(); ++it)
    {
        CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(it->second->ID);

        if (server_object)
        {
            this->unregister_member(server_object->ID);
            const_cast<CALifeSimulator*>(ai().get_alife())->release(server_object);
        }
    }

    this->hide();
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
            CScriptGameObject* npc = DataBase::Storage::getInstance().getStorage()[(*it).second->ID].m_object;

            if (npc)
                Globals::GameRelations::set_npc_sympathy(npc, _sympathy);
            else
                DataBase::Storage::getInstance().getGoodwill_Sympathy()[(*it).second->ID] = _sympathy;
        }
    }
}

void Script_SE_SimulationSquad::set_squad_behaviour(void)
{
    xr_string behaviour_section_name =
        Globals::Utils::cfg_get_string(Globals::get_system_ini(), this->m_settings_id_name, "behaviour");

    if (!squad_behavior_ini.section_exist(behaviour_section_name.c_str()))
    {
        Msg("[Scripts/Script_SE_SimulationSquad/set_squad_behaviour()] There is no section [%s] in "
            "misc\\squad_behaviours.ltx",
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
    Msg("[Scripts/Script_SE_SimulationSquad/init_squad_on_load()] Init squad %s on load",
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

    xr_string spot_name = "";

    if (!Script_GlobalHelper::getInstance().getSimulationSquadIsSquadMonster()[this->m_player_id_name])
    {
        xr_string relation_name = Globals::Game::get_squad_relation_to_actor_by_id(this->ID);

        if (relation_name == Globals::kRelationsTypeFriends)
            spot_name = "alife_presentation_squad_friend";

        if (relation_name == Globals::kRelationsTypeNeutral)
            spot_name = "alife_presentation_squad_neutral";
    }

    if (spot_name.size())
    {
        if (spot_name == this->m_spot_section_name)
        {
            Globals::Game::level::map_change_spot_hint(this->m_current_spot_id, this->m_spot_section_name.c_str(), "");
            return;
        }

        if (!this->m_spot_section_name.size())
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
        if (this->m_spot_section_name.size())
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

} // namespace Scripts
} // namespace Cordis
