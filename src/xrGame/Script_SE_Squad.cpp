#include "stdafx.h"
#include "Script_SE_Squad.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_Squad::Script_SE_Squad(LPCSTR section_name) : inherited(section_name) {}

Script_SE_Squad::~Script_SE_Squad(void) {}

void Script_SE_Squad::on_register(void) 
{
    inherited::on_register(); 
    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);
    Script_SimulationObjects::getInstance().registrate(this);
}

void Script_SE_Squad::on_unregister(void) 
{
    Script_StoryObject::getInstance().unregistrate_by_id(this->ID); 
    inherited::on_unregister();
    Script_SimulationObjects::getInstance().unregistrate(this);
}

} // namespace Scripts
} // namespace Cordis
