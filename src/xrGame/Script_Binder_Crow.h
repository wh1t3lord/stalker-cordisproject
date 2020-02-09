#pragma once

#include "script_binder_object.h"

namespace Cordis
{
namespace Scripts
{
class Script_Binder_Crow : public CScriptBinderObject
{
    using inherited = CScriptBinderObject;
public:
    Script_Binder_Crow(void) = delete;
    Script_Binder_Crow(CScriptGameObject* p_object);
    ~Script_Binder_Crow(void);

    virtual void reinit(void);
    virtual bool net_Spawn(SpawnType DC);
    virtual void net_Destroy(void);
    virtual void shedule_Update(std::uint32_t time_delta);
    virtual void save(NET_Packet* output_packet);
    virtual void load(IReader* input_packet);
    virtual bool net_SaveRelevant(void);

    virtual void death_callback(CScriptGameObject* p_client_victim, CScriptGameObject* p_client_who);


private:
    bool m_is_use_death_callback;
    std::uint32_t m_body_timer;
};

}
} // namespace Cordis
