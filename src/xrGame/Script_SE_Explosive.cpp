#include "stdafx.h"
#include "Script_SE_Explosive.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_Explosive::Script_SE_Explosive(LPCSTR section_name) : inherited(section_name), m_is_secret_item(false)
{
    Msg("[Scripts/Script_SE_Explosive/ctor(section_name)] %s", section_name);
}

Script_SE_Explosive::~Script_SE_Explosive(void) {}

void Script_SE_Explosive::on_register(void)
{
    inherited::on_register();
    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);

    this->m_is_secret_item = Script_TreasureManager::getInstance().register_item(this);
}

void Script_SE_Explosive::on_unregister(void) 
{ 
    Script_StoryObject::getInstance().unregistrate_by_id(this->ID);
    inherited::on_unregister(); 
}

bool Script_SE_Explosive::can_switch_online(void) const \
{
    if (this->m_is_secret_item)
        return false;

    return inherited::can_switch_online(); 
}
} // namespace Scripts
} // namespace Cordis
