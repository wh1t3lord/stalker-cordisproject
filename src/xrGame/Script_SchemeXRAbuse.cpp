#include "stdafx.h"
#include "Script_SchemeXRAbuse.h"

namespace Cordis
{
namespace Scripts
{
Script_EvaluatorAbuse::_value_type Script_EvaluatorAbuse::evaluate(void)
{
    if (!this->m_p_storage->getXRAbuseManager())
    {
        R_ASSERT2(false, "can't be! Do you initialize and set it in add_to_binder function?");
        return false;
    }

    return this->m_p_storage->getXRAbuseManager()->update();
}

void Script_SchemeXRAbuse::execute(void) 
{ CScriptActionBase::execute(); }

void Script_SchemeXRAbuse::finalize(void) { CScriptActionBase::finalize(); }

void Script_SchemeXRAbuse::initialize(void) 
{
    CScriptActionBase::initialize();
    // @ Можете доделать!
}

void Script_SchemeXRAbuse::add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
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

    Msg("[Scripts/add_to_binder(p_client_object, p_ini, scheme_name, section_name, storage)] added "
        "Script_SchemeMobWalker scheme to binder, name=%s scheme=%s section=%s",
        p_client_object->Name(), scheme_name.c_str(), section_name.c_str());
}


void Script_SchemeXRAbuse::set_abuse(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name)
{
}

void Script_SchemeXRAbuse::add_abuse(CScriptGameObject* const p_client_object, const float value)
{
    if (!p_client_object)
    {
        Msg("[Scripts/Script_SchemeXRAbuse/add_abuse(p_client_object, value)] WARNING: can't add because "
            "p_client_object == nullptr!");
        return;
    }

    if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().find("abuse") !=
        DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().end())
    {
        DataBase::Storage::getInstance()
            .getStorage()
            .at(p_client_object->ID())
            .getSchemes()
            .at("abuse")
            .getXRAbuseManager()
            ->AddAbuse(value);
    }
    else
    {
        Msg("[Scripts/Script_SchemeXRAbuse/add_abuse(p_client_object, value)] wARNING: can't add because abuse "
            "scheme doesnt exist!");
        return;
    }
}

void Script_SchemeXRAbuse::clear_abuse(CScriptGameObject* const p_client_object)
{
    if (!p_client_object)
    {
        Msg("[Scripts/Script_SchemeXRAbuse/clear_abuse(p_client_object)] WARNING: can't add because "
            "p_client_object == nullptr!");
        return;
    }

    if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().find("abuse") !=
        DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().end())
    {
        DataBase::Storage::getInstance()
            .getStorage()
            .at(p_client_object->ID())
            .getSchemes()
            .at("abuse")
            .getXRAbuseManager()
            ->ClearAbuse();
    }
    else
    {
        Msg("[Scripts/Script_SchemeXRAbuse/clear_abuse(p_client_object)] wARNING: can't add because abuse "
            "scheme doesnt exist!");
        return;
    }
}

void Script_SchemeXRAbuse::disable_abuse(CScriptGameObject* const p_client_object)
{
    if (!p_client_object)
    {
        Msg("[Scripts/Script_SchemeXRAbuse/disable_abuse(p_client_object)] WARNING: can't add because "
            "p_client_object == nullptr!");
        return;
    }

    if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().find("abuse") !=
        DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().end())
    {
        DataBase::Storage::getInstance()
            .getStorage()
            .at(p_client_object->ID())
            .getSchemes()
            .at("abuse")
            .getXRAbuseManager()
            ->DisableAbuse();
    }
    else
    {
        Msg("[Scripts/Script_SchemeXRAbuse/disable_abuse(p_client_object)] wARNING: can't add because abuse "
            "scheme doesnt exist!");
        return;
    }
}

void Script_SchemeXRAbuse::enable_abuse(CScriptGameObject* const p_client_object)
{
    if (!p_client_object)
    {
        Msg("[Scripts/Script_SchemeXRAbuse/enable_abuse(p_client_object)] WARNING: can't add because "
            "p_client_object == nullptr!");
        return;
    }

    if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().find("abuse") !=
        DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().end())
    {
        DataBase::Storage::getInstance()
            .getStorage()
            .at(p_client_object->ID())
            .getSchemes()
            .at("abuse")
            .getXRAbuseManager()
            ->EnableAbuse();
    }
    else
    {
        Msg("[Scripts/Script_SchemeXRAbuse/enable_abuse(p_client_object)] wARNING: can't add because abuse "
            "scheme doesnt exist!");
        return;
    }
}

} // namespace Scripts
} // namespace Cordis
