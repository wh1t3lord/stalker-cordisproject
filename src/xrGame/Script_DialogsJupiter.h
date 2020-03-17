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
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "jup_b32_scanner_device", "3");
        }

        inline void jup_b32_transfer_scanners_2(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "jup_b32_scanner_device", "2");
        }

        inline void jup_b32_give_reward_to_actor(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            constexpr std::uint32_t amount = 5000;
            relocate_money_to_actor(p_first_speaker, p_second_speaker, amount);
        }


    }
}
