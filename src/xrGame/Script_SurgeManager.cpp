#include "stdafx.h"
#include "Script_SurgeManager.h"

static float previous_sec = 0.0f;

namespace Cordis
{
namespace Scripts
{
bool check_squad_level(const std::uint16_t squad_id)
{
    CSE_Abstract* const p_squad = ai().alife().objects().object(squad_id);

    if (p_squad)
    {
        xr_string squad_level_name = Globals::get_level_name(
            Globals::Game::get_game_graph()->vertex(p_squad->cast_alife_dynamic_object()->m_tGraphID)->level_id());

        if (squad_level_name == Globals::Game::level::get_name())
            return true;
    }

    return false;
}

bool check_squad_community_and_story_id(const std::uint16_t squad_id)
{
    CSE_Abstract* const p_squad = ai().alife().objects().object(squad_id);

    if (p_squad)
    {
        if (Script_GlobalHelper::getInstance().getSurgeManagerImmunedToSurgeSquads().at(
                p_squad->cast_script_se_simulationsquad()->getPlayerIDName()))
            return false;

        if (!Globals::get_object_story_id(p_squad->ID).empty())
            return false;
    }

    return true;
}

bool check_squad_community(const std::uint16_t squad_id)
{
    CSE_Abstract* const p_squad = ai().alife().objects().object(squad_id);

    if (p_squad)
    {
        if (Script_GlobalHelper::getInstance().getSurgeManagerImmunedToSurgeSquads().at(
                p_squad->cast_script_se_simulationsquad()->getPlayerIDName()))
            return false;
    }

    return true;
}

bool check_squad_smart_props(const std::uint16_t squad_id)
{
    CSE_Abstract* const p_server_object = ai().alife().objects().object(squad_id);
    Script_SE_SimulationSquad* const p_server_squad = p_server_object->cast_script_se_simulationsquad();

    if (!p_server_squad)
    {
        Msg("[Scripts/check_squad_smart_props(squad_id)] WARNING: bad cast return ...");
        return false;
    }

    if (p_server_object)
    {
        if (p_server_squad->getSmartTerrainID() &&
            (Script_SimulationBoard::getInstance().getSmarts().find(p_server_squad->getSmartTerrainID()) !=
                Script_SimulationBoard::getInstance().getSmarts().end()))
        {
            Script_SE_SmartTerrain* const p_smart_terrain = Script_SimulationBoard::getInstance()
                                                                .getSmarts()
                                                                .at(p_server_squad->getSmartTerrainID())
                                                                .getServerSmartTerrain();

            if (p_smart_terrain)
            {
                // Lord: проверить что за значение float или int
                if (atoi(p_smart_terrain->getProperties().at("surge").c_str()) <= 0)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

xr_map<std::uint16_t, bool> get_squad_members(const std::uint16_t squad_id)
{
    xr_map<std::uint16_t, bool> result;
    CSE_Abstract* const p_server_object = ai().alife().objects().object(squad_id);

    if (p_server_object)
    {
        Script_SE_SimulationSquad* const p_squad = p_server_object->cast_script_se_simulationsquad();
        if (!p_squad)
        {
            Msg("[Scripts/get_squad_members(squad_id)] WARNING: bad cast return ...");
            return result;
        }

        for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
                 p_squad->squad_members().begin();
             it != p_squad->squad_members().end(); ++it)
        {
            result[it->second->ID] = true;
        }
    }

    return result;
}

Script_SurgeManager::~Script_SurgeManager(void) {}

void Script_SurgeManager::init_surge_covers(void)
{
    const char* id_name;
    const char* temp_name;
    for (std::uint32_t i = 0; i < this->m_ini.line_count("list"); ++i)
    {
        if (this->m_ini.r_line("list", i, &id_name, &temp_name))
        {
            if (DataBase::Storage::getInstance().getZoneByName().find(id_name) == DataBase::Storage::getInstance().getZoneByName().end())
                return;

            CScriptGameObject* const p_zone = DataBase::Storage::getInstance().getZoneByName().at(id_name);
            if (p_zone)
            {
                ++this->m_count;
                this->m_covers[this->m_count].first = p_zone;
                if (this->m_ini.line_exist(id_name, "condlist"))
                {
                    this->m_covers[this->m_count].second = XR_LOGIC::parse_condlist_by_script_object(
                        id_name, "condlist", this->m_ini.r_string(id_name, "condlist"));
                }
            }
        }
    }
}

std::uint16_t Script_SurgeManager::get_nearest_cover(void)
{
    if (this->m_is_loaded)
        this->init_surge_covers();

    xr_map<int, std::pair<CScriptGameObject*, xr_map<std::uint32_t, CondlistData>>> hides = this->m_covers;

    if (this->m_count)
    {
        for (const std::pair<int, std::pair<CScriptGameObject*, xr_map<std::uint32_t, CondlistData>>>& it : hides)
        {
            if (!it.second.second.empty())
            {
                xr_string picked_section_name =
                    XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
                        static_cast<CScriptGameObject*>(nullptr), it.second.second);

                if (!picked_section_name.empty() && picked_section_name != "true")
                {
                    hides.erase(it.first);
                }
            }
        }

        std::uint16_t nearest_cover_id = hides[0].first->ID();
        float nearest_cover_distance =
            hides[0].first->Position().distance_to(DataBase::Storage::getInstance().getActor()->Position());

        for (const std::pair<int, std::pair<CScriptGameObject*, xr_map<std::uint32_t, CondlistData>>>& it : hides)
        {
            if (DataBase::Storage::getInstance()
                    .getStorage()
                    .at(it.second.first->ID())
                    .getClientObject()
                    ->inside(DataBase::Storage::getInstance().getActor()->Position()))
            {
                return it.second.first->ID();
            }

            float distance =
                it.second.first->Position().distance_to(DataBase::Storage::getInstance().getActor()->Position());

            if (distance < nearest_cover_distance)
            {
                if (!it.second.second.empty())
                {
                    xr_string picked_section_name =
                        XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
                            static_cast<CScriptGameObject*>(nullptr), it.second.second);
                    if (picked_section_name == "true")
                    {
                        nearest_cover_distance = distance;
                        nearest_cover_id = it.second.first->ID();
                    }
                }
                else
                {
                    nearest_cover_distance = distance;
                    nearest_cover_id = it.second.first->ID();
                }
            }
        }

        if (nearest_cover_id == hides[0].first->ID())
        {
            if (!hides[0].second.empty())
            {
                xr_string picked_section_name =
                    XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
                        static_cast<CScriptGameObject*>(nullptr), hides[0].second);

                if (!picked_section_name.empty() && picked_section_name != "true")
                    return 0;
            }
        }

        return nearest_cover_id;
    }

    return 0;
}

void Script_SurgeManager::update(void)
{
    if (Device.dwPrecacheFrame > 1)
        return;

    if (!this->m_is_started)
    {
        xrTime global_time = Globals::Game::get_game_time();
        if (this->m_is_time_forwarded)
        {
            Msg("[Scripts/Script_SurgeManager/update()] delta %d", this->m_delta);
            Msg("[Scripts/Script_SurgeManager/update()] diffSec %f", global_time.diffSec(this->m_last_surge_time));
            std::uint32_t diff = abs(this->m_delta - global_time.diffSec(this->m_last_surge_time));
            Msg("[Scripts/Script_SurgeManager/update()] diff %d", diff);

            if (diff < 3600)
                this->m_delta = (3 * 3600) + global_time.diffSec(this->m_last_surge_time);

            this->m_is_time_forwarded = false;
        }

        if (static_cast<std::uint32_t>(global_time.diffSec(this->m_last_surge_time)) < this->m_delta)
        {
            return;
        }

        if (XR_LOGIC::pick_section_from_condlist(Globals::get_story_object("actor"),
                static_cast<CScriptGameObject*>(nullptr), this->m_condlist) != "true")
            return;

        if (!this->get_nearest_cover())
            return;

        this->start();
        return;
    }

    float diff_sec =
        ceilf(Globals::Game::get_game_time().diffSec(this->m_inited_time) / Globals::Game::level::get_time_factor());

    if (previous_sec != diff_sec)
    {
        previous_sec = diff_sec;
        Msg("[Scripts/Script_SurgeManager/update()] diff_sec %f", diff_sec);

        std::uint16_t cover_id = this->get_nearest_cover();

        if (!cover_id && !this->m_count)
        {
            this->init_surge_covers();
            return;
        }

        xr_string level_name = Globals::Game::level::get_name();

        if (level_name == "labx8" || level_name == "jupiter_underground")
        {
            Msg("[Scripts/Script_SurgeManager/update()] Surge stopped because level is restricted!");
            this->end_surge();
            return;
        }

        if (static_cast<std::uint32_t>(diff_sec) >= this->m_surge_time)
        {
            if (!Globals::Game::level::get_name().empty())
            {
                if (Globals::Game::level::get_name() == "zaton")
                {
                    xr_string faction_name;
                    XR_SOUND::set_sound_play(DataBase::Storage::getInstance().getActor()->ID(),
                        "zat_a2_stalker_barmen_after_surge", faction_name, 0);
                }
                else if (Globals::Game::level::get_name() == "jupiter")
                {
                    xr_string faction_name;
                    XR_SOUND::set_sound_play(DataBase::Storage::getInstance().getActor()->ID(),
                        "jup_a6_stalker_medik_after_surge", faction_name, 0);
                }
                else if (!Globals::has_alife_info("pri_b305_fifth_cam_end"))
                {
                    xr_string faction_name;
                    XR_SOUND::set_sound_play(DataBase::Storage::getInstance().getActor()->ID(),
                        "pri_a17_kovalsky_after_surge", faction_name, 0);
                }
            }

            this->end_surge();
        }
        else
        {
            if (this->m_is_loaded)
            {
                if (this->m_is_blowout_sound)
                {
                    XR_SOUND::play_sound_looped(DataBase::Storage::getInstance().getActor()->ID(), "blowout_rumble");
                }

                if (this->m_is_effector_set)
                {
                    Globals::Game::level::add_pp_effector("surge_shock.ppe", _kSurgeShockPPeffector, true);
                }

                if (this->m_is_second_message_given)
                {
                    XR_SOUND::play_sound_looped(
                        DataBase::Storage::getInstance().getActor()->ID(), "surge_earthquake_sound_looped");
                    Globals::Game::level::add_cam_effector(
                        "camera_effects\\earthquake.anm", _kEathquakeCamEffector, true, "");
                }

                this->m_is_loaded = false;
            }

            this->launch_rockets();
            if (this->m_is_effector_set)
                Globals::Game::level::set_pp_effector_factor(_kSurgeShockPPeffector, diff_sec / 90.0f, 0.2f);

            if (this->m_is_blowout_sound)
                XR_SOUND::set_volume_sound_looped(
                    DataBase::Storage::getInstance().getActor()->ID(), "blowout_rumble", diff_sec / 180.0f);

            if (diff_sec >= 140.0f && !(this->m_is_ui_disabled) &&
                (!cover_id ||
                    !(DataBase::Storage::getInstance().getStorage().at(cover_id).getClientObject()->inside(
                        DataBase::Storage::getInstance().getActor()->Position()))))
            {
                float att = 1 - (185.0f - diff_sec) / (185.0f - 140.0f);
                att *= att;
                att *= att;
                att *= att;
                att *= 0.3f;

                CScriptHit hit;
                hit.m_tHitType = ALife::eHitTypeTelepatic;
                hit.m_fPower = att;
                hit.m_fImpulse = 0.0f;
                hit.m_tDirection = Fvector().set(0.0f, 0.0f, 1.0f);
                hit.m_tpDraftsman = DataBase::Storage::getInstance().getActor();

                if (XR_LOGIC::pick_section_from_condlist(Globals::get_story_object("actor"),
                        static_cast<CScriptGameObject*>(nullptr), this->m_survive_condlist) == "true")
                {
                    if (DataBase::Storage::getInstance().getActor()->GetHealth() <= hit.m_fPower)
                    {
                        hit.m_fPower = DataBase::Storage::getInstance().getActor()->GetHealth() - 0.05f;
                        if (hit.m_fPower < 0.0f)
                            hit.m_fPower = 0.0f;
                    }
                }

                DataBase::Storage::getInstance().getActor()->Hit(&hit);
            }

            if (diff_sec >= 185.0f && !(this->m_is_ui_disabled))
            {
                this->kill_all_unhided();
                this->m_is_ui_disabled = true;
            }
            else if (diff_sec >= 140.0f && !(this->m_is_second_message_given))
            {
                if (!Globals::Game::level::get_name().empty())
                {
                    if (Globals::Game::level::get_name() == "zaton")
                    {
                        xr_string faction_name;
                        XR_SOUND::set_sound_play(DataBase::Storage::getInstance().getActor()->ID(),
                            "zat_a2_stalker_barmen_after_surge", faction_name, 0);
                    }
                    else if (Globals::Game::level::get_name() == "jupiter")
                    {
                        xr_string faction_name;
                        XR_SOUND::set_sound_play(DataBase::Storage::getInstance().getActor()->ID(),
                            "jup_a6_stalker_medik_after_surge", faction_name, 0);
                    }
                    else if (!Globals::has_alife_info("pri_b305_fifth_cam_end"))
                    {
                        xr_string faction_name;
                        XR_SOUND::set_sound_play(DataBase::Storage::getInstance().getActor()->ID(),
                            "pri_a17_kovalsky_after_surge", faction_name, 0);
                    }

                    XR_SOUND::play_sound_looped(
                        DataBase::Storage::getInstance().getActor()->ID(), "surge_earthquake_sound_looped");
                    Globals::Game::level::add_cam_effector(
                        "camera_effects\\earthquake.anm", _kEathquakeCamEffector, true, "");
                    this->m_is_second_message_given = true;
                }
            }
            else if (diff_sec >= 100.0f && !(this->m_is_effector_set))
            {
                Globals::Game::level::add_pp_effector("surge_shock.ppe", _kSurgeShockPPeffector, true);
                this->m_is_effector_set = true;
            }
            else if (diff_sec >= 35.0f && !(this->m_is_blowout_sound))
            {
                XR_SOUND::set_sound_play(
                    DataBase::Storage::getInstance().getActor()->ID(), "blowout_begin", xr_string(), 0);
                XR_SOUND::play_sound_looped(DataBase::Storage::getInstance().getActor()->ID(), "blowout_rumble");
                XR_SOUND::set_volume_sound_looped(
                    DataBase::Storage::getInstance().getActor()->ID(), "blowout_rumble", 0.25f);
                this->m_is_blowout_sound = true;
            }
            else if (diff_sec >= 0.0f && !(this->m_is_task_given))
            {
                if (!Globals::Game::level::get_name().empty())
                {
                    if (Globals::Game::level::get_name() == "zaton")
                    {
                        xr_string faction_name;
                        XR_SOUND::set_sound_play(DataBase::Storage::getInstance().getActor()->ID(),
                            "zat_a2_stalker_barmen_after_surge", faction_name, 0);
                    }
                    else if (Globals::Game::level::get_name() == "jupiter")
                    {
                        xr_string faction_name;
                        XR_SOUND::set_sound_play(DataBase::Storage::getInstance().getActor()->ID(),
                            "jup_a6_stalker_medik_after_surge", faction_name, 0);
                    }
                    else if (!Globals::has_alife_info("pri_b305_fifth_cam_end"))
                    {
                        xr_string faction_name;
                        XR_SOUND::set_sound_play(DataBase::Storage::getInstance().getActor()->ID(),
                            "pri_a17_kovalsky_after_surge", faction_name, 0);
                    }
                }

                Globals::Game::level::set_weather_fx("fx_surge_day_3");
                this->give_surge_hide_task();
                this->m_is_task_given = true;
            }
        }
    }
}

void Script_SurgeManager::load(IReader* packet)
{
    Globals::set_save_marker(*packet, Globals::kSaveMarkerMode_Load, false, "Script_SurgeManager");

    this->m_is_finished = packet->r_u8();
    this->m_is_started = packet->r_u8();

    this->m_last_surge_time = Globals::Utils::r_CTime(*packet);

    if (this->m_is_started)
    {
        this->m_inited_time = Globals::Utils::r_CTime(*packet);

        this->m_levels_respawn["zaton"] = packet->r_u8();
        this->m_levels_respawn["jupiter"] = packet->r_u8();
        this->m_levels_respawn["pripyat"] = packet->r_u8();

        this->m_is_task_given = packet->r_u8();
        this->m_is_effector_set = packet->r_u8();
        this->m_is_second_message_given = packet->r_u8();
        this->m_is_ui_disabled = packet->r_u8();
        this->m_is_blowout_sound = packet->r_u8();

        packet->r_stringZ(this->m_surge_message_name);
        packet->r_stringZ(this->m_surge_task_section_name);
    }

    this->m_delta = packet->r_u32();
    this->m_is_loaded = true;
    Globals::set_save_marker(*packet, Globals::kSaveMarkerMode_Load, true, "Script_SurgeManager");
}

void Script_SurgeManager::save(NET_Packet& packet)
{
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, false, "Script_SurgeManager");

    packet.w_u8(this->m_is_finished);
    packet.w_u8(this->m_is_started);
    Globals::Utils::w_CTime(packet, this->m_last_surge_time);

    if (this->m_is_started)
    {
        Globals::Utils::w_CTime(packet, this->m_inited_time);

        packet.w_u8(this->m_levels_respawn.at("zaton"));
        packet.w_u8(this->m_levels_respawn.at("jupiter"));
        packet.w_u8(this->m_levels_respawn.at("pripyat"));

        packet.w_u8(this->m_is_task_given);
        packet.w_u8(this->m_is_effector_set);
        packet.w_u8(this->m_is_second_message_given);
        packet.w_u8(this->m_is_ui_disabled);
        packet.w_u8(this->m_is_blowout_sound);

        packet.w_stringZ(this->m_surge_message_name.c_str());
        packet.w_stringZ(this->m_surge_task_section_name.c_str());
    }

    packet.w_u32(this->m_delta);

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, true, "Script_SurgeManager");
}

void Script_SurgeManager::launch_rockets(void)
{
    // Можете реализовать сами
}

void Script_SurgeManager::kill_all_unhided_after_actor_death(void)
{
    for (const std::pair<std::uint32_t, Script_SE_SimulationSquad*>& it :
        Script_SimulationBoard::getInstance().getSquads())
    {
        Script_SE_SimulationSquad* const p_squad = it.second;

        if (check_squad_level(p_squad->ID))
        {
            if (check_squad_community(p_squad->ID))
            {
                xr_map<std::uint16_t, bool> squad_npcs = get_squad_members(p_squad->ID);

                for (const std::pair<std::uint16_t, bool>& it : squad_npcs)
                {
                    CSE_Abstract* const p_server_object = ai().alife().objects().object(it.first);

                    if (p_server_object)
                    {
                        bool is_release = true;

                        for (const std::pair<int, std::pair<CScriptGameObject*, xr_map<std::uint32_t, CondlistData>>>&
                                 it_cover : this->m_covers)
                        {
                            CScriptGameObject* const p_client_cover = it_cover.second.first;

                            if (p_client_cover && p_client_cover->inside(p_server_object->o_Position))
                                is_release = false;
                        }

                        if (is_release)
                        {
                            Msg("[Scripts/Script_SurgeManager/kill_all_unhide_after_actor_death()] Releasing npc %s "
                                "from squad %s after actot's death",
                                p_server_object->name_replace(), p_squad->name_replace());
                            CScriptGameObject* const p_client_object =
                                Globals::Game::level::get_object_by_id(p_server_object->ID);

                            if (p_client_object)
                                p_client_object->Kill(p_client_object);
                            else
                                p_server_object->cast_monster_abstract()->kill();
                        }
                    }
                }
            }
        }
    }
}

void Script_SurgeManager::kill_all_unhided(void)
{
    CScriptHit hit;

    hit.m_tHitType = ALife::eHitTypeFireWound;
    hit.m_fPower = 0.9f;
    hit.m_fImpulse = 0.0f;
    hit.m_tDirection = Fvector().set(0.0f, 0.0f, 1.0f);
    hit.m_tpDraftsman = DataBase::Storage::getInstance().getActor();

    for (const std::pair<std::uint16_t, std::uint16_t>& it : DataBase::Storage::getInstance().getCrowStorage())
    {
        CSE_Abstract* const p_server_object = ai().alife().objects().object(it.second);

        if (p_server_object)
        {
            CScriptGameObject* const p_client_object = Globals::Game::level::get_object_by_id(p_server_object->ID);

            if (p_client_object && p_client_object->Alive())
                p_client_object->Hit(&hit);
        }
    }

    for (const std::pair<std::uint16_t, Script_SE_SimulationSquad*>& it :
        Script_SimulationBoard::getInstance().getSquads())
    {
        Script_SE_SimulationSquad* const p_squad = it.second;
        if (check_squad_level(p_squad->ID))
        {
            if (check_squad_community_and_story_id(p_squad->ID))
            {
                xr_map<std::uint16_t, bool> squad_npcs = get_squad_members(p_squad->ID);

                for (const std::pair<std::uint16_t, bool>& it : squad_npcs)
                {
                    CSE_Abstract* const p_server_object = ai().alife().objects().object(it.first);

                    if (p_server_object && (Globals::get_object_story_id(p_server_object->ID).empty()))
                    {
                        if (check_squad_smart_props(p_squad->ID))
                        {
                            Msg("[Scripts/Script_SurgeManager/kill_all_unhided()] Releasing npc %s from squad %s",
                                p_server_object->name_replace(), p_squad->name_replace());

                            CScriptGameObject* const p_client_object =
                                Globals::Game::level::get_object_by_id(p_server_object->ID);

                            if (p_client_object)
                                p_client_object->Kill(p_client_object);
                            else
                                p_server_object->cast_monster_abstract()->kill();
                        }
                        else
                        {
                            bool is_release = true;

                            for (const std::pair<int,
                                     std::pair<CScriptGameObject*, xr_map<std::uint32_t, CondlistData>>>& it_cover :
                                this->m_covers)
                            {
                                CScriptGameObject* const p_client_cover = it_cover.second.first;

                                if (p_client_cover && p_client_cover->inside(p_server_object->o_Position))
                                    is_release = false;
                            }

                            if (is_release)
                            {
                                Msg("[Scripts/Script_SurgeManager/kill_all_unhide_after_actor_death()] Releasing npc "
                                    "%s "
                                    "from squad %s after actot's death",
                                    p_server_object->name_replace(), p_squad->name_replace());
                                CScriptGameObject* const p_client_object =
                                    Globals::Game::level::get_object_by_id(p_server_object->ID);

                                if (p_client_object)
                                    p_client_object->Kill(p_client_object);
                                else
                                    p_server_object->cast_monster_abstract()->kill();
                            }
                        }
                    }
                }
            }
        }
    }
}

void Script_SurgeManager::give_surge_hide_task(void)
{
    if (this->m_surge_message_name != "empty")
    {
        xr_string message_name;
        if (this->m_surge_message_name.empty())
            message_name = Globals::Game::translate_string("hide_from_surge_message");
        else
            message_name = Globals::Game::translate_string(this->m_surge_message_name.c_str());

        if (this->m_surge_task_section_name != "empty")
        {
            if (this->m_surge_task_section_name.empty())
            {
                Script_TaskManager::getInstance().give_task("hide_from_surge");
            }
            else
            {
                Script_TaskManager::getInstance().give_task(this->m_surge_task_section_name);
            }
        }
    }
}

void Script_SurgeManager::respawn_artefacts_and_replace_anomaly_zone(void)
{
    if (this->m_levels_respawn.at(Globals::Game::level::get_name()))
        this->m_levels_respawn[Globals::Game::level::get_name()] = false;

    for (const std::pair<xr_string, CScriptGameObject*>& it : DataBase::Storage::getInstance().getZoneByName())
    {
        Script_Binder_Anomaly* const p_binder = dynamic_cast<Script_Binder_Anomaly*>(it.second->binded_object());
        if (!p_binder)
        {
            R_ASSERT2(false, "Are you sure that is an anomaly client object?");
            Msg("Object name: %s", it.second->Name());
            return;
        }

        p_binder->respawn_artefacts_and_replace_anomaly_zone();
    }

    Globals::change_anomalies_names();
}

void Script_SurgeManager::end_surge(bool is_manual)
{
    this->m_is_started = false;
    this->m_is_finished = true;

    for (std::pair<const xr_string, bool>& it : this->m_levels_respawn)
        it.second = true;

    this->m_last_surge_time = Globals::Game::get_game_time();
    this->m_delta =
        Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(_kStartMinSurgeTime, _kStartMaxSurgeTime);
    this->m_surge_message_name.clear();
    this->m_surge_task_section_name.clear();
    this->m_is_task_given = false;

    if (this->m_is_effector_set)
        XR_SOUND::stop_sound_looped(DataBase::Storage::getInstance().getActor()->ID(), "blowout_rumble");

    if (this->m_is_second_message_given)
        XR_SOUND::stop_sound_looped(DataBase::Storage::getInstance().getActor()->ID(), "surge_earthquake_sound_looped");

    Globals::Game::level::remove_pp_effector(_kSurgeShockPPeffector);
    Globals::Game::level::remove_cam_effector(_kEathquakeCamEffector);

    if (is_manual || (this->m_is_time_forwarded && !Script_WeatherManager::getInstance().getWeatherFXName().empty()))
    {
        Globals::Game::level::stop_weather_fx();
        Script_WeatherManager::getInstance().forced_weather_change();
    }

    this->m_is_effector_set = false;
    this->m_is_second_message_given = false;
    this->m_is_ui_disabled = false;
    this->m_is_blowout_sound = false;
    previous_sec = 0.0f;

    // Signal light here, but I don't want to use it in game

    if (this->m_is_loaded)
        this->kill_all_unhided();

    this->respawn_artefacts_and_replace_anomaly_zone();

    // Lord: добавить когда будет xr_statistic
}

void Script_SurgeManager::skip_surge(void)
{
    std::uint32_t year, month, day, hour, minute, second, milisecond = 0;
    this->m_inited_time.get(year, month, day, hour, minute, second, milisecond);

    this->m_last_surge_time.set(year, month, day, hour, minute, second + this->m_surge_time, milisecond);

    this->m_is_started = false;
    this->m_is_finished = true;

    for (std::pair<const xr_string, bool>& it : this->m_levels_respawn)
        it.second = true;

    this->m_delta =
        Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(_kStartMinSurgeTime, _kStartMaxSurgeTime);
    this->m_surge_message_name.clear();
    this->m_surge_task_section_name.clear();
    this->m_is_task_given = false;
    this->m_is_effector_set = false;
    this->m_is_second_message_given = false;
    this->m_is_ui_disabled = false;
    this->m_is_blowout_sound = false;
    previous_sec = 0.0f;
    this->respawn_artefacts_and_replace_anomaly_zone();
    // Lord:добавить xr_statistic

    if (!this->m_is_skip_message)
    {
        Script_NewsManager::getInstance().SendTip(
            DataBase::Storage::getInstance().getActor(), "st_surge_while_asleep", "recent_surge");
        this->m_is_skip_message = true;
    }
}

void Script_SurgeManager::start(const bool is_manual)
{
    std::uint32_t year, month, day, hour, minute, second, milisecond = 0;
    this->m_last_surge_time.get(year, month, day, hour, minute, second, milisecond);

    if (is_manual)
        this->m_inited_time = Globals::Game::get_game_time();
    else
        this->m_inited_time.set(year, month, day, hour, minute, second + this->m_delta, milisecond);

    float diff_sec =
        ceilf(Globals::Game::get_game_time().diffSec(this->m_inited_time) / Globals::Game::level::get_time_factor());
    Msg("[Scripts/Script_SurgeManager/start(is_manual)] %f", diff_sec);

    xr_string level_name = Globals::Game::level::get_name();

    if (level_name == "labx8" || level_name == "jupiter_underground")
    {
        Msg("[Scripts/Script_SurgeManager/start(is_manual)] Surge skipped while time forwarding!");
        this->m_is_skip_message = true;
        this->skip_surge();
        return;
    }

    if (static_cast<std::uint32_t>(diff_sec + 6.0f) > this->m_surge_time)
    {
        Msg("[Scripts/Script_SurgeManager/start(is_manual)] Surge skipped while time forwarding!");
        this->skip_surge();
    }
    else
    {
        this->m_is_started = true;
        this->m_is_finished = false;

        if (!Globals::has_alife_info("pri_b305_fifth_cam_end") || Globals::has_alife_info("pri_a28_actor_in_zone_stay"))
        {
            XR_EFFECTS::scenario_autosave(nullptr, nullptr, {"st_save_uni_surge_start"});
        }
    }
}

} // namespace Scripts
} // namespace Cordis
