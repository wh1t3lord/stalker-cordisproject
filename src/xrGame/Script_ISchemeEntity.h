#pragma once

namespace Cordis
{
namespace Scripts
{
// @ Main interface for schemes
// @ Супер класс
// @ Добавлять методы сюда для последующих схем которые могут существовать!
class Script_ISchemeEntity
{
public:
    Script_ISchemeEntity(void) = delete;
    Script_ISchemeEntity(CScriptGameObject* client_object, DataBase::Storage_Scheme& storage_scheme)
        : m_npc(client_object), m_storage(&storage_scheme), m_id(m_npc->ID())
    {
    }

    virtual ~Script_ISchemeEntity(void) { this->m_npc = nullptr; }

    virtual void reset_scheme(void)
    {
        Msg("[Scripts/Script_ISchemeEntity/reset_scheme()] WARNING: NOT OVERLOADED FUNCTION!");
    }

    virtual void update(const float delta)
    {
        Msg("[Scripts/Script_ISchemeEntity/update(delta)] WARNING: NOT OVERLOADED FUNCTION!");
    }

    virtual void waypoint_callback(CScriptGameObject* p_client_object, const std::uint32_t index)
    {
        Msg("[Scripts/Script_ISchemeEntity/waypoint_callback(p_client_object, index)] WARNING: NOT OVERLOADED "
            "FUNCTION!");
        return;
    }

    virtual void update_movement_state(void)
    {
        Msg("[Scripts/Script_ISchemeEntity/update_movement_state()] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    virtual void update_standing_state(void)
    {
        Msg("[Scripts/Script_ISchemeEntity/update_standing_state()] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    virtual void deactivate(void)
    {
        Msg("[Scripts/Script_ISchemeEntity/deactivate()] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    virtual void look_at_waypoint(const xr_string& point_name)
    {
        Msg("[Scripts/Script_ISchemeEntity/look_at_waypoint(point_name)] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    virtual void look_at_waypoint(const Fvector& point) 
    {
        Msg("[Scripts/Script_ISchemeEntity/look_at_waypoint(point)] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    virtual inline bool is_arrived_to_first_waypoint(void)
    {
        Msg("[Scripts/Script_ISchemeEntity/is_arrived_to_first_waypoint] WARNING: NOT OVERLOADED FUNCTION!");
        return false;
    }

    virtual inline 

    // @ Возвращаем действительное наименование схема которую реализует текущий класс
    inline const xr_string& getSchemeName(void) const noexcept
    {
        if (this->m_scheme_name.empty())
        {
            R_ASSERT2(false, "it can't be, you must initialize you m_scheme_name field in your scheme class!!!!!");
        }

        return this->m_scheme_name;
    }

    // inline void setSchemeName(const xr_string& scheme_name) noexcept { this->m_scheme_name = scheme_name; }

    // @ using for unsubscribing that stuff
    inline std::uint32_t getID(void) const noexcept { return this->m_id; }

protected:
    // @ Это используется для удаления запоминается ID нашего npc
    std::uint32_t m_id;
    CScriptGameObject* m_npc;
    DataBase::Storage_Scheme* m_storage;
    xr_string m_scheme_name;
};
} // namespace Scripts
} // namespace Cordis
