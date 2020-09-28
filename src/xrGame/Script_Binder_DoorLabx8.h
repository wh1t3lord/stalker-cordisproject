#pragma once

#include "script_binder_object.h"

namespace Cordis
{
namespace Scripts
{
class Script_Binder_DoorLabx8 : public CScriptBinderObject
{
    using inherited = CScriptBinderObject;

public:
    Script_Binder_DoorLabx8(void) = delete;
    Script_Binder_DoorLabx8(CScriptGameObject* object);
    ~Script_Binder_DoorLabx8(void);

    virtual bool net_Spawn(SpawnType DC);
    virtual void net_Destroy();
    virtual void shedule_Update(u32 time_delta);
    virtual void save(NET_Packet* output_packet);
    virtual void load(IReader* input_packet);
    virtual bool net_SaveRelevant();

    virtual void use_callback(CScriptGameObject* p_client_object, CScriptGameObject* p_client_who);
    virtual void animation_callback(const bool is_end);

    void animation_forward(void);
    void animation_backward(void);
    void animation_stop(void);

private:
    bool m_is_idle;
    bool m_is_play_forward;
    bool m_is_loaded;
    float m_idle_delay;
    float m_start_delay;
    float m_animation_time;
    CScriptSound* m_p_sound_idle;
    CScriptSound* m_p_sound_start;
    CScriptSound* m_p_sound_stop;
    xr_map<std::uint32_t, CondlistData> m_tip_condlist;
    xr_map<std::uint32_t, CondlistData> m_on_use_condlist;
    xr_map<std::uint32_t, CondlistData> m_on_start_condlist;
    xr_map<std::uint32_t, CondlistData> m_on_stop_condlist;
};
} // namespace Scripts
} // namespace Cordis
