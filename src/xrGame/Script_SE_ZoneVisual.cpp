#include "stdafx.h"
#include "Script_SE_ZoneVisual.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_ZoneVisual::Script_SE_ZoneVisual(LPCSTR section_name) : inherited(section_name) {}

Script_SE_ZoneVisual::~Script_SE_ZoneVisual(void) {}

void Script_SE_ZoneVisual::on_register(void)
{
    inherited::on_register();

    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);
}

/*
void Script_SE_ZoneVisual::update(int value)
{
    inherited::update_script(value);

}*/

 

void Script_SE_ZoneVisual::STATE_Read(NET_Packet& packet, std::uint16_t size)
{
    inherited::STATE_Read(packet, size);

    if (FS.IsSDK())
        return;
}

} // namespace Scripts
} // namespace Cordis
