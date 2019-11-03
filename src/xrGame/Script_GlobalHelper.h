#pragma once

#include "object_factory.h"
#include "Script_XR_Patrol.h"

#define _REGISTER_FULL_FUNCTION_XR_CONDITION(function_name, function)                                                 \
    this->m_registered_functions_xr_conditions[#function_name "_client"] = function##_client;               \
    this->m_registered_functions_xr_conditions[#function_name "_client_server"] = function##_client_server; \
    this->m_registered_functions_xr_conditions[#function_name "_server"] = function##_server;

namespace Cordis
{
namespace Scripts
{
struct Script_GlobalHelper
{
private:
    Script_GlobalHelper(void)
    {
        
        // @ Здесь инициализация всех "локальных" карт которые находятся в каждом скрипте, проверить что оно реально
        // раньше всех инициализирует
#pragma region Cordis XR_PATROL Initializing
        std::pair<Fvector, float> xr_patrol_formations_data;
        xr_patrol_formations_data.first = Fvector().set(0.3f, 0.0f, -1.0f);
        xr_patrol_formations_data.second = 1.2f;
        /*        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_BACK] = xr_vector<std::pair<Fvector,
         * float>>();*/
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_BACK].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-0.3f, 0.0f, -1.0f);
        xr_patrol_formations_data.second = 2.4f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_BACK].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(0.3f, 0.0f, -1.0f);
        xr_patrol_formations_data.second = 3.6f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_BACK].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-0.3f, 0.0f, -1.0f);
        xr_patrol_formations_data.second = 4.8f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_BACK].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(0.3f, 0.0f, -1.0f);
        xr_patrol_formations_data.second = 6.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_BACK].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-0.3f, 0.0f, -1.0f);
        xr_patrol_formations_data.second = 7.2f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_BACK].push_back(xr_patrol_formations_data);

        xr_patrol_formations_data.first = Fvector().set(-1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 2.0f;
        /*        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_LINE] = xr_vector<std::pair<Fvector,
         * float>>();*/
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_LINE].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 4.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_LINE].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 6.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_LINE].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 2.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_LINE].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 4.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_LINE].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 6.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_LINE].push_back(xr_patrol_formations_data);

        xr_patrol_formations_data.first = Fvector().set(0.44721359f, 0.0f, -0.89442718f);
        xr_patrol_formations_data.second = 2.2360680f;
        /*        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_AROUND] = xr_vector<std::pair<Fvector,
         * float>>();*/

        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_AROUND].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-0.44721359f, 0.0f, -0.89442718f);
        xr_patrol_formations_data.second = 2.2360680f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_AROUND].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 2.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_AROUND].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 2.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_AROUND].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(0.44721359f, 0.0f, -0.89442718f);
        xr_patrol_formations_data.second = 2.2360680f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_AROUND].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-0.44721359f, 0.0f, -0.89442718f);
        xr_patrol_formations_data.second = 2.2360680f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_AROUND].push_back(xr_patrol_formations_data);
#pragma endregion

#pragma region Cordis SmartCovers Initialzing

#pragma region Cordis Combat Prone Smart Cover
        SmartCoverData combat_prone_smartcover;
        SmartCoverLoopholeData combat_prone_loophole;

#pragma region Cordis Loophole
        combat_prone_loophole.m_id = "prone";
        combat_prone_loophole.m_is_usable = true;
        combat_prone_loophole.m_is_exitable = true;
        combat_prone_loophole.m_is_enterable = true;
        combat_prone_loophole.m_fieldofview = 60.0f;
        combat_prone_loophole.m_fieldofview_danger = 90.0f;
        combat_prone_loophole.m_range = 70.0f;
        combat_prone_loophole.m_fieldofview_direction = Fvector().set(-1.0f, 0.0f, 0.0f);
        combat_prone_loophole.m_fieldofview_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_prone_loophole.m_danger_fieldofview_direction = Fvector().set(-1.0f, 0.0f, 0.0f);
        combat_prone_loophole.m_enter_direction = Fvector().set(-1.0f, 0.0f, 0.0f);

        SmartCoverLoopholeData::SmartCoverActionsData combat_prone_action1;
        combat_prone_action1.m_id = "idle";
        combat_prone_action1.register_animation("idle", "loophole_9_idle_0");

        combat_prone_loophole.register_action(combat_prone_action1);

        SmartCoverLoopholeData::SmartCoverActionsData combat_prone_action2;
        combat_prone_action2.m_id = "lookout";
        combat_prone_action2.register_animation("idle", "loophole_9_look_idle_0");

        combat_prone_loophole.register_action(combat_prone_action2);

        SmartCoverLoopholeData::SmartCoverActionsData combat_prone_action3;
        combat_prone_action3.m_id = "fire";
        combat_prone_action3.register_animation("idle", "loophole_9_attack_idle_0");
        combat_prone_action3.register_animation("shoot", "loophole_9_attack_shoot_0");
        combat_prone_action3.register_animation("shoot", "loophole_9_attack_shoot_1");

        combat_prone_loophole.register_action(combat_prone_action3);

        SmartCoverLoopholeData::SmartCoverActionsData combat_prone_action4;
        combat_prone_action4.m_id = "fire_no_lookout";
        combat_prone_action4.register_animation("idle", "loophole_9_attack_idle_0");
        combat_prone_action4.register_animation("shoot", "loophole_9_attack_shoot_0");
        combat_prone_action4.register_animation("shoot", "loophole_9_attack_shoot_1");

        combat_prone_loophole.register_action(combat_prone_action4);

        SmartCoverLoopholeData::SmartCoverActionsData combat_prone_action5;
        combat_prone_action5.m_id = "reload";
        combat_prone_action5.register_animation("idle", "loophole_9_reload_0");

        combat_prone_loophole.register_action(combat_prone_action5);

        SmartCoverLoopholeData::SmartCoverTransitionsData combat_prone_transition1;
        combat_prone_transition1.m_action_from = "idle";
        combat_prone_transition1.m_action_to = "lookout";
        combat_prone_transition1.m_weight = 1.2f;
        combat_prone_transition1.m_animations.push_back("loophole_9_look_in_0");

        combat_prone_loophole.register_transition(combat_prone_transition1);

        SmartCoverLoopholeData::SmartCoverTransitionsData combat_prone_transition2;
        combat_prone_transition2.m_action_from = "lookout";
        combat_prone_transition2.m_action_to = "idle";
        combat_prone_transition2.m_animations.push_back("loophole_9_look_out_0");
        combat_prone_transition2.m_weight = 1.2f;

        combat_prone_loophole.register_transition(combat_prone_transition2);

        SmartCoverLoopholeData::SmartCoverTransitionsData combat_prone_transition3;
        combat_prone_transition3.m_action_from = "idle";
        combat_prone_transition3.m_action_to = "fire";
        combat_prone_transition3.m_weight = 1.2f;
        combat_prone_transition3.m_animations.push_back("loophole_9_attack_in_0");

        combat_prone_loophole.register_transition(combat_prone_transition3);

        SmartCoverLoopholeData::SmartCoverTransitionsData combat_prone_transition4;
        combat_prone_transition4.m_action_from = "fire";
        combat_prone_transition4.m_action_to = "idle";
        combat_prone_transition4.m_weight = 1.2f;
        combat_prone_transition4.m_animations.push_back("loophole_9_attack_out_0");

        combat_prone_loophole.register_transition(combat_prone_transition4);

        SmartCoverLoopholeData::SmartCoverTransitionsData combat_prone_transition5;
        combat_prone_transition5.m_action_from = "idle";
        combat_prone_transition5.m_action_to = "fire_no_lookout";
        combat_prone_transition5.m_weight = 1.2f;
        combat_prone_transition5.m_animations.push_back("loophole_9_attack_in_0");

        combat_prone_loophole.register_transition(combat_prone_transition5);

        SmartCoverLoopholeData::SmartCoverTransitionsData combat_prone_transition6;
        combat_prone_transition6.m_action_from = "fire_no_lookout";
        combat_prone_transition6.m_action_to = "idle";
        combat_prone_transition6.m_weight = 1.2f;
        combat_prone_transition6.m_animations.push_back("loophole_9_attack_out_0");

        combat_prone_loophole.register_transition(combat_prone_transition6);
#pragma endregion Cordis Loophole

#pragma region Cordis Transition
        SmartCoverData::SmartCoverTransitionsData combat_prone_smartcover_transition1;
        combat_prone_smartcover_transition1.m_vertex0 = "";
        combat_prone_smartcover_transition1.m_vertex1 = "prone";
        combat_prone_smartcover_transition1.m_weight = 1.0f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_prone_smartcover_transition1_action1;
        combat_prone_smartcover_transition1_action1.m_action.m_animation = "loophole_9_in_front_0";
        combat_prone_smartcover_transition1_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_prone_smartcover_transition1_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_prone_smartcover_transition1_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_prone_smartcover_transition1_action1.m_precondition_functor = "true";
        combat_prone_smartcover_transition1_action1.m_preconditions_params = "";

        combat_prone_smartcover_transition1.m_actions.push_back(combat_prone_smartcover_transition1_action1);

        SmartCoverData::SmartCoverTransitionsData combat_prone_smartcover_transition2;
        combat_prone_smartcover_transition2.m_vertex0 = "";
        combat_prone_smartcover_transition2.m_vertex1 = "prone";
        combat_prone_smartcover_transition2.m_weight = 1.1f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_prone_smartcover_transition2_action1;
        combat_prone_smartcover_transition2_action1.m_action.m_animation = "loophole_9_jump_1";
        combat_prone_smartcover_transition2_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_prone_smartcover_transition2_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_prone_smartcover_transition2_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_prone_smartcover_transition2_action1.m_precondition_functor = "true";
        combat_prone_smartcover_transition2_action1.m_preconditions_params = "";

        combat_prone_smartcover_transition2.m_actions.push_back(combat_prone_smartcover_transition2_action1);

#pragma endregion Cordis Transition

        combat_prone_smartcover.setNeedWeapon(true);
        combat_prone_smartcover.register_loophole(combat_prone_loophole);
        combat_prone_smartcover.register_transition(combat_prone_smartcover_transition1);
        combat_prone_smartcover.register_transition(combat_prone_smartcover_transition2);
        this->m_registered_smartcovers["combat_prone"] = combat_prone_smartcover;
#pragma endregion Cordis Combat Prone Smart Cover

#pragma region Cordis Combat Front Smart Cover
        SmartCoverData combat_front_smartcover;

#pragma region Cordis Loophole
        SmartCoverLoopholeData combat_front_loophole_crouch_front_left;
        combat_front_loophole_crouch_front_left.m_id = "crouch_front_left";
        combat_front_loophole_crouch_front_left.m_is_usable = true;
        combat_front_loophole_crouch_front_left.m_is_exitable = true;
        combat_front_loophole_crouch_front_left.m_is_enterable = true;
        combat_front_loophole_crouch_front_left.m_fieldofview = 70.0f;
        combat_front_loophole_crouch_front_left.m_fieldofview_danger = 90.0f;
        combat_front_loophole_crouch_front_left.m_range = 70.0f;
        combat_front_loophole_crouch_front_left.m_fieldofview_direction = Fvector().set(-1.0f, 0.0f, -0.7f);
        combat_front_loophole_crouch_front_left.m_fieldofview_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_loophole_crouch_front_left.m_danger_fieldofview_direction = Fvector().set(-1.0f, 0.0f, -1.0f);
        combat_front_loophole_crouch_front_left.m_enter_direction = Fvector().set(0.0f, 0.0f, 0.0f);

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_left_action;
            combat_front_loophole_crouch_front_left_action.m_id = "idle";
            combat_front_loophole_crouch_front_left_action.register_animation(
                "idle", "loophole_crouch_front_left_idle_0");

            combat_front_loophole_crouch_front_left.register_action(combat_front_loophole_crouch_front_left_action);
        } // 1

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_left_action;
            combat_front_loophole_crouch_front_left_action.m_id = "lookout";
            combat_front_loophole_crouch_front_left_action.register_animation(
                "idle", "loophole_crouch_front_left_look_idle_0");

            combat_front_loophole_crouch_front_left.register_action(combat_front_loophole_crouch_front_left_action);
        } // 2

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_left_action;
            combat_front_loophole_crouch_front_left_action.m_id = "fire";
            combat_front_loophole_crouch_front_left_action.register_animation(
                "idle", "loophole_crouch_front_left_attack_idle_0");
            combat_front_loophole_crouch_front_left_action.register_animation(
                "shoot", "loophole_crouch_front_left_attack_shoot_0");
            combat_front_loophole_crouch_front_left_action.register_animation(
                "shoot", "loophole_crouch_front_left_attack_shoot_1");

            combat_front_loophole_crouch_front_left.register_action(combat_front_loophole_crouch_front_left_action);
        } // 3

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_left_action;
            combat_front_loophole_crouch_front_left_action.m_id = "fire_no_lookout";
            combat_front_loophole_crouch_front_left_action.register_animation(
                "idle", "loophole_crouch_front_left_attack_idle_0");
            combat_front_loophole_crouch_front_left_action.register_animation(
                "shoot", "loophole_crouch_front_left_attack_idle_0");

            combat_front_loophole_crouch_front_left.register_action(combat_front_loophole_crouch_front_left_action);
        } // 4

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_left_action;
            combat_front_loophole_crouch_front_left_action.m_id = "reload";
            combat_front_loophole_crouch_front_left_action.register_animation(
                "idle", "loophole_crouch_front_left_reload_0");

            combat_front_loophole_crouch_front_left.register_action(combat_front_loophole_crouch_front_left_action);
        } // 5

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_left_transition;
            combat_front_loophole_crouch_front_left_transition.m_action_from = "idle";
            combat_front_loophole_crouch_front_left_transition.m_action_to = "lookout";
            combat_front_loophole_crouch_front_left_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_left_transition.m_animations.push_back(
                "loophole_crouch_front_left_look_in_0");

            combat_front_loophole_crouch_front_left.register_transition(
                combat_front_loophole_crouch_front_left_transition);
        } // 11

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_left_transition;
            combat_front_loophole_crouch_front_left_transition.m_action_from = "lookout";
            combat_front_loophole_crouch_front_left_transition.m_action_to = "idle";
            combat_front_loophole_crouch_front_left_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_left_transition.m_animations.push_back(
                "loophole_crouch_front_left_look_out_0");

            combat_front_loophole_crouch_front_left.register_transition(
                combat_front_loophole_crouch_front_left_transition);
        } // 12

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_left_transition;
            combat_front_loophole_crouch_front_left_transition.m_action_from = "idle";
            combat_front_loophole_crouch_front_left_transition.m_action_to = "fire";
            combat_front_loophole_crouch_front_left_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_left_transition.m_animations.push_back(
                "loophole_crouch_front_left_attack_in_0");

            combat_front_loophole_crouch_front_left.register_transition(
                combat_front_loophole_crouch_front_left_transition);
        } // 13

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_left_transition;
            combat_front_loophole_crouch_front_left_transition.m_action_from = "fire";
            combat_front_loophole_crouch_front_left_transition.m_action_to = "idle";
            combat_front_loophole_crouch_front_left_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_left_transition.m_animations.push_back(
                "loophole_crouch_front_left_attack_out_0");

            combat_front_loophole_crouch_front_left.register_transition(
                combat_front_loophole_crouch_front_left_transition);
        } // 14

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_left_transition;
            combat_front_loophole_crouch_front_left_transition.m_action_from = "idle";
            combat_front_loophole_crouch_front_left_transition.m_action_to = "fire_no_lookout";
            combat_front_loophole_crouch_front_left_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_left_transition.m_animations.push_back(
                "loophole_crouch_front_left_attack_in_0");

            combat_front_loophole_crouch_front_left.register_transition(
                combat_front_loophole_crouch_front_left_transition);
        } // 15

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_left_transition;
            combat_front_loophole_crouch_front_left_transition.m_action_from = "fire_no_lookout";
            combat_front_loophole_crouch_front_left_transition.m_action_to = "idle";
            combat_front_loophole_crouch_front_left_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_left_transition.m_animations.push_back(
                "loophole_crouch_front_left_attack_out_0");

            combat_front_loophole_crouch_front_left.register_transition(
                combat_front_loophole_crouch_front_left_transition);
        } // 16
#pragma endregion Cordis Loophole

#pragma region Cordis Loophole
        SmartCoverLoopholeData combat_front_loophole_crouch_front_right;
        combat_front_loophole_crouch_front_right.m_id = "crouch_front_right";
        combat_front_loophole_crouch_front_right.m_is_usable = true;
        combat_front_loophole_crouch_front_right.m_is_exitable = true;
        combat_front_loophole_crouch_front_right.m_is_enterable = true;
        combat_front_loophole_crouch_front_right.m_fieldofview = 70.0f;
        combat_front_loophole_crouch_front_right.m_fieldofview_danger = 90.0f;
        combat_front_loophole_crouch_front_right.m_range = 70.0f;
        combat_front_loophole_crouch_front_right.m_fieldofview_direction = Fvector().set(-1.0f, 0.0f, 0.7f);
        combat_front_loophole_crouch_front_right.m_fieldofview_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_loophole_crouch_front_right.m_danger_fieldofview_direction = Fvector().set(-1.0f, 0.0f, 1.0f);
        combat_front_loophole_crouch_front_right.m_enter_direction = Fvector().set(-1.0f, 0.0f, 0.0f);

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_right_action;
            combat_front_loophole_crouch_front_right_action.m_id = "idle";
            combat_front_loophole_crouch_front_right_action.register_animation(
                "idle", "loophole_crouch_front_right_idle_0");

            combat_front_loophole_crouch_front_right.register_action(combat_front_loophole_crouch_front_right_action);
        } // 1

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_right_action;
            combat_front_loophole_crouch_front_right_action.m_id = "lookout";
            combat_front_loophole_crouch_front_right_action.register_animation(
                "idle", "loophole_crouch_front_right_look_idle_0");

            combat_front_loophole_crouch_front_right.register_action(combat_front_loophole_crouch_front_right_action);
        } // 2

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_right_action;
            combat_front_loophole_crouch_front_right_action.m_id = "fire";
            combat_front_loophole_crouch_front_right_action.register_animation(
                "idle", "loophole_crouch_front_right_attack_idle_0");
            combat_front_loophole_crouch_front_right_action.register_animation(
                "shoot", "loophole_crouch_front_right_attack_shoot_0");
            combat_front_loophole_crouch_front_right_action.register_animation(
                "shoot", "loophole_crouch_front_right_attack_shoot_1");

            combat_front_loophole_crouch_front_right.register_action(combat_front_loophole_crouch_front_right_action);
        } // 3

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_right_action;
            combat_front_loophole_crouch_front_right_action.m_id = "fire_no_lookout";
            combat_front_loophole_crouch_front_right_action.register_animation(
                "idle", "loophole_crouch_front_right_attack_idle_0");
            combat_front_loophole_crouch_front_right_action.register_animation(
                "shoot", "loophole_crouch_front_right_attack_shoot_0");
            combat_front_loophole_crouch_front_right_action.register_animation(
                "shoot", "loophole_crouch_front_right_attack_shoot_1");

            combat_front_loophole_crouch_front_right.register_action(combat_front_loophole_crouch_front_right_action);
        } // 4

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_right_action;
            combat_front_loophole_crouch_front_right_action.m_id = "reload";
            combat_front_loophole_crouch_front_right_action.register_animation(
                "idle", "loophole_crouch_front_right_reload_0");

            combat_front_loophole_crouch_front_right.register_action(combat_front_loophole_crouch_front_right_action);
        } // 5

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_right_transition;
            combat_front_loophole_crouch_front_right_transition.m_action_from = "idle";
            combat_front_loophole_crouch_front_right_transition.m_action_to = "lookout";
            combat_front_loophole_crouch_front_right_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_right_transition.m_animations.push_back(
                "loophole_crouch_front_right_look_in_0");

            combat_front_loophole_crouch_front_right.register_transition(
                combat_front_loophole_crouch_front_right_transition);
        } // 11

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_right_transition;
            combat_front_loophole_crouch_front_right_transition.m_action_from = "lookout";
            combat_front_loophole_crouch_front_right_transition.m_action_to = "idle";
            combat_front_loophole_crouch_front_right_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_right_transition.m_animations.push_back(
                "loophole_crouch_front_right_look_out_0");

            combat_front_loophole_crouch_front_right.register_transition(
                combat_front_loophole_crouch_front_right_transition);
        } // 12

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_right_transition;
            combat_front_loophole_crouch_front_right_transition.m_action_from = "idle";
            combat_front_loophole_crouch_front_right_transition.m_action_to = "fire";
            combat_front_loophole_crouch_front_right_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_right_transition.m_animations.push_back(
                "loophole_crouch_front_right_attack_in_0");

            combat_front_loophole_crouch_front_right.register_transition(
                combat_front_loophole_crouch_front_right_transition);
        } // 13

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_right_transition;
            combat_front_loophole_crouch_front_right_transition.m_action_from = "fire";
            combat_front_loophole_crouch_front_right_transition.m_action_to = "idle";
            combat_front_loophole_crouch_front_right_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_right_transition.m_animations.push_back(
                "loophole_crouch_front_right_attack_out_0");

            combat_front_loophole_crouch_front_right.register_transition(
                combat_front_loophole_crouch_front_right_transition);
        } // 14

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_right_transition;
            combat_front_loophole_crouch_front_right_transition.m_action_from = "idle";
            combat_front_loophole_crouch_front_right_transition.m_action_to = "fire_no_lookout";
            combat_front_loophole_crouch_front_right_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_right_transition.m_animations.push_back(
                "loophole_crouch_front_right_attack_in_0");

            combat_front_loophole_crouch_front_right.register_transition(
                combat_front_loophole_crouch_front_right_transition);
        } // 15

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_right_transition;
            combat_front_loophole_crouch_front_right_transition.m_action_from = "fire_no_lookout";
            combat_front_loophole_crouch_front_right_transition.m_action_to = "idle";
            combat_front_loophole_crouch_front_right_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_right_transition.m_animations.push_back(
                "loophole_crouch_front_right_attack_out_0");

            combat_front_loophole_crouch_front_right.register_transition(
                combat_front_loophole_crouch_front_right_transition);
        } // 16
#pragma endregion Cordis Loophole

#pragma region Cordis Loophole
        SmartCoverLoopholeData combat_front_loophole_crouch_front;
        combat_front_loophole_crouch_front.m_id = "crouch_front";
        combat_front_loophole_crouch_front.m_is_usable = true;
        combat_front_loophole_crouch_front.m_is_exitable = true;
        combat_front_loophole_crouch_front.m_is_enterable = true;
        combat_front_loophole_crouch_front.m_fieldofview = 70.0f;
        combat_front_loophole_crouch_front.m_fieldofview_danger = 110.0f;
        combat_front_loophole_crouch_front.m_range = 70.0f;
        combat_front_loophole_crouch_front.m_fieldofview_direction = Fvector().set(-1.0f, 0.0f, 0.0f);
        combat_front_loophole_crouch_front.m_fieldofview_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_loophole_crouch_front.m_danger_fieldofview_direction = Fvector().set(-1.0f, 0.0f, 0.0f);
        combat_front_loophole_crouch_front.m_enter_direction = Fvector().set(-1.0f, 0.0f, 0.0f);

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_action;
            combat_front_loophole_crouch_front_action.m_id = "idle";
            combat_front_loophole_crouch_front_action.register_animation("idle", "loophole_crouch_front_idle_0");

            combat_front_loophole_crouch_front.register_action(combat_front_loophole_crouch_front_action);
        } // 1

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_action;
            combat_front_loophole_crouch_front_action.m_id = "lookout";
            combat_front_loophole_crouch_front_action.register_animation("idle", "loophole_crouch_front_look_idle_0");

            combat_front_loophole_crouch_front.register_action(combat_front_loophole_crouch_front_action);
        } // 2

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_action;
            combat_front_loophole_crouch_front_action.m_id = "fire";
            combat_front_loophole_crouch_front_action.register_animation("idle", "loophole_crouch_front_attack_idle_0");
            combat_front_loophole_crouch_front_action.register_animation(
                "shoot", "loophole_crouch_front_attack_shoot_0");
            combat_front_loophole_crouch_front_action.register_animation(
                "shoot", "loophole_crouch_front_attack_shoot_1");

            combat_front_loophole_crouch_front.register_action(combat_front_loophole_crouch_front_action);
        } // 3

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_action;
            combat_front_loophole_crouch_front_action.m_id = "fire_no_lookout";
            combat_front_loophole_crouch_front_action.register_animation("idle", "loophole_crouch_front_attack_idle_0");
            combat_front_loophole_crouch_front_action.register_animation(
                "shoot", "loophole_crouch_front_attack_shoot_0");
            combat_front_loophole_crouch_front_action.register_animation(
                "shoot", "loophole_crouch_front_attack_shoot_1");

            combat_front_loophole_crouch_front.register_action(combat_front_loophole_crouch_front_action);
        } // 4

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_crouch_front_action;
            combat_front_loophole_crouch_front_action.m_id = "reload";
            combat_front_loophole_crouch_front_action.register_animation("idle", "loophole_crouch_front_reload_0");

            combat_front_loophole_crouch_front.register_action(combat_front_loophole_crouch_front_action);
        } // 5

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_transition;
            combat_front_loophole_crouch_front_transition.m_action_from = "idle";
            combat_front_loophole_crouch_front_transition.m_action_to = "lookout";
            combat_front_loophole_crouch_front_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_transition.m_animations.push_back("loophole_crouch_front_look_in_0");

            combat_front_loophole_crouch_front.register_transition(combat_front_loophole_crouch_front_transition);
        } // 11

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_transition;
            combat_front_loophole_crouch_front_transition.m_action_from = "lookout";
            combat_front_loophole_crouch_front_transition.m_action_to = "idle";
            combat_front_loophole_crouch_front_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_transition.m_animations.push_back("loophole_crouch_front_look_out_0");

            combat_front_loophole_crouch_front.register_transition(combat_front_loophole_crouch_front_transition);
        } // 12

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_transition;
            combat_front_loophole_crouch_front_transition.m_action_from = "idle";
            combat_front_loophole_crouch_front_transition.m_action_to = "fire";
            combat_front_loophole_crouch_front_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_transition.m_animations.push_back("loophole_crouch_front_attack_in_0");

            combat_front_loophole_crouch_front.register_transition(combat_front_loophole_crouch_front_transition);
        } // 13

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_transition;
            combat_front_loophole_crouch_front_transition.m_action_from = "fire";
            combat_front_loophole_crouch_front_transition.m_action_to = "idle";
            combat_front_loophole_crouch_front_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_transition.m_animations.push_back("loophole_crouch_front_attack_out_0");

            combat_front_loophole_crouch_front.register_transition(combat_front_loophole_crouch_front_transition);
        } // 14

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_transition;
            combat_front_loophole_crouch_front_transition.m_action_from = "idle";
            combat_front_loophole_crouch_front_transition.m_action_to = "fire_no_lookout";
            combat_front_loophole_crouch_front_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_transition.m_animations.push_back("loophole_crouch_front_attack_in_0");

            combat_front_loophole_crouch_front.register_transition(combat_front_loophole_crouch_front_transition);
        } // 15

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_crouch_front_transition;
            combat_front_loophole_crouch_front_transition.m_action_from = "fire_no_lookout";
            combat_front_loophole_crouch_front_transition.m_action_to = "idle";
            combat_front_loophole_crouch_front_transition.m_weight = 1.2f;
            combat_front_loophole_crouch_front_transition.m_animations.push_back("loophole_crouch_front_attack_out_0");

            combat_front_loophole_crouch_front.register_transition(combat_front_loophole_crouch_front_transition);
        } // 16
#pragma endregion Cordis Loophole

#pragma region Cordis Loophole
        SmartCoverLoopholeData combat_front_loophole_stand_front_left;
        combat_front_loophole_stand_front_left.m_id = "stand_front_left";
        combat_front_loophole_stand_front_left.m_is_usable = true;
        combat_front_loophole_stand_front_left.m_is_exitable = true;
        combat_front_loophole_stand_front_left.m_is_enterable = true;
        combat_front_loophole_stand_front_left.m_fieldofview = 60.0f;
        combat_front_loophole_stand_front_left.m_fieldofview_danger = 90.0f;
        combat_front_loophole_stand_front_left.m_range = 50.0f;
        combat_front_loophole_stand_front_left.m_fieldofview_direction = Fvector().set(-1.0f, 0.0f, -0.7f);
        combat_front_loophole_stand_front_left.m_fieldofview_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_loophole_stand_front_left.m_danger_fieldofview_direction = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_loophole_stand_front_left.m_enter_direction = Fvector().set(-1.0f, 0.0f, 0.0f);

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_stand_front_left_action;
            combat_front_loophole_stand_front_left_action.m_id = "idle";
            combat_front_loophole_stand_front_left_action.register_animation("idle", "loophole_stand_back_idle_0");

            combat_front_loophole_stand_front_left.register_action(combat_front_loophole_stand_front_left_action);
        } // 1

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_stand_front_left_action;
            combat_front_loophole_stand_front_left_action.m_id = "lookout";
            combat_front_loophole_stand_front_left_action.register_animation("idle", "loophole_stand_back_idle_0");

            combat_front_loophole_stand_front_left.register_action(combat_front_loophole_stand_front_left_action);
        } // 2

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_stand_front_left_action;
            combat_front_loophole_stand_front_left_action.m_id = "fire";
            combat_front_loophole_stand_front_left_action.register_animation(
                "idle", "loophole_stand_back_attack_idle_0");
            combat_front_loophole_stand_front_left_action.register_animation(
                "shoot", "loophole_stand_back_attack_shoot_0");
            combat_front_loophole_stand_front_left_action.register_animation(
                "shoot", "loophole_stand_back_attack_shoot_1");

            combat_front_loophole_stand_front_left.register_action(combat_front_loophole_stand_front_left_action);
        } // 3

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_stand_front_left_action;
            combat_front_loophole_stand_front_left_action.m_id = "fire_no_lookout";
            combat_front_loophole_stand_front_left_action.register_animation("idle", "loophole_stand_back_idle_0");
            combat_front_loophole_stand_front_left_action.register_animation(
                "shoot", "loophole_stand_back_attack_shoot_0");
            combat_front_loophole_stand_front_left_action.register_animation(
                "shoot", "loophole_stand_back_attack_shoot_1");

            combat_front_loophole_stand_front_left.register_action(combat_front_loophole_stand_front_left_action);
        } // 4

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_stand_front_left_action;
            combat_front_loophole_stand_front_left_action.m_id = "reload";
            combat_front_loophole_stand_front_left_action.register_animation("idle", "loophole_stand_back_reload_0");

            combat_front_loophole_stand_front_left.register_action(combat_front_loophole_stand_front_left_action);
        } // 5

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_stand_front_left_transition;
            combat_front_loophole_stand_front_left_transition.m_action_from = "idle";
            combat_front_loophole_stand_front_left_transition.m_action_to = "lookout";
            combat_front_loophole_stand_front_left_transition.m_weight = 1.2f;
            combat_front_loophole_stand_front_left_transition.m_animations.push_back("loophole_stand_back_idle_0");

            combat_front_loophole_stand_front_left.register_transition(
                combat_front_loophole_stand_front_left_transition);
        } // 11

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_stand_front_left_transition;
            combat_front_loophole_stand_front_left_transition.m_action_from = "lookout";
            combat_front_loophole_stand_front_left_transition.m_action_to = "idle";
            combat_front_loophole_stand_front_left_transition.m_weight = 1.2f;
            combat_front_loophole_stand_front_left_transition.m_animations.push_back("loophole_stand_back_idle_0");

            combat_front_loophole_stand_front_left.register_transition(
                combat_front_loophole_stand_front_left_transition);
        } // 12

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_stand_front_left_transition;
            combat_front_loophole_stand_front_left_transition.m_action_from = "idle";
            combat_front_loophole_stand_front_left_transition.m_action_to = "fire";
            combat_front_loophole_stand_front_left_transition.m_weight = 1.2f;
            combat_front_loophole_stand_front_left_transition.m_animations.push_back("loophole_stand_back_attack_in_0");

            combat_front_loophole_stand_front_left.register_transition(
                combat_front_loophole_stand_front_left_transition);
        } // 13

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_stand_front_left_transition;
            combat_front_loophole_stand_front_left_transition.m_action_from = "fire";
            combat_front_loophole_stand_front_left_transition.m_action_to = "idle";
            combat_front_loophole_stand_front_left_transition.m_weight = 1.2f;
            combat_front_loophole_stand_front_left_transition.m_animations.push_back(
                "loophole_stand_back_attack_out_0");

            combat_front_loophole_stand_front_left.register_transition(
                combat_front_loophole_stand_front_left_transition);
        } // 14

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_stand_front_left_transition;
            combat_front_loophole_stand_front_left_transition.m_action_from = "idle";
            combat_front_loophole_stand_front_left_transition.m_action_to = "fire_no_lookout";
            combat_front_loophole_stand_front_left_transition.m_weight = 1.2f;
            combat_front_loophole_stand_front_left_transition.m_animations.push_back("loophole_stand_back_attack_in_0");

            combat_front_loophole_stand_front_left.register_transition(
                combat_front_loophole_stand_front_left_transition);
        } // 15

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_stand_front_left_transition;
            combat_front_loophole_stand_front_left_transition.m_action_from = "fire_no_lookout";
            combat_front_loophole_stand_front_left_transition.m_action_to = "idle";
            combat_front_loophole_stand_front_left_transition.m_weight = 1.2f;
            combat_front_loophole_stand_front_left_transition.m_animations.push_back(
                "loophole_stand_back_attack_out_0");

            combat_front_loophole_stand_front_left.register_transition(
                combat_front_loophole_stand_front_left_transition);
        } // 16
#pragma endregion Cordis Loophole

#pragma region Cordis Loophole
        SmartCoverLoopholeData combat_front_loophole_stand_front_right;
        combat_front_loophole_stand_front_right.m_id = "stand_front_right";
        combat_front_loophole_stand_front_right.m_is_usable = true;
        combat_front_loophole_stand_front_right.m_is_exitable = true;
        combat_front_loophole_stand_front_right.m_is_enterable = true;
        combat_front_loophole_stand_front_right.m_fieldofview = 60.0f;
        combat_front_loophole_stand_front_right.m_fieldofview_danger = 90.0f;
        combat_front_loophole_stand_front_right.m_range = 50.0f;
        combat_front_loophole_stand_front_right.m_fieldofview_direction = Fvector().set(-1.0f, 0.0f, 0.7f);
        combat_front_loophole_stand_front_right.m_fieldofview_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_loophole_stand_front_right.m_danger_fieldofview_direction = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_loophole_stand_front_right.m_enter_direction = Fvector().set(-1.0f, 0.0f, 0.0f);

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_stand_front_right_action;
            combat_front_loophole_stand_front_right_action.m_id = "idle";
            combat_front_loophole_stand_front_right_action.register_animation("idle", "loophole_stand_back_idle_0");

            combat_front_loophole_stand_front_right.register_action(combat_front_loophole_stand_front_right_action);
        } // 1

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_stand_front_right_action;
            combat_front_loophole_stand_front_right_action.m_id = "lookout";
            combat_front_loophole_stand_front_right_action.register_animation("idle", "loophole_stand_back_idle_0");

            combat_front_loophole_stand_front_right.register_action(combat_front_loophole_stand_front_right_action);
        } // 2

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_stand_front_right_action;
            combat_front_loophole_stand_front_right_action.m_id = "fire";
            combat_front_loophole_stand_front_right_action.register_animation(
                "idle", "loophole_stand_back_attack_idle_0");
            combat_front_loophole_stand_front_right_action.register_animation(
                "shoot", "loophole_stand_back_attack_shoot_0");
            combat_front_loophole_stand_front_right_action.register_animation(
                "shoot", "loophole_stand_back_attack_shoot_1");

            combat_front_loophole_stand_front_right.register_action(combat_front_loophole_stand_front_right_action);
        } // 3

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_stand_front_right_action;
            combat_front_loophole_stand_front_right_action.m_id = "fire_no_lookout";
            combat_front_loophole_stand_front_right_action.register_animation(
                "idle", "loophole_stand_back_attack_idle_0");
            combat_front_loophole_stand_front_right_action.register_animation(
                "shoot", "loophole_stand_back_attack_shoot_0");
            combat_front_loophole_stand_front_right_action.register_animation(
                "shoot", "loophole_stand_back_attack_shoot_1");

            combat_front_loophole_stand_front_right.register_action(combat_front_loophole_stand_front_right_action);
        } // 4

        {
            SmartCoverLoopholeData::SmartCoverActionsData combat_front_loophole_stand_front_right_action;
            combat_front_loophole_stand_front_right_action.m_id = "reload";
            combat_front_loophole_stand_front_right_action.register_animation("idle", "loophole_stand_back_reload_0");

            combat_front_loophole_stand_front_right.register_action(combat_front_loophole_stand_front_right_action);
        } // 5

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_stand_front_right_transition;
            combat_front_loophole_stand_front_right_transition.m_action_from = "idle";
            combat_front_loophole_stand_front_right_transition.m_action_to = "lookout";
            combat_front_loophole_stand_front_right_transition.m_weight = 1.2f;
            combat_front_loophole_stand_front_right_transition.m_animations.push_back("loophole_stand_back_idle_0");

            combat_front_loophole_stand_front_right.register_transition(
                combat_front_loophole_stand_front_right_transition);
        } // 11

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_stand_front_right_transition;
            combat_front_loophole_stand_front_right_transition.m_action_from = "lookout";
            combat_front_loophole_stand_front_right_transition.m_action_to = "idle";
            combat_front_loophole_stand_front_right_transition.m_weight = 1.2f;
            combat_front_loophole_stand_front_right_transition.m_animations.push_back("loophole_stand_back_idle_0");

            combat_front_loophole_stand_front_right.register_transition(
                combat_front_loophole_stand_front_right_transition);
        } // 12

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_stand_front_right_transition;
            combat_front_loophole_stand_front_right_transition.m_action_from = "idle";
            combat_front_loophole_stand_front_right_transition.m_action_to = "fire";
            combat_front_loophole_stand_front_right_transition.m_weight = 1.2f;
            combat_front_loophole_stand_front_right_transition.m_animations.push_back(
                "loophole_stand_back_attack_in_0");

            combat_front_loophole_stand_front_right.register_transition(
                combat_front_loophole_stand_front_right_transition);
        } // 13

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_stand_front_right_transition;
            combat_front_loophole_stand_front_right_transition.m_action_from = "fire";
            combat_front_loophole_stand_front_right_transition.m_action_to = "idle";
            combat_front_loophole_stand_front_right_transition.m_weight = 1.2f;
            combat_front_loophole_stand_front_right_transition.m_animations.push_back(
                "loophole_stand_back_attack_out_0");

            combat_front_loophole_stand_front_right.register_transition(
                combat_front_loophole_stand_front_right_transition);
        } // 14

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_stand_front_right_transition;
            combat_front_loophole_stand_front_right_transition.m_action_from = "idle";
            combat_front_loophole_stand_front_right_transition.m_action_to = "fire_no_lookout";
            combat_front_loophole_stand_front_right_transition.m_weight = 1.2f;
            combat_front_loophole_stand_front_right_transition.m_animations.push_back(
                "loophole_stand_back_attack_in_0");

            combat_front_loophole_stand_front_right.register_transition(
                combat_front_loophole_stand_front_right_transition);
        } // 15

        {
            SmartCoverLoopholeData::SmartCoverTransitionsData combat_front_loophole_stand_front_right_transition;
            combat_front_loophole_stand_front_right_transition.m_action_from = "fire_no_lookout";
            combat_front_loophole_stand_front_right_transition.m_action_to = "idle";
            combat_front_loophole_stand_front_right_transition.m_weight = 1.2f;
            combat_front_loophole_stand_front_right_transition.m_animations.push_back(
                "loophole_stand_back_attack_out_0");

            combat_front_loophole_stand_front_right.register_transition(
                combat_front_loophole_stand_front_right_transition);
        } // 16

#pragma endregion Cordis Loophole

#pragma region Cordis Transition
        SmartCoverData::SmartCoverTransitionsData combat_front_transition1;
        combat_front_transition1.m_vertex0 = "";
        combat_front_transition1.m_vertex1 = "crouch_front_left";
        combat_front_transition1.m_weight = 1.0f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition1_action1;
        combat_front_transition1_action1.m_action.m_animation = "loophole_crouch_in_front_left_0";
        combat_front_transition1_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition1_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition1_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition1_action1.m_precondition_functor = "true";
        combat_front_transition1_action1.m_preconditions_params = "";

        combat_front_transition1.m_actions.push_back(combat_front_transition1_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition2;
        combat_front_transition2.m_vertex0 = "";
        combat_front_transition2.m_vertex1 = "crouch_front";
        combat_front_transition2.m_weight = 1.0f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition2_action1;
        combat_front_transition2_action1.m_action.m_animation = "loophole_crouch_in_front_0";
        combat_front_transition2_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition2_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition2_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition2_action1.m_precondition_functor = "true";
        combat_front_transition2_action1.m_preconditions_params = "";

        combat_front_transition2.m_actions.push_back(combat_front_transition2_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition3;
        combat_front_transition3.m_vertex0 = "";
        combat_front_transition3.m_vertex1 = "crouch_front_right";
        combat_front_transition3.m_weight = 1.0f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition3_action1;
        combat_front_transition3_action1.m_action.m_animation = "loophole_crouch_in_front_right_0";
        combat_front_transition3_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition3_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition3_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition3_action1.m_precondition_functor = "true";
        combat_front_transition3_action1.m_preconditions_params = "";

        combat_front_transition3.m_actions.push_back(combat_front_transition3_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition4;
        combat_front_transition4.m_vertex0 = "";
        combat_front_transition4.m_vertex1 = "stand_front_left";
        combat_front_transition4.m_weight = 1.0f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition4_action1;
        combat_front_transition4_action1.m_action.m_animation = "loophole_stand_in_front_left_0";
        combat_front_transition4_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition4_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition4_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition4_action1.m_precondition_functor = "true";
        combat_front_transition4_action1.m_preconditions_params = "";

        combat_front_transition4.m_actions.push_back(combat_front_transition4_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition5;
        combat_front_transition5.m_vertex0 = "";
        combat_front_transition5.m_vertex1 = "stand_front_right";
        combat_front_transition5.m_weight = 1.0f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition5_action1;
        combat_front_transition5_action1.m_action.m_animation = "loophole_stand_in_front_right_0";
        combat_front_transition5_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition5_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition5_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition5_action1.m_precondition_functor = "true";
        combat_front_transition5_action1.m_preconditions_params = "";

        combat_front_transition5.m_actions.push_back(combat_front_transition5_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition6;
        combat_front_transition6.m_vertex0 = "crouch_front";
        combat_front_transition6.m_vertex1 = "crouch_front_left";
        combat_front_transition6.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition6_action1;
        combat_front_transition6_action1.m_action.m_animation = "crouch_front_to_crouch_front_left";
        combat_front_transition6_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition6_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition6_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition6_action1.m_precondition_functor = "true";
        combat_front_transition6_action1.m_preconditions_params = "";

        combat_front_transition6.m_actions.push_back(combat_front_transition6_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition7;
        combat_front_transition7.m_vertex0 = "crouch_front";
        combat_front_transition7.m_vertex1 = "crouch_front_right";
        combat_front_transition7.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition7_action1;
        combat_front_transition7_action1.m_action.m_animation = "crouch_front_to_crouch_front_right";
        combat_front_transition7_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition7_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition7_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition7_action1.m_precondition_functor = "true";
        combat_front_transition7_action1.m_preconditions_params = "";

        combat_front_transition7.m_actions.push_back(combat_front_transition7_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition8;
        combat_front_transition8.m_vertex0 = "crouch_front";
        combat_front_transition8.m_vertex1 = "stand_front_left";
        combat_front_transition8.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition8_action1;
        combat_front_transition8_action1.m_action.m_animation = "crouch_front_to_stand_front_left";
        combat_front_transition8_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition8_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition8_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition8_action1.m_precondition_functor = "true";
        combat_front_transition8_action1.m_preconditions_params = "";

        combat_front_transition8.m_actions.push_back(combat_front_transition8_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition9;
        combat_front_transition9.m_vertex0 = "crouch_front";
        combat_front_transition9.m_vertex1 = "stand_front_right";
        combat_front_transition9.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition9_action1;
        combat_front_transition9_action1.m_action.m_animation = "crouch_front_to_stand_front_right";
        combat_front_transition9_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition9_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition9_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition9_action1.m_precondition_functor = "true";
        combat_front_transition9_action1.m_preconditions_params = "";

        combat_front_transition9.m_actions.push_back(combat_front_transition9_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition10;
        combat_front_transition10.m_vertex0 = "crouch_front_right";
        combat_front_transition10.m_vertex1 = "crouch_front";
        combat_front_transition10.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition10_action1;
        combat_front_transition10_action1.m_action.m_animation = "crouch_front_right_to_crouch_front";
        combat_front_transition10_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition10_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition10_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition10_action1.m_precondition_functor = "true";
        combat_front_transition10_action1.m_preconditions_params = "";

        combat_front_transition10.m_actions.push_back(combat_front_transition10_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition11;
        combat_front_transition11.m_vertex0 = "crouch_front_right";
        combat_front_transition11.m_vertex1 = "crouch_front_left";
        combat_front_transition11.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition11_action1;
        combat_front_transition11_action1.m_action.m_animation = "crouch_front_right_to_crouch_front_left";
        combat_front_transition11_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition11_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition11_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition11_action1.m_precondition_functor = "true";
        combat_front_transition11_action1.m_preconditions_params = "";

        combat_front_transition11.m_actions.push_back(combat_front_transition11_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition12;
        combat_front_transition12.m_vertex0 = "crouch_front_right";
        combat_front_transition12.m_vertex1 = "stand_front_left";
        combat_front_transition12.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition12_action1;
        combat_front_transition12_action1.m_action.m_animation = "crouch_front_right_to_stand_front_left";
        combat_front_transition12_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition12_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition12_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition12_action1.m_precondition_functor = "true";
        combat_front_transition12_action1.m_preconditions_params = "";

        combat_front_transition12.m_actions.push_back(combat_front_transition12_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition13;
        combat_front_transition13.m_vertex0 = "crouch_front_right";
        combat_front_transition13.m_vertex1 = "stand_front_right";
        combat_front_transition13.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition13_action1;
        combat_front_transition13_action1.m_action.m_animation = "crouch_front_right_to_stand_front_right";
        combat_front_transition13_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition13_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition13_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition13_action1.m_precondition_functor = "true";
        combat_front_transition13_action1.m_preconditions_params = "";

        combat_front_transition13.m_actions.push_back(combat_front_transition13_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition14;
        combat_front_transition14.m_vertex0 = "crouch_front_left";
        combat_front_transition14.m_vertex1 = "crouch_front";
        combat_front_transition14.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition14_action1;
        combat_front_transition14_action1.m_action.m_animation = "crouch_front_left_to_crouch_front";
        combat_front_transition14_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition14_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition14_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition14_action1.m_precondition_functor = "true";
        combat_front_transition14_action1.m_preconditions_params = "";

        combat_front_transition14.m_actions.push_back(combat_front_transition14_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition15;
        combat_front_transition15.m_vertex0 = "crouch_front_left";
        combat_front_transition15.m_vertex1 = "crouch_front_right";
        combat_front_transition15.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition15_action1;
        combat_front_transition15_action1.m_action.m_animation = "crouch_front_left_to_crouch_front_right";
        combat_front_transition15_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition15_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition15_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition15_action1.m_precondition_functor = "true";
        combat_front_transition15_action1.m_preconditions_params = "";

        combat_front_transition15.m_actions.push_back(combat_front_transition15_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition16;
        combat_front_transition16.m_vertex0 = "crouch_front_left";
        combat_front_transition16.m_vertex1 = "stand_front_left";
        combat_front_transition16.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition16_action1;
        combat_front_transition16_action1.m_action.m_animation = "crouch_front_left_to_stand_front_left";
        combat_front_transition16_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition16_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition16_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition16_action1.m_precondition_functor = "true";
        combat_front_transition16_action1.m_preconditions_params = "";

        combat_front_transition16.m_actions.push_back(combat_front_transition16_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition17;
        combat_front_transition17.m_vertex0 = "crouch_front_left";
        combat_front_transition17.m_vertex1 = "stand_front_right";
        combat_front_transition17.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition17_action1;
        combat_front_transition17_action1.m_action.m_animation = "crouch_front_left_to_stand_front_right";
        combat_front_transition17_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition17_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition17_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition17_action1.m_precondition_functor = "true";
        combat_front_transition17_action1.m_preconditions_params = "";

        combat_front_transition17.m_actions.push_back(combat_front_transition17_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition18;
        combat_front_transition18.m_vertex0 = "stand_front_left";
        combat_front_transition18.m_vertex1 = "crouch_front";
        combat_front_transition18.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition18_action1;
        combat_front_transition18_action1.m_action.m_animation = "stand_front_left_to_crouch_front";
        combat_front_transition18_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition18_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition18_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition18_action1.m_precondition_functor = "true";
        combat_front_transition18_action1.m_preconditions_params = "";

        combat_front_transition18.m_actions.push_back(combat_front_transition18_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition19;
        combat_front_transition19.m_vertex0 = "stand_front_left";
        combat_front_transition19.m_vertex1 = "crouch_front_left";
        combat_front_transition19.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition19_action1;
        combat_front_transition19_action1.m_action.m_animation = "stand_front_left_to_crouch_front_left";
        combat_front_transition19_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition19_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition19_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition19_action1.m_precondition_functor = "true";
        combat_front_transition19_action1.m_preconditions_params = "";

        combat_front_transition19.m_actions.push_back(combat_front_transition19_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition20;
        combat_front_transition20.m_vertex0 = "stand_front_left";
        combat_front_transition20.m_vertex1 = "crouch_front_right";
        combat_front_transition20.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition20_action1;
        combat_front_transition20_action1.m_action.m_animation = "stand_front_left_to_crouch_front_right";
        combat_front_transition20_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition20_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition20_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition20_action1.m_precondition_functor = "true";
        combat_front_transition20_action1.m_preconditions_params = "";

        combat_front_transition20.m_actions.push_back(combat_front_transition20_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition21;
        combat_front_transition21.m_vertex0 = "stand_front_left";
        combat_front_transition21.m_vertex1 = "stand_front_right";
        combat_front_transition21.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition21_action1;
        combat_front_transition21_action1.m_action.m_animation = "stand_front_left_to_stand_front_right";
        combat_front_transition21_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition21_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition21_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition21_action1.m_precondition_functor = "true";
        combat_front_transition21_action1.m_preconditions_params = "";

        combat_front_transition21.m_actions.push_back(combat_front_transition21_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition22;
        combat_front_transition22.m_vertex0 = "stand_front_right";
        combat_front_transition22.m_vertex1 = "crouch_front";
        combat_front_transition22.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition22_action1;
        combat_front_transition22_action1.m_action.m_animation = "stand_front_right_to_crouch_front";
        combat_front_transition22_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition22_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition22_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition22_action1.m_precondition_functor = "true";
        combat_front_transition22_action1.m_preconditions_params = "";

        combat_front_transition22.m_actions.push_back(combat_front_transition22_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition23;
        combat_front_transition23.m_vertex0 = "stand_front_right";
        combat_front_transition23.m_vertex1 = "crouch_front_left";
        combat_front_transition23.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition23_action1;
        combat_front_transition23_action1.m_action.m_animation = "stand_front_right_to_crouch_front_left";
        combat_front_transition23_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition23_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition23_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition23_action1.m_precondition_functor = "true";
        combat_front_transition23_action1.m_preconditions_params = "";

        combat_front_transition23.m_actions.push_back(combat_front_transition23_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition24;
        combat_front_transition24.m_vertex0 = "stand_front_right";
        combat_front_transition24.m_vertex1 = "crouch_front_right";
        combat_front_transition24.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition24_action1;
        combat_front_transition24_action1.m_action.m_animation = "stand_front_right_to_crouch_front_right";
        combat_front_transition24_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition24_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition24_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition24_action1.m_precondition_functor = "true";
        combat_front_transition24_action1.m_preconditions_params = "";

        combat_front_transition24.m_actions.push_back(combat_front_transition24_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition25;
        combat_front_transition25.m_vertex0 = "stand_front_right";
        combat_front_transition25.m_vertex1 = "stand_front_left";
        combat_front_transition25.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition25_action1;
        combat_front_transition25_action1.m_action.m_animation = "stand_front_right_to_stand_front_left";
        combat_front_transition25_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition25_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition25_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition25_action1.m_precondition_functor = "true";
        combat_front_transition25_action1.m_preconditions_params = "";

        combat_front_transition25.m_actions.push_back(combat_front_transition25_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition26;
        combat_front_transition26.m_vertex0 = "crouch_front_left";
        combat_front_transition26.m_vertex1 = "";
        combat_front_transition26.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition26_action1;
        combat_front_transition26_action1.m_action.m_animation = "";
        combat_front_transition26_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition26_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition26_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition26_action1.m_precondition_functor = "true";
        combat_front_transition26_action1.m_preconditions_params = "";

        combat_front_transition26.m_actions.push_back(combat_front_transition26_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition27;
        combat_front_transition27.m_vertex0 = "crouch_front";
        combat_front_transition27.m_vertex1 = "";
        combat_front_transition27.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition27_action1;
        combat_front_transition27_action1.m_action.m_animation = "";
        combat_front_transition27_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition27_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition27_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition27_action1.m_precondition_functor = "true";
        combat_front_transition27_action1.m_preconditions_params = "";

        combat_front_transition27.m_actions.push_back(combat_front_transition27_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition28;
        combat_front_transition28.m_vertex0 = "crouch_front_right";
        combat_front_transition28.m_vertex1 = "";
        combat_front_transition28.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition28_action1;
        combat_front_transition28_action1.m_action.m_animation = "";
        combat_front_transition28_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition28_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition28_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition28_action1.m_precondition_functor = "true";
        combat_front_transition28_action1.m_preconditions_params = "";

        combat_front_transition28.m_actions.push_back(combat_front_transition28_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition29;
        combat_front_transition29.m_vertex0 = "stand_front_left";
        combat_front_transition29.m_vertex1 = "";
        combat_front_transition29.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition29_action1;
        combat_front_transition29_action1.m_action.m_animation = "";
        combat_front_transition29_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition29_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition29_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition29_action1.m_precondition_functor = "true";
        combat_front_transition29_action1.m_preconditions_params = "";

        combat_front_transition29.m_actions.push_back(combat_front_transition29_action1);

        SmartCoverData::SmartCoverTransitionsData combat_front_transition30;
        combat_front_transition30.m_vertex0 = "stand_front_right";
        combat_front_transition30.m_vertex1 = "";
        combat_front_transition30.m_weight = 1.1f;
        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData combat_front_transition30_action1;
        combat_front_transition30_action1.m_action.m_animation = "";
        combat_front_transition30_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        combat_front_transition30_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        combat_front_transition30_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        combat_front_transition30_action1.m_precondition_functor = "true";
        combat_front_transition30_action1.m_preconditions_params = "";

        combat_front_transition30.m_actions.push_back(combat_front_transition30_action1);
#pragma endregion

        combat_front_smartcover.setNeedWeapon(true);
        combat_front_smartcover.register_loophole(combat_front_loophole_crouch_front_left);
        combat_front_smartcover.register_loophole(combat_front_loophole_crouch_front);
        combat_front_smartcover.register_loophole(combat_front_loophole_crouch_front_right);
        combat_front_smartcover.register_loophole(combat_front_loophole_stand_front_left);
        combat_front_smartcover.register_loophole(combat_front_loophole_stand_front_right);
        combat_front_smartcover.register_transition(combat_front_transition1);
        combat_front_smartcover.register_transition(combat_front_transition2);
        combat_front_smartcover.register_transition(combat_front_transition3);
        combat_front_smartcover.register_transition(combat_front_transition4);
        combat_front_smartcover.register_transition(combat_front_transition5);
        combat_front_smartcover.register_transition(combat_front_transition6);
        combat_front_smartcover.register_transition(combat_front_transition7);
        combat_front_smartcover.register_transition(combat_front_transition8);
        combat_front_smartcover.register_transition(combat_front_transition9);
        combat_front_smartcover.register_transition(combat_front_transition10);
        combat_front_smartcover.register_transition(combat_front_transition11);
        combat_front_smartcover.register_transition(combat_front_transition12);
        combat_front_smartcover.register_transition(combat_front_transition13);
        combat_front_smartcover.register_transition(combat_front_transition14);
        combat_front_smartcover.register_transition(combat_front_transition15);
        combat_front_smartcover.register_transition(combat_front_transition16);
        combat_front_smartcover.register_transition(combat_front_transition17);
        combat_front_smartcover.register_transition(combat_front_transition18);
        combat_front_smartcover.register_transition(combat_front_transition19);
        combat_front_smartcover.register_transition(combat_front_transition20);
        combat_front_smartcover.register_transition(combat_front_transition21);
        combat_front_smartcover.register_transition(combat_front_transition22);
        combat_front_smartcover.register_transition(combat_front_transition23);
        combat_front_smartcover.register_transition(combat_front_transition24);
        combat_front_smartcover.register_transition(combat_front_transition25);
        combat_front_smartcover.register_transition(combat_front_transition26);
        combat_front_smartcover.register_transition(combat_front_transition27);
        combat_front_smartcover.register_transition(combat_front_transition28);
        combat_front_smartcover.register_transition(combat_front_transition29);
        combat_front_smartcover.register_transition(combat_front_transition30);
        this->m_registered_smartcovers["combat_front"] = combat_front_smartcover;
#pragma endregion Cordis Combat Front Smart Cover

#pragma region Cordis Animpoint Stay Wall Smart Cover Initialization
        SmartCoverData animpoint_stay_wall_smartcover;
        SmartCoverLoopholeData animpoint_stay_wall_loophole;

#pragma region Cordis Loophole
        animpoint_stay_wall_loophole.m_id = "animpoint_stay_wall";
        animpoint_stay_wall_loophole.m_is_usable = true;
        animpoint_stay_wall_loophole.m_is_exitable = false;
        animpoint_stay_wall_loophole.m_is_enterable = false;
        animpoint_stay_wall_loophole.m_fieldofview = 45.0f;
        animpoint_stay_wall_loophole.m_fieldofview_danger = 45.0f;
        animpoint_stay_wall_loophole.m_range = 70.0f;
        animpoint_stay_wall_loophole.m_fieldofview_direction = Fvector().set(0.0f, 0.0f, -1.0f);
        animpoint_stay_wall_loophole.m_fieldofview_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_stay_wall_loophole.m_danger_fieldofview_direction = Fvector().set(-1.0f, 0.0f, 0.0f);
        animpoint_stay_wall_loophole.m_enter_direction = Fvector().set(0.0f, 0.0f, -1.0f);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_stay_wall_loophole_action1;
        animpoint_stay_wall_loophole_action1.m_id = "idle";
        animpoint_stay_wall_loophole_action1.register_animation("idle", "animpoint_stay_wall_idle_1");

        animpoint_stay_wall_loophole.register_action(animpoint_stay_wall_loophole_action1);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_stay_wall_loophole_action2;
        animpoint_stay_wall_loophole_action2.m_id = "lookout";
        animpoint_stay_wall_loophole_action2.register_animation("idle", "animpoint_stay_wall_idle_1");

        animpoint_stay_wall_loophole.register_action(animpoint_stay_wall_loophole_action2);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_stay_wall_loophole_action3;
        animpoint_stay_wall_loophole_action3.m_id = "fire";
        animpoint_stay_wall_loophole_action3.register_animation("idle", "animpoint_stay_wall_idle_1");
        animpoint_stay_wall_loophole_action3.register_animation("shoot", "animpoint_stay_wall_idle_1");

        animpoint_stay_wall_loophole.register_action(animpoint_stay_wall_loophole_action3);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_stay_wall_loophole_action4;
        animpoint_stay_wall_loophole_action4.m_id = "fire_no_lookout";
        animpoint_stay_wall_loophole_action4.register_animation("idle", "animpoint_stay_wall_idle_1");
        animpoint_stay_wall_loophole_action4.register_animation("shoot", "animpoint_stay_wall_idle_1");

        animpoint_stay_wall_loophole.register_action(animpoint_stay_wall_loophole_action4);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_stay_wall_loophole_action5;
        animpoint_stay_wall_loophole_action5.m_id = "reload";
        animpoint_stay_wall_loophole_action5.register_animation("idle", "animpoint_stay_wall_idle_1");

        animpoint_stay_wall_loophole.register_action(animpoint_stay_wall_loophole_action5);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_stay_wall_loophole_transition1;
        animpoint_stay_wall_loophole_transition1.m_action_from = "idle";
        animpoint_stay_wall_loophole_transition1.m_action_to = "lookout";
        animpoint_stay_wall_loophole_transition1.m_animations.push_back("animpoint_stay_wall_in_1");
        animpoint_stay_wall_loophole_transition1.m_weight = 1.2f;

        animpoint_stay_wall_loophole.register_transition(animpoint_stay_wall_loophole_transition1);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_stay_wall_loophole_transition2;
        animpoint_stay_wall_loophole_transition2.m_action_from = "lookout";
        animpoint_stay_wall_loophole_transition2.m_action_to = "idle";
        animpoint_stay_wall_loophole_transition2.m_animations.push_back("animpoint_stay_wall_in_1");
        animpoint_stay_wall_loophole_transition2.m_weight = 1.2f;

        animpoint_stay_wall_loophole.register_transition(animpoint_stay_wall_loophole_transition2);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_stay_wall_loophole_transition3;
        animpoint_stay_wall_loophole_transition3.m_action_from = "idle";
        animpoint_stay_wall_loophole_transition3.m_action_to = "fire";
        animpoint_stay_wall_loophole_transition3.m_animations.push_back("animpoint_stay_wall_in_1");
        animpoint_stay_wall_loophole_transition3.m_weight = 1.2f;

        animpoint_stay_wall_loophole.register_transition(animpoint_stay_wall_loophole_transition3);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_stay_wall_loophole_transition4;
        animpoint_stay_wall_loophole_transition4.m_action_from = "fire";
        animpoint_stay_wall_loophole_transition4.m_action_to = "idle";
        animpoint_stay_wall_loophole_transition4.m_animations.push_back("animpoint_stay_wall_in_1");
        animpoint_stay_wall_loophole_transition4.m_weight = 1.2f;

        animpoint_stay_wall_loophole.register_transition(animpoint_stay_wall_loophole_transition4);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_stay_wall_loophole_transition5;
        animpoint_stay_wall_loophole_transition5.m_action_from = "idle";
        animpoint_stay_wall_loophole_transition5.m_action_to = "fire_no_lookout";
        animpoint_stay_wall_loophole_transition5.m_animations.push_back("animpoint_stay_wall_in_1");
        animpoint_stay_wall_loophole_transition5.m_weight = 1.2f;

        animpoint_stay_wall_loophole.register_transition(animpoint_stay_wall_loophole_transition5);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_stay_wall_loophole_transition6;
        animpoint_stay_wall_loophole_transition6.m_action_from = "fire_no_lookout";
        animpoint_stay_wall_loophole_transition6.m_action_to = "idle";
        animpoint_stay_wall_loophole_transition6.m_animations.push_back("animpoint_stay_wall_in_1");
        animpoint_stay_wall_loophole_transition6.m_weight = 1.2f;

        animpoint_stay_wall_loophole.register_transition(animpoint_stay_wall_loophole_transition6);

#pragma endregion

#pragma region Cordis Transition
        SmartCoverData::SmartCoverTransitionsData animpoint_stay_wall_transition;
        animpoint_stay_wall_transition.m_vertex0 = "";
        animpoint_stay_wall_transition.m_vertex1 = "animpoint_stay_wall";
        animpoint_stay_wall_transition.m_weight = 1.0f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData animpoint_stay_wall_transition_action1;
        animpoint_stay_wall_transition_action1.m_action.m_animation = "animpoint_stay_wall_in_1";
        animpoint_stay_wall_transition_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_stay_wall_transition_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        animpoint_stay_wall_transition_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        animpoint_stay_wall_transition_action1.m_precondition_functor = "true";
        animpoint_stay_wall_transition_action1.m_preconditions_params = "";

        animpoint_stay_wall_transition.m_actions.push_back(animpoint_stay_wall_transition_action1);

        /*
                SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData animpoint_stay_wall_transition_action2;
                animpoint_stay_wall_transition_action2.m_action.m_animation = "animpoint_stay_wall_out_1";
                animpoint_stay_wall_transition_action2.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
                animpoint_stay_wall_transition_action2.m_action.m_body_state = MonsterSpace::eBodyStateStand;
                animpoint_stay_wall_transition_action2.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
                animpoint_stay_wall_transition_action2.m_precondition_functor = "true";
                animpoint_stay_wall_transition_action2.m_preconditions_params = "";

                animpoint_stay_wall_transition.m_actions.push_back(animpoint_stay_wall_transition_action2);
                */

        SmartCoverData::SmartCoverTransitionsData animpoint_stay_wall_transition1;
        animpoint_stay_wall_transition1.m_vertex0 = "animpoint_stay_wall";
        animpoint_stay_wall_transition1.m_vertex1 = "";
        animpoint_stay_wall_transition1.m_weight = 1.1f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData animpoint_stay_wall_transition1_action1;
        animpoint_stay_wall_transition1_action1.m_action.m_animation = "animpoint_stay_wall_out_1";
        animpoint_stay_wall_transition1_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_stay_wall_transition1_action1.m_action.m_body_state = MonsterSpace::eBodyStateStand;
        animpoint_stay_wall_transition1_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        animpoint_stay_wall_transition1_action1.m_precondition_functor = "true";
        animpoint_stay_wall_transition1_action1.m_preconditions_params = "";

        animpoint_stay_wall_transition1.m_actions.push_back(animpoint_stay_wall_transition1_action1);
#pragma endregion

        animpoint_stay_wall_smartcover.setNeedWeapon(false);
        animpoint_stay_wall_smartcover.register_loophole(animpoint_stay_wall_loophole);
        animpoint_stay_wall_smartcover.register_transition(animpoint_stay_wall_transition);
        animpoint_stay_wall_smartcover.register_transition(animpoint_stay_wall_transition1);
        this->m_registered_smartcovers["animpoint_stay_wall"] = animpoint_stay_wall_smartcover;
#pragma endregion

#pragma region Cordis Animpoint Stay Table Smart Cover
        SmartCoverData animpoint_stay_table_smartcover;
        SmartCoverLoopholeData animpoint_stay_table_loophole;

#pragma region Cordis Loophole
        animpoint_stay_table_loophole.m_id = "animpoint_stay_table";
        animpoint_stay_table_loophole.m_is_usable = true;
        animpoint_stay_table_loophole.m_is_exitable = true;
        animpoint_stay_table_loophole.m_is_enterable = true;
        animpoint_stay_table_loophole.m_fieldofview = 45.0f;
        animpoint_stay_table_loophole.m_fieldofview_danger = 45.0f;
        animpoint_stay_table_loophole.m_range = 70.0f;
        animpoint_stay_table_loophole.m_fieldofview_direction = Fvector().set(0.0f, 0.0f, -1.0f);
        animpoint_stay_table_loophole.m_fieldofview_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_stay_table_loophole.m_danger_fieldofview_direction = Fvector().set(-1.0f, 0.0f, 0.0f);
        animpoint_stay_table_loophole.m_enter_direction = Fvector().set(0.0f, 0.0f, -1.0f);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_stay_table_loophole_action1;
        animpoint_stay_table_loophole_action1.m_id = "idle";
        animpoint_stay_table_loophole_action1.register_animation("idle", "animpoint_stay_table_idle_1");

        animpoint_stay_table_loophole.register_action(animpoint_stay_table_loophole_action1);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_stay_table_loophole_action2;
        animpoint_stay_table_loophole_action2.m_id = "lookout";
        animpoint_stay_table_loophole_action2.register_animation("idle", "animpoint_stay_table_idle_1");

        animpoint_stay_table_loophole.register_action(animpoint_stay_table_loophole_action2);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_stay_table_loophole_action3;
        animpoint_stay_table_loophole_action3.m_id = "fire";
        animpoint_stay_table_loophole_action3.register_animation("idle", "animpoint_stay_table_idle_1");
        animpoint_stay_table_loophole_action3.register_animation("shoot", "animpoint_stay_table_idle_1");

        animpoint_stay_table_loophole.register_action(animpoint_stay_table_loophole_action3);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_stay_table_loophole_action4;
        animpoint_stay_table_loophole_action4.m_id = "fire_no_lookout";
        animpoint_stay_table_loophole_action4.register_animation("idle", "animpoint_stay_table_idle_1");
        animpoint_stay_table_loophole_action4.register_animation("shoot", "animpoint_stay_table_idle_1");

        animpoint_stay_table_loophole.register_action(animpoint_stay_table_loophole_action4);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_stay_table_loophole_action5;
        animpoint_stay_table_loophole_action5.m_id = "reload";
        animpoint_stay_table_loophole_action5.register_animation("idle", "animpoint_stay_table_idle_1");

        animpoint_stay_table_loophole.register_action(animpoint_stay_table_loophole_action5);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_stay_table_loophole_transition1;
        animpoint_stay_table_loophole_transition1.m_action_from = "idle";
        animpoint_stay_table_loophole_transition1.m_action_to = "lookout";
        animpoint_stay_table_loophole_transition1.m_animations.push_back("animpoint_stay_table_in_1");
        animpoint_stay_table_loophole_transition1.m_weight = 1.2f;

        animpoint_stay_table_loophole.register_transition(animpoint_stay_table_loophole_transition1);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_stay_table_loophole_transition2;
        animpoint_stay_table_loophole_transition2.m_action_from = "lookout";
        animpoint_stay_table_loophole_transition2.m_action_to = "idle";
        animpoint_stay_table_loophole_transition2.m_animations.push_back("animpoint_stay_table_in_1");
        animpoint_stay_table_loophole_transition2.m_weight = 1.2f;

        animpoint_stay_table_loophole.register_transition(animpoint_stay_table_loophole_transition2);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_stay_table_loophole_transition3;
        animpoint_stay_table_loophole_transition3.m_action_from = "idle";
        animpoint_stay_table_loophole_transition3.m_action_to = "fire";
        animpoint_stay_table_loophole_transition3.m_animations.push_back("animpoint_stay_table_in_1");
        animpoint_stay_table_loophole_transition3.m_weight = 1.2f;

        animpoint_stay_table_loophole.register_transition(animpoint_stay_table_loophole_transition3);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_stay_table_loophole_transition4;
        animpoint_stay_table_loophole_transition4.m_action_from = "fire";
        animpoint_stay_table_loophole_transition4.m_action_to = "idle";
        animpoint_stay_table_loophole_transition4.m_animations.push_back("animpoint_stay_table_in_1");
        animpoint_stay_table_loophole_transition4.m_weight = 1.2f;

        animpoint_stay_table_loophole.register_transition(animpoint_stay_table_loophole_transition4);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_stay_table_loophole_transition5;
        animpoint_stay_table_loophole_transition5.m_action_from = "idle";
        animpoint_stay_table_loophole_transition5.m_action_to = "fire_no_lookout";
        animpoint_stay_table_loophole_transition5.m_animations.push_back("animpoint_stay_table_in_1");
        animpoint_stay_table_loophole_transition5.m_weight = 1.2f;

        animpoint_stay_table_loophole.register_transition(animpoint_stay_table_loophole_transition5);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_stay_table_loophole_transition6;
        animpoint_stay_table_loophole_transition6.m_action_from = "fire_no_lookout";
        animpoint_stay_table_loophole_transition6.m_action_to = "idle";
        animpoint_stay_table_loophole_transition6.m_animations.push_back("animpoint_stay_table_in_1");
        animpoint_stay_table_loophole_transition6.m_weight = 1.2f;

        animpoint_stay_table_loophole.register_transition(animpoint_stay_table_loophole_transition6);
#pragma endregion

#pragma region Cordis Transition
        SmartCoverData::SmartCoverTransitionsData animpoint_stay_table_transition;
        animpoint_stay_table_transition.m_vertex0 = "";
        animpoint_stay_table_transition.m_vertex1 = "animpoint_stay_table";
        animpoint_stay_table_transition.m_weight = 1.0f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData animpoint_stay_table_transition_action1;
        animpoint_stay_table_transition_action1.m_action.m_animation = "animpoint_stay_table_in_1";
        animpoint_stay_table_transition_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_stay_table_transition_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        animpoint_stay_table_transition_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        animpoint_stay_table_transition_action1.m_precondition_functor = "true";
        animpoint_stay_table_transition_action1.m_preconditions_params = "";

        animpoint_stay_table_transition.m_actions.push_back(animpoint_stay_table_transition_action1);

        SmartCoverData::SmartCoverTransitionsData animpoint_stay_table_transition1;
        animpoint_stay_table_transition1.m_vertex0 = "animpoint_stay_table";
        animpoint_stay_table_transition1.m_vertex1 = "";
        animpoint_stay_table_transition1.m_weight = 1.1f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData animpoint_stay_table_transition1_action1;
        animpoint_stay_table_transition1_action1.m_action.m_animation = "animpoint_stay_table_out_1";
        animpoint_stay_table_transition1_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_stay_table_transition1_action1.m_action.m_body_state = MonsterSpace::eBodyStateStand;
        animpoint_stay_table_transition1_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        animpoint_stay_table_transition1_action1.m_precondition_functor = "true";
        animpoint_stay_table_transition1_action1.m_preconditions_params = "";

        animpoint_stay_table_transition1.m_actions.push_back(animpoint_stay_table_transition1_action1);
#pragma endregion

        animpoint_stay_table_smartcover.setNeedWeapon(false);
        animpoint_stay_table_smartcover.register_loophole(animpoint_stay_table_loophole);
        animpoint_stay_table_smartcover.register_transition(animpoint_stay_table_transition);
        animpoint_stay_table_smartcover.register_transition(animpoint_stay_table_transition1);
        this->m_registered_smartcovers["animpoint_stay_table"] = animpoint_stay_table_smartcover;
#pragma endregion

#pragma region Cordis Animpoint Sit High Smart Cover
        SmartCoverData animpoint_sit_high_smartcover;
        SmartCoverLoopholeData animpoint_sit_high_loophole;

#pragma region Cordis Loophole
        animpoint_sit_high_loophole.m_id = "animpoint_sit_high";
        animpoint_sit_high_loophole.m_is_usable = true;
        animpoint_sit_high_loophole.m_is_exitable = false;
        animpoint_sit_high_loophole.m_is_enterable = false;
        animpoint_sit_high_loophole.m_fieldofview = 45.0f;
        animpoint_sit_high_loophole.m_fieldofview_danger = 45.0f;
        animpoint_sit_high_loophole.m_range = 70.0f;
        animpoint_sit_high_loophole.m_fieldofview_direction = Fvector().set(0.0f, 0.0f, -1.0f);
        animpoint_sit_high_loophole.m_fieldofview_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_sit_high_loophole.m_danger_fieldofview_direction = Fvector().set(-1.0f, 0.0f, 0.0f);
        animpoint_sit_high_loophole.m_enter_direction = Fvector().set(0.0f, 0.0f, -1.0f);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_high_loophole_action1;
        animpoint_sit_high_loophole_action1.m_id = "idle";
        animpoint_sit_high_loophole_action1.register_animation("idle", "animpoint_sit_high_idle_1");

        animpoint_sit_high_loophole.register_action(animpoint_sit_high_loophole_action1);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_high_loophole_action2;
        animpoint_sit_high_loophole_action2.m_id = "lookout";
        animpoint_sit_high_loophole_action2.register_animation("idle", "animpoint_sit_high_idle_1");

        animpoint_sit_high_loophole.register_action(animpoint_sit_high_loophole_action2);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_high_loophole_action3;
        animpoint_sit_high_loophole_action3.m_id = "fire";
        animpoint_sit_high_loophole_action3.register_animation("idle", "animpoint_sit_high_idle_1");
        animpoint_sit_high_loophole_action3.register_animation("shoot", "animpoint_sit_high_idle_1");

        animpoint_sit_high_loophole.register_action(animpoint_sit_high_loophole_action3);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_high_loophole_action4;
        animpoint_sit_high_loophole_action4.m_id = "fire_no_lookout";
        animpoint_sit_high_loophole_action4.register_animation("idle", "animpoint_sit_high_idle_1");
        animpoint_sit_high_loophole_action4.register_animation("shoot", "animpoint_sit_high_idle_1");

        animpoint_sit_high_loophole.register_action(animpoint_sit_high_loophole_action4);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_high_loophole_action5;
        animpoint_sit_high_loophole_action5.m_id = "reload";
        animpoint_sit_high_loophole_action5.register_animation("idle", "animpoint_sit_high_idle_1");

        animpoint_sit_high_loophole.register_action(animpoint_sit_high_loophole_action5);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_high_loophole_transition1;
        animpoint_sit_high_loophole_transition1.m_action_from = "idle";
        animpoint_sit_high_loophole_transition1.m_action_to = "lookout";
        animpoint_sit_high_loophole_transition1.m_animations.push_back("animpoint_sit_high_in_1");
        animpoint_sit_high_loophole_transition1.m_weight = 1.2f;

        animpoint_sit_high_loophole.register_transition(animpoint_sit_high_loophole_transition1);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_high_loophole_transition2;
        animpoint_sit_high_loophole_transition2.m_action_from = "lookout";
        animpoint_sit_high_loophole_transition2.m_action_to = "idle";
        animpoint_sit_high_loophole_transition2.m_animations.push_back("animpoint_sit_high_in_1");
        animpoint_sit_high_loophole_transition2.m_weight = 1.2f;

        animpoint_sit_high_loophole.register_transition(animpoint_sit_high_loophole_transition2);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_high_loophole_transition3;
        animpoint_sit_high_loophole_transition3.m_action_from = "idle";
        animpoint_sit_high_loophole_transition3.m_action_to = "fire";
        animpoint_sit_high_loophole_transition3.m_animations.push_back("animpoint_sit_high_in_1");
        animpoint_sit_high_loophole_transition3.m_weight = 1.2f;

        animpoint_sit_high_loophole.register_transition(animpoint_sit_high_loophole_transition3);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_high_loophole_transition4;
        animpoint_sit_high_loophole_transition4.m_action_from = "fire";
        animpoint_sit_high_loophole_transition4.m_action_to = "idle";
        animpoint_sit_high_loophole_transition4.m_animations.push_back("animpoint_sit_high_in_1");
        animpoint_sit_high_loophole_transition4.m_weight = 1.2f;

        animpoint_sit_high_loophole.register_transition(animpoint_sit_high_loophole_transition4);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_high_loophole_transition5;
        animpoint_sit_high_loophole_transition5.m_action_from = "idle";
        animpoint_sit_high_loophole_transition5.m_action_to = "fire_no_lookout";
        animpoint_sit_high_loophole_transition5.m_animations.push_back("animpoint_sit_high_in_1");
        animpoint_sit_high_loophole_transition5.m_weight = 1.2f;

        animpoint_sit_high_loophole.register_transition(animpoint_sit_high_loophole_transition5);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_high_loophole_transition6;
        animpoint_sit_high_loophole_transition6.m_action_from = "fire_no_lookout";
        animpoint_sit_high_loophole_transition6.m_action_to = "idle";
        animpoint_sit_high_loophole_transition6.m_animations.push_back("animpoint_sit_high_in_1");
        animpoint_sit_high_loophole_transition6.m_weight = 1.2f;

        animpoint_sit_high_loophole.register_transition(animpoint_sit_high_loophole_transition6);
#pragma endregion

#pragma region Cordis Transition
        SmartCoverData::SmartCoverTransitionsData animpoint_sit_high_transition;
        animpoint_sit_high_transition.m_vertex0 = "";
        animpoint_sit_high_transition.m_vertex1 = "animpoint_sit_high";
        animpoint_sit_high_transition.m_weight = 1.0f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData animpoint_sit_high_transition_action1;
        animpoint_sit_high_transition_action1.m_action.m_animation = "animpoint_sit_high_in_1";
        animpoint_sit_high_transition_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_sit_high_transition_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        animpoint_sit_high_transition_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        animpoint_sit_high_transition_action1.m_precondition_functor = "true";
        animpoint_sit_high_transition_action1.m_preconditions_params = "";

        animpoint_sit_high_transition.m_actions.push_back(animpoint_sit_high_transition_action1);

        SmartCoverData::SmartCoverTransitionsData animpoint_sit_high_transition1;
        animpoint_sit_high_transition1.m_vertex0 = "animpoint_sit_high";
        animpoint_sit_high_transition1.m_vertex1 = "";
        animpoint_sit_high_transition1.m_weight = 1.1f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData animpoint_sit_high_transition1_action1;
        animpoint_sit_high_transition1_action1.m_action.m_animation = "animpoint_sit_high_out_1";
        animpoint_sit_high_transition1_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_sit_high_transition1_action1.m_action.m_body_state = MonsterSpace::eBodyStateStand;
        animpoint_sit_high_transition1_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        animpoint_sit_high_transition1_action1.m_precondition_functor = "true";
        animpoint_sit_high_transition1_action1.m_preconditions_params = "";

        animpoint_sit_high_transition1.m_actions.push_back(animpoint_sit_high_transition1_action1);
#pragma endregion

        animpoint_sit_high_smartcover.setNeedWeapon(false);
        animpoint_sit_high_smartcover.register_loophole(animpoint_sit_high_loophole);
        animpoint_sit_high_smartcover.register_transition(animpoint_sit_high_transition);
        animpoint_sit_high_smartcover.register_transition(animpoint_sit_high_transition1);
        this->m_registered_smartcovers["animpoint_sit_high"] = animpoint_sit_high_smartcover;
#pragma endregion

#pragma region Cordis Animpoint Sit Normal
        SmartCoverData animpoint_sit_normal_smartcover;
        SmartCoverLoopholeData animpoint_sit_normal_loophole;

#pragma region Cordis Loophole
        animpoint_sit_normal_loophole.m_id = "animpoint_sit_normal";
        animpoint_sit_normal_loophole.m_is_usable = true;
        animpoint_sit_normal_loophole.m_is_exitable = false;
        animpoint_sit_normal_loophole.m_is_enterable = false;
        animpoint_sit_normal_loophole.m_fieldofview = 45.0f;
        animpoint_sit_normal_loophole.m_fieldofview_danger = 45.0f;
        animpoint_sit_normal_loophole.m_range = 70.0f;
        animpoint_sit_normal_loophole.m_fieldofview_direction = Fvector().set(0.0f, 0.0f, -1.0f);
        animpoint_sit_normal_loophole.m_fieldofview_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_sit_normal_loophole.m_danger_fieldofview_direction = Fvector().set(-1.0f, 0.0f, 0.0f);
        animpoint_sit_normal_loophole.m_enter_direction = Fvector().set(0.0f, 0.0f, -1.0f);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_normal_loophole_action1;
        animpoint_sit_normal_loophole_action1.m_id = "idle";
        animpoint_sit_normal_loophole_action1.register_animation("idle", "animpoint_sit_normal_idle_1");

        animpoint_sit_normal_loophole.register_action(animpoint_sit_normal_loophole_action1);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_normal_loophole_action2;
        animpoint_sit_normal_loophole_action2.m_id = "lookout";
        animpoint_sit_normal_loophole_action2.register_animation("idle", "animpoint_sit_normal_idle_1");

        animpoint_sit_normal_loophole.register_action(animpoint_sit_normal_loophole_action2);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_normal_loophole_action3;
        animpoint_sit_normal_loophole_action3.m_id = "fire";
        animpoint_sit_normal_loophole_action3.register_animation("idle", "animpoint_sit_normal_idle_1");
        animpoint_sit_normal_loophole_action3.register_animation("shoot", "animpoint_sit_normal_idle_1");

        animpoint_sit_normal_loophole.register_action(animpoint_sit_normal_loophole_action3);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_normal_loophole_action4;
        animpoint_sit_normal_loophole_action4.m_id = "fire_no_lookout";
        animpoint_sit_normal_loophole_action4.register_animation("idle", "animpoint_sit_normal_idle_1");
        animpoint_sit_normal_loophole_action4.register_animation("shoot", "animpoint_sit_normal_idle_1");

        animpoint_sit_normal_loophole.register_action(animpoint_sit_normal_loophole_action4);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_normal_loophole_action5;
        animpoint_sit_normal_loophole_action5.m_id = "reload";
        animpoint_sit_normal_loophole_action5.register_animation("idle", "animpoint_sit_normal_idle_1");

        animpoint_sit_normal_loophole.register_action(animpoint_sit_normal_loophole_action5);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_normal_loophole_transition1;
        animpoint_sit_normal_loophole_transition1.m_action_from = "idle";
        animpoint_sit_normal_loophole_transition1.m_action_to = "lookout";
        animpoint_sit_normal_loophole_transition1.m_animations.push_back("animpoint_sit_normal_in_1");
        animpoint_sit_normal_loophole_transition1.m_weight = 1.2f;

        animpoint_sit_normal_loophole.register_transition(animpoint_sit_normal_loophole_transition1);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_normal_loophole_transition2;
        animpoint_sit_normal_loophole_transition2.m_action_from = "lookout";
        animpoint_sit_normal_loophole_transition2.m_action_to = "idle";
        animpoint_sit_normal_loophole_transition2.m_animations.push_back("animpoint_sit_normal_in_1");
        animpoint_sit_normal_loophole_transition2.m_weight = 1.2f;

        animpoint_sit_normal_loophole.register_transition(animpoint_sit_normal_loophole_transition2);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_normal_loophole_transition3;
        animpoint_sit_normal_loophole_transition3.m_action_from = "idle";
        animpoint_sit_normal_loophole_transition3.m_action_to = "fire";
        animpoint_sit_normal_loophole_transition3.m_animations.push_back("animpoint_sit_normal_in_1");
        animpoint_sit_normal_loophole_transition3.m_weight = 1.2f;

        animpoint_sit_normal_loophole.register_transition(animpoint_sit_normal_loophole_transition3);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_normal_loophole_transition4;
        animpoint_sit_normal_loophole_transition4.m_action_from = "fire";
        animpoint_sit_normal_loophole_transition4.m_action_to = "idle";
        animpoint_sit_normal_loophole_transition4.m_animations.push_back("animpoint_sit_normal_in_1");
        animpoint_sit_normal_loophole_transition4.m_weight = 1.2f;

        animpoint_sit_normal_loophole.register_transition(animpoint_sit_normal_loophole_transition4);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_normal_loophole_transition5;
        animpoint_sit_normal_loophole_transition5.m_action_from = "idle";
        animpoint_sit_normal_loophole_transition5.m_action_to = "fire_no_lookout";
        animpoint_sit_normal_loophole_transition5.m_animations.push_back("animpoint_sit_normal_in_1");
        animpoint_sit_normal_loophole_transition5.m_weight = 1.2f;

        animpoint_sit_normal_loophole.register_transition(animpoint_sit_normal_loophole_transition5);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_normal_loophole_transition6;
        animpoint_sit_normal_loophole_transition6.m_action_from = "fire_no_lookout";
        animpoint_sit_normal_loophole_transition6.m_action_to = "idle";
        animpoint_sit_normal_loophole_transition6.m_animations.push_back("animpoint_sit_normal_in_1");
        animpoint_sit_normal_loophole_transition6.m_weight = 1.2f;

        animpoint_sit_normal_loophole.register_transition(animpoint_sit_normal_loophole_transition6);
#pragma endregion

#pragma region Cordis Transition
        SmartCoverData::SmartCoverTransitionsData animpoint_sit_normal_transition;
        animpoint_sit_normal_transition.m_vertex0 = "";
        animpoint_sit_normal_transition.m_vertex1 = "animpoint_sit_normal";
        animpoint_sit_normal_transition.m_weight = 1.0f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData animpoint_sit_normal_transition_action1;
        animpoint_sit_normal_transition_action1.m_action.m_animation = "animpoint_sit_normal_in_1";
        animpoint_sit_normal_transition_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_sit_normal_transition_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        animpoint_sit_normal_transition_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        animpoint_sit_normal_transition_action1.m_precondition_functor = "true";
        animpoint_sit_normal_transition_action1.m_preconditions_params = "";

        animpoint_sit_normal_transition.m_actions.push_back(animpoint_sit_normal_transition_action1);

        SmartCoverData::SmartCoverTransitionsData animpoint_sit_normal_transition1;
        animpoint_sit_normal_transition1.m_vertex0 = "animpoint_sit_normal";
        animpoint_sit_normal_transition1.m_vertex1 = "";
        animpoint_sit_normal_transition1.m_weight = 1.1f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData animpoint_sit_normal_transition1_action1;
        animpoint_sit_normal_transition1_action1.m_action.m_animation = "animpoint_sit_normal_out_1";
        animpoint_sit_normal_transition1_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_sit_normal_transition1_action1.m_action.m_body_state = MonsterSpace::eBodyStateStand;
        animpoint_sit_normal_transition1_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        animpoint_sit_normal_transition1_action1.m_precondition_functor = "true";
        animpoint_sit_normal_transition1_action1.m_preconditions_params = "";

        animpoint_sit_normal_transition1.m_actions.push_back(animpoint_sit_normal_transition1_action1);
#pragma endregion

        animpoint_sit_normal_smartcover.setNeedWeapon(false);
        animpoint_sit_normal_smartcover.register_loophole(animpoint_sit_normal_loophole);
        animpoint_sit_normal_smartcover.register_transition(animpoint_sit_normal_transition);
        animpoint_sit_normal_smartcover.register_transition(animpoint_sit_normal_transition1);
        this->m_registered_smartcovers["animpoint_sit_normal"] = animpoint_sit_normal_smartcover;
#pragma endregion

#pragma region Cordis Animpoint Sit Low
        SmartCoverData animpoint_sit_low_smartcover;
        SmartCoverLoopholeData animpoint_sit_low_loophole;

#pragma region Cordis Loophole
        animpoint_sit_low_loophole.m_id = "animpoint_sit_low";
        animpoint_sit_low_loophole.m_is_usable = true;
        animpoint_sit_low_loophole.m_is_exitable = false;
        animpoint_sit_low_loophole.m_is_enterable = false;
        animpoint_sit_low_loophole.m_fieldofview = 45.0f;
        animpoint_sit_low_loophole.m_fieldofview_danger = 45.0f;
        animpoint_sit_low_loophole.m_range = 70.0f;
        animpoint_sit_low_loophole.m_fieldofview_direction = Fvector().set(0.0f, 0.0f, -1.0f);
        animpoint_sit_low_loophole.m_fieldofview_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_sit_low_loophole.m_danger_fieldofview_direction = Fvector().set(-1.0f, 0.0f, 0.0f);
        animpoint_sit_low_loophole.m_enter_direction = Fvector().set(0.0f, 0.0f, -1.0f);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_low_loophole_action1;
        animpoint_sit_low_loophole_action1.m_id = "idle";
        animpoint_sit_low_loophole_action1.register_animation("idle", "animpoint_sit_low_idle_1");

        animpoint_sit_low_loophole.register_action(animpoint_sit_low_loophole_action1);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_low_loophole_action2;
        animpoint_sit_low_loophole_action2.m_id = "lookout";
        animpoint_sit_low_loophole_action2.register_animation("idle", "animpoint_sit_low_in_1");

        animpoint_sit_low_loophole.register_action(animpoint_sit_low_loophole_action2);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_low_loophole_action3;
        animpoint_sit_low_loophole_action3.m_id = "fire";
        animpoint_sit_low_loophole_action3.register_animation("idle", "animpoint_sit_low_in_1");
        animpoint_sit_low_loophole_action3.register_animation("shoot", "animpoint_sit_low_in_1");

        animpoint_sit_low_loophole.register_action(animpoint_sit_low_loophole_action3);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_low_loophole_action4;
        animpoint_sit_low_loophole_action4.m_id = "fire_no_lookout";
        animpoint_sit_low_loophole_action4.register_animation("idle", "animpoint_sit_low_in_1");
        animpoint_sit_low_loophole_action4.register_animation("shoot", "animpoint_sit_low_in_1");

        animpoint_sit_low_loophole.register_action(animpoint_sit_low_loophole_action4);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_sit_low_loophole_action5;
        animpoint_sit_low_loophole_action5.m_id = "reload";
        animpoint_sit_low_loophole_action5.register_animation("idle", "animpoint_sit_low_in_1");

        animpoint_sit_low_loophole.register_action(animpoint_sit_low_loophole_action5);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_low_loophole_transition1;
        animpoint_sit_low_loophole_transition1.m_action_from = "idle";
        animpoint_sit_low_loophole_transition1.m_action_to = "lookout";
        animpoint_sit_low_loophole_transition1.m_animations.push_back("animpoint_sit_low_in_1");
        animpoint_sit_low_loophole_transition1.m_weight = 1.2f;

        animpoint_sit_low_loophole.register_transition(animpoint_sit_low_loophole_transition1);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_low_loophole_transition2;
        animpoint_sit_low_loophole_transition2.m_action_from = "lookout";
        animpoint_sit_low_loophole_transition2.m_action_to = "idle";
        animpoint_sit_low_loophole_transition2.m_animations.push_back("animpoint_sit_low_in_1");
        animpoint_sit_low_loophole_transition2.m_weight = 1.2f;

        animpoint_sit_low_loophole.register_transition(animpoint_sit_low_loophole_transition2);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_low_loophole_transition3;
        animpoint_sit_low_loophole_transition3.m_action_from = "idle";
        animpoint_sit_low_loophole_transition3.m_action_to = "fire";
        animpoint_sit_low_loophole_transition3.m_animations.push_back("animpoint_sit_low_in_1");
        animpoint_sit_low_loophole_transition3.m_weight = 1.2f;

        animpoint_sit_low_loophole.register_transition(animpoint_sit_low_loophole_transition3);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_low_loophole_transition4;
        animpoint_sit_low_loophole_transition4.m_action_from = "fire";
        animpoint_sit_low_loophole_transition4.m_action_to = "idle";
        animpoint_sit_low_loophole_transition4.m_animations.push_back("animpoint_sit_low_in_1");
        animpoint_sit_low_loophole_transition4.m_weight = 1.2f;

        animpoint_sit_low_loophole.register_transition(animpoint_sit_low_loophole_transition4);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_low_loophole_transition5;
        animpoint_sit_low_loophole_transition5.m_action_from = "idle";
        animpoint_sit_low_loophole_transition5.m_action_to = "fire_no_lookout";
        animpoint_sit_low_loophole_transition5.m_animations.push_back("animpoint_sit_low_in_1");
        animpoint_sit_low_loophole_transition5.m_weight = 1.2f;

        animpoint_sit_low_loophole.register_transition(animpoint_sit_low_loophole_transition5);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_sit_low_loophole_transition6;
        animpoint_sit_low_loophole_transition6.m_action_from = "fire_no_lookout";
        animpoint_sit_low_loophole_transition6.m_action_to = "idle";
        animpoint_sit_low_loophole_transition6.m_animations.push_back("animpoint_sit_low_in_1");
        animpoint_sit_low_loophole_transition6.m_weight = 1.2f;

        animpoint_sit_low_loophole.register_transition(animpoint_sit_low_loophole_transition6);
#pragma endregion

#pragma region Cordis Transition
        SmartCoverData::SmartCoverTransitionsData animpoint_sit_low_transition;
        animpoint_sit_low_transition.m_vertex0 = "";
        animpoint_sit_low_transition.m_vertex1 = "animpoint_sit_low";
        animpoint_sit_low_transition.m_weight = 1.0f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData animpoint_sit_low_transition_action1;
        animpoint_sit_low_transition_action1.m_action.m_animation = "animpoint_sit_low_in_1";
        animpoint_sit_low_transition_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_sit_low_transition_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        animpoint_sit_low_transition_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        animpoint_sit_low_transition_action1.m_precondition_functor = "true";
        animpoint_sit_low_transition_action1.m_preconditions_params = "";

        animpoint_sit_low_transition.m_actions.push_back(animpoint_sit_low_transition_action1);

        SmartCoverData::SmartCoverTransitionsData animpoint_sit_low_transition1;
        animpoint_sit_low_transition1.m_vertex0 = "animpoint_sit_low";
        animpoint_sit_low_transition1.m_vertex1 = "";
        animpoint_sit_low_transition1.m_weight = 1.1f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData animpoint_sit_low_transition1_action1;
        animpoint_sit_low_transition1_action1.m_action.m_animation = "animpoint_sit_low_out_1";
        animpoint_sit_low_transition1_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_sit_low_transition1_action1.m_action.m_body_state = MonsterSpace::eBodyStateStand;
        animpoint_sit_low_transition1_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        animpoint_sit_low_transition1_action1.m_precondition_functor = "true";
        animpoint_sit_low_transition1_action1.m_preconditions_params = "";

        animpoint_sit_low_transition1.m_actions.push_back(animpoint_sit_low_transition1_action1);
#pragma endregion

        animpoint_sit_low_smartcover.setNeedWeapon(false);
        animpoint_sit_low_smartcover.register_loophole(animpoint_sit_low_loophole);
        animpoint_sit_low_smartcover.register_transition(animpoint_sit_low_transition);
        animpoint_sit_low_smartcover.register_transition(animpoint_sit_low_transition1);
        this->m_registered_smartcovers["animpoint_sit_low"] = animpoint_sit_low_smartcover;
#pragma endregion

#pragma region Cordis Animpoint Pri A15
        SmartCoverData animpoint_pri_a15_smartcover;
        SmartCoverLoopholeData animpoint_pri_a15_loophole;

#pragma region Cordis Loophole
        animpoint_pri_a15_loophole.m_id = "animpoint_pri_a15";
        animpoint_pri_a15_loophole.m_is_usable = true;
        animpoint_pri_a15_loophole.m_is_exitable = false;
        animpoint_pri_a15_loophole.m_is_enterable = false;
        animpoint_pri_a15_loophole.m_fieldofview = 45.0f;
        animpoint_pri_a15_loophole.m_fieldofview_danger = 45.0f;
        animpoint_pri_a15_loophole.m_range = 70.0f;
        animpoint_pri_a15_loophole.m_fieldofview_direction = Fvector().set(0.0f, 0.0f, -1.0f);
        animpoint_pri_a15_loophole.m_fieldofview_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_pri_a15_loophole.m_danger_fieldofview_direction = Fvector().set(-1.0f, 0.0f, 0.0f);
        animpoint_pri_a15_loophole.m_enter_direction = Fvector().set(0.0f, 0.0f, -1.0f);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_pri_a15_loophole_action1;
        animpoint_pri_a15_loophole_action1.m_id = "idle";
        animpoint_pri_a15_loophole_action1.register_animation("idle", "idle_0_idle_0");

        animpoint_pri_a15_loophole.register_action(animpoint_pri_a15_loophole_action1);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_pri_a15_loophole_action2;
        animpoint_pri_a15_loophole_action2.m_id = "lookout";
        animpoint_pri_a15_loophole_action2.register_animation("idle", "idle_0_idle_0");

        animpoint_pri_a15_loophole.register_action(animpoint_pri_a15_loophole_action2);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_pri_a15_loophole_action3;
        animpoint_pri_a15_loophole_action3.m_id = "fire";
        animpoint_pri_a15_loophole_action3.register_animation("idle", "idle_0_idle_0");
        animpoint_pri_a15_loophole_action3.register_animation("shoot", "idle_0_idle_0");

        animpoint_pri_a15_loophole.register_action(animpoint_pri_a15_loophole_action3);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_pri_a15_loophole_action4;
        animpoint_pri_a15_loophole_action4.m_id = "fire_no_lookout";
        animpoint_pri_a15_loophole_action4.register_animation("idle", "idle_0_idle_0");
        animpoint_pri_a15_loophole_action4.register_animation("shoot", "idle_0_idle_0");

        animpoint_pri_a15_loophole.register_action(animpoint_pri_a15_loophole_action4);

        SmartCoverLoopholeData::SmartCoverActionsData animpoint_pri_a15_loophole_action5;
        animpoint_pri_a15_loophole_action5.m_id = "reload";
        animpoint_pri_a15_loophole_action5.register_animation("idle", "idle_0_idle_0");

        animpoint_pri_a15_loophole.register_action(animpoint_pri_a15_loophole_action5);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_pri_a15_loophole_transition1;
        animpoint_pri_a15_loophole_transition1.m_action_from = "idle";
        animpoint_pri_a15_loophole_transition1.m_action_to = "lookout";
        animpoint_pri_a15_loophole_transition1.m_animations.push_back("idle_0_idle_0");
        animpoint_pri_a15_loophole_transition1.m_weight = 1.2f;

        animpoint_pri_a15_loophole.register_transition(animpoint_pri_a15_loophole_transition1);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_pri_a15_loophole_transition2;
        animpoint_pri_a15_loophole_transition2.m_action_from = "lookout";
        animpoint_pri_a15_loophole_transition2.m_action_to = "idle";
        animpoint_pri_a15_loophole_transition2.m_animations.push_back("idle_0_idle_0");
        animpoint_pri_a15_loophole_transition2.m_weight = 1.2f;

        animpoint_pri_a15_loophole.register_transition(animpoint_pri_a15_loophole_transition2);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_pri_a15_loophole_transition3;
        animpoint_pri_a15_loophole_transition3.m_action_from = "idle";
        animpoint_pri_a15_loophole_transition3.m_action_to = "fire";
        animpoint_pri_a15_loophole_transition3.m_animations.push_back("idle_0_idle_0");
        animpoint_pri_a15_loophole_transition3.m_weight = 1.2f;

        animpoint_pri_a15_loophole.register_transition(animpoint_pri_a15_loophole_transition3);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_pri_a15_loophole_transition4;
        animpoint_pri_a15_loophole_transition4.m_action_from = "fire";
        animpoint_pri_a15_loophole_transition4.m_action_to = "idle";
        animpoint_pri_a15_loophole_transition4.m_animations.push_back("idle_0_idle_0");
        animpoint_pri_a15_loophole_transition4.m_weight = 1.2f;

        animpoint_pri_a15_loophole.register_transition(animpoint_pri_a15_loophole_transition4);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_pri_a15_loophole_transition5;
        animpoint_pri_a15_loophole_transition5.m_action_from = "idle";
        animpoint_pri_a15_loophole_transition5.m_action_to = "fire_no_lookout";
        animpoint_pri_a15_loophole_transition5.m_animations.push_back("idle_0_idle_0");
        animpoint_pri_a15_loophole_transition5.m_weight = 1.2f;

        animpoint_pri_a15_loophole.register_transition(animpoint_pri_a15_loophole_transition5);

        SmartCoverLoopholeData::SmartCoverTransitionsData animpoint_pri_a15_loophole_transition6;
        animpoint_pri_a15_loophole_transition6.m_action_from = "fire_no_lookout";
        animpoint_pri_a15_loophole_transition6.m_action_to = "idle";
        animpoint_pri_a15_loophole_transition6.m_animations.push_back("idle_0_idle_0");
        animpoint_pri_a15_loophole_transition6.m_weight = 1.2f;

        animpoint_pri_a15_loophole.register_transition(animpoint_pri_a15_loophole_transition6);
#pragma endregion

#pragma region Cordis Transition
        SmartCoverData::SmartCoverTransitionsData animpoint_pri_a15_transition;
        animpoint_pri_a15_transition.m_vertex0 = "";
        animpoint_pri_a15_transition.m_vertex1 = "animpoint_pri_a15";
        animpoint_pri_a15_transition.m_weight = 1.0f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData animpoint_pri_a15_transition_action1;
        animpoint_pri_a15_transition_action1.m_action.m_animation = "idle_0_idle_0";
        animpoint_pri_a15_transition_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_pri_a15_transition_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        animpoint_pri_a15_transition_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        animpoint_pri_a15_transition_action1.m_precondition_functor = "true";
        animpoint_pri_a15_transition_action1.m_preconditions_params = "";

        animpoint_pri_a15_transition.m_actions.push_back(animpoint_pri_a15_transition_action1);

        SmartCoverData::SmartCoverTransitionsData animpoint_pri_a15_transition1;
        animpoint_pri_a15_transition1.m_vertex0 = "animpoint_pri_a15";
        animpoint_pri_a15_transition1.m_vertex1 = "";
        animpoint_pri_a15_transition1.m_weight = 1.1f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData animpoint_pri_a15_transition1_action1;
        animpoint_pri_a15_transition1_action1.m_action.m_animation = "idle_0_idle_0";
        animpoint_pri_a15_transition1_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        animpoint_pri_a15_transition1_action1.m_action.m_body_state = MonsterSpace::eBodyStateStand;
        animpoint_pri_a15_transition1_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        animpoint_pri_a15_transition1_action1.m_precondition_functor = "true";
        animpoint_pri_a15_transition1_action1.m_preconditions_params = "";

        animpoint_pri_a15_transition1.m_actions.push_back(animpoint_pri_a15_transition1_action1);
#pragma endregion

        animpoint_pri_a15_smartcover.setNeedWeapon(false);
        animpoint_pri_a15_smartcover.register_loophole(animpoint_pri_a15_loophole);
        animpoint_pri_a15_smartcover.register_transition(animpoint_pri_a15_transition);
        animpoint_pri_a15_smartcover.register_transition(animpoint_pri_a15_transition1);
        this->m_registered_smartcovers["animpoint_pri_a15"] = animpoint_pri_a15_smartcover;
#pragma endregion

#pragma region Cordis Anim Pri a15
        SmartCoverData anim_pri_a15_smartcover;
        SmartCoverLoopholeData anim_pri_a15_loophole;

#pragma region Cordis Loophole
        anim_pri_a15_loophole.m_id = "anim_pri_a15";
        anim_pri_a15_loophole.m_is_usable = true;
        anim_pri_a15_loophole.m_is_exitable = false;
        anim_pri_a15_loophole.m_is_enterable = false;
        anim_pri_a15_loophole.m_fieldofview = 45.0f;
        anim_pri_a15_loophole.m_fieldofview_danger = 45.0f;
        anim_pri_a15_loophole.m_range = 70.0f;
        anim_pri_a15_loophole.m_fieldofview_direction = Fvector().set(0.0f, 0.0f, -1.0f);
        anim_pri_a15_loophole.m_fieldofview_position = Fvector().set(0.0f, 0.0f, 0.0f);
        anim_pri_a15_loophole.m_danger_fieldofview_direction = Fvector().set(-1.0f, 0.0f, 0.0f);
        anim_pri_a15_loophole.m_enter_direction = Fvector().set(0.0f, 0.0f, -1.0f);

        SmartCoverLoopholeData::SmartCoverActionsData anim_pri_a15_loophole_action1;
        anim_pri_a15_loophole_action1.m_id = "idle";
        anim_pri_a15_loophole_action1.register_animation("idle", "pri_a22_colonel_lean_on_tabl_in");

        anim_pri_a15_loophole.register_action(anim_pri_a15_loophole_action1);

        SmartCoverLoopholeData::SmartCoverActionsData anim_pri_a15_loophole_action2;
        anim_pri_a15_loophole_action2.m_id = "lookout";
        anim_pri_a15_loophole_action2.register_animation("idle", "pri_a22_colonel_lean_on_tabl_in");

        anim_pri_a15_loophole.register_action(anim_pri_a15_loophole_action2);

        SmartCoverLoopholeData::SmartCoverActionsData anim_pri_a15_loophole_action3;
        anim_pri_a15_loophole_action3.m_id = "fire";
        anim_pri_a15_loophole_action3.register_animation("idle", "pri_a22_colonel_lean_on_tabl_in");
        anim_pri_a15_loophole_action3.register_animation("shoot", "pri_a22_colonel_lean_on_tabl_in");

        anim_pri_a15_loophole.register_action(anim_pri_a15_loophole_action3);

        SmartCoverLoopholeData::SmartCoverActionsData anim_pri_a15_loophole_action4;
        anim_pri_a15_loophole_action4.m_id = "fire_no_lookout";
        anim_pri_a15_loophole_action4.register_animation("idle", "pri_a22_colonel_lean_on_tabl_in");
        anim_pri_a15_loophole_action4.register_animation("shoot", "pri_a22_colonel_lean_on_tabl_in");

        anim_pri_a15_loophole.register_action(anim_pri_a15_loophole_action4);

        SmartCoverLoopholeData::SmartCoverActionsData anim_pri_a15_loophole_action5;
        anim_pri_a15_loophole_action5.m_id = "reload";
        anim_pri_a15_loophole_action5.register_animation("idle", "pri_a22_colonel_lean_on_tabl_in");

        anim_pri_a15_loophole.register_action(anim_pri_a15_loophole_action5);

        SmartCoverLoopholeData::SmartCoverTransitionsData anim_pri_a15_loophole_transition1;
        anim_pri_a15_loophole_transition1.m_action_from = "idle";
        anim_pri_a15_loophole_transition1.m_action_to = "lookout";
        anim_pri_a15_loophole_transition1.m_animations.push_back("pri_a22_colonel_lean_on_tabl_in");
        anim_pri_a15_loophole_transition1.m_weight = 1.2f;

        anim_pri_a15_loophole.register_transition(anim_pri_a15_loophole_transition1);

        SmartCoverLoopholeData::SmartCoverTransitionsData anim_pri_a15_loophole_transition2;
        anim_pri_a15_loophole_transition2.m_action_from = "lookout";
        anim_pri_a15_loophole_transition2.m_action_to = "idle";
        anim_pri_a15_loophole_transition2.m_animations.push_back("pri_a22_colonel_lean_on_tabl_in");
        anim_pri_a15_loophole_transition2.m_weight = 1.2f;

        anim_pri_a15_loophole.register_transition(anim_pri_a15_loophole_transition2);

        SmartCoverLoopholeData::SmartCoverTransitionsData anim_pri_a15_loophole_transition3;
        anim_pri_a15_loophole_transition3.m_action_from = "idle";
        anim_pri_a15_loophole_transition3.m_action_to = "fire";
        anim_pri_a15_loophole_transition3.m_animations.push_back("pri_a22_colonel_lean_on_tabl_in");
        anim_pri_a15_loophole_transition3.m_weight = 1.2f;

        anim_pri_a15_loophole.register_transition(anim_pri_a15_loophole_transition3);

        SmartCoverLoopholeData::SmartCoverTransitionsData anim_pri_a15_loophole_transition4;
        anim_pri_a15_loophole_transition4.m_action_from = "fire";
        anim_pri_a15_loophole_transition4.m_action_to = "idle";
        anim_pri_a15_loophole_transition4.m_animations.push_back("pri_a22_colonel_lean_on_tabl_in");
        anim_pri_a15_loophole_transition4.m_weight = 1.2f;

        anim_pri_a15_loophole.register_transition(anim_pri_a15_loophole_transition4);

        SmartCoverLoopholeData::SmartCoverTransitionsData anim_pri_a15_loophole_transition5;
        anim_pri_a15_loophole_transition5.m_action_from = "idle";
        anim_pri_a15_loophole_transition5.m_action_to = "fire_no_lookout";
        anim_pri_a15_loophole_transition5.m_animations.push_back("pri_a22_colonel_lean_on_tabl_in");
        anim_pri_a15_loophole_transition5.m_weight = 1.2f;

        anim_pri_a15_loophole.register_transition(anim_pri_a15_loophole_transition5);

        SmartCoverLoopholeData::SmartCoverTransitionsData anim_pri_a15_loophole_transition6;
        anim_pri_a15_loophole_transition6.m_action_from = "fire_no_lookout";
        anim_pri_a15_loophole_transition6.m_action_to = "idle";
        anim_pri_a15_loophole_transition6.m_animations.push_back("pri_a22_colonel_lean_on_tabl_in");
        anim_pri_a15_loophole_transition6.m_weight = 1.2f;

        anim_pri_a15_loophole.register_transition(anim_pri_a15_loophole_transition6);
#pragma endregion

#pragma region Cordis Transition
        SmartCoverData::SmartCoverTransitionsData anim_pri_a15_transition;
        anim_pri_a15_transition.m_vertex0 = "";
        anim_pri_a15_transition.m_vertex1 = "anim_pri_a15";
        anim_pri_a15_transition.m_weight = 1.0f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData anim_pri_a15_transition_action1;
        anim_pri_a15_transition_action1.m_action.m_animation = "pri_a22_colonel_lean_on_tabl_in";
        anim_pri_a15_transition_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        anim_pri_a15_transition_action1.m_action.m_body_state = MonsterSpace::eBodyStateCrouch;
        anim_pri_a15_transition_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        anim_pri_a15_transition_action1.m_precondition_functor = "true";
        anim_pri_a15_transition_action1.m_preconditions_params = "";

        anim_pri_a15_transition.m_actions.push_back(anim_pri_a15_transition_action1);

        SmartCoverData::SmartCoverTransitionsData anim_pri_a15_transition1;
        anim_pri_a15_transition1.m_vertex0 = "anim_pri_a15";
        anim_pri_a15_transition1.m_vertex1 = "";
        anim_pri_a15_transition1.m_weight = 1.1f;

        SmartCoverData::SmartCoverTransitionsData::SmartCoverActionsData anim_pri_a15_transition1_action1;
        anim_pri_a15_transition1_action1.m_action.m_animation = "pri_a22_colonel_lean_on_tabl_in";
        anim_pri_a15_transition1_action1.m_action.m_position = Fvector().set(0.0f, 0.0f, 0.0f);
        anim_pri_a15_transition1_action1.m_action.m_body_state = MonsterSpace::eBodyStateStand;
        anim_pri_a15_transition1_action1.m_action.m_movement_type = MonsterSpace::eMovementTypeRun;
        anim_pri_a15_transition1_action1.m_precondition_functor = "true";
        anim_pri_a15_transition1_action1.m_preconditions_params = "";

        anim_pri_a15_transition1.m_actions.push_back(anim_pri_a15_transition1_action1);
#pragma endregion

        anim_pri_a15_smartcover.setNeedWeapon(false);
        anim_pri_a15_smartcover.register_loophole(anim_pri_a15_loophole);
        anim_pri_a15_smartcover.register_transition(anim_pri_a15_transition);
        anim_pri_a15_smartcover.register_transition(anim_pri_a15_transition1);
        this->m_registered_smartcovers["anim_pri_a15"] = anim_pri_a15_smartcover;
#pragma endregion

#pragma endregion

#pragma region Cordis Animpoint System Table initialization
        // @ Eatable
        this->setRegisteredEatableVisuals("stalker_hero_1", true);
        this->setRegisteredEatableVisuals("stalker_hero_novice_1", true);
        this->setRegisteredEatableVisuals("stalker_hero_stalker_1", true);
        this->setRegisteredEatableVisuals("stalker_hero_dolg_1", true);
        this->setRegisteredEatableVisuals("stalker_hero_dolg_2", true);
        this->setRegisteredEatableVisuals("stalker_hero_freedom_1", true);
        this->setRegisteredEatableVisuals("stalker_hero_freedom_2", true);
        this->setRegisteredEatableVisuals("stalker_hero_specops", true);
        this->setRegisteredEatableVisuals("stalker_hero_military", true);
        this->setRegisteredEatableVisuals("stalker_hero_neutral_nauchniy", true);
        this->setRegisteredEatableVisuals("stalker_hero_cs_heavy", true);
        this->setRegisteredEatableVisuals("stalker_hero_exo", true);
        this->setRegisteredEatableVisuals("stalker_bandit_3", true);
        this->setRegisteredEatableVisuals("stalker_bandit_3_face_1", true);
        this->setRegisteredEatableVisuals("stalker_bandit_3_mask", true);
        this->setRegisteredEatableVisuals("stalker_bandit_4", true);
        this->setRegisteredEatableVisuals("stalker_dolg_2_face_1", true);
        this->setRegisteredEatableVisuals("stalker_dolg_1_face_1", true);
        this->setRegisteredEatableVisuals("stalker_dolg_3_face_1", true);
        this->setRegisteredEatableVisuals("stalker_freedom_1_face_1", true);
        this->setRegisteredEatableVisuals("stalker_freedom_2_face_1", true);
        this->setRegisteredEatableVisuals("stalker_freedom_2_face_2", true);
        this->setRegisteredEatableVisuals("stalker_freedom_3", true);
        this->setRegisteredEatableVisuals("stalker_freedom_3_face_1", true);
        this->setRegisteredEatableVisuals("stalker_monolith_1_face_1", true);
        this->setRegisteredEatableVisuals("stalker_nebo_2_face_1", true);
        this->setRegisteredEatableVisuals("stalker_neutral_1_face_1", true);
        this->setRegisteredEatableVisuals("stalker_neutral_1_face_2", true);
        this->setRegisteredEatableVisuals("stalker_neutral_1_face_3", true);
        this->setRegisteredEatableVisuals("stalker_bandit_3_face_3", true);
        this->setRegisteredEatableVisuals("stalker_neutral_2_face_1", true);
        this->setRegisteredEatableVisuals("stalker_neutral_2_face_2", true);
        this->setRegisteredEatableVisuals("stalker_neutral_2_face_3", true);
        this->setRegisteredEatableVisuals("stalker_neutral_2_face_4", true);
        this->setRegisteredEatableVisuals("stalker_neutral_2_face_5", true);
        this->setRegisteredEatableVisuals("stalker_neutral_2_face_6", true);
        this->setRegisteredEatableVisuals("stalker_neutral_2_face_7", true);
        this->setRegisteredEatableVisuals("stalker_bandit_3_face_2", true);
        this->setRegisteredEatableVisuals("stalker_neutral_3_face_1", true);
        this->setRegisteredEatableVisuals("stalker_neutral_nauchniy_face_1", true);
        this->setRegisteredEatableVisuals("stalker_neutral_nauchniy_face_3", true);
        this->setRegisteredEatableVisuals("stalker_soldier_1", true);
        this->setRegisteredEatableVisuals("stalker_soldier_1_face_1", true);
        this->setRegisteredEatableVisuals("stalker_solider_2", true);
        this->setRegisteredEatableVisuals("stalker_solider_2_face_1", true);
        this->setRegisteredEatableVisuals("stalker_solider_3_face_1", true);
        this->setRegisteredEatableVisuals("stalker_solider_ecolog_face_1", true);
        this->setRegisteredEatableVisuals("stalker_ucheniy_1_face_1", true);
        this->setRegisteredEatableVisuals("stalker_ucheniy_1_face_2", true);
        this->setRegisteredEatableVisuals("stalker_zombied_1", true);
        this->setRegisteredEatableVisuals("stalker_zombied_3", true);
        this->setRegisteredEatableVisuals("stalker_neutral_nauchniy_face_2", true);

        // @ Harmonica
        this->setRegisteredHarmonicaVisuals("stalker_hero_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_hero_novice_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_hero_stalker_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_hero_dolg_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_hero_dolg_2", true);
        this->setRegisteredHarmonicaVisuals("stalker_hero_freedom_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_hero_freedom_2", true);
        this->setRegisteredHarmonicaVisuals("stalker_hero_specops", true);
        this->setRegisteredHarmonicaVisuals("stalker_hero_military", true);
        this->setRegisteredHarmonicaVisuals("stalker_hero_neutral_nauchniy", true);
        this->setRegisteredHarmonicaVisuals("stalker_hero_cs_heavy", true);
        this->setRegisteredHarmonicaVisuals("stalker_hero_exo", true);
        this->setRegisteredHarmonicaVisuals("stalker_bandit_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_bandit_2", true);
        this->setRegisteredHarmonicaVisuals("stalker_bandit_3", true);
        this->setRegisteredHarmonicaVisuals("stalker_bandit_3_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_bandit_3_mask", true);
        this->setRegisteredHarmonicaVisuals("stalker_bandit_4", true);
        this->setRegisteredHarmonicaVisuals("stalker_dolg_2_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_dolg_1_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_dolg_2_mask", true);
        this->setRegisteredHarmonicaVisuals("stalker_dolg_3_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_freedom_1_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_freedom_2_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_freedom_2_face_2", true);
        this->setRegisteredHarmonicaVisuals("stalker_freedom_2_mask", true);
        this->setRegisteredHarmonicaVisuals("stalker_freedom_3", true);
        this->setRegisteredHarmonicaVisuals("stalker_freedom_3_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_monolith_1_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_nebo_2_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_1_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_1_face_2", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_1_face_3", true);
        this->setRegisteredHarmonicaVisuals("stalker_bandit_3_face_3", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_2_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_2_face_2", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_2_face_3", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_2_face_4", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_2_face_5", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_2_face_6", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_2_face_7", true);
        this->setRegisteredHarmonicaVisuals("stalker_bandit_3_face_2", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_2_mask", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_3_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_nauchniy_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_nauchniy_face_3", true);
        this->setRegisteredHarmonicaVisuals("stalker_soldier_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_soldier_1_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_solider_2", true);
        this->setRegisteredHarmonicaVisuals("stalker_solider_2_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_solider_3_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_solider_ecolog_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_ucheniy_1_face_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_ucheniy_1_face_2", true);
        this->setRegisteredHarmonicaVisuals("stalker_zombied_1", true);
        this->setRegisteredHarmonicaVisuals("stalker_zombied_2", true);
        this->setRegisteredHarmonicaVisuals("stalker_zombied_3", true);
        this->setRegisteredHarmonicaVisuals("stalker_zombied_4", true);
        this->setRegisteredHarmonicaVisuals("stalker_neutral_nauchniy_face_2", true);

        this->m_animpoint_table["animpoint_stay_wall"].push_back(
            {Globals::predicate_const_true, "animpoint_stay_wall"});
        this->m_animpoint_table["animpoint_stay_wall"].push_back(
            {Globals::predicate_animpoint_bread, "predicate_animpoint_bread"});
        this->m_animpoint_table["animpoint_stay_wall"].push_back(
            {Globals::predicate_animpoint_kolbasa, "animpoint_stay_wall_eat_kolbasa"});
        this->m_animpoint_table["animpoint_stay_wall"].push_back(
            {Globals::predicate_animpoint_vodka, "animpoint_stay_wall_drink_vodka"});
        this->m_animpoint_table["animpoint_stay_wall"].push_back(
            {Globals::predicate_animpoint_energy, "animpoint_stay_wall_drink_energy"});
        this->m_animpoint_table["animpoint_stay_wall"].push_back(
            {Globals::predicate_animpoint_weapon, "animpoint_stay_wall_weapon"});

        this->m_animpoint_table["animpoint_stay_table"].push_back(
            {Globals::predicate_const_true, "animpoint_stay_table"});
        this->m_animpoint_table["animpoint_stay_table"].push_back(
            {Globals::predicate_animpoint_bread, "animpoint_stay_table_eat_bread"});
        this->m_animpoint_table["animpoint_stay_table"].push_back(
            {Globals::predicate_animpoint_kolbasa, "animpoint_stay_table_eat_kolbasa"});
        this->m_animpoint_table["animpoint_stay_table"].push_back(
            {Globals::predicate_animpoint_vodka, "animpoint_stay_table_drink_vodka"});
        this->m_animpoint_table["animpoint_stay_table"].push_back(
            {Globals::predicate_animpoint_energy, "animpoint_stay_table_drink_energy"});
        this->m_animpoint_table["animpoint_stay_table"].push_back(
            {Globals::predicate_animpoint_weapon, "animpoint_stay_table_weapon"});

        this->m_animpoint_table["animpoint_sit_high"].push_back({Globals::predicate_const_true, "animpoint_sit_high"});
        this->m_animpoint_table["animpoint_sit_high"].push_back(
            {Globals::predicate_animpoint_bread, "animpoint_sit_high_eat_bread"});
        this->m_animpoint_table["animpoint_sit_high"].push_back(
            {Globals::predicate_animpoint_kolbasa, "animpoint_sit_high_eat_kolbasa"});
        this->m_animpoint_table["animpoint_sit_high"].push_back(
            {Globals::predicate_animpoint_vodka, "animpoint_sit_high_drink_vodka"});
        this->m_animpoint_table["animpoint_sit_high"].push_back(
            {Globals::predicate_animpoint_energy, "animpoint_sit_high_drink_energy"});
        this->m_animpoint_table["animpoint_sit_high"].push_back(
            {Globals::predicate_animpoint_harmonica, "animpoint_sit_high_harmonica"});

        this->m_animpoint_table["animpoint_sit_normal"].push_back(
            {Globals::predicate_const_true, "animpoint_sit_normal"});
        this->m_animpoint_table["animpoint_sit_normal"].push_back(
            {Globals::predicate_animpoint_bread, "animpoint_sit_normal_eat_bread"});
        this->m_animpoint_table["animpoint_sit_normal"].push_back(
            {Globals::predicate_animpoint_kolbasa, "animpoint_sit_normal_eat_kolbasa"});
        this->m_animpoint_table["animpoint_sit_normal"].push_back(
            {Globals::predicate_animpoint_vodka, "animpoint_sit_normal_drink_vodka"});
        this->m_animpoint_table["animpoint_sit_normal"].push_back(
            {Globals::predicate_animpoint_energy, "animpoint_sit_normal_drink_energy"});
        this->m_animpoint_table["animpoint_sit_normal"].push_back(
            {Globals::predicate_animpoint_guitar, "animpoint_sit_normal_guitar"});

        this->m_animpoint_table["animpoint_sit_low"].push_back({Globals::predicate_const_true, "animpoint_sit_low"});
        this->m_animpoint_table["animpoint_sit_low"].push_back(
            {Globals::predicate_animpoint_bread, "animpoint_sit_low_eat_bread"});
        this->m_animpoint_table["animpoint_sit_low"].push_back(
            {Globals::predicate_animpoint_kolbasa, "animpoint_sit_low_eat_kolbasa"});
        this->m_animpoint_table["animpoint_sit_low"].push_back(
            {Globals::predicate_animpoint_vodka, "animpoint_sit_low_drink_vodka"});
        this->m_animpoint_table["animpoint_sit_low"].push_back(
            {Globals::predicate_animpoint_energy, "animpoint_sit_low_drink_energy"});
        this->m_animpoint_table["animpoint_sit_low"].push_back(
            {Globals::predicate_animpoint_guitar, "animpoint_sit_low_guitar"});
        this->m_animpoint_table["animpoint_sit_low"].push_back(
            {Globals::predicate_animpoint_harmonica, "animpoint_sit_low_harmonica"});

        this->m_animpoint_table["walker_camp"].push_back({Globals::predicate_animpoint_guitar, "play_guitar"});
        this->m_animpoint_table["walker_camp"].push_back({Globals::predicate_animpoint_harmonica, "play_harmonica"});
#pragma endregion

#pragma region Cordis Jobs Types Initializing
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameWalker] =
            Globals::GulagGenerator::kGulagJobPath;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameCamper] =
            Globals::GulagGenerator::kGulagJobPath;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNamePatrol] =
            Globals::GulagGenerator::kGulagJobPath;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameAnimpoint] =
            Globals::GulagGenerator::kGulagJobSmartCover;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameSmartCover] =
            Globals::GulagGenerator::kGulagJobSmartCover;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameRemark] =
            Globals::GulagGenerator::kGulagJobPoint;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameCover] =
            Globals::GulagGenerator::kGulagJobPoint;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameSleeper] =
            Globals::GulagGenerator::kGulagJobPath;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameMobWalker] =
            Globals::GulagGenerator::kGulagJobPath;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameMobHome] =
            Globals::GulagGenerator::kGulagJobPath;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameMobJump] =
            Globals::GulagGenerator::kGulagJobPoint;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameCompanion] =
            Globals::GulagGenerator::kGulagJobPoint;
#pragma endregion

#pragma region Cordis State Library initialization
        xr_map<xr_string, StateLibData> state_library;
        StateLibData state_lib_data = StateLibData(Globals::kUnsignedInt32Undefined, Globals::kUnsignedInt32Undefined,
            Globals::kUnsignedInt32Undefined, Globals::kUnsignedInt32Undefined, Globals::kStringUndefined,
            Globals::kStringUndefined, Globals::kStringUndefined);
        state_library["idle"] = state_lib_data;
        state_lib_data = StateLibData(Globals::kUnsignedInt32Undefined, Globals::kUnsignedInt32Undefined,
            Globals::kUnsignedInt32Undefined, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, Globals::kStringUndefined);
        state_library["smartcover"] = state_lib_data;
        state_lib_data =
            StateLibData(MonsterSpace::eMovementTypeWalk, MonsterSpace::eMentalStateFree, MonsterSpace::eBodyStateStand,
                Globals::kUnsignedInt32Undefined, "none", Globals::kStringUndefined, Globals::kStringUndefined);
        state_library["walk"] = state_lib_data;
        state_lib_data =
            StateLibData(MonsterSpace::eMovementTypeWalk, MonsterSpace::eMentalStateFree, MonsterSpace::eBodyStateStand,
                Globals::kUnsignedInt32Undefined, "none", Globals::kStringUndefined, Globals::kStringUndefined);
        state_library["walk_noweap"] = state_lib_data;
        state_lib_data =
            StateLibData(MonsterSpace::eMovementTypeRun, MonsterSpace::eMentalStateFree, MonsterSpace::eBodyStateStand,
                Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined, Globals::kStringUndefined);
        state_library["run"] = state_lib_data;
        state_lib_data =
            StateLibData(MonsterSpace::eMovementTypeRun, MonsterSpace::eMentalStatePanic, MonsterSpace::eBodyStateStand,
                Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined, Globals::kStringUndefined);
        state_library["sprint"] = state_lib_data;
        state_lib_data =
            StateLibData(MonsterSpace::eMovementTypeWalk, MonsterSpace::eMentalStateFree, MonsterSpace::eBodyStateStand,
                Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined, Globals::kStringUndefined);
        state_library["patrol"] = state_lib_data;
        state_lib_data =
            StateLibData(MonsterSpace::eMovementTypeWalk, MonsterSpace::eMentalStateFree, MonsterSpace::eBodyStateStand,
                Globals::kUnsignedInt32Undefined, "fire", Globals::kStringUndefined, Globals::kStringUndefined);
        state_library["patrol_fire"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeWalk, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            Globals::kStringUndefined, false, 0, true);
        state_library["raid"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeWalk, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "fire", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["raid_fire"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeWalk, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["sneak"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeRun, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["sneak_run"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeWalk, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["sneak_no_wpn"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeRun, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["assault"] = state_lib_data;
        state_lib_data =
            StateLibData(MonsterSpace::eMovementTypeRun, MonsterSpace::eMentalStateFree, MonsterSpace::eBodyStateStand,
                Globals::kUnsignedInt32Undefined, "fire", Globals::kStringUndefined, Globals::kStringUndefined);
        state_library["assault_fire"] = state_lib_data;
        state_lib_data =
            StateLibData(MonsterSpace::eMovementTypeRun, MonsterSpace::eMentalStateFree, MonsterSpace::eBodyStateStand,
                Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined, Globals::kStringUndefined);
        state_library["rush"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "idle");
        state_library["wait"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "none", Globals::kStringUndefined, "idle");
        state_library["wait_trade"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["wait_na"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            "idle");
        state_library["guard"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["guard_chasovoy"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["guard_na"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "fire", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["guard_fire"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            Globals::kStringUndefined, true, 0, false);
        state_library["threat"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            "bloodsucker_search");
        state_library["threat_danger"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            "give_orders");
        state_library["give_orders"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["threat_heli"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            Globals::kStringUndefined, true);
        state_library["threat_na"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "fire", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["threat_fire"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "sniper_fire", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["threat_sniper_fire"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            "hide");
        state_library["hide"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["hide_na"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "fire", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["hide_fire"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "sniper_fire", Globals::kStringUndefined,
            Globals::kStringUndefined);
        state_library["hide_sniper_fire"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, Globals::kStringUndefined,
            Globals::kStringUndefined, "caution");
        state_library["caution"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "choosing");
        state_library["choose"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "press");
        state_library["press"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "warding");
        state_library["ward"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "none", Globals::kStringUndefined,
            "ward_short");
        state_library["ward_short"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "none", Globals::kStringUndefined,
            "warding");
        state_library["ward_noweap"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "none", Globals::kStringUndefined,
            "warding_short");
        state_library["ward_noweap_short"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "none", Globals::kStringUndefined,
            "fold_arms");
        state_library["fold_arms"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, Globals::kStringUndefined,
            Globals::kStringUndefined, "poisk");
        state_library["search"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "stoop_no_weap");
        state_library["stoop_no_weap"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "salut");
        state_library["salut"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "salut_free");
        state_library["salut_free"] = state_lib_data;
        state_lib_data = StateLibData(Globals::kUnsignedInt32Undefined, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "prisoner");
        state_library["prisoner"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "hide");
        state_library["hide_no_wpn"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", "sit",
            Globals::kStringUndefined);
        state_library["sit"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", "sit_knee",
            Globals::kStringUndefined);
        state_library["sit_knee"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", "sit_ass",
            Globals::kStringUndefined);
        state_library["sit_ass"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", "sit_knee", "play_guitar");
        state_library["play_guitar"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", "sit_ass", "play_harmonica");
        state_library["play_harmonica"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "sleeping");
        state_library["sleep"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "hello");
        state_library["hello"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            "hello");
        state_library["hello_wpn"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "refuse");
        state_library["refuse"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            "claim");
        state_library["claim"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            "backoff");
        state_library["backoff"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            "backoff");
        state_library["backoff2"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, Globals::kStringUndefined,
            Globals::kStringUndefined, "punch");
        state_library["punch"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "search_corpse");
        state_library["search_corpse"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "help_wounded");
        state_library["help_wounded"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "dynamite");
        state_library["dynamite"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "binocular");
        state_library["binocular"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            "cr_raciya");
        state_library["hide_rac"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "raciya");
        state_library["wait_rac"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "none", Globals::kStringUndefined,
            "raciya");
        state_library["wait_rac_noweap"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            "raciya_stc");
        state_library["wait_rac_stc"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "unstrapped", Globals::kStringUndefined,
            "raciya");
        state_library["guard_rac"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "probe_stand");
        state_library["probe_stand"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "probe_stand_detector_advanced");
        state_library["probe_stand_detector_advanced"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "probe_stand_detector_elite");
        state_library["probe_stand_detector_elite"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "probe_way");
        state_library["probe_way"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "probe_way_detector_advanced");
        state_library["probe_way_detector_advanced"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "probe_way_detector_elite");
        state_library["probe_way_detector_elite"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "probe_crouch");
        state_library["probe_crouch"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "probe_crouch_detector_advanced");
        state_library["probe_crouch_detector_advanced"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "probe_crouch_detector_elite");
        state_library["probe_crouch_detector_elite"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "scaner_stand");
        state_library["scaner_stand"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "scaner_way");
        state_library["scaner_way"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "scaner_crouch");
        state_library["scaner_crouch"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "hands_up");
        state_library["hands_up"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, SightManager::eSightTypeCurrentDirection, "strapped",
            Globals::kStringUndefined, "wounded");
        state_library["wounded"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, SightManager::eSightTypeCurrentDirection, "drop", Globals::kStringUndefined,
            "wounded_heavy_1");
        state_library["wounded_heavy"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, SightManager::eSightTypeCurrentDirection, "drop", Globals::kStringUndefined,
            "wounded_heavy_2");
        state_library["wounded_heavy_2"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, SightManager::eSightTypeCurrentDirection, "drop", Globals::kStringUndefined,
            "wounded_heavy_3");
        state_library["wounded_heavy_3"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, SightManager::eSightTypeCurrentDirection, "drop", Globals::kStringUndefined,
            "wounded_zombie");
        state_library["wounded_zombie"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "trans_0");
        state_library["trans_0"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "trans_1");
        state_library["trans_1"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "trans_zombied");
        state_library["trans_zombied"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "talk_default");
        state_library["talk_default"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "drop", Globals::kStringUndefined,
            "psy_armed");
        state_library["psy_pain"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "psy_armed");
        state_library["psy_armed"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateCrouch, Globals::kUnsignedInt32Undefined, "fire", Globals::kStringUndefined,
            "psy_shoot", false, 1);
        state_library["psy_shoot"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateCrouch, SightManager::eSightTypeCurrentDirection, "drop", Globals::kStringUndefined,
            "wounded_heavy_1");
        state_library["lay_on_bed"] = state_lib_data;

#pragma region Cordis State Library Animpoint stuff
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_wall", "animpoint_stay_wall");
        state_library["animpoint_stay_wall"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_table", "animpoint_stay_table");
        state_library["animpoint_stay_table"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_high",
            "animpoint_sit_high");
        state_library["animpoint_sit_high"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_sit_normal", "animpoint_sit_normal");
        state_library["animpoint_sit_normal"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_low",
            "animpoint_sit_low");
        state_library["animpoint_sit_low"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_wall", "animpoint_stay_wall_eat_bread");
        state_library["animpoint_stay_wall_eat_bread"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_wall", "animpoint_stay_wall_eat_kolbasa");
        state_library["animpoint_stay_wall_eat_kolbasa"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_table", "animpoint_stay_table_eat_bread");
        state_library["animpoint_stay_table_eat_bread"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_table", "animpoint_stay_table_eat_kolbasa");
        state_library["animpoint_stay_table_eat_kolbasa"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_high",
            "animpoint_sit_high_eat_bread");
        state_library["animpoint_sit_high_eat_bread"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_high",
            "animpoint_sit_high_eat_kolbasa");
        state_library["animpoint_sit_high_eat_kolbasa"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_sit_normal", "animpoint_sit_normal_eat_bread");
        state_library["animpoint_sit_normal_eat_bread"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_sit_normal", "animpoint_sit_normal_eat_kolbasa");
        state_library["animpoint_sit_normal_eat_kolbasa"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_low",
            "animpoint_sit_low_eat_bread");
        state_library["animpoint_sit_low_eat_bread"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_low",
            "animpoint_sit_low_eat_kolbasa");
        state_library["animpoint_sit_low_eat_kolbasa"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_wall", "animpoint_stay_wall_drink_vodka");
        state_library["animpoint_stay_wall_drink_vodka"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_wall", "animpoint_stay_wall_drink_energy");
        state_library["animpoint_stay_wall_drink_energy"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_table", "animpoint_stay_table_drink_vodka");
        state_library["animpoint_stay_table_drink_vodka"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_table", "animpoint_stay_table_drink_energy");
        state_library["animpoint_stay_table_drink_energy"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_high",
            "animpoint_sit_high_drink_vodka");
        state_library["animpoint_sit_high_drink_vodka"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_high",
            "animpoint_sit_high_drink_energy");
        state_library["animpoint_sit_high_drink_energy"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_sit_normal", "animpoint_sit_normal_drink_vodka");
        state_library["animpoint_sit_normal_drink_vodka"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_low",
            "animpoint_sit_low_drink_vodka");
        state_library["animpoint_sit_low_drink_vodka"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_low",
            "animpoint_sit_low_drink_energy");
        state_library["animpoint_sit_low_drink_energy"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_wall", "animpoint_stay_wall_guitar");
        state_library["animpoint_stay_wall_guitar"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_table", "animpoint_stay_table_guitar");
        state_library["animpoint_stay_table_guitar"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_high",
            "animpoint_sit_high_guitar");
        state_library["animpoint_sit_high_guitar"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_sit_normal", "animpoint_sit_normal_guitar");
        state_library["animpoint_sit_normal_guitar"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_low",
            "animpoint_sit_low_guitar");
        state_library["animpoint_sit_low_guitar"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_wall", "animpoint_stay_wall_harmonica");
        state_library["animpoint_stay_wall_harmonica"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_table", "animpoint_stay_table_harmonica");
        state_library["animpoint_stay_table_harmonica"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_high",
            "animpoint_sit_high_harmonica");
        state_library["animpoint_sit_high_harmonica"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_sit_normal", "animpoint_sit_normal_harmonica");
        state_library["animpoint_sit_normal_harmonica"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_low",
            "animpoint_sit_low_harmonica");
        state_library["animpoint_sit_low_harmonica"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            "animpoint_stay_wall_weapon", Globals::kStringUndefined);
        state_library["animpoint_stay_wall_weapon"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            "animpoint_stay_table_weapon", Globals::kStringUndefined);
        state_library["animpoint_stay_table_weapon"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            "animpoint_sit_high_weapon", Globals::kStringUndefined);
        state_library["animpoint_sit_high_weapon"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            "animpoint_sit_normal_weapon", Globals::kStringUndefined);
        state_library["animpoint_sit_normal_weapon"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            "animpoint_sit_low_weapon", Globals::kStringUndefined);
        state_library["animpoint_sit_low_weapon"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_wall", Globals::kStringUndefined);
        state_library["animpoint_stay_wall_no_rnd"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_stay_table", Globals::kStringUndefined);
        state_library["animpoint_stay_table_no_rnd"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_high",
            Globals::kStringUndefined);
        state_library["animpoint_sit_high_no_rnd"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "animpoint_sit_normal", Globals::kStringUndefined);
        state_library["animpoint_sit_normal_no_rnd"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_low",
            Globals::kStringUndefined);
        state_library["animpoint_sit_low_no_rnd"] = state_lib_data;
#pragma endregion

#pragma region Cordis State Library Manager Scenario
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "zat_b14_give_artefact_idle");
        state_library["zat_b14_give_artefact_idle"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, Globals::kUnsignedInt32Undefined, "strapped", Globals::kStringUndefined,
            "zat_b14_give_artefact_act");
        state_library["zat_b14_give_artefact_act"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "zat_b14_stay_wall");
        state_library["zat_b14_stay_wall"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "zat_b38_stalker_break_lock");
        state_library["zat_b38_stalker_break_lock"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "zat_b38_stalker_turn_on_lift");
        state_library["zat_b38_stalker_turn_on_lift"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "zat_b38_stalker_jump_tonnel");
        state_library["zat_b38_stalker_jump_tonnel"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "zat_b38_stalker_alert");
        state_library["zat_b38_stalker_alert"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "zat_b20_noah_jump");
        state_library["zat_b20_noah_jump"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "pri_a17_ice_climb");
        state_library["pri_a17_ice_climb"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a17_fall_down");
        state_library["pri_a17_fall_down"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a17_pray_in");
        state_library["pri_a17_pray_in"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a17_pray");
        state_library["pri_a17_pray"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "zat_b22_medic_turn_idle");
        state_library["zat_b22_medic_turn_idle"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "zat_b22_medic_turn_out");
        state_library["zat_b22_medic_turn_out"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "fire",
            Globals::kStringUndefined, "zat_b22_medic_suicide");
        state_library["zat_b22_medic_suicide"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none", "zat_b3_tech_idle",
            "zat_b3_tech_drunk");
        state_library["zat_b3_tech_drunk"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none", "zat_b3_tech_idle",
            "zat_b3_tech_drink");
        state_library["zat_b3_tech_drink"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none", "zat_b3_tech_idle",
            "zat_b3_tech_idle_1");
        state_library["zat_b3_tech_idle"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none", "zat_b3_tech_idle",
            Globals::kStringUndefined);
        state_library["zat_b3_tech_idle_short"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none", "zat_b3_tech_idle",
            "zat_b3_tech_surprise");
        state_library["zat_b3_tech_surprise"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "give_orders");
        state_library["give_orders"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "bloodsucker_search");
        state_library["bloodsucker_search"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateDanger,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "fire",
            Globals::kStringUndefined, "bloodsucker_panic");
        state_library["bloodsucker_panic"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "fire",
            Globals::kStringUndefined, "suicide_animation");
        state_library["suicide_animation"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "jup_b10_drunk_ravings");
        state_library["jup_b10_drunk_ravings"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "pas_b400_vano_probe");
        state_library["pas_b400_vano_probe"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped", "animpoint_sit_high",
            "pri_a28_kirillov_sit_high_radio");
        state_library["pri_a28_kirillov_sit_high_radio"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a18_inspert_monolit_actor");
        state_library["pri_a18_inspert_monolit_actor"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            "pri_a20_colonel_radio", Globals::kStringUndefined);
        state_library["pri_a20_colonel_radio"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "fire", "pri_a21_sentry_madness",
            Globals::kStringUndefined);
        state_library["pri_a21_sentry_madness"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "fire",
            "pri_a21_sentry_madness_idle", "pri_a21_sentry_madness_suicide");
        state_library["pri_a21_sentry_madness_suicide"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            "pri_a22_colonel_lean_on_table", Globals::kStringUndefined);
        state_library["pri_a22_colonel_lean_on_table"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a28_army_trance_out");
        state_library["pri_a28_army_trance_out"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "zat_b106_wounded_idle");
        state_library["zat_b106_wounded_idle"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "zat_b38_cop_dead");
        state_library["zat_b38_cop_dead"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            "jup_b15_zulus_sit_idle_short", "jup_b15_zulus_sit_drink");
        state_library["jup_b15_zulus_sit_drink"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            "jup_b15_zulus_sit_idle_short", "jup_b15_zulus_sit_idle");
        state_library["jup_b15_zulus_sit_idle"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            "jup_b15_zulus_sit_idle_short", Globals::kStringUndefined);
        state_library["jup_b15_zulus_sit_idle_short"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "jup_b15_zulus_sit_out");
        state_library["jup_b15_zulus_sit_out"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_actor_one");
        state_library["jup_b219_actor_one"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_actor_one_vano");
        state_library["jup_b219_actor_one_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_actor_one_monolith");
        state_library["jup_b219_actor_one_monolith"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_actor_one_soldier");
        state_library["jup_b219_actor_one_soldier"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_actor_all");
        state_library["jup_b219_actor_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_actor_soldier_vano");
        state_library["jup_b219_actor_soldier_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_actor_soldier_monolith");
        state_library["jup_b219_actor_soldier_monolith"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_actor_vano_monolith");
        state_library["jup_b219_actor_vano_monolith"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_zulus_one");
        state_library["jup_b219_zulus_one"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_zulus_one_vano");
        state_library["jup_b219_zulus_one_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_zulus_one_monolith");
        state_library["jup_b219_zulus_one_monolith"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_zulus_one_soldier");
        state_library["jup_b219_zulus_one_soldier"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_zulus_all");
        state_library["jup_b219_zulus_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_zulus_soldier_vano");
        state_library["jup_b219_zulus_soldier_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_zulus_soldier_monolith");
        state_library["jup_b219_zulus_soldier_monolith"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_zulus_vano_monolith");
        state_library["jup_b219_zulus_vano_monolith"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_soldier_one");
        state_library["jup_b219_soldier_one"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_soldier_all");
        state_library["jup_b219_soldier_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_soldier_soldier_vano");
        state_library["jup_b219_soldier_soldier_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_soldier_soldier_monolith");
        state_library["jup_b219_soldier_soldier_monolith"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_monolith_one");
        state_library["jup_b219_monolith_one"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_monolith_all");
        state_library["jup_b219_monolith_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_monolith_soldier_monolith");
        state_library["jup_b219_monolith_soldier_monolith"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_monolith_vano_monolith");
        state_library["jup_b219_monolith_vano_monolith"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_vano_one");
        state_library["jup_b219_vano_one"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_vano_all");
        state_library["jup_b219_vano_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_vano_soldier_vano");
        state_library["jup_b219_vano_soldier_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_b219_vano_vano_monolith");
        state_library["jup_b219_vano_vano_monolith"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "jup_b219_azot_one");
        state_library["jup_b219_azot_one"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "jup_b219_azot_one_vano");
        state_library["jup_b219_azot_one_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "jup_b219_azot_one_monolith");
        state_library["jup_b219_azot_one_monolith"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "jup_b219_azot_one_soldier");
        state_library["jup_b219_azot_one_soldier"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "jup_b219_azot_soldier_vano");
        state_library["jup_b219_azot_soldier_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "jup_b219_azot_soldier_monolith");
        state_library["jup_b219_azot_soldier_monolith"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "jup_b219_azot_vano_monolith");
        state_library["jup_b219_azot_vano_monolith"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "jup_b219_azot_all");
        state_library["jup_b219_azot_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "zat_b100_heli_2_serch");
        state_library["zat_b100_heli_2_serch"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "zat_b101_heli_5_serch");
        state_library["zat_b101_heli_5_serch"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "jup_b217_guide_stand");
        state_library["jup_b217_guide_stand"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "jup_b217_nitro_stand");
        state_library["jup_b217_nitro_stand"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "jup_b41_novikov_stand");
        state_library["jup_b41_novikov_stand"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_b305_actor");
        state_library["pri_b305_actor"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_a9_cam1_actor");
        state_library["jup_a9_cam1_actor"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_a9_cam2_actor");
        state_library["jup_a9_cam2_actor"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "jup_a9_cam3_actor");
        state_library["jup_a9_cam3_actor"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "jup_b217_nitro_straight");
        state_library["jup_b217_nitro_straight"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "pri_a25_psy_medic_idle");
        state_library["pri_a25_psy_medic_idle"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "pri_a25_psy_medic_out");
        state_library["pri_a25_psy_medic_out"] = state_lib_data;
#pragma endregion

#pragma region Cordis State Library PriA15
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_idle_none");
        state_library["pri_a15_idle_none"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "strapped",
            Globals::kStringUndefined, "pri_a15_idle_none");
        state_library["pri_a15_idle_strap"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_idle_unstrap");
        state_library["pri_a15_idle_unstrap"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_vano_all");
        state_library["pri_a15_vano_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_vano_1_sokolov");
        state_library["pri_a15_vano_1_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_vano_1_zulus");
        state_library["pri_a15_vano_1_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_vano_1_wanderer");
        state_library["pri_a15_vano_1_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_vano_2_sokolov_zulus");
        state_library["pri_a15_vano_2_sokolov_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_vano_2_sokolov_wanderer");
        state_library["pri_a15_vano_2_sokolov_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_vano_2_zulus_wanderer");
        state_library["pri_a15_vano_2_zulus_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_vano_3_vano_alive");
        state_library["pri_a15_vano_3_vano_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_sokolov_all");
        state_library["pri_a15_sokolov_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_sokolov_1_vano");
        state_library["pri_a15_sokolov_1_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_sokolov_1_zulus");
        state_library["pri_a15_sokolov_1_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_sokolov_1_wanderer");
        state_library["pri_a15_sokolov_1_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_sokolov_2_vano_zulus");
        state_library["pri_a15_sokolov_2_vano_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_sokolov_2_vano_wanderer");
        state_library["pri_a15_sokolov_2_vano_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_sokolov_2_zulus_wanderer");
        state_library["pri_a15_sokolov_2_zulus_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_sokolov_3_sokolov_alive");
        state_library["pri_a15_sokolov_3_sokolov_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_zulus_all");
        state_library["pri_a15_zulus_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_zulus_1_vano");
        state_library["pri_a15_zulus_1_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_zulus_1_sokolov");
        state_library["pri_a15_zulus_1_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_zulus_1_wanderer");
        state_library["pri_a15_zulus_1_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_zulus_2_vano_sokolov");
        state_library["pri_a15_zulus_2_vano_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_zulus_2_vano_wanderer");
        state_library["pri_a15_zulus_2_vano_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_zulus_2_sokolov_wanderer");
        state_library["pri_a15_zulus_2_sokolov_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_zulus_3_zulus_alive");
        state_library["pri_a15_zulus_3_zulus_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_wanderer_all");
        state_library["pri_a15_wanderer_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_wanderer_1_vano");
        state_library["pri_a15_wanderer_1_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_wanderer_1_sokolov");
        state_library["pri_a15_wanderer_1_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_wanderer_1_zulus");
        state_library["pri_a15_wanderer_1_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_wanderer_2_vano_sokolov");
        state_library["pri_a15_wanderer_2_vano_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_wanderer_2_vano_zulus");
        state_library["pri_a15_wanderer_2_vano_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_wanderer_2_sokolov_zulus");
        state_library["pri_a15_wanderer_2_sokolov_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_wanderer_3_wanderer_alive");
        state_library["pri_a15_wanderer_3_wanderer_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_all");
        state_library["pri_a15_actor_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_1_vano");
        state_library["pri_a15_actor_1_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_1_sokolov");
        state_library["pri_a15_actor_1_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_1_zulus");
        state_library["pri_a15_actor_1_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_1_wanderer");
        state_library["pri_a15_actor_1_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_2_vano_sokolov");
        state_library["pri_a15_actor_2_vano_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_2_vano_zulus");
        state_library["pri_a15_actor_2_vano_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_2_vano_wanderer");
        state_library["pri_a15_actor_2_vano_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_2_sokolov_zulus");
        state_library["pri_a15_actor_2_sokolov_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_2_sokolov_wanderer");
        state_library["pri_a15_actor_2_sokolov_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_2_zulus_wanderer");
        state_library["pri_a15_actor_2_zulus_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_3_vano_alive");
        state_library["pri_a15_actor_3_vano_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_3_sokolov_alive");
        state_library["pri_a15_actor_3_sokolov_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_3_zulus_alive");
        state_library["pri_a15_actor_3_zulus_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_3_wanderer_alive");
        state_library["pri_a15_actor_3_wanderer_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "none",
            Globals::kStringUndefined, "pri_a15_actor_all_dead");
        state_library["pri_a15_actor_all_dead"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_all");
        state_library["pri_a15_military_tarasov_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_1_vano");
        state_library["pri_a15_military_tarasov_1_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_1_sokolov");
        state_library["pri_a15_military_tarasov_1_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_1_zulus");
        state_library["pri_a15_military_tarasov_1_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_1_wanderer");
        state_library["pri_a15_military_tarasov_1_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_2_vano_sokolov");
        state_library["pri_a15_military_tarasov_2_vano_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_2_vano_zulus");
        state_library["pri_a15_military_tarasov_2_vano_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_2_vano_wanderer");
        state_library["pri_a15_military_tarasov_2_vano_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_2_sokolov_zulus");
        state_library["pri_a15_military_tarasov_2_sokolov_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_2_sokolov_wanderer");
        state_library["pri_a15_military_tarasov_2_sokolov_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_2_zulus_wanderer");
        state_library["pri_a15_military_tarasov_2_zulus_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_3_vano_alive");
        state_library["pri_a15_military_tarasov_3_vano_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_3_sokolov_alive");
        state_library["pri_a15_military_tarasov_3_sokolov_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_3_zulus_alive");
        state_library["pri_a15_military_tarasov_3_zulus_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_3_wanderer_alive");
        state_library["pri_a15_military_tarasov_3_wanderer_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_tarasov_all_dead");
        state_library["pri_a15_military_tarasov_all_dead"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_all");
        state_library["pri_a15_military_2_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_1_vano");
        state_library["pri_a15_military_2_1_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_1_sokolov");
        state_library["pri_a15_military_2_1_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_1_zulus");
        state_library["pri_a15_military_2_1_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_1_wanderer");
        state_library["pri_a15_military_2_1_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_2_vano_sokolov");
        state_library["pri_a15_military_2_2_vano_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_2_vano_zulus");
        state_library["pri_a15_military_2_2_vano_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_2_vano_wanderer");
        state_library["pri_a15_military_2_2_vano_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_2_sokolov_zulus");
        state_library["pri_a15_military_2_2_sokolov_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_2_sokolov_wanderer");
        state_library["pri_a15_military_2_2_sokolov_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_2_zulus_wanderer");
        state_library["pri_a15_military_2_2_zulus_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_3_vano_alive");
        state_library["pri_a15_military_2_3_vano_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_3_sokolov_alive");
        state_library["pri_a15_military_2_3_sokolov_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_3_zulus_alive");
        state_library["pri_a15_military_2_3_zulus_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_3_wanderer_alive");
        state_library["pri_a15_military_2_3_wanderer_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_2_all_dead");
        state_library["pri_a15_military_2_all_dead"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_all");
        state_library["pri_a15_military_3_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_1_vano");
        state_library["pri_a15_military_3_1_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_1_sokolov");
        state_library["pri_a15_military_3_1_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_1_zulus");
        state_library["pri_a15_military_3_1_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_1_wanderer");
        state_library["pri_a15_military_3_1_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_2_vano_sokolov");
        state_library["pri_a15_military_3_2_vano_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_2_vano_zulus");
        state_library["pri_a15_military_3_2_vano_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_2_vano_wanderer");
        state_library["pri_a15_military_3_2_vano_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_2_sokolov_zulus");
        state_library["pri_a15_military_3_2_sokolov_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_2_sokolov_wanderer");
        state_library["pri_a15_military_3_2_sokolov_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_2_zulus_wanderer");
        state_library["pri_a15_military_3_2_zulus_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_3_vano_alive");
        state_library["pri_a15_military_3_3_vano_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_3_sokolov_alive");
        state_library["pri_a15_military_3_3_sokolov_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_3_zulus_alive");
        state_library["pri_a15_military_3_3_zulus_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_3_wanderer_alive");
        state_library["pri_a15_military_3_3_wanderer_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_3_all_dead");
        state_library["pri_a15_military_3_all_dead"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_all");
        state_library["pri_a15_military_4_all"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_1_vano");
        state_library["pri_a15_military_4_1_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_1_vano");
        state_library["pri_a15_military_4_1_vano"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_1_sokolov");
        state_library["pri_a15_military_4_1_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_1_zulus");
        state_library["pri_a15_military_4_1_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_1_wanderer");
        state_library["pri_a15_military_4_1_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_2_vano_sokolov");
        state_library["pri_a15_military_4_2_vano_sokolov"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_2_vano_zulus");
        state_library["pri_a15_military_4_2_vano_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_2_vano_wanderer");
        state_library["pri_a15_military_4_2_vano_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_2_sokolov_zulus");
        state_library["pri_a15_military_4_2_sokolov_zulus"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_2_sokolov_wanderer");
        state_library["pri_a15_military_4_2_sokolov_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_2_zulus_wanderer");
        state_library["pri_a15_military_4_2_zulus_wanderer"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_3_vano_alive");
        state_library["pri_a15_military_4_3_vano_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_3_sokolov_alive");
        state_library["pri_a15_military_4_3_sokolov_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_3_zulus_alive");
        state_library["pri_a15_military_4_3_zulus_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_3_wanderer_alive");
        state_library["pri_a15_military_4_3_wanderer_alive"] = state_lib_data;
        state_lib_data = StateLibData(MonsterSpace::eMovementTypeStand, MonsterSpace::eMentalStateFree,
            MonsterSpace::eBodyStateStand, SightManager::eSightTypeAnimationDirection, "unstrapped",
            Globals::kStringUndefined, "pri_a15_military_4_all_dead");
        state_library["pri_a15_military_4_all_dead"] = state_lib_data;
#pragma endregion
#pragma endregion

#pragma region SmartTerrain Initializing
        this->m_game_server_nearest_to_actor_smart_terrain.first = std::uint32_t(-1);
        this->m_game_server_nearest_to_actor_smart_terrain.second = std::uint32_t(-1);
        this->m_registered_smart_terrain_territory_type[Globals::kSmartTerrainTerritoryBase] = true;
        this->m_registered_smart_terrain_territory_type[Globals::kSmartTerrainTerritoryDefault] = true;
        this->m_registered_smart_terrain_territory_type[Globals::kSmartTerrainTerritoryResource] = true;
        this->m_registered_smart_terrain_territory_type[Globals::kSmartTerrainTerritoryTerritory] = true;
        this->m_registered_smart_terrain_path_fields.push_back(Globals::kSmartTerrainPathFieldCenterPoint);
        this->m_registered_smart_terrain_path_fields.push_back(Globals::kSmartTerrainPathFieldPathHome);
        this->m_registered_smart_terrain_path_fields.push_back(Globals::kSmartTerrainPathFieldPathMain);
        this->m_registered_smart_terrain_path_fields.push_back(Globals::kSmartTerrainPathFieldPathWalk);

        this->m_registered_smart_terrain_control_script_states["normal"] = Script_SmartTerrainControl_States::kNormal;
        this->m_registered_smart_terrain_control_script_states["danger"] = Script_SmartTerrainControl_States::kDanger;
        this->m_registered_smart_terrain_control_script_states["alarm"] = Script_SmartTerrainControl_States::kAlarm;
#pragma endregion

#pragma region Cordis SimulationBoard Initializing
        // @ Lord: реализовать автоматическое считываение и сделать нумерацию от 0
        this->m_simulationboard_group_id_by_levels_name["zaton"] = 1;
        this->m_simulationboard_group_id_by_levels_name["pripyat"] = 2;
        this->m_simulationboard_group_id_by_levels_name["jupiter"] = 3;
        this->m_simulationboard_group_id_by_levels_name["labx8"] = 4;
        this->m_simulationboard_group_id_by_levels_name["jupiter_underground"] = 5;
#pragma endregion

#pragma region Cordis SimulationSquad Initializing
        this->m_simulationsquad_is_squad_monster_by_type["monster_predatory_day"] = true;
        this->m_simulationsquad_is_squad_monster_by_type["monster_predatory_night"] = true;
        this->m_simulationsquad_is_squad_monster_by_type["monster_vegetarian"] = true;
        this->m_simulationsquad_is_squad_monster_by_type["monster_zombied_day"] = true;
        this->m_simulationsquad_is_squad_monster_by_type["monster_zombied_night"] = true;
        this->m_simulationsquad_is_squad_monster_by_type["monster_special"] = true;
        this->m_simulationsquad_is_squad_monster_by_type["monster"] = true;
#pragma endregion

#pragma region Cordis SE_Actor initializing
        this->m_game_smarts_by_no_assault_zone["zat_a2_sr_no_assault"] = "zat_stalker_base_smart";
        this->m_game_smarts_by_no_assault_zone["jup_a6_sr_no_assault"] = "jup_a6";
        this->m_game_smarts_by_no_assault_zone["jup_b41_sr_no_assault"] = "jup_b41";
#pragma endregion

#pragma region Cordis Registering XR_CONDITINO Functions
        _REGISTER_FULL_FUNCTION_XR_CONDITION(actor_on_level, XR_CONDITION::is_actor_on_level)
#pragma endregion
    }

public:
    inline static Script_GlobalHelper& getInstance(void) noexcept
    {
        static Script_GlobalHelper instance;
        return instance;
    }

    ~Script_GlobalHelper(void)
    {
        if (this->m_tutorial)
        {
            Msg("[Scripts/Script_GlobalHelper/dtor()] Deleting GlobalTutorial!");
            delete this->m_tutorial;
            this->m_tutorial = nullptr;
        }
    }
    Script_GlobalHelper(const Script_GlobalHelper&) = delete;
    Script_GlobalHelper& operator=(const Script_GlobalHelper&) = delete;
    Script_GlobalHelper(Script_GlobalHelper&&) = delete;
    Script_GlobalHelper& operator=(Script_GlobalHelper&&) = delete;

    inline void InitializeAlifeSimulator(void) {}
    inline void RegisterFunctionsFromAnotherFiles(void)
    {
        // @ Lord: реализовать
#pragma region XR_CONDITION

#pragma endregion

#pragma region XR_EFFECTS

#pragma endregion
    }

    inline const xr_map<std::uint32_t, bool>& getMonsterClasses(void) const noexcept { return this->m_monster_classes; }

    inline void setMonsterClasses(const xr_map<std::uint32_t, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setMonsterClasses(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map! No assignment!");
            return;
        }

        this->m_monster_classes = map;
    }

    inline void setMonsterClasses(const std::pair<std::uint32_t, bool>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setMonsterClasses(pair)] WARNING: pair.first = std::uint32_t(-1)! Your value is "
                "undefined! No assignment!");
            return;
        }

        this->m_monster_classes.insert(pair);
    }

    inline void setMonsterClasses(const std::uint32_t& monster_id, const bool& value) noexcept
    {
        if (monster_id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setMonsterClasses(monster_id, value)] WARNING: monster_id = std::uint32_t(-1)! "
                "Your value is undefined! No assignment!");
            return;
        }

        this->m_monster_classes[monster_id] = value;
    }

    inline const xr_map<std::uint32_t, bool>& getStalkerClasses(void) const noexcept { return this->m_stalker_classes; }

    inline void setStalkerClasses(const xr_map<std::uint32_t, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setStalkerClasses(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map! No assignment!");
            return;
        }

        this->m_stalker_classes = map;
    }

    inline void setStalkerClasses(const std::pair<std::uint32_t, bool>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setStalkerClasses(pair)] WARNING: pair.first = std::uint32_t(-1)! Your value is "
                "undefined! No assignment!");
            return;
        }

        this->m_stalker_classes.insert(pair);
    }

    inline void setStalkerClasses(const std::uint32_t& stalker_id, const bool& value) noexcept
    {
        if (stalker_id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setStalkerClasses(stalker_id, value)] WARNING: stalker_id = std::uint32_t(-1)! "
                "Your value is undefined! No assignment!");
            return;
        }

        this->m_stalker_classes[stalker_id] = value;
    }

    inline const xr_map<std::uint32_t, bool>& getWeaponClasses(void) const noexcept { return this->m_weapon_classes; }

    inline void setWeaponClasses(const xr_map<std::uint32_t, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setWeaponClasses(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map! No assignment!");
            return;
        }

        this->m_weapon_classes = map;
    }

    inline void setWeaponClasses(const std::pair<std::uint32_t, bool>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setWeaponClasses(pair)] WARNING: pair.first = std::uint32_t(-1)! You value is "
                "undefined! No assignment!");
            return;
        }

        this->m_weapon_classes.insert(pair);
    }

    inline void setWeaponClasses(const std::uint32_t& weapon_id, const bool& value) noexcept
    {
        if (weapon_id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setWeaponClasses(weapon_id, value)] WARNING: weapon_id = std::uint32_t(-1)! Your "
                "value is undefined! No assignment!");
            return;
        }

        this->m_weapon_classes[weapon_id] = value;
    }

    inline const xr_map<std::uint32_t, bool>& getArtefactClasses(void) const noexcept
    {
        return this->m_artefact_classes;
    }

    inline void setArtefactClasses(const xr_map<std::uint32_t, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setArtefactClasses(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map! No assignment!");
            return;
        }

        this->m_artefact_classes = map;
    }

    inline void setArtefactClasses(const std::pair<std::uint32_t, bool>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setArtefactClasses(pair)] WARNING: pair.first == std::uint32_t(-1)! You are "
                "trying to set an undefined value! No assignment!");
            return;
        }

        this->m_artefact_classes.insert(pair);
    }

    inline void setArtefactClasses(const std::uint32_t& artefact_id, const bool& value) noexcept
    {
        if (artefact_id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setArtefactClasses(artefact_id, value)] WARNING: artefact_id = "
                "std::uint32_t(-1)! Your value is undefined! No assignment!");
            return;
        }

        this->m_artefact_classes[artefact_id] = value;
    }

    inline const xr_map<std::uint32_t, bool>& getAmmoSection(void) const noexcept { return this->m_ammo_section; }

    inline void setAmmoSection(const xr_map<std::uint32_t, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setAmmoSection(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map! No assignment!");
            return;
        }

        this->m_ammo_section = map;
    }

    inline void setAmmoSection(const std::pair<std::uint32_t, bool>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setAmmoSection(pair)] WARNING: pair.first = std::uint32_t(-1)! "
                "Your value is undefined!!! No assignment!");
            return;
        }

        this->m_ammo_section.insert(pair);
    }

    inline void setAmmoSection(const std::uint32_t& ammo_section, const bool& value) noexcept
    {
        if (ammo_section == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setAmmoSection(ammo_section, value)] WARNING: ammo_section = std::uint32_t(-1)! "
                "Your value is undefined!!! No assignment!");
            return;
        }

        this->m_ammo_section[ammo_section] = value;
    }

    inline const xr_map<xr_string, bool>& getQuestSection(void) const noexcept { return this->m_quest_section; }

    inline void setQuestSection(const xr_map<xr_string, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setQuestSection(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map! No assignment!");
            return;
        }

        this->m_quest_section = map;
    }

    inline void setQuestSection(const std::pair<xr_string, bool>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setQuestSection] WARNING: pair.first.size() = 0! You are trying to set an "
                "empty pair! No assignment!");
            return;
        }

        this->m_quest_section.insert(pair);
    }

    inline void setQuestSection(const xr_string& section_name, const bool& value) noexcept
    {
        if (!section_name.size())
        {
            Msg("[Script_GlobalHelper/setQuestSection] WARNING: section_name.size() = 0! You are trying to set an "
                "empty string! No assignment!");
            return;
        }

        this->m_quest_section[section_name] = value;
    }

    inline const xr_map<xr_string, xr_string>& getSquadCommunityByBehavior(void) const noexcept
    {
        return this->m_squad_community_by_behavior;
    }

    inline void setSquadCommunityByBehavior(const xr_map<xr_string, xr_string>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setSquadCommunityByBehavior(map)] WARNING: map.size() = 0! You are trying to set "
                "an empty map! No assignment!");
            return;
        }

        this->m_squad_community_by_behavior = map;
    }

    inline void setSquadCommunityByBehavior(const std::pair<xr_string, xr_string>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setSquadCommunityByBehavior(pair)] WARNING: "
                "pair.first.size() = 0! You are trying to set an empty pair! No assignment!");
            return;
        }

        this->m_squad_community_by_behavior.insert(pair);
    }

    inline void setSquadCommunityByBehavior(const xr_string& community_name, const xr_string& behavior_name) noexcept
    {
        if (!community_name.size())
        {
            Msg("[Script_GlobalHelper/setSquadCommunityByBehavior(community_name, behavior_name)] WARNING: "
                "community_name.size() = 0! You are trying to set an empty string! No assignment!");
            return;
        }

        this->m_squad_community_by_behavior[community_name] = behavior_name;
    }

    inline xr_map<xr_string, AnyCallable<void>>& getRegisteredFunctionsXREffects(void) noexcept
    {
        return this->m_registered_functions_xr_effects;
    }

    inline void setRegisteredFunctionsXREffects(const xr_map<xr_string, AnyCallable<void>>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredFunctionsXREffects(map)] WARNING: map.size() = 0! You are trying to "
                "set an empty map! No assignment!");
            return;
        }

        this->m_registered_functions_xr_effects = map;
    }

    inline void setRegisteredFunctionsXREffects(const std::pair<xr_string, AnyCallable<void>>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredFunctionsXREffects(pair)] WARNING: "
                "pair.first.size() = 0! You are trying to set an empty pair! No assignment!");
            return;
        }

        this->m_registered_functions_xr_effects.insert(pair);
    }

    inline void setRegisteredFunctionsXREffects(const xr_string& function_name, const AnyCallable<void>& function)
    {
        if (!function_name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredFunctionsXREffects(function_name, function)] WARNING: "
                "function_name.size() = 0! You are trying to set an empty string! No assignment!");
            return;
        }

        this->m_registered_functions_xr_effects[function_name] = function;
    }

    inline xr_map<xr_string, AnyCallable<bool>>& getRegisteredFunctionsXRCondition(void) noexcept
    {
        return this->m_registered_functions_xr_conditions;
    }

    inline void setRegisteredFunctionsXRCondition(const xr_map<xr_string, AnyCallable<bool>>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredFunctionsXRCondition(map)] WARNING: map.size() = 0! You are trying "
                "to set an empty map! No assignment!");
            return;
        }

        this->m_registered_functions_xr_conditions = map;
    }

    inline void setRegisteredFunctionsXRCondition(const std::pair<xr_string, AnyCallable<bool>>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredFunctionsXRCondition(pair)] WARNING: "
                "pair.first.size() = 0! You are trying to set an empty pair! No assignment!");
            return;
        }

        this->m_registered_functions_xr_conditions.insert(pair);
    }

    inline void setRegisteredFunctionsXRCondition(const xr_string& function_name, const AnyCallable<bool>& function)
    {
        if (!function_name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredFunctionsXRCondition(function_name, function)] WARNING: "
                "function_name.size() = 0! You are trying to set an empty string! No assignment!");
            return;
        }

        this->m_registered_functions_xr_conditions[function_name] = function;
    }

    // @ In-Game
    inline const xr_map<xr_string, Script_SE_SmartCover*>& getGameRegisteredServerSmartCovers(void) const noexcept
    {
        return this->m_game_registered_smartcovers;
    }

    inline void setGameRegisteredServerSmartCovers(const xr_map<xr_string, Script_SE_SmartCover*>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCovers(map)] WARNING: map.size() = 0! You are trying "
                "to set an empty map! No assignment!");
            return;
        }

        this->m_game_registered_smartcovers = map;
    }

    inline void setGameRegisteredServerSmartCovers(const std::pair<xr_string, Script_SE_SmartCover*>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCovers(name, server_smartcover)] WARNING: "
                "pair.first.size() = 0! You are trying to set an empty string! No assignment!");
            return;
        }

        if (!pair.second)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCovers(name, server_smartcover)] WARNING: "
                "pair.second = null! You are trying to set an empty object! No assignment!");
            return;
        }

        this->m_game_registered_smartcovers.insert(pair);
    }

    inline void setGameRegisteredServerSmartCovers(const xr_string& name, Script_SE_SmartCover* server_smartcover)
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCovers(name, server_smartcover)] WARNING: "
                "name.size() = 0! You are trying to set an empty string! No assignment!");
            return;
        }

        if (!server_smartcover)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCovers(name, server_smartcover)] WARNING: "
                "server_smartcover = null! You are trying to set an empty object! No assignment!");
            return;
        }

        this->m_game_registered_smartcovers[name] = server_smartcover;
    }

    // @ In-Game
    inline const xr_map<std::uint8_t, xr_map<std::uint32_t, Script_SE_SmartCover*>>&
    getGameRegisteredServerSmartCoversByLevelID(void) const noexcept
    {
        return this->m_game_registered_smartcovers_by_level_id;
    }

    // Lord: Проверить данные сеттеры на правильность наименования аргументов!
    inline void setGameRegisteredServerSmartCoversByLevelID(
        const xr_map<std::uint8_t, xr_map<std::uint32_t, Script_SE_SmartCover*>>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(map)] WARNING: map.size() = 0! You "
                "are trying to set an empty map! No assignment!");
            return;
        }

        this->m_game_registered_smartcovers_by_level_id = map;
    }

    inline void setGameRegisteredServerSmartCoversByLevelID(
        const std::uint8_t& level_id, const xr_map<std::uint32_t, Script_SE_SmartCover*>& map)
    {
        if (level_id == Globals::kUnsignedInt8Undefined)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, map)] "
                "WARNING: level_id = std::uint8_t(-1)! You are trying to set an undefined number of unsigned int! No "
                "assignment");
            return;
        }

        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, map)] WARNING: map.size() "
                "= 0! You are trying to set an empty map! No assignment!");
            return;
        }

        this->m_game_registered_smartcovers_by_level_id[level_id] = map;
    }

    inline void setGameRegisteredServerSmartCoversByLevelID(
        const std::uint8_t& level_id, const std::pair<std::uint32_t, Script_SE_SmartCover*>& pair)
    {
        if (level_id == Globals::kUnsignedInt8Undefined)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, pair)] "
                "WARNING: level_id = std::uint8_t(-1)! You are trying to set an undefined number of unsigned int! No "
                "assignment");
            return;
        }

        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, pair)] "
                "WARNING: pair.first = std::uint32_t(-1)! You are trying to set an undefined number of unsigned int! "
                "No "
                "assignment!");
            return;
        }

        if (!pair.second)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, pair)] "
                "WARNING: pair.second = null! You are trying to set an empty object! No assignment!");
            return;
        }

        this->m_game_registered_smartcovers_by_level_id[level_id].insert(pair);
    }

    inline void setGameRegisteredServerSmartCoversByLevelID(
        const std::uint8_t& level_id, const std::uint32_t& id, Script_SE_SmartCover* server_smartcover)
    {
        if (level_id == Globals::kUnsignedInt8Undefined)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, id, server_smartcover)] "
                "WARNING: level_id = std::uint8_t(-1)! You are trying to set an undefined number of unsigned int! No "
                "assignment");
            return;
        }

        if (id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, id, server_smartcover)] "
                "WARNING: id = std::uint32_t(-1)! You are trying to set an undefined number of unsigned int! No "
                "assignment!");
            return;
        }

        if (!server_smartcover)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, id, server_smartcover)] "
                "WARNING: server_smartcover = null! You are trying to set an empty object! No assignment!");
            return;
        }

        this->m_game_registered_smartcovers_by_level_id[level_id][id] = server_smartcover;
    }

    // @ It uses in Level Editor as list of smartcovers as spawnelement and it's using in-game
    inline const xr_map<xr_string, SmartCoverData>& getRegisteredSmartCovers(void) const noexcept
    {
        return this->m_registered_smartcovers;
    }

    inline void setRegisteredSmartCovers(const xr_map<xr_string, SmartCoverData>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartCovers(map)] WARNING: map.size() = 0! You are trying to set an "
                "empty map! No assignment!");
            return;
        }

        this->m_registered_smartcovers = map;
    }

    inline void setRegisteredSmartCovers(const std::pair<xr_string, SmartCoverData>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartCovers(smart_name, data)] WARNING: pair.first.size() = 0! You "
                "are trying to set an empty string, No assignment!");
            return;
        }

        this->m_registered_smartcovers.insert(pair);
    }

    inline void setRegisteredSmartCovers(const xr_string& smart_name, const SmartCoverData& data) noexcept
    {
        if (!smart_name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartCovers(smart_name, data)] WARNING: smart_name.size() = 0! You "
                "are trying to set an empty string, No assignment");
            return;
        }

        this->m_registered_smartcovers[smart_name] = data;
    }

    inline const xr_map<xr_string, CScriptGameObject*>& getGameRegisteredCombatSpaceRestrictors(void) const noexcept
    {
        return this->m_game_registered_combat_spacerestrictors;
    }

    inline void setGameRegisteredCombatSpaceRestrictors(const xr_map<xr_string, CScriptGameObject*>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredCombatSpaceRestrictors(space_name, client_zone)] WARNING: "
                "map.size() = 0! You are trying to set an empty map! No assignment!");
            return;
        }

        this->m_game_registered_combat_spacerestrictors = map;
    }

    inline void setGameRegisteredCombatSpaceRestrictors(const std::pair<xr_string, CScriptGameObject*>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredCombatSpaceRestrictors(space_name, client_zone)] WARNING: "
                "pair.first.size() = 0! You are trying to set an empty string! No assignemnt!");
            return;
        }

        if (!pair.second)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredCombatSpaceRestrictors(space_name, client_zone)] WARNING: "
                "pair.second = null! You are trying to set an empty object! No assignment!");
            return;
        }

        this->m_game_registered_combat_spacerestrictors.insert(pair);
    }

    inline void setGameRegisteredCombatSpaceRestrictors(const xr_string& space_name, CScriptGameObject* client_zone)
    {
        if (!space_name.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredCombatSpaceRestrictors(space_name, client_zone)] WARNING: "
                "space_name.size() = 0! You are trying to set an empty string! No assignemnt!");
            return;
        }

        if (!client_zone)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredCombatSpaceRestrictors(space_name, client_zone)] WARNING: "
                "client_zone = null! You are trying to set an empty object! No assignment!");
            return;
        }

        this->m_game_registered_combat_spacerestrictors[space_name] = client_zone;
    }

    inline const xr_map<xr_string, xr_string>& getJobTypesByScheme(void) const noexcept
    {
        return this->m_job_type_by_scheme;
    }

    inline void setJobTypesByScheme(const xr_map<xr_string, xr_string>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setJobTypesByScheme(map)] WARNING: map.size() = 0! You are trying to set an "
                "empty map! No assignment!");
            return;
        }

        this->m_job_type_by_scheme = map;
    }

    inline void setJobTypesByScheme(const std::pair<xr_string, xr_string>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setJobTypesByScheme(type_name, scheme_name)] WARNING: pair.first.size() = 0! You "
                "are trying to set an empty string, No assignment");
            return;
        }

        this->m_job_type_by_scheme.insert(pair);
    }

    inline void setJobTypesByScheme(const xr_string& type_name, const xr_string& scheme_name) noexcept
    {
        if (!type_name.size())
        {
            Msg("[Script_GlobalHelper/setJobTypesByScheme(type_name, scheme_name)] WARNING: type_name.size() = 0! You "
                "are trying to set an empty string, No assignment!");
            return;
        }

        this->m_job_type_by_scheme[type_name] = scheme_name;
    }

#pragma region Cordis InGame
    inline const xr_map<xr_string, Script_SE_SmartTerrain*>& getGameRegisteredServerSmartTerrainsByName(void) const
        noexcept
    {
        return this->m_game_registered_server_smartterrains_by_name;
    }

    inline void setGameRegisteredServerSmartTerrainsByName(const xr_map<xr_string, Script_SE_SmartTerrain*>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartTerrainsByName(map)] WARNING: map.size() = 0! You "
                "are trying to set an empty map! No assignment!");
            return;
        }

        this->m_game_registered_server_smartterrains_by_name = map;
    }

    inline void setGameRegisteredServerSmartTerrainsByName(const std::pair<xr_string, Script_SE_SmartTerrain*>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartTerrainsByName(name, server_smart)] WARNING: "
                "pair.first.size() = 0! You are trying to set an empty string! No assignment!");
            return;
        }

        if (!pair.second)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartTerrainsByName(name, server_smart)] WARNING: "
                "pair.second = null! You are trying to set an empty object! No assignment!");
            return;
        }

        this->m_game_registered_server_smartterrains_by_name.insert(pair);
    }

    inline void setGameRegisteredServerSmartTerrainsByName(const xr_string& name, Script_SE_SmartTerrain* server_smart)
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartTerrainsByName(name, server_smart)] WARNING: "
                "name.size() = 0! You are trying to set an empty string! No assignment!");
            return;
        }

        if (!server_smart)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartTerrainsByName(name, server_smart)] WARNING: "
                "server_smart = null! You are trying to set an empty object! No assignment!");
            return;
        }

        this->m_game_registered_server_smartterrains_by_name[name] = server_smart;
    }

    // @ First - id | Second - distance
    inline const std::pair<std::uint32_t, std::uint32_t>& getGameNearestToActorServerSmartTerrain(void) const noexcept
    {
        return this->m_game_server_nearest_to_actor_smart_terrain;
    }

    inline void setGameNearestToActorServerSmartTerrain(const std::pair<std::uint32_t, std::uint32_t>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt32Undefined || pair.second == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setGameNearestToActorServerSmartTerrain(id, distance)] WARNING: pair.first || "
                "pair.second = "
                "std::uint32_t(-1)!!! Values id or distance are not initialized at all or equals minus one!");
        }

        this->m_game_server_nearest_to_actor_smart_terrain = pair;
    }

    inline void setGameNearestToActorServerSmartTerrain(const std::uint32_t& id, const std::uint32_t& distance) noexcept
    {
        if (id == Globals::kUnsignedInt32Undefined || distance == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setGameNearestToActorServerSmartTerrain(id, distance)] WARNING: id || distance = "
                "std::uint32_t(-1)!!! Values id or distance are undefined at all or equals minus one!");
        }

        this->m_game_server_nearest_to_actor_smart_terrain.first = id;
        this->m_game_server_nearest_to_actor_smart_terrain.second = distance;
    }

    inline const xr_map<xr_string, xr_string>& getGameSmartsByAssaultZones(void) const noexcept
    {
        return this->m_game_smarts_by_no_assault_zone;
    }

    inline void setGameSmartsByAssaultZones(const xr_map<xr_string, xr_string>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setGameSmartsByAssaultZones(map)] WARNING: map.size() = 0! You are trying to set "
                "an empty map! No assignment!");
            return;
        }

        this->m_game_smarts_by_no_assault_zone = map;
    }

    inline void setGameSmartsByAssaultZones(const std::pair<xr_string, xr_string>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setGameSmartsByAssaultZones(pair)] WARNING: pair.first.size() = 0! You are "
                "trying to set an empty string! No assignment!");
            return;
        }

        this->m_game_smarts_by_no_assault_zone.insert(pair);
    }

    inline void setGameSmartsByAssaultZones(const xr_string& smart_name, const xr_string& zone_name) noexcept
    {
        if (!smart_name.size())
        {
            Msg("[Script_GlobalHelper/setGameSmartsByAssaultZones(smart_name, zone_name)] WARNING: smart_name.size() = "
                "0! You are trying to set an empty string! No assignment!");
            return;
        }

        this->m_game_smarts_by_no_assault_zone[smart_name] = zone_name;
    }
#pragma endregion

#pragma region Cordis System variables
    inline CUISequencer* getGlobalTutorial(void) const { return this->m_tutorial; }
    inline void setGlobalTutorial(CUISequencer* sequence)
    {
        if (!sequence)
        {
            Msg("[Scripts/Script_GlobalHelper/setGlobalTutorial(sequence)] WARNING: sequence = nullptr! Can't assign! "
                "Return");
            return;
        }

        this->m_tutorial = sequence;
    }

    inline const xr_map<xr_string, bool>& getRegisteredSmartTerrainTerritoryType(void) const noexcept
    {
        return this->m_registered_smart_terrain_territory_type;
    }

    inline void setRegisteredSmartTerrainTerritoryType(const xr_map<xr_string, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainTerritoryType(map)] WARNING: map.size() = 0! You are "
                "trying to set an empty map! No assignment!");
            return;
        }

        this->m_registered_smart_terrain_territory_type = map;
    }

    inline void setRegisteredSmartTerrainTerritoryType(const std::pair<xr_string, bool>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainTerritoryType(pair)] WARNING: pair.first.size() = 0! "
                "You are trying to set an empty pair! No assignment!");
            return;
        }

        this->m_registered_smart_terrain_territory_type.insert(pair);
    }

    inline void setRegisteredSmartTerrainTerritoryType(const xr_string& name, const bool& value) noexcept
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainTerritoryType(name, value)] WARNING: name.size() = 0! "
                "You are trying to set an empty string! No assignment!");
            return;
        }

        this->m_registered_smart_terrain_territory_type[name] = value;
    }

    inline const xr_vector<xr_string>& getRegisteredSmartTerrainPathFileds(void) const noexcept
    {
        return this->m_registered_smart_terrain_path_fields;
    }

    inline void setRegisteredSmartTerrainPathFields(const xr_vector<xr_string>& vector) noexcept
    {
        if (!vector.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainPathFields(vector)] WARNING: vector.size() = 0! You are "
                "trying to set an empty vector! No assigment!");
            return;
        }

        this->m_registered_smart_terrain_path_fields = vector;
    }

    inline void setRegisteredSmartTerrainPathFields(const xr_string& name) noexcept
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainFields(name)] WARNING: name.size() = 0! You are trying "
                "to set an empty string! No assigment!");
            return;
        }

        this->m_registered_smart_terrain_path_fields.push_back(name);
    }

    inline const xr_map<xr_string, std::uint32_t>& getSimulationBoardGroupIDLevelsByName(void) const noexcept
    {
        return this->m_simulationboard_group_id_by_levels_name;
    }

    inline void setSimulationBoardGroupIDLevelsByName(const xr_map<xr_string, std::uint32_t>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setSimulationBoardGroupIDLevelsByName(map)] WARNING: map.size() = 0! You are "
                "trying to set an empty map! No assignment!");
            return;
        }

        this->m_simulationboard_group_id_by_levels_name = map;
    }

    inline void setSimulationBoardGroupIDLevelsByName(const std::pair<xr_string, std::uint32_t>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setSimulationBoardGroupIDLevelsByName(pair)] WARNING: pair.first.size() = 0! you "
                "are trying to set an empty string! No assignment!");
            return;
        }

        this->m_simulationboard_group_id_by_levels_name.insert(pair);
    }

    inline void setSimulationBoardGroupIDLevelsByName(const xr_string& name, const std::uint32_t& id) noexcept
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setSimulationBoardGropIDLevelsByName(name, id)] WARNING: name.size() = 0! you "
                "are trying to set an empty string! No assignment!");
            return;
        }

        this->m_simulationboard_group_id_by_levels_name[name] = id;
    }

    inline const xr_map<xr_string, bool>& getSimulationSquadIsSquadMonster(void) const noexcept
    {
        return this->m_simulationsquad_is_squad_monster_by_type;
    }

    inline void setSimulationSquadIsSquadMonster(const xr_map<xr_string, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setSimulationSquadIsSquadMonster(map)] map.size() = 0! you are trying to set an "
                "empty map! No assignment!");
            return;
        }

        this->m_simulationsquad_is_squad_monster_by_type = map;
    }

    inline void setSimulationSquadIsSquadMonster(const std::pair<xr_string, bool>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setSimulationSquadIsSquadMonster(pair)] WARNING: pair.first.size() = 0! you are "
                "trying to set an empty string, No assignment");
            return;
        }

        this->m_simulationsquad_is_squad_monster_by_type.insert(pair);
    }

    inline void setSimulationSquadIsSquadMonster(const xr_string& name, const bool& value) noexcept
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setSimulationSquadIsSquadMonster(name, value)] WARNING: name.size() = 0! you are "
                "trying to set an empty value, No assignment");
            return;
        }

        this->m_simulationsquad_is_squad_monster_by_type[name] = value;
    }

    inline const xr_map<xr_string, Script_SmartTerrainControl_States>& getRegisteredSmartTerrainControlScriptStates(
        void) const noexcept
    {
        return this->m_registered_smart_terrain_control_script_states;
    }

    inline void setRegisteredSmartTerrainControlScriptStates(
        const xr_map<xr_string, Script_SmartTerrainControl_States>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainControlScriptStates(map)] WARNING: map.size() = 0! you "
                "are trying to set an empty map! No assignment!");
            return;
        }

        this->m_registered_smart_terrain_control_script_states = map;
    }

    inline void setRegisteredSmartTerrainControlScriptStates(
        const std::pair<xr_string, Script_SmartTerrainControl_States>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainControlScriptStates(pair)] WARNING: pair.first is empty "
                "string! You are trying to set an empty string, No assignment");
            return;
        }

        this->m_registered_smart_terrain_control_script_states.insert(pair);
    }

    inline void setRegisteredSmartTerrainControlScriptStates(
        const xr_string& name, const Script_SmartTerrainControl_States& state) noexcept
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainControlScriptStates(name, state)] WARNING: you are "
                "trying to set an empty string, No assignment");
            return;
        }

        this->m_registered_smart_terrain_control_script_states[name] = state;
    }

    inline const xr_map<xr_string, bool>& getRegisteredEatableVisuals(void) const noexcept
    {
        return this->m_registered_eatable_visuals;
    }

    inline void setRegisteredEatableVisuals(const xr_map<xr_string, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredEatableVisuals(map)] WARNING: you're set an empty map! No "
                "assignment!");
            return;
        }

        this->m_registered_eatable_visuals = map;
    }

    inline void setRegisteredEatableVisuals(const std::pair<xr_string, bool>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredEatableVisuals(pair)] WARNING: you're trying to set an empty pair "
                "to map! No assignment!");
            return;
        }

        this->m_registered_eatable_visuals.insert(pair);
    }

    inline void setRegisteredEatableVisuals(const xr_string& name, const bool& is_used) noexcept
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredEatableVisuals(name, is_used)] WARNING: name.size() = 0! (string is "
                "null!) No assignment!");
            return;
        }

        this->m_registered_eatable_visuals[name] = is_used;
    }

    inline const xr_map<xr_string, bool>& getRegisteredHarmonicaVisuals(void) const noexcept
    {
        return this->m_registered_harmonica_visuals;
    }

    inline void setRegisteredHarmonicaVisuals(const xr_map<xr_string, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredHarmonicaVisuals(map)] WARNING: map.size() = 0! You are trying to "
                "set an empty map! No assignment!");
            return;
        }

        this->m_registered_harmonica_visuals = map;
    }

    inline void setRegisteredHarmonicaVisuals(const std::pair<xr_string, bool>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredHarmonicaVisuals(pair)] WARNING: pair.first.size() = 0! You are "
                "trying to set an empty pair! No assignment!");
            return;
        }

        this->m_registered_harmonica_visuals.insert(pair);
    }

    inline void setRegisteredHarmonicaVisuals(const xr_string& visual_name, const bool& is_used) noexcept
    {
        if (!visual_name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredHarmonicaVisuals(visual_name, value)] WARNING: visual_name.size() = "
                "0! You are trying to set an empty string! No assignment!");
            return;
        }

        this->m_registered_harmonica_visuals[visual_name] = is_used;
    }

    inline const xr_map<xr_string, xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>>>&
    getAnimpointTable(void) noexcept
    {
        return this->m_animpoint_table;
    }

    inline void setAnimpointTable(
        const xr_map<xr_string, xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>>>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setAnimpointTable(pair)] WARNING: "
                "map.size() = 0! You are trying to set an empty string! No assignment!");
            return;
        }

        this->m_animpoint_table = map;
    }

    inline const xr_map<xr_string, StateLibData>& getStateLibrary(void) const noexcept { return this->m_state_library; }
    inline void setStateLibrary(const xr_map<xr_string, StateLibData>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage/setStateLibrary(map)] WARNING: map.size() = 0! You are trying to set an empty map! "
                "No assignment!");
            return;
        }

        this->m_state_library = map;
    }

    inline void setStateLibrary(const std::pair<xr_string, StateLibData>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[DataBase/Storage/setStateLibrary(pair)] WARNING: pair.first.size() = 0! You are trying to set an "
                "empty pair! No assignment!");
            return;
        }

        this->m_state_library.insert(pair);
    }

    inline void setStateLibrary(const xr_string& animation_name, const StateLibData& data) noexcept
    {
        if (!animation_name.size())
        {
            Msg("[DataBase/Storage/setStateLibrary(animation_name, data)] WARNING: animation_name.size() = 0! You are "
                "trying to set an empty string! No assignment!");
            return;
        }

        this->m_state_library[animation_name] = data;
    }

    /*
        inline void setAnimpointTable(
            const std::pair<xr_string, std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>>& pair)
        {
            if (!pair.first.size())
            {
                Msg("[Script_GlobalHelper/setAnimpointTable(pair)] WARNING: "
                    "pair.first.size() = 0! You are trying to set an empty string! No assignment!");
                return;
            }

            if (!pair.second.first)
            {
                Msg("[Script_GlobalHelper/setAnimpointTable(pair)] WARNING: "
                    "pair.second.first = null! You are trying to set an empty function! No assignment!");
                return;
            }

            if (!pair.second.second.size())
            {
                Msg("[Script_GlobalHelper/setAnimpointTable(pair)] WARNING: "
                    "pair.second.second.size() = 0! You are trying to set an empty string! No assignment!");
                return;
            }

            this->m_animpoint_table.insert(pair);
        }*/

    inline void setAnimpointTable(
        const xr_string& animpoint_name, const std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>& pair)
    {
        if (!animpoint_name.size())
        {
            Msg("[Script_GlobalHelper/setAnimpointTable(animpoint_name, function, animpoint_animation_name)] WARNING: "
                "animation_name.size() = 0! You are trying to set an empty string! No assignment!");
            return;
        }

        if (!pair.first)
        {
            Msg("[Script_GlobalHelper/setAnimpointTable(animpoint_name, function, animpoint_animation_name)] WARNING: "
                "pair.first = null! You are trying to set an empty function! No assignment!");
            return;
        }

        if (!pair.second.size())
        {
            Msg("[Script_GlobalHelper/setAnimpointTable(animpoint_name, function, animpoint_animation_name)] WARNING: "
                "pair.second.size() = 0! You are trying to set an empty string! No assignment!");
            return;
        }

        this->m_animpoint_table[animpoint_name].push_back(pair);
    }

    inline void setAnimpointTable(const xr_string& animpoint_name,
        const std::function<bool(std::uint16_t, bool)> function, const xr_string& animpoint_animation_name)
    {
        if (!animpoint_name.size())
        {
            Msg("[Script_GlobalHelper/setAnimpointTable(animpoint_name, function, animpoint_animation_name)] WARNING: "
                "animation_name.size() = 0! You are trying to set an empty string! No assignment!");
            return;
        }

        if (!function)
        {
            Msg("[Script_GlobalHelper/setAnimpointTable(animpoint_name, function, animpoint_animation_name)] WARNING: "
                "function = null! You are trying to set an empty function! No assignment!");
            return;
        }

        if (!animpoint_animation_name.size())
        {
            Msg("[Script_GlobalHelper/setAnimpointTable(animpoint_name, function, animpoint_animation_name)] WARNING: "
                "animpoint_animation_name.size() = 0! You are trying to set an empty string! No assignment!");
            return;
        }

        this->m_animpoint_table[animpoint_name].push_back({function, animpoint_animation_name});
    }
#pragma endregion

private:
    CUISequencer* m_tutorial;
    xr_map<std::uint32_t, bool> m_monster_classes;
    xr_map<std::uint32_t, bool> m_stalker_classes;
    xr_map<std::uint32_t, bool> m_weapon_classes;
    xr_map<std::uint32_t, bool> m_artefact_classes;
    xr_map<std::uint32_t, bool> m_ammo_section;
    xr_map<xr_string, bool> m_registered_eatable_visuals;
    xr_map<xr_string, bool> m_registered_harmonica_visuals;
    xr_map<xr_string, bool> m_quest_section;
    xr_map<xr_string, Script_SmartTerrainControl_States> m_registered_smart_terrain_control_script_states;
    xr_map<xr_string, xr_string> m_squad_community_by_behavior;
    xr_map<xr_string, xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>>> m_animpoint_table;
    xr_map<xr_string, AnyCallable<void>> m_registered_functions_xr_effects;
    xr_map<xr_string, AnyCallable<bool>> m_registered_functions_xr_conditions;
    xr_map<xr_string, Script_SE_SmartCover*> m_game_registered_smartcovers;
    xr_map<std::uint8_t, xr_map<std::uint32_t, Script_SE_SmartCover*>> m_game_registered_smartcovers_by_level_id;
    xr_map<xr_string, StateLibData> m_state_library;
    xr_map<xr_string, SmartCoverData> m_registered_smartcovers;
    xr_map<xr_string, CScriptGameObject*> m_game_registered_combat_spacerestrictors;
    xr_map<xr_string, Script_SE_SmartTerrain*> m_game_registered_server_smartterrains_by_name;
    xr_map<xr_string, xr_string> m_job_type_by_scheme;
    xr_map<xr_string, xr_string> m_game_smarts_by_no_assault_zone;
    xr_map<xr_string, std::uint32_t> m_simulationboard_group_id_by_levels_name;
    xr_map<xr_string, bool> m_registered_smart_terrain_territory_type;
    xr_map<xr_string, bool> m_simulationsquad_is_squad_monster_by_type;
    // @ First - id | Second - distance
    std::pair<std::uint32_t, std::uint32_t> m_game_server_nearest_to_actor_smart_terrain;
    xr_vector<xr_string> m_registered_smart_terrain_path_fields;
};

} // namespace Scripts
} // namespace Cordis
