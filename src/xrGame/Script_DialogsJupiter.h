#pragma once

namespace Cordis
{
    namespace Scripts
    {
        inline void jup_b208_give_reward(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            constexpr int amount = 5000;
            relocate_money_to_actor(p_first_speaker, p_second_speaker, amount);
            Script_TreasureManager::getInstance().give_treasure("jup_hiding_place_18");
            Script_TreasureManager::getInstance().give_treasure("jup_hiding_place_35");
            Script_TreasureManager::getInstance().give_treasure("jup_hiding_place_45");
        }

        inline bool jupiter_a9_actor_has_all_mail_items(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
            bool is_statement_1 = !!(p_actor->GetObjectByName("jup_a9_conservation_info"));
            bool is_statement_2 = !!(p_actor->GetObjectByName("jup_a9_power_info"));
            bool is_statement_3 = !!(p_actor->GetObjectByName("jup_a9_way_info"));

            return (is_statement_1 && is_statement_2 && is_statement_3);
        }

        inline bool jupiter_a9_actor_hasnt_all_mail_items(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jupiter_a9_actor_has_all_mail_items(p_first_speaker, p_second_speaker));
        }

        inline bool jupiter_a9_actor_has_any_items(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
            bool is_has_delivery = p_actor->GetObjectByName("jup_a9_delivery_info");
            bool is_has_evac = p_actor->GetObjectByName("jup_a9_evacuation_info");
            bool is_has_losses = p_actor->GetObjectByName("jup_a9_losses_info");
            bool is_has_power = p_actor->GetObjectByName("jup_a9_power_info");
            bool is_has_conversation = p_actor->GetObjectByName("jup_a9_conservation_info");
            bool is_has_way = p_actor->GetObjectByName("jup_a9_way_info");
            bool is_has_meeting = p_actor->GetObjectByName("jup_a9_meeting_info");

            return (is_has_delivery || is_has_evac || is_has_losses || is_has_power || is_has_conversation || is_has_way || is_has_meeting);
        }

        inline bool jupiter_a9_actor_has_any_mail_items(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
            bool is_has_coversation = p_actor->GetObjectByName("jup_a9_conservation_info");
            bool is_has_power = p_actor->GetObjectByName("jup_a9_power_info");
            bool is_has_way = p_actor->GetObjectByName("jup_a9_way_info");

            return (is_has_coversation || is_has_power || is_has_way);
        }

        inline bool jupiter_a9_actor_has_any_secondary_items(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
            bool is_has_delivery = p_actor->GetObjectByName("jup_a9_delivery_info");
            bool is_has_evac = p_actor->GetObjectByName("jup_a9_evacuation_info");
            bool is_has_losses = p_actor->GetObjectByName("jup_a9_losses_info");
            bool is_has_meeting = p_actor->GetObjectByName("jup_a9_meeting_info");

            return (is_has_delivery || is_has_evac || is_has_losses || is_has_meeting);
        }

        inline bool jupiter_a9_actor_hasnt_any_mail_items(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
            bool is_has_coversation = p_actor->GetObjectByName("jup_a9_conservation_info");
            bool is_has_power = p_actor->GetObjectByName("jup_a9_power_info");
            bool is_has_way = p_actor->GetObjectByName("jup_a9_way_info");

            return (!is_has_coversation || !is_has_power || !is_has_way);
        }

        inline void jupiter_a9_freedom_leader_jupiter_delivery(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 500);
        }

        inline void jupiter_a9_freedom_leader_jupiter_evacuation(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 500);
        }

        inline void jupiter_a9_freedom_leader_jupiter_losses(
            CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 500 );
        }

        inline void jupiter_a9_freedom_leader_jupiter_meeting(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 500);
        }

        inline void jupiter_a9_dolg_leader_jupiter_delivery(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 500);
        }

        inline void jupiter_a9_dolg_leader_jupiter_evacuation(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 500);
        }

        inline void jupiter_a9_dolg_leader_jupiter_losses(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 500);
        }

        inline void jupiter_a9_dolg_leader_jupiter_meeting(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 500);
        }

        inline void jup_a9_owl_stalker_trader_sell_jup_a9_evacuation_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_evacuation_info");
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 750);
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a9_evacuation_info_sold");
        }

        inline void jup_a9_owl_stalker_trader_sell_jup_a9_meeting_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_meeting_info");
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 750);
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a9_meeting_info_sold");
        }

        inline void jup_a9_owl_stalker_trader_sell_jup_a9_losses_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_losses_info");
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 750);
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a9_losses_info_sold");
        }

        inline void jup_a9_owl_stalker_trader_sell_jup_a9_delivery_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_delivery_info");
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 750);
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a9_delivery_info_sold");
        }

        inline bool jup_a9_actor_has_conservation_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_a9_conservation_info"));
        }

        inline bool jup_a9_actor_hasnt_conservation_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_a9_actor_has_conservation_info(p_first_speaker, p_second_speaker));
        }

        inline void actor_relocate_conservation_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_conservation_info");
        }

        inline bool jup_a9_actor_has_power_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_a9_power_info"));
        }

        inline bool jup_a9_actor_hasnt_power_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_a9_actor_has_power_info(p_first_speaker, p_second_speaker));
        }

        inline void actor_relocate_power_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_power_info");
        }

        inline bool jup_a9_actor_has_way_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_a9_way_info"));
        }

        inline bool jup_a9_actor_hasnt_way_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_a9_actor_has_way_info(p_first_speaker, p_second_speaker));
        }

        inline void actor_relocate_way_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_way_info");
        }

        inline bool jup_a9_actor_has_evacuation_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_a9_evacuation_info"));
        }

        inline bool jup_a9_actor_hasnt_evacuation_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_a9_actor_has_evacuation_info(p_first_speaker, p_second_speaker));
        }
        
        inline void actor_relocate_evacuation_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_evacuation_info");
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a9_evacuation_info_sold");
        }

        inline bool jup_a9_actor_has_meeting_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_a9_meeting_info"));
        }

        inline bool jup_a9_actor_hasnt_meeting_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_a9_actor_has_meeting_info(p_first_speaker, p_second_speaker));
        }

        inline void actor_relocate_meeting_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_meeting_info");
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a9_meeting_info_sold");
        }

        inline bool jup_a9_actor_has_delivery_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !!(DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a9_delivery_info"));
        }

        inline bool jup_a9_actor_hasnt_delivery_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_a9_actor_has_delivery_info(p_first_speaker, p_second_speaker));
        }

        inline void actor_relocate_delivery_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_delivery_info");
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a9_delivery_info_sold");
        }

        inline bool jup_a9_actor_has_losses_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_a9_losses_info"));
        }

        inline bool jup_a9_actor_hasnt_losses_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_a9_actor_has_losses_info(p_first_speaker, p_second_speaker));
        }

        inline void actor_relocate_losses_info(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_a9_losses_info");
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a9_losses_info_sold");
        }

        inline bool actor_has_plant(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b206_plant"));
        }

        inline void actor_relocate_plant(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b206_plant");
        }

        inline void actor_relocate_trapper_reward(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "wpn_wincheaster1300_trapper");
        }

        inline void zat_b106_trapper_reward(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (Globals::has_alife_info("zat_b106_one_hit"))
            {
                relocate_money_to_actor(p_first_speaker, p_second_speaker, 3000);
            }
            else
            {
                relocate_money_to_actor(p_first_speaker, p_second_speaker, 2000);
            }
        }

        inline bool jup_a10_proverka_wpn(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            xr_vector<xr_string> data = {
                "wpn_desert_eagle",
                "wpn_desert_eagle_nimble",
                "wpn_sig550",
                "wpn_g36",
                "wpn_g36_nimble",
                "wpn_fn2000",
                "wpn_fn2000_nimble",
                "wpn_groza",
                "wpn_groza_nimble",
                "wpn_val",
                "wpn_vintorez",
                "wpn_vintorez_nimble",
                "wpn_svd",
                "wpn_svu",
                "wpn_pkm",
                "wpn_spas12",
                "wpn_spas12_nimble",
                "wpn_protecta",
                "wpn_protecta_nimble",
                "wpn_gauss",
                "wpn_rpg7",
                "wpn_rg-6",
                "wpn_pkm_zulus"
            };

            for (const xr_string& it : data)
            {
                if (DataBase::Storage::getInstance().getActor()->item_in_slot(2) && DataBase::Storage::getInstance().getActor()->item_in_slot(2)->Section() == it)
                {
                    return true;
                }
                else if (DataBase::Storage::getInstance().getActor()->item_in_slot(3) && DataBase::Storage::getInstance().getActor()->item_in_slot(3)->Section() == it)
                {
                    return true;
                }
            }

            return false;
        }

        inline bool jup_a10_proverka_wpn_false(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_a10_proverka_wpn(p_first_speaker, p_second_speaker));
        }

        inline bool jup_a10_actor_has_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (Globals::has_alife_info("jup_a10_debt_wo_percent"))
                return (DataBase::Storage::getInstance().getActor()->Money() >= 5000);
            

            return (DataBase::Storage::getInstance().getActor()->Money() >= 7000);
        }

        inline bool jup_a10_actor_has_not_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_a10_actor_has_money(p_first_speaker, p_second_speaker));
        }

        inline void jupiter_a9_dolg_leader_jupiter_sell_all_secondary_items(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (jup_a9_actor_has_evacuation_info(p_first_speaker, p_second_speaker))
            {
                actor_relocate_evacuation_info(p_first_speaker, p_second_speaker);
                jupiter_a9_dolg_leader_jupiter_evacuation(p_first_speaker, p_second_speaker);
            }

            if (jup_a9_actor_has_meeting_info(p_first_speaker, p_second_speaker))
            {
                actor_relocate_meeting_info(p_first_speaker, p_second_speaker);
                jupiter_a9_dolg_leader_jupiter_meeting(p_first_speaker, p_second_speaker);
            }

            if (jup_a9_actor_has_losses_info(p_first_speaker, p_second_speaker))
            {
                actor_relocate_losses_info(p_first_speaker, p_second_speaker);
                jupiter_a9_dolg_leader_jupiter_losses(p_first_speaker, p_second_speaker);
            }

            if (jup_a9_actor_has_delivery_info(p_first_speaker, p_second_speaker))
            {
                actor_relocate_delivery_info(p_first_speaker, p_second_speaker);
                jupiter_a9_dolg_leader_jupiter_delivery(p_first_speaker, p_second_speaker);
            }
        }

        inline void jupiter_a9_freedom_leader_jupiter_sell_all_secondary_items(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (jup_a9_actor_has_evacuation_info(p_first_speaker, p_second_speaker))
            {
                actor_relocate_evacuation_info(p_first_speaker, p_second_speaker);
                jupiter_a9_freedom_leader_jupiter_evacuation(p_first_speaker, p_second_speaker);
            }
            if (jup_a9_actor_has_meeting_info(p_first_speaker, p_second_speaker))
            {
                actor_relocate_meeting_info(p_first_speaker, p_second_speaker);
                jupiter_a9_freedom_leader_jupiter_meeting(p_first_speaker, p_second_speaker);
            }
            if (jup_a9_actor_has_losses_info(p_first_speaker, p_second_speaker))
            {
                actor_relocate_losses_info(p_first_speaker, p_second_speaker);
                jupiter_a9_freedom_leader_jupiter_losses(p_first_speaker, p_second_speaker);
            }
            if (jup_a9_actor_has_delivery_info(p_first_speaker, p_second_speaker))
            {
                actor_relocate_delivery_info(p_first_speaker, p_second_speaker);
                jupiter_a9_freedom_leader_jupiter_delivery(p_first_speaker, p_second_speaker);
            }
        }

        inline void jup_a10_actor_give_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (Globals::has_alife_info("jup_a10_debt_wo_percent"))
            {
                relocate_money_from_actor(p_first_speaker, p_second_speaker, 5000);
                DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a10_bandit_take_money");
            }
            else
            {
                relocate_money_from_actor(p_first_speaker, p_second_speaker, 7000);
                DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_a10_bandit_take_all_money");
            }
        }

        inline void jup_a10_vano_give_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 5000);
        }

        inline void jup_a10_actor_give_outfit_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_from_actor(p_first_speaker, p_second_speaker, 5000);
        }

        inline bool jup_a10_actor_has_outfit_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->Money() >= 5000);
        }

        inline bool jup_a10_actor_has_not_outfit_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_a10_actor_has_outfit_money(p_first_speaker, p_second_speaker));
        }

        inline bool if_actor_has_jup_b16_oasis_artifact(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (p_first_speaker->GetObjectByName("af_oasis_heart"));
        }

        inline bool if_actor_hasnt_jup_b16_oasis_artifact(
            CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(if_actor_has_jup_b16_oasis_artifact(p_first_speaker, p_second_speaker));
        }

        inline void jupiter_b16_reward(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 7000);
        }

        inline void give_jup_b16_oasis_artifact(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "af_oasis_heart");
        }

        inline bool jup_a12_actor_has_15000_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->Money() >= 15000);
        }

        inline bool jup_a12_actor_do_not_has_15000_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->Money() < 15000);
        }

        inline bool jup_a12_actor_has_artefacts(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            for (const std::pair<std::uint32_t, xr_string>& it : Script_GlobalHelper::getInstance().getJupA12AfTable())
            {
                if (DataBase::Storage::getInstance().getActor()->GetObjectByName(it.second.c_str()))
                {
                    return true;
                }
            }

            return false;
        }

        inline bool jup_a12_actor_do_not_has_artefacts(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            for (const std::pair<std::uint32_t, xr_string>& it : Script_GlobalHelper::getInstance().getJupA12AfTable())
            {
                if (DataBase::Storage::getInstance().getActor()->GetObjectByName(it.second.c_str()))
                {
                    return false;
                }
            }

            return true;
        }

        inline bool jup_a12_actor_has_artefact_1(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->GetObjectByName(
                Script_GlobalHelper::getInstance().getJupA12AfTable().at(1).c_str()));
        }

        inline bool jup_a12_actor_has_artefact_2(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->GetObjectByName(Script_GlobalHelper::getInstance().getJupA12AfTable().at(2).c_str()));
        }

        inline bool jup_a12_actor_has_artefact_3(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->GetObjectByName(
                Script_GlobalHelper::getInstance().getJupA12AfTable().at(3).c_str()));
        }

        inline bool jup_a12_actor_has_artefact_4(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->GetObjectByName(Script_GlobalHelper::getInstance().getJupA12AfTable().at(4).c_str()));
        }

        inline void jup_a12_transfer_ransom_from_actor(
            CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (Globals::has_alife_info("jup_a12_ransom_by_money"))
            {
                relocate_money_from_actor(p_first_speaker, p_second_speaker, 15000);
                return;
            }

            xr_vector<xr_string> artefacts = {
                "jup_a12_af_fire", "jup_a12_af_gold_fish", "jup_a12_af_glass", "jup_a12_af_ice"};

            for (const xr_string& it : artefacts)
            {
                if (Globals::has_alife_info(it))
                {
                    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, it);
                    return;
                }
            }
        }

        inline void jup_a12_transfer_5000_money_to_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 5000);
            Script_TreasureManager::getInstance().give_treasure("jup_hiding_place_40");
            Script_TreasureManager::getInstance().give_treasure("jup_hiding_place_34");
        }

        inline void jup_a12_transfer_artefact_to_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "af_gold_fish");
            if (Globals::has_alife_info("jup_a12_stalker_prisoner_free_dialog_done"))
            {
                Script_TreasureManager::getInstance().give_treasure("jup_hiding_place_40");
                Script_TreasureManager::getInstance().give_treasure("jup_hiding_place_34");
            }
        }

        inline void jup_a12_transfer_cashier_money_from_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            std::uint32_t amount =
                Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(20, 50) * 100;
            if (DataBase::Storage::getInstance().getActor()->Money() < amount)
            {
                amount = DataBase::Storage::getInstance().getActor()->Money();
            }

            relocate_money_from_actor(p_first_speaker, p_second_speaker, amount);
        }

        inline void zat_b30_transfer_detectors(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            constexpr const char* amount = "3";
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "detector_elite", amount);
        }

        inline bool zat_b30_actor_has_transfer_items(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            int count = 0;

            auto iterate_detector = [&](CScriptGameObject* const p_item) -> void {
                if (p_item->Section() == xr_string("detector_elite"))
                    ++count;
            };

            DataBase::Storage::getInstance().getActor()->IterateInventory(iterate_detector);

            if (count > 2)
            {
                return true;
            }

            return false;
        }

        inline bool zat_b30_actor_do_not_has_transfer_items(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(zat_b30_actor_has_transfer_items(p_first_speaker, p_second_speaker));
        }

        inline bool jup_b6_scientist_nuclear_physicist_scan_anomaly_precond(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (!Globals::has_alife_info("jup_b6_b32_quest_active"))
                return false;
            else if (Globals::has_alife_info("jup_b6_give_task") && Globals::has_alife_info("jup_b32_task_addon_start"))
                return false;

            return true;
        }

        inline bool jup_b32_task_give_dialog_precond(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (Globals::has_alife_info("jup_b32_task_start") && !Globals::has_alife_info("jup_b32_task_end"))
                return false;

            return true;
        }

        inline void jup_b32_transfer_scanners(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "jup_b32_scanner_device", 3);
        }

        inline void jup_b32_transfer_scanners_2(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "jup_b32_scanner_device", 2);
        }

        inline void jup_b32_give_reward_to_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            constexpr std::uint32_t amount = 5000;
            relocate_money_to_actor(p_first_speaker, p_second_speaker, amount);
        }

        inline bool jup_b32_anomaly_has_af(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            xr_vector<xr_string> anomaly_zones = {
                "jup_b32_anomal_zone",
                "jup_b201_anomal_zone",
                "jup_b209_anomal_zone",
                "jup_b211_anomal_zone",
                "jup_b10_anomal_zone"
            };

            xr_vector<xr_string> infoportions = {
                "jup_b32_anomaly_1",
                "jup_b32_anomaly_2",
                "jup_b32_anomaly_3",
                "jup_b32_anomaly_4",
                "jup_b32_anomaly_5"
            };

            std::uint32_t index = 0;
            for (const xr_string& it : infoportions)
            {
                if (Globals::has_alife_info(it))
                {
                    break;
                }
                ++index;
            }

            if (!index)
                return false;

            CScriptGameObject* const p_anomal_zone =
                DataBase::Storage::getInstance().getAnomalyByName().at(anomaly_zones.at(index));

            if (!p_anomal_zone)
                return false;

            Script_Binder_Anomaly* const p_binder = dynamic_cast<Script_Binder_Anomaly*>(p_anomal_zone->binded_object());

            if (!p_binder)
            {
                Msg("[Scripts/jup_b32_anomaly_has_af(p_first_speaker, p_second_speaker)] WARNING: bad cast! Return ...");
                return false;
            }
            
            if (p_binder->getSpawnedCount())
            {
                if (Globals::has_alife_info(infoportions.at(index)))
                {
                    DataBase::Storage::getInstance().getActor()->DisableInfoPortion(infoportions.at(index).c_str());
                }

                DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b32_anomaly_true");
                return true;
            }

            return false;
        }

        inline bool jup_b32_anomaly_do_not_has_af(
            CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (Globals::has_alife_info("jup_b32_anomaly_true"))
            {
                DataBase::Storage::getInstance().getActor()->DisableInfoPortion("jup_b32_anomaly_true");
                return false;
            }

            xr_vector<xr_string> anomaly_zones = {"jup_b32_anomal_zone", "jup_b201_anomal_zone", "jup_b209_anomal_zone",
                "jup_b211_anomal_zone", "jup_b10_anomal_zone"};

            xr_vector<xr_string> infoportions = {"jup_b32_anomaly_1", "jup_b32_anomaly_2", "jup_b32_anomaly_3",
                "jup_b32_anomaly_4", "jup_b32_anomaly_5"};

            std::uint32_t index = 0;
            for (const xr_string& it : infoportions)
            {
                if (Globals::has_alife_info(it))
                {
                    break;
                }
                ++index;
            }

            if (!index)
                return true;

            CScriptGameObject* const p_anomal_zone =
                DataBase::Storage::getInstance().getAnomalyByName().at(anomaly_zones.at(index));

            if (!p_anomal_zone)
            {
                if (Globals::has_alife_info(infoportions.at(index)))
                {
                    DataBase::Storage::getInstance().getActor()->DisableInfoPortion(infoportions.at(index).c_str());
                }

                return true;
            }


            Script_Binder_Anomaly* const p_binder =
                dynamic_cast<Script_Binder_Anomaly*>(p_anomal_zone->binded_object());

            if (!p_binder)
            {
                Msg("[Scripts/jup_b32_anomaly_has_af(p_first_speaker, p_second_speaker)] WARNING: bad cast! Return "
                    "...");
                if (Globals::has_alife_info(infoportions.at(index)))
                {
                    DataBase::Storage::getInstance().getActor()->DisableInfoPortion(infoportions.at(index).c_str());
                }
                return true;
            }

            if (p_binder->getSpawnedCount())
            {
                return false;
            }

            if (Globals::has_alife_info(infoportions.at(index)))
            {
                DataBase::Storage::getInstance().getActor()->DisableInfoPortion(infoportions.at(index).c_str());
            }
            return true;
        }

        inline void jup_b209_get_monster_scanner(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "jup_b209_monster_scanner");
        }

        inline void jup_b209_return_monster_scanner(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b209_monster_scanner");
        }

        inline bool jup_b207_generic_decrypt_need_dialog_precond(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b9_blackbox"))
                return true;

            if (XR_CONDITION::check_npc_name_client(p_first_speaker, p_second_speaker, {"mechanic"}) ||
                XR_CONDITION::check_npc_name_client(p_first_speaker, p_second_speaker, {"tech"}))
                return false;
            
            return false;
        }

        inline bool jup_b207_actor_has_dealers_pda(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("device_pda_zat_b5_dealer"));
        }

        inline void jup_b207_sell_dealers_pda(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            constexpr std::uint32_t amount = 4000;
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "device_pda_zat_b5_dealer");
            relocate_money_to_actor(p_first_speaker, p_second_speaker, amount);
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b207_dealers_pda_sold");
        }

        inline void jup_b207_give_dealers_pda(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "device_pda_zat_b5_dealer");
        }

        inline bool jup_b207_actor_has_merc_pda_with_contract(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b207_merc_pda_with_contract"));
        }

        inline void jup_b207_sell_merc_pda_with_contract(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            constexpr std::uint32_t amount = 1000;
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b207_merc_pda_with_contract");
            relocate_money_to_actor(p_first_speaker, p_second_speaker, amount);
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b207_merc_pda_with_contract_sold");
        }

        inline void jup_b207_transfer_blackmail_reward(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b207_merc_pda_with_contract");
        }

        inline void jup_b207_transfer_blackmail_reward_for_pda(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b207_merc_pda_with_contract");
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "wpn_abakan");
        }

        inline void give_jup_b1_art(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b1_half_artifact");
        }

        inline bool if_actor_has_jup_b1_art(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !!(DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b1_half_artifact"));
        }

        inline bool jup_b1_actor_have_good_suit(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            bool is_actor_true = false;
            xr_map<xr_string, bool> suits_table;
            suits_table["scientific_outfit"] = true;
            suits_table["military_outfit"] = true;
            suits_table["dolg_heavy_outfit"] = true;
            suits_table["exo_outfit"] = true;
            suits_table["svoboda_light_outfit"] = true;
            suits_table["svoboda_heavy_outfit"] = true;
            suits_table["cs_heavy_outfit"] = true;

            xr_map<xr_string, bool> helmets_table;
            helmets_table["helm_battle"] = true;
            helmets_table["helm_tactic"] = true;
            helmets_table["helm_protective"] = true;

            if (DataBase::Storage::getInstance().getActor()->item_in_slot(7) && suits_table.at(DataBase::Storage::getInstance().getActor()->item_in_slot(7)->Section()))
            {
                is_actor_true = true;
            }

            if (DataBase::Storage::getInstance().getActor()->item_in_slot(12) && suits_table.at(DataBase::Storage::getInstance().getActor()->item_in_slot(12)->Section()))
            {
                is_actor_true = true;
            }

            return is_actor_true;
        }

        inline bool jup_b1_actor_do_not_have_good_suit(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_b1_actor_have_good_suit(p_first_speaker, p_second_speaker));
        }

        inline void jup_b1_reward_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 6000);
        }

        inline bool jup_b6_actor_outfit_cs(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (DataBase::Storage::getInstance().getActor()->item_in_slot(7) && DataBase::Storage::getInstance().getActor()->Section() == xr_string("cs_heavy_outfit"))
            {
                return true;
            }

            return false;
        }

        inline void jup_b6_first_reward_for_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 2500);
        }

        inline void jup_b6_second_reward_for_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 2500);
        }

        inline void jup_b6_all_reward_for_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 5000);
        }

        inline void jup_b6_first_reward_for_actor_extra(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 3500);
        }

        inline void jup_b6_second_reward_for_actor_extra(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 3500);
        }
        
        inline void jup_b6_all_reward_for_actor_extra(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 7000);
        }

        inline void jup_b6_reward_actor_by_detector(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "detector_elite");
        }

        inline bool jup_b6_actor_can_start(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (Globals::has_alife_info("jup_b1_squad_is_dead") && !(Globals::has_alife_info("jup_b6_freedom_employed") || Globals::has_alife_info("jup_b6_duty_employed") || Globals::has_alife_info("jup_b6_gonta_employed") || Globals::has_alife_info("jup_b6_exprisoner_work_on_sci")))
                return false;
            

            return true;
        }

        inline bool jup_b6_actor_can_not_start(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_b6_actor_can_start(p_first_speaker, p_second_speaker));
        }

        inline bool jup_b6_stalker_dialog_precond(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
            CSE_Abstract* const p_server_npc = ai().alife().objects().object(p_npc->ID());

            if (!p_server_npc)
                return false;
            CSE_ALifeHumanAbstract* const p_creature = p_server_npc->cast_human_abstract();
            if (!ai().alife().objects().object(p_creature->m_group_id))
            {
                return false;
            }

            xr_string squad_name = ai().alife().objects().object(p_creature->m_group_id)->name();

            if (squad_name.empty() == false)
            {
                if (!Globals::has_alife_info("jup_b1_squad_is_dead") && squad_name == "jup_b1_stalker_squad")
                    return true;
                else if (Globals::has_alife_info("jup_b6_freedom_employed") &&
                    squad_name == "jup_b6_stalker_freedom_squad")
                    return true;
                else if (Globals::has_alife_info("jup_b6_duty_employed") && squad_name == "jup_b6_stalker_duty_squad")
                    return true;
                else if (Globals::has_alife_info("jup_b6_gonta_employed") && squad_name == "jup_b6_stalker_gonta_squad")
                    return true;
                else if (Globals::has_alife_info("jup_b6_exprisoner_work_on_sci") &&
                    squad_name == "jup_b6_stalker_exprisoner_squad")
                    return true;
            }

            return false;
        }

        inline void jup_b1_stalker_squad_thanks(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "medkit_scientic", 3);
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "antirad", 5);
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "drug_psy_blockade", 2);
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "drug_antidot", 2);
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "drug_radioprotector", 2);
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "drug_anabiotic");
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "helm_protective");
        }

        inline bool jup_b217_actor_got_toolkit(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            xr_string tool_name;
            auto is_toolkit = [&](CScriptGameObject* const p_item) -> void {
                xr_string section_name = p_item->Section();

                if ((section_name == "toolkit_1" && !Globals::has_alife_info("jup_b217_tech_instrument_1_brought")) ||
                    (section_name == "toolkit_2" && !Globals::has_alife_info("jup_b217_tech_instrument_2_brought")) ||
                    (section_name == "toolkit_3" && !Globals::has_alife_info("jup_b217_tech_instrument_3_brought")))
                {
                    tool_name = section_name;
                    return;
                }

            };

            DataBase::Storage::getInstance().getActor()->IterateInventory(is_toolkit);

            return (!tool_name.empty());
        }

        inline void jupiter_b200_tech_materials_relocate(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            xr_map<xr_string, bool> need_items;
            need_items["jup_b200_tech_materials_wire"] = true;
            need_items["jup_b200_tech_materials_acetone"] = true;
            need_items["jup_b200_tech_materials_textolite"] = true;
            need_items["jup_b200_tech_materials_transistor"] = true;
            need_items["jup_b200_tech_materials_capacitor"] = true;

            std::uint32_t count = 0;
            xr_map<xr_string, std::uint32_t> items_to_relocate;

            auto items_iterator = [&](CScriptGameObject* const p_item) -> void {
                if (need_items.find(p_item->Section()) != need_items.end())
                {
                    ++count;

                    if (items_to_relocate.find(p_item->Section()) == items_to_relocate.end())
                    {
                        items_to_relocate[p_item->Section()] = 1;
                    }
                    else
                    {
                        ++items_to_relocate[p_item->Section()];
                    }
                }
            };

            DataBase::Storage::getInstance().getActor()->IterateInventory(items_iterator);
            XR_EFFECTS::inc_counter(DataBase::Storage::getInstance().getActor(), nullptr, {"jup_b200_tech_materials_brought_counter", std::to_string(count).c_str()});

            for (const std::pair<xr_string, std::uint32_t>& it : items_to_relocate)
            {
                relocate_item_section_from_actor(p_first_speaker, p_second_speaker, it.first, std::to_string(it.second).c_str());
            }
        }

        inline bool npc_in_b4_smart(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return is_npc_in_current_smart(p_first_speaker, p_second_speaker, "jup_b4");
        }

        inline void jup_b202_transfer_medkit(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();

            if (p_actor->GetObjectByName("medkit"))
            {
                relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "medkit");
            }
            else if (p_actor->GetObjectByName("medkit_army"))
            {
                relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "medkit_army");
            }
            else if (p_actor->GetObjectByName("medkit_scientic"))
            {
                relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "medkit_scientic");
            }
        }

        inline bool jup_b202_actor_has_medkit(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
            bool is_has_medkit = p_actor->GetObjectByName("medkit");
            bool is_has_medkit_army = p_actor->GetObjectByName("medkit_army");
            bool is_has_medkit_scientic = p_actor->GetObjectByName("medkit_scientic");

            return ((is_has_medkit) || (is_has_medkit_army) || (is_has_medkit_scientic));
        }

        inline void jup_b202_hit_bandit_from_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
            CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);

            p_actor->GiveInfoPortion("jup_b202_bandit_hited");
            p_actor->GiveInfoPortion("jup_b202_bandit_hited_by_actor");

            XR_EFFECTS::set_squad_goodwill(p_actor, p_npc, {"jup_b202_bandit_squad", "bandit"});
        }

        inline bool jup_b202_medic_dialog_precondition(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (Globals::has_alife_info("jup_b218_gather_squad_complete"))
                return !(Globals::has_alife_info("jup_b202_polustanok"));
            else
                return !(Globals::has_alife_info("jup_b52_medic_testimony"));
        }

        inline bool jupiter_b220_all_hunted(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            bool is_info1 = Globals::has_alife_info("jup_b220_trapper_bloodsucker_lair_hunted_told");
            bool is_info2 = Globals::has_alife_info("jup_b220_trapper_zaton_chimera_hunted_told");
            bool is_info3 = Globals::has_alife_info("jup_b211_swamp_bloodsuckers_hunt_done");
            bool is_info4 = Globals::has_alife_info("jup_b208_burers_hunt_done");
            bool is_info5 = Globals::has_alife_info("jup_b212_jupiter_chimera_hunt_done");

            if (is_info1 && is_info2 && is_info3 && is_info4 && is_info5)
                return false;
            
            return true;
        }

        inline bool jupiter_b220_no_one_hunted(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (Globals::has_alife_info("jup_b220_trapper_about_himself_told") && Globals::has_alife_info("zat_b57_den_of_the_bloodsucker_tell_stalkers_about_destroy_lair_give") && (!(Globals::has_alife_info("jup_b220_trapper_bloodsucker_lair_hunted_told"))))
            {
                return false;
            }
            else if (Globals::has_alife_info("zat_b106_chimera_dead") && !(Globals::has_alife_info("jup_b220_trapper_zaton_chimera_hunted_told")))
            {
                return false;
            }
            else if (Globals::has_alife_info("jup_b6_all_hunters_are_dead") && !(Globals::has_alife_info("jup_b211_swamp_bloodsuckers_hunt_done")))
            {
                return false;
            }
            else if (Globals::has_alife_info("jup_b208_burers_dead") && !(Globals::has_alife_info("jup_b208_burers_hunt_done")))
            {
                return false;
            }
            else if (Globals::has_alife_info("jup_b212_jupiter_chimera_dead") && !(Globals::has_alife_info("jup_b212_jupiter_chimera_hunt_done")))
            {
                return false;
            }

            return true;
        }

        inline bool jup_b9_actor_has_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            std::uint32_t money = Script_GlobalHelper::getInstance().getJupB9MoneyCount()[0];
            for (std::uint32_t i = 1; i <= 9; ++i)
            {
                if (Globals::has_alife_info(xr_string("jup_b200_tech_materials_brought_counter_").append(std::to_string(i).c_str())))
                {
                    money = Script_GlobalHelper::getInstance().getJupB9MoneyCount().at(i);
                }
            }

            return (DataBase::Storage::getInstance().getActor()->Money() >= money);
        }

        inline void give_jup_b9_blackbox(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b9_blackbox");
        }

        inline bool jup_b9_actor_has_not_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_b9_actor_has_money(p_first_speaker, p_second_speaker));
        }

        inline void jupiter_b9_relocate_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            std::uint32_t money = Script_GlobalHelper::getInstance().getJupB9MoneyCount()[0];
            for (std::uint32_t i = 1; i <= 9; ++i)
            {
                if (Globals::has_alife_info(
                        xr_string("jup_b200_tech_materials_brought_counter_").append(std::to_string(i).c_str())))
                {
                    money = Script_GlobalHelper::getInstance().getJupB9MoneyCount().at(i);
                }
            }

            relocate_money_from_actor(p_first_speaker, p_second_speaker, money);
        }

        inline bool if_actor_has_jup_b9_blackbox(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b9_blackbox"));
        }

        inline bool if_actor_has_af_mincer_meat(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (p_first_speaker->GetObjectByName("af_mincer_meat"));
        }

        inline bool if_actor_has_af_fuzz_kolobok(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (p_first_speaker->GetObjectByName("af_fuzz_kolobok"));
        }

        inline bool actor_has_first_or_second_artefact(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (p_first_speaker->GetObjectByName("af_mincer_meat") || p_first_speaker->GetObjectByName("af_fuzz_kolobok"));
        }

        inline void transfer_af_mincer_meat(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "af_mincer_meat");
        }

        inline void transfer_af_fuzz_kolobok(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "af_fuzz_kolobok");
        }

        inline void pay_cost_to_guide_to_pripyat(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_from_actor(p_first_speaker, p_second_speaker, 5000);
        }

        inline bool jup_b43_actor_has_5000_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->Money() >= 5000);
        }

        inline bool jup_b43_actor_do_not_has_5000_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_b43_actor_has_5000_money(p_first_speaker, p_second_speaker));
        }

        inline void jup_b43_reward_for_first_artefact(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 2500);
        }

        inline void jup_b43_reward_for_second_artefact(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 3500);
        }

        inline void jup_b43_reward_for_both_artefacts(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 6000);
        }

        inline void jup_b15_dec_counter(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
            CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);

            XR_EFFECTS::dec_counter(p_actor, p_npc, {"jup_b15_full_drunk_count", "1"});
        }

        inline void jup_b46_transfer_duty_founder_pda(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
            if (DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b46_duty_founder_pda"))
            {
                relocate_item_section(p_npc, "jup_b46_duty_founder_pda", "out");
            }
        }

        inline void jup_b46_sell_duty_founder_pda_to_owl(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b46_duty_founder_pda");
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 2500);
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b46_duty_founder_pda_sold");
            DataBase::Storage::getInstance().getActor()->GiveInfoPortion("jup_b46_duty_founder_pda_to_stalkers");
        }

        inline bool jup_b46_actor_has_founder_pda(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b46_duty_founder_pda"));
        }

        inline void jup_b46_sell_duty_founder_pda(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);

            if (Globals::has_alife_info("jup_b46_duty_founder_pda_to_freedom"))
            {
                relocate_money_to_actor(p_first_speaker, p_second_speaker, 4000);
                relocate_item_section(p_actor, "wpn_sig550", "in", 1);
                relocate_item_section(p_actor, "ammo_5.56x45_ss190", "in", 5);
            }
            else if (Globals::has_alife_info("jup_b46_duty_founder_pda_to_duty"))
            {
                relocate_money_to_actor(p_first_speaker, p_second_speaker, 4000);
                relocate_item_section(p_actor, "wpn_groza", "in");
                relocate_item_section(p_actor, "ammo_9x39_ap", "in", 2);
                relocate_item_section(p_actor, "ammo_vog-25", "in", 2);
            }
        }

        inline bool jup_b218_counter_not_3(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
            CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);

            return !(XR_CONDITION::is_counter_equal_client(p_actor, p_npc, {"jup_b218_squad_members_count", "3"}));
        }

        inline bool jup_b218_counter_equal_3(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
            CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);

            return (XR_CONDITION::is_counter_equal_client(p_actor, p_npc, {"jup_b218_squad_members_count", "3"}));
        }

        inline bool jup_b218_counter_not_0(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
            CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);

            return !(XR_CONDITION::is_counter_equal_client(p_actor, p_npc, {"jup_b218_squad_members_count", "0"}));
        }

        inline bool jup_b4_is_actor_enemies_to_freedom(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return Globals::GameRelations::get_npcs_relation(p_first_speaker, p_second_speaker) ==
                ALife::ERelationType::eRelationTypeEnemy;
        }

        inline bool jup_b4_is_actor_not_enemies_to_freedom(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_b4_is_actor_enemies_to_freedom(p_first_speaker, p_second_speaker));
        }

        inline bool jup_b4_is_actor_friend_to_freedom(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return Globals::GameRelations::get_npcs_relation(p_first_speaker, p_second_speaker) ==
                ALife::ERelationType::eRelationTypeFriend;
        }

        inline bool jup_b4_is_actor_neutral_to_freedom(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return Globals::GameRelations::get_npcs_relation(p_first_speaker, p_second_speaker) ==
                ALife::ERelationType::eRelationTypeNeutral;
        }

        inline bool jup_b4_is_actor_enemies_to_dolg(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return Globals::GameRelations::get_npcs_relation(p_first_speaker, p_second_speaker) ==
                ALife::ERelationType::eRelationTypeEnemy;
        }

        inline bool jup_b4_is_actor_not_enemies_to_dolg(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_b4_is_actor_enemies_to_dolg(p_first_speaker, p_second_speaker));
        }

        inline bool jup_b4_is_actor_friend_to_dolg(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return Globals::GameRelations::get_npcs_relation(p_first_speaker, p_second_speaker) ==
                ALife::ERelationType::eRelationTypeFriend;
        }

        inline bool jup_b4_is_actor_neutral_to_dolg(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return Globals::GameRelations::get_npcs_relation(p_first_speaker, p_second_speaker) == ALife::ERelationType::eRelationTypeNeutral;
        }

        inline void jup_b25_frase_count_inc(
            CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
            CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);

            XR_EFFECTS::inc_counter(p_actor, p_npc, {"jup_b25_frase", "1"});
        }

        inline bool jup_b47_jupiter_docs_enabled(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            bool is_a = false;
            bool is_b = false;
            bool is_c = false;

            xr_vector<xr_string> items_table = {"jup_a9_conservation_info", "jup_a9_power_info", "jup_a9_way_info",
                "jup_a9_evacuation_info", "jup_a9_meeting_info", "jup_a9_losses_info",
                "jup_a9_delivery_info"
                "jup_b47_jupiter_products_info"};

            for (const xr_string& it : items_table)
            {
                if (DataBase::Storage::getInstance().getActor()->GetObjectByName(it.c_str()))
                {
                    is_a = true;
                    break;
                }
            }

            is_b = ((!Globals::has_alife_info("jup_b47_jupiter_products_start")) &&
                (DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b47_jupiter_products_info")));
            is_c = (Globals::has_alife_info("jup_b6_scientist_nuclear_physicist_jupiter_docs_talked"));

            return (is_a || is_b) && (!is_c);        
        }

        inline bool jup_b47_jupiter_products_info_enabled(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b47_jupiter_products_info");
        }

        inline bool jup_b47_jupiter_products_info_disabled(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b47_jupiter_products_info"));
        }

        inline void jup_b47_jupiter_products_info_revard(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
            CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);

            relocate_item_section(p_npc, "jup_b47_jupiter_products_info", "out");
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 7000);
            relocate_item_section(p_actor, "medkit_scientic", "in", 3);
            relocate_item_section(p_actor, "antirad", "in", 5);
            relocate_item_section(p_actor, "drug_psy_blockade", "in", 2);
            relocate_item_section(p_actor, "drug_antidot", "in", 2);
            relocate_item_section(p_actor, "drug_radioprotector", "in", 2);
        }

        inline bool jup_b47_actor_has_merc_pda(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b47_merc_pda"));
        }

        inline bool jup_b47_actor_has_not_merc_pda(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_b47_actor_has_merc_pda(p_first_speaker, p_second_speaker));
        }

        inline void jup_b47_merc_pda_revard(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
            relocate_item_section(p_npc, "jup_b47_merc_pda", "out");
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 2500);
        }

        inline bool jup_b47_actor_can_take_task(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            bool is_a = Globals::has_alife_info("jup_b6_task_done") && (!Globals::has_alife_info("jup_b6_task_fail"));
            bool is_b = Globals::has_alife_info("jup_b6_task_fail") && (!Globals::has_alife_info("jup_b6_task_done"));

            return is_a || is_b;
        }

        inline bool jup_b47_employ_squad(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            bool is_a = Globals::has_alife_info("jup_b47_bunker_guards_started") && !(Globals::has_alife_info("jup_b47_bunker_guards_done"));
            bool is_b = Globals::has_alife_info("jup_b6_employ_stalker") && !(Globals::has_alife_info("jup_b6_employed_stalker"));

            return is_a || is_b;
        }

        inline void jup_b47_bunker_guard_revard(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 4000);
            relocate_item_section(p_actor, "drug_psy_blockade", "in", 2);
            relocate_item_section(p_actor, "drug_antidot", "in", 3);
            relocate_item_section(p_actor, "drug_radioprotector", "in", 3);
        }

        inline void jup_b47_gauss_rifle_revard(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 12000);
        }

        inline bool jup_b47_actor_has_hauss_rifle_docs(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->GetObjectByName("zat_a23_gauss_rifle_docs"));
        }

        inline void jup_b10_ufo_memory_give_to_npc(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b10_ufo_memory");
        }

        inline bool jup_b10_ufo_memory_give_to_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return p_first_speaker->GetObjectByName("jup_b10_ufo_memory");
        }

        inline void jup_b10_ufo_memory_2_give_to_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "jup_b10_ufo_memory_2");
        }

        inline bool jup_b10_ufo_has_money_1000(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->Money() >= 1000);
        }

        inline bool jup_b10_ufo_has_money_3000(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->Money() >= 2000);
        }

        inline bool jup_b10_ufo_hasnt_money_1000(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_b10_ufo_has_money_1000(p_first_speaker, p_second_speaker));
        }

        inline bool jup_b10_ufo_hasnt_money_3000(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_b10_ufo_has_money_3000(p_first_speaker, p_second_speaker));
        }

        inline void jup_b10_ufo_relocate_money_1000(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_from_actor(p_first_speaker, p_second_speaker, 1000);
        }

        inline void jup_b10_ufo_relocate_money_3000(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_from_actor(p_first_speaker, p_second_speaker, 2000);
        }

        inline bool jup_b10_actor_has_ufo_memory(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b10_ufo_memory"));
        }

        inline void jup_b211_kill_bludsuckers_reward(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 3000);
        }

        inline void jup_b19_transfer_conserva_to_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "conserva");
        }

        inline void jupiter_b6_sell_halfartefact(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_to_actor(p_first_speaker, p_second_speaker, 2000);
        }

        inline bool pri_a15_sokolov_actor_has_note(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return DataBase::Storage::getInstance().getActor()->GetObjectByName("jup_b205_sokolov_note");
        }

        inline bool pri_a15_sokolov_actor_has_not_note(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(pri_a15_sokolov_actor_has_note(p_first_speaker, p_second_speaker));
        }

        inline void pri_a15_sokolov_actor_give_note(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_from_actor(p_first_speaker, p_second_speaker, "jup_b205_sokolov_note");
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "medkit_army");
        }

        inline bool jup_b47_actor_not_enemy_to_freedom(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
            CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);

            return !(XR_CONDITION::is_faction_enemy_to_actor_client(p_actor, p_npc, {"freedom"}));
        }

        inline bool jup_b47_actor_not_enemy_to_dolg(
            CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_npc = who_is_npc(p_first_speaker, p_second_speaker);
            CScriptGameObject* const p_actor = who_is_actor(p_first_speaker, p_second_speaker);

            return !(XR_CONDITION::is_faction_enemy_to_actor_client(p_actor, p_npc, {"dolg"}));
        }

        inline bool jup_b15_actor_sci_outfit(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return (DataBase::Storage::getInstance().getActor()->GetObjectByName("scientific_outfit"));
        }

        inline bool jup_b15_no_actor_sci_outfit(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_b15_actor_sci_outfit(p_first_speaker, p_second_speaker));
        }

        inline void jup_b19_reward(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            Script_TreasureManager::getInstance().give_treasure("jup_hiding_place_38");
        }
    }
}
