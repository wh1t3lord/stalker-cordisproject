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

    inline void All_Deativate(void)
    {
        for (Script_ILogicEntity* it : this->m_data)
        {
            
        }
    }

private:
    xr_vector<Script_ILogicEntity*> m_data;
};
} // namespace Scripts
} // namespace Cordis
