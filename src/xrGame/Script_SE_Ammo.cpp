#include "stdafx.h"
#include "Script_SE_Ammo.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_Ammo::Script_SE_Ammo(LPCSTR section_name) : inherited(section_name), m_is_secret_item(false) { Msg("[Scripts/Script_SE_Ammo/ctor(section_name)] %s", section_name); }

Script_SE_Ammo::~Script_SE_Ammo(void) {}

void Script_SE_Ammo::on_register(void)
{
    inherited::on_register();
    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);

    this->m_is_secret_item = Script_TreasureManager::getInstance().register_item(this);
}

void Script_SE_Ammo::on_unregister(void)
{
    Script_StoryObject::getInstance().unregistrate_by_id(this->ID);
    inherited::on_unregister();
}

bool Script_SE_Ammo::can_switch_online(void) const
{
    if (this->m_is_secret_item)
        return false;

    return inherited::can_switch_online();
}
} // namespace Scripts
} // namespace Cordis
