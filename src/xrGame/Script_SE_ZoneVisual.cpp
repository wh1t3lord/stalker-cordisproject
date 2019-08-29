#include "stdafx.h"
#include "Script_SE_ZoneVisual.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_ZoneVisual::Script_SE_ZoneVisual(LPCSTR section_name) : inherited(section_name) {}

Script_SE_ZoneVisual::~Script_SE_ZoneVisual(void) {}

void Script_SE_ZoneVisual::on_register(void)
{
    inherited::on_register();

    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);

    this->m_artefact_spawn_idle = 60.0f * 60.0f * 1000.0f *
        (Globals::Utils::cfg_get_number(Globals::get_system_ini(), this->name(), "artefact_spawn_idle", this) ?
                Globals::Utils::cfg_get_number(pSettings, this->name(), "artefact_spawn_idle", this) :
                24.0f);
    this->m_artefact_spawn_random =
        Globals::Utils::cfg_get_number(Globals::get_system_ini(), this->name(), "artefact_spawn_rnd") ?
        Globals::Utils::cfg_get_number(Globals::get_system_ini(), this->name(), "artefact_spawn_rnd") :
        100.0f;
}

/*
void Script_SE_ZoneVisual::update(int value)
{
    inherited::update_script(value);

}*/

 

void Script_SE_ZoneVisual::STATE_Read(NET_Packet& packet, std::uint16_t size)
{
    if (FS.IsSDK())
        return;
}

} // namespace Scripts
} // namespace Cordis
