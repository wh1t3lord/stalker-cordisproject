#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_EvaluatorCheckCombat : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorCheckCombat(const xr_string& evaluator_name, DataBase::Script_ComponentScheme_XRCombat* storage)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(storage)
    {
    }

    ~Script_EvaluatorCheckCombat(void) {}

    virtual _value_type evaluate(void);

private:
    DataBase::Script_ComponentScheme_XRCombat* m_p_storage;
};

class Script_EvaluatorEnemy : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorEnemy(const xr_string& evaluator_name, DataBase::Script_ComponentScheme_XRCombat* storage)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(storage)
    {
    }
    ~Script_EvaluatorEnemy(void) {}

    virtual _value_type evaluate(void);

private:
    DataBase::Script_ComponentScheme_XRCombat* m_p_storage;
};

namespace XR_COMBAT
{
// @ PRIVATE uses, in XR_LOGIC
    static void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, void* storage);

    static void set_combat_checker(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

static void disable_scheme(CScriptGameObject* const p_client_object, const xr_string& scheme_name) 
{
    if (p_client_object == nullptr)
    {
        MESSAGEWR("invalid object!");
        return;
    }

    if (scheme_name.empty())
    {
        MESSAGEWR("invalid scheme name!");
        return; 
    }

    if (DataBase::Storage::getInstance().getStorage().find(p_client_object->ID()) == DataBase::Storage::getInstance().getStorage().end())
    {
        MESSAGEWR("invalid storage for %d", p_client_object->ID());
        return;
    }

    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(p_client_object->ID());

    if (storage.getSchemes().find(scheme_name) == storage.getSchemes().end())
    {
        MESSAGEWR("Can't find scheme by name %s", scheme_name.c_str());
        return;
    }

    DataBase::Script_ComponentScheme_XRCombat* const p_storage_scheme = static_cast<DataBase::Script_ComponentScheme_XRCombat*>(storage.getSchemes().at(scheme_name));

    if (p_storage_scheme == nullptr)
    {
        MESSAGEWR("invalid storage!");
        return;
    }

    p_storage_scheme->setEnabled(false);
}

inline void set_combat_type(
    CScriptGameObject* const p_npc, CScriptGameObject* const p_actor, const xr_map<std::uint32_t, CondlistData>& condlist)
{
    if (condlist.empty())
        return;

    if (!p_npc)
    {
        Msg("p_npc == nullptr!");
        return;
    }

    DataBase::Storage::getInstance().setStorageEnemy(p_npc->ID(), p_npc->GetBestEnemy());
    xr_string script_combat_type_name;

    script_combat_type_name = XR_LOGIC::pick_section_from_condlist(p_actor, p_npc, condlist);
    
    DataBase::Storage::getInstance().setStorageScriptCombatTypeName(p_npc->ID(), script_combat_type_name);
}

} // namespace XR_COMBAT

} // namespace Scripts
} // namespace Cordis
