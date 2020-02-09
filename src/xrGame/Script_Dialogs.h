#include "Script_GlobalDefinitions.h"
#pragma once

namespace Cordis
{
namespace Scripts
{
inline CScriptGameObject* who_is_npc(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* p_result = p_second_speaker;
    if (DataBase::Storage::getInstance().getActor()->ID() == p_result->ID())
        p_result = p_first_speaker;

    return p_result;
}

inline CScriptGameObject* who_is_actor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* p_result = p_second_speaker;

    if (DataBase::Storage::getInstance().getActor()->ID() != p_result->ID())
        p_result = p_first_speaker;

    return p_result;
}

inline void relocate_money_from_actor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker, const int value)
{
    CScriptGameObject* p_victim = who_is_npc(p_first_speaker, p_second_speaker);

    if (!p_victim)
    {
        R_ASSERT2(false, "object is null!");
    }

    DataBase::Storage::getInstance().getActor()->TransferMoney(value, p_victim);
    Script_NewsManager::getInstance().relocate_money("out", value);
}

inline void relocate_money_to_actor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker, const int value)
{
    DataBase::Storage::getInstance().getActor()->GiveMoney(value);
    Script_NewsManager::getInstance().relocate_money("in", value);
}

inline void relocate_money(CScriptGameObject* const p_victim, const int number, const xr_string& type_name)
{
    if (DataBase::Storage::getInstance().getActor())
    {
        if (type_name == "in")
        {
            DataBase::Storage::getInstance().getActor()->GiveMoney(number);
            // Lord: game_stats когда будет
        }
        else if (type_name == "out")
        {
            if (!p_victim)
            {
                R_ASSERT2(false, "Couldn't relocate money to NULL");
            }

            DataBase::Storage::getInstance().getActor()->TransferMoney(number, p_victim);
            // Lord: game_stats когда будет
        }

        Script_NewsManager::getInstance().relocate_money(type_name, number);
    }
}

inline void relocate_item_section(
    CScriptGameObject* const p_npc, const xr_string& section_name, const xr_string& type_name, const int amount)
{
    if (DataBase::Storage::getInstance().getActor())
    {
        int amnt = 0;
        if (!amount)
            amnt = 1;

        for (std::uint32_t i = 1; i <= amnt; ++i)
        {
            if (type_name == "in")
            {
                Globals::Game::alife_create(section_name, DataBase::Storage::getInstance().getActor()->Position(),
                    DataBase::Storage::getInstance().getActor()->level_vertex_id(),
                    DataBase::Storage::getInstance().getActor()->game_vertex_id(),
                    DataBase::Storage::getInstance().getActor()->ID());
            }
            else if (type_name == "out")
            {
                if (!p_npc)
                {
                    Msg("[Scripts/relocate_item_section(p_npc, section_name, type_name, amount)] WARNING: can't reloca item when p_npc is nullptr!");
                    return;
                }

                DataBase::Storage::getInstance().getActor()->TransferItem(
                    DataBase::Storage::getInstance().getActor()->GetObjectByName(section_name.c_str()), p_npc);
            }

            Script_NewsManager::getInstance().relocate_item(type_name, section_name, amount);
        }
    }
}

inline void relocate_item_section_from_actor(CScriptGameObject* const p_first_speaker,
    CScriptGameObject* const p_second_speaker, const xr_string& section_name, const xr_string& amount_name)
{
    CScriptGameObject* p_npc = who_is_npc(p_first_speaker, p_second_speaker);
    CScriptGameObject* p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    int i = 0;
    std::uint32_t _amount = 0;

    auto local_transfer_object_item = [&](CScriptGameObject* const p_item) -> void {
        if (p_item->Section() == section_name.c_str() && i != 0)
        {
            p_actor->TransferItem(p_item, p_npc);
            --i;
        }
    };

    if (amount_name == "all")
    {
        i = -1;
        p_actor->IterateInventory(local_transfer_object_item);
        _amount = (i + 1) * (-1);
        i = 0;
    }
    else
    {
        _amount = boost::lexical_cast<std::uint32_t>(amount_name);
        if (!_amount)
            _amount = 1;

        if (_amount > 1)
        {
            i = _amount;
            p_actor->IterateInventory(local_transfer_object_item);
        }
        else if (_amount == Globals::kUnsignedInt32Undefined)
        {
            R_ASSERT2(false, "bad value!");
            return;
        }
        else
        {
            p_actor->TransferItem(p_actor->GetObjectByName(section_name.c_str()), p_npc);
        }
    }

    if (i != 0)
    {
        R_ASSERT2(false, "something is not right. not all items transfer check your code!!!!");
    }

    // Lord: доделать когда будет death_manager
}

inline void relocate_item_section_to_actor(CScriptGameObject* const p_first_speaker,
    CScriptGameObject* const p_second_speaker, const xr_string& section_name, const std::uint32_t amount)
{
    std::uint32_t current_amount = amount;
    if (current_amount == Globals::kUnsignedInt32Undefined)
    {
        Msg("[Scripts/relocate_item_section_to_actor(p_first_speaker, p_second_speaker, section_name, amount)] "
            "WARNING: amount was undefined set to 1. Will be deleted ONLY ONE!");
        current_amount = 1;
    }

    if (current_amount == 0)
    {
        Msg("[Scripts/relocate_item_section_to_actor(p_first_speaker, p_second_speaker, section_name, amount)] "
            "WARNING: amount was NULL set to 1. Will be deleted ONLY ONE!");
        current_amount = 1;
    }

    CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
    std::uint32_t item_counter = 0;
    auto local_transfer_object_item = [&](CScriptGameObject* const p_item) -> void {
        if (p_item->Section() == section_name && item_counter != 0)
        {
            p_npc->TransferItem(p_item, DataBase::Storage::getInstance().getActor());
            --item_counter;
        }
    };

    if (current_amount > 1)
    {
        item_counter = current_amount;
        p_npc->IterateInventory(local_transfer_object_item);
    }
    else
    {
        if (p_npc->GetObjectByName(section_name.c_str()))
        {
            p_npc->TransferItem(
                p_npc->GetObjectByName(section_name.c_str()), DataBase::Storage::getInstance().getActor());
        }
        else
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
            Globals::Game::alife_create(section_name, p_actor->Position(), p_actor->level_vertex_id(),
                p_actor->game_vertex_id(), p_actor->ID());
        }
    }

    if (item_counter != 0)
    {
        CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
        for (std::uint32_t i = 0; i < item_counter; ++i)
        {
            Globals::Game::alife_create(section_name, p_actor->Position(), p_actor->level_vertex_id(),
                p_actor->game_vertex_id(), p_actor->ID());
        }
    }

    // Lord: доделать когда будет death_manager

    Script_NewsManager::getInstance().relocate_item("in", section_name, current_amount);
}

} // namespace Scripts
} // namespace Cordis
