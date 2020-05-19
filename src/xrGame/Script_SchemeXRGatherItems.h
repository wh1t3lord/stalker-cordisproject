#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_EvaluatorGatherItems : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorGatherItems(const xr_string& evaluator_name, DataBase::Storage_Scheme& storage)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(&storage)
    {
    }

    ~Script_EvaluatorGatherItems(void) {}

    virtual _value_type evaluate(void)
    {
        if (!this->m_p_storage->isXRGatherItemsEnabled())
            return false;

        return this->m_object->is_there_items_to_pickup();
    }

    // @ PRIVATE uses, in XR_LOGIC
    static inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, DataBase::Storage_Scheme& storage)
    {
        if (!p_client_object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        if (!p_ini)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        MESSAGEI("added scheme to binder, name=%s scheme=%s section=%s",
            p_client_object->Name(), scheme_name.c_str(), section_name.c_str());

        CScriptActionPlanner* p_planner = Globals::get_script_action_planner(p_client_object);

        if (!p_planner)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        p_planner->remove_evaluator(StalkerDecisionSpace::eWorldPropertyItems);
        p_planner->add_evaluator(StalkerDecisionSpace::eWorldPropertyItems,
            new Script_EvaluatorGatherItems("is_there_items_to_pickup", storage));
    }

    static void set_gather_items(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

    static inline void reset_gather_items(
        CScriptGameObject* const p_client_object, const xr_string& scheme_name, const xr_string& section_name)
    {
        if (!p_client_object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(p_client_object->ID());

        if (storage.getSchemes().find("gather_items") == storage.getSchemes().end())
        {
            R_ASSERT2(false, "can't be!");
            return;
        }

        bool result = true;

        if (storage.getIni()->line_exist(section_name.c_str(), "gather_items_enabled"))
            result = Globals::Utils::cfg_get_bool(storage.getIni(), section_name, "gather_items_enabled");

        storage.getSchemes().at("gather_items")->setXRGatherItemsEnabled(result);
    }

private:
    DataBase::Storage_Scheme* m_p_storage;
};
} // namespace Scripts
} // namespace Cordis
