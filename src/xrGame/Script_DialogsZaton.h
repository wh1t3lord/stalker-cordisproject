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
}

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

} // namespace Scripts
} // namespace Cordis
