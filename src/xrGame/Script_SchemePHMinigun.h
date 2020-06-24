#pragma once

namespace Cordis
{
namespace Scripts
{
// @ Можете завершить
    //  не используется
class Script_SchemePHMinigun : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemePHMinigun(void) = delete;
    Script_SchemePHMinigun(CScriptGameObject* const p_client_object, void* storage);
    ~Script_SchemePHMinigun(void);

    virtual void reset_scheme(const bool value, CScriptGameObject* const p_client_object);

private:
    bool m_is_destroyed;
    int m_fastcall_update_average;
    std::uint32_t m_start_delaying_time;
    std::uint32_t m_start_shooting_time;    
    std::uint32_t m_fastcall_update_time;
    std::uint32_t m_fastcall_update_num;
    std::uint32_t m_state_firetarget;
    std::uint32_t m_state_cannon;
    std::uint32_t m_state_shooting;
    CCar* m_p_minigun;
    CScriptGameObject* m_p_target_object;
    Fvector m_start_look_direction;
    Fvector m_start_look_posiiton;
    Fvector m_target_fire_patrol_position;
    LogicData m_on_target_visibile;
    LogicData m_on_target_invisible;
};
} // namespace Scripts
} // namespace Cordis
