#include "stdafx.h"
#include "Script_SE_Restrictor.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_Restrictor::Script_SE_Restrictor(LPCSTR section_name) : inherited(section_name) 
{
    Msg("[Scripts/Script_SE_Restrictor/ctor(section_name)] %s", section_name); 
}

Script_SE_Restrictor::~Script_SE_Restrictor(void) {}

void Script_SE_Restrictor::on_register(void) 
{
    inherited::on_register(); 
    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);

    Script_TreasureManager::getInstance().register_restrictor(this);
}

bool Script_SE_Restrictor::keep_saved_data_anyway(void) { return true; }
} // namespace Scripts
} // namespace Cordis
