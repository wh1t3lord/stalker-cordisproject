#include "stdafx.h"
#include "Script_SchemeMobWalker.h"

namespace Cordis
{
namespace Scripts
{
const std::uint16_t default_wait_time = 5000;
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

} // namespace Scripts
} // namespace Cordis
