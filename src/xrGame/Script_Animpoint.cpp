#include "stdafx.h"
#include "Script_Animpoint.h"

#include "Script_GlobalHelper.h"

namespace Cordis
{
namespace Scripts
{
Script_Animpoint::Script_Animpoint(CScriptGameObject* npc, DataBase::StorageAnimpoint_Data& storage)
    : m_npc_id(npc->ID()), m_storage(&storage)
{
}

Script_Animpoint::~Script_Animpoint(void)
{
    Msg("[Scripts/Script_Animpoint/~dtor] Deleting: [%s]", this->m_cover_name.c_str());
}

bool Script_Animpoint::is_position_riched(void) { return false; }

void Script_Animpoint::start(void) {}

void Script_Animpoint::stop(void) {}

void Script_Animpoint::update(void) {}

void Script_Animpoint::activate_scheme(void)
{
    this->calculate_position();

    if (this->m_is_started)
    {
        if (!this->m_storage->IsUseCamp() && this->m_cover_name == this->m_storage->getCoverName())
        {
            this->fill_approved_actions();

            const xr_string& target_action_name = this->m_storage->getApprovedActions()
                                                      .at(Globals::Script_RandomInt::getInstance().Generate(
                                                          0, this->m_storage->getApprovedActions().size()))
                                                      .second;

            const xr_string& current_state_animstate_name =
                Script_GlobalHelper::getInstance().getStateLibrary().at(target_action_name).getAnimStateTypeName();
            const xr_string& target_state_animstate_name = Script_GlobalHelper::getInstance()
                                                               .getStateLibrary()
                                                               .at(this->m_current_action_name)
                                                               .getAnimStateTypeName();

            if (current_state_animstate_name == target_state_animstate_name)
            {
                Msg("[Script_Animpoint/activate_scheme()] No animpoint stop");

                if (target_action_name != this->m_current_action_name)
                {
                    this->m_current_action_name = this->m_storage->getApprovedActions().at(Globals::Script_RandomInt::getInstance().Generate(0, this->m_storage->getApprovedActions().size())).second;
                    return;
                }
            }

            this->stop();
        }
    }
}

void Script_Animpoint::fill_approved_actions(void)
{
    bool is_in_camp = this->m_is_camp;

    if (this->m_storage->getAvailAnimations().size())
    {
        for (const xr_string& it : this->m_storage->getAvailAnimations())
        {
            this->m_storage->setApprovedAction(Globals::predicate_const_true, it);
        }
    }
    else
    {
        if (this->m_avail_actions.size())
        {
            for (std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>& it : this->m_avail_actions)
            {
                if (it.first(this->m_npc_id, is_in_camp))
                {
                    this->m_storage->setApprovedAction(
                        nullptr, it.second); // Lord: проследить где вызывается метод если он в итоге вызывается, а то
                                             // может вылететь из-за nullptr, хотя так и должны устаналивать!
                }
            }
        }
    }

    if (!this->m_storage->getApprovedActions().size())
    {
        Msg("[Script_Animpoint/fill_approved_actions()] ERROR: There is no approved actions for stalker[%s] in "
            "animpoint[%s]",
            DataBase::Storage::getInstance().getStorage().at(this->m_npc_id).getClientObject()->Name());
        R_ASSERT(false);
    }
}

void Script_Animpoint::calculate_position(void)
{
    Script_SE_SmartCover* server_smartcover =
        Script_GlobalHelper::getInstance().getGameRegisteredServerSmartCovers().at(this->m_cover_name);
}

} // namespace Scripts
} // namespace Cordis
