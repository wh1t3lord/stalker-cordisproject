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
    if (!npc)
    {
        R_ASSERT2(false, "object in null!");
        return;
    }

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

bool Script_SoundNPC::play(const std::uint16_t& npc_id, xr_string& faction, std::uint16_t point)
{
    CScriptGameObject* npc = DataBase::Storage::getInstance().getStorage()[npc_id].m_object;
    if (!npc)
    {
        R_ASSERT2(false, "Object is null!");
        return false;
    }

    if (this->m_group_sound)
    {
        if (!this->m_can_play_group_sound)
        {
            return false;
        }
        else
        {
            if (!this->m_can_play_sound[npc_id])
            {
                return false;
            }
        }
    }

    if (!this->m_played_time && ((Device.dwTimeGlobal - this->m_played_time) < this->m_idle_time))
    {
        return false;
    }

    this->m_played_time = 0;

    this->m_played_id = this->select_next_sound(npc_id);

    if (this->m_played_id == -1)
        return false;

    if (npc)
        npc->play_sound(this->m_npc[npc_id].first, this->m_delay_sound + 0.06f, this->m_delay_sound + 0.05f, 1, 0,
            this->m_played_id);

    std::uint32_t table_id = this->m_played_id + 1;
    xr_string& sound_name = this->m_sound_path[npc_id][table_id];

    if (sound_name.size())
    {
        sound_name.append("_pda.ogg");
        if (FS.exist("$game_sounds$", sound_name.c_str()) &&
            (npc->Position().distance_to_sqr(DataBase::Storage::getInstance().getActor()->Position()) >= 100))
        {
            if (this->m_pda_sound_object)
            {
                if (this->m_pda_sound_object->IsPlaying())
                {
                    this->m_pda_sound_object->Stop();
                }
            }

            this->m_pda_sound_object = new CScriptSound(sound_name.c_str());
            this->m_pda_sound_object->SetVolume(0.8f);
            this->m_pda_sound_object->PlayAtPos(
                DataBase::Storage::getInstance().getActor(), Fvector().set(0, 0, 0), this->m_delay_sound, sm_2D);
        }
    }

    // Lord: проверить
    xr_string& replaced_string = sound_name.replace(sound_name.begin(), sound_name.end(), '\\', '_');

    if (this->m_group_sound)
        this->m_can_play_group_sound = true;
    else
        this->m_can_play_sound[npc_id] = true;
    xr_string translated_string = Globals::Game::translate_string(replaced_string.c_str());
    if (translated_string != replaced_string)
    {
        if (!this->m_faction.size())
        {
            this->m_faction = Globals::character_community(npc);
        }

        if (!this->m_point.size())
        {
            this->m_point = npc->ProfileName();
            this->m_point.append("_name");
            xr_string translated = Globals::Game::translate_string(this->m_point.c_str());
            if (translated.size())
            {
                if (translated == this->m_point)
                {
                    this->m_point = "";
                }
            }
        }
        Script_NewsManager::getInstance().SendSound(
            npc, this->m_faction, this->m_point, sound_name, replaced_string, this->m_delay_sound);
    }
    else
    {
        Script_NewsManager::getInstance().SendSound(
            npc, this->m_faction, this->m_point, sound_name, "", this->m_delay_sound);
    }

    return true;
}

bool Script_SoundNPC::play(const std::uint16_t& obj_id) { return false; }

int Script_SoundNPC::select_next_sound(const std::uint16_t& npc_id)
{
    if (this->m_shuffle != "rnd")
    {
        if (!this->m_npc[npc_id].second)
        {
            return 0;
        }

        std::random_device random_device1;
        std::mt19937 range1(random_device1);
        std::uniform_int_distribution<int> urandom1(0, this->m_npc[npc_id].second);
        if (!this->m_played_id)
        {
            std::random_device random_device;
            std::mt19937 range(random_device);
            std::uniform_int_distribution<int> urandom(0, this->m_npc[npc_id].second - 1);
            int played_id = urandom(range);

            if (played_id >= this->m_played_id)
            {
                return played_id + 1;
            }

            return played_id;
        }

        return urandom1(range1);
    }

    if (this->m_shuffle == "seq")
    {
        if (this->m_played_id == -1)
        {
            return -1;
        }

        if (this->m_played_id < this->m_npc[npc_id].second)
        {
            return this->m_played_id + 1;
        }

        return -1;
    }

    if (this->m_shuffle == "loop")
    {
        if (this->m_played_id < this->m_npc[npc_id].second)
        {
            return this->m_played_id + 1;
        }
    }

    return 0;
}

void Script_SoundNPC::stop(const std::uint16_t& obj_id)
{
    CScriptGameObject* npc = DataBase::Storage::getInstance().getStorage()[obj_id].m_object;

    if (!npc)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    if (npc->Alive())
    {
        npc->set_sound_mask(-1);
        npc->set_sound_mask(0);
    }

    if (this->m_pda_sound_object)
    {
        if (this->m_pda_sound_object->IsPlaying())
        {
            this->m_pda_sound_object->Stop();
            delete this->m_pda_sound_object;
            this->m_pda_sound_object = nullptr;
        }
    }
}

void Script_SoundNPC::save(NET_Packet& packet)
{
    packet.w_stringZ((std::to_string(this->m_played_id).c_str()));

    if (this->m_group_sound)
    {
        packet.w_u8(1);
    }
}

void Script_SoundNPC::load(NET_Packet& packet)
{
    shared_str id;
    packet.r_stringZ(id);

    // Lord: протестировать здесь!
    if (id.size())
        this->m_played_id = atoi(id.c_str());
    else
        this->m_played_id = 0;

    if (this->m_group_sound)
        this->m_can_play_group_sound = (!!packet.r_u8());
}

void Script_SoundNPC::save_npc(NET_Packet& packet, const std::uint16_t& npc_id)
{
    if (!this->m_group_sound)
        packet.w_u8((this->m_can_play_sound[npc_id] == true) ? 1 : 0);
}

void Script_SoundNPC::load_npc(NET_Packet& packet, const std::uint16_t& npc_id)
{
    if (!this->m_group_sound)
        this->m_can_play_sound[npc_id] = (!!packet.r_u8());
}

Script_SoundActor::Script_SoundActor(const CInifile& ini, const xr_string& section)
    : m_is_stereo(Globals::Utils::cfg_get_bool(&ini, section, "actor_stereo", nullptr)),
      m_is_prefix(Globals::Utils::cfg_get_bool(&ini, section, "npc_prefix", nullptr)),
      m_path(Globals::Utils::cfg_get_string(&ini, section, "path")),
      m_shuffle(Globals::Utils::cfg_get_string(&ini, section, "play_always")), m_section(section), m_played_id(0),
      m_can_play_sound(true), m_sound_object(nullptr),
      m_faction(Globals::Utils::cfg_get_string(&ini, section, "faction")),
      m_point(Globals::Utils::cfg_get_string(&ini, section, "point")),
      m_message(Globals::Utils::cfg_get_string(&ini, section, "message"))
{
    // Lord: Реализовать парсинг
    xr_string iterval = Globals::Utils::cfg_get_string(&ini, section, "idle");

    if (FS.exist("$game_sounds$", (this->m_path + ".ogg").c_str()))
    {
        this->m_sound[0] = this->m_path;
    }
    else
    {
        std::uint16_t id = 1;
        while (FS.exist("$game_sounds$", ((this->m_path + std::to_string(id).c_str()) + ".ogg").c_str()))
        {
            this->m_sound[id - 1] = this->m_path + std::to_string(id).c_str();
            ++id;
        }
    }

    if (!this->m_sound.size())
    {
        R_ASSERT2(false, ("There are no sound collection with path: " + this->m_path).c_str());
    }
}

Script_SoundActor::~Script_SoundActor(void)
{
    if (this->m_sound_object)
    {
        delete this->m_sound_object;
        this->m_sound_object = nullptr;
    }
}

void Script_SoundActor::reset(const std::uint16_t& npc_id) {}

bool Script_SoundActor::is_playing(const std::uint16_t& npc_id)
{
    if (this->m_sound_object)
        return this->m_sound_object->IsPlaying();

    return false;
}

void Script_SoundActor::callback(const std::uint16_t& npc_id)
{
    this->m_played_time = Device.dwTimeGlobal;
    std::random_device random_device;
    std::mt19937 range(random_device);
    std::uniform_int_distribution<int> urandom(this->m_min_idle, this->m_max_idle);

    this->m_idle_time = urandom(range);
    if (this->m_sound_object)
    {
        delete this->m_sound_object;
        this->m_sound_object = nullptr;
    }

    this->m_can_play_sound = true;

    CurrentGameUI()->RemoveCustomStatic("cs_subtitles_actor");

    DataBase::Storage_Data& storage_data = DataBase::Storage::getInstance().getStorage()[npc_id];

    if (!storage_data.m_active_scheme.size())
        return;

    if (!storage_data[storage_data.m_active_scheme].size())
        return;

    // Lord: проверить больше или всё же оно равно этому значению (про размер карты)
    if (this->m_played_id == this->m_sound.size() && (this->m_shuffle != "rnd"))
    {
        Msg("[Script_SoundActor] -> [%s] signalled 'theme_end' in section [%s]", std::to_string(npc_id).c_str(),
            storage_data.m_active_section.c_str());
        storage_data[storage_data.m_active_scheme]["theme_end"] = true;
        storage_data[storage_data.m_active_scheme]["sound_end"] = true;
    }
    else
    {
        Msg("[Script_SoundActor] -> [%s] signalled 'sound_end' in section [%s]", std::to_string(npc_id).c_str(),
            storage_data.m_active_section.c_str());

        storage_data[storage_data.m_active_scheme]["sound_end"] = true;
    }
}

bool Script_SoundActor::play(const std::uint16_t& npc_id, xr_string& faction, std::uint16_t point)
{
    if (!this->m_can_play_sound)
    {
        Msg("[Script_SoundActor] -> this->m_can_play_sound == false!");
        return false;
    }

    if (!this->m_played_time && (Device.dwTimeGlobal - this->m_played_time < this->m_idle_time))
    {
        return false;
    }

    this->m_played_time = 0;
    this->m_played_id = this->select_next_sound();
    // Lord: доделать
    return true;
}

int Script_SoundActor::select_next_sound(const std::uint16_t& npc_id) { return 0; }

void Script_SoundActor::stop(const std::uint16_t& obj_id) {}

void Script_SoundActor::save(NET_Packet& packet) {}

void Script_SoundActor::load(NET_Packet& packet) {}

} // namespace Scripts
} // namespace Cordis
