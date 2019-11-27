#include "stdafx.h"
#include "Script_SchemeMobJump.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeMobJump::Script_SchemeMobJump(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
    this->m_scheme_name = "mob_jump";
}
Script_SchemeMobJump::~Script_SchemeMobJump(void) {}

void Script_SchemeMobJump::reset_scheme(void)
{
    Msg("[Scripts/Script_SchemeMobJump/reset_scheme()] %s", this->m_npc->Name());
    this->m_storage->ClearSignals();

    if (!this->m_storage->getPathJumpName().empty())
    {
        this->m_storage->setJumpPath(new CPatrolPathParams(this->m_storage->getPathJumpName().c_str()));
    }
    else
    {
        this->m_storage->setPathJumpName("[not defined]");
    }

    if (!this->m_storage->getJumpPath())
    {
        R_ASSERT2(false, "you must defined jump_path_name!");
    }

    this->m_point =
        Fvector().set(0.0f, 0.0f, 0.0f).add(this->m_storage->getJumpPath()->point(0), this->m_storage->getOffset());
    this->m_current_state = STATE_START_LOOK;
}

void Script_SchemeMobJump::update(const float delta)
{
    switch (this->m_current_state)
    {
    case STATE_START_LOOK:
    {
        if (!this->m_npc->GetCurrentAction())
        {
            Globals::action(this->m_npc, CScriptWatchAction(SightManager::eSightTypePosition, this->m_point),
                CScriptActionCondition(CScriptActionCondition::WATCH_FLAG));

            this->m_current_state = STATE_WAIT_LOOK_END;
        }

        break;
    }
    case STATE_JUMP:
    {
        this->m_npc->jump(this->m_point, this->m_storage->getPHJumpFactor());
        this->m_storage->setSignals("jumped", true);
        XR_LOGIC::mob_release(this->m_npc, this->m_scheme_name);
        break;
    }
    case STATE_WAIT_LOOK_END:
    {
        if (!this->m_npc->GetCurrentAction())
            this->m_current_state = STATE_JUMP;

        break;
    }
    }
}

} // namespace Scripts
} // namespace Cordis