#include "stdafx.h"
#include "Script_SchemeMobRemark.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeMobRemark::Script_SchemeMobRemark(CScriptGameObject* p_client_object, DataBase::Script_ComponentScheme_MobRemark* storage)
    : inherited_scheme(p_client_object, storage), m_is_tip_sent(false), m_is_action_end_signalled(false)
{
    this->m_scheme_name = "mob_remark";
}

Script_SchemeMobRemark::~Script_SchemeMobRemark(void) {}

void Script_SchemeMobRemark::reset_scheme(const bool, CScriptGameObject* const p_client_object)
{
    MESSAGE("called for %s", this->m_npc->Name());

    Script_MobStateManager::getInstance().set_state(this->m_npc, this->m_p_storage->getStateName());

    this->m_npc->DisableTalk();

    XR_LOGIC::mob_capture(this->m_npc, !this->m_p_storage->isNoReset(), this->m_scheme_name);

    xr_vector<xr_string> sounds_name;
    xr_vector<xr_string> times_name;
    xr_vector<xr_string> animations_name;
    xr_string picked_sound_name;
    float picked_time = 0.0f;

    animations_name = Globals::Utils::parse_names(this->m_p_storage->getAnimationName());

    if (!this->m_p_storage->getSoundName().empty())
    {
        sounds_name = Globals::Utils::parse_names(this->m_p_storage->getSoundName());
    }

    if (!this->m_p_storage->getTimeName().empty())
    {
        times_name = Globals::Utils::parse_names(this->m_p_storage->getTimeName());
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
                R_ASSERT2(false, "NOT REACHED CODE! NOT EXISTED CODE! SEE ORIGINAL "
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

                if (this->m_p_storage->isAnimationMovement())
                {
                    const Fvector& position = this->m_npc->Position();
                    const Fvector& direction = this->m_npc->Direction();

                    Msg("snork position[%f %f %f] direction[%f %f %f]",
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
    this->m_p_storage->ClearSignals();
    this->m_is_action_end_signalled = false;
}

void Script_SchemeMobRemark::update(const float delta)
{
    if (!this->m_p_storage->getDialogCondlist().empty())
    {
        if (XR_LOGIC::pick_section_from_condlist(
                DataBase::Storage::getInstance().getActor(), this->m_npc, this->m_p_storage->getDialogCondlist())
                 .empty() == false)
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
        if (!this->m_p_storage->getTipName().empty())
        {
            // Lord: доделать когда будет news_manager, можешь посмотреть оригинальный скрипт
        }
    }

    if (this->m_npc->GetScriptControl() && !this->m_npc->GetCurrentAction())
    {
        if (!this->m_is_action_end_signalled)
        {
            this->m_is_action_end_signalled = true;
            this->m_p_storage->setSignals("action_end", true);
            MESSAGEI("signalling action_end!");
        }
    }
}

void Script_SchemeMobRemark::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Script_ComponentScheme_MobRemark* p_storage =
        XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_MobRemark>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
    p_storage->setStateName(Script_MobStateManager::getInstance().get_state(p_ini, section_name));
    p_storage->setDialogCondlist(
        XR_LOGIC::cfg_get_condlist(p_ini, section_name, "dialog_cond", p_client_object).getCondlist());
    p_storage->setNoReset(true);
    p_storage->setAnimationName(Globals::Utils::cfg_get_string(p_ini, section_name, "anim"));
    p_storage->setAnimationMovement(Globals::Utils::cfg_get_bool(p_ini, section_name, "anim_movement"));
    p_storage->setAnimationHeadName(Globals::Utils::cfg_get_string(p_ini, section_name, "anim_head"));
    p_storage->setTipName(Globals::Utils::cfg_get_string(p_ini, section_name, "tip"));
    p_storage->setSoundName(Globals::Utils::cfg_get_string(p_ini, section_name, "snd"));
    p_storage->setTimeName(Globals::Utils::cfg_get_string(p_ini, section_name, "time"));
}

} // namespace Scripts
} // namespace Cordis
