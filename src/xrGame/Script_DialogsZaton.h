#include "Script_GlobalDefinitions.h"
#pragma once

namespace Cordis
{
namespace Scripts
{
inline bool zat_b30_owl_stalker_trader_actor_has_item_to_sell(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    xr_vector<xr_string> items;
    xr_map<xr_string, xr_string> info_table;

    items.push_back("zat_b20_noah_pda");
    items.push_back("zat_b40_notebook");
    items.push_back("zat_b40_pda_1");
    items.push_back("zat_b40_pda_2");
    items.push_back("pri_b36_monolith_hiding_place_pda");
    items.push_back("pri_b306_envoy_pda");
    items.push_back("jup_b46_duty_founder_pda");
    items.push_back("jup_b207_merc_pda_with_contract");
    items.push_back("device_pda_zat_b5_dealer");
    items.push_back("jup_b10_notes_01");
    items.push_back("jup_b10_notes_02");
    items.push_back("jup_b10_notes_03");
    items.push_back("jup_a9_evacuation_info");
    items.push_back("jup_a9_meeting_info");
    items.push_back("jup_a9_losses_info");
    items.push_back("jup_a9_delivery_info");
    items.push_back("zat_b12_documents_1");
    items.push_back("zat_b12_documents_2");
    items.push_back("device_flash_snag");
    items.push_back("jup_b202_bandit_pda");
    items.push_back("device_pda_port_bandit_leader");
    items.push_back("jup_b10_ufo_memory_2");
    items.push_back("jup_b1_half_artifact");
    items.push_back("af_quest_b14_twisted");
    items.push_back("af_oasis_heart");
    items.push_back("detector_scientific");

    info_table["jup_b1_half_artifact"] = "zat_b30_owl_stalker_about_halfart_jup_b6_asked";
    info_table["af_quest_b14_twisted"] = "zat_b30_owl_stalker_about_halfart_zat_b14_asked";
    info_table["af_oasis_heart"] = "zat_b30_owl_stalker_trader_about_osis_art";
    info_table["detector_scientific"] = "zat_b30_owl_detectors_approached";

    for (const xr_string& it : items)
    {
        if (DataBase::Storage::getInstance().getActor()->GetObjectByName(it.c_str()))
        {
            if (it == "detector_scientific" && !Globals::has_alife_info("zat_b30_second_detector"))
            {
                // empty code here
            }
            else
            {
                if (info_table.find(it) != info_table.end())
                {
                    if (!Globals::has_alife_info(info_table.at(it).c_str()))
                    {
                        return true;
                    }
                }
                else
                {
                    return true;
                }
            }
        }
    }

    return false;
}

inline bool zat_b30_owl_can_say_about_heli(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    int count = 3;
    xr_vector<xr_string> data;
    data.push_back("zat_b28_heli_3_searched");
    data.push_back("zat_b100_heli_2_searched");
    data.push_back("zat_b101_heli_5_searched");
    data.push_back("zat_b30_owl_scat_1");
    data.push_back("zat_b30_owl_scat_2");
    data.push_back("zat_b30_owl_scat_3");

    for (const xr_string& it : data)
    {
        if (Globals::has_alife_info(it.c_str()))
        {
            --count;
        }
    }

    return (count > 0);
}

inline bool zat_b30_actor_has_1000(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->Money() >= 1000);
}

inline bool zat_b30_actor_has_200(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->Money() >= 200);
}

inline bool zat_b30_actor_has_pri_b36_monolith_hiding_place_pda(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->GetObjectByName("pri_b36_monolith_hiding_place_pda"));
}

inline bool zat_b30_actor_has_pri_b306_envoy_pda(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->GetObjectByName("pri_b306_envoy_pda"));
}

inline bool zat_b30_actor_has_jup_b10_strelok_notes_1(
    CScriptAbstractAction* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b10_notes_01"));
}

inline bool zat_b30_actor_has_jup_b10_strelok_notes_2(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b10_notes_02"));
}

inline bool zat_b30_actor_has_jup_b10_strelok_notes_3(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b10_notes_03"));
}

inline bool zat_b30_actor_has_detector_scientific(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->GetObjectByName("detector_scientific"));
}

inline bool zat_b30_actor_has_device_flash_snag(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->GetObjectByName("device_flash_snag"));
}

inline bool zat_b30_actor_has_device_pda_port_bandit_leader(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->GetObjectByName(
        "zat_b30_actor_has_device_pda_port_bandit_leader"));
}

inline bool zat_b30_actor_has_jup_b10_ufo_memory(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b10_ufo_memory_2"));
}

inline bool zat_b30_actor_has_jup_b202_bandit_pda(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b202_bandit_pda"));
}

inline void zat_b30_transfer_1000(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 1000);
}

inline void zat_b30_transfer_200(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 200);
}

inline void zat_b30_sell_pri_b36_monolith_hiding_place_pda(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "pri_b36_monolith_hiding_place_pda");
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 5000);
}

inline void zat_b30_sell_pri_b306_envoy_pda(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "pri_b306_envoy_pda");
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 4000);
}

inline void zat_b30_sell_jup_b207_merc_pda_with_contract(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b207_merc_pda_with_contract");
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 1000);
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b207_merc_pda_with_contract_sold");
}

inline void zat_b30_sell_jup_b10_strelok_notes_1(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b10_notes_01");
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 500);
}

inline void zat_b30_sell_jup_b10_strelok_notes_2(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b10_notes_02");
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 500);
}

inline void zat_b30_sell_jup_b10_strelok_notes_3(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b10_notes_03");
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 500);
}

/* NOT USED
inline void jup_a9_owl_stalker_trader_sell_jup_a9_evacuation_info(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_evacuation_info");
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 750);
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a9_evacuation_info_sold");
}

inline void jup_a9_owl_stalker_trader_sell_jup_a9_meeting_info(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_meeting_info");
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 750);
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a9_meeting_info_sold");
}

inline void jup_a9_owl_stalker_trader_sell_jup_a9_losses_info(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_losses_info");
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 750);
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a9_losses_info_sold");
}

inline void jup_a9_owl_stalker_trader_sell_jup_a9_delivery_info(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_delivery_info");
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 750);
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a9_delivery_info_sold");
}*/

inline void zat_b30_owl_stalker_trader_sell_device_flash_snag(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "device_flash_snag");
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 200);
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("device_flash_snag_sold");
}

inline void zat_b30_owl_stalker_trader_sell_device_pda_port_bandit_leader(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "device_pda_port_bandit_leader");
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 1000);
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("device_pda_port_bandit_leader_sold");
}

inline void zat_b30_owl_stalker_trader_sell_jup_b10_ufo_memory(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b10_ufo_memory_2");
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 500);
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b10_ufo_memory_2_sold");
}

inline void zat_b30_owl_stalker_trader_sell_jup_b202_bandit_pda(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b202_bandit_pda");
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 500);
}

inline void zat_b14_bar_transfer_money(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 1000);
}

inline void zat_b14_transfer_artefact(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "af_quest_b14_twisted");
}

inline bool zat_actor_has_artefact(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(p_first_speaker->GetObjectByName("af_quest_b14_twisted"));
}

inline bool zat_actor_hasnt_artefact(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !(zat_actor_has_artefact(p_first_speaker, p_second_speaker));
}

inline void zat_b7_give_bandit_reward_to_actor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    std::uint32_t amount = Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(15, 30) * 100;
    relocate_money_from_actor(p_first_speaker, p_second_speaker, amount);
    Script_TreasureManager::getInstance().give_treasure("zat_hiding_place_30");
}

inline void zat_b7_give_stalker_reward_to_actor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    std::uint16_t amount = Globals::Script_RandomInt::getInstance().Generate<std::uint16_t>(1, 3);
    switch (amount)
    {
    case 1: {
        relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "bandage", 6);
        relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "vodka", 4);
        break;
    }
    case 2: {
        relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "medkit", 2);
        relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "vodka", 4);
        break;
    }
    case 3: {
        relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "antirad", 3);
        relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "vodka", 4);
        break;
    }
    }

    Script_TreasureManager::getInstance().give_treasure("zat_hiding_place_29");
}

inline void zat_b7_give_stalker_reward_2_to_actor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "bandage", 4);
    relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "medkit", 2);
    relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "antirad", 2);
}

inline void zat_b7_rob_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    std::uint32_t amount = iFloor(DataBase::Storage::getInstance().getActor()->Money() *
        (Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(75, 100) / 100));
    if (DataBase::Storage::getInstance().getActor()->Money() < amount)
        amount = DataBase::Storage::getInstance().getActor()->Money();

    relocate_money_from_actor(p_first_speaker, p_second_speaker, amount);
}

inline bool zat_b7_killed_self_precond(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info("zat_b7_stalkers_raiders_meet") ||
        Globals::has_alife_info("zat_b7_victims_disappeared"))
        return false;

    if (XR_CONDITION::is_squad_exist_client(nullptr, nullptr, {"zat_b7_stalkers_victims_1"}))
        return false;

    return true;
}

inline bool zat_b7_squad_alive(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (XR_CONDITION::is_squad_exist_client(nullptr, nullptr, {"zat_b7_stalkers_victims_1"}))
        return true;

    return false;
}

inline void zat_b103_transfer_merc_supplies(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    int i = 6;
    int j = 0;
    xr_string section_name;
    xr_vector<xr_string> item_sections = {"conserva", "kolbasa", "bread"};

    auto local_transfer_object_item = [&](CScriptGameObject* const p_item) -> void {
        if (p_item->Section() == section_name && i != 0)
        {
            p_actor->TransferItem(p_item, p_npc);
            --i;
        }
    };

    for (const xr_string& it : item_sections)
    {
        section_name = it;
        j = i;
        p_actor->IterateInventory(local_transfer_object_item);
        if ((j - i) != 0)
            Script_NewsManager::getInstance().relocate_item("out", section_name, j - i);
    }
}

inline void zat_b103_transfer_mechanic_toolkit_2(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "toolkit_2");
}

inline bool check_npc_name_mechanics(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return ((!XR_CONDITION::check_npc_name_client(p_first_speaker, p_second_speaker, {"mechanic"})) &&
        (!XR_CONDITION::check_npc_name_client(p_first_speaker, p_second_speaker, {"zat_b103_lost_merc"})) &&
        (!XR_CONDITION::check_npc_name_client(p_first_speaker, p_second_speaker, {"tech"})) &&
        (!XR_CONDITION::check_npc_name_client(p_first_speaker, p_second_speaker, {"zulus"})) &&
        (XR_CONDITION::check_npc_name_client(p_first_speaker, p_second_speaker, {"stalker"})));
}

inline void zat_b33_set_counter_10(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    XR_EFFECTS::set_counter(p_actor, nullptr, {"zat_b33_items", "10"});
}

inline bool zat_b33_counter_ge_2(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    return XR_CONDITION::is_counter_greater_client(p_actor, nullptr, {"zat_b33_items", "1"});
}

inline bool zat_b33_counter_ge_4(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    return XR_CONDITION::is_counter_greater_client(p_actor, nullptr, {"zat_b33_items", "3"});
}

inline bool zat_b33_counter_ge_8(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    return XR_CONDITION::is_counter_greater_client(p_actor, nullptr, {"zat_b33_items", "7"});
}

inline bool zat_b33_counter_le_2(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !(zat_b33_counter_ge_2(p_first_speaker, p_second_speaker));
}

inline bool zat_b33_counter_le_4(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !(zat_b33_counter_ge_4(p_first_speaker, p_second_speaker));
}

inline bool zat_b33_counter_le_8(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !(zat_b33_counter_le_8(p_first_speaker, p_second_speaker));
}

inline void zat_b33_counter_de_2(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    XR_EFFECTS::dec_counter(p_actor, nullptr, {"zat_b33_items", "2"});
}

inline void zat_b33_counter_de_4(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    XR_EFFECTS::dec_counter(p_actor, nullptr, {"zat_b33_items", "4"});
}

inline void zat_b33_counter_de_8(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    XR_EFFECTS::dec_counter(p_actor, nullptr, {"zat_b33_items", "8"});
}

inline bool zat_b33_counter_eq_10(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    return XR_CONDITION::is_counter_equal_client(p_actor, nullptr, {"zat_b33_items", "10"});
}

inline bool zat_b33_counter_ne_10(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !(zat_b33_counter_eq_10(p_first_speaker, p_second_speaker));
}

inline void zat_b33_transfer_first_item(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    relocate_item_section(p_actor, "wpn_fort_snag", "in");
}

inline void zat_b33_transfer_second_item(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    relocate_item_section(p_actor, "medkit_scientic", "in", 3);
    relocate_item_section(p_actor, "antirad", "in", 3);
    relocate_item_section(p_actor, "bandage", "in", 5);
}

inline void zat_b33_transfer_third_item(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    relocate_item_section(p_actor, "wpn_ak74u_snag", "in");
}

inline void zat_b33_transfer_fourth_item(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    relocate_item_section(p_first_speaker, "af_soul", "in");
}

inline void zat_b33_transfer_fifth_item(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    relocate_item_section(p_actor, "helm_hardhat_snag", "in");
}

inline void zat_b33_transfer_safe_container(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
    relocate_item_section(p_npc, "zat_b33_safe_container", "out");
}

inline bool zat_b33_aractor_has_habar(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("zat_b33_safe_container"));
}

inline bool zat_b33_actor_hasnt_habar(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !(!!(zat_b33_aractor_has_habar(p_first_speaker, p_second_speaker)));
}

inline bool zat_b33_actor_has_needed_money(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    std::uint32_t zat_b33_tech_money = 500;
    return (DataBase::Storage::getInstance().getActor()->Money() >= zat_b33_tech_money);
}

inline bool zat_b33_actor_hasnt_needed_money(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !(zat_b33_actor_hasnt_needed_money(p_first_speaker, p_second_speaker));
}

inline void zat_b33_relocate_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    std::uint32_t zat_b33_tech_money = 500;
    if (zat_b33_actor_has_needed_money(p_first_speaker, p_second_speaker))
        relocate_money_from_actor(p_first_speaker, p_second_speaker, zat_b33_tech_money);
}

inline void zat_b29_create_af_in_anomaly(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    xr_map<std::uint32_t, xr_string> anomaly_table;
    anomaly_table[16] = "gravi";
    anomaly_table[17] = "thermal";
    anomaly_table[18] = "acid";
    anomaly_table[19] = "electra";
    anomaly_table[20] = "gravi";
    anomaly_table[21] = "thermal";
    anomaly_table[22] = "acid";
    anomaly_table[23] = "electra";

    xr_map<xr_string, xr_map<std::uint32_t, xr_string>> anomalies_table;

    anomalies_table["gravi"][1] = "zat_b14_anomal_zone";
    anomalies_table["gravi"][2] = "zat_b55_anomal_zone";
    anomalies_table["gravi"][3] = "zat_b44_anomal_zone_gravi";

    anomalies_table["thermal"][1] = "zat_b20_anomal_zone";
    anomalies_table["thermal"][2] = "zat_b53_anomal_zone";
    anomalies_table["thermal"][3] = "zaton_b56_anomal_zone";

    anomalies_table["acid"][1] = "zat_b39_anomal_zone";
    anomalies_table["acid"][2] = "zat_b101_anomal_zone";
    anomalies_table["acid"][3] = "zat_b44_anomal_zone_acid";

    anomalies_table["electra"][1] = "zat_b54_anomal_zone";
    anomalies_table["electra"][2] = "zat_b100_anomal_zone";

    xr_string zone;
    xr_string zone_name;
    std::uint16_t key;

    for (const std::pair<std::uint16_t, xr_string>& it : Script_GlobalHelper::getInstance().getZatB29InfopBringTable())
    {
        if (Globals::has_alife_info(it.second.c_str()))
        {
            key = it.first;
            zone = anomaly_table[key];
            break;
        }
    }

    zone_name = anomalies_table[zone][Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(
        1, anomalies_table[zone].size())];

    CScriptGameObject* const p_object = DataBase::Storage::getInstance().getAnomalyByName().at(zone_name);
    Script_Binder_Anomaly* const p_binder = dynamic_cast<Script_Binder_Anomaly*>(p_object->binded_object());

    if (!p_binder)
    {
        Msg("[Scripts/zat_b29_create_af_in_anomaly(p_first_speaker, p_second_speaker)] WARNING: bad cast! Return ...");
        return;
    }

    p_binder->set_forced_override(Script_GlobalHelper::getInstance().getZatB29AfTable().at(key));
}

inline xr_string zat_b29_linker_give_adv_task(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    xr_string result_name;
    bool is_first = true;

    for (std::uint16_t i = 16; i <= 23; ++i)
    {
        if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopBringTable().at(i).c_str()))
        {
            DataBase::Storage::getInstance().getActor()->DisableInfoPortion(
                Script_GlobalHelper::getInstance().getZatB29InfopBringTable().at(i).c_str());
        }

        if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(i)))
        {
            if (is_first)
            {
                result_name =
                    Globals::Game::translate_string(Script_GlobalHelper::getInstance().getZatB29AfNamesTable().at(i));
                is_first = false;
            }
            else
            {
                result_name += ",";
                result_name +=
                    Globals::Game::translate_string(Script_GlobalHelper::getInstance().getZatB29AfNamesTable().at(i));
            }
        }
    }

    result_name += ".";
    return result_name;
}

inline bool zat_b29_actor_do_not_has_adv_task_af(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    for (std::uint16_t i = 16; i <= 23; ++i)
    {
        if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopBringTable().at(i)) &&
            DataBase::Storage::getInstance().getActor()->GetObjectByName(
                Script_GlobalHelper::getInstance().getZatB29AfTable().at(i).c_str()))
        {
            return false;
        }
    }

    return true;
}

inline bool zat_b29_actor_has_adv_task_af(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    for (std::uint16_t i = 16; i <= 23; ++i)
    {
        if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopBringTable().at(i)) &&
            DataBase::Storage::getInstance().getActor()->GetObjectByName(
                Script_GlobalHelper::getInstance().getZatB29AfTable().at(i).c_str()))
        {
            return true;
        }
    }

    return false;
}

inline bool zat_b29_actor_do_not_has_adv_task_af_1(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(16)) &&
        !DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(16).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_do_not_has_adv_task_af_2(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(17)) &&
        !DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(17).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_do_not_has_adv_task_af_3(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(18)) &&
        !DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(18).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_do_not_has_adv_task_af_4(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(19)) &&
        !DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(19).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_do_not_has_adv_task_af_5(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(20)) &&
        !DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(20).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_do_not_has_adv_task_af_6(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(21)) &&
        !DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(21).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_do_not_has_adv_task_af_7(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(22)) &&
        !DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(22).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_do_not_has_adv_task_af_8(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(23)) &&
        !DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(23).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_has_adv_task_af_1(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(16)) &&
        DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(16).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_has_adv_task_af_2(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(17)) &&
        DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(17).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_has_adv_task_af_3(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(18)) &&
        DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(18).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_has_adv_task_af_4(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (!Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(19)) &&
        DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(19).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_has_adv_task_af_5(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(20)) &&
        DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(20).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_has_adv_task_af_6(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(21)) &&
        DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(21).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_has_adv_task_af_7(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(22)) &&
        DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(22).c_str()))
    {
        return true;
    }

    return false;
}

inline bool zat_b29_actor_has_adv_task_af_8(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopTable().at(23)) &&
        DataBase::Storage::getInstance().getActor()->GetObjectByName(
            Script_GlobalHelper::getInstance().getZatB29AfTable().at(23).c_str()))
    {
        return true;
    }

    return false;
}

inline void zat_b29_linker_get_adv_task_af(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    for (std::uint16_t i = 16; i <= 23; ++i)
    {
        if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopBringTable().at(i)))
        {
            if (Globals::has_alife_info("zat_b29_adv_task_given"))
                DataBase::Storage::getInstance().getActor()->DisableInfoPortion("zat_b29_adv_task_given");

            relocate_item_section_from_actor(
                p_first_speaker, p_second_speaker, Script_GlobalHelper::getInstance().getZatB29AfTable().at(i));

            if (i < 20)
            {
                if (Globals::has_alife_info("zat_b29_linker_take_af_from_rival"))
                {
                    relocate_money_to_actor(p_first_speaker, p_second_speaker, 12000);
                }
                else
                {
                    relocate_money_to_actor(p_first_speaker, p_second_speaker, 18000);
                }
            }
            else if (i > 19)
            {
                if (Globals::has_alife_info("zat_b29_linker_take_af_from_rival"))
                {
                    relocate_money_to_actor(p_first_speaker, p_second_speaker, 18000);
                }
                else
                {
                    relocate_money_to_actor(p_first_speaker, p_second_speaker, 24000);
                }
            }

            break;
        }
    }
}

static xr_vector<xr_string> actor_wpn_table;

inline void is_good_gun(CScriptGameObject* const p_item)
{
    xr_string section_name = p_item->Section();
    xr_map<std::uint32_t, xr_string> wpn_table;

    wpn_table[1] = "wpn_sig550";
    wpn_table[2] = "wpn_g36";
    wpn_table[3] = "wpn_val";
    wpn_table[4] = "wpn_groza";
    wpn_table[5] = "wpn_vintorez";
    wpn_table[6] = "wpn_fn2000";

    for (const std::pair<std::uint32_t, xr_string>& it : wpn_table)
    {
        if (it.second == section_name)
        {
            actor_wpn_table.push_back(it.second);
            break;
        }
    }
}

inline bool zat_b29_actor_has_exchange_item(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
    Script_Binder_Actor* const p_binder = dynamic_cast<Script_Binder_Actor*>(p_actor->binded_object());

    if (!p_binder)
    {
        Msg("[Scripts/zat_b29_actor_has_exchange_item(p_first_speaker, p_second_speaker)] WARNING: SOMETHING IS VERY "
            "BAD, WHAT YOU HAVE DONE! THE BINDER FOR ACTOR IS NOT ACTOR's BINDER!!! CHECK YOUR CODE AGAIN! Return ...");
        return false;
    }

    p_actor->IterateInventory(is_good_gun);
    if (actor_wpn_table.empty() == false)
    {
        p_binder->setGoodGunName(actor_wpn_table[Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(
            0, actor_wpn_table.size() - 1)]);
    }

    if (!p_binder->getGoodGunName().empty())
    {
        return true;
    }

    return false;
}

inline void zat_b29_actor_exchange(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    for (std::uint16_t i = 16; i <= 23; ++i)
    {
        if (Globals::has_alife_info(Script_GlobalHelper::getInstance().getZatB29InfopBringTable().at(i)))
        {
            Script_Binder_Actor* const p_binder =
                dynamic_cast<Script_Binder_Actor*>(DataBase::Storage::getInstance().getActor()->binded_object());

            if (!p_binder)
            {
                Msg("[Scripts/zat_b29_actor_exchange(p_first_speaker, p_second_speaker)] WARNING: bad cast! Binder is "
                    "not Actor's binder! Return ...");
                return;
            }

            if (!p_binder->getGoodGunName().empty())
            {
                relocate_item_section_from_actor(p_first_speaker, p_second_speaker, p_binder->getGoodGunName());
                relocate_item_section_to_actor(
                    p_first_speaker, p_second_speaker, Script_GlobalHelper::getInstance().getZatB29AfTable().at(i), 1);
                p_binder->setGoodGunName("");
                break;
            }
        }
    }
}

inline void zat_b30_transfer_detector_to_actor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "detector_scientific", 1);
}

inline void zat_b30_give_owls_share_to_actor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    constexpr std::uint32_t amount = 1500;
    relocate_money_to_actor(p_first_speaker, p_second_speaker, amount);
}

inline bool zat_b30_actor_has_compass(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("af_compass"));
}

inline void zat_b30_transfer_af_from_actor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    constexpr std::uint32_t amount = 10000;
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "af_compass");
    relocate_money_to_actor(p_first_speaker, p_second_speaker, amount);
    Script_TreasureManager::getInstance().give_treasure("zat_hiding_place_49");
    Script_TreasureManager::getInstance().give_treasure("zat_hiding_place_15");
}

inline bool zat_b30_barmen_has_percent(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    std::uint8_t count = XR_LOGIC::pstor_retrieve_number(p_actor, "zat_b30_days_cnt");

    return (count > 0);
}

inline bool zat_b30_barmen_do_not_has_percent(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    std::uint8_t count = XR_LOGIC::pstor_retrieve_number(p_actor, "zat_b30_days_cnt");

    return (count < 1);
}

inline void zat_b30_transfer_percent(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    std::uint32_t amount = Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(5, 25) * 100;
    std::uint8_t days =
        XR_LOGIC::pstor_retrieve_number(DataBase::Storage::getInstance().getActor(), "zat_b30_days_cnt");

    amount *= days;

    relocate_money_to_actor(p_first_speaker, p_second_speaker, amount);
    XR_LOGIC::pstor_store(DataBase::Storage::getInstance().getActor(), "zat_b30_days_cnt", "0");
}

inline bool zat_b30_npc_has_detector(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);

    return !!(p_npc->GetObjectByName("detector_scientific"));
}

inline void zat_b30_actor_exchange(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
    Script_Binder_Actor* const p_binder = dynamic_cast<Script_Binder_Actor*>(p_actor->binded_object());

    if (!p_binder)
    {
        Msg("[Scripts/zat_b30_actor_exchange(p_first_speaker, p_second_speaker)] WARNING: bad cast! The binder is not "
            "Actor's binder! Return ...");
        return;
    }

    if (!p_binder->getGoodGunName().empty())
    {
        relocate_item_section_from_actor(p_first_speaker, p_second_speaker, p_binder->getGoodGunName());
        relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "detector_scientific", 1);
        p_binder->setGoodGunName("");
    }

    if (XR_CONDITION::check_npc_name_client(p_first_speaker, p_second_speaker, {"zat_b29_stalker_rival_1"}))
    {
        p_actor->GiveInfoPortion("zat_b30_rival_1_wo_detector");
    }
    else if (XR_CONDITION::check_npc_name_client(p_first_speaker, p_second_speaker, {"zat_b29_stalker_rival_2"}))
    {
        p_actor->GiveInfoPortion("zat_b30_rival_2_wo_detector");
    }
}

inline bool zat_b30_actor_has_two_detectors(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    std::uint32_t count = 0;
    auto count_function = [&](CScriptGameObject* const p_item) -> void {
        if (xr_string(p_item->Section()) == xr_string("detector_scientific"))
        {
            ++count;
        }
    };

    DataBase::Storage::getInstance().getActor()->IterateInventory(count_function);

    if (count > 1)
        return true;

    return false;
}

inline void zat_b30_actor_second_exchange(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "detector_scientific", 1);
}

inline bool zat_actor_has_nimble_weapon(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
    xr_vector<xr_string> items;
    items.push_back("wpn_groza_nimble");
    items.push_back("wpn_vintorez_nimble");
    items.push_back("wpn_desert_eagle_nimble");
    items.push_back("wpn_fn2000_nimble");
    items.push_back("wpn_g36_nimble");
    items.push_back("wpn_protecta_nimble");
    items.push_back("wpn_mp5_nimble");
    items.push_back("wpn_sig220_nimble");
    items.push_back("wpn_spas12_nimble");
    items.push_back("wpn_usp_nimble");
    items.push_back("wpn_svu_nimble");
    items.push_back("wpn_svd_nimble");

    for (const xr_string& it : items)
        if (p_actor->GetObjectByName(it.c_str()))
            return true;

    return false;
}

inline void zat_b51_robbery(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
    std::uint32_t amount = static_cast<std::uint32_t>(
        floor(p_actor->Money() * (Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(35, 50) / 100)));

    if (amount > p_actor->Money())
        amount = p_actor->Money();

    xr_map<xr_string, bool> need_items;
    need_items["wpn_usp"] = true;
    need_items["wpn_desert_eagle"] = true;
    need_items["wpn_protecta"] = true;
    need_items["wpn_sig550"] = true;
    need_items["wpn_fn2000"] = true;
    need_items["wpn_g36"] = true;
    need_items["wpn_val"] = true;
    need_items["wpn_vintorez"] = true;
    need_items["wpn_groza"] = true;
    need_items["wpn_svd"] = true;
    need_items["wpn_svu"] = true;
    need_items["wpn_pkm"] = true;
    need_items["wpn_sig550_luckygun"] = true;
    need_items["wpn_pkm_zulus"] = true;
    need_items["wpn_wincheaster1300_trapper"] = true;
    need_items["wpn_gauss"] = true;
    need_items["wpn_groza_nimble"] = true;
    need_items["wpn_desert_eagle_nimble"] = true;
    need_items["wpn_fn2000_nimble"] = true;
    need_items["wpn_g36_nimble"] = true;
    need_items["wpn_protecta_nimble"] = true;
    need_items["wpn_mp5_nimble"] = true;
    need_items["wpn_sig220_nimble"] = true;
    need_items["wpn_spas12_nimble"] = true;
    need_items["wpn_usp_nimble"] = true;
    need_items["wpn_vintorez_nimble"] = true;
    need_items["wpn_svu_nimble"] = true;
    need_items["wpn_svd_nimble"] = true;

    for (const std::pair<xr_string, bool>& it : need_items)
    {
        if (p_actor->GetObjectByName(it.first.c_str()))
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, it.first, "all");
        }
    }

    relocate_money_from_actor(p_first_speaker, p_second_speaker, amount);
}

inline void zat_b51_rob_nimble_weapon(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
    xr_map<xr_string, bool> need_items;
    xr_vector<xr_string> actor_has_item;

    need_items["wpn_groza_nimble"] = true;
    need_items["wpn_desert_eagle_nimble"] = true;
    need_items["wpn_fn2000_nimble"] = true;
    need_items["wpn_g36_nimble"] = true;
    need_items["wpn_protecta_nimble"] = true;
    need_items["wpn_mp5_nimble"] = true;
    need_items["wpn_sig220_nimble"] = true;
    need_items["wpn_spas12_nimble"] = true;
    need_items["wpn_usp_nimble"] = true;
    need_items["wpn_vintorez_nimble"] = true;
    need_items["wpn_svu_nimble"] = true;
    need_items["wpn_svd_nimble"] = true;

    for (const std::pair<xr_string, bool>& it : need_items)
    {
        if (p_actor->GetObjectByName(it.first.c_str()))
        {
            actor_has_item.push_back(it.first);
        }

        if (p_actor->item_in_slot(2) && p_actor->item_in_slot(2)->Section() == it.first)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, it.first);
            return;
        }
        else if (p_actor->item_in_slot(3) && p_actor->item_in_slot(3)->Section() == it.first)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, it.first);
            return;
        }
    }

    if (!actor_has_item.empty())
        relocate_item_section_from_actor(p_first_speaker, p_second_speaker,
            actor_has_item[Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(
                0, actor_has_item.size() - 1)]);
}

inline void give_compass_to_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "af_compass", 1);
}

inline void zat_b51_randomize_item(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    for (std::uint32_t i = 1; i <= 7; ++i)
    {
        if (Globals::has_alife_info(xr_string("zat_b51_processing_category_").append(std::to_string(i).c_str())))
        {
            xr_vector<std::uint32_t> zat_b51_available_items;

            for (std::uint32_t j = 1; j <= Script_GlobalHelper::getInstance().getZatItemCountByCategory().at(i); ++j)
            {
                if (!Globals::has_alife_info(xr_string("zat_b51_done_item_")
                                                 .append(std::to_string(i).c_str())
                                                 .append("_")
                                                 .append(std::to_string(j).c_str())))
                {
                    zat_b51_available_items.push_back(j);
                }
            }

            xr_string infoporion_name = "zat_b51_ordered_item_";
            infoporion_name += std::to_string(i).c_str();
            infoporion_name += "_";
            infoporion_name += std::to_string(
                zat_b51_available_items.at(Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(
                    0, zat_b51_available_items.size() - 1)))
                                   .c_str();
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion(infoporion_name.c_str());
        }
    }
}

inline void zat_b51_give_prepay(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    for (std::uint32_t i = 1; i <= 7; ++i)
    {
        if (Globals::has_alife_info(xr_string("zat_b51_processing_category_").append(std::to_string(i).c_str())))
        {
            if (!Globals::has_alife_info("zat_b51_order_refused"))
            {
                relocate_money_from_actor(p_first_speaker, p_second_speaker,
                    Script_GlobalHelper::getInstance().getZatB51CostsTable().at(i).at("prepay_agreed"));
                return;
            }

            relocate_money_from_actor(p_first_speaker, p_second_speaker,
                Script_GlobalHelper::getInstance().getZatB51CostsTable().at(i).at("prepay_refused"));
            return;
        }
    }
}

inline bool zat_b51_has_prepay(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    for (std::uint32_t i = 1; i <= 7; ++i)
    {
        if (Globals::has_alife_info(xr_string("zat_b51_processing_category_").append(std::to_string(i).c_str())))
        {
            if (!Globals::has_alife_info("zat_b51_order_refused"))
            {
                return (DataBase::Storage::getInstance().getActor()->Money() >=
                    Script_GlobalHelper::getInstance().getZatB51CostsTable().at(i).at("prepay_agreed"));
            }

            return (DataBase::Storage::getInstance().getActor()->Money() >=
                Script_GlobalHelper::getInstance().getZatB51CostsTable().at(i).at("prepay_refused"));
        }
    }
}

inline bool zat_b51_hasnt_prepay(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !(zat_b51_has_prepay(p_first_speaker, p_second_speaker));
}

inline void zat_b51_buy_item(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    for (std::uint32_t i = 1; i <= 7; ++i)
    {
        if (Globals::has_alife_info(xr_string("zat_b51_processing_category_").append(std::to_string(i).c_str())))
        {
            for (std::uint32_t j = 1; j <= Script_GlobalHelper::getInstance().getZatB51BuyItemTable().at(i).size(); ++j)
            {
                if (Globals::has_alife_info(xr_string("zat_b51_ordered_item_")
                                                .append(std::to_string(i).c_str())
                                                .append("_")
                                                .append(std::to_string(j).c_str())))
                {
                    for (const xr_string& it : Script_GlobalHelper::getInstance().getZatB51BuyItemTable().at(i).at(j))
                    {
                        relocate_item_section_to_actor(p_first_speaker, p_second_speaker, it, 1);
                    }

                    relocate_money_from_actor(p_first_speaker, p_second_speaker,
                        Script_GlobalHelper::getInstance().getZatB51CostsTable().at(i).at("cost"));
                    if (Globals::has_alife_info(
                            xr_string("zat_b51_processing_category_").append(std::to_string(i).c_str())))
                    {
                        DataBase::Storage::getInstance().getActor()->DisableInfoPortion(
                            xr_string("zat_b51_processing_category_").append(std::to_string(i).c_str()).c_str());
                    }

                    if (Globals::has_alife_info(xr_string("zat_b51_ordered_item_")
                                                    .append(std::to_string(i).c_str())
                                                    .append("_")
                                                    .append(std::to_string(j).c_str())))
                    {
                        DataBase::Storage::getInstance().getActor()->DisableInfoPortion(
                            xr_string("zat_b51_ordered_item_")
                                .append(std::to_string(i).c_str())
                                .append("_")
                                .append(std::to_string(j).c_str())
                                .c_str());
                    }

                    DataBase::Storage::getInstance().getActor()->GiveInfoPortion(xr_string("zat_b51_done_item_")
                                                                                     .append(std::to_string(i).c_str())
                                                                                     .append("_")
                                                                                     .append(std::to_string(j).c_str())
                                                                                     .c_str());
                    break;
                }

                bool is_category_finishing = true;

                for (std::uint32_t j = 1; j <= Script_GlobalHelper::getInstance().getZatB51BuyItemTable().at(i).size();
                     ++j)
                {
                    if (!Globals::has_alife_info(xr_string("zat_b51_done_item_")
                                                     .append(std::to_string(i).c_str())
                                                     .append("_")
                                                     .append(std::to_string(j).c_str())))
                    {
                        is_category_finishing = false;
                        break;
                    }
                }

                if (is_category_finishing)
                    DataBase::Storage::getInstance().getActor()->GiveInfoPortion(
                        xr_string("zat_b51_finishing_category_").append(std::to_string(i).c_str()).c_str());

                return;
            }
        }
    }
}

inline void zat_b51_refuse_item(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    for (std::uint32_t i = 1; i <= 7; ++i)
    {
        if (Globals::has_alife_info(xr_string("zat_b51_processing_category_").append(std::to_string(i).c_str())))
        {
            for (std::uint32_t j = 1; j <= Script_GlobalHelper::getInstance().getZatB51BuyItemTable().at(i).size(); ++j)
            {
                if (Globals::has_alife_info(xr_string("zat_b51_ordered_item_")
                                                .append(std::to_string(i).c_str())
                                                .append("_")
                                                .append(std::to_string(j).c_str())))
                {
                    if (Globals::has_alife_info(
                            xr_string("zat_b51_processing_category_").append(std::to_string(i).c_str())))
                    {
                        DataBase::Storage::getInstance().getActor()->DisableInfoPortion(
                            xr_string("zat_b51_processing_category_").append(std::to_string(i).c_str()).c_str());
                    }

                    if (Globals::has_alife_info(xr_string("zat_b51_ordered_item_")
                                                    .append(std::to_string(i).c_str())
                                                    .append("_")
                                                    .append(std::to_string(j).c_str())))
                    {
                        DataBase::Storage::getInstance().getActor()->DisableInfoPortion(
                            xr_string("zat_b51_ordered_item_")
                                .append(std::to_string(i).c_str())
                                .append("_")
                                .append(std::to_string(j).c_str())
                                .c_str());
                    }

                    DataBase::Storage::getInstance().getActor()->GiveInfoPortion(xr_string("zat_b51_done_item_")
                                                                                     .append(std::to_string(i).c_str())
                                                                                     .append("_")
                                                                                     .append(std::to_string(j).c_str())
                                                                                     .c_str());

                    break;
                }
            }

            bool is_category_finishing = true;

            for (std::uint32_t j = 1; j <= Script_GlobalHelper::getInstance().getZatB51BuyItemTable().at(i).size(); ++j)
            {
                if (!Globals::has_alife_info(xr_string("zat_b51_done_item_")
                                                 .append(std::to_string(i).c_str())
                                                 .append("_")
                                                 .append(std::to_string(j).c_str())
                                                 .c_str()))
                {
                    is_category_finishing = false;
                    break;
                }
            }

            if (is_category_finishing)
                DataBase::Storage::getInstance().getActor()->GiveInfoPortion(
                    xr_string("zat_b51_finishing_category_").append(std::to_string(i).c_str()).c_str());

            return;
        }
    }
}

inline bool zat_b51_has_item_cost(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    for (std::uint32_t i = 1; i <= 7; ++i)
    {
        if (Globals::has_alife_info(xr_string("zat_b51_processing_category_").append(std::to_string(i).c_str())))
        {
            return (DataBase::Storage::getInstance().getActor()->Money() >=
                Script_GlobalHelper::getInstance().getZatB51CostsTable().at(i).at("cost"));
        }
    }

    return false;
}

inline bool zat_b51_hasnt_item_cost(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !(zat_b51_has_item_cost(p_first_speaker, p_second_speaker));
}

inline bool zat_b12_actor_have_documents(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (DataBase::Storage::getInstance().getActor()->GetObjectByName("zat_b12_documents_1") ||
        DataBase::Storage::getInstance().getActor()->GetObjectByName("zat_b12_documents_2"))
    {
        return true;
    }

    return false;
}

inline void zat_b12_actor_transfer_documents(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    constexpr std::uint32_t amount_doc1 = 1000;
    constexpr std::uint32_t amount_doc2 = 600;
    constexpr std::uint32_t amount_doc3 = 400;
    std::uint32_t amount_total = 0;
    CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
    std::uint32_t count = 0;
    std::uint32_t count2 = 0;

    auto item_iterator_npc = [&](CScriptGameObject* const p_item) -> void {
        if (p_item->Section() == xr_string("zat_b12_documents_2"))
            ++count;
    };

    auto item_iterator_actor = [&](CScriptGameObject* const p_item) -> void {
        if (p_item->Section() == xr_string("zat_b12_documents_2"))
            ++count2;
    };

    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();

    if (p_actor->GetObjectByName("zat_b12_documents_1"))
    {
        relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "zat_b12_documents_1");
        p_actor->GiveInfoPortion("zat_b12_documents_sold_1");
        amount_total += amount_doc1;
    }

    p_npc->IterateInventory(item_iterator_npc);
    p_actor->IterateInventory(item_iterator_actor);

    if (p_actor->GetObjectByName("zat_b12_documents_2"))
    {
        if (count < 1)
        {
            amount_total += amount_doc2;
            if (count2 > 1)
            {
                amount_total += amount_doc3 * (count2 - 1);
                p_actor->GiveInfoPortion("zat_b12_documents_sold_2");
            }
        }
        else
        {
            amount_total += amount_doc3 * count2;
            p_actor->GiveInfoPortion("zat_b12_documents_sold_3");
        }

        relocate_item_section_from_actor(
            p_first_speaker, p_second_speaker, "zat_b12_documents_2", std::to_string(count2).c_str());
    }

    relocate_money_to_actor(p_first_speaker, p_second_speaker, amount_total);
}

inline bool zat_b3_actor_got_toolkit(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    bool is_found_toolkit = false;
    auto have_toolkit = [&](CScriptGameObject* const p_item) -> void {
        xr_string section_name = p_item->Section();
        if ((section_name == "toolkit_1" && !Globals::has_alife_info("zat_b3_tech_instrument_1_brought")) ||
            (section_name == "toolkit_2" && !Globals::has_alife_info("zat_b3_tech_instrument_2_brought")) ||
            (section_name == "toolkit_3" && !Globals::has_alife_info("zat_b3_tech_instrument_3_brought")))
        {
            is_found_toolkit = true;
            return;
        }
    };

    DataBase::Storage::getInstance().getActor()->IterateInventory(have_toolkit);

    return is_found_toolkit;
}

inline void give_toolkit_1(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "toolkit_1");
    constexpr std::uint32_t amount = 1000;
    relocate_money_to_actor(p_first_speaker, p_second_speaker, amount);
}

inline bool if_actor_has_toolkit_1(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("toolkit_1"));
}

inline void give_toolkit_2(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "toolkit_2");
    constexpr std::uint32_t amount = 1200;
    relocate_money_to_actor(p_first_speaker, p_second_speaker, amount);
}

inline bool if_actor_has_toolkit_2(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("toolkit_2"));
}

inline void give_toolkit_3(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "toolkit_3");
    constexpr std::uint32_t amount = 1500;
    relocate_money_to_actor(p_first_speaker, p_second_speaker, amount);
}

inline bool if_actor_has_toolkit_3(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("toolkit_3"));
}

// @ RENAMED
inline void zat_give_vodka(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "vodka");
}

inline bool if_actor_has_vodka(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("vodka"));
}

inline bool actor_has_more_then_need_money_to_buy_battery(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->Money() >= 2000);
}

inline bool actor_has_less_then_need_money_to_buy_battery(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->Money() < 2000);
}

// @ RENAMED
inline void zat_relocate_need_money_to_buy_battery(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    constexpr std::uint32_t money_to_buy_battery = 2000;
    relocate_money_from_actor(p_first_speaker, p_second_speaker, money_to_buy_battery);
}

// @ RENAMED
inline void zat_give_actor_battery(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "ammo_gauss_cardan");
}

inline void give_actor_zat_a23_access_card(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "zat_a23_access_card");
}

inline void give_zat_a23_gauss_rifle_docs(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "zat_a23_gauss_rifle_docs");
}

inline void return_zat_a23_gauss_rifle_docs(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "zat_a23_gauss_rifle_docs");
}

inline bool if_actor_has_zat_a23_gauss_rifle_docs(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(p_first_speaker->GetObjectByName("zat_a23_gauss_rifle_docs"));
}

inline bool if_actor_has_gauss_rifle(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(p_first_speaker->GetObjectByName("pri_a17_gauss_rifle"));
}

inline void give_tech_gauss_rifle(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "pri_a17_gauss_rifle");
}

inline void give_actor_repaired_gauss_rifle(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "wpn_gauss");
}

inline bool zat_b215_actor_has_money_poor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->Money() >= 1000);
}

inline bool zat_b215_actor_has_no_money_poor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->Money() < 1000);
}

inline bool zat_b215_actor_has_money_poor_pripyat(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->Money() >= 4000);
}

inline bool zat_b215_actor_has_no_money_poor_pripyat(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->Money() < 4000);
}

inline bool zat_b215_actor_has_money_rich(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->Money() >= 3000);
}

inline bool zat_b215_actor_has_no_money_rich(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->Money() < 3000);
}

inline bool zat_b215_actor_has_money_rich_pripyat(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->Money() >= 6000);
}

inline bool zat_b215_actor_has_no_money_rich_pripyat(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->Money() < 6000);
}

inline void zat_b215_relocate_money_poor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    constexpr std::uint32_t value = 1000;
    relocate_money_from_actor(p_first_speaker, p_second_speaker, value);
}

inline void zat_b215_relocate_money_poor_pripyat(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    constexpr std::uint32_t value = 4000;
    relocate_money_from_actor(p_first_speaker, p_second_speaker, value);
}

inline void zat_b215_relocate_money_rich(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    constexpr std::uint32_t value = 3000;
    relocate_money_from_actor(p_first_speaker, p_second_speaker, value);
}

inline void zat_b215_relocate_money_rich_pripyat(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    constexpr std::uint32_t value = 6000;
    relocate_money_from_actor(p_first_speaker, p_second_speaker, value);
}

inline bool zat_b215_counter_greater_3(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
    return XR_CONDITION::is_counter_greater_client(p_actor, p_npc, {"zat_a9_way_to_pripyat_counter", "3"});
}

inline bool zat_b215_counter_less_4(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
    CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);

    return !(XR_CONDITION::is_counter_greater_client(p_actor, p_npc, {"jup_a9_way_gates_counter", "4"}));
}

inline bool zat_b30_actor_has_noah_pda(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("zat_b20_noah_pda"));
}

inline void zat_b30_sell_noah_pda(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    constexpr std::uint32_t amount = 1000;
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "zat_b20_noah_pda");
    relocate_money_to_actor(p_first_speaker, p_second_speaker, amount);
}

inline bool zat_b40_actor_has_notebook(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("zat_b40_notebook"));
}

inline bool zat_b40_actor_has_merc_pda_1(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("zat_b40_pda_1"));
}

inline bool zat_b40_actor_has_merc_pda_2(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("zat_b40_pda_2"));
}

inline void zat_b40_transfer_notebook(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "zat_b40_notebook");
    DataBase::Storage::getInstance().getActor()->GiveInfoPortion("zat_b40_notebook_saled");
    relocate_money_to_actor(p_first_speaker, p_second_speaker, 2000);
}

inline void zat_b40_transfer_merc_pda_1(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "zat_b40_pda_1");
    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();

    p_actor->GiveInfoPortion("zat_b40_pda_1_saled");
    relocate_money_to_actor(p_first_speaker, p_second_speaker, 1000);

    if (p_actor->HasInfo("zat_b40_notebook_saled") && p_actor->HasInfo("zat_b40_pda_1_saled") &&
        p_actor->HasInfo("zat_b40_pda_2_saled"))
    {
        p_actor->GiveInfoPortion("zat_b40_all_item_saled");
    }
}

inline void zat_b40_transfer_merc_pda_2(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "zat_b40_pda_2");
    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();

    relocate_money_to_actor(p_first_speaker, p_second_speaker, 1000);

    if (p_actor->HasInfo("zat_b40_notebook_saled") && p_actor->HasInfo("zat_b40_pda_1_saled") &&
        p_actor->HasInfo("zat_b40_pda_2_saled"))
    {
        p_actor->GiveInfoPortion("zat_b40_all_item_saled");
    }
}

inline bool zat_b44_actor_has_pda_global(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
    return ((p_actor->GetObjectByName("zat_b39_joker_pda")) || (p_actor->GetObjectByName("zat_b44_barge_pda")));
}

inline bool zat_b44_actor_has_not_pda_global(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
    return ((p_actor->GetObjectByName("zat_b39_joker_pda") == nullptr) ||
        (p_actor->GetObjectByName("zat_b44_barge_pda") == nullptr));
}

inline bool zat_b44_actor_has_pda_barge(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("zat_b44_barge_pda"));
}

inline bool zat_b44_actor_has_pda_joker(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("zat_b39_joker_pda"));
}

inline bool zat_b44_actor_has_pda_both(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();

    return ((p_actor->GetObjectByName("zat_b39_joker_pda")) || (p_actor->GetObjectByName("zat_b44_barge_pda")));
}

inline void zat_b44_transfer_pda_barge(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "zat_b44_barge_pda");
}

inline void zat_b44_transfer_pda_joker(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "zat_b39_joker_pda");
}

inline void zat_b44_transfer_pda_both(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "zat_b44_barge_pda");
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "zat_b39_joker_pda");
}

inline bool zat_b44_frends_dialog_enabled(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
    bool is_logic_statement_1 =
        ((p_actor->HasInfo("zat_b3_tech_have_couple_dose")) && (p_actor->HasInfo("zat_b3_tech_discount_1")));
    bool is_logic_statement_2 = zat_b44_actor_has_pda_global(p_first_speaker, p_second_speaker);

    return (is_logic_statement_1 || is_logic_statement_2);
}

inline bool zat_b53_if_actor_has_detector_advanced(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();

    return (p_actor->GetObjectByName("detector_advanced") || p_actor->GetObjectByName("detector_elite") ||
        p_actor->GetObjectByName("detector_scientific"));
}

inline bool zat_b53_if_actor_hasnt_detector_advanced(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !(zat_b53_if_actor_has_detector_advanced(p_first_speaker, p_second_speaker));
}

inline void zat_b53_transfer_detector_advanced_to_actor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_to_actor(p_second_speaker, p_first_speaker, "detector_advanced");
}

inline void zat_b53_transfer_fireball_to_actor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_to_actor(p_second_speaker, p_first_speaker, "af_fireball");
}

inline void zat_b53_transfer_medkit_to_actor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_to_actor(p_second_speaker, p_first_speaker, "medkit");
}

inline void zat_b53_transfer_medkit_to_npc(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    xr_string section_name;
    CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
    if (p_actor->GetObjectByName("medkit"))
    {
        section_name = "medkit";
    }
    else if (p_actor->GetObjectByName("medkit_army"))
    {
        section_name = "medkit_army";
    }
    else if (p_actor->GetObjectByName("medkit_scientic"))
    {
        section_name = "medkit_scientic";
    }

    Globals::Game::alife_release(
        ai().alife().objects().object(p_actor->GetObjectByName(section_name.c_str())->ID()), true);

    Script_NewsManager::getInstance().relocate_item("out", section_name);

    p_actor->ChangeCharacterReputation(10);
}

inline bool zat_a23_actor_has_access_card(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("zat_a23_access_card"));
}

inline void zat_a23_transfer_access_card_to_tech(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "zat_a23_access_card");
    relocate_item_section_to_actor(p_second_speaker, p_first_speaker, "medkit_scientic", 3);
}

inline void zat_b57_stalker_reward_to_actor_detector(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "detector_elite");
    Script_TreasureManager::getInstance().give_treasure("zat_hiding_place_54");
}

inline bool actor_has_gas(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("zat_b57_gas"));
}

inline bool actor_has_not_gas(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !(actor_has_gas(p_first_speaker, p_second_speaker));
}

inline bool zat_b57_actor_has_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (DataBase::Storage::getInstance().getActor()->Money() >= 2000);
}

inline bool zat_b57_actor_hasnt_money(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !(zat_b57_actor_has_money(p_first_speaker, p_second_speaker));
}

inline void zat_b57_transfer_gas_money(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_money_from_actor(p_first_speaker, p_second_speaker, 2000);
}

inline bool is_zat_b106_hunting_time(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::get_time_hours() >= 2 && Globals::get_time_hours() < 5)
    {
        if (Globals::get_time_hours() > 2)
        {
            return true;
        }
        else if (Globals::get_time_minutes() >= 45)
        {
            return true;
        }
    }

    return false;
}

inline bool is_not_zat_b106_hunting_time(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::get_time_hours() >= 2 && Globals::get_time_hours() < 5)
    {
        if (Globals::get_time_hours() > 2)
        {
            return false;
        }
        else if (Globals::get_time_minutes() >= 45)
        {
            return false;
        }
    }

    return true;
}

inline void zat_b106_transfer_weap_to_actor(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "wpn_spas12");
}

inline void zat_b106_soroka_reward(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info("jup_b25_flint_blame_done_to_duty") ||
        Globals::has_alife_info("jup_b25_flint_blame_done_to_freedom"))
        relocate_money_to_actor(p_first_speaker, p_second_speaker, 1000);
    else
        relocate_money_to_actor(p_first_speaker, p_second_speaker, 3000);
}

inline bool zat_b106_soroka_gone(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (Globals::has_alife_info("jup_b25_flint_blame_done_to_duty") ||
        Globals::has_alife_info("jup_b25_flint_blame_done_to_freedom"));
}

inline bool zat_b106_soroka_not_gone(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return !(zat_b106_soroka_gone(p_first_speaker, p_second_speaker));
}

inline bool zat_b22_actor_has_proof(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    return (actor_has_item(p_first_speaker, p_second_speaker, "zat_b22_medic_pda"));
}

inline void zat_b22_transfer_proof(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "zat_b22_medic_pda");
}

// @ Сделан rename потому что идёт коллизия в Script_GlobalHelper при регистрации XR_CONDITION функции
inline bool zat_dialog_b103_actor_has_needed_food(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    xr_vector<xr_string> items_sections;
    std::uint32_t needed = 6;
    std::uint32_t count = 0;
    xr_string item_section_name;

    auto iterator = [&](CScriptGameObject* const p_item) -> void {
        if (p_item->Section() == item_section_name)
            ++count;
    };

    for (const xr_string& it : items_sections)
    {
        item_section_name = it;
        Globals::get_story_object("actor")->IterateInventory(iterator);
    }

    return (count >= needed);
}

inline void zat_b5_stalker_transfer_money(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_money_to_actor(p_first_speaker, p_second_speaker, 2500);
    Script_TreasureManager::getInstance().give_treasure("zat_hiding_place_7");
}

inline void zat_b5_dealer_full_revard(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_money_to_actor(p_first_speaker, p_second_speaker, 6000);
}

inline void zat_b5_dealer_easy_revard(
    CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_money_to_actor(p_first_speaker, p_second_speaker, 3000);
}

inline void zat_b5_bandits_revard(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    relocate_money_to_actor(p_first_speaker, p_second_speaker, 5000);
    Script_TreasureManager::getInstance().give_treasure("zat_hiding_place_20");
}

inline void zat_b106_give_reward(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker) 
{
    Script_TreasureManager::getInstance().give_treasure("zat_hiding_place_50");
}

inline bool is_zat_b3_tech_drinks_precond(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
{
    if (Globals::has_alife_info("zat_b3_gauss_repaired") && Globals::has_alife_info("zat_b3_tech_drink_no_more") == false)
    {
        return true;
    }
    else if (Globals::has_alife_info("zat_b3_tech_see_produce_62") == false)
    {
        return true;
    }

    return false;
}

} // namespace Scripts
} // namespace Cordis
