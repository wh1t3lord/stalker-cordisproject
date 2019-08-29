#include "stdafx.h"
#include "Script_SE_Actor.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_Actor::Script_SE_Actor(LPCSTR section)
    : inherited(section), m_is_registered(false), m_is_start_position_filled(false)
{
}

Script_SE_Actor::~Script_SE_Actor(void) {}

void Script_SE_Actor::on_register(void)
{
    inherited::on_register();
    Script_StoryObject::getInstance().registrate(this->ID, "actor");
    Script_SimulationObjects::getInstance().registrate(this);

    this->m_is_registered = true;

    if (!this->m_is_start_position_filled)
    {
        Script_SimulationBoard::getInstance().fill_start_position();
    }
}

} // namespace Scripts
} // namespace Cordis
