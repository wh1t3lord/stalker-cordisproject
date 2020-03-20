#pragma once


namespace Cordis
{
    namespace Scripts
    {
        inline void pri_b301_zulus_reward(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "wpn_pkm_zulus");
        }

        inline void pri_a17_reward(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (Globals::has_alife_info("pri_a17_reward_well"))
            {
                relocate_money_to_actor(p_first_speaker, p_second_speaker, 7500);
            }
            else if (Globals::has_alife_info("pri_a17_reward_norm"))
            {
                relocate_money_to_actor(p_first_speaker, p_second_speaker, 4000);
            }
            else if (Globals::has_alife_info("pri_a17_reward_bad"))
            {
                relocate_money_to_actor(p_first_speaker, p_second_speaker, 3000);
            }
        }

        inline bool actor_has_pri_a17_gauss_rifle(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return DataBase::Storage::getInstance().getActor()->GetObjectByName("pri_a17_gauss_rifle");
        }

        inline bool actor_hasnt_pri_a17_gauss_rifle(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(actor_has_pri_a17_gauss_rifle(p_first_speaker, p_second_speaker));
        }

        inline void transfer_artifact_af_baloon(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "af_baloon");
        }

        inline void pay_cost_to_guide_to_zaton(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (Globals::has_alife_info("zat_b215_gave_maps"))
            {
                relocate_money_from_actor(p_first_speaker, p_second_speaker, 1000);
            }
            else
            {
                relocate_money_from_actor(p_first_speaker, p_second_speaker, 3000);
            }
        }

        inline bool jup_b43_actor_has_10000_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (Globals::has_alife_info("zat_b215_gave_maps"))
                return DataBase::Storage::getInstance().getActor()->Money() >= 3000;

            return DataBase::Storage::getInstance().getActor()->Money() >= 5000;
        }

        inline bool jup_b43_actor_do_not_has_10000_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_b43_actor_has_10000_money(p_first_speaker, p_second_speaker));
        }

        inline void pay_cost_to_guide_to_jupiter(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_money_from_actor(p_first_speaker, p_second_speaker, 7000);
        }

        inline bool jup_b43_actor_has_7000_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return DataBase::Storage::getInstance().getActor()->Money() >= 7000;
        }

        inline bool jup_b43_actor_do_not_has_7000_money(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            return !(jup_b43_actor_has_7000_money(p_first_speaker, p_second_speaker));
        }

        inline void pri_b35_transfer_svd(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "wpn_svd");
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "ammo_7.62x54_7h1");
        }

        inline void pri_b35_give_actor_reward(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            std::uint32_t amount = 1;

            if (Globals::has_alife_info("pri_b35_secondary"))
                amount = 3;

            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "ammo_7.62x54_7h1", amount);
        }

        inline void pri_a25_medic_give_kit(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            xr_string kit_name = "basic";

            if (Globals::has_alife_info("pri_a25_actor_needs_medikit_advanced_supply"))
            {
                kit_name = "advanced";
            }
            else if (Globals::has_alife_info("pri_a25_actor_needs_medikit_elite_supply"))
            {
                kit_name = "elite";
            }

            for (const std::pair<xr_string, xr_map<xr_string, std::uint32_t>>& it : Script_GlobalHelper::getInstance().getPriA25MedicItems())
            {
                if (kit_name == it.first)
                {
                    for (const std::pair<xr_string, std::uint32_t>& it2 : it.second)
                    {
                        relocate_item_section_to_actor(p_first_speaker, p_second_speaker, it2.first, it2.second);
                    }

                    if (Globals::has_alife_info(it.first))
                    {
                        DataBase::Storage::getInstance().getActor()->DisableInfoPortion(it.first.c_str());
                    }
                }
            }
        }

        inline void pri_a22_army_signaller_supply(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            for (const std::pair<xr_string, xr_map<xr_string, std::uint32_t>>& it : Script_GlobalHelper::getInstance().getPriA22SuppItems())
            {
                if (Globals::has_alife_info(it.first))
                {
                    for (const std::pair<xr_string, std::uint32_t>& it2 : it.second)
                    {
                        relocate_item_section_to_actor(p_first_speaker, p_second_speaker, it2.first, it2.second);
                    }

                    DataBase::Storage::getInstance().getActor()->DisableInfoPortion(it.first.c_str());
                }
            }
        }

        inline void pri_a22_give_actor_outfit(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "military_outfit");
            relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "helm_battle");
        }

        inline bool pri_b305_actor_has_strelok_notes(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
            bool is_note_1 = !!p_actor->GetObjectByName("jup_b10_notes_01");
            bool is_note_2 = !!p_actor->GetObjectByName("jup_b10_notes_02");
            bool is_note_3 = !!p_actor->GetObjectByName("jup_b10_notes_03");

            return (is_note_1) || (is_note_2) || (is_note_3);
        }

        inline bool pri_b305_actor_has_strelok_note_1(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
            bool is_note_1 = !!p_actor->GetObjectByName("jup_b10_notes_01");
            bool is_note_2 = !!p_actor->GetObjectByName("jup_b10_notes_02");
            bool is_note_3 = !!p_actor->GetObjectByName("jup_b10_notes_03");

            return (is_note_1) && (!is_note_2) && (!is_note_3);
        }

        inline bool pri_b305_actor_has_strelok_note_2(CScriptGameObject* const p_first_speaekr, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
            bool is_note_1 = !!p_actor->GetObjectByName("jup_b10_notes_01");
            bool is_note_2 = !!p_actor->GetObjectByName("jup_b10_notes_02");
            bool is_note_3 = !!p_actor->GetObjectByName("jup_b10_notes_03");

            return (!is_note_1) && (is_note_2) && (!is_note_3);
        }

        inline bool pri_b305_actor_has_strelok_note_3(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
            bool is_note_1 = !!p_actor->GetObjectByName("jup_b10_notes_01");
            bool is_note_2 = !!p_actor->GetObjectByName("jup_b10_notes_02");
            bool is_note_3 = !!p_actor->GetObjectByName("jup_b10_notes_03");

            return (!is_note_1) && (!is_note_2) && (is_note_3);
        }

        inline bool pri_b305_actor_has_strelok_note_12(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
            bool is_note_1 = !!p_actor->GetObjectByName("jup_b10_notes_01");
            bool is_note_2 = !!p_actor->GetObjectByName("jup_b10_notes_02");
            bool is_note_3 = !!p_actor->GetObjectByName("jup_b10_notes_03");

            return (is_note_1) && (is_note_2) && (!is_note_3);
        }

        inline bool pri_b305_actor_has_strelok_note_13(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
            bool is_note_1 = !!p_actor->GetObjectByName("jup_b10_notes_01");
            bool is_note_2 = !!p_actor->GetObjectByName("jup_b10_notes_02");
            bool is_note_3 = !!p_actor->GetObjectByName("jup_b10_notes_03");

            return (is_note_1) && (!is_note_2) && (is_note_3);
        }

        inline bool pri_b305_actor_has_strelok_note_23(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
            bool is_note_1 = !!p_actor->GetObjectByName("jup_b10_notes_01");
            bool is_note_2 = !!p_actor->GetObjectByName("jup_b10_notes_02");
            bool is_note_3 = !!p_actor->GetObjectByName("jup_b10_notes_03");

            return (!is_note_1) && (is_note_2) && (is_note_3);
        }

        inline bool pri_b305_actor_has_strelok_note_all(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            CScriptGameObject* const p_actor = DataBase::Storage::getInstance().getActor();
            bool is_note_1 = !!p_actor->GetObjectByName("jup_b10_notes_01");
            bool is_note_2 = !!p_actor->GetObjectByName("jup_b10_notes_02");
            bool is_note_3 = !!p_actor->GetObjectByName("jup_b10_notes_03");

            return (is_note_1) && (is_note_2) && (is_note_3);
        }

        inline void pri_b305_sell_strelok_notes(
            CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            std::uint32_t amount = 0;
            xr_vector<xr_string> notes = {"jup_b10_notes_01", "jup_b10_notes_02", "jup_b10_notes_03"};

            for (const xr_string& it : notes)
            {
                if (DataBase::Storage::getInstance().getActor()->GetObjectByName(it.c_str()))
                {
                    relocate_item_section_from_actor(p_first_speaker, p_second_speaker, it);
                    ++amount;
                }
            }

            if (DataBase::Storage::getInstance().getActor()->GetObjectByName("wpn_gauss"))
                relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "ammo_gauss", 2);
            else
                relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "medkit_scientic", 3);

            if (amount > 1)
                relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "af_fire");

            if (amount > 2)
            {
                relocate_item_section_to_actor(p_first_speaker, p_second_speaker, "af_glass");
                DataBase::Storage::getInstance().getActor()->GiveInfoPortion("pri_b305_all_strelok_notes_given");
            }

        }

        inline bool pri_a17_sokolov_is_not_at_base(CScriptGameObject* const p_first_speaker, CScriptGameObject* const p_second_speaker)
        {
            if (Globals::has_alife_info("pri_a15_sokolov_out") && Globals::has_alife_info("pas_b400_sokolov_dead"))
                return true;

            return false;
        }
    }
}
