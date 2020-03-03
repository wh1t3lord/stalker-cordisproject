#pragma once
#include "Script_GlobalDefinitions.h"
#include "Script_SchemeXRAbuse.h"
#include "HangingLamp.h"

namespace Cordis
{
namespace Scripts
{
namespace XR_EFFECTS
{
inline void update_npc_logic(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_object, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/update_npc_logic(p_actor, p_object, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    for (const xr_string& it : buffer)
    {
        CScriptGameObject* p_npc = Globals::get_story_object(it);

        if (p_npc)
        {
            Globals::update_logic(p_npc);

            CScriptActionPlanner* p_planner = Globals::get_script_action_planner(p_npc);
            p_planner->update();
            p_planner->update();
            p_planner->update();

            Script_StateManager* const p_state_manager =
                DataBase::Storage::getInstance().getStorage().at(p_npc->ID()).getStateManager();

            p_state_manager->update();
            p_state_manager->update();
            p_state_manager->update();
            p_state_manager->update();
            p_state_manager->update();
            p_state_manager->update();
            p_state_manager->update();
        }
    }
}

static int ui_active_slot = 0;

inline void update_obj_logic(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_object, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/update_obj_logic(p_actor, p_object, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    for (const xr_string& it : buffer)
    {
        CScriptGameObject* const p_object = Globals::get_story_object(it);

        if (p_object)
        {
            const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(p_object->ID());
            XR_LOGIC::try_switch_to_another_section(
                p_object, *storage.getSchemes().at(storage.getActiveSchemeName()), p_actor);
        }
    }
}

inline void disable_ui(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (DataBase::Storage::getInstance().getActor()->IsTalking())
    {
        DataBase::Storage::getInstance().getActor()->StopTalk();
    }

    Globals::Game::level::show_weapon(false);

    if (buffer.empty() || (buffer[0] != "true"))
    {
        int current_slot = DataBase::Storage::getInstance().getActor()->active_slot();
        if (current_slot != 0)
        {
            ui_active_slot = current_slot;
            DataBase::Storage::getInstance().getActor()->activate_slot(0);
        }
    }

    Globals::Game::level::disable_input();
    Globals::Game::level::hide_indicators_safe();
    CurrentGameUI()->HideActorMenu();
    CurrentGameUI()->HidePdaMenu();
    disable_actor_nightvision(nullptr, nullptr, {});
    disable_actor_torch(nullptr, nullptr, {});
}

inline void disable_ui_only(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (DataBase::Storage::getInstance().getActor()->IsTalking())
    {
        DataBase::Storage::getInstance().getActor()->StopTalk();
    }

    Globals::Game::level::show_weapon(false);

    int current_slot = DataBase::Storage::getInstance().getActor()->active_slot();
    if (current_slot != 0)
    {
        ui_active_slot = current_slot;
        DataBase::Storage::getInstance().getActor()->activate_slot(0);
    }

    Globals::Game::level::disable_input();
    Globals::Game::level::hide_indicators_safe();
    CurrentGameUI()->HideActorMenu();
    CurrentGameUI()->HidePdaMenu();
}

inline void enable_ui(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty() || (buffer[0] != "true"))
    {
        if (ui_active_slot != 0 && DataBase::Storage::getInstance().getActor()->item_in_slot(ui_active_slot))
        {
            DataBase::Storage::getInstance().getActor()->activate_slot(ui_active_slot);
        }
    }

    ui_active_slot = 0;
    Globals::Game::level::show_weapon(true);
    Globals::Game::level::enable_input();
    Globals::Game::level::show_indicators();
    enable_actor_nightvision(nullptr, nullptr, {});
    enable_actor_torch(nullptr, nullptr, {});
}

static std::uint16_t _camera_effector_playing_object_id = 0;

inline void run_cam_effector(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/run_cam_effector(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    bool is_loop = false;
    int value = 1000 + Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(1, 100);
    if (buffer.size() > 1)
    {
        value = boost::lexical_cast<int>(buffer[1]);
    }

    if (buffer.size() > 2)
    {
        if (buffer[2] == "true")
            is_loop = true;
    }

    if (value)
        Globals::Game::level::add_cam_effector(xr_string("camera_effects\\").append(buffer[0]).append(".anm").c_str(),
            value, is_loop, "xr_effects.cam_effector_callback");
    _camera_effector_playing_object_id = p_npc->ID();
}

inline void stop_cam_effector(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/stop_cam_effector(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    int id = boost::lexical_cast<int>(buffer[0]);
    if (id)
        Globals::Game::level::remove_cam_effector(id);
}

inline void run_cam_effector_global(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/run_cam_effector_global(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! "
            "Return ...");
        return;
    }

    int value = 1000 + Globals::Script_RandomInt::getInstance().Generate<int>(1, 100);

    if (buffer.size() > 1)
    {
        value = boost::lexical_cast<int>(buffer[1]);
    }

    float current_field_of_view = Device.fFOV;
    if (buffer.size() > 2)
    {
        current_field_of_view = boost::lexical_cast<float>(buffer[2]);
    }

    if (value)
        Globals::Game::level::add_cam_effector2(xr_string("camera_effects\\").append(buffer[0]).append(".anm").c_str(),
            value, false, "xr_effects.cam_effector_callback", current_field_of_view);
    _camera_effector_playing_object_id = p_npc->ID();
}

// Lord: просмотерть методы add_cam_effector там используется данный callback и исправить это!!!
inline void cam_effector_callback(void)
{
    if (!_camera_effector_playing_object_id)
    {
        return;
    }

    if (DataBase::Storage::getInstance().getStorage().find(_camera_effector_playing_object_id) ==
        DataBase::Storage::getInstance().getStorage().end())
        return;

    if (DataBase::Storage::getInstance()
            .getStorage()
            .at(_camera_effector_playing_object_id)
            .getActiveSchemeName()
            .empty())
        return;

    if (DataBase::Storage::getInstance()
            .getStorage()
            .at(_camera_effector_playing_object_id)
            .getSchemes()
            .at(DataBase::Storage::getInstance()
                    .getStorage()
                    .at(_camera_effector_playing_object_id)
                    .getActiveSchemeName())
            ->getSignals()
            .empty())
    {
        return;
    }

    DataBase::Storage::getInstance().setStorageSignal(_camera_effector_playing_object_id, "cameff_end", true);
}

inline void run_postprocess(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/run_postprocess(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (Globals::get_system_ini()->section_exist(buffer[0].c_str()))
    {
        int value = 2000 + Globals::Script_RandomInt::getInstance().Generate<int>(1, 100);

        if (buffer.size() > 1)
        {
            value = boost::lexical_cast<int>(buffer[1]);
        }

        if (value)
        {
            Msg("[Scripts/XR_EFFECTS/run_postprocess(p_actor, p_npc, buffer)] adding complex effector %s id %d from %s",
                buffer[0].c_str(), value, p_npc->Name());
            Globals::Game::level::add_complex_effector(buffer[0].c_str(), value);
        }
    }
    else
    {
        R_ASSERT2(false, "Complex effector section is not set!");
    }
}

inline void stop_postprocess(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/stop_postprocess(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    int value = boost::lexical_cast<int>(buffer[0]);

    if (value)
    {
        Msg("[Scripts/XR_EFFECTS/stop_postprocess(p_actor, p_npc, buffer)] removing complex effector id %s from %s",
            buffer[0].c_str(), p_npc->Name());
        Globals::Game::level::remove_complex_effector(value);
    }
}

inline void run_tutorial(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/run_tutorial(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    Msg("[Scripts/XR_EFFECTS/run_tutorial(p_actor, p_npc, buffer)] run tutorial is called!");
    Globals::Game::start_tutorial(buffer[0].c_str());
}

inline void jup_b32_place_scanner(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    for (std::uint32_t i = 1; i < 6; ++i)
    {
        xr_string zone_name = "jup_b32_sr_scanner_place_";
        xr_string to_spawn_item_name = "jup_b32_scanner_place_";
        zone_name += std::to_string(i).c_str();
        xr_string infoportion_name = "jup_b32_scanner_";
        infoportion_name += std::to_string(i).c_str();
        infoportion_name += "_placed";

        if (XR_CONDITION::is_actor_in_zone_client(p_actor, p_npc, {zone_name}) &&
            !Globals::has_alife_info(infoportion_name.c_str()))
        {
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion(infoportion_name.c_str());
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b32_tutorial_done");
            remove_item(p_actor, p_npc, {"jup_b32_scanner_device"});
            to_spawn_item_name += std::to_string(i).c_str();
            spawn_object(p_actor, nullptr, {"jup_b32_ph_scanner", to_spawn_item_name});
        }
    }
}

inline void jup_b32_pda_check(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    Globals::change_anomalies_names();
}

inline void pri_b306_generator_start(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (XR_CONDITION::is_actor_in_zone_client(p_actor, p_npc, {"pri_b306_sr_generator"}))
        DataBase::Storage::getInstance().getActor()->GiveInfoPortion("pri_b306_lift_generator_used");
}

inline void jup_b206_get_plant(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (XR_CONDITION::is_actor_in_zone_client(p_actor, p_npc, {"jup_b206_sr_quest_line"}))
    {
        DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b206_anomalous_grove_has_plant");
        give_actor(p_actor, p_npc, {"jup_b206_plant"});
        destroy_object(p_actor, p_npc, {"story", "jup_b206_plant_ph"});
    }
}

inline void pas_b400_switcher(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (XR_CONDITION::is_actor_in_zone_client(p_actor, p_npc, {"pas_b400_sr_switcher"}))
    {
        DataBase::Storage::getInstance().getActor()->GiveInfoPortion("pas_b400_switcher_use");
    }
}

inline void jup_b209_place_scanner(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (XR_CONDITION::is_actor_in_zone_client(p_actor, p_npc, {"jup_b209_hypotheses"}))
    {
        scenario_autosave(
            DataBase::Storage::getInstance().getActor(), nullptr, {"st_save_jup_b209_placed_mutant_scanner"});
        DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b209_scanner_placed");
        remove_item(p_actor, nullptr, {"jup_b209_monster_scanner"});
        spawn_object(p_actor, nullptr, {"jup_b209_ph_scanner", "jup_b209_scanner_place_point"});
    }
}

inline void jup_b9_heli_1_searching(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (XR_CONDITION::is_actor_in_zone_client(p_actor, p_npc, {"jup_b9_heli_1"}))
        DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b9_heli_1_searching");
}

inline void pri_a18_use_idol(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (XR_CONDITION::is_actor_in_zone_client(p_actor, p_npc, {"pri_a18_use_idol_restrictor"}))
    {
        DataBase::Storage::getInstance().getActor()->GiveInfoPortion("pri_a18_run_cam");
    }
}

inline void jup_b8_heli_4_searching(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (XR_CONDITION::is_actor_in_zone_client(p_actor, p_npc, {"jup_b8_heli_4"}))
    {
        DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b8_heli_4_searching");
    }
}

inline void jup_b10_ufo_searching(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (XR_CONDITION::is_actor_in_zone_client(p_actor, p_npc, {"jup_b10_ufo_restrictor"}))
    {
        DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b10_ufo_memory_started");
        give_actor(DataBase::Storage::getInstance().getActor(), nullptr, {"jup_b10_ufo_memory"});
    }
}

inline void zat_b101_heli_5_searching(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (XR_CONDITION::is_actor_in_zone_client(p_actor, p_npc, {"zat_b101_heli_5"}))
    {
        DataBase::Storage::getInstance().getActor()->GiveInfoPortion("zat_b101_heli_5_searching");
    }
}

inline void zat_b28_heli_3_searching(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (XR_CONDITION::is_actor_in_zone_client(p_actor, p_npc, {"zat_b28_heli_3"}))
    {
        DataBase::Storage::getInstance().getActor()->GiveInfoPortion("zat_b28_heli_3_searching");
    }
}

inline void zat_b100_heli_2_searching(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (XR_CONDITION::is_actor_in_zone_client(p_actor, p_npc, {"zat_b100_heli_2"}))
    {
        DataBase::Storage::getInstance().getActor()->GiveInfoPortion("zat_b100_heli_2_searching");
    }
}

inline void teleport_actor(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/teleport_actor(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    CPatrolPathParams patrol(buffer[0].c_str());
    float direction = 0.0f;
    if (buffer.size() > 1)
    {
        CPatrolPathParams look_point(buffer[1].c_str());
        Fvector temp = look_point.point(std::uint32_t(0));
        direction = -temp.sub(patrol.point(std::uint32_t(0))).getH();
        DataBase::Storage::getInstance().getActor()->SetActorDirection(direction);
    }

    for (const std::pair<xr_string, bool>& it : DataBase::Storage::getInstance().getNoWeaponZones())
    {
        CScriptGameObject* const p_zone = DataBase::Storage::getInstance().getZoneByName().at(it.first);

        if (Globals::Utils::is_npc_in_zone(DataBase::Storage::getInstance().getActor(), p_zone))
            DataBase::Storage::getInstance().setNoWeaponZones(it.first, true);
    }

    if (p_npc && p_npc->Name())
        Msg("[Scripts/XR_EFFECTS/teleport_actor(p_actor, p_npc, buffer)] teleporting actor from %s", p_npc->Name());

    DataBase::Storage::getInstance().getActor()->SetActorPosition(patrol.point(std::uint32_t(0)));
}

// @ Local function don't use it everywhere!!
inline void _reset_animation(CScriptGameObject* const p_npc)
{
    if (!p_npc)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    Script_StateManager* p_state_manager =
        DataBase::Storage::getInstance().getStorage().at(p_npc->ID()).getStateManager();

    if (!p_state_manager)
        return;

    CScriptActionPlanner* p_planner = Globals::get_script_action_planner(p_npc);

    p_state_manager->getAnimation()->set_state();
    p_state_manager->getAnimation()->set_control();
    p_state_manager->getAnimation()->set_state();
    p_state_manager->getAnimation()->set_control();

    StateManagerExtraData data;
    data.setFastSet(true);
    p_state_manager->set_state(
        "idle", StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(Fvector(), nullptr), data);

    p_state_manager->update();
    p_state_manager->update();
    p_state_manager->update();
    p_state_manager->update();
    p_state_manager->update();
    p_state_manager->update();
    p_state_manager->update();

    p_npc->set_body_state(eBodyStateStand);
    p_npc->set_mental_state(eMentalStateFree);
}

inline void teleport_npc(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/teleport_npc(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (!p_npc)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    std::uint32_t patrol_point_index = static_cast<std::uint32_t>(atoi(buffer[1].c_str()));
    _reset_animation(p_npc);
    p_npc->SetNpcPosition(CPatrolPathParams(buffer[0].c_str()).point(patrol_point_index));
}

inline void teleport_npc_by_story_id(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/teleport_npc_by_story_id(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! "
            "Return ...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/teleport_npc_by_story_id(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! Return "
            "...");
        return;
    }

    std::uint32_t patrol_point_index = 0;

    if (buffer.size() > 2)
        patrol_point_index = static_cast<std::uint32_t>(atoi(buffer[2].c_str()));

    Fvector position = CPatrolPathParams(buffer[1].c_str()).point(patrol_point_index);
    std::uint16_t npc_id = Globals::get_story_object_id(buffer[0]);

    if (!npc_id)
    {
        R_ASSERT2(false, "there is no story object");
        return;
    }

    CScriptGameObject* const p_client_object = Globals::Game::level::get_object_by_id(npc_id);

    if (p_client_object)
    {
        _reset_animation(p_client_object);
        p_client_object->SetNpcPosition(position);
    }
    else
    {
        ai().alife().objects().object(npc_id)->o_Position = position;
    }
}

inline void teleport_squad(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/teleport_squad(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/teleport_squad(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! Return ...");
        return;
    }

    std::uint32_t patrol_point_index = 0;

    if (buffer.size() > 2)
        patrol_point_index = static_cast<std::uint32_t>(atoi(buffer[2].c_str()));

    Fvector position = CPatrolPathParams(buffer[1].c_str()).point(patrol_point_index);
    Script_SE_SimulationSquad* const p_server_squad = Globals::get_story_squad(buffer[0]);

    if (!p_server_squad)
    {
        Msg("[Scripts/XR_EFFECTS/teleport_squad(p_actor, p_npc, buffer)] WARNING: p_sever_squad == nullptr! Can't find "
            "%s Return ...",
            buffer[0].c_str());
        return;
    }
}

inline void jup_teleport_actor(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    Fvector point_in = CPatrolPathParams("jup_b16_teleport_in").point(std::uint32_t(0));
    Fvector point_out = CPatrolPathParams("jup_b16_teleport_out").point(std::uint32_t(0));
    const Fvector& actor_position = p_actor->Position();
    Fvector result_position = Fvector().set(actor_position.x - (point_in.x + point_out.x),
        actor_position.y - (point_in.y + point_out.y), actor_position.z - (point_in.z + point_out.z));
    DataBase::Storage::getInstance().getActor()->SetActorPosition(result_position);
}

inline void give_items(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/give_items(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/give_items(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    for (const xr_string& it : buffer)
    {
        Globals::Game::alife_create(
            it, p_npc->Position(), p_npc->level_vertex_id(), p_npc->game_vertex_id(), p_npc->ID());
    }
}

inline void give_item(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/give_item(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/give_item(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    std::uint16_t npc_id = 0;
    if (buffer.size() > 1)
        npc_id = Globals::get_story_object_id(buffer[1]);
    else
        npc_id = p_npc->ID();

    CSE_Abstract* const p_server_object = ai().alife().objects().object(npc_id);

    Globals::Game::alife_create(buffer[0], p_server_object->o_Position,
        p_server_object->cast_alife_dynamic_object()->m_tNodeID,
        p_server_object->cast_alife_dynamic_object()->m_tGraphID, npc_id);
}

inline void play_particle_on_path(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/play_particle_on_path(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! "
            "Return ...");
        return;
    }

    std::uint32_t point_prob;
    if (buffer.size() < 2)
        point_prob = 100;
    else
        point_prob = static_cast<std::uint32_t>(atoi(buffer[2].c_str()));

    if (!point_prob)
        point_prob = 100;

    CPatrolPathParams path(buffer[1].c_str());

    for (std::uint32_t i = 0; i < path.count(); ++i)
    {
        CScriptParticles patricle(buffer[0].c_str());
        if (Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(1, 100) <= point_prob)
            patricle.PlayAtPos(path.point(i));
    }
}

inline void send_tip(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/send_tip(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (buffer.size() < 2 || buffer.size() < 3)
    {
        Msg("[Scripts/XR_EFFECTS/send_tip(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2 || buffer.size() < 3. "
            "Return ...");
        return;
    }

    Script_NewsManager::getInstance().SendTip(p_actor, buffer[0], buffer[1], 0, 0, buffer[2]);
}

inline void hit_npc(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/hit_npc(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (buffer.size() < 5)
    {
        Msg("[Scripts/XR_EFFECTS/hit_npc(p_actor, p_npc, buffer)] WARNING: buffer.size() < 5! Return ...");
        return;
    }

    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/hit_npc(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    bool is_rev = false;
    if (buffer.size() > 5)
        is_rev = (buffer[5] == "true");

    CScriptHit hit;
    hit.m_tpDraftsman = p_npc;
    hit.m_tHitType = ALife::EHitType::eHitTypeWound;
    if (buffer[0] != "self")
    {
        CScriptGameObject* const p_client_object = Globals::get_story_object(buffer[0]);
        if (!p_client_object)
            return;

        if (is_rev)
        {
            hit.m_tpDraftsman = p_client_object;
            hit.m_tDirection = p_client_object->Position().sub(p_npc->Position());
        }
        else
        {
            hit.m_tDirection = p_npc->Position().sub(p_client_object->Position());
        }
    }
    else
    {
        if (is_rev)
        {
            hit.m_tpDraftsman = nullptr;
            hit.m_tDirection = p_npc->Position().sub(CPatrolPathParams(buffer[1].c_str()).point(std::uint32_t(0)));
        }
        else
        {
            Fvector data = CPatrolPathParams(buffer[1].c_str()).point(std::uint32_t(0));
            hit.m_tDirection = data.sub(p_npc->Position());
        }
    }

    hit.set_bone_name(buffer[2].c_str());
    hit.m_fPower = boost::lexical_cast<float>(buffer[3]);
    hit.m_fImpulse = boost::lexical_cast<float>(buffer[4]);

    Msg("[Scripts/XR_EFFECTS/hit_npc(p_actor, p_npc, buffer)] hit effect %s %s %f %f %f", p_npc->Name(),
        buffer[1].c_str(), hit.m_fPower, hit.m_fImpulse, p_npc->GetHealth());

    p_npc->Hit(&hit);
}

inline void hit_obj(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/hit_obj(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (buffer.size() < 4)
    {
        Msg("[Scripts/XR_EFFECTS/hit_obj(p_actor, p_npc, buffer)] WARNING: buffer.size() < 5! Return ...");
        return;
    }

    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/hit_obj(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    CScriptHit hit;
    CScriptGameObject* const p_client_object = Globals::get_story_object(buffer[0]);

    if (!p_client_object)
    {
        Msg("[Scripts/XR_EFFECTS/hit_obj(p_actor, p_npc, buffer)] WARNING: p_client_object == nullptr! Return ...");
        return;
    }

    hit.set_bone_name(buffer[1].c_str());
    hit.m_fPower = boost::lexical_cast<float>(buffer[2]);
    hit.m_fImpulse = boost::lexical_cast<float>(buffer[3]);

    if (buffer.size() > 4)
    {
        hit.m_tDirection =
            Fvector().sub(CPatrolPathParams(buffer[4].c_str()).point(std::uint32_t(0)), p_client_object->Position());
    }
    else
    {
        hit.m_tDirection = Fvector().sub(p_npc->Position(), p_client_object->Position());
    }

    hit.m_tpDraftsman = p_npc;
    hit.m_tHitType = ALife::EHitType::eHitTypeWound;

    p_client_object->Hit(&hit);
}

inline void hit_by_killer(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/hit_by_killer(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/hit_by_killer(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    std::uint16_t killer_id =
        DataBase::Storage::getInstance().getStorage().at(p_npc->ID()).getDeathData().getKillerID();

    if (!killer_id || killer_id == Globals::kUnsignedInt16Undefined)
    {
        Msg("[Scripts/XR_EFFECTS/hit_by_killer(p_actor, p_npc, buffer)] WARNING: killer_id is undefined. Return ...");
        return;
    }

    const DataBase::Storage_Data& storage_killer = DataBase::Storage::getInstance().getStorage().at(killer_id);

    if (!storage_killer.getClientObject())
    {
        Msg("[Scripts/XR_EFFECTS/hit_by_killer(p_actor, p_npc, buffer)] WARNING: can't obtain client object!");
        return;
    }

    Fvector point1 = p_npc->Position();
    Fvector point2 = storage_killer.getClientObject()->Position();

    CScriptHit hit;
    hit.m_tpDraftsman = p_npc;
    hit.m_tHitType = ALife::EHitType::eHitTypeWound;
    hit.m_tDirection = point1.sub(point2);
    hit.set_bone_name(buffer[0].c_str());
    hit.m_fPower = boost::lexical_cast<float>(buffer[1]);
    hit.m_fImpulse = boost::lexical_cast<float>(buffer[2]);

    p_npc->Hit(&hit);
}

inline void hit_npc_from_actor(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_actor)
    {
        Msg("[Scripts/XR_EFFECTS/hit_npc_from_actor(p_actor, p_npc, buffer)] WARNING: p_actor == nullptr! Return ...");
        return;
    }

    CScriptHit hit;
    hit.m_tpDraftsman = p_actor;
    hit.m_tHitType = ALife::EHitType::eHitTypeWound;
    CScriptGameObject* p_client_object = nullptr;
    if (!buffer.empty())
        p_client_object = Globals::get_story_object(buffer[0]);

    if (p_client_object)
        hit.m_tDirection = p_actor->Position().sub(p_client_object->Position());
    else
        hit.m_tDirection = p_actor->Position().sub(p_npc->Position());

    if (buffer.empty())
        hit.m_tDirection = p_actor->Position().sub(p_npc->Position());

    hit.set_bone_name("bip01_spine");
    hit.m_fPower = 0.001f;
    hit.m_fImpulse = 0.001f;
    p_npc->Hit(&hit);
}

inline void restore_health(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/restore_health(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    p_npc->SetHealth(1.0f);
}

inline void make_enemy(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/make_enemy(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    CScriptGameObject* p_hitted_npc = p_npc;
    CScriptHit hit;
    hit.m_tpDraftsman = Globals::get_story_object(buffer[0]);

    if (buffer.size() > 1)
        p_hitted_npc = Globals::get_story_object(buffer[0]);

    hit.m_tHitType = ALife::EHitType::eHitTypeWound;
    hit.m_tDirection = hit.m_tpDraftsman->Position().sub(p_hitted_npc->Position());
    hit.set_bone_name("bip01_spine");
    hit.m_fPower = 0.03f;
    hit.m_fImpulse = 0.03f;
    p_hitted_npc->Hit(&hit);
}

inline void sniper_fire_mode(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/sniper_fire_mode(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
    }
    else
    {
        if (buffer[0] == "true")
            p_npc->sniper_fire_mode(true);
    }

    p_npc->sniper_fire_mode(false);
}

inline void kill_npc(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    CScriptGameObject* p_client_object = p_npc;
    if (!buffer.empty())
        p_client_object = Globals::get_story_object(buffer[0]);

    if (p_client_object && p_client_object->Alive())
        p_client_object->Kill(p_client_object, false);
}

inline void remove_npc(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    std::uint16_t npc_id = 0;

    if (!buffer.empty())
        npc_id = Globals::get_story_object_id(buffer[0]);

    if (npc_id)
        Globals::Game::alife_release(ai().alife().objects().object(npc_id), true);
}

inline void inc_counter(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/inc_counter(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (!p_actor)
    {
        Msg("[Scripts/XR_EFFECTS/inc_counter(p_actor, p_npc, buffer)] WARNING: p_actor == nullptr! Return ...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/inc_counter(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! Return ...");
        return;
    }

    std::uint8_t inc_value = static_cast<std::uint8_t>(atoi(buffer[1].c_str()));
    if (!inc_value)
        inc_value = 1;

    std::uint8_t new_value = XR_LOGIC::pstor_retrieve_number(p_actor, buffer[0]) + inc_value;

    if (p_npc && p_npc->Name())
        Msg("[Scripts/XR_EFFECTS/inc_counter(p_actor, p_npc, buffer)] inc_counter %s to value %s by %s",
            buffer[0].c_str(), std::to_string(new_value).c_str(), p_npc->Name());

    XR_LOGIC::pstor_store(p_actor, buffer[0], std::to_string(new_value).c_str());
}

inline void dec_counter(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/dec_counter(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (!p_actor)
    {
        Msg("[Scripts/XR_EFFECTS/dec_counter(p_actor, p_npc, buffer)] WARNING: p_actor == nullptr! Return ...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/dec_counter(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! Return ...");
        return;
    }

    std::uint8_t dec_value = static_cast<std::uint8_t>(atoi(buffer[1].c_str()));
    if (!dec_value)
        dec_value = 1;

    int new_value = XR_LOGIC::pstor_retrieve_number(p_actor, buffer[0]) - dec_value;

    if (new_value < 0)
        new_value = 0;

    XR_LOGIC::pstor_store(p_actor, buffer[0], std::to_string(new_value).c_str());

    if (p_npc && p_npc->Name())
        Msg("[Scripts/XR_EFFECTS/dec_counter(p_actor, p_npc, buffer)] dec_counter %s value %s by %s", buffer[0].c_str(),
            std::to_string(new_value).c_str(), p_npc->Name());
}

inline void set_counter(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/set_counter(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (!p_actor)
    {
        Msg("[Scripts/XR_EFFECTS/set_counter(p_actor, p_npc, buffer)] WARNING: p_actor == nullptr! Return ...");
        return;
    }

    xr_string value_name = (atoi(buffer[0].c_str())) ? std::to_string(atoi(buffer[0].c_str())).c_str() : "0";

    XR_LOGIC::pstor_store(p_actor, buffer[0], value_name);
}

// @ Используется только аргумент p_npc
inline void actor_punch(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/actor_punch(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    Globals::set_inactivate_input_time(30);
    Globals::Game::level::add_cam_effector("camera_effects\\fusker.anm", 999, false, "");

    std::uint32_t current_slot = DataBase::Storage::getInstance().getActor()->active_slot();

    if (current_slot != 2 && current_slot != 3)
    {
        Msg("[Scripts/XR_EFFECTS/actor_punch(p_actor, p_npc, buffer)] current_slot != 2 and current_slot != 3 Return "
            "...");
        return;
    }

    CScriptGameObject* const p_active_item = DataBase::Storage::getInstance().getActor()->GetActiveItem();

    if (p_active_item)
        DataBase::Storage::getInstance().getActor()->DropItem(p_active_item);
}

inline void clearAbuse(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/clearAbuse(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    Script_SchemeXRAbuse::clear_abuse(p_npc);
    Msg("[Scripts/XR_EFFECTS/clearAbuse(p_actor, p_npc, buffer)] abuse is cleared!");
}

inline void turn_off_underpass_lamps(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    xr_map<xr_string, bool> lamps;

    lamps["pas_b400_lamp_start_flash"] = true;
    lamps["pas_b400_lamp_start_red"] = true;
    lamps["pas_b400_lamp_elevator_green"] = true;
    lamps["pas_b400_lamp_elevator_flash"] = true;
    lamps["pas_b400_lamp_elevator_green_1"] = true;
    lamps["pas_b400_lamp_elevator_flash_1"] = true;
    lamps["pas_b400_lamp_track_green"] = true;
    lamps["pas_b400_lamp_track_flash"] = true;
    lamps["pas_b400_lamp_downstairs_green"] = true;
    lamps["pas_b400_lamp_downstairs_flash"] = true;
    lamps["pas_b400_lamp_tunnel_green"] = true;
    lamps["pas_b400_lamp_tunnel_flash"] = true;
    lamps["pas_b400_lamp_tunnel_green_1"] = true;
    lamps["pas_b400_lamp_tunnel_flash_1"] = true;
    lamps["pas_b400_lamp_control_down_green"] = true;
    lamps["pas_b400_lamp_control_down_flash"] = true;
    lamps["pas_b400_lamp_control_up_green"] = true;
    lamps["pas_b400_lamp_control_up_flash"] = true;
    lamps["pas_b400_lamp_hall_green"] = true;
    lamps["pas_b400_lamp_hall_flash"] = true;
    lamps["pas_b400_lamp_way_green"] = true;
    lamps["pas_b400_lamp_way_flash"] = true;

    for (const std::pair<xr_string, bool>& it : lamps)
    {
        CScriptGameObject* const p_client_object = Globals::get_story_object(it.first);

        if (p_client_object)
            p_client_object->get_hanging_lamp()->TurnOff();
        else
            Msg("[Scripts/XR_EFFECTS/turn_off_underpass_lamps(p_actor, p_npc, buffer)] can't turn off because object "
                "is nullptr!");
    }
}

inline void turn_off(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/turn_off(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    for (const xr_string& it : buffer)
    {
        CScriptGameObject* const p_client_object = Globals::get_story_object(it);

        if (!p_client_object)
        {
            Msg("[Scripts/XR_EFFECTS/turn_off(p_actor, p_npc, buffer)] WARNIN: p_client_object == nullptr! Return ...");
            return;
        }

        p_client_object->get_hanging_lamp()->TurnOff();
    }
}

inline void turn_off_object(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFFECTS/turn_off_object(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    p_npc->get_hanging_lamp()->TurnOff();
}

inline void turn_on(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/turn_on(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    for (const xr_string& it : buffer)
    {
        CScriptGameObject* const p_client_object = Globals::get_story_object(it);

        if (!p_client_object)
        {
            Msg("[Scripts/XR_EFFECTS/turn_on(p_actor, p_npc, buffer)] WARNING: p_client_object == nullptr! Return ...");
            return;
        }

        p_client_object->get_hanging_lamp()->TurnOn();
    }
}

inline void turn_on_and_force(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/turn_on_and_force(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    CScriptGameObject* const p_client_object = Globals::get_story_object(buffer[0]);

    if (!p_client_object)
    {
        Msg("[Scripts/XR_EFFECTS/turn_on_and_force(p_actor, p_npc, buffer)] WARNING: p_client_object == nullptr! "
            "Return ...");
        return;
    }

    float value = 55.0f;
    std::uint32_t time_interval = 14000;

    if (buffer.size() >= 2 && !fis_zero(atof(buffer[1].c_str())))
        value = atof(buffer[1].c_str());

    if (buffer.size() >= 3 && atoi(buffer[2].c_str()) != 0)
        value = static_cast<std::uint32_t>(atoi(buffer[2].c_str()));

    p_client_object->set_const_force(Fvector().set(0.0f, 1.0f, 0.0f), value, time_interval);
    p_client_object->start_particles("weapons\\light_signal", "link");
    p_client_object->get_hanging_lamp()->TurnOn();
}

inline void turn_off_and_force(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/turn_off_and_force(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    CScriptGameObject* const p_client_object = Globals::get_story_object(buffer[0]);

    if (!p_client_object)
    {
        Msg("[Scripts/XR_EFFECTS/turn_off_and_force(p_actor, p_npc, buffer)] WARNING: p_client_object == nullptr! "
            "Return ...");
        return;
    }

    p_client_object->stop_particles("weapons\\light_signal", "link");
    p_client_object->get_hanging_lamp()->TurnOff();
}

inline void turn_on_object(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/turn_on_object(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    p_npc->get_hanging_lamp()->TurnOn();
}

inline void disable_combat_handler(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/disable_combat_handler(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return "
            "...");
        return;
    }

    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(p_npc->ID());
    if (storage.getSchemes().find("combat") != storage.getSchemes().end())
    {
        storage.getSchemes().at("combat")->setEnabled(false);
    }

    if (storage.getSchemes().find("mob_combat") != storage.getSchemes().end())
    {
        storage.getSchemes().at("mob_combat")->setEnabled(false);
    }
}

inline void disable_combat_ignore_handler(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/disable_combat_ignore_handler(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! "
            "Return ...");
        return;
    }

    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(p_npc->ID());

    if (storage.getSchemes().find("combat_ignore") != storage.getSchemes().end())
        storage.getSchemes().at("combat_ignore")->setEnabled(false);
}

inline void game_disconnect(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    Console->Execute("disconnect");
}

inline void game_credits(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    Script_GlobalHelper::getInstance().setGameOverCreditsStarted(true);
    Globals::Game::start_tutorial("credits_seq");
}

inline void game_over(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!Script_GlobalHelper::getInstance().isGameOverCreditsStarted())
    {
        Msg("[Scripts/XR_EFFECTS/game_over(p_actor, p_npc, buffer)] WARNING: flag is not true! Return ...");
        return;
    }

    Console->Execute("main_menu on");
}

inline void after_credits(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    Console->Execute("main_menu on");
}

inline void before_credits(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    Console->Execute("main_menu off");
}

inline void on_tutor_gameover_stop(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    Console->Execute("main_menu on");
}

inline void on_tutor_gameover_quickload(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    Console->Execute("load_last_save");
}

inline void switch_to_desired_job(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    XR_GULAG::switch_to_desired_job(p_npc);
}

inline void remove_item(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/remove_item(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    CScriptGameObject* const p_finding_object =
        DataBase::Storage::getInstance().getActor()->GetObjectByName(buffer[0].c_str());
    if (p_finding_object)
    {
        Globals::Game::alife_release(ai().alife().objects().object(p_finding_object->ID()), true);
    }
    else
    {
        Msg("[Scripts/XR_EFFECTS/remove_item(p_actor, p_npc, buffer)] WARNING: actor doesnt have item -> %s",
            buffer[0].c_str());
    }

    Script_NewsManager::getInstance().relocate_item("out", buffer[0].c_str());
}

static bool _is_actor_nightvision_disabled = false;
static bool _is_actor_torch_disabled = false;
inline void disable_actor_nightvision(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    CScriptGameObject* const p_nightvision =
        DataBase::Storage::getInstance().getActor()->GetObjectByName("device_torch");

    if (p_nightvision && p_nightvision->night_vision_enabled())
    {
        p_nightvision->enable_night_vision(false);
        _is_actor_nightvision_disabled = true;
    }
}

inline void enable_actor_nightvision(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    CScriptGameObject* const p_nightvision =
        DataBase::Storage::getInstance().getActor()->GetObjectByName("device_torch");

    if (p_nightvision && !p_nightvision->night_vision_enabled() && _is_actor_nightvision_disabled)
    {
        p_nightvision->enable_night_vision(true);
        _is_actor_nightvision_disabled = false;
    }
}

inline void disable_actor_torch(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    CScriptGameObject* const p_torch = DataBase::Storage::getInstance().getActor()->GetObjectByName("device_torch");

    if (p_torch && p_torch->torch_enabled())
    {
        p_torch->enable_torch(false);
        _is_actor_torch_disabled = true;
    }
}

inline void enable_actor_torch(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    CScriptGameObject* const p_torch = DataBase::Storage::getInstance().getActor()->GetObjectByName("device_torch");

    if (p_torch && !p_torch->torch_enabled() && _is_actor_torch_disabled)
    {
        p_torch->enable_torch(true);
        _is_actor_torch_disabled = false;
    }
}

inline void spawn_object(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/spawn_object(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/spawn_object(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    xr_string spawn_section_name = buffer[0];
    xr_string path_name = buffer[1];

    if (!Globals::patrol_path_exists(path_name.c_str()))
    {
        Msg("[Scripts/XR_EFFECTS/spawn_object(p_actor, p_npc, buffer)] WARNING: patrol doesn't exist. CAN NOT SPAWN "
            "ITEM %s %s",
            path_name.c_str(), spawn_section_name.c_str());
        return;
    }

    CPatrolPathParams patrol(path_name.c_str());

    int index = 0;

    if (buffer.size() > 2)
        index = boost::lexical_cast<int>(buffer[2]);

    float yaw = 0.0f;

    if (buffer.size() > 3)
        yaw = boost::lexical_cast<float>(buffer[3]);

    CSE_Abstract* p_server_object = Globals::Game::alife_create(
        spawn_section_name, patrol.point(index), patrol.level_vertex_id(0), patrol.game_vertex_id(0));

    if (Globals::IsStalker((CSE_ALifeDynamicObject*)(nullptr), p_server_object->script_clsid()))
    {
        p_server_object->cast_human_abstract()->o_torso.yaw = yaw * PI / 180.f;
    }
    else if (p_server_object->script_clsid() == CLSID_SE_PHYSICS_OBJECT)
    {
        p_server_object->o_Angle.y = yaw * PI / 180.0f;
    }
}

inline void give_actor(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/give_actor(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    CScriptGameObject* const p_client_actor = DataBase::Storage::getInstance().getActor();
    for (const xr_string& it : buffer)
    {
        Globals::Game::alife_create(it, p_client_actor->Position(), p_client_actor->level_vertex_id(),
            p_client_actor->game_vertex_id(), p_client_actor->ID());
        Script_NewsManager::getInstance().relocate_item("in", it);
    }
}

inline void destroy_object(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    CSE_Abstract* p_server_object = nullptr;
    if (buffer.empty())
    {
        p_server_object = ai().alife().objects().object(p_npc->ID());
    }
    else
    {
        if (buffer.size() < 2)
        {
            R_ASSERT2(false, "not right ...");
            return;
        }

        xr_string target_str;
        if (buffer.size() > 2)
        {
            target_str = buffer[0];
            target_str += "|";
            target_str += buffer[1];
            target_str += ",";
            target_str += buffer[2];
        }
        else
        {
            target_str = buffer[0];
            target_str += "|";
            target_str += buffer[1];
        }
        Fvector target_positon;
        std::uint16_t target_id = 0;
        bool is_target_init = false;
        init_target(p_npc, target_str, target_positon, target_id, is_target_init);
        if (!target_id)
        {
            R_ASSERT2(false, "You are trying to set non-existant target");
            return;
        }
        p_server_object = ai().alife().objects().object(target_id);
    }

    if (!p_server_object)
    {
        Msg("[Scripts/XR_EFFECTS/destroy_object(p_actor, p_npc, buffer)] WARNING: p_server_object == nullptr! Return "
            "...");
        return;
    }

    Msg("[Scripts/XR_EFFECTS/destory_object(p_actor, p_npc, buffer)] releasing object %s",
        p_server_object->name_replace());
    Globals::Game::alife_release(p_server_object, true);
}

inline void scenario_autosave(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/scenario_autosave(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    if (Globals::isImportantSave())
    {
        xr_string save_name = "save ";
        save_name += Core.UserName;
        save_name += " - ";
        save_name += Globals::Game::translate_string(buffer[0].c_str());

        Console->Execute(save_name.c_str());
    }
}

static Fvector jup_b219_position;
static std::uint32_t jup_b219_level_vertex_id = 0;
static std::uint16_t jup_b219_game_vertex_id = 0;

inline void jup_b219_save_pos(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    CScriptGameObject* const p_client_object = Globals::get_story_object("jup_b219_gate_id");

    if (p_client_object)
    {
        jup_b219_position = p_client_object->Position();
        jup_b219_level_vertex_id = p_client_object->level_vertex_id();
        jup_b219_game_vertex_id = p_client_object->game_vertex_id();
    }
    else
    {
        Msg("[Scripts/XR_EFFECTS/jup_b219_save_pos(p_actor, p_npc, buffer)] WARNING: p_client_object == nullptr! "
            "Return ...");
        return;
    }

    CSE_Abstract* p_server_object = ai().alife().objects().object(p_client_object->ID());
    if (p_client_object)
        Globals::Game::alife_release(p_server_object, true);
}

inline void jup_b219_restore_gate(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    float yaw = 0.0f;
    xr_string spawn_section_name = "jup_b219_gate";

    if (!Globals::is_vector_nil(jup_b219_position))
    {
        CSE_Abstract* p_server_object = Globals::Game::alife_create(
            spawn_section_name, jup_b219_position, jup_b219_level_vertex_id, jup_b219_game_vertex_id);

        p_server_object->o_Angle.y = yaw * PI / 180.0f;
    }
}

inline void spawn_corpse(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/spawn_corpse(p_actor, p_npc, buffer)] WARNIING: buffer.empty() == true! Return ...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/spawn_corpse(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! Return ...");
        return;
    }

    xr_string spawn_section_name = buffer[0];
    xr_string path_name = buffer[1];

    if (!Globals::patrol_path_exists(path_name.c_str()))
    {
        Msg("[Scripts/XR_EFFECTS/spawn_corpse(p_actor, p_npc, buffer)] WARNING: path_name doesn't exist! %s",
            path_name.c_str());
        return;
    }

    CPatrolPathParams patrol(path_name.c_str());

    std::uint32_t index = 0;
    if (buffer.size() >= 3)
        index = static_cast<std::uint32_t>(atoi(buffer[2].c_str()));

    CSE_Abstract* const p_server_object = Globals::Game::alife_create(
        spawn_section_name, patrol.point(index), patrol.level_vertex_id(0), patrol.game_vertex_id(0));

    CSE_ALifeHumanAbstract* const p_human = p_server_object->cast_human_abstract();

    if (!p_human)
    {
        Msg("[Scripts/XR_EFFECTS/spawn_corpse(p_actor, p_npc, buffer)] WARNING: server object can't cast to alive "
            "human/creature and kill it! Check your spawn section -> %s",
            spawn_section_name.c_str());
        return;
    }

    p_human->kill();
}

inline void spawn_object_in(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/spawn_object_in(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/spawn_object_in(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! Return ...");
        return;
    }

    xr_string spawn_section_name = buffer[0];

    std::uint16_t target_id = Globals::get_story_object_id(buffer[1]);

    if (target_id)
    {
        CSE_Abstract* const p_server_object = ai().alife().objects().object(target_id);
        if (!p_server_object)
        {
            Msg("[Scripts/XR_EFFECTS/spawn_object_in(p_actor, p_npc, buffer)] WARNING: p_server_object == nullptr! "
                "Return ...");
            return;
        }

        Globals::Game::alife_create(spawn_section_name, Fvector(), 0, 0, target_id);
        return;
    }

    Msg("[Scripts/XR_EFFECTS/spawn_object_in(p_actor, p_npc, buffer)] WARNING: can't find object id by %s",
        buffer[1].c_str());
}

inline void anim_obj_forward(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/anim_obj_forward(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    for (const xr_string& it : buffer)
    {
        DataBase::Storage::getInstance().getAnimationObjectsByName().at(it)->animation_forward();
    }
}

inline void anim_obj_backward(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/anim_obj_backward(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    DataBase::Storage::getInstance().getAnimationObjectsByName().at(buffer[0])->animation_backward();
}

inline void anim_obj_stop(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/anim_obj_stop(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    DataBase::Storage::getInstance().getAnimationObjectsByName().at(buffer[0])->animation_stop();
}

inline void play_sound(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/play_sound(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    xr_string theme_name = buffer[0];
    xr_string faction_name;

    if (buffer.size() > 1)
        faction_name = buffer[1];

    Script_SE_SmartTerrain* p_server_smart = nullptr;
    std::uint16_t point = 0;

    if (buffer.size() > 2)
        p_server_smart = Script_SimulationBoard::getInstance().getSmartTerrainsByName().at(buffer[2]);

    if (p_server_smart)
    {
        point = p_server_smart->ID;
    }
    else
    {
        if (buffer.size() > 3)
            point = static_cast<std::uint16_t>(atoi(buffer[2].c_str()));
    }

    if (p_npc && Globals::IsStalker(p_npc, 0))
    {
        if (!p_npc->Alive())
        {
            Msg("[Scripts/XR_EFFECTS/play_sound(p_actor, p_npc, buffer)] WARNING: stalker is dead!");
            return;
        }
    }

    XR_SOUND::set_sound_play(p_npc->ID(), theme_name, faction_name, point);
}

inline void play_sound_by_story(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/play_sound_by_story(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/play_sound_by_story(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! Return ...");
        return;
    }

    std::uint16_t story_id = Globals::get_story_object_id(buffer[0]);
    xr_string theme_name = buffer[1];

    xr_string faction_name;

    if (buffer.size() > 2)
        faction_name = buffer[2];

    Script_SE_SmartTerrain* p_server_smart = nullptr;

    if (buffer.size() > 3)
        p_server_smart = Script_SimulationBoard::getInstance().getSmartTerrainsByName().at(buffer[3]);

    std::uint16_t point = 0;
    if (p_server_smart)
    {
        point = p_server_smart->ID;
    }
    else
    {
        if (buffer.size() > 4)
            point = static_cast<std::uint16_t>(atoi(buffer[4].c_str()));
    }

    XR_SOUND::set_sound_play(story_id, theme_name, faction_name, point);
}

inline void stop_sound(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/stop_sound(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    XR_SOUND::stop_sounds_by_id(p_npc->ID());
}

inline void play_sound_looped(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/play_sound_looper(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/play_sound_looped(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    xr_string theme_name = buffer[0];
    XR_SOUND::play_sound_looped(p_npc->ID(), theme_name);
}

inline void stop_sound_looped(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/stop_sound_looped(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    XR_SOUND::stop_sound_looped(p_npc->ID(), "");
}

inline void barrel_explode(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/barrel_explode(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    CScriptGameObject* const p_object = Globals::get_story_object(buffer[0]);

    if (p_object)
        p_object->explode(0);
}

inline void create_squad(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/create_squad(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/create_squad(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/create_squad(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! Return ...");
        return;
    }

    std::uint16_t squad_id = static_cast<std::uint16_t>(atoi(buffer[0].c_str()));
    if (!squad_id)
    {
        Msg("[Scripts/XR_EFFECTS/create_squad(p_actor, p_npc, buffer)] squad_id == 0! Return ...");
        return;
    }

    xr_string smart_terrain_name = buffer[1];

    const CInifile* const p_ini = Globals::get_system_ini();

    if (!p_ini->section_exist(buffer[0].c_str()))
    {
        Msg("[Scripts/XR_EFFECTS/create_squad(p_actor, p_npc, buffer)] WARNING: section doesn't exist %s",
            buffer[0].c_str());
        return;
    }

    Script_SE_SmartTerrain* const p_server_smart =
        Script_SimulationBoard::getInstance().getSmartTerrainsByName().at(smart_terrain_name);

    if (!p_server_smart)
    {
        Msg("[Scripts/XR_EFFECTS/create_squad(p_actor, p_npc, buffer)] WARNING: p_server_smart == nullptr! Return ...");
        return;
    }

    Script_SE_SimulationSquad* const p_server_squad =
        Script_SimulationBoard::getInstance().create_squad(p_server_smart, buffer[0]);
    Script_SimulationBoard::getInstance().enter_squad_to_smart(p_server_squad, p_server_smart->ID);

    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
             p_server_squad->squad_members().begin();
         it != p_server_squad->squad_members().end(); ++it)
    {
        Script_SimulationBoard::getInstance().setup_squad_and_group(it->second);
    }

    p_server_squad->update();
}

inline void create_squad_member(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/create_squad_member(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/create_squad_member(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! Return ...");
        return;
    }

    xr_string squad_member_section_name = buffer[0];
    xr_string squad_id_name = buffer[1];

    Script_SE_SimulationSquad* const p_server_squad = Globals::get_story_squad(squad_id_name);
    Script_SE_SmartTerrain* const p_server_smart =
        Script_SimulationBoard::getInstance().getSmarts().at(p_server_squad->ID).getServerSmartTerrain();

    Fvector position;
    std::uint32_t level_vertex_id;
    std::uint16_t game_vertex_id;
    if (buffer.size() > 2)
    {
        xr_string picked_section_name;
        xr_map<std::uint32_t, CondlistData> spawn_point_condlist;
        xr_string spawn_point_name;

        if (buffer[2] == "simulation_point")
        {
            spawn_point_name =
                Globals::Utils::cfg_get_string(Globals::get_system_ini(), p_server_squad->name(), "spawn_point");
            if (spawn_point_name.empty())
                spawn_point_condlist = XR_LOGIC::parse_condlist_by_script_object(
                    "spawn_point", "spawn_point", p_server_smart->getSpawnPointName());
            else
                spawn_point_condlist =
                    XR_LOGIC::parse_condlist_by_script_object("spawn_point", "spawn_point", spawn_point_name);

            picked_section_name = XR_LOGIC::pick_section_from_condlist(
                DataBase::Storage::getInstance().getActor(), p_npc, spawn_point_condlist);
        }
        else
        {
            spawn_point_name = buffer[2];
        }

        CPatrolPathParams patrol(spawn_point_name.c_str());

        position = patrol.point(static_cast<std::uint32_t>(0));
        level_vertex_id = patrol.level_vertex_id(0);
        game_vertex_id = patrol.game_vertex_id(0);
    }
    else
    {
        CSE_ALifeDynamicObject* const p_server_commander =
            ai().alife().objects().object(p_server_squad->commander_id());
        position = p_server_commander->o_Position;
        level_vertex_id = p_server_commander->m_tNodeID;
        game_vertex_id = p_server_commander->m_tGraphID;
    }

    std::uint16_t new_member_id =
        p_server_squad->add_squad_member(squad_member_section_name, position, level_vertex_id, game_vertex_id);
    p_server_squad->assign_squad_member_to_smart(new_member_id, p_server_smart, 0);
    Script_SimulationBoard::getInstance().setup_squad_and_group(ai().alife().objects().object(new_member_id));
    p_server_squad->update();
}

inline void remove_squad(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/remove_squad(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    Script_SE_SimulationSquad* const p_server_squad = Globals::get_story_squad(buffer[0]);
    if (!p_server_squad)
    {
        Msg("[Scripts/XR_EFFECTS/remove_squad(p_actor, p_npc, buffer)] WARNING: can't find squad by %s",
            buffer[0].c_str());
        return;
    }

    Script_SimulationBoard::getInstance().remove_squad(p_server_squad);
}

inline void kill_squad(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/kill_squad(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    Script_SE_SimulationSquad* const p_server_squad = Globals::get_story_squad(buffer[0]);
    if (!p_server_squad)
    {
        Msg("[Scripts/XR_EFFECTS/kill_squad(p_actor, p_npc, buffer)] WARNING: can't find squad by %s Return ...",
            buffer[0].c_str());
        return;
    }

    CScriptGameObject* p_client_object = nullptr;
    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
             p_server_squad->squad_members().begin();
         it != p_server_squad->squad_members().end(); ++it)
    {
        if (DataBase::Storage::getInstance().getStorage().find(it->first) !=
            DataBase::Storage::getInstance().getStorage().end())
        {
            p_client_object = DataBase::Storage::getInstance().getStorage().at(it->first).getClientObject();
        }

        if (p_client_object)
            p_client_object->Kill(p_client_object);
        else
            it->second->kill(); // Lord: проверить правильно ли?
    }
}

inline void heal_squad(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/heal_squad(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    xr_string story_id_name = buffer[0];
    float health_mod = 1.0f;

    if (buffer.size() >= 2)
    {
        health_mod = ::ceilf(atof(buffer[1].c_str()) / 100.0f);
    }

    Script_SE_SimulationSquad* const p_server_squad = Globals::get_story_squad(story_id_name);

    if (!p_server_squad)
    {
        Msg("[Scripts/XR_EFFECTS/heal_squad(p_actor, p_npc, buffer)] WARNING: can't find server squad by %s",
            story_id_name.c_str());
        return;
    }

    CScriptGameObject* p_client_object = nullptr;
    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
             p_server_squad->squad_members().begin();
         it != p_server_squad->squad_members().end(); ++it)
    {
        if (DataBase::Storage::getInstance().getStorage().find(it->first) !=
            DataBase::Storage::getInstance().getStorage().end())
        {
            p_client_object = DataBase::Storage::getInstance().getStorage().at(it->first).getClientObject();
        }

        if (p_client_object)
            p_client_object->SetHealth(health_mod);
    }
}

inline void clear_smart_terrain(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/clear_smart_terrain(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    Script_SE_SmartTerrain* const p_server_smart =
        Script_SimulationBoard::getInstance().getSmartTerrainsByName().at(buffer[0]);
    std::uint16_t smart_id = p_server_smart->ID;

    for (const std::pair<std::uint16_t, Script_SE_SimulationSquad*>& it :
        Script_SimulationBoard::getInstance().getSmarts().at(smart_id).getSquads())
    {
        if (buffer.size() >= 2 && buffer[1] == "false")
        {
            if (Globals::get_object_story_id(it.second->ID).empty())
            {
                Script_SimulationBoard::getInstance().exit_smart(it.second, smart_id);
                Script_SimulationBoard::getInstance().remove_squad(it.second);
            }
        }
        else
        {
            Script_SimulationBoard::getInstance().exit_smart(it.second, smart_id);
            Script_SimulationBoard::getInstance().remove_squad(it.second);
        }
    }
}

inline void give_task(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/give_task(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    Script_TaskManager::getInstance().give_task(buffer[0]);
}

inline void actor_friend(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_actor)
    {
        Msg("[Scripts/XR_EFFECTS/actor_friend(p_actor, p_npc, buffer)] WARNING: p_actor == nullptr! Return ...");
        return;
    }

    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/actor_friend(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    p_npc->ForceSetGoodwill(1000, p_actor);
}

inline void actor_neutral(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_actor)
    {
        Msg("[Scripts/XR_EFFECTS/actor_neutral(p_actor, p_npc, buffer)] WARNING: p_actor == nullptr! Return ...");
        return;
    }

    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/actor_neutral(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    p_npc->ForceSetGoodwill(0, p_actor);
}

inline void actor_enemy(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_actor)
    {
        Msg("[Scripts/XR_EFFECTS/actor_enemy(p_actor, p_npc, buffer)] WARNING: p_actor == nullptr! Return ...");
        return;
    }

    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/actor_enemy(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    p_npc->ForceSetGoodwill(-1000, p_actor);
}

inline void set_squad_neutral_to_actor(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/set_squad_neutral_to_actor(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! "
            "Return ...");
        return;
    }

    Script_SE_SimulationSquad* const p_squad = Globals::get_story_squad(buffer[0]);

    if (!p_squad)
    {
        Msg("[Scripts/XR_EFFECTS/set_squad_neutral_to_actor(p_actor, p_npc, buffer)] WARNING: p_squad == nullptr! "
            "There is no squad! Return ...");
        return;
    }

    p_squad->set_squad_relation("neutral");
}

inline void set_squad_friend_to_actor(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/set_squad_friend_to_actor(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! "
            "Return ...");
        return;
    }

    Script_SE_SimulationSquad* const p_squad = Globals::get_story_squad(buffer[0]);

    if (!p_squad)
    {
        Msg("[Scripts/XR_EFFECTS/set_squad_friend_to_actor(p_actor, p_npc, buffer)] WARNING: p_squad == nullptr! "
            "There is no squad! Return ...");
        return;
    }

    p_squad->set_squad_relation("friend");
}

inline void set_squad_enemy_to_actor(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/set_squad_enemy_to_actor(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! "
            "Return ...");
        return;
    }

    Script_SE_SimulationSquad* const p_squad = Globals::get_story_squad(buffer[0]);

    if (!p_squad)
    {
        Msg("[Scripts/XR_EFFECTS/set_squad_enemy_to_actor(p_actor, p_npc, buffer)] WARNING: p_squad == nullptr! "
            "There is no squad! Return ...");
        return;
    }

    p_squad->set_squad_relation("enemy");
}

inline void set_squad_goodwill(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/set_squad_goodwill(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/set_squad_goodwill(p_actor, p_npc, buffer)] WARNING: not enough size for buffer "
            "argument list! Must be == 2! Return ...");
        return;
    }

    Globals::GameRelations::set_squad_goodwill(buffer[0], buffer[1]);
}

inline void set_squad_goodwill_to_npc(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/set_squad_goodwill_to_npc(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! "
            "Return ...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/set_squad_goodwill_to_npc(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! Return "
            "...");
        return;
    }

    Globals::GameRelations::set_squad_goodwill_to_npc(p_npc, buffer[0], buffer[1]);
}

inline void inc_faction_goodwill_to_actor(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/inc_faction_goodwill_to_actor(p_actor, p_npc, buffer)] WARNING: buffer.empty() == "
            "true! Return ...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scrpts/XR_EFFECTS/inc_faction_goodwill_to_actor(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! "
            "Return ...");
        return;
    }

    if (!p_actor)
    {
        Msg("[Scripts/XR_EFFECTS/inc_faction_goodwill_to_actor(p_actor, p_npc, buffer)] WARNING: p_actor == nullptr! "
            "Return ...");
        return;
    }

    Globals::GameRelations::change_factions_community_num(buffer[0], p_actor->ID(), atoi(buffer[1].c_str()));
}

inline void dec_faction_goodwill_to_actor(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/dec_faction_goodwill_to_actor(p_actor, p_npc, buffer)] WARNING: buffer.empty() == "
            "true! Return ...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scrpts/XR_EFFECTS/dec_faction_goodwill_to_actor(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! "
            "Return ...");
        return;
    }

    if (!p_actor)
    {
        Msg("[Scripts/XR_EFFECTS/dec_faction_goodwill_to_actor(p_actor, p_npc, buffer)] WARNING: p_actor == nullptr! "
            "Return ...");
        return;
    }

    Globals::GameRelations::change_factions_community_num(buffer[0], p_actor->ID(), -atoi(buffer[1].c_str()));
}

inline void kill_actor(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->Kill(DataBase::Storage::getInstance().getActor());
}

inline void give_treasure(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/give_treasure(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    for (const xr_string& it : buffer)
    {
        Script_TreasureManager::getInstance().give_treasure(it);
    }
}

inline void start_surge(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    Globals::start_surge();
}

inline void stop_surge(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    Globals::stop_surge();
}

inline void set_surge_mess_and_task(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/set_surge_mess_and_task(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! "
            "Return ...");
        return;
    }

    Script_SurgeManager::getInstance().set_surge_message(buffer[0]);

    if (buffer.size() >= 2)
        Script_SurgeManager::getInstance().set_surge_task(buffer[1]);
    else
        Msg("[Scripts/XR_EFFECTS/set_surge_mess_and_task(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! Can't "
            "set task!");
}

inline void make_actor_visible_to_squad(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/make_actor_visible_to_squad(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! "
            "Return ...");
        return;
    }

    Script_SE_SimulationSquad* const p_squad = Globals::get_story_squad(buffer[0]);

    if (!p_squad)
    {
        Msg("[Scripts/XR_EFFECTS/make_actor_visible_to_squad(p_actor, p_npc, buffer)] WARNING: can't obtain squad from "
            "id %s Return ...",
            buffer[0].c_str());
        return;
    }

    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
             p_squad->squad_members().begin();
         it != p_squad->squad_members().end(); ++it)
    {
        CScriptGameObject* const p_client_object = Globals::Game::level::get_object_by_id(it->first);
        if (p_client_object)
            p_client_object->make_object_visible_somewhen(DataBase::Storage::getInstance().getActor());
    }
}

inline void stop_sr_cutscene(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/stop_sr_cutscene(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(p_npc->ID());

    if (!storage.getActiveSchemeName().empty())
    {
        DataBase::Storage::getInstance().setStorageSignal(p_npc->ID(), "cam_effector_stop", true);
    }
}

inline void enable_anomaly(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/enable_anomaly(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    CScriptGameObject* const p_object = Globals::get_story_object(buffer[0]);
    if (!p_object)
    {
        Msg("[Scripts/XR_EFFECTS/enable_anomaly(p_actor, p_npc, buffer)] WARNING: p_object == nullptr! %s Return ...",
            buffer[0].c_str());
        return;
    }

    p_object->EnableAnomaly();
}

inline void disable_anomaly(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/disable_anomaly(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    CScriptGameObject* const p_object = Globals::get_story_object(buffer[0]);
    if (!p_object)
    {
        Msg("[Scripts/XR_EFFECTS/disable_anomaly(p_actor, p_npc, buffer)] WARNING: can't find object by %s Return ...",
            buffer[0].c_str());
        return;
    }

    p_object->DisableAnomaly();
}

// @ Сигнальные ракеты не использую

inline void add_cs_text(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/add_cs_text(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    CUIGameCustom* const p_hud = CurrentGameUI();
    StaticDrawableWrapper* p_static = p_hud->GetCustomStatic("text_on_screen_center");

    if (p_static)
        p_hud->RemoveCustomStatic("text_on_screen_center");

    p_hud->AddCustomStatic("text_on_screen_center", true);
    p_static = p_hud->GetCustomStatic("text_on_screen_center");
    p_static->wnd()->TextItemControl()->SetText(Globals::Game::translate_string(buffer[0].c_str()));
}

inline void del_cs_text(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    CUIGameCustom* const p_hud = CurrentGameUI();
    StaticDrawableWrapper* const p_static = p_hud->GetCustomStatic("text_on_screen_center");

    if (p_static)
        p_hud->RemoveCustomStatic("text_on_screen_center");
}

inline void spawn_item_to_npc(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/spawn_item_to_npc(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    Globals::Game::alife_create(
        buffer[0], p_npc->Position(), p_npc->level_vertex_id(), p_npc->game_vertex_id(), p_npc->ID());
}

inline void give_money_to_npc(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/give_money_to_npc(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    int value = atoi(buffer[0].c_str());

    if (!value)
        Msg("[Scripts/XR_EFFECTS/give_money_to_npc(p_actor, p_npc, buffer)] WARNING: money == 0! You give Zero!");

    p_npc->GiveMoney(value);
}

inline void seize_money_to_npc(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/seize_money_to_npc(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    int value = atoi(buffer[0].c_str());

    if (!value)
        Msg("[Scripts/XR_EFFECTS/seize_money_to_npc(p_actor, p_npc, buffer)] WARNING: money == 0! You give Zero!");

    p_npc->GiveMoney(-value);
}

inline void relocate_item(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/relocate_item(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    if (buffer.size() < 3)
    {
        Msg("[Scripts/XR_EFFECTS/relocate_item(p_actor, p_npc, buffer)] WARNING: buffer.size() < 3! Return ...");
        return;
    }

    CScriptGameObject* const p_from_object = Globals::get_story_object(buffer[1]);
    CScriptGameObject* const p_to_object = Globals::get_story_object(buffer[2]);

    if (p_to_object)
    {
        if (p_from_object && p_from_object->GetObjectByName(buffer[0].c_str()))
        {
            p_from_object->TransferItem(p_from_object->GetObjectByName(buffer[0].c_str()), p_to_object);
        }
        else
        {
            Globals::Game::alife_create(buffer[0], p_to_object->Position(), p_to_object->level_vertex_id(),
                p_to_object->game_vertex_id(), p_to_object->ID());
        }
    }
    else
    {
        Msg("[Scripts/XR_EFFECTS/relocate_item(p_actor, p_npc, buffer)] WARNING: p_to_object == nullptr! Return ...");
    }
}

inline void set_squads_enemies(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/set_squads_enemies(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    Script_SE_SimulationSquad* const p_squad1 = Globals::get_story_squad(buffer[0]);
    Script_SE_SimulationSquad* const p_squad2 = Globals::get_story_squad(buffer[1]);

    if (!p_squad1)
    {
        Msg("[Scripts/XR_EFFECTS/set_squads_enemies(p_actor, p_npc, buffer)] WARNING: p_squad1 == nullptr! Return ...");
        return;
    }

    if (!p_squad2)
    {
        Msg("[Scripts/XR_EFFECTS/set_squads_enemies(p_actor, p_npc, buffer)] WARNING: p_squad2 == nullptr! Return ...");
        return;
    }

    // Lord: если будет вылетать то сделать обработку на find Storage_Data
    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
             p_squad1->squad_members().begin();
         it != p_squad1->squad_members().end(); ++it)
    {
        CScriptGameObject* const p_npc1 = DataBase::Storage::getInstance().getStorage().at(it->first).getClientObject();
        if (p_npc1)
        {
            for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it2 =
                     p_squad2->squad_members().begin();
                 it != p_squad2->squad_members().end(); ++it)
            {
                CScriptGameObject* const p_npc2 =
                    DataBase::Storage::getInstance().getStorage().at(it->first).getClientObject();

                if (p_npc2)
                {
                    p_npc1->SetRelation(ALife::eRelationTypeEnemy, p_npc2);
                    p_npc2->SetRelation(ALife::eRelationTypeEnemy, p_npc1);
                }
            }
        }
    }
}

inline void set_bloodsucker_state(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/set_bloodsucker_state(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! "
            "Return ...");
        return;
    }

    xr_string state_name = buffer[0];
    CScriptGameObject* p_object = p_npc;

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/set_bloodsucker_state(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2!");
    }
    else
    {
        state_name = buffer[1];
        p_object = Globals::get_story_object(buffer[0]);
    }

    if (p_object)
    {
        if (state_name == "default")
            p_object->force_visibility_state(-1);
        else
            p_object->force_visibility_state(atoi(state_name.c_str()));
    }
}

inline void zat_b29_create_random_infop(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/zat_b29_create_random_infop(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! "
            "Return ...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/zat_b29_create_random_infop(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! "
            "Return ...");
        return;
    }

    int amount_needed = atoi(buffer[0].c_str());
    int current_infop = 0;
    int total_infop = 0;

    if (amount_needed == 0)
        amount_needed = 1;

    for (int i = 1; i < buffer.size(); ++i)
    {
        ++total_infop;

        if (Globals::has_alife_info(buffer[i].c_str()))
        {
            DataBase::Storage::getInstance().getActor()->DisableInfoPortion(buffer[i].c_str());
        }
    }

    if (amount_needed > total_infop)
        amount_needed = total_infop;

    // Lord: протестировать
    int j = 1;
    for (int i = 0; i < amount_needed; ++i)
    {
        current_infop = Globals::Script_RandomInt::getInstance().Generate<int>(0, total_infop - 1);
        for (const xr_string& it : buffer)
        {
            if (j > 1)
            {
                if (j == current_infop + 1 && (!Globals::has_alife_info(it.c_str())))
                {
                    DataBase::Storage::getInstance().getActor()->GiveInfoPortion(it.c_str());
                    break;
                }
            }
            ++j;
        }
    }
}

inline void pick_artefact_from_anomaly(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/pick_artefact_from_anomaly(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! "
            "Return ...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/pick_artefact_from_anomaly(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! "
            "Return ...");
        return;
    }

    std::uint16_t npc_id = Globals::get_story_object_id(buffer[0]);
    if (!npc_id)
    {
        Msg("[Scripts/XR_EFFECTS/pick_artefact_from_anomaly(p_actor, p_npc, buffer)] WARNING: npc_id == 0! Can't find "
            "that id by %s Return ...",
            buffer[0].c_str());
        return;
    }

    CSE_Abstract* const p_server_npc = ai().alife().objects().object(npc_id);

    if (p_server_npc)
    {
        if ((!Globals::IsStalker(p_server_npc->cast_alife_dynamic_object()) ||
                !p_server_npc->cast_creature_abstract()->g_Alive()))
        {
            Msg("[Scripts/XR_EFFECTS/pick_artefact_from_anomaly(p_actor, p_npc, buffer)] WARNING: Can't relocate item "
                "to dead or not stalker! Return ...");
            return;
        }
    }

    xr_string anomalzone_name = buffer[1];
    xr_string artefact_name = buffer.size() > 2 ? buffer[2] : "";

    CScriptGameObject* const p_zone = DataBase::Storage::getInstance().getAnomalyByName().at(anomalzone_name);

    if (!p_zone)
    {
        Msg("[Scripts/XR_EFFECTS/pick_artefact_from_anomaly(p_actor, p_npc, buffer)] WARNING: Can't find anomal zone "
            "(or it can be deleted) %s Return ...",
            anomalzone_name.c_str());
        return;
    }

    Script_Binder_Anomaly* const p_zone_object = dynamic_cast<Script_Binder_Anomaly*>(p_zone->binded_object());
    if (!p_zone_object)
    {
        Msg("[Scripts/XR_EFFECTS/pick_artefact_from_anomaly(p_actor, p_npc, buffer)] WARNING: Bad cast. It is not "
            "anomal zone at all, check your object and his binder! Return ...");
        return;
    }

    if (p_zone_object->getSpawnedCount() < 1)
    {
        Msg("[Scripts/XR_EFFECTS/pick_artefact_from_anomaly(p_actor, p_npc, buffer)] WARNING: spawned count < 1! "
            "Return ...");
        return;
    }

    std::uint16_t artefact_id = 0;
    CSE_Abstract* p_server_artefact = nullptr;

    for (const std::pair<std::uint16_t, xr_string>& it : p_zone_object->getArtefactWaysByID())
    {
        if (ai().alife().objects().object(it.first) && artefact_name == ai().alife().objects().object(it.first)->name())
        {
            artefact_id = it.first;
            p_server_artefact = ai().alife().objects().object(it.first);
            break;
        }

        if (artefact_name.empty())
        {
            artefact_id = it.first;
            p_server_artefact = ai().alife().objects().object(it.first);
            artefact_name = p_server_artefact->name();
            break;
        }
    }

    if (artefact_id == 0)
    {
        Msg("[Scripts/XR_EFFECTS/pick_artefact_from_anomaly(p_actor, p_npc, buffer)] WARNING: Can't find artefact in "
            "anomaly %s Return ...",
            anomalzone_name.c_str());
        return;
    }

    p_zone_object->on_artefact_take(artefact_id);
    Globals::Game::alife_release(p_server_artefact, true);
    give_item(DataBase::Storage::getInstance().getActor(), p_npc, {artefact_name, buffer[0]});
}

inline void give_item_b29(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/give_item_b29(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    xr_string anomalzone_name;
    xr_vector<xr_string> anomalzone_table = {
        "zat_b55_anomal_zone",
        "zat_b54_anomal_zone",
        "zat_b53_anomal_zone",
        "zat_b39_anomal_zone",
        "zaton_b56_anomal_zone",
    };

    for (int i = 16; i <= 23; ++i)
    {
        if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopBringTable().at(i).c_str()))
        {
            for (const xr_string& it : anomalzone_table)
            {
                if (Globals::has_alife_info(it.c_str()))
                {
                    anomalzone_name = it;
                    DataBase::Storage::getInstance().getActor()->DisableInfoPortion(it.c_str());
                    break;
                }
            }

            pick_artefact_from_anomaly(nullptr, nullptr,
                {buffer[0], anomalzone_name, Script_GlobalHelper::getInstance().getZatB29AfTable().at(i)});
            break;
        }
    }
}

inline void relocate_item_b29(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/relocate_item_b29(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/relocate_item_b29(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! Return ...");
        return;
    }

    xr_string item_name;

    for (std::uint16_t i = 16; i <= 23; ++i)
    {
        if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopBringTable().at(i).c_str()))
        {
            item_name = Script_GlobalHelper::getInstance().getZatB29AfTable().at(i);
            break;
        }
    }

    CScriptGameObject* const p_from_object = Globals::get_story_object(buffer[0]);
    CScriptGameObject* const p_to_object = Globals::get_story_object(buffer[1]);

    if (p_to_object)
    {
        if (p_from_object && p_from_object->GetObjectByName(item_name.c_str()))
        {
            p_from_object->TransferItem(p_from_object->GetObjectByName(item_name.c_str()), p_to_object);
        }
        else
        {
            Globals::Game::alife_create(item_name, p_to_object->Position(), p_to_object->level_vertex_id(),
                p_to_object->game_vertex_id(), p_to_object->ID());
        }
    }
    else
    {
        Msg("[Scripts/XR_EFFECTS/relocate_item_b29(p_actor, p_npc, buffer)] WARNING: can't find object by %s Return "
            "...",
            buffer[1].c_str());
    }
}

inline void reset_sound_npc(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (!p_npc)
    {
        Msg("[Scripts/XR_EFFECTS/reset_sound_npc(p_actor, p_npc, buffer)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    std::uint16_t object_id = p_npc->ID();

    if (object_id && XR_SOUND::getSoundDatabase().find(object_id) != XR_SOUND::getSoundDatabase().end())
    {
        XR_SOUND::getSoundDatabase().at(object_id)->reset(object_id);
    }
}

inline void jup_b202_inventory_box_relocate(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    CScriptGameObject* const p_inventory_box_out = Globals::get_story_object("jup_b202_actor_treasure");
    CScriptGameObject* const p_inventory_box_in = Globals::get_story_object("jup_b202_snag_treasure");

    xr_vector<CScriptGameObject*> items_to_relocate;

    auto iterate_function = [&](CScriptGameObject* const p_item) -> void { items_to_relocate.push_back(p_item); };

    p_inventory_box_out->IterateInventoryBox(iterate_function);

    for (CScriptGameObject*& it : items_to_relocate)
    {
        p_inventory_box_out->TransferItem(it, p_inventory_box_in);
    }
}

inline void clear_box(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/clear_box(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    CScriptGameObject* const p_inventory_box = Globals::get_story_object(buffer[0]);

    if (!p_inventory_box)
    {
        Msg("[Scripts/XR_EFFECTS/clear_box(p_actor, p_npc, buffer)] WARNING: p_inventory_box == nullptr! Can't find by "
            "%s Return ...",
            buffer[0].c_str());
        return;
    }

    xr_vector<CScriptGameObject*> items_of_inventory_box;

    auto iterate_function = [&](CScriptGameObject* const p_item) -> void { items_of_inventory_box.push_back(p_item); };

    p_inventory_box->IterateInventoryBox(iterate_function);

    for (CScriptGameObject*& it : items_of_inventory_box)
    {
        Globals::Game::alife_release(ai().alife().objects().object(it->ID()), true);
    }
}

inline void activate_weapon(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/activate_weapon(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    CScriptGameObject* const p_weapon = p_actor->GetObjectByName(buffer[0].c_str());

    if (!p_weapon)
    {
        Msg("[Scripts/XR_EFFECTS/activate_weapon(p_actor, p_npc, buffer)] WARNING: p_weapon == nullptr! can't find by "
            "%s Return ...",
            buffer[0].c_str());
        return;
    }

    p_actor->MakeItemActive(p_weapon);
}

inline void set_game_time(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/set_game_time(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    std::uint32_t real_hours = Globals::get_time_hours();
    std::uint32_t real_minutes = Globals::get_time_minutes();

    int hours = atoi(buffer[0].c_str());

    if (hours == 0)
    {
        Msg("[Scripts/XR_EFFECTS/set_game_time(p_actor, p_npc, buffer)] WARNING: hours == 0! Something will be wrong! "
            "Check your arguments!");
    }

    int minutes = buffer.size() > 1 ? (atoi(buffer[1].c_str())) : 0;

    if (minutes == 0)
    {
        Msg("[Scripts/XR_EFFECTS/set_game_time(p_actor, p_npc, buffer)] minutes == 0!");
    }

    int hours_to_change = hours - real_hours;

    if (hours_to_change <= 0)
        hours_to_change += 24;

    int minutes_to_change = minutes - real_minutes;

    if (minutes_to_change <= 0)
    {
        minutes_to_change += 60;
        hours_to_change -= 1;
    }
    else if (hours == real_hours)
    {
        hours_to_change -= 24;
    }

    Globals::change_game_time(0, hours_to_change, minutes_to_change);
    Script_WeatherManager::getInstance().forced_weather_change();
    Script_SurgeManager::getInstance().setTimeForwarded(true);

    Msg("[Scripts/XR_EFFECTS/set_game_time(p_actor, p_npc, buffer)] Time is changed to [%d][%d]", hours_to_change,
        minutes_to_change);
}

inline void forward_game_time(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/forward_game_time(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    std::uint32_t hours = static_cast<std::uint32_t>(atoi(buffer[0].c_str()));
    std::uint32_t minutes = buffer.size() < 2 ? 0 : static_cast<std::uint32_t>(atoi(buffer[1].c_str()));

    Globals::change_game_time(0, hours, minutes);
    Script_WeatherManager::getInstance().forced_weather_change();
    Script_SurgeManager::getInstance().setTimeForwarded(true);
    Msg("[Scripts/XR_EFFECTS/forward_game_time(p_actor, p_npc, buffer)] time is forwarded to %d %d", hours, minutes);
}

inline void stop_tutorial(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    Globals::Game::stop_tutorial();
}

inline void jup_b10_spawn_drunk_dead_items(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    xr_map<xr_string, std::uint32_t> items_all;

    items_all["wpn_ak74"] = 1;
    items_all["ammo_5.45x39_fmj"] = 5;
    items_all["ammo_5.45x39_ap"] = 3;
    items_all["wpn_fort"] = 1;
    items_all["ammo_9x18_fmj"] = 3;
    items_all["ammo_12x70_buck"] = 5;
    items_all["ammo_11.43x23_hydro"] = 2;
    items_all["grenade_rgd5"] = 3;
    items_all["grenade_f1"] = 2;
    items_all["medkit_army"] = 2;
    items_all["medkit"] = 4;
    items_all["bandage"] = 4;
    items_all["antirad"] = 2;
    items_all["vodka"] = 3;
    items_all["energy_drink"] = 2;
    items_all["conserva"] = 1;
    items_all["jup_b10_ufo_memory_2"] = 1;

    xr_map<std::uint32_t, xr_map<xr_string, std::uint32_t>> items;
    items[2]["wpn_sig550_luckygun"] = 1;

    items[1]["ammo_5.45x39_fmj"] = 5;
    items[1]["ammo_5.45x39_ap"] = 3;
    items[1]["wpn_fort"] = 1;
    items[1]["ammo_9x18_fmj"] = 3;
    items[1]["ammo_12x70_buck"] = 5;
    items[1]["ammo_11.43x23_hydro"] = 2;
    items[1]["grenade_rgd5"] = 3;
    items[1]["grenade_f1"] = 2;

    items[0]["medkit_army"] = 2;
    items[0]["medkit"] = 4;
    items[0]["bandage"] = 4;
    items[0]["antirad"] = 2;
    items[0]["vodka"] = 3;
    items[0]["energy_drink"] = 2;
    items[0]["conserva"] = 1;

    if (!buffer.empty())
    {
        std::uint32_t count = XR_LOGIC::pstor_retrieve_number(p_actor, "jup_b10_ufo_counter");

        if (count > 2)
            return;

        for (const std::pair<xr_string, std::uint32_t>& it : items[count])
        {
            std::uint16_t target_object_id = Globals::get_story_object_id(buffer[0]);

            if (target_object_id)
            {
                CSE_Abstract* const p_server_box = ai().alife().objects().object(target_object_id);

                if (!p_server_box)
                {
                    Msg("[Scripts/XR_EFFECTS/jup_b10_spawn_drunk_dead_items(p_actor, p_npc, buffer)] WARNING: can't "
                        "find server object by %d Return ...",
                        target_object_id);
                    return;
                }

                for (std::uint32_t i = 0; i < it.second; ++i)
                {
                    Globals::Game::alife_create(it.first, Fvector(), 0, 0, target_object_id);
                }
            }
            else
            {
                Msg("[Scripts/XR_EFFECTS/jup_b10_spawn_drunk_dead_items(p_actor, p_npc, buffer)] WARNING: can't find "
                    "id by %s",
                    buffer[0].c_str());
            }
        }
    }
    else
    {
        for (const std::pair<xr_string, std::uint32_t>& it : items_all)
        {
            for (std::uint32_t i = 0; i < it.second; ++i)
            {
                Globals::Game::alife_create(
                    it.first, p_npc->Position(), p_npc->level_vertex_id(), p_npc->game_vertex_id(), p_npc->ID());
            }
        }
    }
}

inline void anomaly_turn_off(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/anomaly_turn_off(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    CScriptGameObject* const p_anomaly = DataBase::Storage::getInstance().getAnomalyByName().at(buffer[0]);

    if (!p_anomaly)
    {
        Msg("[Scripts/XR_EFFECTS/anomaly_turn_off(p_actor, p_npc, buffer)] WARNING: can't find a client object by %s "
            "Return ...",
            buffer[0].c_str());
        return;
    }

    Script_Binder_Anomaly* const p_binder = dynamic_cast<Script_Binder_Anomaly*>(p_anomaly->binded_object());

    if (!p_binder)
    {
        Msg("[Scripts/XR_EFFECTS/anomaly_turn_off(p_actor, p_npc, buffer)] WARNING: p_binder == nullptr! Bad cast "
            "Return ...");
        return;
    }

    p_binder->turn_off();
}

inline void anomaly_turn_on(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/anomaly_turn_on(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    CScriptGameObject* const p_anomaly = DataBase::Storage::getInstance().getAnomalyByName().at(buffer[0]);

    if (!p_anomaly)
    {
        Msg("[Scripts/XR_EFFECTS/anomaly_turn_on(p_actor, p_npc, buffer)] WARNING: p_anomaly == nullptr! Can't find "
            "the anomaly by %s Return ...",
            buffer[0].c_str());
        return;
    }

    Script_Binder_Anomaly* const p_binder = dynamic_cast<Script_Binder_Anomaly*>(p_anomaly->binded_object());

    if (!p_binder)
    {
        Msg("[Scripts/XR_EFFECTS/anomaly_turn_on(p_actor, p_npc, buffer)] WARNING: p_binder == nullptr! Bad cast "
            "Return ...");
        return;
    }

    if (buffer.size() < 2)
    {
        p_binder->turn_on(false);
    }
    else
    {
        p_binder->turn_on(true);
    }
}

inline void zat_b202_spawn_random_loot(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/zat_b202_spawn_random_loot(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! "
            "Return ...");
        return;
    }

    xr_map<std::uint32_t, xr_map<std::uint32_t, xr_vector<xr_string>>> items;

    items[1][1].push_back("bandage");
    items[1][1].push_back("bandage");
    items[1][1].push_back("bandage");
    items[1][1].push_back("bandage");
    items[1][1].push_back("bandage");
    items[1][1].push_back("medkit");
    items[1][1].push_back("medkit");
    items[1][1].push_back("medkit");
    items[1][1].push_back("conserva");
    items[1][1].push_back("conserva");

    items[1][2].push_back("medkit");
    items[1][2].push_back("medkit");
    items[1][2].push_back("medkit");
    items[1][2].push_back("medkit");
    items[1][2].push_back("medkit");
    items[1][2].push_back("vodka");
    items[1][2].push_back("vodka");
    items[1][2].push_back("vodka");
    items[1][2].push_back("kolbasa");
    items[1][2].push_back("kolbasa");

    items[1][3].push_back("antirad");
    items[1][3].push_back("antirad");
    items[1][3].push_back("antirad");
    items[1][3].push_back("medkit");
    items[1][3].push_back("medkit");
    items[1][3].push_back("bandage");
    items[1][3].push_back("kolbasa");
    items[1][3].push_back("kolbasa");
    items[1][3].push_back("conserva");

    items[2][1].push_back("grenade_f1");
    items[2][1].push_back("grenade_f1");
    items[2][1].push_back("grenade_f1");

    items[2][2].push_back("grenade_rgd5");
    items[2][2].push_back("grenade_rgd5");
    items[2][2].push_back("grenade_rgd5");
    items[2][2].push_back("grenade_rgd5");
    items[2][2].push_back("grenade_rgd5");

    items[3][1].push_back("detector_elite");
    items[3][2].push_back("detector_advanced");

    items[4][1].push_back("helm_hardhat");
    items[4][2].push_back("helm_respirator");

    items[5][1].push_back("wpn_val");
    items[5][1].push_back("ammo_9x39_ap");
    items[5][1].push_back("ammo_9x39_ap");
    items[5][1].push_back("ammo_9x39_ap");

    items[5][2].push_back("wpn_spas12");
    items[5][2].push_back("ammo_12x70_buck");
    items[5][2].push_back("ammo_12x70_buck");
    items[5][2].push_back("ammo_12x70_buck");
    items[5][2].push_back("ammo_12x70_buck");

    items[5][3].push_back("wpn_desert_eagle");
    items[5][3].push_back("ammo_11.43x23_fmj");
    items[5][3].push_back("ammo_11.43x23_fmj");
    items[5][3].push_back("ammo_11.43x23_hydro");
    items[5][3].push_back("ammo_11.43x23_hydro");

    items[5][4].push_back("wpn_abakan");
    items[5][4].push_back("ammo_5.45x39_ap");
    items[5][4].push_back("ammo_5.45x39_ap");

    items[5][5].push_back("wpn_sig550");
    items[5][5].push_back("ammo_5.56x45_ap");
    items[5][5].push_back("ammo_5.56x45_ap");

    items[5][6].push_back("wpn_ak74");
    items[5][6].push_back("ammo_5.45x39_fmj");
    items[5][6].push_back("ammo_5.45x39_fmj");

    items[5][7].push_back("wpn_l85");
    items[5][7].push_back("ammo_5.56x45_ss190");
    items[5][7].push_back("ammo_5.56x45_ss190");

    items[6][1].push_back("specops_outfit");

    items[6][2].push_back("stalker_outfit");

    xr_map<std::uint32_t, std::uint32_t> weight_table;
    weight_table[1] = 2;
    weight_table[2] = 2;
    weight_table[3] = 2;
    weight_table[4] = 2;
    weight_table[5] = 4;
    weight_table[6] = 4;

    xr_vector<std::uint32_t> spawned_item;
    std::uint32_t max_weight = 12;

    // Lord: протестировать!
    do
    {
        std::uint32_t n = 0;
        bool is_prap = true;
        do
        {
            n = Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(1, weight_table.size());
            for (std::uint32_t it : spawned_item)
            {
                if (it == n)
                {
                    is_prap = false;
                    break;
                }
            }
        } while (is_prap && ((max_weight - weight_table[n]) >= 0));

        max_weight = max_weight - weight_table[n];
        spawned_item.push_back(n);

        std::uint32_t item = Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(1, items[n].size());

        for (const xr_string& it : items[n][item])
        {
            spawn_object_in(p_actor, p_npc, {it, "jup_b202_snag_treasure"});
        }

    } while (max_weight <= 0);
}

inline void zat_a1_tutorial_end_give(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("zat_a1_tutorial_end");
}

inline void oasis_heal(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    float health_koeff = 0.005f;
    float power_koeff = 0.01f;
    float bleeding_koeff = 0.05f;
    float radiation_koeff = -0.05f;

    CScriptGameObject* const p_client_actor = DataBase::Storage::getInstance().getActor();

    if (p_client_actor->GetHealth() < 1.0f)
    {
        p_client_actor->SetHealth(health_koeff);
    }

    if (p_client_actor->GetPower() < 1.0f)
    {
        p_client_actor->SetPower(power_koeff);
    }

    if (p_client_actor->GetRadiation() > 0.0f)
    {
        p_client_actor->SetRadiation(radiation_koeff);
    }

    if (p_client_actor->GetBleeding() > 0.0f)
    {
        p_client_actor->SetBleeding(bleeding_koeff);
    }

    p_client_actor->ChangeSatiety(0.01f);
}

inline void jup_b221_play_main(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/jup_b221_play_main(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    xr_string main_theme_name;
    xr_string reply_theme_name;
    xr_string info_need_reply_name;
    xr_map<std::uint32_t, xr_string> info_table;
    xr_vector<std::uint32_t> theme_indexes;
    std::uint32_t index_theme = 0;

    if (buffer[0] == "duty")
    {
        info_table[1] = "jup_b25_freedom_flint_gone";
        info_table[2] = "jup_b25_flint_blame_done_to_duty";
        info_table[3] = "jup_b4_monolith_squad_in_duty";
        info_table[4] = "jup_a6_duty_leader_bunker_guards_work";
        info_table[5] = "jup_a6_duty_leader_employ_work";
        info_table[6] = "jup_b207_duty_wins";

        main_theme_name = "jup_b221_duty_main_";
        reply_theme_name = "jup_b221_duty_reply_";
        info_need_reply_name = "jup_b221_duty_reply";
    }
    else if (buffer[0] == "freedom")
    {
        info_table[1] = "jup_b207_freedom_know_about_depot";
        info_table[2] = "jup_b46_duty_founder_pda_to_freedom";
        info_table[3] = "jup_b4_monolith_squad_in_freedom";
        info_table[4] = "jup_a6_freedom_leader_bunker_guards_work";
        info_table[5] = "jup_a6_freedom_leader_employ_work";
        info_table[6] = "jup_b207_freedom_wins";

        main_theme_name = "jup_b221_freedom_main_";
        reply_theme_name = "jup_b221_freedom_reply_";
        info_need_reply_name = "jup_b221_freedom_reply";
    }
    else
    {
        Msg("[Scripts/XR_EFFECTS/jup_b221_play_main(p_actor, p_npc, buffer)] WARNING: wrong argument, check it, it "
            "must be community name! Return ...");
        return;
    }

    for (const std::pair<std::uint32_t, xr_string>& it : info_table)
    {
        if (Globals::has_alife_info(it.second.c_str()) &&
            (!Globals::has_alife_info(xr_string(main_theme_name).append(std::to_string(it.first).c_str()).append("_played").c_str()))
        {
            theme_indexes.push_back(it.first);
        }
    }

    if (!theme_indexes.empty())
    {
        if (Globals::has_alife_info(info_need_reply_name.c_str()))
        {
            DataBase::Storage::getInstance().getActor()->DisableInfoPortion(info_need_reply_name.c_str());
        }

        index_theme = theme_indexes[Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(
            0, theme_indexes.size() - 1)];
        XR_LOGIC::pstor_store(p_actor, "jup_b221_played_main_theme", std::to_string(index_theme).c_str());
        DataBase::Storage::getInstance().getActor()->GiveInfoPortion(
            xr_string(main_theme_name).append(std::to_string(index_theme).c_str()).append("_played").c_str());
        if (index_theme)
        {
            play_sound(p_actor, p_npc, {xr_string(main_theme_name).append(std::to_string(index_theme).c_str())});
        }
        else
        {
            Msg("[Scripts/XR_EFFECTS/jup_b221_play_main(p_actor, p_npc, buffer)] WARNING: can't pick index_theme check "
                "your code here! Return ...");
            return;
        }
    }
    else
    {
        DataBase::Storage::getInstance().getActor()->GiveInfoPortion(info_need_reply_name.c_str());
        index_theme = static_cast<std::uint32_t>(
            atoi(XR_LOGIC::pstor_retrieve_string(p_actor, "jup_b221_played_main_theme").c_str()));
        if (index_theme)
        {
            play_sound(p_actor, p_npc, {xr_string(reply_theme_name).append(std::to_string(index_theme).c_str())});
        }
        else
        {
            Msg("[Scripts/XR_EFFECTS/jup_b221_play_main(p_actor, p_npc, buffer)] WARNING: can't pick index_theme check "
                "code Return ...");
        }

        XR_LOGIC::pstor_store(p_actor, "jup_b221_played_main_theme", "0");
    }
}

inline void pas_b400_play_particle(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->start_particles("zones\\zone_acidic_idle", "bip01_head");
}

inline void pas_b400_stop_particle(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->stop_particles("zones\\zone_acidic_idle", "bip01_head");
}

inline void damage_actor_items_on_start(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    CScriptGameObject* const p_client_actor = DataBase::Storage::getInstance().getActor();

    CScriptGameObject* p_object = p_client_actor->GetObjectByName("helm_respirator");

    if (p_object)
        p_object->SetCondition(0.8f);

    p_object = p_client_actor->GetObjectByName("stalker_outfit");
    if (p_object)
        p_object->SetCondition(0.76f);

    p_object = p_client_actor->GetObjectByName("wpn_pm_actor");
    if (p_object)
        p_object->SetCondition(0.9f);

    p_object = p_client_actor->GetObjectByIndex("wpn_ak74u");
    if (p_object)
        p_object->SetCondition(0.7f);
}

inline void damage_pri_a17_gauss(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    CScriptGameObject* const p_gauss = Globals::get_story_object("pri_a17_gauss_rifle");

    if (p_gauss)
        p_gauss->SetCondition(0.0f);
}

inline void jup_b217_hard_animation_reset(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    Script_StateManager* const p_state_manager =
        DataBase::Storage::getInstance().getStorage().at(p_npc->ID()).getStateManager();

    if (!p_state_manager)
    {
        Msg("[Scripts/XR_EFFECTS/jup_b217_hard_animation_reset(p_actor, p_npc, buffer)] WARNING: p_state_manager == "
            "nullptr! Return ...");
        return;
    }

    p_state_manager->set_state("jup_b217_nitro_straight", StateManagerCallbackData(), 0,
        std::pair<Fvector, CScriptGameObject* const>(Fvector(), nullptr), StateManagerExtraData());

    if (p_state_manager)
    {
        p_state_manager->getAnimation()->set_state();
        p_state_manager->getAnimation()->set_state("jup_b217_nitro_straight", false);
        p_state_manager->getAnimation()->set_control();
    }
}

inline void mech_discount(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/mech_discount(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return ...");
        return;
    }

    // Lord: доделать когда будет invetory_upgrades
}

inline void polter_actor_ignore(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/polter_actor_ignore(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    if (buffer[0] == "true")
        p_npc->poltergeist_set_actor_ignore(true);
    else
        p_npc->poltergeist_set_actor_ignore(false);
}

inline void burer_force_gravi_attack(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    p_npc->burer_set_force_gravi_attack(true);
}

inline void burer_force_anti_aim(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    p_npc->set_force_anti_aim(true);
}

inline void show_freeplay_dialog(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    if (buffer.empty())
    {
        Msg("[Scripts/XR_EFFECTS/show_freeplay_dialog(p_actor, p_npc, buffer)] WARNING: buffer.empty() == true! Return "
            "...");
        return;
    }

    if (buffer.size() < 2)
    {
        Msg("[Scripts/XR_EFFECTS/show_freeplay_dialog(p_actor, p_npc, buffer)] WARNING: buffer.size() < 2! Return ...");
        return;
    }

    // Lord: доделать когда будет ui_freeplay_dialog
}

inline void get_best_detector(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    xr_vector<xr_string> detectors = {"detector_simple", "detector_advanced", "detector_elite", "detector_scientific"};

    for (const xr_string& detector_name : detectors)
    {
        CScriptGameObject* const p_detector = p_npc->GetObjectByName(detector_name.c_str());
        if (p_detector)
        {
            p_detector->enable_attachable_item(true);
            return;
        }
    }
}

inline void hide_best_detector(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    xr_vector<xr_string> detectors = {"detector_simple", "detector_advanced", "detector_elite", "detector_scientific"};

    for (const xr_string& detector_name : detectors)
    {
        CScriptGameObject* const p_detector = p_npc->GetObjectByName(detector_name.c_str());
        if (p_detector)
        {
            p_detector->enable_attachable_item(false);
            return;
        }
    }
}

inline void pri_a18_radio_start(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("pri_a18_radio_start");
}

inline void pri_a17_ice_climb_end(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("pri_a17_ice_climb_end");
}

inline void jup_b219_opening(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b219_opening");
}

inline void jup_b219_entering_underpass(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b219_entering_underpass");
}

inline void pri_a17_pray_start(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("pri_a17_pray_start");
}

inline void zat_b38_open_info(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("zat_b38_open_info");
}

inline void zat_b38_switch_info(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("zat_b38_switch_info");
}

inline void zat_b38_cop_dead(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("zat_b38_cop_dead");
}

inline void jup_b15_zulus_drink_anim_info(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b15_zulus_drink_anim_info");
}

inline void pri_a17_preacher_death(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("pri_a17_preacher_death");
}

inline void zat_b3_tech_surprise_anim_end(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("zat_b3_tech_surprise_anim_end");
}

inline void zat_b3_tech_waked_up(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("zat_b3_tech_waked_up");
}

inline void zat_b3_tech_drinked_out(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("zat_b3_tech_drinked_out");
}

inline void pri_a28_kirillov_hq_online(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("pri_a28_kirillov_hq_online");
}

inline void pri_a20_radio_start(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("pri_a20_radio_start");
}

inline void pri_a22_kovalski_speak(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("pri_a22_kovalski_speak");
}

inline void zat_b38_underground_door_open(
    CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("zat_b38_underground_door_open");
}

inline void zat_b38_jump_tonnel_info(CScriptGameObject* const p_actor, CScriptGameObject* const p_npc, const xr_vector<xr_string>& buffer)
{
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("zat_b38_jump_tonnel_info");
}

} // namespace XR_EFFECTS
} // namespace Scripts
} // namespace Cordis
