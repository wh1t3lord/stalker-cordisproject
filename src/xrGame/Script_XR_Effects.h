#include "Script_GlobalDefinitions.h"
#pragma once

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
        direction = -look_point.point(std::uint32_t(0)).sub(patrol.point(std::uint32_t(0))).getH();
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

} // namespace XR_EFFECTS
} // namespace Scripts
} // namespace Cordis
