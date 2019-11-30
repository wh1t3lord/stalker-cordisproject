#include "stdafx.h"
#include "Script_SchemeHelicopterMove.h"

 

namespace Cordis
{
namespace Scripts
{
Script_SchemeHelicopterMove::Script_SchemeHelicopterMove(
    CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_p_helicopter(p_client_object->get_helicopter()),
      m_p_manager_fire(Script_HelicopterFireStorage::getInstance().AllocateFirer(p_client_object)),
      m_p_manager_fly(Script_HelicopterFlyStorage::getInstance().AllocateFlyer(p_client_object)),
      m_p_manager_look(Script_HelicopterLookStorage::getInstance().AllocateLooker(p_client_object)),
      m_p_patrol_move(nullptr), m_p_patrol_look(nullptr), m_last_index(0), m_state(0), m_next_index(0),
      m_is_callback(false), m_is_by_stop_fire_fly(false), m_flag_to_wp_callback(false)
{
    this->m_scheme_name = "heli_move";
}

Script_SchemeHelicopterMove::~Script_SchemeHelicopterMove(void)
{
    if (this->m_p_patrol_look)
    {
        Msg("[Scripts/Script_SchemeHelicopterMove/~dtor()] deleteing patrol_look!");
        xr_delete(this->m_p_patrol_look);
    }

    if (this->m_p_patrol_move)
    {
        Msg("[Scripts/Script_SchemeHelicopterMove/~dtor()] deleting patrol_move");
        xr_delete(this->m_p_patrol_move);
    }

    if (this->m_p_manager_fire)
    {
        Script_HelicopterFireStorage::getInstance().DeallocateFirer(this->m_npc->ID());
    }

    if (this->m_p_manager_fly)
    {
        Script_HelicopterFlyStorage::getInstance().DeallocateFlyer(this->m_npc->ID());
    }

    if (this->m_p_manager_look)
    {
        Script_HelicopterLookStorage::getInstance().DeallocateLooker(this->m_npc->ID());
    }
}

void Script_SchemeHelicopterMove::reset_scheme(const bool is_loading)
{
    Msg("[Scripts/Scripts_SchemeHelicopterMove/reset_scheme(is_loading)] %s", this->m_npc->Name());
    this->m_storage->ClearSignals();
    this->m_p_helicopter->TurnEngineSound(this->m_storage->IsHelicopterEngineSound());

    if (!Globals::patrol_path_exists(this->m_storage->getHelicopterPathMoveName().c_str()))
    {
        R_ASSERT2(false, "Patrol path doesnt exist!");
        return;
    }

    this->m_p_patrol_move = new CPatrolPathParams(this->m_storage->getHelicopterPathMoveName().c_str());
    this->m_patrol_move_info =
        Globals::Utils::path_parse_waypoints(this->m_storage->getHelicopterPathMoveName().c_str());

    if (!this->m_storage->getHelicopterPathLookName().empty())
    {
        if (this->m_storage->getHelicopterPathLookName() == "actor")
        {
            this->m_p_manager_fly->set_look_point(DataBase::Storage::getInstance().getActor()->Position());
            this->update_look_state();
        }
        else
        {
            this->m_p_patrol_look = new CPatrolPathParams(this->m_storage->getHelicopterPathLookName().c_str());
            this->m_p_manager_fly->set_look_point(this->m_p_patrol_look->point(std::uint32_t(0)));
            this->update_look_state();

            if (!this->m_p_patrol_look->m_path)
            {
                R_ASSERT2(false, "unable to find waypoint!");
                return;
            }
        }
    }
    else
    {
        this->m_p_patrol_look = nullptr;
    }

    this->m_max_velocity = this->m_storage->getHelicopterVelocity();

    if (is_loading)
    {
        this->m_state = XR_LOGIC::pstor_retrieve_number(this->m_npc, "st");
        this->m_last_index = XR_LOGIC::pstor_retrieve_number(this->m_npc, "li");
        this->m_next_index = XR_LOGIC::pstor_retrieve_number(this->m_npc, "ni");
        this->m_is_callback = XR_LOGIC::pstor_retrieve_bool(this->m_npc, "wc");
    }
    else
    {
        this->m_last_index = 0;
        this->m_next_index = 0;

        this->m_p_manager_fly->setMaxVelocity(this->m_max_velocity);
        this->m_p_manager_fly->setAccFW(this->m_max_velocity / 15.0f);
        this->m_p_manager_fly->setAccBW(2.0f * (this->m_p_manager_fly->getAccFW() / 3.0f));

        this->m_p_helicopter->SetLinearAcc(this->m_p_manager_fly->getAccFW(), this->m_p_manager_fly->getAccBW());
        this->m_p_helicopter->SetMaxVelocity(this->m_max_velocity);

        this->m_state = 0;
        this->m_stop_point.set(0.0f, 0.0f, 0.0f);
        this->m_is_by_stop_fire_fly = false;
        this->m_is_callback = false;
        this->m_flag_to_wp_callback = false;

        this->m_p_manager_fire->setEnemyTypeName(this->m_storage->getHelicopterEnemyName());
        this->m_p_manager_fire->setEnemy(nullptr);
        this->m_p_manager_fire->setFlagByEnemy(true);

        if (!this->m_storage->getHelicopterFirePointName().empty())
        {
            this->m_p_manager_fire->setFirePoint(
                CPatrolPathParams(this->m_storage->getHelicopterFirePointName().c_str()).point(std::uint32_t(0)));
        }

        if (!fis_zero(this->m_storage->getHelicopterMaxMinigunDistance()))
        {
            this->m_p_helicopter->m_max_mgun_dist = this->m_storage->getHelicopterMaxMinigunDistance();
        }

        if (!fis_zero(this->m_storage->getHelicopterMaxRocketDistance()))
        {
            this->m_p_helicopter->m_max_rocket_dist = this->m_storage->getHelicopterMaxRocketDistance();
        }

        if (!fis_zero(this->m_storage->getHelicopterMinMinigunDistance()))
        {
            this->m_p_helicopter->m_min_mgun_dist = this->m_storage->getHelicopterMinMinigunDistance();
        }

        if (!fis_zero(this->m_storage->getHelicopterMinRocketDistance()))
        {
            this->m_p_helicopter->m_min_rocket_dist = this->m_storage->getHelicopterMinRocketDistance();
        }

        if (this->m_storage->IsHelicopterUseMinigun())
        {
            this->m_p_helicopter->m_use_mgun_on_attack = true;
        }
        else
        {
            this->m_p_helicopter->m_use_mgun_on_attack = false;
        }

        if (this->m_storage->IsHelicopterUseRocket())
        {
            this->m_p_helicopter->m_use_rocket_on_attack = true;
        }
        else
        {
            this->m_p_helicopter->m_use_rocket_on_attack = false;
        }

        this->m_p_manager_fire->setUpdVis(this->m_storage->getHelicopterUpdVis());
        this->m_p_manager_fire->update_enemy_state();
        this->update_movement_state();

        if (this->m_storage->IsHelicopterShowHealth())
        {
            this->m_p_manager_fire->cs_remove();
            this->m_p_manager_fire->setShowHealth(true);
            this->m_p_manager_fire->cs_heli();
        }
        else
        {
            this->m_p_manager_fire->setShowHealth(false);
            this->m_p_manager_fire->cs_remove();
        }

        this->m_p_helicopter->UseFireTrail(this->m_storage->IsHelicopterFireTrail());
    }
}

void Script_SchemeHelicopterMove::save(void)
{
    XR_LOGIC::pstor_store(this->m_npc, "st", this->m_state);
    XR_LOGIC::pstor_store(this->m_npc, "li", static_cast<std::uint8_t>(this->m_last_index));
    XR_LOGIC::pstor_store(this->m_npc, "ni", static_cast<std::uint8_t>(this->m_next_index));
    XR_LOGIC::pstor_store(this->m_npc, "wc", this->m_is_callback);
}

void Script_SchemeHelicopterMove::update(const float delta)
{
    // Lord: доделать когда будет XR_LOGIC::try_switch_to_another_section

    if (this->m_is_callback)
    {
        this->update_movement_state();
        this->m_is_callback = false;
    }
    else
    {
        if (!this->m_storage->getHelicopterPathLookName().empty())
        {
            if (this->m_storage->getHelicopterPathLookName() == "actor")
            {
                this->m_p_manager_fly->set_look_point(DataBase::Storage::getInstance().getActor()->Position());

                if (this->m_storage->IsHelicopterStopFire())
                {
                    if (this->m_p_helicopter->isVisible(DataBase::Storage::getInstance().getActor()))
                    {
                        if (!this->m_is_by_stop_fire_fly)
                        {
                            this->m_stop_point = this->m_npc->Position();
                            this->m_is_by_stop_fire_fly = true;
                            this->m_is_callback = true;
                        }
                        else
                        {
                            this->m_is_by_stop_fire_fly = false;
                            this->m_is_callback = true;
                        }
                    }
                }
            }
            this->update_look_state();
        }

        if (this->m_storage->getHelicopterPathLookName().empty() && this->m_p_manager_look->getLookState())
        {
            this->m_p_manager_look->calculate_look_point(this->m_p_manager_fly->getDestinationPoint(), true);
        }
    }
}

void Script_SchemeHelicopterMove::waypoint_callback(
    CScriptGameObject* p_client_object, const std::uint32_t action_movement_type, const std::uint32_t index)
{
    if (!this->m_flag_to_wp_callback)
    {
        if (this->m_p_patrol_move)
        {
            if (index == this->m_last_index)
            {
                return;
            }

            if (index != Globals::kUnsignedInt32Undefined)
            {
                this->m_last_index = index;
            }
            else
            {
                if (this->m_patrol_move_info.getData().size() <= this->m_last_index)
                {
                    xr_string signal_name = this->m_patrol_move_info.getData().at(this->m_last_index).getValue("sig");
                    if (!signal_name.empty())
                    {
                        this->m_storage->setSignals(signal_name, true);
                    }
                }

                if (this->m_p_patrol_move->count() > 1)
                {
                    this->m_last_index = this->m_next_index;
                }
            }
        }
    }

    this->m_is_callback = true;
}

} // namespace Scripts
} // namespace Cordis
