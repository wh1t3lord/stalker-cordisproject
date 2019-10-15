#include "stdafx.h"
#include "Script_SE_SmartCover.h"
#include "Script_StoryObject.h"
namespace Cordis
{
namespace Scripts
{
Script_SE_SmartCover::Script_SE_SmartCover(LPCSTR section_name) : inherited(section_name), m_lastdescription_name("") {}
Script_SE_SmartCover::~Script_SE_SmartCover(void) {}
void Script_SE_SmartCover::STATE_Write(NET_Packet& Packet)
{
    inherited::STATE_Write(Packet);
    Packet.w_stringZ(this->m_lastdescription_name.c_str());
    Packet.w_u8(this->m_loopholes.size());

    for (const std::pair<xr_string, bool>& it : this->m_loopholes)
    {
        Packet.w_stringZ(it.first.c_str());
        Packet.w_u8(it.second ? 1 : 0);
    }
}

void Script_SE_SmartCover::STATE_Read(NET_Packet& Packet, std::uint16_t size)
{
    inherited::STATE_Read(Packet, size);

    // Lord: подумать зачем скрипт вержин и сделать по красивее и грамотнее
    if (this->m_script_version >= 9)
    {
        Packet.r_stringZ(this->m_lastdescription_name);

        xr_string smart_cover_description_name = "";
        if (this->m_lastdescription_name.size())
            smart_cover_description_name = this->m_lastdescription_name;
        else
            smart_cover_description_name = this->description();

        xr_map<xr_string, bool> existing_loopholes;

        if (smart_cover_description_name.size())
        {
            Msg("[Scripts/Script_SE_SmartCover/STATE_Read(Packet, size)] name %s description %s", this->name_replace(),
                smart_cover_description_name.c_str());

            const xr_vector<SmartCoverLoopholeData>& loopholes = Script_GlobalHelper::getInstance()
                                                                     .getRegisteredSmartCovers()
                                                                     .at(smart_cover_description_name)
                                                                     .getLoopholes();

            if (loopholes.size())
            {
                for (const SmartCoverLoopholeData& it : loopholes)
                {
                    if (!it.m_id.size())
                    {
                        R_ASSERT2(false, "You are not registered a new smart cover!!!!!!");
                        return;
                    }

                    existing_loopholes[it.m_id] = true;
                }
            }
            else
            {
                Msg("[Scripts/Script_SE_SmartCover/STATE_Read(packet, size)] WARNING: you are not initialize in "
                    "Script_GlobalHelper your smart_cover! [%s]",
                    smart_cover_description_name.c_str());

                return;
            }
        }

        std::uint8_t it = Packet.r_u8();

        for (std::uint8_t i = 0; i < it; ++i)
        {
            xr_string loophole_id;
            Packet.r_stringZ(loophole_id);
            bool is_loophole_exist = Packet.r_u8();
            if (existing_loopholes[loophole_id])
                this->m_loopholes[loophole_id] = is_loophole_exist;
        }
    }
    else
    {
        xr_string smart_cover_description_name = this->description();

        if (smart_cover_description_name.size())
        {
            const xr_vector<SmartCoverLoopholeData>& loopholes = Script_GlobalHelper::getInstance()
                                                                     .getRegisteredSmartCovers()
                                                                     .at(smart_cover_description_name)
                                                                     .getLoopholes();
            for (const SmartCoverLoopholeData& it : loopholes)
                this->m_loopholes[it.m_id] = true;

            this->m_lastdescription_name = smart_cover_description_name;
        }
    }
}

void Script_SE_SmartCover::on_before_register(void)
{
    inherited::on_before_register();
    Msg("[Scripts/Script_SE_SmartCover/on_before_register()] Registering Script_SE_SmartCover [%s] entity to "
        "GameRegisteredServerSmartCovers",
        this->name_replace());
    Script_GlobalHelper::getInstance().setGameRegisteredServerSmartCovers(this->name_replace(), this);
}

void Script_SE_SmartCover::on_register(void)
{
    inherited::on_register();
    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);
    const std::uint8_t& level_id = GEnv.AISpace->game_graph().vertex(this->m_tGraphID)->level_id();

    Msg("[Scripts/Script_SE_SmartCover/on_register()] Registering Script_SE_SmartCover [%s] entity to "
        "GameRegisteredServerSmartCoversByLevelID",
        this->name());
    Script_GlobalHelper::getInstance().setGameRegisteredServerSmartCoversByLevelID(level_id, this->m_tNodeID, this);
}

void Script_SE_SmartCover::on_unregister(void)
{
    Msg("[Scripts/Script_SE_SmartCover/on_unregister()] Unregistering Script_SE_SmartCover [%s] entity!", this->name());
    Script_StoryObject::getInstance().unregistrate_by_id(this->ID);
    const std::uint8_t& level_id = GEnv.AISpace->game_graph().vertex(this->m_tGraphID)->level_id();
    // @ Lord: здесь наверное просто зануление а не удаление сущности из памяти!
    Script_GlobalHelper::getInstance().setGameRegisteredServerSmartCoversByLevelID(level_id, this->m_tNodeID, nullptr);
    inherited::on_unregister();
}

} // namespace Scripts
} // namespace Cordis
