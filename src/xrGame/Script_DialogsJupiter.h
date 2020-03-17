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
    }
}
