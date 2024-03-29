#include "stdafx.h"
#include "Script_EntitySounds.h"
#include <random>

/*std::uint32_t Cordis::Scripts::sounds_base = ;*/

namespace Cordis
{
namespace Scripts
{
Script_SoundNPC::Script_SoundNPC(CScriptIniFile& sound_ini, const xr_string& section)
    : m_prefix(Globals::Utils::cfg_get_bool(&sound_ini, section, "npc_prefix")),
      m_path(Globals::Utils::cfg_get_string(&sound_ini, section, "path")),
      m_shuffle(Globals::Utils::cfg_get_string(&sound_ini, section, "shuffle")),
      m_group_sound(Globals::Utils::cfg_get_bool(&sound_ini, section, "group_snd")),
      m_is_play_always(Globals::Utils::cfg_get_bool(&sound_ini, section, "play_always")),
      m_is_combat_sound(Globals::Utils::cfg_get_bool(&sound_ini, section, "is_combat_sound")), m_section(section),
      m_played_id(0), m_delay_sound(Globals::Utils::cfg_get_number(&sound_ini, section, "delay_sound")),
      m_can_play_group_sound(true), m_faction(Globals::Utils::cfg_get_string(&sound_ini, section, "faction")),
      m_point(Globals::Utils::cfg_get_string(&sound_ini, section, "point")),
      m_message(Globals::Utils::cfg_get_string(&sound_ini, section, "message")), m_min_idle(3.0f), m_max_idle(5.0f),
      m_random(100), m_played_time(0), m_idle_time(0), m_class_id(SCRIPTSOUNDTYPE_NPC)
{
    
    xr_vector<xr_string> interval = Globals::Utils::parse_names(Globals::Utils::cfg_get_string(&sound_ini, section, "idle"));
    if (interval.size() > 0)
        this->m_min_idle = boost::lexical_cast<float>(interval[0]);

    if (interval.size() >= 1)
        this->m_max_idle = boost::lexical_cast<float>(interval[1]);

    if (interval.size() >= 2)
        this->m_random = boost::lexical_cast<int>(interval[2]);

    xr_string data_avail_communities = Globals::Utils::cfg_get_string(&sound_ini, section, "avail_communities");
    xr_vector<xr_string> avail_communities = Globals::Utils::parse_names(data_avail_communities);

    for (const xr_string& it : avail_communities)
        this->m_avail_communities[it] = true;

    if (this->m_shuffle.empty())
        this->m_shuffle = "rnd";
}

Script_SoundNPC::~Script_SoundNPC(void)
{
    if (this->m_sound_object)
    {
    //    delete this->m_sound_object;
        this->m_sound_object = nullptr;
    }

    if (this->m_pda_sound_object)
    {
     //   delete this->m_pda_sound_object;
        this->m_pda_sound_object = nullptr;
    }
}

void Script_SoundNPC::reset(const std::uint16_t& npc_id)
{
    const DataBase::Storage_Data& storage_data = DataBase::Storage::getInstance().getStorage().at(npc_id);
    if (!storage_data.getClientObject())
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    this->m_played_id = 0;
    this->m_played_time = 0;
    this->m_can_play_group_sound = true;
    this->m_can_play_sound[npc_id] = true;
    storage_data.getClientObject()->set_sound_mask(-1);
    storage_data.getClientObject()->set_sound_mask(0);

    if (this->m_pda_sound_object)
    {
        this->m_pda_sound_object->Stop();
        delete this->m_pda_sound_object;
        this->m_pda_sound_object = nullptr;
    }
}

bool Script_SoundNPC::is_playing(const std::uint16_t& npc_id)
{
    const DataBase::Storage_Data& storage_data = DataBase::Storage::getInstance().getStorage().at(npc_id);
    if (!storage_data.getClientObject())
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (storage_data.getClientObject()->active_sound_count() != 0)
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

void Script_SoundNPC::callback(const std::uint16_t npc_id)
{
    this->m_played_time = Device.dwTimeGlobal;

    this->m_idle_time = Globals::Script_RandomFloat::getInstance().Generate(this->m_min_idle, this->m_max_idle);

    if (this->m_group_sound)
        this->m_can_play_group_sound = true;
    else
        this->m_can_play_sound[npc_id] = true;

    CurrentGameUI()->RemoveCustomStatic("cs_subtitles_npc");

    const DataBase::Storage_Data& storage_data = DataBase::Storage::getInstance().getStorage().at(npc_id);

    if (storage_data.getActiveSchemeName().empty())
    {
        MESSAGEWR("ActiveScheme is empty return ...");
        return;
    }

    if (storage_data.getSchemes().at(storage_data.getActiveSchemeName())->getSignals().empty())
    {
        MESSAGEWR("Signals is empty return ...");
        return;
    }

    if (this->m_played_id == this->m_npc[npc_id].second && this->m_shuffle != "rnd")
    {
        //   storage_data[storage_data.m_active_scheme].getSignals()["theme_end"] = true;
        //   storage_data[storage_data.m_active_scheme].getSignals()["sound_end"] = true;
        DataBase::Storage::getInstance().setStorageSignal(npc_id, "theme_end", true);
        DataBase::Storage::getInstance().setStorageSignal(npc_id, "sound_end", true);
    }
    else
    {
        //   storage_data[storage_data.m_active_scheme].getSignals()["sound_end"] = true;
        DataBase::Storage::getInstance().setStorageSignal(npc_id, "sound_end", true);
    }
}

bool Script_SoundNPC::play(const std::uint16_t& npc_id, xr_string& faction, std::uint16_t point)
{
    CScriptGameObject* npc = DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject();
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

    if (this->m_played_time && ((Device.dwTimeGlobal - this->m_played_time) < this->m_idle_time))
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

    if (sound_name.empty() == false)
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

    if (this->m_group_sound)
        this->m_can_play_group_sound = true;
    else
        this->m_can_play_sound[npc_id] = true;


    return true;
}

// bool Script_SoundNPC::play(const std::uint16_t& obj_id) { return false; }

int Script_SoundNPC::select_next_sound(const std::uint16_t& npc_id)
{
    if (this->m_shuffle != "rnd")
    {
        if (!this->m_npc[npc_id].second)
        {
            return 0;
        }

        //         std::random_device random_device1;
        //         std::mt19937 range1(random_device1);
        //         std::uniform_int_distribution<int> urandom1(0, this->m_npc[npc_id].second);
        if (!this->m_played_id)
        {
            //             std::random_device random_device;
            //             std::mt19937 range(random_device);
            //             std::uniform_int_distribution<int> urandom(0, this->m_npc[npc_id].second - 1);
            int played_id = Globals::Script_RandomInt::getInstance().Generate(
                static_cast<std::uint32_t>(0), this->m_npc[npc_id].second - 1);

            if (played_id >= this->m_played_id)
            {
                return played_id + 1;
            }

            return played_id;
        }

        return Globals::Script_RandomInt::getInstance().Generate(
            static_cast<std::uint32_t>(0), this->m_npc[npc_id].second);
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
    CScriptGameObject* npc = DataBase::Storage::getInstance().getStorage().at(obj_id).getClientObject();

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

Script_SoundActor::Script_SoundActor(CScriptIniFile& ini, const xr_string& section)
    : m_is_stereo(Globals::Utils::cfg_get_bool(&ini, section, "actor_stereo")),
      m_is_prefix(Globals::Utils::cfg_get_bool(&ini, section, "npc_prefix")),
      m_path(Globals::Utils::cfg_get_string(&ini, section, "path")),
      m_shuffle(Globals::Utils::cfg_get_string(&ini, section, "shuffle")),
      m_is_play_always(Globals::Utils::cfg_get_bool(&ini, section, "play_always")),
      m_section(section), m_played_id(0),
      m_can_play_sound(true), m_sound_object(nullptr),
      m_faction(Globals::Utils::cfg_get_string(&ini, section, "faction")),
      m_point(Globals::Utils::cfg_get_string(&ini, section, "point")),
      m_message(Globals::Utils::cfg_get_string(&ini, section, "message")), m_class_id(SCRIPTSOUNDTYPE_ACTOR)
{
    if (this->m_shuffle.empty())
        this->m_shuffle = "rnd";

    xr_vector<xr_string> iterval = Globals::Utils::parse_names(Globals::Utils::cfg_get_string(&ini, section, "idle"));

    if (iterval.size() > 0)
        this->m_min_idle = boost::lexical_cast<float>(iterval[0]);
    if (iterval.size() >= 1)
        this->m_max_idle = boost::lexical_cast<float>(iterval[1]);
    if (iterval.size() >= 2)
        this->m_random = boost::lexical_cast<int>(iterval[2]);

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
       // delete this->m_sound_object;
        this->m_sound_object = nullptr;
    }
}

void Script_SoundActor::reset(const std::uint16_t& npc_id)
{
    this->m_played_id = 0;
    this->m_played_time = 0;
}

bool Script_SoundActor::is_playing(const std::uint16_t& npc_id)
{
    if (this->m_sound_object)
        return this->m_sound_object->IsPlaying();

    return false;
}

void Script_SoundActor::callback(const std::uint16_t npc_id)
{
    this->m_played_time = Device.dwTimeGlobal;
    this->m_idle_time = Globals::Script_RandomFloat::getInstance().Generate(this->m_min_idle, this->m_max_idle);
    if (this->m_sound_object)
    {
        delete this->m_sound_object;
        this->m_sound_object = nullptr;
    }

    this->m_can_play_sound = true;

    CurrentGameUI()->RemoveCustomStatic("cs_subtitles_actor");

    const DataBase::Storage_Data& storage_data = DataBase::Storage::getInstance().getStorage().at(npc_id);

    if (storage_data.getActiveSchemeName().empty())
    {
        Msg("[Scripts/Script_SoundActor/callback(npc_id)] active scheme is empty return ...");
        return;
    }


    if (storage_data.getSchemes().at(storage_data.getActiveSchemeName())->getSignals().empty())
    {
        MESSAGEWR("signals is empty return ...");
        return;
    }

    // Lord: проверить больше или всё же оно равно этому значению (про размер карты)
    if (this->m_played_id == this->m_sound.size() && (this->m_shuffle != "rnd"))
    {
        MESSAGEI("[%s] signalled 'theme_end' in section [%s]", std::to_string(npc_id).c_str(),
            storage_data.getActiveSectionName().c_str());
        //    storage_data[storage_data.m_active_scheme].getSignals()["theme_end"] = true;
        //    storage_data[storage_data.m_active_scheme].getSignals()["sound_end"] = true;
        DataBase::Storage::getInstance().setStorageSignal(npc_id, "theme_end", true);
        DataBase::Storage::getInstance().setStorageSignal(npc_id, "sound_end", true);
    }
    else
    {
        MESSAGEI("[%s] signalled 'sound_end' in section [%s]", std::to_string(npc_id).c_str(),
            storage_data.getActiveSectionName().c_str());

        //  storage_data[storage_data.m_active_scheme].getSignals()["sound_end"] = true;
        DataBase::Storage::getInstance().setStorageSignal(npc_id, "sound_end", true);
    }
}

bool Script_SoundActor::play(const std::uint16_t& npc_id, xr_string& faction, std::uint16_t point)
{
    if (!this->m_can_play_sound)
    {
        MESSAGEWR("this->m_can_play_sound == false!");
        return false;
    }

    if (this->m_played_time && (Device.dwTimeGlobal - this->m_played_time < this->m_idle_time))
    {
        return false;
    }

    this->m_played_time = 0;
    this->m_played_id = this->select_next_sound();

    if (this->m_played_id == Globals::kUnsignedInt32Undefined)
        return false;

    xr_string sound_name;
    if (this->m_played_id < this->m_sound.size())
        sound_name = this->m_sound.at(this->m_played_id);
    else if (this->m_played_id == this->m_sound.size())
        sound_name = this->m_sound.at(this->m_played_id - 1);
    else
        R_ASSERT2(false, "can't be!");

    this->m_sound_object = new CScriptSound(sound_name.c_str());
    this->m_sound_object->SetVolume(0.8f);
    this->m_sound_object->PlayAtPos(DataBase::Storage::getInstance().getActor(), Fvector().set(0, 0, 0), 0.0f, sm_2D);
    this->m_sound_object->SetVolume(0.8f);
    this->m_can_play_sound = false;

    return true;
}

int Script_SoundActor::select_next_sound(const std::uint16_t& npc_id)
{
    size_t sound_map_size = this->m_sound.size();

    if (this->m_shuffle == "rnd")
    {
        if (sound_map_size == 1)
        {
            return 1;
        }

        if (this->m_played_id)
        {
            size_t generated_value = Globals::Script_RandomInt::getInstance().Generate<size_t>(1, sound_map_size - 1);

            if (generated_value >= this->m_played_id)
                return generated_value + 1;

            return generated_value;
        }

        size_t generated_value = Globals::Script_RandomInt::getInstance().Generate<size_t>(1, sound_map_size);

        return generated_value;
    }

    if (this->m_shuffle == "seq")
    {
        if (this->m_played_id == -1)
            return Globals::kUnsignedInt32Undefined;

        if (this->m_played_id == Globals::kUnsignedInt32Undefined)
            return 1;

        if (this->m_played_id < sound_map_size)
            return this->m_played_id + 1;

        return Globals::kUnsignedInt32Undefined;
    }

    if (this->m_shuffle == "loop")
    {
        if (this->m_played_id == Globals::kUnsignedInt32Undefined)
            return 1;

        if (this->m_played_id < sound_map_size)
            return this->m_played_id + 1;

        return 1;
    }

    // Lord: хз гарантировано он будет проходить все ифы но какое значение здесь должен возвращать по тестировать!!!
    R_ASSERT(false);
    return Globals::kUnsignedInt32Undefined;
}

void Script_SoundActor::stop(const std::uint16_t& obj_id)
{
    if (this->m_sound_object)
        this->m_sound_object->Stop();
}

void Script_SoundActor::save(NET_Packet& packet) { packet.w_stringZ(std::to_string(this->m_played_id).c_str()); }

void Script_SoundActor::load(NET_Packet& packet)
{
    xr_string id;
    packet.r_stringZ(id);

    if (id != "nil") // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
        this->m_played_id = atoi(id.c_str());
    else
        this->m_played_id = 0;
}

Script_SoundObject::Script_SoundObject(CScriptIniFile& ini, const xr_string& section)
    : m_class_id(SCRIPTSOUNDTYPE_OBJECT), m_path(Globals::Utils::cfg_get_string(&ini, section, "path")),
      m_shuffle(Globals::Utils::cfg_get_string(&ini, section, "shuffle")), m_can_play_sound(true), m_played_id(0),
      m_played_time(0), m_faction(Globals::Utils::cfg_get_string(&ini, section, "faction")),
      m_point(Globals::Utils::cfg_get_string(&ini, section, "point")),
      m_message(Globals::Utils::cfg_get_string(&ini, section, "message")), m_section(section), m_sound_object(nullptr),
      m_idle_time(0), m_pda_sound_object(nullptr)
{
    if(this->m_shuffle.empty())
        this->m_shuffle = "rnd";

    xr_vector<xr_string> to_parse_numbers = Globals::Utils::parse_names(Globals::Utils::cfg_get_string(&ini, section, "idle"));

    if (to_parse_numbers.size() > 0)
        this->m_min_idle = boost::lexical_cast<float>(to_parse_numbers[0]);
    if (to_parse_numbers.size() >= 1)
        this->m_max_idle = boost::lexical_cast<float>(to_parse_numbers[1]);
    if (to_parse_numbers.size() >= 2)
        this->m_random = boost::lexical_cast<int>(to_parse_numbers[2]);

    if (FS.exist("$game_sounds$", (this->m_path + ".ogg").c_str()))
    {
        this->m_sound[0] = this->m_path;
    }
    else
    {
        size_t id = 1;
        while (FS.exist("$game_sounds$", ((this->m_path + std::to_string(id).c_str()) + ".ogg").c_str()))
        {
            this->m_sound[id] = this->m_path + std::to_string(id).c_str();
            ++id;
        }
    }

    if (!this->m_sound.size())
    {
        R_ASSERT2(false, ("There are no sound collection with path: " + this->m_path).c_str());
    }
}

Script_SoundObject::~Script_SoundObject(void)
{
    if (this->m_sound_object)
    {
      //  delete this->m_sound_object;
        this->m_sound_object = nullptr;
    }
}

bool Script_SoundObject::is_playing(const std::uint16_t& npc_id)
{
    if (this->m_sound_object)
        return this->m_sound_object->IsPlaying();

    return false;
}

void Script_SoundObject::callback(const std::uint16_t npc_id)
{
    this->m_played_time = Device.dwTimeGlobal;
    if (this->m_sound_object)
    {
        delete this->m_sound_object;
        this->m_sound_object = nullptr;
    }
    //     std::random_device random_device;
    //     std::mt19937 range(random_device);
    //     std::uniform_int_distribution<int> urandom(this->m_min_idle, this->m_max_idle);
    this->m_idle_time = Globals::Script_RandomFloat::getInstance().Generate(this->m_min_idle, this->m_max_idle) * 1000;
    this->m_can_play_sound = true;

    CurrentGameUI()->RemoveCustomStatic("cs_subtitles_object");

    const DataBase::Storage_Data& storage_data = DataBase::Storage::getInstance().getStorage().at(npc_id);
    if (storage_data.getActiveSchemeName().empty())
    {
        Msg("[Scripts/Script_SoundObject/callback(npc_id)] active scheme is empty return ...");
        return;
    }

    if (storage_data.getSchemes().at(storage_data.getActiveSchemeName())->getSignals().empty())
    {
        Msg("[Scripts/Script_SoundObject/callback(npc_id)] signals is empty return ...");
        return;
    }

    if (this->m_played_id == this->m_sound.size() && this->m_shuffle != "rnd")
    {
        DataBase::Storage::getInstance().setStorageSignal(npc_id, "theme_end", true);
        DataBase::Storage::getInstance().setStorageSignal(npc_id, "sound_end", true);
    }
    else
    {
        DataBase::Storage::getInstance().setStorageSignal(npc_id, "sound_end", true);
    }
}

bool Script_SoundObject::play(const std::uint16_t& npc_id, xr_string& faction, std::uint16_t point)
{
    CScriptGameObject* npc = DataBase::Storage::getInstance().getStorage().at(npc_id).getClientObject();

    if (!npc)
    {
        R_ASSERT2(false, "object is null!");
        return false;
    }

    if (!this->m_can_play_sound)
        return false;

    if (this->m_played_time && (Device.dwTimeGlobal - this->m_played_time < this->m_idle_time))
        return false;

    this->m_played_time = 0;

    this->m_played_id = this->select_next_sound();

    if (this->m_played_id == Globals::kUnsignedInt32Undefined)
        return false;

	xr_string sound_name;
	if (this->m_played_id < this->m_sound.size())
		sound_name = this->m_sound.at(this->m_played_id);
	else if (this->m_played_id == this->m_sound.size())
		sound_name = this->m_sound.at(this->m_played_id - 1);
	else
		R_ASSERT2(false, "can't be!");

    if (sound_name.size())
    {
        if (FS.exist("$game_sounds$", (sound_name + "_pda.ogg").c_str()) &&
            (npc->Position().distance_to_sqr(DataBase::Storage::getInstance().getActor()->Position()) >= 5))
        {
            if (this->m_pda_sound_object)
            {
                delete this->m_pda_sound_object;
                this->m_pda_sound_object = nullptr;
            }

            this->m_pda_sound_object = new CScriptSound((sound_name + "_pda").c_str());
            this->m_pda_sound_object->SetVolume(0.8f);
            this->m_pda_sound_object->PlayAtPos(
                DataBase::Storage::getInstance().getActor(), Fvector().set(0, 0, 0), 0.0f, sm_2D);
        }
    }

    if (this->m_sound_object)
    {
        delete this->m_sound_object;
        this->m_sound_object = nullptr;
    }

    this->m_sound_object = new CScriptSound(sound_name.c_str());
    this->m_sound_object->PlayAtPos(npc, npc->Position(), 0.0f, 0);
    this->m_can_play_sound = false;

    return true;
}

int Script_SoundObject::select_next_sound(const std::uint16_t& npc_id)
{
    size_t sound_map_size = this->m_sound.size();

    if (this->m_shuffle == "rnd")
    {
        if (sound_map_size == 1)
        {
            return 1;
        }

        if (this->m_played_id)
        {
            //             std::random_device random_device;
            //             std::mt19937 range(random_device);
            //             std::uniform_int_distribution<size_t> urandom(1, sound_map_size - 1);

            size_t generated_value = Globals::Script_RandomInt::getInstance().Generate<size_t>(1, sound_map_size - 1);

            if (generated_value >= this->m_played_id)
                return generated_value + 1;

            return generated_value;
        }

        //         std::random_device random_device;
        //         std::mt19937 range(random_device);
        //         std::uniform_int_distribution<size_t> urandom(1, sound_map_size);
        size_t generated_value = Globals::Script_RandomInt::getInstance().Generate<size_t>(1, sound_map_size);

        return generated_value;
    }

    if (this->m_shuffle == "seq")
    {
        if (this->m_played_id == -1)
            return Globals::kUnsignedInt32Undefined;

        if (this->m_played_id == Globals::kUnsignedInt32Undefined)
            return 1;

        if (this->m_played_id < sound_map_size)
            return this->m_played_id + 1;

        return Globals::kUnsignedInt32Undefined;
    }

    if (this->m_shuffle == "loop")
    {
        if (this->m_played_id == Globals::kUnsignedInt32Undefined)
            return 1;

        if (this->m_played_id < sound_map_size)
            return this->m_played_id + 1;

        return 1;
    }

    // Lord: хз гарантировано он будет проходить все ифы но какое значение здесь должен возвращать по тестировать!!!
    R_ASSERT(false);
    return Globals::kUnsignedInt32Undefined;
}

void Script_SoundObject::stop(const std::uint16_t& obj_id)
{
    if (this->m_sound_object)
    {
        this->m_sound_object->Stop();
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

void Script_SoundObject::save(NET_Packet& packet) { packet.w_stringZ(std::to_string(this->m_played_id).c_str()); }

void Script_SoundObject::load(NET_Packet& packet)
{
    xr_string id;
    packet.r_stringZ(id);

    if (id != "nil") // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
        this->m_played_id = atoi(id.c_str());
    else
        this->m_played_id = 0;
}

Script_SoundLooped::Script_SoundLooped(CScriptIniFile& ini, const xr_string& section)
    : m_section(section), m_path(Globals::Utils::cfg_get_string(&ini, section, "path")), m_sound_object(nullptr),
      m_class_id(SCRIPTSOUNDTYPE_LOOPED), m_sound("")
{
    if (FS.exist("$game_sounds$", (this->m_path + ".ogg").c_str()))
    {
        this->m_sound = this->m_path;
    }

    if (!this->m_sound.size())
    {
        R_ASSERT2(false, "Can't initialize path normally please check your initialization where FS!!!");
    }
}

Script_SoundLooped::~Script_SoundLooped(void)
{
    if (this->m_sound_object)
    {
       // delete this->m_sound_object;
        this->m_sound_object = nullptr;
    }
}

bool Script_SoundLooped::play(const std::uint16_t& obj_id)
{
    CScriptGameObject* object = DataBase::Storage::getInstance().getStorage().at(obj_id).getClientObject();
    if (!object)
    {
        R_ASSERT2(false, "object is null!");
        return false;
    }

    if (this->m_sound_object)
    {
        delete this->m_sound_object;
        this->m_sound_object = nullptr;
    }

    this->m_sound_object = new CScriptSound(this->m_sound.c_str());
    this->m_sound_object->PlayAtPos(
        object, object->Position(), 0.0f, 0 + sm_Looped); // Lord: думаю всё же стоит убрать ноль)

    return true;
}

bool Script_SoundLooped::is_playing(const std::uint16_t& npc_id)
{
    if (this->m_sound_object)
        return this->m_sound_object->IsPlaying();

    return false;
}

void Script_SoundLooped::stop(const std::uint16_t& obj_id)
{
    if (this->m_sound_object)
        this->m_sound_object->Stop();
}

} // namespace Scripts
} // namespace Cordis
