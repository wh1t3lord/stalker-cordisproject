#include "stdafx.h"
#include "Script_SE_Outfit.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_Outfit::Script_SE_Outfit(LPCSTR section_name) : inherited(section_name), m_is_secret_item(false) { }

Script_SE_Outfit::~Script_SE_Outfit(void) {}

void Script_SE_Outfit::on_register(void) 
{
    inherited::on_register();
    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);

  //  this->m_is_secret_item = 
}

void Script_SE_Outfit::on_unregister(void) {}

bool Script_SE_Outfit::can_switch_online() const { return false; }

} // namespace Scripts
} // namespace Cordis
