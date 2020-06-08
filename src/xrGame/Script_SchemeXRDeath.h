#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SchemeXRDeath : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemeXRDeath(void) = delete;
    Script_SchemeXRDeath(CScriptGameObject* const p_client_object, void* storage);
    ~Script_SchemeXRDeath(void);

    virtual void death_callback(CScriptGameObject* const p_client_victim, CScriptGameObject* const p_client_who);

    // @ PRIVATE uses, in XR_LOGIC
    static inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, void* storage)
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

        Msg("[Scripts/add_to_binder(p_client_object, p_ini, scheme_name, section_name, storage)] added "
            "Script_SchemeMobWalker scheme to binder, name=%s scheme=%s section=%s",
            p_client_object->Name(), scheme_name.c_str(), section_name.c_str());

        Script_ISchemeEntity* p_scheme = new Script_SchemeXRDeath(p_client_object, storage);
        DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme);
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

    static inline void reset_death(
        CScriptGameObject* const p_client_object, const xr_string& scheme_name, const xr_string& section_name)
    {
        if (!p_client_object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(p_client_object->ID());
        xr_string death_section_name =
            Globals::Utils::cfg_get_string(storage.getIni(), storage.getSectionLogicName(), "on_death");

        if (!death_section_name.empty())
        {
            if (!storage.getIni()->section_exist(death_section_name.c_str()))
            {
                R_ASSERT2(false, "There is no section for npc");
                return;
            }

            xr_string temporary_name = Globals::Utils::cfg_get_string(storage.getIni(), death_section_name, "on_info");
            if (!temporary_name.empty())
            {
                xr_map<std::uint32_t, CondlistData> not_used_but_called_from_function_for_using_that_condlist =
                    XR_LOGIC::parse_condlist_by_script_object(death_section_name, "death", temporary_name);
            }

            temporary_name = Globals::Utils::cfg_get_string(storage.getIni(), death_section_name, "on_info2");
            if (!temporary_name.empty())
            {
                xr_map<std::uint32_t, CondlistData> not_used_but_called_from_function_for_using_that_condlist =
                    XR_LOGIC::parse_condlist_by_script_object(death_section_name, "death", temporary_name);  
            }
        }
    }
};
} // namespace Scripts
} // namespace Cordis
