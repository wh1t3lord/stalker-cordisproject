////////////////////////////////////////////////////////////////////////////
//	Module 		: script_binder.cpp
//	Created 	: 26.03.2004
//  Modified 	: 26.03.2004
//	Author		: Dmitriy Iassenev
//	Description : Script objects binder
////////////////////////////////////////////////////////////////////////////

#include "pch_script.h"
#include "ai_space.h"
#include "xrScriptEngine/script_engine.hpp"
#include "script_binder.h"
#include "xrServer_Objects_ALife.h"
#include "script_binder_object.h"
#include "script_game_object.h"
#include "GameObject.h"
#include "Level.h"

// comment next string when commiting
//#define DBG_DISABLE_SCRIPTS

CScriptBinder::CScriptBinder(CGameObject* owner)
{
    this->owner = owner;
    init();
}

CScriptBinder::~CScriptBinder() { VERIFY(!m_object); }
void CScriptBinder::init() { m_object = 0; }
void CScriptBinder::clear()
{
    if (this->m_object)
    {
        xr_delete(this->m_object);
        this->m_object = nullptr;
    }
        /*
    try
    {
        xr_delete(m_object);
    }
    catch (...)
    {
        m_object = 0;
    }*/
    init();
}

void CScriptBinder::reinit()
{
    if (this->m_object)
    {
/*
        try
        {*/
        this->m_object->reinit();
/*
        }
        catch (...)
        {
            clear();
        }*/
    }
}

void CScriptBinder::reload(LPCSTR section)
{
    VERIFY(!m_object);
    Msg("[CScriptBinder/reload(section_name)] TO LORD IMPLEMENT BINDING FOR INSTANCES. %s", this->owner->Name());

    int owner_clsid = this->owner->clsid();

    if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_ACTOR))
    {
        this->m_object = new Cordis::Scripts::Script_Binder_Actor(this->owner->lua_game_object());
        Msg("[CScriptBinder/reload(section_name)] binding %s the instance {%s}", section, this->owner->Name());
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_STALKER))
    {
        Msg("[CScriptBinder/reload(section_name)] %s binding stalker the instance {%s}", section, this->owner->Name());
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_AMMO))
    {
        Msg("[CScriptBinder/reload(section_name)] %s binding ammo the instance {%s}", section, this->owner->Name());
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_HELI))
    {
        Msg("[CScriptBinder/reload(section_name)] %s binding helicopter the instance {%s}", section, this->owner->Name());
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_ADVANCEDDETECTOR))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_ELITEDETECTOR))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_SCIENTIFICDETECTOR))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_SIMPLEDETECTOR))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_HELMET))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_OUTFIT))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_GRENADE_F1))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_GRENADE_RGD))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_LEVEL_CHANGER))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_UI_MAIN_MENU))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_ONLINE_OFFLINE_GROUP))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_DESTROYABLE_PHYSICS_OBJECT))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_PHYSICS_OBJECT))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_ARTEFACT))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_SMART_TERRAIN))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_SMART_COVER))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_BLOODSUCKER))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_BOAR))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_BURER))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_CAT))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_CHIMERA))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_CONTROLLER))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_DOG_PSY_PHANTOM))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_DOG_PSY))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_DOG))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_FLESH))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_PSEUDO_GIGANT))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_IZLOM))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_POLTERGEIST))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_DOG_PSEUDO))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_SNORK))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_TUSHKANO))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MONSTER_ZOMBI))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_LAMP))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_RESTRICTOR))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_ANTIRAD))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_BANDAGE))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_BOTTLE))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_EXPLOSION))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_FOOD))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_INVENTORY_BOX))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_M209))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_MEDKIT))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_OG78B))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_PDA))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_VOG25))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_TORCH))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_AK74))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_SHOTGUN_AUTOMATIC))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_BINOCULAR))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_BM16))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_GRENADE_LAUNCHER))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_GROZA))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_HPSA))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_KNIFE))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_LR300))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_PM))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_RG6))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_RPG7))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_SCOPE))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_SHOTGUN))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_SILENCER))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_SVD))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_SVU))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_USP45))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_VAL))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_VINTOREZ))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_WEAPON_WALTHER))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_ZONE_HAIRS))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_ZONE_GALANT))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_ZONE_MOSQUITOBALD))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_ZONE_MINCE))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_ZONE_RADIOACTIVE))
    {
    }
    else if (owner_clsid == Cordis::Scripts::Globals::get_script_clsid(CLSID_SE_ZONE_TORRID))
    {
    }
    else
    {
        Msg("[CScriptBinder/reload(section_name)] WARNING: YOU ARE NOT REGISTER (HANDLE) THE CLSID OF INSTANCE {name:%s | clsid: %d}. YOU MUST TO FIX IT...", section, this->owner->clsid());
    }

    /*
    #ifndef DBG_DISABLE_SCRIPTS


    / *
        if (!pSettings->line_exist(section, "script_binding"))
            return;

        luabind::functor<void> lua_function;
        if (!GEnv.ScriptEngine->functor(pSettings->r_string(section, "script_binding"), lua_function))
        {
            GEnv.ScriptEngine->script_log(
                LuaMessageType::Error, "function %s is not loaded!", pSettings->r_string(section, "script_binding"));
            return;
        }

        try
        {
            lua_function(owner->lua_game_object());
        }
        catch (...)
        {
            clear();
            return;
        }

        if (m_object)
        {
            try
            {
                m_object->reload(section);
            }
            catch (...)
            {
                clear();
            }
        }* /
    #endif*/
}

BOOL CScriptBinder::net_Spawn(CSE_Abstract* DC)
{
    CSE_Abstract* abstract = (CSE_Abstract*)DC;
    CSE_ALifeObject* object = smart_cast<CSE_ALifeObject*>(abstract);

    if (object && this->m_object)
    {
        return static_cast<BOOL>(this->m_object->net_Spawn(object)); // Lord: уйти от каста переписать наверное стоит
    }

    /*
        if (object && m_object)
        {
            try
            {
                return ((BOOL)m_object->net_Spawn(object));
            }
            catch (...)
            {
                clear();
            }
        }*/

    return (TRUE);
}

void CScriptBinder::net_Destroy()
{
    if (this->m_object)
    {
#ifdef _DEBUG
        Msg("* Core object %s is UNbinded from the script object", owner->cName());
#endif // _DEBUG
/*
        try
        {*/
        this->m_object->net_Destroy();
/*
        }
        catch (...)
        {
            clear();
        }*/
    }
    xr_delete(this->m_object);
    this->m_object = nullptr;
}

void CScriptBinder::set_object(CScriptBinderObject* object)
{
    if (IsGameTypeSingle())
    {
        VERIFY2(!m_object, "Cannot bind to the object twice!");
#ifdef _DEBUG
        Msg("* Core object %s is binded with the script object", owner->cName());
#endif // _DEBUG
        m_object = object;
    }
    else
    {
        xr_delete(object);
    }
}

void CScriptBinder::shedule_Update(u32 time_delta)
{
    if (m_object)
    {
        try
        {
            m_object->shedule_Update(time_delta);
        }
        catch (...)
        {
            clear();
        }
    }
}

void CScriptBinder::save(NET_Packet& output_packet)
{
    if (m_object)
    {
        try
        {
            m_object->save(&output_packet);
        }
        catch (...)
        {
            clear();
        }
    }
}

void CScriptBinder::load(IReader& input_packet)
{
    if (m_object)
    {
/*
        try
        {*/
            m_object->load(&input_packet);
/*
        }
        catch (...)
        {
            clear();
        }*/
    }
}

BOOL CScriptBinder::net_SaveRelevant()
{
    if (m_object)
    {
/*
        try
        {*/
            return (m_object->net_SaveRelevant());
/*
        }
        catch (...)
        {
            clear();
        }*/
    }
    return (FALSE);
}

void CScriptBinder::net_Relcase(IGameObject* object)
{
    CGameObject* game_object = smart_cast<CGameObject*>(object);
    if (m_object && game_object)
    {
/*
        try
        {*/
            m_object->net_Relcase(game_object->lua_game_object());
/*
        }
        catch (...)
        {
            clear();
        }*/
    }
}
