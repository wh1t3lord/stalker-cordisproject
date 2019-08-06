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
        // @ Lord: проверить нужно ли так удалять?
        for (Script_ILogicEntity* it : this->m_data)
        {
            if (it)
            {
                delete it;
                it = nullptr;
            }
        }

        this->m_data.clear();
    }

    inline void all_deactivate(CScriptGameObject* npc)
    {
        if (!npc)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        for (Script_ILogicEntity* it : this->m_data)
        {
            if (it)
                it->deactivate(npc);
        }
    }

    inline void all_reset_scheme(void)
    {
        for (Script_ILogicEntity* it : this->m_data)
        {
            if (it)
                it->reset_scheme();
        }
    }

    inline void all_save(void)
    {
        for (Script_ILogicEntity* it : this->m_data)
        {
            if (it)
                it->save();
        }
    }

    inline void all_load(void)
    {
        for (Script_ILogicEntity* it : this->m_data)
        {
            if (it)
                it->load();
        }
    }

    inline void all_extrapolate_callback(void)
    {
        for (Script_ILogicEntity* it : this->m_data)
        {
            if (it)
                it->extrapolate_callback();
        }
    }

    inline void all_activate_scheme(void)
    {
        for (Script_ILogicEntity* it : this->m_data)
        {
            if (it)
                it->activate_scheme();
        }
    }

    inline void all_cutscene_callback(void)
    {
        for (Script_ILogicEntity* it : this->m_data)
        {
            if (it)
                it->cutscene_callback();
        }
    }

private:
    xr_vector<Script_ILogicEntity*> m_data;
};
} // namespace Scripts
} // namespace Cordis
