#include "stdafx.h"
#include "Script_SE_SmartCover.h"

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
            Msg("[Scripts/Script_SE_SmartCover/STATE_Read(Packet, size)] name %s descirption %s", this->name_replace(),
                smart_cover_description_name.c_str());

            xr_vector<SmartCoverLoopholeData>& loopholes =
                Script_GlobalHelper::getInstance().getRegisteredSmartCovers()[smart_cover_description_name].m_loopholes;
            if (loopholes[0].m_id.size())
            {
                for (SmartCoverLoopholeData& it : loopholes)
                    existing_loopholes[it.m_id] = true;
            }
            else
            {
                // @ Lord: попробовать сделать warning уведомление чем выводить полноценный Assertion
                R_ASSERT2(false, "You are not registered a new smart cover!!!!!!");
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
            xr_vector<SmartCoverLoopholeData>& loopholes =
                Script_GlobalHelper::getInstance().getRegisteredSmartCovers()[smart_cover_description_name].m_loopholes;
            for (SmartCoverLoopholeData& it : loopholes)
                this->m_loopholes[it.m_id] = true;

            this->m_lastdescription_name = smart_cover_description_name;
        }
    }
}

void Script_SE_SmartCover::on_before_register(void) {}

void Script_SE_SmartCover::on_register(void) {}
void Script_SE_SmartCover::on_unregister(void) {}
} // namespace Scripts
} // namespace Cordis
