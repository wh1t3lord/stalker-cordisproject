////////////////////////////////////////////////////////////////////////////
//	Module 		: smart_cover_planner_target_selector.h
//	Created 	: 18.09.2007
//	Author		: Alexander Dudin
//	Description : Target selector for smart covers animation planner
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Noncopyable.hpp"
#include "smart_cover_detail.h"
#include "action_planner_action.h"
#include "xrScriptEngine/script_callback_ex.h"

namespace smart_cover
{
class animation_planner;

class target_selector final : public CActionPlannerAction<animation_planner>, private Noncopyable
{
private:
    typedef CActionPlannerAction<animation_planner> inherited;

public:
//    typedef CScriptCallbackEx<void> callback_type;

private:
 //   callback_type m_script_callback;
    std::function<void(CScriptGameObject* const)> m_callback;
    CRandom m_random;

private:
    void add_evaluators();
    void add_actions();

public:
    virtual void setup(animation_planner* object, CPropertyStorage* storage);
    virtual void update();
    virtual LPCSTR object_name() const;

    inline void delete_callback(void) noexcept
    {
        this->m_callback = nullptr;
    }

    inline void register_callback(std::function<void(CScriptGameObject* const)>& my_function) 
    {
		if (my_function == nullptr)
		{
			MESSAGEWR("can't register callback because it is empty!");
			return;
		}

		this->m_callback = my_function;
    }

    inline bool isExist(void) const noexcept { return !!(this->m_callback); }
    // Lord - [Script] Re-write
   // void callback(callback_type const& callback);
   // IC callback_type const& callback() const;
};

} // namespace smart_cover

#include "smart_cover_planner_target_selector_inline.h"


