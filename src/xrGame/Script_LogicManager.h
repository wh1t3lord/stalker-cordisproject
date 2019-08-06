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

    inline void all_save(void) 
    {
    
    }

    inline void all_load(void) {}

    inline void all_extrapolate_callback(void) {}

    inline void all_activate_scheme(void) {}

    inline void all_cutscene_callback(void) {}

    inline void all_update(const float& delta)
    {
        if (delta < 0.0f)
            Msg("[Scripts/Script_LogicManager/all_update] The delta under zero!");

        if (fis_zero(delta))
            Msg("[Scripts/Script_LogicManager/all_update] The delta is zero!");
    }
};
} // namespace Scripts
} // namespace Cordis
