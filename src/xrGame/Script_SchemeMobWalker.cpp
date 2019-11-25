#include "stdafx.h"
#include "Script_SchemeMobWalker.h"

namespace Cordis
{
namespace Scripts
{
const std::uint32_t default_wait_time = 5000;
const std::uint32_t default_anim_standing = MonsterSpace::eAA_StandIdle;
const bool state_moving = false;
const bool state_standing = true;

Script_SchemeMobWalker::Script_SchemeMobWalker(CScriptGameObject* p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_is_crouch(false), m_is_running(false), m_last_index(0),
      m_last_look_index(0), m_patrol_walk(nullptr), m_patrol_look(nullptr)
{
    this->m_scheme_name = "mob_walker";
}

Script_SchemeMobWalker::~Script_SchemeMobWalker(void)
{
    if (this->m_patrol_walk)
        xr_delete(this->m_patrol_walk);

    if (this->m_patrol_look)
        xr_delete(this->m_patrol_look);
}

void Script_SchemeMobWalker::reset_scheme(void)
{
    Msg("[Scripts/Script_SchemeMobWalker/reset_scheme()] %s", this->m_npc->Name());

    Script_MobStateManager::getInstance().set_state(this->m_npc, this->m_storage->getStateName());

    if (this->m_patrol_walk)
        xr_delete(this->m_patrol_walk);

    this->m_patrol_walk = new CPatrolPathParams(this->m_storage->getPathWalkName().c_str());
    if (!this->m_patrol_walk->m_path)
    {
        R_ASSERT2(false, "Unable to find the path!");
    }

    if (!this->m_storage->getPathLookName().empty())
    {
        if (this->m_patrol_look)
            xr_delete(this->m_patrol_look);

        this->m_patrol_look = new CPatrolPathParams(this->m_storage->getPathLookName().c_str());
        if (!this->m_patrol_look->m_path)
        {
            R_ASSERT2(false, "Unable to find the path!");
        }
    }

    if (this->m_storage->getPathWalkInfo().getData().empty())
    {
        this->m_storage->setPathWalkInfo(Globals::Utils::path_parse_waypoints(this->m_storage->getPathWalkName()));
        this->m_path_walk_info = this->m_storage->getPathWalkInfo();
    }

    if (this->m_storage->getPathLookInfo().getData().empty())
    {
        this->m_storage->setPathLookInfo(Globals::Utils::path_parse_waypoints(this->m_storage->getPathLookName()));
        this->m_path_look_info = this->m_storage->getPathLookInfo();
    }

    this->m_state = state_moving;
    this->m_is_crouch = false;
    this->m_is_running = false;
    this->m_current_animation_set = default_anim_standing;
    this->m_patrol_wait_time = default_wait_time;
    this->m_sheduled_sound_name.clear();

    this->m_last_index = 0;
    this->m_last_look_index = 0;

    Globals::action(this->m_npc,
        CScriptMovementAction(MonsterSpace::eMA_WalkFwd,
            new CPatrolPathParams(
                this->m_storage->getPathWalkName().c_str(), ePatrolStartTypeNext, ePatrolRouteTypeContinue)),
        CScriptActionCondition(CScriptActionCondition::MOVEMENT_FLAG));
}

void Script_SchemeMobWalker::update(const float delta)
{
    if (!XR_LOGIC::is_mob_captured(this->m_npc))
    {
        this->reset_scheme();
        return;
    }

    if (this->m_state == state_standing)
    {
        if (!this->m_npc->GetCurrentAction())
        {
            std::uint32_t patrol_walk_count = this->m_patrol_walk->count();

            if (patrol_walk_count == 1 && Globals::Utils::is_stalker_at_waypoint(this->m_npc, *this->m_patrol_walk, 0))
            {
                this->m_state = state_moving;
            }
        }
    }
}

void Script_SchemeMobWalker::waypoint_callback(
    CScriptGameObject* p_client_object, const std::uint32_t action_movement_type, const std::uint32_t index)
{
    Msg("[Scripts/Script_SchemeMobWalker/waypoint_callback(p_client_object, action_movement_type, index)] name = %s | "
        "index = %d",
        this->m_npc->Name(), index);

    if (index == Globals::kUnsignedInt32Undefined || !index)
    {
        Msg("[Scripts/Script_SchemeMobWalker/waypoint_callback(p_client_object, action_movement_type, index)] WARNING: "
            "index equals std::uint32_t(-1) or 0! Return ...");
        return;
    }

    this->m_last_index = index;

    if (!(index > this->m_path_walk_info.getData().size()))
    {
        Msg("[Scripts/Script_SchemeMobWalker/waypoint_callback(p_client_object, action_movement_type, index)] WARNING: "
            "can't index out of range, return ...");
        xr_string suggested_sound_name = this->m_path_walk_info.getData().at(index).getValue("s");
        if (!suggested_sound_name.empty())
        {
            this->m_sheduled_sound_name = suggested_sound_name;
        }

        xr_string suggested_crouch = this->m_path_walk_info.getData().at(index).getValue("c");
        if (suggested_crouch == Globals::kStringTrue)
            this->m_is_crouch = true;
        else
            this->m_is_crouch = false;

        xr_string suggested_running = this->m_path_walk_info.getData().at(index).getValue("r");
        if (suggested_running == Globals::kStringTrue)
            this->m_is_running = true;
        else
            this->m_is_running = false;

        xr_string signal_name = this->m_path_walk_info.getData().at(index).getValue("sig");
        if (!signal_name.empty())
        {
            // NOT IMPLEMENTED, see original scripts
            Msg("[Scripts/Script_ScheemMobWalker/waypoint_callback(p_client_object, action_movement_type, index)] "
                "WARNING: not implemented, signal handler");
        }

        xr_string monster_state_name = this->m_path_walk_info.getData().at(index).getValue("b");
        if (!monster_state_name.empty())
        {
            Script_MobStateManager::getInstance().set_state(this->m_npc, monster_state_name);
        }
        else
        {
            Script_MobStateManager::getInstance().set_state(this->m_npc, this->m_storage->getStateName());
        }

        Flags32 search_for = this->m_path_walk_info.getData().at(index).getFlags();
        if (!search_for.get())
        {
            Msg("[Scripts/Script_SchemeMobWalker/waypoint_callback(p_client_object, action_movement_type, index)] no "
                "flags. update_movement_state");
            this->update_movement_state();
            return;
        }

        std::uint32_t point_choosen_index =
            Globals::choose_look_point(this->m_patrol_look, this->m_path_look_info, search_for);

        if (point_choosen_index != Globals::kUnsignedInt32Undefined)
        {
            xr_string suggested_wait_time_name = this->m_path_look_info.getData().at(point_choosen_index).getValue("t");
            if (!suggested_wait_time_name.empty())
            {
                this->m_point_wait_time = atoi(suggested_wait_time_name.c_str());
            }
            else
            {
                std::uint32_t patrol_walk_count = this->m_patrol_walk->count();
                if (patrol_walk_count == 1 &&
                    Globals::Utils::is_stalker_at_waypoint(this->m_npc, *this->m_patrol_walk, 0))
                {
                    this->m_point_wait_time = Globals::kTimeInfinite;
                }
                else
                {
                    this->m_point_wait_time = default_wait_time;
                }
            }

            xr_string suggested_animation_set_name = this->m_path_look_info.getData().at(index).getValue("a");
            if (!suggested_animation_set_name.empty())
            {
                if (suggested_animation_set_name == "nil")
                    suggested_animation_set_name.clear();
                // Lord: протестировать!
                this->m_current_animation_set =
                    Script_GlobalHelper::getInstance().getMonsterAnimationToAction(suggested_animation_set_name);
            }
            else
            {
                this->m_current_animation_set = default_anim_standing;
            }

            xr_string monster_statename = this->m_path_walk_info.getData().at(index).getValue("b");
            if (!monster_statename.empty())
            {
                Script_MobStateManager::getInstance().set_state(this->m_npc, monster_statename);
            }
            else
            {
                Script_MobStateManager::getInstance().set_state(this->m_npc, monster_statename);
            }

            if (point_choosen_index != this->m_last_look_index)
                this->look_at_waypoint(point_choosen_index);

            this->m_state = state_standing;
            this->update_standing_state();

            this->update(1.0f);
        }
        else
        {
            R_ASSERT2(false, "can't find a point!");
        }
    }
}

} // namespace Scripts
} // namespace Cordis
