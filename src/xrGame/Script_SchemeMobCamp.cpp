#include "stdafx.h"
#include "Script_SchemeMobCamp.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeMobCamp::Script_SchemeMobCamp(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_path_look(nullptr), m_path_home(nullptr), m_current_point_index(0),
      m_time_point_changed(0), m_state_previous(0), m_state_current(0), m_is_previous_enemy(false), m_camp_node(0)
{
}

Script_SchemeMobCamp::~Script_SchemeMobCamp(void)
{
    if (this->m_path_home)
    {
        Msg("[Scripts/Script_SchemeMobCamp/~dtor()] deleting path_home");
        xr_delete(this->m_path_home);
    }

    if (this->m_path_look)
    {
        Msg("[SCripts/Script_SchemeMobCamp/~dtor()] deleting path_look");
        xr_delete(this->m_path_look);
    }
}

void Script_SchemeMobCamp::reset_scheme(const bool)
{
    Msg("[Scripts/Script_SchemeMobCamp/reset_scheme()] %s", this->m_npc->Name());

    XR_LOGIC::mob_capture(this->m_npc, true, this->m_scheme_name);
    this->m_storage->ClearSignals();

    this->m_path_look = new CPatrolPathParams(this->m_storage->getLookPointName().c_str());

    if (!this->m_path_look->m_path)
    {
        R_ASSERT2(false, "Can't find look_point!");
    }

    if (!this->m_storage->getHomePointName().empty())
    {
        this->m_path_home = new CPatrolPathParams(this->m_storage->getHomePointName().c_str());

        if (!this->m_path_home->m_path)
        {
            R_ASSERT2(false, "Cam't find home_point");
        }
    }

    if (this->m_path_home)
    {
        if (this->m_path_home->count() != this->m_path_look->count())
        {
            R_ASSERT2(false, "you must setup home path points count must be equal to look path points count!");
        }
    }

    this->m_camp_position = this->m_npc->Position();
    this->m_camp_node = this->m_npc->level_vertex_id();

    this->m_state_current = STATE_CAMP;
    this->m_state_previous = this->m_state_current;

    this->m_current_point_index = 0;
    this->select_current_home_point(true);

    this->m_time_point_changed = Globals::get_time_global();

    this->m_is_previous_enemy = false;

    if (this->m_storage->IsSkipTransferEnemy())
        this->m_npc->skip_transfer_enemy(true);
}

void Script_SchemeMobCamp::update(const float delta)
{
    // Lord: доделать когда будет сделан XR_LOGIC::try_switch_to_another_section

    if (!this->m_npc->Alive())
    {
        XR_LOGIC::mob_release(this->m_npc, this->m_scheme_name);
        return;
    }

    if ((this->m_time_point_changed + this->m_storage->getTimeChangePoint() < Globals::get_time_global()))
    {
        this->select_current_home_point(false);
        this->m_time_point_changed = Globals::get_time_global();
    }

    this->select_state();
    this->execute_state();
}

void Script_SchemeMobCamp::deactivate(void)
{
    if (this->m_path_home)
    {
        // Lord: проверить правильно что мы сюда прибавляем единицу
        // Lord: проверить везде на правильное использование this->m_current_point_index
        DataBase::Storage::getInstance().setCampStorage(
            this->m_storage->getHomePointName(), this->m_current_point_index + 1, false);
    }

    this->m_npc->skip_transfer_enemy(false);
}

void Script_SchemeMobCamp::net_destroy(CScriptGameObject* const p_client_object)
{
    if (this->m_path_home)
    {
        // Lord: проверить правильно что мы сюда прибавляем единицу
        // Lord: проверить везде на правильное использование this->m_current_point_index
        DataBase::Storage::getInstance().setCampStorage(
            this->m_storage->getHomePointName(), this->m_current_point_index + 1, false);
    }

    this->m_npc->skip_transfer_enemy(false);
}

void Script_SchemeMobCamp::execute_state(void)
{
    if (this->m_state_current == STATE_ALIFE && this->m_state_previous == STATE_ALIFE)
        return;

    if (this->m_state_current == STATE_ALIFE && this->m_state_previous != STATE_ALIFE)
    {
        XR_LOGIC::mob_release(this->m_npc, this->m_scheme_name);
        return;
    }

    if (this->m_state_current != STATE_ALIFE && this->m_state_previous == STATE_ALIFE)
    {
        XR_LOGIC::mob_capture(this->m_npc, true, this->m_scheme_name);
    }

    if (this->m_state_current == STATE_CAMP)
    {
        if (!this->m_npc->GetCurrentAction())
        {
            Globals::action(this->m_npc, CScriptAnimationAction(eAA_StandIdle, 0), CScriptWatchAction(SightManager::eSightTypePosition, this->m_path_look->point(this->m_current_point_index)), CScriptActionCondition(CScriptActionCondition::WATCH_FLAG));
        }

        return;
    }
}

void Script_SchemeMobCamp::select_state(void)
{
    this->m_state_previous = this->m_state_current;

    Fvector home_position = this->m_camp_position;
    std::uint32_t home_node = this->m_camp_node;

    if (this->m_path_home)
    {
        home_position = this->m_path_home->point(this->m_current_point_index);
        home_node = this->m_path_home->level_vertex_id(this->m_current_point_index);
    }

    CScriptGameObject* const p_client_enemy = this->m_npc->GetEnemy();

    if (p_client_enemy)
    {
        if (!this->m_is_previous_enemy)
        {
            this->m_storage->setSignals("enemy", true);
        }

        this->m_is_previous_enemy = true;
    }
    else
    {
        this->m_is_previous_enemy = false;
    }

    if (p_client_enemy)
    {
        float enemy_distance = p_client_enemy->Position().distance_to(home_position);

        if (this->m_state_previous == STATE_MOVE_HOME && (enemy_distance > this->m_storage->getHomeMinRadius()))
        {
            // empty code
        }
        else if (this->m_state_previous == STATE_ALIFE && (enemy_distance > this->m_storage->getHomeMaxRadius()))
        {
            this->m_state_current = STATE_MOVE_HOME;
        }
        else if (this->m_state_previous == STATE_CAMP && (enemy_distance > this->m_storage->getHomeMinRadius()))
        {
            // empty code
        }
        else
        {
            this->m_state_current = STATE_ALIFE;
        }
    }

    if ((!p_client_enemy) || (p_client_enemy && (this->m_state_current != STATE_ALIFE)))
    {
        if ((home_position.distance_to(this->m_npc->Position()) > 1.0f) &&
            (home_node != this->m_npc->level_vertex_id()))

        {
            this->m_state_current = STATE_MOVE_HOME;
        }
        else
        {
            this->m_state_current = STATE_CAMP;
        }
    }

    const CScriptMonsterHitInfo& hit_data = this->m_npc->GetMonsterHitInfo();

    if ((!p_client_enemy) && hit_data.who && (hit_data.time != 0) &&
        (Globals::IsMonster(hit_data.who) || Globals::IsStalker(hit_data.who)))
    {
        float distance = this->m_npc->Position().distance_to(home_position);

        if (distance < this->m_storage->getHomeMinRadius())
            this->m_state_current = STATE_ALIFE;
    }
}

void Script_SchemeMobCamp::select_current_home_point(const bool is_first_call)
{
    std::uint32_t previous_point_index = this->m_current_point_index;

    if (this->m_path_home)
    {
        xr_vector<std::uint32_t> free_points_indecies;

        for (std::uint32_t i = 0; i < this->m_path_home->count(); ++i)
        {
            if (!DataBase::Storage::getInstance().getCampStorage().at(this->m_storage->getHomePointName()).at(i))
            {
                free_points_indecies.push_back(i);
            }

            if (free_points_indecies.size() < 1)
            {
                if (is_first_call)
                {
                    R_ASSERT2(false, "too many campers for home path");
                }
            }
            else
            {
                std::uint32_t free_point_random_index = Globals::Script_RandomInt::getInstance().Generate(
                    std::uint32_t(0), free_points_indecies.size() - 1);
                this->m_current_point_index = free_points_indecies[free_point_random_index] - 1; // Lord: проверить
            }
        }

        if (!is_first_call)
        {
            if (previous_point_index != this->m_current_point_index)
            {
                DataBase::Storage::getInstance().setCampStorage(
                    this->m_storage->getHomePointName(), previous_point_index + 1, false);
            }
        }

        DataBase::Storage::getInstance().setCampStorage(
            this->m_storage->getHomePointName(), this->m_current_point_index + 1, true);
    }
    else
    {
        this->m_current_point_index = Globals::Script_RandomInt::getInstance().Generate(
            std::uint32_t(0), this->m_path_look->count() - 1); // Lord: проверить
    }
}

} // namespace Scripts
} // namespace Cordis
