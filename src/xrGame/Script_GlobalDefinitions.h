#pragma once

#include "script_particles.h"

namespace Cordis
{
namespace Scripts
{
// @ Uses in Script_SchemeSRParticle
class ParticleData
{
public:
    ParticleData(void) = default;
    ~ParticleData(void)
    {
        if (this->m_particle)
        {
            Msg("[Scripts/ParticleData/~dtor()] WARNING: deleting this->m_particle!");
            xr_delete(this->m_particle);
        }
    }
    inline bool IsPlayed(void) const noexcept { return this->m_is_played; }
    inline void setPlayed(const bool value) noexcept { this->m_is_played = value; }

    inline std::uint32_t getDelay(void) const noexcept { return this->m_delay; }
    inline void setDelay(const std::uint32_t value) noexcept { this->m_delay = value; }

    inline std::uint32_t getTime(void) const noexcept { return this->m_time; }
    inline void setTime(const std::uint32_t value) noexcept { this->m_time = value; }

    /*
        inline const CScriptSound& getSound(void) const noexcept { return this->m_sound; }
        inline void setSound(const CScriptSound& data) noexcept { this->m_sound = data; }*/

    inline CScriptParticles* const getParticle(void) const { return this->m_particle; }
    inline void setParticle(CScriptParticles* data)
    {
        if (!data)
        {
            Msg("[Scripts/ParticleData/setParticle(data)] WARNING: data == nullptr! You set an empty object");
        }

        this->m_particle = data;
    }

private:
    bool m_is_played = false;
    std::uint32_t m_delay = 0;
    std::uint32_t m_time = 0;
    /*    CScriptSound m_sound;*/
    CScriptParticles* m_particle = nullptr;
};

struct JobDataSmartTerrain
{
    ~JobDataSmartTerrain(void)
    {
        if (this->m_alife_task)
        {
            Msg("[Scripts/Script_SE_SmartTerrain/JobDataSmartTerrain/~dtor()] deleting alife task for %s",
                this->m_job_id.first.c_str());
            xr_delete(this->m_alife_task);
        }
    }

    std::uint8_t m_level_id;
    std::uint32_t m_priority;
    std::uint32_t m_game_vertex_id;
    CALifeSmartTerrainTask* m_alife_task = nullptr;
    CScriptIniFile* m_ini_file = nullptr; // @ Мы его не удаляем, оно удаляется само в JobDataExclusive!
    // @ First - section | Second - job_type (that taking from gulag_general as JobData_SubData::m_job_id respectively)
    Fvector m_position;
    std::pair<xr_string, xr_string> m_job_id;
    xr_string m_ini_path_name;
};

class Script_SE_SmartTerrain;
class Script_SchemeXRAbuse;
class Script_StateManager;

namespace DataBase
{
class Storage_Scheme;
class Storage;
class Data_Overrides;
} // namespace DataBase

namespace GulagGenerator
{
inline static xr_string& getLtx(void) noexcept
{
    static xr_string instance;
    return instance;
}
} // namespace GulagGenerator

namespace XR_REMARK
{
constexpr std::uint32_t kStateInitial = 0;
constexpr std::uint32_t kStateAnimation = 1;
constexpr std::uint32_t kStateSound = 2;
} // namespace XR_REMARK

namespace XR_DANGER
{
constexpr float kDangerIgnoreDistance = 150.0f;
constexpr std::uint32_t kDangerInertionTime = 30000;
inline xr_string get_danger_name(CDangerObject* const p_best_danger);
inline bool is_danger(CScriptGameObject* const p_client_object);
inline std::uint32_t get_danger_time(const CDangerObject* p_client_danger_object);
} // namespace XR_DANGER

namespace XR_LOGIC
{
inline void parse_infopotions(xr_map<std::uint32_t, CondlistData::CondlistValues>& data, xr_string& buffer);
inline bool switch_to_section(
    CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& section_name);
inline bool is_active(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage);
/*
inline void activate_by_section(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
const xr_string& section_name, const xr_string& gulag_name, const bool is_loading);*/
inline void reset_generic_schemes_on_scheme_switch(
    CScriptGameObject* const p_client_object, const xr_string& scheme_name, const xr_string& section_name);
inline DataBase::Data_Overrides cfg_get_overrides(
    CScriptIniFile* const p_ini, const xr_string& section_name, CScriptGameObject* const p_client_object);
void activate_by_section(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& section_name, const xr_string& gulag_name, const bool is_loading);
CScriptIniFile* configure_schemes(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& ini_filename, std::uint32_t stype, const xr_string& section_logic_name,
    const xr_string& gulag_name);
} // namespace XR_LOGIC

namespace CRD_DialogManager
{
// @ Private
// @ don't call it in other fields of code and that file!!!!!!!!!
inline static std::uint32_t generate_id(void) noexcept
{
    static std::uint32_t id = 5;
    return (++id);
}

inline void init_start_dialogs(CPhraseDialog* dialog, const xr_string& dialog_type_name);
inline void init_hello_dialogs(CPhraseDialog* p_dialog);
} // namespace CRD_DialogManager

namespace XR_CONDITION
{
inline bool is_surge_started(void);
inline bool is_surge_complete(void);
inline bool is_surge_kill_all(void);
inline bool is_enemy_actor(CScriptGameObject* enemy, CScriptGameObject* object);
inline bool is_fighting_dist_ge_client(
    CScriptGameObject* enemy, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_fighting_dist_ge_server(
    CSE_ALifeDynamicObject* enemy, CSE_ALifeDynamicObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_fighting_dist_le_client(
    CScriptGameObject* enemy, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_fighting_dist_le_server(
    CSE_ALifeDynamicObject* enemy, CSE_ALifeDynamicObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_enemy_in_zone_client(
    CScriptGameObject* enemy, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_enemy_in_zone_server(
    CSE_ALifeDynamicObject* server_enemy, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_black_screen_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_black_screen_client_server(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool check_npc_name_client(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool check_npc_name_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool check_enemy_name_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_playing_sound(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_see_npc(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_actor_see_npc(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_npc_in_actor_frustrum(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_wounded(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_dist_to_actor_ge_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_dist_to_actor_ge_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_dist_to_actor_ge_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_obj_on_job(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_distance_to_obj_on_job_le(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_story_obj_in_zone_by_name_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_story_obj_in_zone_by_name_server(
    CSE_ALifeDynamicObject* actor, CSE_ALifeDynamicObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_actor_in_zone_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_actor_in_zone_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_actor_in_zone_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_npc_in_zone_client(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_npc_in_zone_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_npc_in_zone_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_health_le(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_actor_health_le(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_npc_community_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_npc_community_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_npc_community_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_hitted_by(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_hitted_on_bone(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_best_pistol(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_deadly_hit(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_killed_by(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_alive_one_client(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_alive_one_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_alive_one_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_alive_client(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_alive_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_alive_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_dead_client(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_dead_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_dead_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_story_object_exist_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_story_object_exist_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_story_object_exist_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_actor_has_item_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_actor_has_item_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_actor_has_item_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_npc_has_item(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_signal(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_counter_greater_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_counter_greater_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_counter_equal_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_counter_equal_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_kamp_talk(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool check_smart_alarm_status_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool check_smart_alarm_status_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool check_smart_alarm_status_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_has_enemy(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_has_actor_enemy(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_see_enemy(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_has_enemy_in_current_loopholes_fov(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_talking(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_npc_talking(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_see_actor(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_actor_enemy(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_actor_friend(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_actor_neutral(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_factions_enemies(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_factions_friends(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_factions_neutrals(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_faction_enemy_to_actor_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_faction_enemy_to_actor_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_faction_enemy_to_actor_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_faction_friend_to_actor_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_faction_friend_to_actor_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_faction_friend_to_actor_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_faction_neutral_to_actor_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_faction_neutral_to_actor_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_faction_neutral_to_actor_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_friend_to_actor_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_friend_to_actor_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_friend_to_actor_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_enemy_to_actor_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_enemy_to_actor_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_enemy_to_actor_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_neutral_to_actor_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_neutral_to_actor_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_neutral_to_actor_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_fighting_actor(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_hit_by_actor(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_killed_by_actor(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_actor_has_weapon_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_actor_has_weapon_client_server(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_actor_active_detector_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_actor_active_detector_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_actor_active_detector_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_heavy_wounded(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_rain_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_rain_server(CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_rain_client_server(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_heavy_rain_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_heavy_rain_server(CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_heavy_rain_client_server(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_day_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_day_server(CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_day_client_server(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_dark_night_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_dark_night_server(CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_dark_night_client_server(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_jup_a12_mercs_time_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_jup_a12_mercs_time_server(CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_jup_a12_mercs_time_client_server(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_zat_b7_is_night_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_zat_b7_is_night_server(CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_zat_b7_is_night_client_server(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_zat_b7_is_late_attack_time_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_zat_b7_is_late_attack_time_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_zat_b7_is_late_attack_time_client_server(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_mob_has_enemy(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_mob_was_hit(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_actor_on_level_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_actor_on_level_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_actor_on_level_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_in_zone_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_in_zone_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_has_enemy_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_has_enemy_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_has_enemy_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_in_zone_all_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_in_zone_all_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_in_zone_all_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squads_in_zone_b41_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_squads_in_zone_b41_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squads_in_zone_b41_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_target_squad_name_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_target_squad_name_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_target_smart_name_client(
    CScriptGameObject* actor, CScriptGameObject* smart, const xr_vector<xr_string>& buffer);
inline bool is_squad_exist_client(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_exist_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_exist_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_commander_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_squad_commander_server(CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_squad_commander_client_server(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_squad_npc_count_ge_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_npc_count_ge_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_npc_count_ge_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_quest_npc_enemy_actor_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_quest_npc_enemy_actor_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_quest_npc_enemy_actor_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_animpoint_reached(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_distance_to_obj_ge_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_distance_to_obj_ge_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_distance_to_obj_ge_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_distance_to_obj_le_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_distance_to_obj_le_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_distance_to_obj_le_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_in_dest_smart_cover(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_actor_nomove_nowpn(void);
inline bool is_jup_b16_is_zone_active(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_dist_to_story_obj_ge_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_dist_to_story_obj_ge_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_dist_to_story_obj_ge_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_actor_has_nimble_weapon_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_actor_has_nimble_weapon_client_server(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_actor_has_active_nimble_weapon_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_actor_has_active_nimble_weapon_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_jup_b202_inventory_box_empty_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_jup_b202_inventory_box_empty_client_server(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_jup_b202_inventory_box_empty_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_object_exist_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_object_exist_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_object_exist_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_curr_action_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_curr_action_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_squad_curr_action_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_monster_snork(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_monster_dog(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_monster_psy_dog(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_monster_polter(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_monster_tushkano(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_monster_burer(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_monster_controller(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_monster_flesh(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_monster_boar(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_jup_b47_npc_online_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_jup_b47_npc_online_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_jup_b47_npc_online_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_zat_b29_anomaly_has_af_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_zat_b29_anomaly_has_af_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_zat_b29_anomaly_has_af_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_jup_b221_who_will_start_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_jup_b221_who_will_start_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_jup_b221_who_will_start_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_pas_b400_actor_far_forward(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_pas_b400_actor_far_backward(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_pri_a28_actor_is_far(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_check_enemy_smart(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_zat_b103_actor_has_needed_food(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_zat_b29_rivals_dialog_precond(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_jup_b202_actor_treasure_not_in_steal_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_jup_b202_actor_treasure_not_in_steal_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_jup_b202_actor_treasure_not_in_steal_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_jup_b25_senya_spawn_condition_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_jup_b25_senya_spawn_condition_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_jup_b25_senya_spawn_condition_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_jup_b25_flint_gone_condition_client(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_jup_b25_flint_gone_condition_client_server(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_jup_b25_flint_gone_condition_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc);
inline bool is_check_deimos_phase(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_actor_in_surge_cover(void);
inline bool is_door_blocked_by_npc(CScriptGameObject* actor, CScriptGameObject* npc);
inline bool is_has_active_tutorial(void);
inline bool is_upgrade_hint_kardan_client(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer);
inline bool is_upgrade_hint_kardan_client_server(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
inline bool is_upgrade_hint_kardan_server(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer);
} // namespace XR_CONDITION
} // namespace Scripts
} // namespace Cordis

namespace Cordis
{
namespace Scripts
{
namespace Globals
{
#pragma region Cordis Constants
constexpr unsigned int kSTypeStalker = 0;
constexpr unsigned int kSTypeMobile = 1;
constexpr unsigned int kSTypeItem = 2;
constexpr unsigned int kSTypeHelicopter = 3;
constexpr unsigned int kSTypeRestrictor = 4;
constexpr const char* kRelationsTypeEnemy = "enemy";
constexpr const char* kRelationsTypeFriends = "friends";
constexpr const char* kRelationsTypeNeutral = "neutral";
constexpr int kRelationKoeffEnemy = -1000;
constexpr int kRelationKoeffNeutral = 0;
constexpr int kRelationKoeffFriend = 1000;
constexpr float kRelationDefaultSympathy = 0.1f;
constexpr const char* kGeneratedIdForBadValue = "EmptyNameItWasEmptyValue";
constexpr const char* kSystemLtxFileName =
    "system.ltx"; // For CScriptIniFile (Don't use Globals::get_system_ini()->fname() for initialization ctor of
                  // CScriptIniFile)
constexpr std::uint32_t kTimeInfinite = 100000000;
constexpr std::uint32_t kStateManagerAnimationMarkerIn = 1;
constexpr std::uint32_t kStateManagerAnimationMarkerOut = 2;
constexpr std::uint32_t kStateManagerAnimationMarkerIdle = 3;
#pragma region Cordis Animpoint prediction functions
inline bool predicate_const_true(std::uint16_t, bool);
inline bool predicate_animpoint_bread(std::uint16_t npc_id, bool);
inline bool predicate_animpoint_kolbasa(std::uint16_t npc_id, bool);
inline bool predicate_animpoint_vodka(std::uint16_t npc_id, bool);
inline bool predicate_animpoint_energy(std::uint16_t npc_id, bool);
inline bool predicate_animpoint_guitar(std::uint16_t npc_id, bool is_in_camp);
inline bool predicate_animpoint_harmonica(std::uint16_t npc_id, bool is_in_camp);
inline bool predicate_animpoint_weapon(std::uint16_t npc_id, bool);
#pragma endregion

#pragma region Cordis SimulationSquad
constexpr float kSimulationSquadActionsStayPointIdleMin = 180.0f * 60.0f;
constexpr float kSimulationSquadActionsStayPointIdleMax = 300.0f * 60.0f;
constexpr const char* kSimulationSquadCurrentActionIDStayOnTarget = "stay_target";
constexpr const char* kSimulationSquadCurrentActionIDReachTarget = "reach_target";
#pragma endregion

#pragma region Cordis SmartTerrain
constexpr std::uint32_t kSmartTerrainDeathIdleTime = 600;
constexpr std::uint32_t kSmartTerrainRespawnIdle = 1000;
constexpr std::uint32_t kSmartTerrainRespawnRadius = 150;
constexpr const char* kSmartTerrainTerritoryDefault = "default";
constexpr const char* kSmartTerrainTerritoryBase = "base";
constexpr const char* kSmartTerrainTerritoryResource = "resource";
constexpr const char* kSmartTerrainTerritoryTerritory = "territory";
constexpr const char* kSmartTerrainSMRTSection = "smart_terrain";
constexpr const char* kSmartTerrainPathFieldPathWalk = "path_walk";
constexpr const char* kSmartTerrainPathFieldPathMain = "path_main";
constexpr const char* kSmartTerrainPathFieldPathHome = "path_home";
constexpr const char* kSmartTerrainPathFieldCenterPoint = "center_point";
#pragma endregion

#pragma region Cordis SYSTEM
constexpr std::uint64_t kUnsignedInt64Undefined = std::uint64_t(-1);
constexpr std::uint32_t kUnsignedInt32Undefined = std::uint32_t(-1);
constexpr std::uint16_t kUnsignedInt16Undefined = std::uint16_t(-1);
constexpr std::uint8_t kUnsignedInt8Undefined = std::uint8_t(-1);
constexpr const char* kStringUndefined = "Undefined";
constexpr const char* kStringTrue = "true";
constexpr const char* kStringFalse = "false";
constexpr char kPstorBooleanTrue = '+';
constexpr char kPstorBooleanUndefined = '0';
constexpr char kPstorBooleanFalse = '-';
constexpr std::uint8_t kPstorTypeNumber = 0;
constexpr std::uint8_t kPstorTypeString = 1;
constexpr std::uint8_t kPstorTypeBoolean = 2;
#pragma endregion

namespace XR_ACTIONS_ID
{
constexpr std::uint32_t kGlobalActions = StalkerDecisionSpace::eWorldOperatorScript;
constexpr std::uint32_t kActionScript = StalkerDecisionSpace::eWorldOperatorScript + 1;
constexpr std::uint32_t kAlife = StalkerDecisionSpace::eWorldOperatorALifePlanner;
constexpr std::uint32_t kScriptCombatPlanner = StalkerDecisionSpace::eWorldOperatorScript + 3;
constexpr std::uint32_t kReachTaskLocation = StalkerDecisionSpace::eWorldOperatorScript + 4;
constexpr std::uint32_t kCorpseExist = StalkerDecisionSpace::eWorldOperatorScript + 50;
constexpr std::uint32_t kWoundedExist = StalkerDecisionSpace::eWorldOperatorScript + 55;
constexpr std::uint32_t kStateManager = StalkerDecisionSpace::eWorldOperatorScript + 100;
constexpr std::uint32_t kSmartcoverAction = StalkerDecisionSpace::eWorldOperatorScript + 215;
constexpr std::uint32_t kTestReaction = StalkerDecisionSpace::eWorldOperatorScript + 218;

constexpr std::uint32_t kStoheActions = StalkerDecisionSpace::eWorldOperatorScript + 220;
constexpr std::uint32_t kStoheTradesellerInit = kStoheActions + 1;
constexpr std::uint32_t kStoheTradesellerBase = kStoheActions + 10;
constexpr std::uint32_t kStoheKampBase = kStoheActions + 20;
constexpr std::uint32_t kStoheMeetBase = kStoheActions + 30;
constexpr std::uint32_t kStoheReactionsBase = kStoheActions + 40;
constexpr std::uint32_t kStoheStateBase = kStoheActions + 50;
constexpr std::uint32_t kStohePosition = kStoheActions + 60;
constexpr std::uint32_t kStoheGulagBase = kStoheActions + 70;
constexpr std::uint32_t kStoheCoverBase = kStoheActions + 80;
constexpr std::uint32_t kStoheCamperBase = kStoheActions + 90;
constexpr std::uint32_t kAssistance = kStoheActions + 100;
constexpr std::uint32_t kDeath = kStoheActions + 110;
constexpr std::uint32_t kAbuseBase = kStoheActions + 120;
constexpr std::uint32_t kPostCombat = kStoheActions + 130;
constexpr std::uint32_t kAnimationPointAction = kStoheActions + 140;
constexpr std::uint32_t kSidorActions = kStoheActions + 1024;
constexpr std::uint32_t kSidorActCommander = kSidorActions + 0; // @ ???
constexpr std::uint32_t kSidorActSoldier = kSidorActions + 5;
constexpr std::uint32_t kSidorActPatrol = kSidorActions + 0; // @ ???
constexpr std::uint32_t kSidorActWoundedBase = kSidorActions + 10;
constexpr std::uint32_t kSidorActBanditsBase = kSidorActions + 20;
constexpr std::uint32_t kSidorActSwatBase = kSidorActions + 40;
constexpr std::uint32_t kSidorActFollowerBase = kSidorActions + 50;
constexpr std::uint32_t kSidorActReportLeader = kSidorActions + 60;
constexpr std::uint32_t kSidorActReportSoldier = kSidorActions + 70;
constexpr std::uint32_t kSidorActRest = kSidorActions + 80;
constexpr std::uint32_t kSidorActBar = kSidorActions + 90;
constexpr std::uint32_t kSidorActChaser = kSidorActions + 100;
constexpr std::uint32_t kSidorActSos = kSidorActions + 110;
constexpr std::uint32_t kSidorActPtr = kSidorActions + 120;
constexpr std::uint32_t kSidorActYantar = kSidorActions + 200;
constexpr std::uint32_t kSidorActSecretLab = kSidorActions + 220;
constexpr std::uint32_t kSidorAttendantAct = kSidorActions + 230;

constexpr std::uint32_t kChugaiActions = kSidorActions + 1024;
constexpr std::uint32_t kChugaiActTutorial = kChugaiActions + 10;
constexpr std::uint32_t kChugaiCommanderBase = kChugaiActions + 20;
constexpr std::uint32_t kChugaiScoutBase = kChugaiActions + 30;
constexpr std::uint32_t kChugaiMilitaryCommanderBase = kChugaiActions + 40;
constexpr std::uint32_t kChugaiEscapeFactoryBanditsBase = kChugaiActions + 50;
constexpr std::uint32_t kChugaiBarmanBase = kChugaiActions + 60;
constexpr std::uint32_t kChugaiAmbushBase = kChugaiActions + 70;
constexpr std::uint32_t kChugaiFormationBase = kChugaiActions + 80;
constexpr std::uint32_t kChugaiReplicsBase = kChugaiActions + 90;
constexpr std::uint32_t kChugaiHelicopterHunterBase = kChugaiActions + 100;
constexpr std::uint32_t kChugaiZombiedBase = kChugaiActions + 110;
constexpr std::uint32_t kChugaiMonolithBase = kChugaiActions + 120;
constexpr std::uint32_t kChugaiCamperBase = kChugaiActions + 130;

constexpr std::uint32_t kZmeyActions = kChugaiActions + 1024;
constexpr std::uint32_t kZmeyActionGuardActivity = kZmeyActions + 100;
constexpr std::uint32_t kZmeyGuardBase = kZmeyActions + 200;
constexpr std::uint32_t kZmeyRobberBase = kZmeyActions + 230;
constexpr std::uint32_t kZmeyCamperBase = kZmeyActions + 260;
constexpr std::uint32_t kZmeySleeperBase = kZmeyActions + 290;
constexpr std::uint32_t kZmeyTalkerBase = kZmeyActions + 320;
constexpr std::uint32_t kZmeyBodyGuardBase = kZmeyActions + 350;
constexpr std::uint32_t kZmeyPunchBase = kZmeyActions + 380;
constexpr std::uint32_t kZmeyGuideBase = kZmeyActions + 410;
constexpr std::uint32_t kZmeyRobberGuardBase = kZmeyActions + 440;
constexpr std::uint32_t kZmeyWalkerBase = kZmeyActions + 480;
constexpr std::uint32_t kZmeyAssassinBase = kZmeyActions + 510;
constexpr std::uint32_t kZmeyScaredBase = kZmeyActions + 540;
constexpr std::uint32_t kZmeyZoneGuardBase = kZmeyActions + 570;
constexpr std::uint32_t kZmeyTestBase = kZmeyActions + 600;
constexpr std::uint32_t kZmeyRemarkBase = kZmeyActions + 630;
constexpr std::uint32_t kZmeyCompanionBase = kZmeyActions + 660;

constexpr std::uint32_t kJonActions = kZmeyActions + 1024;
constexpr std::uint32_t kJonStalkerZombie = kJonActions + 0; // @ ???
constexpr std::uint32_t kJonStalkerNovice = kJonActions + 1;
constexpr std::uint32_t kJonStalkerNovice2 = kJonActions + 2;
constexpr std::uint32_t kJonStalkerNovice3 = kJonActions + 3;
constexpr std::uint32_t kJonStalkerAttack = kJonActions + 4;
constexpr std::uint32_t kJonScientist = kJonActions + 5;
constexpr std::uint32_t kJonSecurity = kJonActions + 6;
constexpr std::uint32_t kJonSniperEnabled = kJonActions + 10;
constexpr std::uint32_t kJonSniperSeeMonster = kJonActions + 11;
constexpr std::uint32_t kJonSniperSeeEnemy = kJonActions + 12;
constexpr std::uint32_t kJonSniperCamper = kJonActions + 13;
constexpr std::uint32_t kJonOutcast = kJonActions + 20;
constexpr std::uint32_t kJonOutcastReplics = kJonActions + 21;

constexpr std::uint32_t kNarratorActions = kJonActions + 1024;
constexpr std::uint32_t kPeacemakerActions = kNarratorActions + 1024;
constexpr std::uint32_t kPeacemakerShBase = kPeacemakerActions + 10;
} // namespace XR_ACTIONS_ID

namespace GulagGenerator
{
constexpr const char* kGulagJobPoint = "point_job";
constexpr const char* kGulagJobPath = "path_job";
constexpr const char* kGulagJobSmartCover = "smartcover_job";
constexpr const char* kGulagJobNameWalker = "walker";
constexpr const char* kGulagJobNameCamper = "camper";
constexpr const char* kGulagJobNamePatrol = "patrol";
constexpr const char* kGulagJobNameAnimpoint = "animpoint";
constexpr const char* kGulagJobNameSmartCover = "smartcover";
constexpr const char* kGulagJobNameRemark = "remark";
constexpr const char* kGulagJobNameCover = "cover";
constexpr const char* kGulagJobNameSleeper = "sleeper";
constexpr const char* kGulagJobNameMobWalker = "mob_walker";
constexpr const char* kGulagJobNameMobHome = "mob_home";
constexpr const char* kGulagJobNameMobJump = "mob_jump";
constexpr const char* kGulagJobNameCompanion = "companion";

enum
{
    kJobsStalker,
    kJobsMonster
};
} // namespace GulagGenerator

#pragma endregion

#pragma region RandomImplementation
class Script_RandomInt
{
private:
    Script_RandomInt(void) : m_generator(std::random_device()()) {}

public:
    inline static Script_RandomInt& getInstance(void) noexcept
    {
        static Script_RandomInt instance;
        return instance;
    }

    ~Script_RandomInt(void) = default;
    Script_RandomInt(const Script_RandomInt&) = delete;
    Script_RandomInt& operator=(const Script_RandomInt&) = delete;
    Script_RandomInt(Script_RandomInt&&) = delete;
    Script_RandomInt& operator=(Script_RandomInt&&) = delete;

    inline int Generate(const int& minimum, const int& maximum)
    {
        if (minimum > maximum)
        {
            R_ASSERT2(false, "It can't be! Rewrite values minimum and maximum!!!");
        }

        std::uniform_int_distribution<int> distribution(minimum, maximum);
        return distribution(this->m_generator);
    }

    inline std::uint32_t Generate(const std::uint32_t& minimum, const std::uint32_t& maximum)
    {
        if (minimum > maximum)
        {
            R_ASSERT2(false, "It can't be! Rewrite values minimum and maximum!!!");
        }

        std::uniform_int_distribution<std::uint32_t> distribution(minimum, maximum);
        return distribution(this->m_generator);
    }

private:
    std::mt19937 m_generator;
};

class Script_RandomFloat
{
private:
    Script_RandomFloat(void) : m_generator(std::random_device()()) {}

public:
    inline static Script_RandomFloat& getInstance(void) noexcept
    {
        static Script_RandomFloat instance;
        return instance;
    }

    ~Script_RandomFloat(void) = default;
    Script_RandomFloat(const Script_RandomFloat&) = delete;
    Script_RandomFloat& operator=(const Script_RandomFloat&) = delete;
    Script_RandomFloat(Script_RandomFloat&&) = delete;
    Script_RandomFloat& operator=(Script_RandomFloat&&) = delete;

    inline float Generate(const float& minimum = 0.0f, const float& maximum = 1.0f)
    {
        if (minimum > maximum)
        {
            R_ASSERT2(false, "It can't be. Rewrite values minimum and maximum!");
        }

        std::uniform_real_distribution<float> distribution(minimum, maximum);
        return distribution(this->m_generator);
    }

private:
    std::mt19937 m_generator;
};
#pragma endregion
} // namespace Globals

struct StateManagerCallbackData
{
    StateManagerCallbackData(void) = default;
    StateManagerCallbackData(const StateManagerCallbackData& data)
    {
        this->m_begin = data.getBegin();
        this->m_timeout = data.getTimeOut();
        this->m_callback_time = data.m_callback_time;
        this->m_callback_turn_end = data.m_callback_turn_end;
    }

    StateManagerCallbackData& operator=(const StateManagerCallbackData& data)
    {
        this->m_begin = data.getBegin();
        this->m_timeout = data.getTimeOut();
        this->m_callback_time = data.m_callback_time;
        this->m_callback_turn_end = data.m_callback_turn_end;

        return *this;
    }

    ~StateManagerCallbackData(void) = default;

    inline bool isAllFieldEmpty(void) const noexcept
    {
        return (!this->m_begin && !this->m_timeout && !this->m_callback_time && !this->m_callback_turn_end);
    }

    inline bool isCallbackTimeExist(void) const noexcept { return !!this->m_callback_time; }
    inline bool isCallbackTurnEndExist(void) const noexcept { return !!this->m_callback_turn_end; }

    inline std::uint32_t getBegin(void) const noexcept { return this->m_begin; }
    inline void setBegin(const std::uint32_t value) noexcept { this->m_begin = value; }

    inline std::uint32_t getTimeOut(void) const noexcept { return this->m_timeout; }
    inline void setTimeOut(const std::uint32_t value) noexcept { this->m_timeout = value; }

    // @ Argument for std::bind()
    inline void setCallbackTime(const std::function<void(void)>& function) { this->m_callback_time = function; }

    // @ Argument for std::bind()
    inline void setCallbackTurnEnd(const std::function<void(void)>& function) { this->m_callback_turn_end = function; }

    inline void CallCallbackTime(void)
    {
        if (this->m_callback_time)
            this->m_callback_time();
        else
            Msg("[Scripts/StateManagerCallbackData/CallCallbackTime()] WARNINIG: function what you binded is nullptr!");
    }

    inline void CallCallbackTurnEnd(void)
    {
        if (this->m_callback_turn_end)
            this->m_callback_turn_end();
        else
            Msg("[Scripts/StateManagerCallbackData/CallCallbackTurnEnd(void)] WARNING: function what you binded is "
                "nullptr!");
    }

private:
    std::uint32_t m_begin = 0;
    std::uint32_t m_timeout = 0;
    std::function<void(void)> m_callback_time = nullptr; // @ lua => func
    std::function<void(void)> m_callback_turn_end = nullptr; // @ lua => turn_end
}; // namespace Scripts

struct StateManagerAnimationStates
{
    StateManagerAnimationStates(void) : m_animation_marker(0), m_sequence_id(0), m_next_random(0), m_last_id(0) {}
    ~StateManagerAnimationStates(void) = default;

    inline std::uint8_t getAnimationMarker(void) const noexcept { return this->m_animation_marker; }
    inline void setAnimationMarker(const std::uint8_t value) noexcept { this->m_animation_marker = value; }

    inline std::uint32_t getSequenceID(void) const noexcept { return this->m_sequence_id; }
    inline void setSequenceID(const std::uint32_t value) noexcept { this->m_sequence_id = value; }

    inline std::uint32_t getNextRandom(void) const noexcept { return this->m_next_random; }
    inline void setNextRandom(const std::uint32_t value) noexcept { this->m_next_random = value; }

    inline std::uint32_t getLastID(void) const noexcept { return this->m_last_id; }
    inline void setLastID(const std::uint32_t value) noexcept { this->m_last_id = value; }

    inline const xr_string& getCurrentStateName(void) const noexcept { return this->m_current_state_name; }
    inline void setCurrentStateName(const xr_string& state_name) noexcept { this->m_current_state_name = state_name; }

    inline const xr_string& getTargetStateName(void) const noexcept { return this->m_target_state_name; }
    inline void setTargetStateName(const xr_string& target_name) noexcept
    {
        if (target_name.empty())
            Msg("[Scripts/StateManagerAnimationStates/setTargetStateName(target_name)] WARNING: target_name.empty() == "
                "true! You set an empty string");

        this->m_target_state_name = target_name;
    }

private:
    std::uint8_t m_animation_marker;
    std::uint32_t m_sequence_id;
    std::uint32_t m_next_random;
    std::uint32_t m_last_id;
    xr_string m_current_state_name;
    xr_string m_target_state_name;
};

struct StateManagerExtraData
{
    StateManagerExtraData(void) : m_is_fast_set(false) {}
    StateManagerExtraData(const bool is_fast_set, const Fvector& animation_position, const Fvector& animation_direction)
        : m_is_fast_set(is_fast_set), m_animation_position(animation_position),
          m_animation_direction(animation_direction)
    {
    }

    ~StateManagerExtraData(void) = default;

    inline bool isInitialize(void) const noexcept
    {
        return !(fis_zero(this->m_animation_direction.x) && fis_zero(this->m_animation_direction.y) &&
                   fis_zero(this->m_animation_direction.z)) &&
            !(fis_zero(this->m_animation_position.x) && fis_zero(this->m_animation_position.y) &&
                fis_zero(this->m_animation_position.z));
    }

    inline bool isFastSet(void) const noexcept { return this->m_is_fast_set; }
    inline void setFastSet(const bool value) noexcept { this->m_is_fast_set = value; }

    inline const Fvector& getAnimationPosition(void) const noexcept { return this->m_animation_position; }
    inline void setAnimationPosition(const Fvector& position) noexcept { this->m_animation_position = position; }

    inline const Fvector& getAnimationDirection(void) const noexcept { return this->m_animation_direction; }
    inline void setAnimationDirection(const Fvector& direction) noexcept { this->m_animation_direction = direction; }

private:
    bool m_is_fast_set;
    Fvector m_animation_position;
    Fvector m_animation_direction;
};

// @ Implementes state_mgr_animation_list and state_mgr_animstate_list
struct StateManagerAnimationData
{
    struct AnimationData
    {
        AnimationData(void) = default;
        ~AnimationData(void) = default;

        inline bool isNotInitialized(void) const
        {
            return (fis_zero(this->m_hit_power) && this->m_animation_name.empty() && this->m_sound_name.empty() &&
                this->m_attach_item_name.empty() && this->m_detach_item_name.empty() && !this->m_function);
        }

        inline float getHitPower(void) const noexcept { return this->m_hit_power; }
        inline void setHitPower(const float value) noexcept { this->m_hit_power = value; }

        inline const xr_string& getAnimationName(void) const noexcept { return this->m_animation_name; }
        inline void setAnimationName(const xr_string& animation_name) noexcept
        {
            if (animation_name.empty())
                Msg("[Scripts/StateManagerAnimationData/AnimationData/setAnimationName(animation_name)] WARNING: "
                    "animation_name.empty() == true! You set an empty string");

            this->m_animation_name = animation_name;
        }

        inline const xr_string& getSoundName(void) const noexcept { return this->m_sound_name; }
        inline void setSoundName(const xr_string& sound_name) noexcept
        {
            if (sound_name.empty())
                Msg("[Scripts/StateManagerAnimationData/AnimationData/setSoundName(sound_name)] WARNING: "
                    "sound_name.empty() == true! You set an empty string");

            this->m_sound_name = sound_name;
        }

        inline const xr_string& getAttachItemName(void) const noexcept { return this->m_attach_item_name; }
        inline void setAttachItemName(const xr_string& item_name) noexcept
        {
            if (item_name.empty())
                Msg("[Scripts/StateManagerAnimationData/AnimationData/setAttachItemName(item_name)] WARNING: "
                    "item_name.empty() == true! You set an empty string");

            this->m_attach_item_name = item_name;
        }

        inline const xr_string& getDetachItemName(void) const noexcept { return this->m_detach_item_name; }
        inline void setDetachItemName(const xr_string& item_name) noexcept
        {
            if (item_name.empty())
                Msg("[Scripts/StateManagerAnimationData/AnimationData/setDetachItemName(item_name)] WARNING: "
                    "item_name.empty() == true! You set an empty string");

            this->m_detach_item_name = item_name;
        }

        inline void CallFunction(CScriptGameObject* const p_client_object)
        {
            if (this->m_function)
                this->m_function(p_client_object);
            else
                Msg("[Scripts/StateManagerAnimationData/AnimationData/CallFunction(p_client_object)] WARNING: "
                    "m_function is nullptr! Can't call! Return ...");
        }

        inline void setFunction(const std::function<void(CScriptGameObject* const)>& function)
        {
            this->m_function = function;
        }

        inline bool isFunctionExist(void) const noexcept { return (!!this->m_function); }

    private:
        float m_hit_power = 0.0f;
        xr_string m_animation_name;
        xr_string m_sound_name;
        xr_string m_attach_item_name;
        xr_string m_detach_item_name;
        std::function<void(CScriptGameObject* const)> m_function = nullptr;
    };

    StateManagerAnimationData(void) = default;
    ~StateManagerAnimationData(void) = default;

    inline std::uint32_t getPropertiesMaxIdle(void) const noexcept { return this->m_properties_max_idle; }
    inline void setPropertiesMaxIdle(const std::uint32_t value) noexcept { this->m_properties_max_idle = value; }

    inline std::uint32_t getPropertiesSumIdle(void) const noexcept { return this->m_properties_sum_idle; }
    inline void setPropertiesSumIdle(const std::uint32_t value) noexcept { this->m_properties_sum_idle = value; }

    inline std::uint32_t getPropertiesRandom(void) const noexcept { return this->m_properties_random; }
    inline void setPropertiesRandom(const std::uint32_t value) noexcept { this->m_properties_random = value; }

    inline const xr_map<std::uint32_t, xr_vector<AnimationData>>& getAnimationList(
        const xr_string& animation_id_name) const
    {
        return this->m_data.at(animation_id_name);
    }

    inline bool isAnimationListExist(const xr_string& animation_id_name) const
    {
        if (animation_id_name.empty())
        {
            Msg("[Scripts/StateManagerAnimationData/isAnimationListExist(animation_id_name)] WARNING: "
                "animation_id_name.empty() == true! Return false ...");
            return false;
        }

        return (this->m_data.find(animation_id_name) != this->m_data.end());
    }

    inline void addAnimation(
        const xr_string& animation_state_name, const std::uint32_t index, const xr_string& animation_name) noexcept
    {
        if (animation_state_name.empty())
        {
            Msg("[Scripts/StateManagerAnimationData/addAnimation(animation_state_name, index, animation_name)] "
                "WARNING: "
                "animation_state_name.empty() == true! Can't add animation to m_data! Return ...");
            return;
        }

        if (animation_name.empty())
        {
            Msg("[Scripts/StateManagerAnimationData/addAnimation(animation_state_name, index, animation_name)] "
                "WARNING: "
                "animation_name.empty() == true! Can't add an empty animation name to m_data! Return ...");
            return;
        }

        AnimationData data;
        data.setAnimationName(animation_name);
        const xr_vector<AnimationData>& animation_list = this->m_data[animation_state_name][index];

        if (!animation_list.empty())
        {
            for (const AnimationData& data : animation_list)
            {
                if (data.getAnimationName() == animation_name)
                    Msg("[Scripts/StateManagerAnimationData/addAnimation(animation_state_name, index, animation_name)] "
                        "WARNING: You are trying to the same animation_name, which already existed in the list! %s %s",
                        animation_state_name.c_str(), animation_name.c_str());
            }
        }

        this->m_data[animation_state_name][index].push_back(data);
    }

    inline void addAnimationAttachItemName(
        const xr_string& animation_state_name, const std::uint32_t index, const xr_string& attach_item_name) noexcept
    {
        if (animation_state_name.empty())
        {
            Msg("[Scripts/StateManagerAnimationData/addAnimation(animation_state_name, index, attach_item_name)] "
                "WARNING: "
                "animation_state_name.empty() == true! Can't add animation to m_data! Return ...");
            return;
        }

        if (attach_item_name.empty())
        {
            Msg("[Scripts/StateManagerAnimationData/addAnimation(animation_state_name, index, attach_item_name)] "
                "WARNING: "
                "attach_item_name.empty() == true! Can't add an empty animation name to m_data! Return ...");
            return;
        }
        AnimationData data;
        data.setAttachItemName(attach_item_name);
        this->m_data[animation_state_name][index].push_back(data);
    }

    inline void addAnimationDettachItemName(
        const xr_string& animation_state_name, const std::uint32_t index, const xr_string& dettach_item_name) noexcept
    {
        if (animation_state_name.empty())
        {
            Msg("[Scripts/StateManagerAnimationData/addAnimation(animation_state_name, index, dettach_item_name)] "
                "WARNING: "
                "animation_state_name.empty() == true! Can't add animation to m_data! Return ...");
            return;
        }

        if (dettach_item_name.empty())
        {
            Msg("[Scripts/StateManagerAnimationData/addAnimation(animation_state_name, index, dettach_item_name)] "
                "WARNING: "
                "dettach_item_name.empty() == true! Can't add an empty animation name to m_data! Return ...");
            return;
        }
        AnimationData data;
        data.setDetachItemName(dettach_item_name);
        this->m_data[animation_state_name][index].push_back(data);
    }

    inline void addAnimationSoundName(
        const xr_string& animation_state_name, const std::uint32_t index, const xr_string& sound_name) noexcept
    {
        if (animation_state_name.empty())
        {
            Msg("[Scripts/StateManagerAnimationData/addAnimation(animation_state_name, index, sound_name)] "
                "WARNING: "
                "animation_state_name.empty() == true! Can't add animation to m_data! Return ...");
            return;
        }

        if (sound_name.empty())
        {
            Msg("[Scripts/StateManagerAnimationData/addAnimation(animation_state_name, index, sound_name)] "
                "WARNING: "
                "sound_name.empty() == true! Can't add an empty animation name to m_data! Return ...");
            return;
        }
        AnimationData data;
        data.setSoundName(sound_name);
        this->m_data[animation_state_name][index].push_back(data);
    }

    inline void addAnimationHitPower(
        const xr_string& animation_state_name, const std::uint32_t index, const float value) noexcept
    {
        if (animation_state_name.empty())
        {
            Msg("[Scripts/StateManagerAnimationData/addAnimation(animation_state_name, index, value)] "
                "WARNING: "
                "animation_state_name.empty() == true! Can't add animation to m_data! Return ...");
            return;
        }

        AnimationData data;
        data.setHitPower(value);
        this->m_data[animation_state_name][index].push_back(data);
    }

    inline void addAnimationFunction(const xr_string& animation_state_name, const std::uint32_t index,
        const std::function<void(CScriptGameObject* const)>& function) noexcept
    {
        if (animation_state_name.empty())
        {
            Msg("[Scripts/StateManagerAnimationData/addAnimation(animation_state_name, index, function)] "
                "WARNING: "
                "animation_state_name.empty() == true! Can't add animation to m_data! Return ...");
            return;
        }

        if (!function)
        {
            Msg("[Scripts/StateManagerAnimationData/addAnimation(animation_state_name, index, function)] "
                "WARNING: "
                "function == nullptr! Can't add an empty animation name to m_data! Return ...");
            return;
        }

        AnimationData data;
        data.setFunction(function);
        this->m_data[animation_state_name][index].push_back(data);
    }

    inline bool isPropertiesMoving(void) const noexcept { return this->m_is_properties_moving; }
    inline void setPropertiesMoving(const bool value) noexcept { this->m_is_properties_moving = value; }

    inline bool isPropertiesInitialized(void) const noexcept
    {
        return (!this->m_is_properties_moving && !this->m_properties_max_idle && !this->m_properties_random &&
            !this->m_properties_sum_idle);
    }
    inline bool isInitialized(void) const noexcept { return (!this->m_data.empty()); }

private:
    bool m_is_properties_moving = false;
    std::uint32_t m_properties_max_idle = 0;
    std::uint32_t m_properties_sum_idle = 0;
    std::uint32_t m_properties_random = 0;
    xr_map<xr_string, xr_map<std::uint32_t, xr_vector<AnimationData>>> m_data;
};

struct StateLibData
{
    StateLibData(void) = default;
    StateLibData(const std::uint32_t& movement_type, const std::uint32_t& mental_type,
        const std::uint32_t& bodystate_type, const std::uint32_t& direction_type, const xr_string& weapon_name,
        const xr_string& animstate_name, const xr_string& animation_name, bool is_fast_set = false,
        const std::uint16_t& weapon_slot = 0, bool is_special_danger_move = false)
        : m_movement_type(movement_type), m_mental_type(mental_type), m_bodystate_type(bodystate_type),
          m_direction_type(direction_type), m_weapon_name(weapon_name), m_animstate_name(animstate_name),
          m_animation_name(animation_name), m_is_special_danger_move(is_special_danger_move),
          m_is_fast_set(is_fast_set), m_weapon_slot(weapon_slot)
    {
    }

    ~StateLibData(void) = default;

    inline bool IsFastSet(void) const noexcept { return this->m_is_fast_set; }
    inline void setFastSet(const bool& value) noexcept { this->m_is_fast_set = value; }

    inline bool IsSpecialDangerMove(void) const noexcept { return this->m_is_special_danger_move; }
    inline void setSpecialDangerMove(const bool& value) noexcept { this->m_is_special_danger_move = value; }

    inline std::uint16_t getWeaponSlot(void) const noexcept { return this->m_weapon_slot; }
    inline void setWeaponSlot(const std::uint16_t& value) noexcept
    {
        if (value == Globals::kUnsignedInt16Undefined)
        {
            Msg("[StateLibData/setWeaponSlot(value)] WARNING: value = std::uint16_t(-1)! You are trying to set an "
                "undefined value! No assignment!");
            return;
        }

        this->m_weapon_slot = value;
    }

    inline std::uint32_t getMovementType(void) const noexcept { return this->m_movement_type; }
    inline void setMovementType(const std::uint32_t& value) noexcept
    {
        if (value == Globals::kUnsignedInt32Undefined)
        {
            Msg("[StateLibData/setMovementType(value)] WARNING: value = std::uint32_t(-1)!");
        }

        this->m_movement_type = value;
    }
    inline std::uint32_t getMentalType(void) const noexcept { return this->m_mental_type; }
    inline void setMentalType(const std::uint32_t& value) noexcept
    {
        if (value == Globals::kUnsignedInt32Undefined)
        {
            Msg("[StateLibData/setMentalType(value)] WARNING: value = std::uint32_t(-1)!");
        }

        this->m_mental_type = value;
    }
    inline std::uint32_t getBodyStateType(void) const noexcept { return this->m_bodystate_type; }
    inline void setBodyStateType(const std::uint32_t& value) noexcept
    {
        if (value == Globals::kUnsignedInt32Undefined)
        {
            Msg("[StateLibData/setBodyStateType(value)] WARNING: value = std::uint32_t(-1)!");
        }

        this->m_bodystate_type = value;
    }
    inline std::uint32_t getDirectionType(void) const noexcept { return this->m_direction_type; }
    inline void setDirectionType(const std::uint32_t& value) noexcept
    {
        if (value == Globals::kUnsignedInt32Undefined)
        {
            Msg("[StateLibData/setDirectionType(value)] WARNING: value = std::uint32_t(-1)!");
        }

        this->m_direction_type = value;
    }
    inline const xr_string& getWeaponTypeName(void) const noexcept { return this->m_weapon_name; }
    inline void setWeaponTypeName(const xr_string& string) noexcept
    {
        if (!string.size())
        {
            Msg("[StateLibData/setWeaponTypeName(string)] WARNING: string.size() = 0!");
        }

        this->m_weapon_name = string;
    }
    inline const xr_string& getAnimStateTypeName(void) const noexcept { return this->m_animstate_name; }
    inline void setAnimStateTypeName(const xr_string& string) noexcept
    {
        if (!string.size())
        {
            Msg("[StateLibData/setAnimStateTypeName(string)] WARNING: string.size() = 0!");
        }

        this->m_animstate_name = string;
    }
    inline const xr_string& getAnimationName(void) const noexcept { return this->m_animation_name; }
    inline void setAnimationName(const xr_string& string) noexcept
    {
        if (!string.size())
        {
            Msg("[StateLibData/setAnimationName(string)] WARNING: string.size() = 0!");
        }

        this->m_animation_name = string;
    }

private:
    bool m_is_special_danger_move;
    bool m_is_fast_set;
    std::uint16_t m_weapon_slot;
    std::uint32_t m_movement_type; // MonsterSpace::Enum*
    std::uint32_t m_mental_type; // MonsterSpace::Enum*
    std::uint32_t m_bodystate_type; // MonsterSpace::Enum*
    std::uint32_t m_direction_type; // SightManager::ESightType
    xr_string m_weapon_name;
    xr_string m_animstate_name;
    xr_string m_animation_name;
};

} // namespace Scripts
} // namespace Cordis
