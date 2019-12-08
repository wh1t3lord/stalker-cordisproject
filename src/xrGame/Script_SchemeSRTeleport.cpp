#include "stdafx.h"
#include "Script_SchemeSRTeleport.h"

const bool teleport_idle = false;
const bool teleport_activated = true;

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRTeleport::Script_SchemeSRTeleport(
    CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_state(false), m_timer(0)
{
    this->m_scheme_name = "sr_teleport";
}

Script_SchemeSRTeleport::~Script_SchemeSRTeleport(void) {}

void Script_SchemeSRTeleport::update(const float delta)
{
    if (!DataBase::Storage::getInstance().getActor())
    {
        return;
    }

    if (this->m_state == teleport_idle)
    {
        if (this->m_npc->inside(DataBase::Storage::getInstance().getActor()->Position()))
        {
            this->m_state = teleport_activated;
            this->m_timer = Globals::get_time_global();
            Globals::Game::level::add_pp_effector("teleport.ppe", 2006, false);
        }
    }
    else
    {
        if (Globals::get_time_global() - this->m_timer >= this->m_p_storage->getSRTeleportTimeout())
        {
            std::uint32_t max_random = 0;
            for (const std::pair<std::uint32_t, std::pair<xr_string, xr_string>>& it :
                this->m_p_storage->getSRTeleportPoints())
            {
                max_random += it.first;
            }

            std::uint32_t generated_value =
                Globals::Script_RandomInt::getInstance().Generate(std::uint32_t(0), max_random);

            for (const std::pair<std::uint32_t, std::pair<xr_string, xr_string>>& it :
                this->m_p_storage->getSRTeleportPoints())
            {
                generated_value -= it.first;
                if (generated_value <= 0)
                {
                    Msg("[Scripts/Script_SchemeSRTeleport/update(delta)] teleporting to %s look %s",
                        it.second.first.c_str(), it.second.second.c_str());
                    DataBase::Storage::getInstance().getActor()->SetActorPosition(
                        CPatrolPathParams(it.second.first.c_str()).point(std::uint32_t(0)));
                    Fvector direction = Fvector(CPatrolPathParams(it.second.second.c_str()).point(std::uint32_t(0)))
                                            .sub(CPatrolPathParams(it.second.first.c_str()).point(std::uint32_t(0)));
                    DataBase::Storage::getInstance().getActor()->SetActorDirection(-direction.getH());
                    CScriptSound object = CScriptSound("affects\\tinnitus3a");
                    object.PlayNoFeedback(DataBase::Storage::getInstance().getActor(), sm_2D, 0.0f, Fvector(), 1.0f);
                    break;
                }
            }

            this->m_state = teleport_idle;
        }
        else
        {
            return;
        }
    }

    if (XR_LOGIC::try_switch_to_another_section(
            this->m_npc, *this->m_p_storage, DataBase::Storage::getInstance().getActor()))
        return;
}

} // namespace Scripts
} // namespace Cordis
