////////////////////////////////////////////////////////////////////////////
//	Module 		: script_property_evaluator_wrapper.cpp
//	Created 	: 19.03.2004
//  Modified 	: 26.03.2004
//	Author		: Dmitriy Iassenev
//	Description : Script property evaluator wrapper
////////////////////////////////////////////////////////////////////////////

#include "pch_script.h"
#include "script_property_evaluator_wrapper.h"
#include "script_game_object.h"
#include "ai_space.h"
#include "xrScriptEngine/script_engine.hpp"

void CScriptPropertyEvaluatorWrapper::setup(CScriptGameObject* object, CPropertyStorage* storage)
{
    this->setup(object, storage);
}

void CScriptPropertyEvaluatorWrapper::setup_static(
    CScriptPropertyEvaluator* evaluator, CScriptGameObject* object, CPropertyStorage* storage)
{
    evaluator->CScriptPropertyEvaluator::setup(object, storage);
}

bool CScriptPropertyEvaluatorWrapper::evaluate()
{
    try
    {
        return this->evaluate();
    }
#ifdef DEBUG
    catch (luabind::cast_failed& exception)
    {
#ifdef LOG_ACTION
		MESSAGEE("evaluator [%s] returns value with not a %s type!", m_evaluator_name.c_str(),
			exception.info().name());
#else
        GEnv.ScriptEngine->script_log(LuaMessageType::Error,
            "SCRIPT RUNTIME ERROR : evaluator returns value with not a %s type!", exception.info().name());
#endif
    }
#endif
    catch (...)
    {
        MESSAGEE("evaluator [%s] returns value with not a bool type!", this->m_evaluator_name.c_str());
    }
    return (false);
}

bool CScriptPropertyEvaluatorWrapper::evaluate_static(CScriptPropertyEvaluator* evaluator)
{
    return (evaluator->CScriptPropertyEvaluator::evaluate());
}
