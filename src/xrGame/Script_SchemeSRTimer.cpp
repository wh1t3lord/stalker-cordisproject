#include "stdafx.h"
#include "Script_SchemeSRTimer.h"

xr_string time_to_string(const std::uint32_t value)
{
    xr_string result = std::to_string(value).c_str();
    if (value >= 10)
        return result;
    else
        return (xr_string("0") + result);
}

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRTimer::Script_SchemeSRTimer(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
    this->m_scheme_name = "sr_timer";
}

Script_SchemeSRTimer::~Script_SchemeSRTimer(void) {}

void Script_SchemeSRTimer::update(const float delta)
{
    if (XR_LOGIC::try_switch_to_another_section(
            this->m_npc, *this->m_p_storage, DataBase::Storage::getInstance().getActor()))
    {
        return;
    }

    std::uint32_t nn =
        Globals::get_time_global() - DataBase::Storage::getInstance().getStorage().at(this->m_id).getActivationTime();
    std::uint32_t value_time = 0;

    if (this->m_p_storage->getSRTimerTypeName() == "inc")
        value_time = this->m_p_storage->getSRTimerStartValue() + nn;
    else
        value_time = this->m_p_storage->getSRTimerStartValue() - nn;

    if (value_time <= 0)
        value_time = 0;

    std::uint32_t hours = floor(value_time / 3600000);
    std::uint32_t minutes = floor((value_time / 60000) - (hours * 60));
    std::uint32_t seconds = floor((value_time / 1000) - (hours * 3600) - (minutes * 60));

    xr_string output_string = std::to_string(hours)
                                  .append(":")
                                  .append(time_to_string(minutes))
                                  .append(":")
                                  .append(time_to_string(seconds))
                                  .c_str();

    this->m_p_storage->getSRTimerTimer()->TextItemControl()->SetTextST(output_string.c_str());

    for (const std::pair<std::uint32_t, xr_map<std::uint32_t, CondlistData>>& it : this->m_p_storage->getSRTimerOnValue())
    {
        if ((this->m_p_storage->getSRTimerTypeName() == "dec" && value_time <= it.first) ||
            (this->m_p_storage->getSRTimerTypeName() == "inc" && value_time >= it.first))
        {
            XR_LOGIC::switch_to_section(this->m_npc, this->m_p_storage->getIni(),
                XR_LOGIC::pick_section_from_condlist(
                    DataBase::Storage::getInstance().getActor(), this->m_npc, it.second));
        }
    }
}

void Script_SchemeSRTimer::deactivate(CScriptGameObject* const p_client_object)
{
    if (!this->m_p_storage->getSRTimerUI())
    {
        Msg("[Scripts/Script_SchemeSRTimer/deactivate(p_client_object)] WARNING: ui field is nullptr! Return ...");
        return;
    }

    this->m_p_storage->getSRTimerUI()->RemoveCustomStatic(this->m_p_storage->getSRTimerTimerIDName().c_str());
    if (!this->m_p_storage->getSRTimerStringName().empty())
    {
        this->m_p_storage->getSRTimerUI()->RemoveCustomStatic(this->m_p_storage->getSRTimerStringName().c_str());
    }
}

} // namespace Scripts
} // namespace Cordis
