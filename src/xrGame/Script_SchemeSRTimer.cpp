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
Script_SchemeSRTimer::Script_SchemeSRTimer(CScriptGameObject* const p_client_object, void* storage)
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

    for (const std::pair<std::uint32_t, xr_map<std::uint32_t, CondlistData>>& it :
        this->m_p_storage->getSRTimerOnValue())
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

void Script_SchemeSRTimer::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Storage_Scheme* p_storage =
        XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
    xr_string timer_type_name = Globals::Utils::cfg_get_string(p_ini, section_name, "type");

    if (timer_type_name.empty())
        timer_type_name = "inc";

    if (timer_type_name != "inc" || timer_type_name != "dec")
    {
        R_ASSERT2(false, "please initialize your timer in proper way");
        return;
    }

    p_storage->setSRTimerTypeName(timer_type_name);

    p_storage->setSRTimerStartValue(
        static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, "start_value")));
    p_storage->setSRTimerOnValue(Globals::Utils::parse_data_1v(
        p_client_object, Globals::Utils::cfg_get_string(p_ini, section_name, "on_value")));

    xr_string timer_id_name = Globals::Utils::cfg_get_string(p_ini, section_name, "timer_id");

    if (timer_id_name.empty())
        timer_id_name = "hud_timer";

    p_storage->setSRTimerTimerIDName(timer_id_name);

    p_storage->setSRTimerStringName(Globals::Utils::cfg_get_string(p_ini, section_name, "string"));
    p_storage->setSRTimerUI(CurrentGameUI());
    p_storage->getSRTimerUI()->AddCustomStatic(p_storage->getSRTimerTimerIDName().c_str(), true);
    p_storage->setSRTimerTimer(p_storage->getSRTimerUI()->GetCustomStatic(p_storage->getSRTimerTimerIDName().c_str())->wnd());

    if (!p_storage->getSRTimerStringName().empty())
    {
        p_storage->getSRTimerUI()->AddCustomStatic("hud_timer_text", true);

        CUIStatic* p_timer_text_ui = p_storage->getSRTimerUI()->GetCustomStatic("hud_timer_text")->wnd();

        if (p_timer_text_ui)
            p_timer_text_ui->TextItemControl()->SetTextST(p_storage->getSRTimerStringName().c_str());
    }
}

} // namespace Scripts
} // namespace Cordis
