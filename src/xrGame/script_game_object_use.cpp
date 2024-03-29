#include "pch_script.h"
#include "script_game_object.h"
#include "script_game_object_impl.h"
#include "GameObject.h"
#include "xrScriptEngine/script_engine.hpp"
#include "stalker_planner.h"
#include "ai/stalker/ai_stalker.h"
#include "searchlight.h"
#include "xrScriptEngine/script_callback_ex.h"
#include "game_object_space.h"
#include "memory_manager.h"
#include "enemy_manager.h"
#include "movement_manager.h"
#include "patrol_path_manager.h"
#include "PHCommander.h"
#include "PHScriptCall.h"
#include "PHSimpleCalls.h"
#include "xrPhysics/IPHWorld.h"
#include "doors_manager.h"

void CScriptGameObject::SetTipText(LPCSTR tip_text) { object().set_tip_text(tip_text); }
void CScriptGameObject::SetTipTextDefault() { object().set_tip_text_default(); }
void CScriptGameObject::SetNonscriptUsable(bool nonscript_usable) { object().set_nonscript_usable(nonscript_usable); }
Fvector CScriptGameObject::GetCurrentDirection()
{
    CProjector* obj = smart_cast<CProjector*>(&object());
    if (!obj)
    {
        GEnv.ScriptEngine->script_log(
            LuaMessageType::Error, "Script Object : cannot access class member GetCurrentDirection!");
        return Fvector().set(0.f, 0.f, 0.f);
    }
    return obj->GetCurrentDirection();
}

CScriptGameObject::CScriptGameObject(CGameObject* game_object) : m_game_object(game_object), m_door(0)
{
    R_ASSERT2(m_game_object, "Null actual object passed!");
}

CScriptGameObject::~CScriptGameObject()
{
    if (!m_door)
        return;

    unregister_door();
}

CScriptGameObject* CScriptGameObject::Parent() const
{
    CGameObject* l_tpGameObject = smart_cast<CGameObject*>(object().H_Parent());
    if (l_tpGameObject)
        return (l_tpGameObject->lua_game_object());
    else
        return (0);
}

int CScriptGameObject::clsid() const 
{
    if (m_game_object == nullptr)
    {
        MESSAGEWR("object is already deleted!");
        return -1;
    }

    return (object().clsid()); 
}

LPCSTR CScriptGameObject::Name() const 
{ 
    if ((this == nullptr) || (m_game_object == nullptr) || (m_game_object->lua_game_object() == nullptr))
    {
        MESSAGEI("instance was deleted and returned this, if you skip this you will get PVC!");
        return xr_string("already_deleted_npc!").c_str();
    }

    return (*object().cName()); 
}
shared_str CScriptGameObject::cName() const { return (object().cName()); }
LPCSTR CScriptGameObject::Section() const { return (*object().cNameSect()); }
void CScriptGameObject::Kill(
    CScriptGameObject* who, bool bypass_actor_check /*AVO: added for actor before death callback*/)
{
    CEntity* l_tpEntity = smart_cast<CEntity*>(&object());
    if (!l_tpEntity)
    {
        R_ASSERT2(false, "bad cast!");
        return;
    }
    if (!l_tpEntity->AlreadyDie())
        l_tpEntity->KillEntity(who ? who->object().ID() : object().ID(), bypass_actor_check);
    else
        GEnv.ScriptEngine->script_log(LuaMessageType::Error, "attempt to kill dead object %s", *object().cName());
}

bool CScriptGameObject::Alive() const
{
    CEntity* entity = smart_cast<CEntity*>(&object());
    if (!entity)
    {
        GEnv.ScriptEngine->script_log(LuaMessageType::Error, "CSciptEntity : cannot access class member Alive!");
        return (false);
    }
    return (!!entity->g_Alive());
}

ALife::ERelationType CScriptGameObject::GetRelationType(CScriptGameObject* who)
{
    CEntityAlive* l_tpEntityAlive1 = smart_cast<CEntityAlive*>(&object());
    if (!l_tpEntityAlive1)
    {
        GEnv.ScriptEngine->script_log(
            LuaMessageType::Error, "%s cannot access class member GetRelationType!", *object().cName());
        return ALife::eRelationTypeDummy;
    }

    CEntityAlive* l_tpEntityAlive2 = smart_cast<CEntityAlive*>(&who->object());
    if (!l_tpEntityAlive2)
    {
        GEnv.ScriptEngine->script_log(LuaMessageType::Error,
            "%s cannot apply GetRelationType method for non-alive object!", *who->object().cName());
        return ALife::eRelationTypeDummy;
    }

    return l_tpEntityAlive1->tfGetRelationType(l_tpEntityAlive2);
}

/*
template <typename T>
IC T* CScriptGameObject::action_planner()*/

void CScriptGameObject::set_patrol_extrapolate_callback(std::function<bool(std::uint32_t)> p_callback)
{
	CCustomMonster* p_monster = smart_cast<CCustomMonster*>(&object());
	if (!p_monster)
	{
		MESSAGEWR("CCustomMonster : cannot access class member set_patrol_extrapolate_callback!");
		return;
	}

    if (p_callback == nullptr)
    {
        MESSAGEI("You turned off extrapolate callback!");
    }

    p_monster->movement().patrol().setExtrapolateCallback(p_callback);
}

void CScriptGameObject::set_enemy_callback(const luabind::functor<bool>& functor)
{
    CCustomMonster* monster = smart_cast<CCustomMonster*>(&object());
    if (!monster)
    {
        GEnv.ScriptEngine->script_log(
            LuaMessageType::Error, "CCustomMonster : cannot access class member set_enemy_callback!");
        return;
    }
    // Lord - [Script] Re-write
    //   monster->memory().enemy().useful_callback().set(functor);
}

void CScriptGameObject::set_enemy_callback(const luabind::functor<bool>& functor, const luabind::object& object)
{
    CCustomMonster* monster = smart_cast<CCustomMonster*>(&this->object());
    if (!monster)
    {
        GEnv.ScriptEngine->script_log(
            LuaMessageType::Error, "CCustomMonster : cannot access class member set_enemy_callback!");
        return;
    }
    // Lord - [Script] Re-write
    //   monster->memory().enemy().useful_callback().set(functor, object);
}

void CScriptGameObject::set_enemy_callback(std::function<bool(CScriptGameObject* const, CScriptGameObject* const)>& func)
{
    if (func == nullptr)
    {
        MESSAGEWR("If you want delete function call the delete_enemy_callback() function!");
        return;
    }

    CCustomMonster* monster = smart_cast<CCustomMonster*>(&this->object());

    if (!monster)
    {
        MESSAGEWR("bad cast can't cast to CCustomMonster!");
        return;
    }

    monster->memory().enemy().setUsefulCallback(func);
}

void CScriptGameObject::delete_enemy_callback(void)
{
    CCustomMonster* monster = smart_cast<CCustomMonster*>(&object());
    if (!monster)
    {
        MESSAGEWR("bad cast can't cast to CCustomMonster");
        return;
    }

    monster->memory().enemy().setUsefulCallback(nullptr);
}

void CScriptGameObject::SetCallback(GameObject::ECallbackType type, const luabind::functor<void>& functor)
{
    // Lord - [Script] Re-write
    //  object().callback(type).set(functor);
}

void CScriptGameObject::SetCallback(
    GameObject::ECallbackType type, const luabind::functor<void>& functor, const luabind::object& object)
{
    // Lord - [Script] Re-write
    //  this->object().callback(type).set(functor, object);
}

void CScriptGameObject::SetCallback(GameObject::ECallbackType type)
{ /*object().callback(type).clear();*/
}
/*
void CScriptGameObject::set_fastcall(const luabind::functor<bool>& functor, const luabind::object& object)
{
    CPHScriptGameObjectCondition* c = new CPHScriptGameObjectCondition(object, functor, m_game_object);
    CPHDummiAction* a = new CPHDummiAction();
    CPHSriptReqGObjComparer cmpr(m_game_object);
    Level().ph_commander_scripts().RemoveCallsDeferred(&cmpr);
    Level().ph_commander_scripts().AddCallDeferred(c, a);
}*/

void CScriptGameObject::set_fastcall(std::function<bool(void)> func)
{
    CPHScriptGameObjectCondition* c = new CPHScriptGameObjectCondition(func, m_game_object);
    CPHDummiAction* a = new CPHDummiAction();
    CPHSriptReqGObjComparer cmpr(m_game_object);
    Level().ph_commander_scripts().RemoveCallsDeferred(&cmpr);
    Level().ph_commander_scripts().AddCallDeferred(c, a);
}

void CScriptGameObject::set_const_force(const Fvector& dir, float value, u32 time_interval)
{
    CPhysicsShell* shell = object().cast_physics_shell_holder()->PPhysicsShell();
    // if( !shell->isEnabled() )
    //	shell->set_LinearVel( Fvector().set(0,0,0) );
    if (!physics_world())
    {
        GEnv.ScriptEngine->script_log(LuaMessageType::Error, "set_const_force : ph_world do not exist!");
        return;
    }
    if (!shell)
    {
        GEnv.ScriptEngine->script_log(
            LuaMessageType::Error, "set_const_force : object %s has no physics shell!", *object().cName());
        return;
    }
    //#ifdef DEBUG
    //	Msg( "const force added: force: %f,  time: %d ,dir(%f,%f,%f)", value, time_interval, dir.x, dir.y, dir.z );
    //#endif
    Fvector force;
    force.set(dir);
    force.mul(value);
    CPHConstForceAction* a = new CPHConstForceAction(shell, force);
    CPHExpireOnStepCondition* cn = new CPHExpireOnStepCondition();
    cn->set_time_interval(time_interval);
    // ph_world->AddCall(cn,a);
    Level().ph_commander_physics_worldstep().add_call_threadsafety(cn, a);
}
