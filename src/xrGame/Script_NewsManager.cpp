#include "stdafx.h"
#include "Script_NewsManager.h"

namespace Cordis
{
namespace Scripts
{
Script_NewsManager::~Script_NewsManager(void) {}

void Script_NewsManager::SendSound(CScriptGameObject* object, const xr_string& faction, const xr_string& point,
    const xr_string& str, const xr_string& str1, const float& delay)
{
    // @ может передаваться и nullptr, так уж не особой роли играет это
/*
    if (!object)
    {

    }


        if (faction.size())
        {
            xr_string point_name = "";
            if (!point.empty())
            {
                Script_SE_SmartTerrain* p_server_smart =
                    Script_SimulationBoard::getInstance().getSmartTerrainsByName().at(point);
                if (p_server_smart)
                    point_name = Script_GlobalHelper::getInstance().getTranslatedSmartTerrainName(p_server_smart);
                else
                    point_name = Globals::Game::translate_string(point.c_str());


            }
        } */
}

bool Script_NewsManager::SendTip(CScriptGameObject* actor, const xr_string& news_id_name, const xr_string& sender_name,
    const std::uint32_t timeout, const std::uint32_t show_time, const xr_string& sender_id_name)
{
    if (news_id_name.empty())
        return false;

    if (!sender_id_name.empty())
    {
        CSE_ALifeDynamicObject* p_server_npc =
            ai().alife().objects().object(Globals::get_story_object_id(sender_id_name));

        if (p_server_npc)
        {
            if (p_server_npc->m_bOnline)
            {
                if (CRD_Wounded::is_heavy_wounded_by_id(p_server_npc->ID))
                {
                    MESSAGEWR("can't send tip by heavy wounded object!");
                    return false;
                }
            }

            if (p_server_npc->cast_creature_abstract()->g_Alive())
            {
                MESSAGEWR("Can't send a tip by dead object!");
                return false;
            }
        }
    }

    XR_SOUND::set_sound_play(DataBase::Storage::getInstance().getActor()->ID(), "pda_tips", xr_string(), 0);

    xr_string texture_name = "ui_iconsTotal_grouping";

    if (sender_name.empty() == false)
    {
        if (Script_GlobalHelper::getInstance().getNewsManagerRegisteredSoundTips().find(sender_name) != Script_GlobalHelper::getInstance().getNewsManagerRegisteredSoundTips().end())
            texture_name = Script_GlobalHelper::getInstance().getNewsManagerRegisteredSoundTips().at(sender_name);
    }

    xr_string news_caption_name = Globals::Game::translate_string("st_tip");
    xr_string news_text_name = Globals::Game::translate_string(news_id_name);

    DataBase::Storage::getInstance().getActor()->GiveGameNews(news_caption_name.c_str(), news_text_name.c_str(), texture_name.c_str(), timeout * 1000, show_time, 0);

    return true;
}

void Script_NewsManager::SendTask(CScriptGameObject* const p_actor, const xr_string& type_name, CGameTask* const p_task)
{
    if (!p_actor)
        return;

    std::uint32_t time_on_screen = 10000;

    if (type_name == "updated")
        time_on_screen = 5000;

    XR_SOUND::set_sound_play(DataBase::Storage::getInstance().getActor()->ID(), "pda_task", xr_string(), 0);
    xr_string news_caption_name = Globals::Game::translate_string(
        Script_GlobalHelper::getInstance().getNewsManagerActionDescriptionByTypeName().at(type_name).c_str());
    xr_string news_text_name = Globals::Game::translate_string(p_task->GetTitle_script());
    xr_string icon_name = p_task->GetIconName_script();

    if (icon_name.empty())
        icon_name = "ui_iconsTotal_storyline";

    if (DataBase::Storage::getInstance().getActor()->IsTalking())
        DataBase::Storage::getInstance().getActor()->AddIconedTalkMessage(
            news_caption_name.c_str(), news_text_name.append(".").c_str(), icon_name.c_str(), "iconed_answer_item");
    else
        DataBase::Storage::getInstance().getActor()->GiveGameNews(
            news_caption_name.c_str(), news_text_name.append(".").c_str(), icon_name.c_str(), 0, time_on_screen);
}

void Script_NewsManager::relocate_money(const xr_string& type_name, const int amount)
{
    if (!DataBase::Storage::getInstance().getActor())
        return;

    if (type_name == "in")
    {
        xr_string news_caption_name = Globals::Game::translate_string("general_in_money");
        xr_string news_about_amount_name = Globals::Game::translate_string(std::to_string(amount).c_str());

        if (DataBase::Storage::getInstance().getActor()->IsTalking())
        {
            DataBase::Storage::getInstance().getActor()->AddIconedTalkMessage(news_caption_name.c_str(),
                news_about_amount_name.c_str(), "ui_inGame2_Dengi_polucheni", "iconed_answer_item");
        }
        else
        {
            DataBase::Storage::getInstance().getActor()->GiveGameNews(
                news_caption_name.c_str(), news_about_amount_name.c_str(), "ui_inGame2_Dengi_polucheni", 0, 3000);
        }
    }
    else if (type_name == "out")
    {
        xr_string news_caption_name = Globals::Game::translate_string("general_out_money");
        xr_string news_about_amount_name = Globals::Game::translate_string(std::to_string(amount).c_str());

        if (DataBase::Storage::getInstance().getActor()->IsTalking())
        {
            DataBase::Storage::getInstance().getActor()->AddIconedTalkMessage(news_caption_name.c_str(),
                news_about_amount_name.c_str(), "ui_inGame2_Dengi_otdani", "iconed_answer_item");
        }
        else
        {
            DataBase::Storage::getInstance().getActor()->GiveGameNews(
                news_caption_name.c_str(), news_about_amount_name.c_str(), "ui_inGame2_Dengi_otdani", 0, 3000);
        }
    }
}

void Script_NewsManager::relocate_item(
    const xr_string& type_name, const xr_string& section_name, const std::uint32_t amount)
{
    if (!DataBase::Storage::getInstance().getActor())
        return;

    xr_string news_caption_name;
    xr_string news_text_name;

    if (type_name == "in")
    {
        if (amount == 1)
        {
            news_caption_name = Globals::Game::translate_string("general_in_item");
            news_text_name =
                Globals::Game::translate_string(Globals::get_system_ini()->r_string(section_name.c_str(), "inv_name"));
        }
        else
        {
            news_caption_name = Globals::Game::translate_string("general_in_item");
            xr_string temporary_name = Globals::get_system_ini()->r_string(section_name.c_str(), "inv_name");
            temporary_name += " x";
            temporary_name += std::to_string(amount).c_str();
            news_text_name = temporary_name;
        }

        if (DataBase::Storage::getInstance().getActor()->IsTalking())
        {
            DataBase::Storage::getInstance().getActor()->AddIconedTalkMessage(
                news_caption_name.c_str(), news_text_name.c_str(), "ui_inGame2_Predmet_poluchen", "iconed_answer_item");
        }
        else
        {
            DataBase::Storage::getInstance().getActor()->GiveGameNews(
                news_caption_name.c_str(), news_text_name.c_str(), "ui_inGame2_Predmet_poluchen", 0, 3000);
        }
    }
    else if (type_name == "out")
    {
        if (amount == 1)
        {
            news_caption_name = Globals::Game::translate_string("general_out_item");
            news_text_name =
                Globals::Game::translate_string(Globals::get_system_ini()->r_string(section_name.c_str(), "inv_name"));
        }
        else
        {
            news_caption_name = Globals::Game::translate_string("general_out_item");
            xr_string temporary_name =
                Globals::Game::translate_string(Globals::get_system_ini()->r_string(section_name.c_str(), "inv_name"));
            temporary_name += " x";
            temporary_name += std::to_string(amount).c_str();

            news_text_name = temporary_name;
        }

        if (DataBase::Storage::getInstance().getActor()->IsTalking())
        {
            DataBase::Storage::getInstance().getActor()->AddIconedTalkMessage(
                news_caption_name.c_str(), news_text_name.c_str(), "ui_inGame2_Predmet_otdan", "iconed_answer_item");
        }
        else
        {
            DataBase::Storage::getInstance().getActor()->GiveGameNews(
                news_caption_name.c_str(), news_text_name.c_str(), "ui_inGame2_Predmet_otdan", 0, 3000);
        }
    }
}

} // namespace Scripts
} // namespace Cordis
