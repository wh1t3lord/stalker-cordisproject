#include "stdafx.h"
#include "Script_SE_Artefact.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_Artefact::Script_SE_Artefact(LPCSTR section_name) : inherited(section_name) 
{ Msg("[Scripts/Script_SE_Artefact/ctor(section_name)] %s", section_name); }

Script_SE_Artefact::~Script_SE_Artefact(void)
{

}

void Script_SE_Artefact::on_register(void) 
{
    inherited::on_register();
    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);
}

void Script_SE_Artefact::on_unregister(void) 
{
    Script_StoryObject::getInstance().unregistrate_by_id(this->ID);
    inherited::on_unregister();
}

bool Script_SE_Artefact::can_switch_offline(void) const 
{ 
    if (ai().alife().graph().actor() &&
        ai().alife().graph().actor()->Position().distance_to(this->o_Position) <= 150.0f)
        return false;

    return inherited::can_switch_offline();
}

bool Script_SE_Artefact::can_switch_online(void) const 
{
    return inherited::can_switch_online(); 
}

} // namespace Scripts
} // namespace Cordis
