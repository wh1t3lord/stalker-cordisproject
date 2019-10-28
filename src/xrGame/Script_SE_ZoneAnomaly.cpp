#include "stdafx.h"
#include "Script_SE_ZoneAnomaly.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_ZoneAnomaly::Script_SE_ZoneAnomaly(LPCSTR section_name) : inherited(section_name), m_is_registered(false)
{
    Msg("[Scripts/Script_SE_ZoneAnomaly/ctor(section_name)] %s", section_name);
}

Script_SE_ZoneAnomaly::~Script_SE_ZoneAnomaly(void) {}

void Script_SE_ZoneAnomaly::on_register(void)
{
    inherited::on_register();
    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);

    this->m_is_registered = true;
}

void Script_SE_ZoneAnomaly::STATE_Read(NET_Packet& packet, std::uint16_t size)
{
    inherited::STATE_Read(packet, size);

    if (FS.IsSDK())
        return;
}

} // namespace Scripts
} // namespace Cordis
