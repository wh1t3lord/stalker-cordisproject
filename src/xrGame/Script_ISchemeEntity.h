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
    Script_ISchemeEntity(CScriptGameObject* client_object, DataBase::Script_IComponentScheme* storage_scheme)
        : m_npc(client_object), m_p_true_storage(storage_scheme), m_is_subscribed_action(true), m_scheme_id(0)
    {
        this->m_scheme_id = (++m_generate_scheme_id);
        this->m_id = client_object ? m_npc->ID() : Globals::kUnsignedInt32Undefined;
        if (this->m_id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEW("bad id of npc! %s", this->m_scheme_name.c_str());
        }
    }

    virtual ~Script_ISchemeEntity(void) { this->m_npc = nullptr; }

    inline void subscribe_action(void) noexcept
    {
        MESSAGE("WARNING TO LORD: DELETE THIS FUNCTION!");
        this->m_is_subscribed_action = true;

        xr_string npc_name;
        if (this->m_npc)
            npc_name = this->m_npc->Name();

        MESSAGEI("[Scripts/Script_ISchemeEntity/subscribe_action()] action of npc %s %d %s %d is subscribed!", this->m_scheme_name.c_str(), this->m_scheme_id, npc_name.empty() ? "npc_undefined" : npc_name.c_str(), this->m_id);
    }

    inline void unsubscribe_action(void) noexcept { this->m_is_subscribed_action = false; }
    inline bool isActionSubscribed(void) const noexcept { return this->m_is_subscribed_action; }

    virtual void reset_scheme(const bool value, CScriptGameObject* const p_client_object)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/reset_scheme(value, p_client_object)] WARNING: NOT OVERLOADED FUNCTION!");
    }

    virtual void activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/activate_scheme(is_loading, p_client_object)] WARNING: NOT OVERLOADED "
            "FUNCTION!");
    }

    virtual void update(const float delta)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/update(delta)] WARNING: NOT OVERLOADED FUNCTION!");
    }

    virtual void waypoint_callback(
        CScriptGameObject* p_client_object, const std::uint32_t action_movement_type, const std::uint32_t index)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/waypoint_callback(p_client_object, index)] WARNING: NOT OVERLOADED "
            "FUNCTION!");
        return;
    }

    virtual void update_movement_state(void)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/update_movement_state()] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    virtual void update_standing_state(void)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/update_standing_state()] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    virtual void deactivate(CScriptGameObject* const p_client_object)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/deactivate(p_client_object)] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    virtual void look_at_waypoint(const xr_string& point_name)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/look_at_waypoint(point_name)] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    virtual void look_at_waypoint(const Fvector& point)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/look_at_waypoint(point)] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    virtual void look_at_waypoint(const std::uint32_t choosen_point)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/look_at_waypoint(choosen_point)] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    virtual inline bool is_arrived_to_first_waypoint(void)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/is_arrived_to_first_waypoint] WARNING: NOT OVERLOADED FUNCTION!");
        return false;
    }

    // @ Возвращаем действительное наименование схема которую реализует текущий класс
    virtual inline const xr_string& getSchemeName(void) const noexcept
    {
        if (this->m_scheme_name.empty())
        {
            MESSAGEW("object already deleted!");
            
        }

        return this->m_scheme_name;
    }

    virtual void death_callback(CScriptGameObject* const p_client_victim, CScriptGameObject* const p_client_who)
    {
        MESSAGEW("[Scripts/SCript_ISchemeEntity/death_callback(p_client_victim, p_client_who)] WARNING: NOT OVERLOADED "
            "FUNCTION!");
        return;
    }

    virtual void combat_callback(void)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/combat_callback()] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    virtual void net_destroy(CScriptGameObject* const p_client_object)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/net_destroy(p_client_object)] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    virtual void save(void)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/save()] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    virtual void use_callback(CScriptGameObject* const p_client_object, CScriptGameObject* const p_client_who)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/use_callback(p_client_object, p_client_who)] WARNING: NOT OVERLOADED "
            "FUNCTION!");
        return;
    }

    virtual void hit_callback(CScriptGameObject* const p_client_object, const float amount,
        const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index)
    {
        MESSAGEW("[Scripts/Script_ISchemeEntity/hit_callback(p_client_object, amount, local_direction, p_client_who, "
            "bone_index)] WARNING: NOT OVERLOADED FUNCTION!");
        return;
    }

    // inline void setSchemeName(const xr_string& scheme_name) noexcept { this->m_scheme_name = scheme_name; }

    // @ using for unsubscribing that stuff
    inline std::uint32_t getID(void) const noexcept { return this->m_id; }
    inline std::uint32_t getSchemeID(void) const noexcept { return this->m_scheme_id; }

protected:
    // @ Это используется для удаления запоминается ID нашего npc
    bool m_is_subscribed_action;
    std::uint32_t m_scheme_id; // вот эот используется для unsubscribe
    static std::uint32_t m_generate_scheme_id;
    std::uint32_t m_id;
    CScriptGameObject* m_npc;
    DataBase::Script_IComponentScheme* m_p_true_storage;
    xr_string m_scheme_name;
};
} // namespace Scripts
} // namespace Cordis
