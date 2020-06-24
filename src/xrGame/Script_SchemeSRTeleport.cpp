#include "stdafx.h"
#include "Script_SchemeSRTeleport.h"

const bool teleport_idle = false;
const bool teleport_activated = true;

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRTeleport::Script_SchemeSRTeleport(
    CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_SRTeleport* storage)
    : inherited_scheme(p_client_object, storage), m_state(false), m_timer(0), m_p_storage(storage)
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
        if (Globals::get_time_global() - this->m_timer >= this->m_p_storage->getTimeout())
        {
            std::uint32_t max_random = 0;
            for (const std::pair<std::uint32_t, std::pair<xr_string, xr_string>>& it :
                this->m_p_storage->getPoints())
            {
                max_random += it.first;
            }

            std::uint32_t generated_value =
                Globals::Script_RandomInt::getInstance().Generate(std::uint32_t(0), max_random);

            for (const std::pair<std::uint32_t, std::pair<xr_string, xr_string>>& it :
                this->m_p_storage->getPoints())
            {
                generated_value -= it.first;
                if (generated_value <= 0)
                {
                    MESSAGE("teleporting to %s look %s",
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

    if (XR_LOGIC::try_switch_to_another_section(this->m_npc, this->m_p_storage, DataBase::Storage::getInstance().getActor()))
        return;
}

void Script_SchemeSRTeleport::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Script_ComponentScheme_SRTeleport* p_storage =
        XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_SRTeleport>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));

    std::uint32_t timeout = static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, "timeout"));

    if (!timeout)
        timeout = 900;

    p_storage->setTimeout(timeout);

    xr_vector<std::pair<std::uint32_t, std::pair<xr_string, xr_string>>> points;
    std::pair<std::uint32_t, std::pair<xr_string, xr_string>> data;
    xr_string point_name;
    xr_string look_name;
    xr_string probability_name;
    for (std::uint32_t i = 1; i < 11; ++i)
    {
        point_name = "point";
        point_name += std::to_string(i);
        look_name = "look";
        look_name += std::to_string(i);
        probability_name = "prob";
        probability_name += std::to_string(i);

        data.second.first = Globals::Utils::cfg_get_string(p_ini, section_name, point_name);
        data.second.second = Globals::Utils::cfg_get_string(p_ini, section_name, look_name);
        data.first = static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, probability_name));

        if (data.second.first.empty() || data.second.second.empty())
            break;

        points.push_back(data);
    }

    p_storage->setPoints(points);

    if (points.empty())
    {
        R_ASSERT2(false, "Wrong point count in sr_teleport!");
    }
}

} // namespace Scripts
} // namespace Cordis
