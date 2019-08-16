#include "stdafx.h"
#include "Script_SE_SmartCover.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_SmartCover::Script_SE_SmartCover(LPCSTR section_name) : inherited(section_name) {}
Script_SE_SmartCover::~Script_SE_SmartCover(void) {}
void Script_SE_SmartCover::STATE_Write(NET_Packet& Packet) 
{ inherited::STATE_Write(Packet); }

void Script_SE_SmartCover::STATE_Read(NET_Packet& Packet, std::uint16_t size) 
{ inherited::STATE_Read(Packet, size); }

void Script_SE_SmartCover::on_before_register(void) 
{ }

void Script_SE_SmartCover::on_register(void) 
{
    
}
void Script_SE_SmartCover::on_unregister(void) {}
} // namespace Scripts
} // namespace Cordis
