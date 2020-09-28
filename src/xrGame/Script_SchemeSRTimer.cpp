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
Script_SchemeSRTimer::Script_SchemeSRTimer(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_SRTimer* storage)
    : inherited_scheme(p_client_object, storage)
{
    this->m_scheme_name = "sr_timer";
}

Script_SchemeSRTimer::~Script_SchemeSRTimer(void) {}

void Script_SchemeSRTimer::update(const float delta)
{
    if (XR_LOGIC::try_switch_to_another_section(
            this->m_npc, this->m_p_storage, DataBase::Storage::getInstance().getActor()))
    {
        return;
    }

    std::uint32_t nn =
        Globals::get_time_global() - DataBase::Storage::getInstance().getStorage().at(this->m_id).getActivationTime();
    std::uint32_t value_time = 0;

    if (this->m_p_storage->getTypeName() == "inc")
        value_time = this->m_p_storage->getStartValue() + nn;
    else
        value_time = this->m_p_storage->getStartValue() - nn;

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

    this->m_p_storage->getTimer()->TextItemControl()->SetTextST(output_string.c_str());

    for (const std::pair<std::uint32_t, xr_map<std::uint32_t, CondlistData>>& it :
        this->m_p_storage->getOnValue())
    {
        if ((this->m_p_storage->getTypeName() == "dec" && value_time <= it.first) ||
            (this->m_p_storage->getTypeName() == "inc" && value_time >= it.first))
        {
            XR_LOGIC::switch_to_section(this->m_npc, this->m_p_storage->getIni(),
                XR_LOGIC::pick_section_from_condlist(
                    DataBase::Storage::getInstance().getActor(), this->m_npc, it.second));
        }
    }
}

void Script_SchemeSRTimer::deactivate(CScriptGameObject* const p_client_object)
{
    if (!this->m_p_storage->getUI())
    {
        MESSAGEWR("ui field is nullptr!");
        return;
    }

    this->m_p_storage->getUI()->RemoveCustomStatic(this->m_p_storage->getTimerIDName().c_str());
    if (!this->m_p_storage->getStringName().empty())
    {
        this->m_p_storage->getUI()->RemoveCustomStatic(this->m_p_storage->getStringName().c_str());
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

    DataBase::Script_ComponentScheme_SRTimer* p_storage =
        XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_SRTimer>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

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

    p_storage->setTypeName(timer_type_name);

    p_storage->setStartValue(
        static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, "start_value")));
    p_storage->setOnValue(Globals::Utils::parse_data_1v(
        p_client_object, Globals::Utils::cfg_get_string(p_ini, section_name, "on_value")));

    xr_string timer_id_name = Globals::Utils::cfg_get_string(p_ini, section_name, "timer_id");

    if (timer_id_name.empty())
        timer_id_name = "hud_timer";

    p_storage->setTimerIDName(timer_id_name);

    p_storage->setStringName(Globals::Utils::cfg_get_string(p_ini, section_name, "string"));
    p_storage->setUI(CurrentGameUI());
    p_storage->getUI()->AddCustomStatic(p_storage->getTimerIDName().c_str(), true);
    p_storage->setTimer(p_storage->getUI()->GetCustomStatic(p_storage->getTimerIDName().c_str())->wnd());

    if (!p_storage->getStringName().empty())
    {
        p_storage->getUI()->AddCustomStatic("hud_timer_text", true);

        CUIStatic* p_timer_text_ui = p_storage->getUI()->GetCustomStatic("hud_timer_text")->wnd();

        if (p_timer_text_ui)
            p_timer_text_ui->TextItemControl()->SetTextST(p_storage->getStringName().c_str());
    }
}

} // namespace Scripts
} // namespace Cordis
