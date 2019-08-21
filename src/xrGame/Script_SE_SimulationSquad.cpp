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
        Msg("[Scripts/Script_SE_SimulationSquad/getScriptTarget()] Incorrect next point [%s] for squad [%s]", _new_target.c_str(), std::to_string(this->ID).c_str());
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

} // namespace Scripts
} // namespace Cordis
