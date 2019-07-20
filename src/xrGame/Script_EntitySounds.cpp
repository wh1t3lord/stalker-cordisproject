#include "stdafx.h"
#include "Script_EntitySounds.h"
#include <random>

namespace Cordis
{
namespace Scripts
{
Script_SoundNPC::Script_SoundNPC(const CInifile& sound_ini, const xr_string& section)
    : m_prefix(Globals::Utils::cfg_get_bool(&sound_ini, section, "npc_prefix", nullptr)),
      m_path(Globals::Utils::cfg_get_string(&sound_ini, section, "path")),
      m_shuffle(Globals::Utils::cfg_get_string(&sound_ini, section, "shuffle")),
      m_group_sound(Globals::Utils::cfg_get_bool(&sound_ini, section, "group_snd", nullptr)),
      m_play_always(Globals::Utils::cfg_get_bool(&sound_ini, section, "play_always", nullptr)),
      m_is_combat_sound(Globals::Utils::cfg_get_bool(&sound_ini, section, "is_combat_sound", nullptr)),
      m_section(section), m_played_id(0),
      m_delay_sound(Globals::Utils::cfg_get_number(&sound_ini, section, "delay_sound", nullptr)),
      m_can_play_group_sound(true), m_faction(Globals::Utils::cfg_get_string(&sound_ini, section, "faction")),
      m_point(Globals::Utils::cfg_get_string(&sound_ini, section, "point")),
      m_message(Globals::Utils::cfg_get_string(&sound_ini, section, "message")), m_min_idle(3), m_max_idle(5),
      m_random(100), m_played_time(0), m_idle_time(0)
{
    // Lord: не совсем доделано! Подебажить и спарсить!
    xr_string interval = Globals::Utils::cfg_get_string(&sound_ini, section, "idle");

    // Lord: не совсем доделано! Подебажить и спарсить!
    xr_string data_avail_communities = Globals::Utils::cfg_get_string(&sound_ini, section, "avail_communities");
}

Script_SoundNPC::~Script_SoundNPC(void)
{
    if (this->m_sound_object)
    {
        delete this->m_sound_object;
        this->m_sound_object = nullptr;
    }

    if (this->m_pda_sound_object)
    {
        delete this->m_pda_sound_object;
        this->m_pda_sound_object = nullptr;
    }
}

void Script_SoundNPC::reset(const std::uint16_t& npc_id)
{
    DataBase::Storage_Data& storage_data = DataBase::Storage::getInstance().getStorage()[npc_id];
    if (!storage_data.m_object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    this->m_played_id = 0;
    this->m_played_time = 0;
    this->m_can_play_group_sound = true;
    this->m_can_play_sound[npc_id] = true;
    storage_data.m_object->set_sound_mask(-1);
    storage_data.m_object->set_sound_mask(0);

    if (this->m_pda_sound_object)
    {
        this->m_pda_sound_object->Stop();
        delete this->m_pda_sound_object;
        this->m_pda_sound_object = nullptr;
    }
}

bool Script_SoundNPC::is_playing(const std::uint16_t& npc_id)
{
    DataBase::Storage_Data& storage_data = DataBase::Storage::getInstance().getStorage()[npc_id];
    if (!storage_data.m_object)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (storage_data.m_object->active_sound_count() != 0)
        return true;

    if (this->m_pda_sound_object)
        return this->m_pda_sound_object->IsPlaying();

    return false;
}

void Script_SoundNPC::initialize_npc(CScriptGameObject* npc)
{
    std::uint16_t npc_id = npc->ID();

    xr_string character_prefix;
    if (!this->m_prefix)
    {
        character_prefix = npc->sound_prefix();
        npc->sound_prefix("characters_voice\\");
    }

    this->m_npc[npc_id].first = generate_id();

    if (this->m_is_combat_sound)
    {
        this->m_npc[npc_id].second = npc->add_combat_sound(
            this->m_path.c_str(), NSTL, SOUND_TYPE_TALKING, 2, 1, this->m_npc[npc_id].first, "bip01_head");
        this->m_npc[npc_id].second -= 1;
    }
    else
    {
        this->m_npc[npc_id].second =
            npc->add_sound(this->m_path.c_str(), NSTL, SOUND_TYPE_TALKING, 2, 1, this->m_npc[npc_id].first);
        this->m_npc[npc_id].second -= 1;
    }
    xr_string _path = npc->sound_prefix();
    _path.append(this->m_path);
    _path.append(".ogg");

    if (FS.exist("$game_sounds$", _path.c_str()) == true)
    {
        this->m_sound_path[npc_id][0] = (xr_string(npc->sound_prefix()) + this->m_path);
    }
    else
    {
        std::uint32_t number = 1;
        xr_string _path2 = npc->sound_prefix();
        _path2.append(this->m_path);
        _path2.append(std::to_string(number));
        _path2.append(".ogg");
        while (FS.exist("$game_sounds$", _path2.c_str()))
        {
            this->m_sound_path[npc_id][number] =
                ((xr_string(npc->sound_prefix()) + this->m_path) + std::to_string(number).c_str());
            number += 1;
            _path2 = npc->sound_prefix();
            _path2.append(this->m_path);
            _path2.append(std::to_string(number));
            _path2.append(".ogg");
        }
    }

    if (this->m_npc[npc_id].second < 0)
    {
        Msg("[Script] ERROR: Couldn't find sounds %s with prefix %s", this->m_path.c_str(), npc->sound_prefix());
        R_ASSERT(false);
    }

    if (!this->m_prefix)
    {
        npc->sound_prefix(character_prefix.c_str());
    }

    if (this->m_group_sound)
    {
        this->m_can_play_group_sound = true;
    }
    else
    {
        if (this->m_can_play_sound[npc_id]) // Lord: вообще это нужно?
        {
            this->m_can_play_sound[npc_id] = true;
        }
    }
}

void Script_SoundNPC::callback(const std::uint16_t& npc_id)
{
    this->m_played_time = Device.dwTimeGlobal;
    // LorD: нормально ли генерирует рандомное значение простетировать
    std::random_device random_device;
    std::mt19937 range(random_device);
    std::uniform_int_distribution<int> urandom(this->m_min_idle, this->m_max_idle);

    this->m_idle_time = urandom(range);

    if (this->m_group_sound)
        this->m_can_play_group_sound = true;
    else
        this->m_can_play_sound[npc_id] = true;

    CurrentGameUI()->RemoveCustomStatic("cs_subtitles_npc");

    DataBase::Storage_Data& storage_data = DataBase::Storage::getInstance().getStorage()[npc_id];

    if (!storage_data.m_active_scheme.size())
    {
        return;
    }

    if (!storage_data[storage_data.m_active_scheme].size())
    {
        return;
    }

    if (this->m_played_id == this->m_npc[npc_id].second && this->m_shuffle != "rnd")
    {
        storage_data[storage_data.m_active_scheme]["theme_end"] = true;
        storage_data[storage_data.m_active_scheme]["sound_end"] = true;
    }
    else
    {
        storage_data[storage_data.m_active_scheme]["sound_end"] = true;
    }
}

bool Script_SoundNPC::play(const std::uint16_t& npc_id, xr_string& faction, std::uint16_t point) { return false; }

bool Script_SoundNPC::play(const std::uint16_t& obj_id) { return false; }

int Script_SoundNPC::select_next_sound(const xr_string& npc_id) { return 0; }

void Script_SoundNPC::stop(const std::uint16_t& obj_id) {}

void Script_SoundNPC::save(const NET_Packet& packet) {}

void Script_SoundNPC::load(const NET_Packet& packet) {}

void Script_SoundNPC::save_npc(const NET_Packet& packet, const xr_string& npc_id) {}

void Script_SoundNPC::load_npc(const NET_Packet& packet, const xr_string& npc_id) {}

void Script_SoundNPC::init_npc(CScriptGameObject* npc) {}

} // namespace Scripts
} // namespace Cordis
