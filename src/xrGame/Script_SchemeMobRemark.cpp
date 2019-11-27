#include "stdafx.h"
#include "Script_SchemeMobRemark.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeMobRemark::Script_SchemeMobRemark(CScriptGameObject* p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_is_tip_sent(false), m_is_action_end_signalled(false)
{
    this->m_scheme_name = "mob_remark";
}

Script_SchemeMobRemark::~Script_SchemeMobRemark(void) {}

void Script_SchemeMobRemark::reset_scheme(void)
{
    Msg("[Scripts/Script_SchemeMobRemark/reset_scheme()] called for %s", this->m_npc->Name());

    Script_MobStateManager::getInstance().set_state(this->m_npc, this->m_storage->getStateName());

    this->m_npc->DisableTalk();

    XR_LOGIC::mob_capture(this->m_npc, !this->m_storage->IsNoReset(), this->m_scheme_name);

    xr_vector<xr_string> sounds_name;
    xr_vector<xr_string> times_name;
    xr_vector<xr_string> animations_name;
    xr_string picked_sound_name;
    float picked_time = 0.0f;

    animations_name = Globals::Utils::parse_names(this->m_storage->getAnimationName());

    if (!this->m_storage->getSoundName().empty())
    {
        sounds_name = Globals::Utils::parse_names(this->m_storage->getSoundName());
    }

    if (!this->m_storage->getTimeName().empty())
    {
        times_name = Globals::Utils::parse_names(this->m_storage->getTimeName());
    }

    {
        std::uint32_t it_num = 0;
        CScriptActionCondition condition;
        for (const xr_string& it : animations_name)
        {
            picked_sound_name = (it_num < sounds_name.size()) ? sounds_name[it_num] : xr_string();
            picked_time = (it_num < times_name.size()) ? static_cast<float>(atof(times_name[it_num].c_str())) : 0.0f;

            if ((!picked_sound_name.empty()) && (!it.empty()))
            {
                Msg("[Scripts/Script_SchemeMobRemark/reset_scheme()] NOT REACHED CODE! NOT EXISTED CODE! SEE ORIGINAL "
                    "SCRIPT mob_remark.script");
            }
            else if (!it.empty())
            {
                if (fis_zero(picked_time))
                {
                    condition = CScriptActionCondition(CScriptActionCondition::ANIMATION_FLAG);
                }
                else
                {
                    condition = CScriptActionCondition(CScriptActionCondition::TIME_FLAG, picked_time);
                }

                if (this->m_storage->IsAnimationMovement())
                {
                    const Fvector& position = this->m_npc->Position();
                    const Fvector& direction = this->m_npc->Direction();

                    Msg("[Scripts/Script_SchemeMobRemark/reset_scheme()] snork position[%f %f %f] direction[%f %f %f]",
                        position.x, position.y, position.z, direction.x, direction.y, direction.z);

                    Globals::action(this->m_npc, CScriptAnimationAction(it.c_str(), true), condition);
                }
                else
                {
                    Globals::action(this->m_npc, CScriptAnimationAction(it.c_str()), condition);
                }
            }

            ++it_num;
        }
    }

    this->m_is_tip_sent = false;
    this->m_storage->ClearSignals();
    this->m_is_action_end_signalled = false;
}

void Script_SchemeMobRemark::update(const float delta)
{
    if (!this->m_storage->getDialogCondlist().empty())
    {
        if (!XR_LOGIC::pick_section_from_condlist(
                DataBase::Storage::getInstance().getActor(), this->m_npc, this->m_storage->getDialogCondlist())
                 .empty())
        {
            if (!this->m_npc->IsTalkEnabled())
                this->m_npc->EnableTalk();
        }
    }
    else
    {
        if (this->m_npc->IsTalkEnabled())
            this->m_npc->DisableTalk();
    }

    if (!this->m_is_tip_sent)
    {
        this->m_is_tip_sent = true;
        if (!this->m_storage->getTipName().empty())
        {
            // Lord: доделать когда будет news_manager, можешь посмотреть оригинальный скрипт
        }
    }

    if (this->m_npc->GetScriptControl() && !this->m_npc->GetCurrentAction())
    {
        if (!this->m_is_action_end_signalled)
        {
            this->m_is_action_end_signalled = true;
            this->m_storage->setSignals("action_end", true);
            Msg("[Scripts/Script_SchemeMobRemark/update(delta)] signalling action_end!");
        }
    }
}

} // namespace Scripts
} // namespace Cordis
