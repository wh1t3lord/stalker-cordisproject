#pragma once

#include "Script_LogicEntity.h"

namespace Cordis
{
namespace Scripts
{
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

    ~Script_LogicManager(void)
    {
 
    }

    inline void all_deactivate(CScriptGameObject* npc)
    {
        if (!npc)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

 
 
    }

    inline void all_reset_scheme(void)
    {
 
    }

    inline void all_save(void)
    {
  
    }

    inline void all_load(void)
    {
 
    }

    inline void all_extrapolate_callback(void)
    {
 
    }

    inline void all_activate_scheme(void)
    {
 
    }

    inline void all_cutscene_callback(void)
    {
 
    }

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
