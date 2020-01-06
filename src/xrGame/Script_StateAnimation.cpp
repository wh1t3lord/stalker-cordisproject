#include "stdafx.h"
#include "Script_StateAnimation.h"

namespace Cordis
{
namespace Scripts
{
constexpr std::uint8_t kAnimationMarkerIn = 1;
constexpr std::uint8_t kAnimationMarkerOut = 2;
constexpr std::uint8_t kAnimationMarkerIdle = 3;

Script_StateAnimation::Script_StateAnimation(
    CScriptGameObject* const p_client_object, Script_StateManager& state_manager, const bool is_animlist_access)
    : m_is_using_animation_list(is_animlist_access), m_p_client_object(p_client_object),
      m_p_state_manager(&state_manager), m_is_control_called(false)
{
    if (is_animlist_access)
        this->m_current_data_list = Script_GlobalHelper::getInstance().getStateManagerAnimationList();
    else
        this->m_current_data_list = Script_GlobalHelper::getInstance().getStateManagerAnimationStateList();

    if (this->m_current_data_list.empty())
    {
        R_ASSERT2(false, "something goes very bad! You don't initialize those lists");
    }
}

Script_StateAnimation::~Script_StateAnimation(void) {}

void Script_StateAnimation::set_state(const xr_string& new_state_name, const bool is_fast_set)
{
    if (is_fast_set)
    {
        this->m_p_client_object->clear_animations();

        StateManagerAnimationData state;

        if (this->m_states.getAnimationMarker() == kAnimationMarkerIn)
            state = this->m_current_data_list.at(this->m_states.getTargetStateName());
        else
            state = this->m_current_data_list.at(this->m_states.getCurrentStateName());

        if (state.isInitialized() && state.isAnimationListExist("out"))
        {
            int weapon_slot_id = this->weapon_slot();
            xr_vector<StateManagerAnimationData::AnimationData>& animations =
                this->anim_for_slot(weapon_slot_id, state.getAnimationList("out"));

            if (!animations.empty())
            {
                for (StateManagerAnimationData::AnimationData& it : animations)
                {
                    if (it.getAnimationName().empty())
                    {
                        this->process_special_action(it);
                    }
                }
            }
        }

        this->m_states.setAnimationMarker(0);

        this->m_states.setCurrentStateName(new_state_name);
        this->m_states.setSequenceID(Globals::kUnsignedInt32Undefined);
    }

    this->m_states.setTargetStateName(new_state_name);
    this->m_states.setNextRandom(Globals::get_time_global());
}

void Script_StateAnimation::set_control(void)
{
    this->m_is_control_called = true;
    if (this->m_is_using_animation_list)
        this->m_p_state_manager->getAnimState()->setStatesAnimationMarker(0);

    if (!this->m_states.getAnimationMarker())
        this->update_anim();
}

void Script_StateAnimation::update_anim(void)
{
    const std::pair<xr_vector<StateManagerAnimationData::AnimationData>, StateManagerAnimationData>& animation_data =
        this->select_animation();

    if (!animation_data.first.empty())
    {
        this->add_animation(animation_data.first, animation_data.second);
    }
}

std::pair<xr_vector<StateManagerAnimationData::AnimationData>, StateManagerAnimationData>
Script_StateAnimation::select_animation(void)
{
    std::pair<xr_vector<StateManagerAnimationData::AnimationData>, StateManagerAnimationData> result;

    if (this->m_states.getTargetStateName() != this->m_states.getCurrentStateName())
    {
        if (this->m_states.getTargetStateName().empty())
        {
            if (!this->m_current_data_list.at(this->m_states.getCurrentStateName()).isAnimationListExist("out"))
            {
                this->m_states.setAnimationMarker(kAnimationMarkerOut);
                this->animation_callback(true);
                return result;
            }

            const xr_map<std::uint32_t, xr_vector<StateManagerAnimationData::AnimationData>>& out_animation_data =
                this->m_current_data_list.at(this->m_states.getCurrentStateName()).getAnimationList("out");

            this->m_states.setAnimationMarker(kAnimationMarkerOut);
            int weapon_slot_id = this->weapon_slot();
            xr_vector<StateManagerAnimationData::AnimationData>& animations =
                this->anim_for_slot(weapon_slot_id, out_animation_data);

            if (animations.empty())
            {
                this->m_states.setAnimationMarker(kAnimationMarkerOut);
                this->animation_callback(true);
                return result;
            }

            if (this->m_states.getSequenceID() && animations.size() &&
                this->m_states.getSequenceID() >= animations.size())
            {
                R_ASSERT2(false, "can't get an element!");
                return result;
            }

            StateManagerAnimationData::AnimationData& next_animation = animations[this->m_states.getSequenceID()];

            if (next_animation.getAnimationName().empty())
            {
                this->process_special_action(next_animation);
                this->animation_callback(false);
                return result;
            }

            result.first.push_back(next_animation);
            result.second = this->m_current_data_list.at(this->m_states.getCurrentStateName());

            return result;
        }

        if (this->m_states.getCurrentStateName().empty())
        {
            if (!this->m_current_data_list.at(this->m_states.getCurrentStateName()).isAnimationListExist("into"))
            {
                this->m_states.setAnimationMarker(kAnimationMarkerIn);
                this->animation_callback(true);
                return result;
            }

            const xr_map<std::uint32_t, xr_vector<StateManagerAnimationData::AnimationData>>& into_animation_data =
                this->m_current_data_list.at(this->m_states.getCurrentStateName()).getAnimationList("into");

            this->m_states.setAnimationMarker(kAnimationMarkerIn);
            int weapon_slot_id = this->weapon_slot();
            xr_vector<StateManagerAnimationData::AnimationData>& animations =
                this->anim_for_slot(weapon_slot_id, into_animation_data);

            if (animations.empty())
            {
                this->m_states.setAnimationMarker(kAnimationMarkerIn);
                this->animation_callback(true);
                return result;
            }

            if (this->m_states.getSequenceID() && animations.size() &&
                this->m_states.getSequenceID() >= animations.size())
            {
                R_ASSERT2(false, "can't get access!");
                return result;
            }

            StateManagerAnimationData::AnimationData& next_animation = animations[this->m_states.getSequenceID()];
            if (next_animation.getAnimationName().empty())
            {
                this->process_special_action(next_animation);
                this->animation_callback(false);
                return result;
            }

            result.first.push_back(next_animation);
            result.second = this->m_current_data_list.at(this->m_states.getCurrentStateName());

            return result;
        }
    }

    if (!this->m_states.getCurrentStateName().empty() &&
        this->m_states.getTargetStateName() == this->m_states.getCurrentStateName())
    {
        int weapon_slot_id = this->weapon_slot();
        const StateManagerAnimationData& state = this->m_current_data_list.at(this->m_states.getCurrentStateName());
        StateManagerAnimationData::AnimationData animation;
        xr_vector<StateManagerAnimationData::AnimationData> _animation_result;
        if (!state.getAnimationList("rnd").empty())
        {
            animation = this->select_rnd(state.getAnimationList("rnd"), weapon_slot_id,
                Globals::get_time_global() >= this->m_states.getNextRandom());
            _animation_result.push_back(animation);
        }

        if (_animation_result.empty() && !state.getAnimationList("idle").empty())
        {
            _animation_result = this->anim_for_slot(weapon_slot_id, state.getAnimationList("idle"));
        }

        if (!_animation_result.empty())
            this->m_states.setAnimationMarker(kAnimationMarkerIdle);

        result.first = _animation_result;
        result.second = state;

        return result;
    }

    R_ASSERT2(false, "unaccessable code!!!");

    return result;
}

int Script_StateAnimation::weapon_slot(void)
{
    CScriptGameObject* const p_client_weapon = this->m_p_client_object->GetActiveItem();
    if (!p_client_weapon || this->m_p_client_object->weapon_strapped())
    {
        return 0;
    }
    return p_client_weapon->animation_slot();
}

void Script_StateAnimation::animation_callback(const bool is_skip_multianim_check)
{
    if (this->m_p_client_object->animation_count() != 0)
    {
        return;
    }

    if (this->m_states.getAnimationMarker() == kAnimationMarkerIn)
    {
        this->m_states.setAnimationMarker(0);

        if (!is_skip_multianim_check)
        {
            xr_vector<StateManagerAnimationData::AnimationData> into_table;
            const StateManagerAnimationData& target_anims =
                this->m_current_data_list.at(this->m_states.getTargetStateName());
            if (target_anims.isInitialized() && target_anims.isAnimationListExist("into"))
            {
                int weapon_slot_id = this->weapon_slot();
                into_table = this->anim_for_slot(weapon_slot_id, target_anims.getAnimationList("into"));
            }
            // Lord: проверить норм
            if (!into_table.empty() && into_table.size() > this->m_states.getSequenceID())
            {
                std::uint32_t new_value = this->m_states.getSequenceID();
                ++new_value;
                this->m_states.setSequenceID(new_value);
                this->update_anim();
                return;
            }
        }
        // Lord: проверить норм ли используется для обращение к элементу вектора наверное должно быть 0 а не 1
        this->m_states.setSequenceID(0);
        this->m_states.setCurrentStateName(this->m_states.getTargetStateName());
        this->update_anim();
        return;
    }

    if (this->m_states.getAnimationMarker() == kAnimationMarkerIdle)
    {
        this->m_states.setAnimationMarker(0);
        const StateManagerAnimationData& state = this->m_current_data_list.at(this->m_states.getCurrentStateName());

        if (!state.getPropertiesMaxIdle())
        {
            this->m_states.setNextRandom(Globals::get_time_global() + (state.getPropertiesSumIdle() * 1000));
        }
        else
        {
            this->m_states.setNextRandom(Globals::get_time_global() +
                (state.getPropertiesSumIdle() +
                    (Globals::Script_RandomInt::getInstance().Generate(std::uint32_t(1), state.getPropertiesMaxIdle()) *
                        1000)));
        }

        this->update_anim();
        return;
    }

    if (this->m_states.getAnimationMarker() == kAnimationMarkerOut)
    {
        this->m_states.setAnimationMarker(0);

        if (!is_skip_multianim_check)
        {
            xr_vector<StateManagerAnimationData::AnimationData> out_table;

            if (this->m_current_data_list.at(this->m_states.getCurrentStateName()).isAnimationListExist("out"))
            {
                int weapon_slot_id = this->weapon_slot();
                out_table = this->anim_for_slot(weapon_slot_id,
                    this->m_current_data_list.at(this->m_states.getCurrentStateName()).getAnimationList("out"));
            }

            if (!out_table.empty() && out_table.size() > this->m_states.getSequenceID())
            {
                std::uint32_t new_value = this->m_states.getSequenceID();
                ++new_value;
                this->m_states.setSequenceID(new_value);
                this->update_anim();
                return;
            }
        }

        this->m_states.setSequenceID(0);
        this->m_states.setCurrentStateName("");

        if (this->m_is_using_animation_list)
        {
            if (this->m_p_state_manager->getAnimState())
                this->set_control();
        }
    }
}

void Script_StateAnimation::process_special_action(StateManagerAnimationData::AnimationData& animation_data)
{
    std::uint8_t once_called = 0; // must be equals 1, потому что по логике пыс каждый такой объект animationdata
                                  // обладает только одним атрибутом!!!! И соответственно мы его только и вызываем один
                                  // единственный раз нет такого что есть функция и attachable_item
    if (!animation_data.getAttachItemName().empty())
    {
        Msg("[Scripts/Script_StateAnimation/process_special_action(animation_data)] item %s attached",
            animation_data.getAttachItemName().c_str());

        CScriptGameObject* const p_client_attach_item =
            this->m_p_client_object->GetObjectByName(animation_data.getAttachItemName().c_str());

        if (p_client_attach_item)
            p_client_attach_item->enable_attachable_item(true);

        ++once_called;
    }

    if (!animation_data.getDetachItemName().empty())
    {
        Msg("[Scripts/Script_StateAnimation/process_speacial_action(animation_data)] item %s detached");
        CScriptGameObject* const p_client_detach_item =
            this->m_p_client_object->GetObjectByName(animation_data.getDetachItemName().c_str());

        if (p_client_detach_item)
            p_client_detach_item->enable_attachable_item(true);

        ++once_called;
    }

    if (!animation_data.getSoundName().empty())
    {
        xr_string faction_name;
        XR_SOUND::set_sound_play(this->m_p_client_object->ID(), animation_data.getSoundName(), faction_name, 0);
        ++once_called;
    }

    if (!fis_zero(animation_data.getHitPower()))
    {
        CScriptHit hit;
        hit.m_fPower = animation_data.getHitPower();
        float angle = 90.0f;
        hit.m_tDirection = Globals::vector_rotate_y(this->m_p_client_object->Direction(), angle);
        hit.m_tpDraftsman = this->m_p_client_object;
        hit.m_fImpulse = 200.0f;
        hit.m_tHitType = ALife::EHitType::eHitTypeWound;
        this->m_p_client_object->Hit(&hit);

        ++once_called;
    }

    if (animation_data.isFunctionExist())
    {
        animation_data.CallFunction(this->m_p_client_object);
        ++once_called;
    }

    if (once_called > 1)
    {
        R_ASSERT2(false, "something goes wrong!");
    }
}

void Script_StateAnimation::add_animation(
    const xr_vector<StateManagerAnimationData::AnimationData>& animations, const StateManagerAnimationData& state)
{
    if (animations.size() > 1 || !animations.size())
    {
        R_ASSERT2(false, "it can't be!");
        return;
    }

    if (animations[0].getAnimationName().empty())
    {
        R_ASSERT2(false, "in this section we use only string data!!!!");
        return;
    }

    if (!this->m_p_client_object->weapon_unstrapped() || !this->m_p_client_object->weapon_strapped())
    {
        R_ASSERT2(false, "illegal call for adding animation, weapon is strapping now!");
    }

    if (!state.isPropertiesInitialized() || this->m_p_state_manager->isPositionDirectionApplied())
    {
        this->m_p_client_object->add_animation(animations[0].getAnimationName().c_str(), true, true);
    }
    else
    {
        if (Globals::is_vector_nil(this->m_p_state_manager->getAnimationDirection()))
        {
            R_ASSERT2(false, "Animation direction missing, something is not right!");
        }

        float angle_rotate_y = -rad2deg(atan2(
            this->m_p_state_manager->getAnimationDirection().x, this->m_p_state_manager->getAnimationDirection().z));

        this->m_p_client_object->add_animation(animations[0].getAnimationName().c_str(), true,
            this->m_p_state_manager->getAnimationPosition(), Fvector().set(0.0f, angle_rotate_y, 0.0f), false);

        this->m_p_state_manager->setPositionDirectionApplied(true);
    }
}

xr_vector<StateManagerAnimationData::AnimationData> Script_StateAnimation::anim_for_slot(
    int& weapon_slot_id, const xr_map<std::uint32_t, xr_vector<StateManagerAnimationData::AnimationData>>& state)
{
    if (state.find(weapon_slot_id) == state.end())
        weapon_slot_id = 0;

    return state.at(weapon_slot_id);
}

StateManagerAnimationData::AnimationData Script_StateAnimation::select_rnd(
    const xr_map<std::uint32_t, xr_vector<StateManagerAnimationData::AnimationData>>& state, int& weapon_slot_id,
    const bool is_must_play)
{
    StateManagerAnimationData::AnimationData result;
    if (!is_must_play &&
        Globals::Script_RandomInt::getInstance().Generate(1, 100) >
            this->m_current_data_list.at(this->m_states.getCurrentStateName()).getPropertiesRandom())
    {
        return result;
    }

    const xr_vector<StateManagerAnimationData::AnimationData>& animations = this->anim_for_slot(weapon_slot_id, state);
    if (animations.empty())
        return result;

    std::uint32_t found_index = 0;

    if (animations.size() > 1)
    {
        if (this->m_states.getLastID() == 0)
        {
            found_index = Globals::Script_RandomInt::getInstance().Generate(0, animations.size() - 1);
        }
        else
        {
            found_index = Globals::Script_RandomInt::getInstance().Generate(0, animations.size() - 2);
            if (found_index >= this->m_states.getLastID())
            {
                ++found_index;
            }
        }

        this->m_states.setLastID(found_index);
    }

    return animations[found_index];
}

} // namespace Scripts
} // namespace Cordis
