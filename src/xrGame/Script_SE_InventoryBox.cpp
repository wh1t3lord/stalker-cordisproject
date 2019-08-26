#include "stdafx.h"
#include "Script_SE_InventoryBox.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_InventoryBox::Script_SE_InventoryBox(LPCSTR section_name) : inherited(section_name), m_is_secret_item(false) {}

Script_SE_InventoryBox::~Script_SE_InventoryBox(void) {}

void Script_SE_InventoryBox::on_register(void) 
{
    inherited::on_register(); 
    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);

    this->m_is_secret_item = Script_TreasureManager::getInstance().register_item(this);
}

void Script_SE_InventoryBox::on_unregister(void) \
{
    Script_StoryObject::getInstance().unregistrate_by_id(this->ID);
    inherited::on_unregister();
}

bool Script_SE_InventoryBox::can_switch_online(void) const
{ 
    if (this->m_is_secret_item)
        return false;

    return inherited::can_switch_online(); 
}
} // namespace Scripts
} // namespace Cordis
