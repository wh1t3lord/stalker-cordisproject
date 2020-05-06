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
#ifdef DEBUG
    if (m_object)
        Msg("Destroying binded object %s", m_object->Name());
#endif
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
#ifdef DEBUG
    MESSAGE("NOT "
        "OVERLOADED FUNCTION! %s",
        p_client_object->Name());
#endif
}

void CScriptBinderObject::hit_callback(CScriptGameObject* p_client_object, const float amount,
    const Fvector& local_direction, CScriptGameObject* p_client_who, const std::int16_t bone_index)
{
#ifdef DEBUG
    MESSAGE("NOT "
        "OVERLOADED FUNCTION!");
#endif
}

void CScriptBinderObject::death_callback(CScriptGameObject* p_client_victim, CScriptGameObject* p_client_who)
{
#ifdef DEBUG
    MESSAGE("NOT "
        "OVERLOADED FUNCTION!");
#endif
}

void CScriptBinderObject::hear_callback(CScriptGameObject* p_client_object, const std::uint16_t who_id,
    const int sound_type, const Fvector& sound_position, const float sound_power)
{
#ifdef DEBUG
    MESSAGE("NOT "
        "OVERLOADED FUNCTION!");
#endif
}

void CScriptBinderObject::use_callback(CScriptGameObject* p_client_object, CScriptGameObject* p_client_who)
{
#ifdef DEBUG
    MESSAGE("NOT OVERLOADED FUNCTION!");
#endif
}

void CScriptBinderObject::animation_callback(const bool) 
{
    #ifdef DEBUG
    MESSAGE("not overloaded function for %s", this->m_object->Name());
    #endif
}

void CScriptBinderObject::use_invertory_item(CScriptGameObject* p_client_object)
{
#ifdef DEBUG
    MESSAGE("NOT OVERLOADED FUNCTION!");
#endif
}

void CScriptBinderObject::take_item_from_box(CScriptGameObject* p_client_box, CScriptGameObject* p_client_item)
{
#ifdef DEBUG
    MESSAGE("NOT OVERLOADED FUNCTION!");
#endif
}

void CScriptBinderObject::task_callback(CGameTask* p_game_task, const std::uint32_t task_state)
{
#ifdef DEBUG
    MESSAGE("NOT OVERLOADED FUNCTION!");
#endif
}

void CScriptBinderObject::on_trade(
    CScriptGameObject* p_client_object, const bool is_sell_bye, const std::uint32_t money)
{
#ifdef DEBUG
    MESSAGE("NOT OVERLOADED FUNCTION!");
#endif
}

void CScriptBinderObject::on_item_drop(CScriptGameObject* p_client_object)
{
#ifdef DEBUG
    MESSAGE("NOT OVERLOADED FUNCTION!");
#endif
}

void CScriptBinderObject::on_item_take(CScriptGameObject* p_client_object)
{
#ifdef DEBUG
    MESSAGE("NOT OVERLOADED FUNCTION!");
#endif
}

void CScriptBinderObject::info_callback(const xr_string& info_name)
{
#ifdef DEBUG
    MESSAGE("NOT OVERLOADED FUNCTION!");
#endif
}

void CScriptBinderObject::on_point(const float distance, const Fvector& position, const std::uint32_t path_index)
{
#ifdef DEBUG
    MESSAGE("not overloaded function!");
#endif // DEBUG
}

void CScriptBinderObject::on_hit(const float power, const float impulse, const ALife::EHitType& hit_type, const std::uint16_t enemy_id)
{
#ifdef DEBUG
    MESSAGE("not overloaded function!");
#endif // DEBUG
}
