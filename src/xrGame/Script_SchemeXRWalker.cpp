#include "stdafx.h"
#include "Script_SchemeXRWalker.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeXRWalker::Script_SchemeXRWalker(const xr_string& action_name, DataBase::Storage_Scheme& storage)
    : inherited_scheme(nullptr, action_name, storage), m_is_in_camp(false)
{
    this->m_p_storage->setXRWalkerDescriptionName("walker_camp");
    this->m_avail_actions =
        Script_GlobalHelper::getInstance().getAnimpointTable().at(this->m_p_storage->getXRWalkerDescriptionName());

    for (std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>& it : this->m_avail_actions)
    {
        if (it.first(this->m_npc->ID(), false))
        {
            this->m_p_storage->setApprovedActions(it);
        }
    }
}

Script_SchemeXRWalker::~Script_SchemeXRWalker(void) {}

void Script_SchemeXRWalker::initialize(void)
{
    CScriptActionBase::initialize();
    this->m_npc->set_desired_position();
    this->m_npc->set_desired_direction();
    this->reset_scheme(false, this->m_npc);
}

void Script_SchemeXRWalker::execute(void)
{
    CScriptActionBase::execute();

    Script_MoveManager* const p_move_manager = DataBase::Storage::getInstance().getStorage().at(this->m_npc->ID()).getMoveManager();

    if (!p_move_manager)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_move_manager->update();
    // Lord: доделать когда будет sr_camp
}

void Script_SchemeXRWalker::finalize(void)
{
    Script_MoveManager* const p_move_manager = DataBase::Storage::getInstance().getStorage().at(this->m_npc->ID()).getMoveManager();

    if (!p_move_manager)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    if (this->m_is_in_camp)
    {
        // Lord: доделать когда будет camp (смотри скрипт)
    }

    CScriptActionBase::finalize();
}

void Script_SchemeXRWalker::activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object)
{
    this->m_p_storage->ClearSignals();
    this->reset_scheme(is_loading, p_client_object);
}

void Script_SchemeXRWalker::update(const float delta) 
{ 
    return; // Lord: доделать когда будет camp! 
}

} // namespace Scripts
} // namespace Cordis
