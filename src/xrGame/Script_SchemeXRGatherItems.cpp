#include "stdafx.h"
#include "Script_SchemeXRGatherItems.h"

namespace Cordis
{
namespace Scripts
{
void Script_EvaluatorGatherItems::set_gather_items(CScriptGameObject* const p_client_object,
    CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Script_ComponentScheme_XRGatherItems* p_storage = XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_XRGatherItems>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }
}

} // namespace Scripts
} // namespace Cordis
