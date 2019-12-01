#pragma once

namespace Cordis
{
namespace Scripts
{

namespace DataBase
{
struct StorageAnimpoint_Data;
}
} // namespace Scripts
} // namespace Cordis

namespace Cordis
{
namespace Scripts
{
    // Lord: переделать под Script_ISchemeEntity
class Script_Animpoint
{
public:
    Script_Animpoint(void) = delete;
    Script_Animpoint(CScriptGameObject* npc, DataBase::StorageAnimpoint_Data& storage);
    ~Script_Animpoint(void);

#pragma region Cordis Getters
    inline void getAnimationParams(Fvector& position, Fvector& smart_direction) const noexcept
    {
        position = this->m_position;
        smart_direction = this->m_smart_direction;
    }

    inline xr_string getActionName(void) const noexcept { return this->m_current_action_name; }
#pragma endregion

    bool is_position_riched(void);
    void start(void);
    void stop(void);
    void update(void);
    void activate_scheme(void);
private:
    void fill_approved_actions(void);
    void calculate_position(void);

private:
    bool m_is_started;
    bool m_is_camp;
    std::uint16_t m_npc_id;
    std::uint32_t m_vertex_id;
    Fvector m_position;
    Fvector m_look_position;
    Fvector m_smart_direction;
    Fvector m_position_vertex;
    xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>> m_avail_actions;
    DataBase::StorageAnimpoint_Data* m_storage;
    // Lord: добавить объект m_camp;!
    xr_string m_current_action_name;
    xr_string m_cover_name;
};
} // namespace Scripts
} // namespace Cordis
