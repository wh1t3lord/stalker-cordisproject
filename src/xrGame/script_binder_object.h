////////////////////////////////////////////////////////////////////////////
//	Module 		: script_binder_object.h
//	Created 	: 29.03.2004
//  Modified 	: 29.03.2004
//	Author		: Dmitriy Iassenev
//	Description : Script object binder
////////////////////////////////////////////////////////////////////////////

#pragma once

class CSE_ALifeObject;
class CScriptGameObject;
class NET_Packet;

class CScriptBinderObject
{
public:
    typedef CSE_ALifeObject* SpawnType;
    CScriptGameObject* m_object;

public:
    CScriptBinderObject(CScriptGameObject* object);
    virtual ~CScriptBinderObject();
    virtual void reinit();
    virtual void reload(LPCSTR section);
    virtual bool net_Spawn(SpawnType DC);
    virtual void net_Destroy();
    virtual void net_Import(NET_Packet* net_packet);
    virtual void net_Export(NET_Packet* net_packet);
    virtual void shedule_Update(u32 time_delta);
    virtual void save(NET_Packet* output_packet);
    virtual void load(IReader* input_packet);
    virtual bool net_SaveRelevant();
    virtual void net_Relcase(CScriptGameObject* object);

#pragma region Cordis Additional callbacks
    virtual void waypoint_callback(CScriptGameObject* p_client_object, const std::uint32_t action_type_movement,
        const std::uint32_t point_index);
    virtual void hit_callback(CScriptGameObject* p_client_object, const float amount, const Fvector& local_direction,
        CScriptGameObject* p_client_who, const std::int16_t bone_index);
    virtual void death_callback(CScriptGameObject* p_client_victim, CScriptGameObject* p_client_who);
    virtual void hear_callback(CScriptGameObject* p_client_object, const std::uint16_t who_id, const int sound_type,
        const Fvector& sound_position, const float sound_power);
    virtual void use_callback(CScriptGameObject* p_client_object, CScriptGameObject* p_client_who);
#pragma endregion 

#pragma region Cordis Actor binder callbacks
    virtual void use_invertory_item(CScriptGameObject* p_client_object);
    virtual void take_item_from_box(CScriptGameObject* p_client_box, CScriptGameObject* p_client_item);
    virtual void task_callback(CGameTask* p_game_task, const std::uint32_t task_state);
    virtual void on_trade(CScriptGameObject* p_client_object, const bool is_sell_bye, const std::uint32_t money);
    virtual void on_item_drop(CScriptGameObject* p_client_object);
    virtual void on_item_take(CScriptGameObject* p_client_object);
    virtual void info_callback(const xr_string& info_name);
#pragma endregion
};
