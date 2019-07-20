#include "stdafx.h"
#include "Script_SE_Actor.h"
#include "Script_StoryObject.h"
namespace Cordis
{
namespace Scripts
{
Script_SE_Actor::Script_SE_Actor(LPCSTR section)
    : inherited(section), m_registred(false), m_start_position_filled(false)
{
}

Script_SE_Actor::~Script_SE_Actor(void) {}

void Script_SE_Actor::on_register(void) 
{ 
    inherited::on_register(); // Lord - [Script] Dodelat!
    Script_StoryObject::getInstance().registrate(this->ID, "actor");

}

} // namespace Scripts
} // namespace Cordis
