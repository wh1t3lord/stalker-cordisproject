#pragma once

#include "Script_LogicEntity.h"

namespace Cordis
{
namespace Scripts
{
// @ Передаются m_actions, а не весь SubStorage_Data!!
class Script_LogicManager
{
private:
    Script_LogicManager(void) {}

public:
    inline static Script_LogicManager& getInstance(void) noexcept
    {
        static Script_LogicManager instance;
        return instance;
    }

    ~Script_LogicManager(void) {}

    inline void all_deactivate(xr_vector<Script_ILogicEntity*> buffer, CScriptGameObject* npc)
    {
        if (!buffer.size())
        {
            Msg("[Scripts/Script_LogicManager/all_deactivate] buffer was empty!");
            return;
        }

        if (!npc)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        for (Script_ILogicEntity* it : buffer)
        {
            if (it)
                it->deactivate(npc);
        }
    }

    inline void all_reset_scheme(xr_vector<Script_ILogicEntity*> buffer)
    {
        if (!buffer.size())
        {
            Msg("[Scripts/Script_LogicManager/all_reset_scheme] buffer was empty!");
            return;
        }

        for (Script_ILogicEntity* it : buffer)
        {
            if (it)
                it->reset_scheme();
        }
    }

    inline void all_save(xr_vector<Script_ILogicEntity*> buffer)
    {
        if (!buffer.size())
        {
            Msg("[Scripts/Script_LogicManager/all_save] buffer was empty!");
            return;
        }

        for (Script_ILogicEntity* it : buffer)
        {
            if (it)
                it->save();
        }
    }

    inline void all_load(xr_vector<Script_ILogicEntity*> buffer)
    {
        if (!buffer.size())
        {
            Msg("[Scripts/Script_LogicManager/all_load] buffer was empty!");
            return;
        }

        for (Script_ILogicEntity* it : buffer)
        {
            if (it)
                it->load();
        }
    }

    inline void all_extrapolate_callback(xr_vector<Script_ILogicEntity*> buffer)
    {
        if (!buffer.size())
        {
            Msg("[Scripts/Script_LogicManager/all_extrapolate_callback] buffer was empty!");
            return;
        }

        for (Script_ILogicEntity* it : buffer)
        {
            if (it)
                it->extrapolate_callback();
        }
    }

    inline void all_activate_scheme(xr_vector<Script_ILogicEntity*> buffer)
    {
        if (!buffer.size())
        {
            Msg("[Scripts/Script_LogicManager/all_activate_scheme] buffer was empty!");
            return;
        }

        for (Script_ILogicEntity* it : buffer)
        {
            if (it)
                it->activate_scheme();
        }
    }

    inline void all_cutscene_callback(xr_vector<Script_ILogicEntity*> buffer)
    {
        if (!buffer.size())
        {
            Msg("[Scripts/Script_LogicManager/all_cutscene_callback] buffer was empty!");
            return;
        }

        for (Script_ILogicEntity* it : buffer)
        {
            if (it)
                it->cutscene_callback();
        }
    }

    inline void all_update(xr_vector<Script_ILogicEntity*> buffer, const float& delta)
    {
        if (!buffer.size())
        {
            Msg("[Scripts/Script_LogicManager/all_update] buffer was empty!");
            return;
        }

        if (delta < 0.0f)
            Msg("[Scripts/Script_LogicManager/all_update] The delta under zero!");

        if (fis_zero(delta))
            Msg("[Scripts/Script_LogicManager/all_update] The delta is zero!");

        for (Script_ILogicEntity* it : buffer)
        {
            if (it)
                it->update(delta);
        }
    }

    inline void all_hit_callback(xr_vector<Script_ILogicEntity*> buffer, CScriptGameObject* object, const int& amount,
        const Fvector& local_direction, CScriptGameObject* who, const int& bone_index)
    {
        if (!buffer.size())
        {
            Msg("[Scripts/Script_LogicManager/all_hit_callback] buffer was empty!");
            return;
        }

        if (!object || !who)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        for (Script_ILogicEntity* it : buffer)
        {
            if (it)
                it->hit_callback(object, amount, local_direction, who, bone_index);
        }
    }

    inline void all_death_callback(
        xr_vector<Script_ILogicEntity*> buffer, CScriptGameObject* victim, CScriptGameObject* who)
    {
        if (!buffer.size())
        {
            Msg("[Script/Script_LogicManager/all_death_callback] buffer was empty!");
            return;
        }

        if (!victim || !who)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        for (Script_ILogicEntity* it : buffer)
        {
            if (it)
                it->death_callback(victim, who);
        }
    }

    inline void all_net_destroy(xr_vector<Script_ILogicEntity*> buffer)
    {
        if (!buffer.size())
        {
            Msg("[Scripts/Script_LogicManager/all_net_destroy] buffer was empty!");
            return;
        }

        for (Script_ILogicEntity* it : buffer)
        {
            if (it)
                it->net_destroy();
        }
    }

    inline void all_waypoint_callback(
        xr_vector<Script_ILogicEntity*> buffer, CScriptGameObject* object, const int& action_type, const int& index)
    {
        if (!buffer.size())
        {
            Msg("[Scripts/Script_LogicManager/all_waypoint_callback] buffer was empty!");
            return;
        }

        if (!object)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        for (Script_ILogicEntity* it : buffer)
        {
            if (it)
                it->waypoint_callback(object, action_type, index);
        }
    }

    inline void all_use_callback(
        xr_vector<Script_ILogicEntity*> buffer, CScriptGameObject* victim, CScriptGameObject* who)
    {
        if (!buffer.size())
        {
            Msg("[Scripts/Script_LogicManager/all_use_callback] buffer was empty!");
            return;
        }

        if (!victim || !who)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        for (Script_ILogicEntity* it : buffer)
        {
            if (it)
                it->use_callback(victim, who);
        }
    }
};
} // namespace Scripts
} // namespace Cordis
