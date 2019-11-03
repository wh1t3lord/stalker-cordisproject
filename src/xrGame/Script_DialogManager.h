#include "Script_GlobalDefinitions.h"
#pragma once

namespace Cordis
{
namespace Scripts
{
namespace CRD_DialogManager
{
inline void init_start_dialogs(CPhraseDialog* dialog, const xr_string& dialog_type_name)
{
    if (!dialog)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    xr_string dlg_type_name = dialog_type_name;
    if (dlg_type_name.empty())
    {
        Msg("[Scripts/CRD_DialogManager/init_start_dialogs(dialog, dialog_type_name)] WARNING: dialog_type_name.empty() == true! So set to a default value {hello}");
        dlg_type_name = "hello";
    }
}

inline void init_hello_dialogs(CPhraseDialog* dialog)
{
    if (!dialog)
    {
        R_ASSERT2(false, "object is empty!");
        return;
    }

    init_start_dialogs(dialog);
}

} // namespace CRD_DialogManager
} // namespace Scripts
} // namespace Cordis
