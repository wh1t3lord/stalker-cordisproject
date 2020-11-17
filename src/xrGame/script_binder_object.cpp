////////////////////////////////////////////////////////////////////////////
//	Module 		: script_binder_object.cpp
//	Created 	: 29.03.2004
//  Modified 	: 29.03.2004
//	Author		: Dmitriy Iassenev
//	Description : Script object binder
////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "script_binder_object.h"
#include "script_game_object.h"

CScriptBinderObject::CScriptBinderObject(CScriptGameObject* object) { m_object = object; }
CScriptBinderObject::~CScriptBinderObject()
{
    if (m_object)
    {
        MESSAGEI("Destroying binded object %s", m_object->Name());
    }
}

void CScriptBinderObject::reinit() {}
void CScriptBinderObject::reload(LPCSTR section) {}
bool CScriptBinderObject::net_Spawn(SpawnType DC) { return (true); }
void CScriptBinderObject::net_Destroy() {}
void CScriptBinderObject::net_Import(NET_Packet* net_packet) {}
void CScriptBinderObject::net_Export(NET_Packet* net_packet) {}
void CScriptBinderObject::shedule_Update(u32 time_delta) {}
void CScriptBinderObject::save(NET_Packet* output_packet) {}
void CScriptBinderObject::load(IReader* input_packet) {}
bool CScriptBinderObject::net_SaveRelevant() { return (false); }
void CScriptBinderObject::net_Relcase(CScriptGameObject* object) {}

void CScriptBinderObject::waypoint_callback(
    CScriptGameObject* p_client_object, const std::uint32_t action_type_movement, const std::uint32_t point_index)
{
    MESSAGEW("NOT OVERLOADED FUNCTION! %s", p_client_object->Name());
}

void CScriptBinderObject::hit_callback(CScriptGameObject* p_client_object, const float amount,
    const Fvector& local_direction, CScriptGameObject* p_client_who, const std::int16_t bone_index)
{
    MESSAGEW("NOT OVERLOADED FUNCTION!");
}

void CScriptBinderObject::death_callback(CScriptGameObject* p_client_victim, CScriptGameObject* p_client_who)
{
 
    MESSAGEW("NOT OVERLOADED FUNCTION!");
 
}

void CScriptBinderObject::hear_callback(CScriptGameObject* p_client_object, const std::uint16_t who_id,
    const int sound_type, const Fvector& sound_position, const float sound_power)
{
 
    MESSAGEW("NOT OVERLOADED FUNCTION!");
 
}

void CScriptBinderObject::use_callback(CScriptGameObject* p_client_object, CScriptGameObject* p_client_who)
{
    MESSAGEW("NOT OVERLOADED FUNCTION!");
}

void CScriptBinderObject::animation_callback(const bool) 
{
    MESSAGEW("not overloaded function for %s", this->m_object->Name());
}

void CScriptBinderObject::use_invertory_item(CScriptGameObject* p_client_object)
{
    MESSAGEW("NOT OVERLOADED FUNCTION!");
}

void CScriptBinderObject::take_item_from_box(CScriptGameObject* p_client_box, CScriptGameObject* p_client_item)
{
    MESSAGEW("NOT OVERLOADED FUNCTION!");
}

void CScriptBinderObject::task_callback(CGameTask* p_game_task, const std::uint32_t task_state)
{
 
    MESSAGEW("NOT OVERLOADED FUNCTION!");
 
    if (task_state != eTaskStateFail)
    {
        if (task_state == eTaskStateCompleted)
        {
            Cordis::Scripts::Script_NewsManager::getInstance().SendTip(Cordis::Scripts::DataBase::Storage::getInstance().getActor(), "complete", "");
        }
        else
        {
            Cordis::Scripts::Script_NewsManager::getInstance().SendTip(Cordis::Scripts::DataBase::Storage::getInstance().getActor(), "new", "");
        }
    }

    Cordis::Scripts::Script_TaskManager::getInstance().TaskCallback(p_game_task, task_state);
}

void CScriptBinderObject::on_trade(
    CScriptGameObject* p_client_object, const bool is_sell_bye, const std::uint32_t money)
{
    MESSAGEW("NOT OVERLOADED FUNCTION!");
}

void CScriptBinderObject::on_item_drop(CScriptGameObject* p_client_object)
{
    MESSAGEW("NOT OVERLOADED FUNCTION!");
}

void CScriptBinderObject::on_item_take(CScriptGameObject* p_client_object)
{
    MESSAGEW("NOT OVERLOADED FUNCTION!");
}

void CScriptBinderObject::info_callback(const xr_string& info_name)
{
    MESSAGEW("NOT OVERLOADED FUNCTION!");
}

void CScriptBinderObject::on_point(const float distance, const Fvector& position, const std::uint32_t path_index)
{
    MESSAGEW("not overloaded function!");
}

void CScriptBinderObject::on_hit(const float power, const float impulse, const ALife::EHitType& hit_type, const std::uint16_t enemy_id)
{
    MESSAGEW("not overloaded function!");
}
