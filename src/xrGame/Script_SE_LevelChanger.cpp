#include "stdafx.h"
#include "Script_SE_LevelChanger.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_LevelChanger::Script_SE_LevelChanger(LPCSTR section_name)
    : inherited(section_name), m_hint_name("level_changer_invitation"), m_is_enabled(false)
{
}

Script_SE_LevelChanger::~Script_SE_LevelChanger(void) {}

void Script_SE_LevelChanger::on_register(void)
{
    inherited::on_register();
    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);
}

void Script_SE_LevelChanger::on_unregister(void)
{
    Script_StoryObject::getInstance().unregistrate_by_id(this->ID);
    inherited::on_unregister();
}

void Script_SE_LevelChanger::STATE_Write(NET_Packet& packet)
{
    inherited::STATE_Write(packet);

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, false, "Script_SE_LevelChanger");
    packet.w_u8(this->m_is_enabled ? 1 : 0);
    packet.w_stringZ(this->m_hint_name.c_str());
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, true, "Script_SE_LevelChanger");
}

void Script_SE_LevelChanger::STATE_Read(NET_Packet& packet, const std::uint16_t size)
{
    inherited::STATE_Read(packet, size);

    // Lord: Не используем при редакторе
    if (FS.IsSDK())
        return;
    
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, false, "Script_SE_LevelChanger");
    this->m_is_enabled = !!packet.r_u8();
    packet.r_stringZ(this->m_hint_name);
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, true, "Script_SE_LevelChanger");
}

} // namespace Scripts
} // namespace Cordis
