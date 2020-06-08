#include "stdafx.h"
#include "Script_SchemeMobJump.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeMobJump::Script_SchemeMobJump(CScriptGameObject* const p_client_object, void* storage)
    : inherited_scheme(p_client_object, storage)
{
    this->m_scheme_name = "mob_jump";
}
Script_SchemeMobJump::~Script_SchemeMobJump(void) {}

void Script_SchemeMobJump::reset_scheme(const bool, CScriptGameObject* const p_client_object)
{
    Msg("[Scripts/Script_SchemeMobJump/reset_scheme()] %s", this->m_npc->Name());
    this->m_p_storage->ClearSignals();

    if (!this->m_p_storage->getPathJumpName().empty())
    {
        this->m_p_storage->setJumpPath(new CPatrolPathParams(this->m_p_storage->getPathJumpName().c_str()));
    }
    else
    {
        this->m_p_storage->setPathJumpName("[not defined]");
    }

    if (!this->m_p_storage->getJumpPath())
    {
        R_ASSERT2(false, "you must defined jump_path_name!");
    }

    this->m_point = Fvector()
                        .set(0.0f, 0.0f, 0.0f)
                        .add(this->m_p_storage->getJumpPath()->point(std::uint32_t(0)), this->m_p_storage->getOffset());
    this->m_current_state = STATE_START_LOOK;
}

void Script_SchemeMobJump::update(const float delta)
{
    switch (this->m_current_state)
    {
    case STATE_START_LOOK: {
        if (!this->m_npc->GetCurrentAction())
        {
            Globals::action(this->m_npc, CScriptWatchAction(SightManager::eSightTypePosition, this->m_point),
                CScriptActionCondition(CScriptActionCondition::WATCH_FLAG));

            this->m_current_state = STATE_WAIT_LOOK_END;
        }

        break;
    }
    case STATE_JUMP: {
        this->m_npc->jump(this->m_point, this->m_p_storage->getPHJumpFactor());
        this->m_p_storage->setSignals("jumped", true);
        XR_LOGIC::mob_release(this->m_npc, this->m_scheme_name);
        break;
    }
    case STATE_WAIT_LOOK_END: {
        if (!this->m_npc->GetCurrentAction())
            this->m_current_state = STATE_JUMP;

        break;
    }
    }
}

void Script_SchemeMobJump::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "it can't be!");
        return;
    }

    DataBase::Storage_Scheme* p_storage =
        XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "it can't be!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
    p_storage->setPathJumpName(Globals::Utils::cfg_get_string(p_ini, section_name, "path_jump"));
    p_storage->setPHJumpFactor(Globals::Utils::cfg_get_number(p_ini, section_name, "ph_jump_factor"));
    xr_vector<xr_string> offset_data = Globals::Utils::parse_names(Globals::Utils::cfg_get_string(p_ini, section_name, "offset"));

    p_storage->setOffset(Fvector().set(atof(offset_data[0].c_str()), atof(offset_data[1].c_str()), atof(offset_data[2].c_str())));

    if (!p_ini->line_exist(section_name.c_str(), "on_signal"))
    {
        R_ASSERT2(false, "must be specified");
        return;
    }
}

} // namespace Scripts
} // namespace Cordis
