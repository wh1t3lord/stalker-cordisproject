#pragma once

#include "Script_StateManager.h"

namespace Cordis
{
namespace Scripts
{
class Script_EvaluatorStateManagerIdle : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerIdle(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager),
          m_p_action_planner(nullptr)
    {
    }
    ~Script_EvaluatorStateManagerIdle(void)
    {
        this->m_p_action_planner = nullptr;
        this->m_p_state_manager = nullptr;
    }

    virtual _value_type evaluate(void)
    {
        const xr_map<xr_string, std::uint32_t>& state_manager_properties = this->m_p_state_manager->getProperties();

        if (state_manager_properties.empty())
        {
            R_ASSERT2(false,
                "it can't be, you must initialize your properties in constructor of Script_StateManager instance");
            return false;
        }

        if (!this->m_p_action_planner)
            this->m_p_action_planner = Globals::get_script_action_planner(this->m_object);

        if (!this->m_p_action_planner->initialized())
            return false;

        if ((this->m_p_state_manager->getTargetStateName() == "idle") &&
            (!(this->m_p_state_manager->getActionPlanner()
                    ->evaluator(state_manager_properties.at("animstate_locked"))
                    .evaluate())) &&
            (!(this->m_p_state_manager->getActionPlanner()
                    ->evaluator(state_manager_properties.at("animation_locked"))
                    .evaluate())) &&
            (this->m_p_state_manager->getActionPlanner()
                    ->evaluator(state_manager_properties.at("movement"))
                    .evaluate()) &&
            (this->m_p_state_manager->getActionPlanner()
                    ->evaluator(state_manager_properties.at("animstate"))
                    .evaluate()) &&
            (this->m_p_state_manager->getActionPlanner()
                    ->evaluator(state_manager_properties.at("animation"))
                    .evaluate()) &&
            (this->m_p_state_manager->getActionPlanner()
                    ->evaluator(state_manager_properties.at("smartcover"))
                    .evaluate()))
        {
            if (this->m_p_action_planner->current_action_id() == Globals::XR_ACTIONS_ID::kStateManager + 1)
                this->m_p_state_manager->setCombat(true);
        }

        if (this->m_p_state_manager->isCombat())
            return true;

        if (!(Globals::cast_planner(
                  &this->m_p_action_planner->action(StalkerDecisionSpace::eWorldOperatorCombatPlanner)))
                 ->initialized())
            return false;

        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
    CScriptActionPlanner* m_p_action_planner;
};

class Script_EvaluatorStateManagerIdleAlife : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerIdleAlife(const xr_string& evaluator_name, Script_StateManager* p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_current_action_id(0), m_p_state_manager(p_state_manager)
    {
    }

    ~Script_EvaluatorStateManagerIdleAlife(void) {}

    virtual _value_type evaluate(void)
    {
        if (!this->m_object->Alive())
            return true;

        CScriptActionPlanner* const p_action_planner = Globals::get_script_action_planner(this->m_object);
        if (!p_action_planner)
        {
            R_ASSERT2(false, "can't be!");
            return false;
        }

        this->m_current_action_id = 0;

        if (p_action_planner->initialized())
        {
            this->m_current_action_id = p_action_planner->current_action_id();

            if (this->m_current_action_id != Globals::XR_ACTIONS_ID::kAlife)
                this->m_p_state_manager->setAlife(false);
        }

        if (!XR_MEET::is_meet(this->m_object))
        {
            const xr_map<xr_string, std::uint32_t>& state_manager_properties = this->m_p_state_manager->getProperties();

            if (state_manager_properties.empty())
            {
                R_ASSERT2(false,
                    "it can't be, you must initialize your properties in constructor of Script_StateManager instance");
                return false;
            }
            bool result = (this->m_p_state_manager->getTargetStateName() == "idle") &&
                (!(this->m_p_state_manager->getActionPlanner()
                        ->evaluator(state_manager_properties.at("animstate_locked"))
                        .evaluate())) &&
                (!(this->m_p_state_manager->getActionPlanner()
                        ->evaluator(state_manager_properties.at("animation_locked"))
                        .evaluate())) &&
                (this->m_p_state_manager->getActionPlanner()
                        ->evaluator(state_manager_properties.at("movement"))
                        .evaluate()) &&
                (!(this->m_p_state_manager->getActionPlanner()
                        ->evaluator(state_manager_properties.at("weapon_locked"))
                        .evaluate())) &&
                (this->m_p_state_manager->getActionPlanner()
                        ->evaluator(state_manager_properties.at("animstate"))
                        .evaluate()) &&
                (this->m_p_state_manager->getActionPlanner()
                        ->evaluator(state_manager_properties.at("animation"))
                        .evaluate()) &&
                (this->m_p_state_manager->getActionPlanner()
                        ->evaluator(state_manager_properties.at("smartcover"))
                        .evaluate());
            if (result)
            {
                this->m_p_state_manager->setAlife(true);
            }

            if (this->m_p_state_manager->isAlife())
            {
                return true;
            }

            return result;
        }

        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
    std::uint32_t m_current_action_id;
};

class Script_EvaluatorStateManagerIdleItems : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerIdleItems(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(nullptr)
    {
    }

    ~Script_EvaluatorStateManagerIdleItems(void) {}

    virtual _value_type evaluate(void)
    {
        if (!this->m_object->Alive())
            return true;

        if (!XR_MEET::is_meet(this->m_object))
        {
            if (this->m_p_state_manager)
            {
				const xr_map<xr_string, std::uint32_t>& state_manager_properties = this->m_p_state_manager->getProperties();

				if (state_manager_properties.empty())
				{
					R_ASSERT2(false,
						"it can't be, you must initialize your properties in constructor of Script_StateManager instance");
					return false;
				}
				bool result = (this->m_p_state_manager->getTargetStateName() == "idle") &&
					(!(this->m_p_state_manager->getActionPlanner()
						->evaluator(state_manager_properties.at("animstate_locked"))
						.evaluate())) &&
					(!(this->m_p_state_manager->getActionPlanner()
						->evaluator(state_manager_properties.at("animation_locked"))
						.evaluate())) &&
					(this->m_p_state_manager->getActionPlanner()
						->evaluator(state_manager_properties.at("movement"))
						.evaluate()) &&
					(this->m_p_state_manager->getActionPlanner()
						->evaluator(state_manager_properties.at("animstate"))
						.evaluate()) &&
					(this->m_p_state_manager->getActionPlanner()
						->evaluator(state_manager_properties.at("animation"))
						.evaluate()) &&
					(this->m_p_state_manager->getActionPlanner()
						->evaluator(state_manager_properties.at("smartcover"))
						.evaluate());

				return result;
            }
        }

        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerLogicActive : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerLogicActive(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str())
    {
    }

    ~Script_EvaluatorStateManagerLogicActive(void) {}

    virtual _value_type evaluate(void)
    {
        if (DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSectionName().empty())
            return false;

        return true;
    }
};

class Script_EvaluatorStateManagerEnd : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerEnd(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_action_planner(nullptr),
          m_p_state_manager(p_state_manager)
    {
    }

    ~Script_EvaluatorStateManagerEnd(void) {}

    virtual _value_type evaluate(void)
    {
        if (!this->m_p_action_planner)
            this->m_p_action_planner = Globals::get_script_action_planner(this->m_object);

        if (!this->m_p_action_planner->initialized())
            return false;

        if (this->m_p_action_planner->current_action_id() == StalkerDecisionSpace::eWorldOperatorCombatPlanner)
        {
            if (Globals::cast_planner(
                    &this->m_p_action_planner->action(StalkerDecisionSpace::eWorldOperatorCombatPlanner)))
                return false;
        }
        else
        {
            if ((this->m_p_action_planner->current_action_id() != StalkerDecisionSpace::eWorldOperatorCombatPlanner) &&
                (this->m_p_action_planner->current_action_id() != StalkerDecisionSpace::eWorldOperatorAnomalyPlanner))
                this->m_p_state_manager->setCombat(true);
        }

        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
    CScriptActionPlanner* m_p_action_planner;
};

class Script_EvaluatorStateManagerLocked : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerLocked(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerLocked(void) {}

    virtual _value_type evaluate(void)
    {
        return this->m_p_state_manager->getActionPlanner()->initialized() &&
            (this->m_p_state_manager->getActionPlanner()
                    ->evaluator(this->m_p_state_manager->getProperties().at("weapon_locked"))
                    .evaluate() ||
                this->m_object->is_body_turning());
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerLockedExternal : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerLockedExternal(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }

    ~Script_EvaluatorStateManagerLockedExternal(void) {}

    virtual _value_type evaluate(void)
    {
        if (this->m_p_state_manager->isCombat() || this->m_p_state_manager->isAlife())
            return true;

        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerAnimation : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerAnimation(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str())
    {
    }

    ~Script_EvaluatorStateManagerAnimation(void) {}

    virtual _value_type evaluate(void)
    {
        return Script_GlobalHelper::getInstance()
            .getStateLibrary()
            .at(this->m_p_state_manager->getTargetStateName())
            .getAnimationName()
            .empty(); // Lord: доделать когда сделаешь state_mgr_animation.animation!
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerAnimationPlayNow : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerAnimationPlayNow(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }

    ~Script_EvaluatorStateManagerAnimationPlayNow(void) {}

    virtual _value_type evaluate(void)
    {
        // Lord: доделать когда сделаешь state_mgr_animation.animation
        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerAnimationNoneNow : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerAnimationNoneNow(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }

    ~Script_EvaluatorStateManagerAnimationNoneNow(void) {}

    virtual _value_type evaluate(void)
    {
        // Lord: доделать когда сделаешь state_mgr_animation.animation
        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerNoneNow : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerNoneNow(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str())
    {
    }

    ~Script_EvaluatorStateManagerNoneNow(void) {}

    virtual _value_type evaluate(void)
    {
        // Lord: доделать когда сделаешь state_mgr_animation.animation
        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerAnimationLocked : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerAnimationLocked(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }

    ~Script_EvaluatorStateManagerAnimationLocked(void) {}

    virtual _value_type evaluate(void)
    {
        // Lord: доделать когда сделаешь state_mgr_animation.animation
        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerAnimationState : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerAnimationState(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerAnimationState(void) {}

    virtual _value_type evaluate(void)
    {
        // Lord: реализовать когда будет state_manager_animastate
        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerAnimationStatePlayNow : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerAnimationStatePlayNow(
        const xr_string& evaluator_name, Script_StateManager* p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }

    ~Script_EvaluatorStateManagerAnimationStatePlayNow(void) {}

    virtual _value_type evaluate(void)
    {
        // Lord: доделать когда будет animstate!
        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerAnimationStateIdleNow : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerAnimationStateIdleNow(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }

    Script_EvaluatorStateManagerAnimationStateIdleNow(void) {}

    virtual _value_type evaluate(void)
    {
        // Lord: реализовать когда будет animstate
        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerAnimationStateLocked : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerAnimationStateLocked(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerAnimationStateLocked(void) {}

    virtual _value_type evaluate(void)
    {
        // Lord: реализовать когда будет state_manager_animastate
        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerBodyState : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerBodyState(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }

    virtual _value_type evaluate(void)
    {
        return ((!Script_GlobalHelper::getInstance()
                        .getStateLibrary()
                        .at(this->m_p_state_manager->getTargetStateName())
                        .getBodyStateType()) ||
            (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getBodyStateType() == static_cast<std::uint32_t>(this->m_object->target_body_state())));
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerBodyStateCrouch : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerBodyStateCrouch(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }

    virtual _value_type evaluate(void)
    {
        return (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getBodyStateType() == MonsterSpace::eBodyStateCrouch);
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerBodyStateStanding : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerBodyStateStanding(
        const xr_string& evalautor_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evalautor_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }

    virtual _value_type evaluate(void)
    {
        return Script_GlobalHelper::getInstance()
                   .getStateLibrary()
                   .at(this->m_p_state_manager->getTargetStateName())
                   .getBodyStateType() == MonsterSpace::eBodyStateStand;
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerBodyStateCrouchNow : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerBodyStateCrouchNow(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }

    virtual _value_type evalaute(void) { return this->m_object->target_body_state() == MonsterSpace::eBodyStateCrouch; }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerBodyStateStandingNow : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerBodyStateStandingNow(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }

    virtual _value_type evaluate(void) { return this->m_object->target_body_state() == MonsterSpace::eBodyStateStand; }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerDirection : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerDirection(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerDirection(void) {}

    virtual _value_type evaluate(void)
    {
        // Lord: реализовать когда будет m_p_state_manager.callback
        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerDirectionSearch : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerDirectionSearch(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerDirectionSearch(void) {}

    virtual _value_type evaluate(void)
    {
        // Lord: когда будет написан сам state_manager, сюда вернуться!
        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
};

#pragma region Cordis Evaluator State Manager about Mental states
class Script_EvaluatorStateManagerMental : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerMental(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerMental(void) {}

    virtual _value_type evaluate(void)
    {
        return ((!Script_GlobalHelper::getInstance()
                        .getStateLibrary()
                        .at(this->m_p_state_manager->getTargetStateName())
                        .getMentalType()) ||
            (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getMentalType() == this->m_object->target_mental_state()));
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerMentalFree : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerMentalFree(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerMentalFree(void) {}

    virtual _value_type evaluate(void)
    {
        return (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getMentalType() == static_cast<std::uint32_t>(MonsterSpace::eMentalStateFree));
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerMentalFreeNow : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerMentalFreeNow(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerMentalFreeNow(void) {}

    virtual _value_type evaluate(void)
    {
        return (this->m_object->target_mental_state() == MonsterSpace::eMentalStateFree);
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerMentalDanger : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerMentalDanger(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerMentalDanger(void) {}

    virtual _value_type evaluate(void)
    {
        return (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getMentalType() == MonsterSpace::eMentalStateDanger);
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerMentalDangerNow : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerMentalDangerNow(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerMentalDangerNow(void) {}

    virtual _value_type evaluate(void)
    {
        return (this->m_object->target_mental_state() == MonsterSpace::eMentalStateDanger);
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerMentalPanic : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerMentalPanic(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerMentalPanic(void) {}

    virtual _value_type evaluate(void)
    {
        return (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getMentalType() == MonsterSpace::eMentalStatePanic);
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerMentalPanicNow : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerMentalPanicNow(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerMentalPanicNow(void) {}

    virtual _value_type evaluate(void)
    {
        return (this->m_object->target_mental_state() == MonsterSpace::eMentalStatePanic);
    }

private:
    Script_StateManager* m_p_state_manager;
};
#pragma endregion

#pragma region Cordis Evaluator State Manager about Movement
class Script_EvaluatorStateManagerMovement : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerMovement(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerMovement(void) {}

    virtual _value_type evaluate(void)
    {
        return (!Script_GlobalHelper::getInstance()
                       .getStateLibrary()
                       .at(this->m_p_state_manager->getTargetStateName())
                       .getMovementType()) ||
            (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getMovementType() == static_cast<std::uint32_t>(this->m_object->target_movement_type()));
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerMovementWalk : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerMovementWalk(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerMovementWalk(void) {}

    virtual _value_type evaluate(void)
    {
        return (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getMovementType() == MonsterSpace::eMovementTypeWalk);
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerMovementRun : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerMovementRun(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerMovementRun(void) {}

    virtual _value_type evaluate(void)
    {
        return (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getMovementType() == MonsterSpace::eMovementTypeRun);
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerMovementStand : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerMovementStand(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerMovementStand(void) {}

    virtual _value_type evaluate(void)
    {
        return (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getMovementType() == MonsterSpace::eMovementTypeStand);
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerMovementStandNow : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerMovementStandNow(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerMovementStandNow(void) {}

    virtual _value_type evaluate(void)
    {
        return (this->m_object->target_movement_type() == MonsterSpace::eMovementTypeStand);
    }

private:
    Script_StateManager* m_p_state_manager;
};
#pragma endregion

#pragma region Cordis Evaluator State Manager about smart covers
class Script_EvaluatorStateManagerSmartCover : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerSmartCover(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerSmartCover(void) {}

    virtual _value_type evaluate(void) { return true; }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerSmartCoverNeed : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerSmartCoverNeed(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerSmartCoverNeed(void) {}

    virtual _value_type evaluate(void) { return false; }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerInSmartCover : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerInSmartCover(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerInSmartCover(void) {}

    virtual _value_type evaluate(void) { return this->m_object->in_smart_cover(); }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerSmartCoverLocked : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerSmartCoverLocked(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerSmartCoverLocked(void) {}

    virtual _value_type evaluate(void) { return false; }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerSmartCoverIn : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerSmartCoverIn(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerSmartCoverIn(void) {}

    virtual _value_type evaluate(void) { return this->m_object->in_smart_cover(); }

private:
    Script_StateManager* m_p_state_manager;
};
#pragma endregion

#pragma region Cordis Evaluator State Manager about Weapon
class Script_EvaluatorStateManagerWeapon : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerWeapon(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerWeapon(void) {}

    virtual _value_type evaluate(void)
    {
        const xr_string& state_weapon_name = Script_GlobalHelper::getInstance()
                                                 .getStateLibrary()
                                                 .at(this->m_p_state_manager->getTargetStateName())
                                                 .getWeaponTypeName();
        if (state_weapon_name.empty())
            return true;

        if (!this->m_object->best_weapon())
            return true;

        CScriptGameObject* const p_client_best_weapon = this->m_object->best_weapon();
        CScriptGameObject* const p_client_active_item = this->m_object->GetActiveItem();

        if ((state_weapon_name == "strapped") &&
            (((Globals::is_strappable_weapon(p_client_best_weapon)) && this->m_object->weapon_strapped() &&
                 this->m_object->is_weapon_going_to_be_strapped(p_client_best_weapon)) ||
                (!Globals::is_strappable_weapon(p_client_best_weapon) && !p_client_active_item)))
            return true;

        if ((state_weapon_name == "unstrapped" || state_weapon_name == "fire" || state_weapon_name == "sniper_fire") &&
            (p_client_active_item) && (p_client_best_weapon) &&
            (p_client_best_weapon->ID() == p_client_active_item->ID()) &&
            (!this->m_object->is_weapon_going_to_be_strapped(p_client_best_weapon) &&
                this->m_object->weapon_unstrapped()))
            return true;

        if (state_weapon_name == "none" && !p_client_active_item)
            return true;

        if (state_weapon_name == "drop" && !p_client_active_item)
            return true;

        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerWeaponLocked : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerWeaponLocked(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerWeaponLocked(void) {}

    virtual _value_type evaluate(void)
    {
        bool is_weapon_unstrapped = this->m_object->weapon_unstrapped();
        bool is_weapon_strapped = this->m_object->weapon_strapped();

        if (!is_weapon_strapped || !is_weapon_unstrapped)
            return true;

        CScriptGameObject* const p_client_best_weapon = this->m_object->best_weapon();
        if (!p_client_best_weapon)
            return false;

        bool is_weapon_going_to_be_strapped = this->m_object->is_weapon_going_to_be_strapped(p_client_best_weapon);

        if (is_weapon_going_to_be_strapped && !is_weapon_strapped)
            return true;

        if (!is_weapon_going_to_be_strapped && !is_weapon_unstrapped && this->m_object->GetActiveItem())
            return true;

        return false;
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerWeaponStrapped : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerWeaponStrapped(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerWeaponStrapped(void) {}

    virtual _value_type evaluate(void)
    {
        return Script_GlobalHelper::getInstance()
                   .getStateLibrary()
                   .at(this->m_p_state_manager->getTargetStateName())
                   .getWeaponTypeName() == "strapped";
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerWeaponStrappedNow : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerWeaponStrappedNow(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerWeaponStrappedNow(void) {}

    virtual _value_type evaluate(void)
    {
        CScriptGameObject* const p_client_best_weapon = this->m_object->best_weapon();

        if (!p_client_best_weapon)
            return true;

        CScriptGameObject* const p_client_active_item = this->m_object->GetActiveItem();

        return (!Globals::is_strappable_weapon(p_client_best_weapon) && !p_client_active_item) ||
            (this->m_object->is_weapon_going_to_be_strapped(p_client_best_weapon) && this->m_object->weapon_strapped());
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerWeaponUnstrapped : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerWeaponUnstrapped(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerWeaponUnstrapped(void) {}

    virtual _value_type evaluate(void)
    {
        return (Script_GlobalHelper::getInstance()
                       .getStateLibrary()
                       .at(this->m_p_state_manager->getTargetStateName())
                       .getWeaponTypeName() == "unstrapped") ||
            (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getWeaponTypeName() == "fire") ||
            (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getWeaponTypeName() == "sniper_fire");
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerWeaponUnstrappedNow : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerWeaponUnstrappedNow(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerWeaponUnstrappedNow(void) {}

    virtual _value_type evaluate(void)
    {
        CScriptGameObject* const p_client_active_item = this->m_object->GetActiveItem();
        CScriptGameObject* const p_client_best_weapon = this->m_object->best_weapon();

        return (p_client_best_weapon) && (p_client_active_item) &&
            (p_client_active_item->ID() == p_client_best_weapon->ID()) &&
            (!this->m_object->is_weapon_going_to_be_strapped(p_client_best_weapon) &&
                this->m_object->weapon_unstrapped());
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerWeaponNone : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerWeaponNone(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerWeaponNone(void) {}

    virtual _value_type evaluate(void)
    {
        return (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getWeaponTypeName() == "none");
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerWeaponNoneNow : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerWeaponNoneNow(
        const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerWeaponNoneNow(void) {}

    virtual _value_type evaluate(void) { return !(this->m_object->GetActiveItem()); }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerWeaponDrop : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerWeaponDrop(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerWeaponDrop(void) {}

    virtual _value_type evaluate(void)
    {
        return (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getWeaponTypeName() == "drop");
    }

private:
    Script_StateManager* m_p_state_manager;
};

class Script_EvaluatorStateManagerWeaponFire : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorStateManagerWeaponFire(const xr_string& evaluator_name, Script_StateManager* const p_state_manager)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_state_manager(p_state_manager)
    {
    }
    ~Script_EvaluatorStateManagerWeaponFire(void) {}

    virtual _value_type evaluate(void)
    {
        return (Script_GlobalHelper::getInstance()
                       .getStateLibrary()
                       .at(this->m_p_state_manager->getTargetStateName())
                       .getWeaponTypeName() == "fire") ||
            (Script_GlobalHelper::getInstance()
                    .getStateLibrary()
                    .at(this->m_p_state_manager->getTargetStateName())
                    .getWeaponTypeName() == "sniper_fire");
    }

private:
    Script_StateManager* m_p_state_manager;
};
#pragma endregion

} // namespace Scripts
} // namespace Cordis
