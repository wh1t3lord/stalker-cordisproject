#pragma once

#include "Script_GlobalHelper.h"

namespace Cordis
{
namespace Scripts
{
namespace CRD_DialogManager
{
// @ Private
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

    for (const std::pair<std::uint32_t, DialogData>& pair :
        Script_GlobalHelper::getInstance().getPhraseTable().at(dialog_type_name))
    {
        is_ph = true;
        const std::uint32_t& key = pair.first;
        const DialogData& value = pair.second;
        p_phrase = p_dialog->AddPhrase(value.getName().c_str(), std::to_string(value.getID()).c_str(), "1", -10000);
        if (p_phrase)
        {
            p_script_dialog_helper = p_phrase->GetScriptHelper();
            p_script_dialog_helper->AddPrecondition(
                xr_string("dialog_manager.precondition_").append(dialog_type_name).append("_dialogs").c_str());
            p_script_dialog_helper->AddAction(
                xr_string("dialog_manager.action_").append(dialog_type_name).append("_dialogs").c_str());

            if (value.getWounded() == Globals::kStringTrue)
            {
                p_script_dialog_helper->AddPrecondition("dialogs.is_wounded");
                std::uint32_t medkit_id = generate_id();
                std::uint32_t sorry_id = generate_id();
                std::uint32_t thanks_id = generate_id();

                p_phrase = p_dialog->AddPhrase("dm_wounded_medkit", std::to_string(medkit_id).c_str(),
                    std::to_string(value.getID()).c_str(), -10000);

                p_script_dialog_helper = p_phrase->GetScriptHelper();
                p_script_dialog_helper->AddPrecondition("dialogs.actor_have_medkit");
                p_script_dialog_helper->AddAction("dialogs.transfer_medkit");
                p_script_dialog_helper->AddAction("dialogs.break_dialog");

                p_phrase = p_dialog->AddPhrase("dm_wounded_sorry", std::to_string(sorry_id).c_str(),
                    std::to_string(value.getID()).c_str(), -10000);
                p_script_dialog_helper = p_phrase->GetScriptHelper();
                p_script_dialog_helper->AddAction("dialogs.break_dialog");
            }
        }
        else
        {
            Msg("[Scripts/CRD_DialogManaager/init_start_dialogs(p_dialog, dialog_type_name)] WARNING: p_phrase was "
                "null!");
            p_script_dialog_helper->AddPrecondition("dialogs.is_not_wounded");
        }
    }

    if (!is_ph)
    {
        // Lord: что брать-то в итоге писать ибо там nil...
        Msg("[Scripts/CRD_DialogManager/init_start_dialogs(p_dialog, dialog_type_name)] WARNING: !is_ph == true! "
            "DELETE THIS AFTER TESTING IS REALLY NEEDED CODE HERE!!!");
    }
}

// @ PRIVATE
inline void _fill_priority_table(
    CScriptGameObject* npc, const xr_map<std::uint32_t, DialogData>& phrase_by_category_table, const xr_string& category_priority_table_name)
{
    const std::uint16_t& npc_id = npc->ID();
}

inline int _calculate_priority(const DialogData& data, CScriptGameObject* npc, const std::uint32_t phrase_id, const xr_string& category_name) 
{

}

inline void init_hello_dialogs(CPhraseDialog* p_dialog)
{
    if (!p_dialog)
    {
        R_ASSERT2(false, "object is empty!");
        return;
    }

    init_start_dialogs(p_dialog, "hello");
}

inline void fill_priority_hello_table(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_string& dialog_name, const xr_string& phrase_id_name)
{
    _fill_priority_table(npc, Script_GlobalHelper::getInstance().getPhraseTable().at("hello"), "hello");
}

} // namespace CRD_DialogManager
} // namespace Scripts
} // namespace Cordis
