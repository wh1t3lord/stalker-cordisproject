#include "stdafx.h"
#include "Script_SE_Helicopter.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_Helicopter::Script_SE_Helicopter(LPCSTR section_name) : inherited(section_name) { Msg("[Scripts/Script_SE_Helicopter/ctor(section_name)] %s", section_name); }

Script_SE_Helicopter::~Script_SE_Helicopter(void) {}

void Script_SE_Helicopter::on_register(void) 
{
    inherited::on_register();
    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);
}

void Script_SE_Helicopter::on_unregister(void) 
{
    Script_StoryObject::getInstance().unregistrate_by_id(this->ID);
    inherited::on_unregister();
}

bool Script_SE_Helicopter::keep_saved_data_anyway(void) { return true; }
} // namespace Scripts
} // namespace Cordis
