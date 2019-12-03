#include "stdafx.h"
#include "Script_SchemePHDoor.h"
#include "physics_joint_scripted.h"
#include "physics_element_scripted.h"
#include "PhysicObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemePHDoor::Script_SchemePHDoor(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_p_joint(nullptr), m_is_initialized(false), m_low_limits(0.0f),
      m_high_limits(0.0f), m_is_block(false), m_is_soundless_block(false), m_is_show_tips(false)
{
    this->m_scheme_name = "ph_door";
    this->m_storage->setPHDoorDoorAction(this);
}

Script_SchemePHDoor::~Script_SchemePHDoor(void) { this->m_storage->setPHDoorDoorAction(nullptr); }

void Script_SchemePHDoor::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    this->m_storage->ClearSignals();

    if (!this->m_npc->get_physics_shell())
    {
        Msg("[Scripts/Script_SchemePHDoor/reset_scheme(is_loading, p_client_object)] WARNING: physics_shell == "
            "nullptr! Return ...");
        return;
    }

    this->m_p_joint = this->m_npc->get_physics_shell()->get_Joint("door");
    if (!this->m_p_joint)
    {
        Msg("[Scripts/Script_SchemePHDoor/reset_scheme(is_loading, p_client_object)] WARNING: m_p_joint == nullptr! "
            "Return ...");
        return;
    }

    this->m_p_joint->GetLimits(this->m_low_limits, this->m_high_limits, 0);
    this->m_is_block = false;
    this->m_is_soundless_block = false;
    this->m_is_show_tips = this->m_storage->IsPHDoorShowTips();

    bool is_disable_sound = false;
    if (!this->m_storage->IsPHDoorScriptUsedMoreThanOnce())
    {
        is_disable_sound = true;
        this->m_storage->setPHDoorScriptUsedMoreThanOnce(true);
    }

    if (this->m_storage->IsPHDoorClosed())
    {
        if (this->is_closed())
        {
            is_disable_sound = true;
        }
        this->close_door(is_disable_sound);
    }
    else
    {
        this->open_door(is_disable_sound);
    }

    this->m_npc->SetNonscriptUsable(false);
    this->m_is_initialized = true;
}

void Script_SchemePHDoor::update(const float delta)
{
    if (!this->m_is_initialized)
    {
        R_ASSERT2(false, "can't be!");
    }

    if (XR_LOGIC::try_switch_to_another_section(
            this->m_npc, *this->m_storage, DataBase::Storage::getInstance().getActor()))
        return;
}

void Script_SchemePHDoor::use_callback(CScriptGameObject* const p_client_object, CScriptGameObject* const p_client_who)
{
    if (this->m_storage->IsPHDoorLocked())
    {
        if (!this->m_storage->getPHDoorSoundOpenStartName().empty())
        {
            xr_string faction_name;
            XR_SOUND::set_sound_play(this->m_id, this->m_storage->getPHDoorSoundOpenStartName(), faction_name, 0);
        }
    }

    if (this->try_switch())
        return;
}

void Script_SchemePHDoor::hit_callback(CScriptGameObject* const p_client_object, const float amount,
    const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index)
{
    if (!this->m_storage->getHitOnBone().at(bone_index).empty())
    {
        XR_LOGIC::switch_to_section(p_client_object, this->m_storage->getIni(),
            XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_npc,
                this->m_storage->getHitOnBone().at(bone_index)));
        return;
    }
}

void Script_SchemePHDoor::deactivate(CScriptGameObject* const p_client_object) 
{ this->m_npc->SetTipText(""); }

void Script_SchemePHDoor::close_door(const bool is_disable_sound)
{
    if (!is_disable_sound)
    {
        if (!this->m_storage->getPHDoorSoundCloseStartName().empty())
        {
            xr_string faction_name;
            XR_SOUND::set_sound_play(this->m_id, this->m_storage->getPHDoorSoundCloseStartName(), faction_name, 0);
        }
    }

    this->m_npc->set_fastcall(std::bind(&Script_SchemePHDoor::fastcall, this));

    if (this->m_storage->IsPHDoorNoForce())
    {
        this->m_p_joint->SetForceAndVelocity(0.0f, 0.0f, 0);
    }
    else
    {
        this->m_p_joint->SetForceAndVelocity(200.0f, 3.0f, 0);
    }

    this->m_is_block = true;
    this->m_is_soundless_block = is_disable_sound;

    CPhysicObject* p_physic_object = this->m_npc->get_physics_object();
    if (p_physic_object)
    {
        p_physic_object->set_door_ignore_dynamics();
    }

    if (this->m_is_show_tips)
    {
        if (this->m_storage->IsPHDoorLocked() && !this->m_storage->getPHDoorTipUnlockName().empty())
        {
            this->m_npc->SetTipText(this->m_storage->getPHDoorTipUnlockName().c_str());
            return;
        }

        if (!this->m_storage->getPHDoorTipOpenName().empty())
        {
            this->m_npc->SetTipText(this->m_storage->getPHDoorTipOpenName().c_str());
        }
    }
}

void Script_SchemePHDoor::open_door(const bool is_disable_sound)
{
    if (!is_disable_sound)
    {
        if (!this->m_storage->getPHDoorSoundOpenStartName().empty())
        {
            xr_string faction_name;
            XR_SOUND::set_sound_play(this->m_id, this->m_storage->getPHDoorSoundOpenStartName(), faction_name, 0);
        }
    }

    this->m_npc->set_fastcall(std::bind(&Script_SchemePHDoor::open_fastcall, this));

    cphysics_shell_scripted* p_shell = this->m_npc->get_physics_shell();
    if (p_shell)
    {
        cphysics_element_scripted* p_element = p_shell->get_Element("door");
        if (p_element->isFixed())
        {
            p_element->ReleaseFixed();

            CPhysicObject* p_physic_object = this->m_npc->get_physics_object();

            if (p_physic_object)
                p_physic_object->set_door_ignore_dynamics();
        }
    }

    if (this->m_storage->IsPHDoorNoForce())
    {
        this->m_p_joint->SetForceAndVelocity(0.0f, 0.0, f 0);
    }
    else
    {
        this->m_p_joint->SetForceAndVelocity(2100.0f, -3.0f, 0);
    }

    this->m_is_block = false;

    if (this->m_is_show_tips && !this->m_storage->getPHDoorTipCloseName().empty())
    {
        this->m_npc->SetTipText(this->m_storage->getPHDoorTipCloseName().c_str());
    }
}

void Script_SchemePHDoor::close_action(void)
{
    Msg("[Scripts/Script_SchemePHDoor/close_action()] %d", Globals::get_time_global());

    if (this->m_storage->IsPHDoorNoForce())
    {
        this->m_p_joint->SetForceAndVelocity(0.0f, 0.0f, 0);
    }
    else
    {
        this->m_p_joint->SetForceAndVelocity(10000.0f, 1.0f, 0);

        cphysics_shell_scripted* p_shell = this->m_npc->get_physics_shell();
        if (p_shell)
        {
            cphysics_element_scripted* p_element = p_shell->get_Element("door");
            if (!p_element->isFixed())
            {
                p_element->Fix();
            }
        }
    }

    CPhysicObject* p_physic_object = this->m_npc->get_physics_object();
    p_physic_object->unset_door_ignore_dynamics();
    this->m_is_block = false;

    if (!this->m_is_soundless_block && !this->m_storage->getPHDoorSoundCloseStopName().empty())
    {
        xr_string faction_name;
        XR_SOUND::set_sound_play(this->m_id, this->m_storage->getPHDoorSoundCloseStopName(), faction_name, 0);
    }
}

bool Script_SchemePHDoor::open_fastcall(void)
{
    if (!this->m_is_initialized)
    {
        return false;
    }

    if (this->is_open())
    {
        CPhysicObject* p_shell = this->m_npc->get_physics_object();

        if (p_shell)
        {
            p_shell->unset_door_ignore_dynamics();
        }
        else
        {
            Msg("[Scripts/Script_SchemePHDoor/open_fastcall()] WARNING: CPhysicObject field is nullptr! Something "
                "wrong, check your dynamic part of the door!");
        }

        this->m_npc->on_door_is_open();

        return true;
    }

    return false;
}

bool Script_SchemePHDoor::fastcall(void)
{
    if (!this->m_is_initialized)
        return false;

    if (this->m_is_block && this->is_closed())
    {
        this->close_action();
        this->m_npc->on_door_is_closed();
        return true;
    }

    return false;
}

} // namespace Scripts
} // namespace Cordis
