#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_Binder_Monster : public CScriptBinderObject
{
    using inherited = CScriptBinderObject;

public:
    Script_Binder_Monster(void) = delete;
    Script_Binder_Monster(CScriptGameObject* object);
    virtual ~Script_Binder_Monster(void);
    virtual void reinit(void);
    virtual bool net_Spawn(SpawnType DC);
    virtual void net_Destroy(void);
    virtual void shedule_Update(std::uint32_t time_delta);
    virtual void save(NET_Packet* output_packet);
    virtual void load(IReader* input_packet);
    virtual bool net_SaveRelevant(void);
    virtual void net_Relcase(CScriptGameObject* object);

	virtual void waypoint_callback(
		CScriptGameObject* p_client_object, const std::uint32_t action_type_movement, const std::uint32_t point_index);
	virtual void hit_callback(CScriptGameObject* p_client_object, const float amount, const Fvector& local_direction,
		CScriptGameObject* p_client_who, const std::int16_t bone_index);
	virtual void death_callback(CScriptGameObject* p_client_victim, CScriptGameObject* p_client_who);
	virtual void hear_callback(CScriptGameObject* p_client_object, const std::uint16_t who_id, const int sound_type,
		const Fvector& sound_position, const float sound_power);
private:
    bool m_is_loaded;
    const DataBase::Storage_Data& m_storage;
};
}
} // namespace Cordis
