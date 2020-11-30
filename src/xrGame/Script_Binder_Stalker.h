#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_Binder_Stalker : public CScriptBinderObject
{
    using inherited = CScriptBinderObject;

public:
    Script_Binder_Stalker(void) = delete;
    Script_Binder_Stalker(CScriptGameObject* object);
    virtual ~Script_Binder_Stalker(void);

    void reinit(void) override;
    void reload(LPCSTR section_name) override;
    bool net_Spawn(SpawnType DC) override;
    void net_Destroy(void) override;
    void net_Import(NET_Packet* packet) override;
    void net_Export(NET_Packet* packet) override;
    void shedule_Update(std::uint32_t time_delta) override;
    void save(NET_Packet* output_packet) override;
    void load(IReader* input_packet) override;
    bool net_SaveRelevant(void) override;
    void net_Relcase(CScriptGameObject* object) override;

	/// <summary>
	/// This method is called when NPC is reached waypoint physically
	/// </summary>
	/// <param name="p_client_object">CScriptGameObject</param>
	/// <param name="action_type_movement"></param>
	/// <param name="point_index"></param>
	void waypoint_callback(
		CScriptGameObject* p_client_object, const std::uint32_t action_type_movement, const std::uint32_t point_index) override;
	

    void hit_callback(CScriptGameObject* p_client_object, const float amount, const Fvector& local_direction,
		CScriptGameObject* p_client_who, const std::int16_t bone_index) override;

	void death_callback(CScriptGameObject* p_client_victim, CScriptGameObject* p_client_who) override;
	
    void hear_callback(CScriptGameObject* p_client_object, const std::uint16_t who_id, const int sound_type,
		const Fvector& sound_position, const float sound_power) override;
	
    void use_callback(CScriptGameObject* p_client_object, CScriptGameObject* p_client_who) override;
    void animation_callback(const bool) override;

    bool extrapolate_callback(std::uint32_t point);
private:
    bool m_is_loaded;
    bool m_is_first_update;
    std::uint32_t m_last_update;
    std::uint32_t m_enemy_helicopter_id;
};
}
} // namespace Cordis

