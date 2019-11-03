////////////////////////////////////////////////////////////////////////////
//	Module 		: smart_cover_action.cpp
//	Created 	: 16.08.2007
//	Author		: Alexander Dudin
//	Description : Action class for smart cover
////////////////////////////////////////////////////////////////////////////

#include "pch_script.h"
#include "smart_cover_action.h"
#include "ai_monster_space.h"
#include "ai_space.h"
#include "xrAICore/Navigation/level_graph.h"
#include "Common/object_broker.h"

using smart_cover::detail::parse_bool;
using smart_cover::detail::parse_fvector;
using smart_cover::detail::parse_int;
using smart_cover::detail::parse_string;
using smart_cover::detail::parse_table;

smart_cover::action::action(const xr_map<xr_string, xr_vector<xr_string>>& data)
{
    /* Lord: an old architecture of smart covers's loopholes
        luabind::object movement = description["movement"];
        if (luabind::type(movement) != LUA_TNIL && luabind::type(movement) == LUA_TBOOLEAN)
        {
            m_movement = luabind::object_cast<bool>(movement);

            luabind::object position = description["position"];
            if (luabind::type(position) != LUA_TNIL)
                m_target_position = luabind::object_cast<Fvector>(position);
        }
        else*/
    this->m_movement = false;

    for (const std::pair<xr_string, xr_vector<xr_string>>& it : data)
    {
        Animations* anim = new Animations();
        for (const xr_string& anim_it : it.second)
        {
            VERIFY2(std::find(anim->begin(), anim->end(), anim_it.c_str()) == anim->end(),
                make_string("duplicated_animation found: %s", anim_it.c_str()));
            anim->push_back(anim_it.c_str());
        }
        this->m_animations.insert(std::make_pair(it.first.c_str(), anim));
    }


    /*
    luabind::object animations;
    parse_table(description, "animations", animations);
    for (luabind::iterator I(animations), E; I != E; ++I)
    {
        VERIFY(luabind::type(I.key()) == LUA_TSTRING);
        LPCSTR animation_type = luabind::object_cast<LPCSTR>(I.key());
        luabind::object table = *I;
        if (luabind::type(table) != LUA_TTABLE)
        {
            VERIFY(luabind::type(table) != LUA_TNIL);
            continue;
        }
        add_animation(animation_type, *I);
    }*/
}

smart_cover::action::~action() { delete_data(m_animations); }
/*
void smart_cover::action::add_animation(LPCSTR type, luabind::object const& table)
{
    VERIFY(luabind::type(table) == LUA_TTABLE);
    Animations* animations = new Animations();
    for (luabind::iterator I(table), E; I != E; ++I)
    {
        luabind::object string = *I;
        if (luabind::type(string) != LUA_TSTRING)
        {
            VERIFY(luabind::type(string) != LUA_TNIL);
            continue;
        }

        shared_str animation = luabind::object_cast<LPCSTR>(string);
        VERIFY2(std::find(animations->begin(), animations->end(), animation) == animations->end(),
            make_string("duplicated_animation found: %s", animation.c_str()));
        animations->push_back(animation);
    }

    m_animations.insert(std::make_pair(type, animations));
}
*/
