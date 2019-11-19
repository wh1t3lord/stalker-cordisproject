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

    virtual void reset_scheme(void) { Msg("[Scripts/Script_ISchemeEntity/reset_scheme()] NOT OVERLOADED FUNCTION!"); }

    virtual void update(const float delta)
    {
        Msg("[Scripts/Script_ISchemeEntity/update(delta)] NOT OVERLOADED FUNCTION!");
    }

    // @ Возвращаем действительное наименование схема которую реализует текущий класс
    inline const xr_string& getSchemeName(void) const noexcept { return this->m_scheme_name; }

    inline void setSchemeName(const xr_string& scheme_name) noexcept { this->m_scheme_name = scheme_name; }

    // @ using for unsubscribing that stuff
    inline std::uint32_t getID(void) const noexcept { return this->m_id; }

private:
    // @ Это используется для удаления запоминается ID нашего npc
    std::uint32_t m_id;
    CScriptGameObject* m_npc;
    DataBase::Storage_Scheme* m_storage;
    xr_string m_scheme_name;
};
} // namespace Scripts
} // namespace Cordis
