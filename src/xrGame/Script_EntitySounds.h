#pragma once

#include "../xrCore/xrCore.h"
#include "ai/stalker/ai_stalker_space.h"
#include "script_sound.h"

namespace Cordis
{
namespace Scripts
{
constexpr std::uint16_t NSTL = 64;
std::uint32_t sounds_base = StalkerSpace::EStalkerSounds::eStalkerSoundScript + 10000;

std::uint32_t generate_id(void)
{
    sounds_base += 1;
    return sounds_base - 1;
}

class IScript_Sound
{
public:
    virtual ~IScript_Sound(void) = default;

    virtual void reset(const std::uint16_t& npc_id) = 0;
    virtual bool is_playing(const std::uint16_t& npc_id) = 0;
    virtual void initialize_npc(CScriptGameObject* npc) = 0;
    virtual void callback(const std::uint16_t& npc_id) = 0;
    virtual bool play(const std::uint16_t& npc_id, xr_string& faction, std::uint16_t point) = 0;
    virtual bool play(const std::uint16_t& obj_id) = 0;
    virtual int select_next_sound(const xr_string& npc_id) = 0;
    virtual void stop(const std::uint16_t& obj_id) = 0;
    virtual void save(const NET_Packet& packet) = 0;
    virtual void load(const NET_Packet& packet) = 0;
    virtual void save_npc(const NET_Packet& packet, const xr_string& npc_id) = 0;
    virtual void load_npc(const NET_Packet& packet, const xr_string& npc_id) = 0;
    virtual void init_npc(CScriptGameObject* npc) = 0;
};

class Script_SoundNPC : public IScript_Sound
{
public:
    Script_SoundNPC(const CInifile& sound_ini, const xr_string& section);
    virtual ~Script_SoundNPC(void);

    void reset(const std::uint16_t& npc_id);
    bool is_playing(const std::uint16_t& npc_id);
    void initialize_npc(CScriptGameObject* npc);
    void callback(const std::uint16_t& npc_id);
    bool play(const std::uint16_t& npc_id, xr_string& faction, std::uint16_t point);
    bool play(const std::uint16_t& obj_id);
    int select_next_sound(const xr_string& npc_id);
    void stop(const std::uint16_t& obj_id);
    void save(const NET_Packet& packet);
    void load(const NET_Packet& packet);
    void save_npc(const NET_Packet& packet, const xr_string& npc_id);
    void load_npc(const NET_Packet& packet, const xr_string& npc_id);
    void init_npc(CScriptGameObject* npc);

private: // Lord: добавить карты
    bool m_prefix;
    bool m_play_always;
    bool m_is_combat_sound;
    bool m_can_play_group_sound;
    bool m_group_sound;
    std::uint32_t m_played_time;
    int m_idle_time;
    int m_min_idle;
    int m_max_idle;
    int m_random;
    int m_played_id;
    float m_delay_sound;
    xr_map<std::uint16_t, bool> m_can_play_sound;
    // @ m_npc[some_value]{first = id | second = max}
    xr_map<std::uint16_t, std::pair<std::uint32_t, std::uint32_t>> m_npc; 
    xr_map<xr_string, bool> m_avail_communities;
    xr_map<std::uint16_t, xr_map<std::uint16_t, xr_string>> m_sound_path;
    xr_string m_faction;
    xr_string m_point;
    xr_string m_message;
    xr_string m_section;
    xr_string m_path;
    xr_string m_shuffle;
    xr_string m_class_id;
    CScriptSound* m_sound_object;
    CScriptSound* m_pda_sound_object;
};
} // namespace Scripts
} // namespace Cordis
