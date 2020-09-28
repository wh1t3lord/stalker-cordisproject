#pragma once

#include "script_binder_object.h"

namespace Cordis
{
namespace Scripts
{
class Script_Binder_AnomalField : public CScriptBinderObject
{
    using inherited = CScriptBinderObject;

public:
    Script_Binder_AnomalField(void) = delete;
    Script_Binder_AnomalField(CScriptGameObject* object);
    ~Script_Binder_AnomalField(void);

    virtual void reinit(void);
    virtual void reload(LPCSTR section_name);
    virtual bool net_Spawn(SpawnType DC);
    virtual void net_Destroy(void);
    virtual void shedule_Update(std::uint32_t time_delta);
    virtual bool net_SaveRelevant(void);

    inline void set_enable(const bool is_enable)
    {
        if (is_enable)
            this->m_object->EnableAnomaly();
        else
            this->m_object->DisableAnomaly();
    }

private:
    std::uint32_t m_last_update;
};
} // namespace Scripts
} // namespace Cordis
