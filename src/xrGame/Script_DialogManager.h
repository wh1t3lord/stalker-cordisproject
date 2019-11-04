#include "Script_GlobalDefinitions.h"
#pragma once

namespace Cordis
{
namespace Scripts
{
namespace CRD_DialogManager
{
inline void init_start_dialogs(CPhraseDialog* p_dialog, const xr_string& dialog_type_name)
{
    if (!p_dialog)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    xr_string dlg_type_name = dialog_type_name;
    if (dlg_type_name.empty())
    {
        Msg("[Scripts/CRD_DialogManager/init_start_dialogs(p_dialog, dialog_type_name)] WARNING: "
            "dialog_type_name.empty() == true! So set to a default value {hello}");
        dlg_type_name = "hello";
    }

    CPhrase* p_phrase = p_dialog->AddPhrase("", "0", "", -10000);

    if (!p_phrase)
    {
        R_ASSERT2(false, "something goes wrong!");
        return;
    }

    p_phrase = p_dialog->AddPhrase("", "1", "0", -10000);
    CDialogScriptHelper* p_script_dialog_helper = p_phrase->GetScriptHelper();

    if (!p_script_dialog_helper)
    {
        R_ASSERT2(false, "something goes wrong!");
        return;
    }

    p_script_dialog_helper->AddAction(
        xr_string("dialog_manager.fill_priority_").append(dialog_type_name).append("_table").c_str());
    bool is_ph = false;

    
}

inline void init_hello_dialogs(CPhraseDialog* p_dialog)
{
    if (!p_dialog)
    {
        R_ASSERT2(false, "object is empty!");
        return;
    }

    init_start_dialogs(p_dialog);
}

} // namespace CRD_DialogManager
} // namespace Scripts
} // namespace Cordis
