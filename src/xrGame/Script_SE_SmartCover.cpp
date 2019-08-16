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
    }
}

void Script_SE_SmartCover::on_before_register(void) {}

void Script_SE_SmartCover::on_register(void) {}
void Script_SE_SmartCover::on_unregister(void) {}
} // namespace Scripts
} // namespace Cordis
