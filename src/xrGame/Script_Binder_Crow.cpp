#include "stdafx.h"
#include "Script_Binder_Crow.h"

namespace Cordis
{
namespace Scripts
{
Script_Binder_Crow::Script_Binder_Crow(CScriptGameObject* p_object) : inherited(p_object), m_body_timer(0) {}

Script_Binder_Crow::~Script_Binder_Crow(void) {}

void Script_Binder_Crow::reinit(void)
{
    this->m_body_timer = 0;
    inherited::reinit();

    // Lord: протестировать
    DataBase::Storage::getInstance().deleteStorage(this->m_object->ID());
}

bool Script_Binder_Crow::net_Spawn(SpawnType DC)
{
    if (!inherited::net_Spawn(DC))
        return false;

    DataBase::Storage::getInstance().addObject(this->m_object);
    DataBase::Storage::getInstance().setCrowStorage(this->m_object->ID(), this->m_object->ID());
    DataBase::Storage::getInstance().addCrowToCount();

    this->m_is_use_death_callback = true;

    return true;
}

void Script_Binder_Crow::net_Destroy(void) 
{
    this->m_is_use_death_callback = false; 

    DataBase::Storage::getInstance().setCrowStorage(this->m_object->ID(), 0);
    DataBase::Storage::getInstance().deleteCrowToCount();
    DataBase::Storage::getInstance().deleteObject(this->m_object);

    inherited::net_Destroy();
}

void Script_Binder_Crow::shedule_Update(std::uint32_t time_delta)
{
    inherited::shedule_Update(time_delta);

    if (!this->m_object->Alive() && this->m_body_timer && (this->m_body_timer <= Globals::get_time_global() - 120000))
    {
        Msg("[Scripts/Script_Binder_Crow/shedule_Update(time_delta)] releasing the object %s", this->m_object->Name());
        Globals::Game::alife_release(ai().alife().objects().object(this->m_object->ID()), true);
    }
}

void Script_Binder_Crow::save(NET_Packet* output_packet)
{
    Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, false, "Script_Binder_Crow");
    inherited::save(output_packet);
    XR_LOGIC::save_object(this->m_object, *output_packet);
    output_packet->w_u32(this->m_body_timer);
    Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, true, "Script_Binder_Crow");
}

void Script_Binder_Crow::load(IReader* input_packet) 
{
    Globals::set_save_marker(*input_packet, Globals::kSaveMarkerMode_Load, false, "Script_Binder_Crow");
    inherited::load(input_packet);
    XR_LOGIC::load_object(this->m_object, *input_packet);
    this->m_body_timer = input_packet->r_u32();
    Globals::set_save_marker(*input_packet, Globals::kSaveMarkerMode_Load, true, "Script_Binder_Crow");
}

bool Script_Binder_Crow::net_SaveRelevant(void) { return true; }

void Script_Binder_Crow::death_callback(CScriptGameObject* p_client_victim, CScriptGameObject* p_client_who)
{
    if (this->m_is_use_death_callback == false)
        return;

    this->m_body_timer = Globals::get_time_global();
    DataBase::Storage::getInstance().setCrowStorage(this->m_object->ID(), 0);
    DataBase::Storage::getInstance().deleteCrowToCount();
}

} // namespace Scripts
} // namespace Cordis
