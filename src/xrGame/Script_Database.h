#pragma once

#include "script_sound.h"
#include "Script_LogicEntity.h"

namespace Cordis
{
namespace Scripts
{
namespace DataBase
{
inline void add_enemy(CSE_Abstract* object)
{
    if (!object)
    {
        R_ASSERT2(false, "can't be null!");
        return;
    }
}

class PStor_Data
{
    friend class Storage;
    char m_boolean =
        Globals::kPstorBooleanUndefined; // Globals::kPstorBooleanFalse -> False, Globals::kPstorBooleanUndefined -> Not
                                         // initialized, Globals::kPstorBooleanTrue -> True
    std::uint8_t m_number = Globals::kUnsignedInt8Undefined;
    xr_string m_string = Globals::kStringUndefined;

public:
    inline bool IsInitializedBool(void) const noexcept { return (this->m_boolean != Globals::kPstorBooleanUndefined); }
    inline bool IsInitializedNumber(void) const noexcept { return (this->m_number != Globals::kUnsignedInt8Undefined); }
    inline bool IsInitializedString(void) const noexcept { return (this->m_string != Globals::kStringUndefined); }
    inline bool IsInitializedSomething(void) const noexcept
    {
        return (this->IsInitializedBool() || this->IsInitializedNumber() || this->IsInitializedString());
    }
    inline bool getBool(void) const noexcept
    {
        switch (this->m_boolean)
        {
        case '-':
        {
            Msg("[Scripts/DataBase/PStor_Data/getBool()] Returns an initialized value -> False");
            break;
        }
        case '0':
        {
            Msg("[Scritps/DataBase/PStor_Data/getBool()] the m_boolean doesn't initialized!");
            break;
        }
        case '+':
        {
            Msg("[Scripts/DataBase/PStor_Data/getBool()] Returns an initialized value -> True");
            return true;
            break;
        }
        };

        return false;
    }

    inline std::uint8_t getNumber(void) const noexcept
    {
        if (this->m_number == Globals::kUnsignedInt8Undefined)
        {
            Msg("[Scripts/DataBase/PStor_Data/getNumber()] the m_number doesn't initialized. Returns 0.");
            return std::uint8_t(0);
        }

        Msg("[Scripts/DataBase/PStor_Data/getNumber()] Returns value [%d], because it was initialized!",
            this->m_number);
        return this->m_number;
    }

    inline xr_string getString(void) const noexcept
    {
        if (this->m_string == Globals::kStringUndefined)
        {
            Msg("[Scripts/DataBase/PStor_Data/getString()] the m_string doesn't initialized. Returns an empty string.");
            return xr_string();
        }

        Msg("[Scripts/DataBase/PStor_Data/getString()] Returns value [%s], it's initialized!", this->m_string.c_str());
        return this->m_string;
    }

    inline void setBool(const bool& value) noexcept
    {
        if (this->m_number != Globals::kUnsignedInt8Undefined || this->m_string != Globals::kStringUndefined)
        {
            Msg("[Scripts/DataBase/PStor_Data/setNumber(value)] You can't assigned a value to m_boolean, because some "
                "member of structure was initialized!");
            return;
        }

        if (value)
        {
            Msg("[Scripts/DataBase/PStor_Data/setBool(value)] -> True");
            this->m_boolean = Globals::kPstorBooleanTrue;
        }
        else
        {
            Msg("[Scripts/DataBase/PStor_Data/setBool(value)] -> False");
            this->m_boolean = Globals::kPstorBooleanFalse;
        }
    }

    inline void setNumber(const std::uint8_t& value) noexcept
    {
        if (this->m_boolean != Globals::kPstorBooleanUndefined || this->m_string != Globals::kStringUndefined)
        {
            Msg("[Scripts/DataBase/PStor_Data/setNumber(value)] You can't assigned a value to m_number, because some "
                "member of structure was initialized!");
            return;
        }

        Msg("[Scripts/DataBase/PStor_Data/setNumber(value)] -> %d", value);
        this->m_number = value;
    }

    inline void setString(const xr_string& string) noexcept
    {
        if (this->m_boolean != Globals::kPstorBooleanUndefined || this->m_number != Globals::kUnsignedInt8Undefined)
        {
            Msg("[Scripts/DataBase/PStor_Data/setNumber(value)] You can't assigned a value to m_string, because some "
                "member of structure was initialized!");
            return;
        }

        Msg("[Scripts/DataBase/PStor_Data/setString(string)] -> %s", string.c_str());
        this->m_string = string;
    }
};
// сделать private, public!
struct SubStorage_Data
{
private:
    friend class Storage;

public:
    SubStorage_Data(void) = default;
    ~SubStorage_Data(void) = default;

    inline void setSignal(const xr_map<xr_string, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/SubStorage_Data/setSignal(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map! No assignment!");
            return;
        }

        this->m_signals = map;
    }

    inline void setSignal(const std::pair<xr_string, bool>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("");
            return;
        }
    }
    inline void setSignal(const xr_string& id_name, const bool& value) noexcept
    {
        if (!id_name.size())
        {
            Msg("[DataBase/SubStorage_Data/setSignal(id_name, value)] WARNING: id_name.size() = 0! You are trying to "
                "set an empty string! No assignment!");
            return;
        }

        this->m_signals[id_name] = value;
    }

    inline void setAction(const xr_vector<Script_ILogicEntity*>& vector)
    {
        if (!vector.size())
        {
            Msg("[DataBase/SubStorage_Data/setAction(vector)] WARNING: vector.size() = 0! You are trying to set an "
                "empty vector! No assignment!");
            return;
        }

        this->m_actions = vector;
    }

    inline void setAction(Script_ILogicEntity* entity)
    {
        if (!entity)
        {
            Msg("[DataBase/SubStorage_Data/setAction(p_entity)] WARNING: p_entity = null! You are trying to set an "
                "empty object! No assignment!");
            return;
        }

        this->m_actions.push_back(entity);
    }

    inline const xr_map<xr_string, bool>& getSignals(void) const noexcept { return this->m_signals; }
    inline const xr_vector<Script_ILogicEntity*>& getActions(void) const noexcept { return this->m_actions; }
    inline void ClearActions(void)
    {
        Msg("[DataBase/SubStorage_Data/ClearActions()] this->m_actions.clear() is called!");
        this->m_actions.clear();
    }
    inline void ClearSignals(void)
    {
        Msg("[DataBase/SubStorage_Data/ClearSignals()] this->m_signals");
        this->m_signals.clear();
    }

private:
    xr_map<xr_string, bool> m_signals;
    xr_vector<Script_ILogicEntity*> m_actions;
};
// сделать private, public!
struct StorageAnimpoint_Data
{
private:
    friend class Storage;

public:
    inline void setCoverName(const xr_string& string) { this->m_cover_name = string; }
    inline bool setUseCamp(const bool& value) { this->m_is_use_camp = value; }
    inline void setAnimpoint(Script_Animpoint* object)
    {
        if (!object)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        this->m_animpoint = object;
    }
    inline void setReachedDistance(const float& value) { this->m_reached_distance = value; }
    inline void setAvailAnimation(const xr_string& animation_name)
    {
        if (!animation_name.size())
        {
            R_ASSERT2(false, "can't be empty!");
            return;
        }

        this->m_avail_animations.push_back(animation_name);
    }
    inline void setApprovedAction(const std::function<bool(std::uint16_t, bool)> function, const xr_string& action_name)
    {
        if (!action_name.size())
        {
            R_ASSERT2(false, "can't be empty!");
            return;
        }

        if (!function)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        std::pair<std::function<bool(std::uint16_t, bool)>, xr_string> insert_pair;

        insert_pair.first = function;
        insert_pair.second = action_name;
        this->m_approved_actions.push_back(insert_pair);
    }

    inline void setDescriptionName(const xr_string& description_name) noexcept
    {
        if (!description_name.size())
        {
            Msg("[Scripts/DataBase/StorageAnimpoint_Data/setDescriptionName(description_name)] WARNING: you are trying "
                "to set an empty string!");
        }

        this->m_description_name = description_name;
    }

    inline float getReachedDistance(void) const noexcept { return this->m_reached_distance; }
    inline xr_string getCoverName(void) const noexcept { return this->m_cover_name; }
    inline xr_string getDescriptionName(void) const noexcept { return this->m_description_name; }
    inline Script_Animpoint* getAnimpoint(void) const noexcept { return this->m_animpoint; }
    inline const xr_vector<xr_string>& getAvailAnimations(void) const noexcept { return this->m_avail_animations; }
    inline const xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>>& getApprovedActions(
        void) const noexcept
    {
        return this->m_approved_actions;
    }
    inline bool IsUseCamp(void) const noexcept { return this->m_is_use_camp; }

private:
    bool m_is_use_camp;
    float m_reached_distance;
    xr_vector<xr_string> m_avail_animations;
    xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>> m_approved_actions;
    xr_string m_cover_name;
    xr_string m_description_name;
    Script_Animpoint* m_animpoint =
        nullptr; // Lord: узнать где выделяется и удаляется данный класс (xr_animpoint.script)
};

struct Storage_Data
{
private:
    friend class Storage;

public:
    inline bool IsInvulnerable(void) const noexcept { return this->m_is_invulnerable; }
    inline void setInvulnerable(const bool& value) noexcept { this->m_is_invulnerable = value; }
    inline bool IsImmortal(void) const noexcept { return this->m_is_immortal; }
    inline void setImmortal(const bool& value) noexcept { this->m_is_immortal = value; }
    inline bool IsMute(void) const noexcept { return this->m_is_mute; }
    inline void setMute(const bool& value) noexcept { this->m_is_mute = value; }
    inline bool IsAnimMovement(void) const noexcept { return this->m_is_anim_movement; }
    inline void setAnimMovement(const bool& value) noexcept { this->m_is_anim_movement = value; }

    inline std::uint16_t getEnemyID(void) const noexcept { return this->m_enemy_id; }
    inline void setEnemyID(const std::uint16_t& value) noexcept
    {
        if (value == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage_Data/setEnemyID(value)] WARNING: value = std::uint16_t(-1)! You are trying to set "
                "an undefined value! No assignment!");
            return;
        }

        this->m_enemy_id = value;
    }

    inline CScriptGameObject* getClientObject(void) const { return this->m_p_client_object; }
    inline void setClientObject(CScriptGameObject* p_client_object)
    {
        if (!p_client_object)
        {
            Msg("[DataBase/Storage_Data/setObjet(p_client_object)] WARNING: p_client_object = null! You are trying to "
                "set an empty object! No assignment!");
            return;
        }

        this->m_p_client_object = p_client_object;
    }

    inline const StorageAnimpoint_Data& getStorageAnimpoint(void) const noexcept { return this->m_storage_animpoint; }
    inline void setStorageAnimpoint(const StorageAnimpoint_Data& data) noexcept { this->m_storage_animpoint = data; }

    inline CSE_ALifeObject* getServerObject(void) const { return this->m_p_server_object; }
    inline void setServerObject(CSE_ALifeObject* p_server_object)
    {
        if (!p_server_object)
        {
            Msg("[DataBase/Storage_Data/setServerObject(p_server_object)] WARNING: p_server_object = null! You are "
                "trying to set an empty object! No assignment!");
            return;
        }

        this->m_p_server_object = p_server_object;
    }

    inline CScriptSound* getSoundObject(void) const { return this->m_p_sound_object; }
    inline void setSoundObject(CScriptSound* p_sound_object)
    {
        if (!p_sound_object)
        {
            Msg("[DataBase/Storage_Data/setSoundObject(p_sound_object)] WARNING: p_sound_object = null! You are trying "
                "to set an empty object! No assignment!");
            return;
        }

        this->m_p_sound_object = p_sound_object;
    }

    inline CInifile* getIni(void) const { return this->m_p_ini; }
    inline void setIni(CInifile* p_ini)
    {
        if (!p_ini)
        {
            Msg("[DataBase/Storage_Data/setIniObject(p_ini)] WARNING: p_ini = null! You are trying to set an empty "
                "object! No assignment!");
            return;
        }

        this->m_p_ini = p_ini;
    }

    inline void ResetSignals(void) { this->m_data.clear(); }

    // @ Gets signals xr_map<xr_string, bool>
    inline const SubStorage_Data& operator[](const xr_string& id) { return m_data[id]; }

    inline const xr_map<xr_string, SubStorage_Data>& getData(void) const noexcept { return this->m_data; }

    inline void setData(const xr_map<xr_string, SubStorage_Data>& map)
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage_Data/setData(map)] WARNING: map.size() = 0! You are trying to set an empty map! No "
                "assignment!");
            return;
        }

        this->m_data = map;
    }

    inline void setData(const std::pair<xr_string, SubStorage_Data>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[DataBase/Storage_Data/setData(pair)] WARNING: pair.first.size() = 0! You are trying to set an empty "
                "string! No assignment!");
            return;
        }

        this->m_data.insert(pair);
    }

    inline void setData(const xr_string& id_name, const SubStorage_Data& data)
    {
        if (!id_name.size())
        {
            Msg("[DataBase/Storage_Data/setData(id_name, data)] WARNING: id_name.size() = 0! You are trying to set an "
                "empty string! No assignment!");
            return;
        }

        this->m_data[id_name] = data;
    }

    inline const xr_map<xr_string, PStor_Data>& getPStor(void) const noexcept { return this->m_pstor; }

    inline void setPStor(const xr_map<xr_string, PStor_Data>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage_Data/setPStor(map)] WARNING: map.size() = 0! You are trying to set an empty map! No "
                "assignment!");
            return;
        }

        this->m_pstor = map;
    }

    inline void setPStor(const std::pair<xr_string, PStor_Data>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[DataBase/Storage_Data/setPStor(pair)] WARNING: pair.first.size() = 0! You are trying to set an empty "
                "pair! No assignment!");
            return;
        }

        this->m_pstor.insert(pair);
    }

    inline void setPStor(const xr_string& id_name, const PStor_Data& data) noexcept
    {
        if (!id_name.size())
        {
            Msg("[DataBase/Storage_Data/setPStor(id_name, data)] WARNING: id_name.size() = 0! You are trying to set an "
                "empty string! No assignment!");
            return;
        }

        this->m_pstor[id_name] = data;
    }

    inline xr_string getActiveSchemeName(void) const noexcept { return this->m_active_scheme_name; }
    inline void setActiveSchemeName(const xr_string& scheme_name) noexcept { this->m_active_scheme_name = scheme_name; }

    inline xr_string getActiveSectionName(void) const noexcept { return this->m_active_section_name; }
    inline void setActiveSectionName(const xr_string& section_name) noexcept
    {
        this->m_active_section_name = section_name;
    }

    inline xr_string getSoundName(void) const noexcept { return this->m_sound_name; }
    inline void setSoundName(const xr_string& sound_name) noexcept { this->m_sound_name = sound_name; }

    inline xr_string getAnimationName(void) const noexcept { return this->m_animation_name; }
    inline void setAnimationName(const xr_string& animation_name) noexcept { this->m_animation_name = animation_name; }

    inline xr_string getAnimationHeadName(void) const noexcept { return this->m_animation_head_name; }
    inline void setAnimationHeadName(const xr_string& animation_head_name) noexcept
    {
        this->m_animation_head_name = animation_head_name;
    }

    inline xr_string getTipName(void) const noexcept { return this->m_tip_name; }
    inline void setTipName(const xr_string& tip_name) noexcept { this->m_tip_name = tip_name; }

    inline xr_string getTimeName(void) const noexcept { return this->m_time_name; }
    inline void setTimeName(const xr_string& time_name) noexcept { this->m_time_name = time_name; }

    inline xr_string getJobIniName(void) const noexcept { return this->m_job_ini_name; }
    inline void setJobIniName(const xr_string& job_ini_name) noexcept { this->m_job_ini_name = job_ini_name; }

private:
    bool m_is_invulnerable = false;
    bool m_is_immortal = false;
    bool m_is_mute = false;
    bool m_is_enabled = false;
    bool m_is_anim_movement = false;
    std::uint16_t m_enemy_id = Globals::kUnsignedInt16Undefined;
    CScriptGameObject* m_p_client_object = nullptr;
    StorageAnimpoint_Data m_storage_animpoint;
    CSE_ALifeObject* m_p_server_object = nullptr;
    CScriptSound* m_p_sound_object = nullptr;
    CInifile* m_p_ini = nullptr;
    xr_map<xr_string, SubStorage_Data> m_data;
    xr_map<xr_string, PStor_Data> m_pstor;
    xr_string m_active_scheme_name = "";
    xr_string m_active_section_name = "";
    xr_string m_sound_name = "";
    xr_string m_animation_name = "";
    xr_string m_animation_head_name = "";
    xr_string m_tip_name = "";
    xr_string m_time_name = "";
    xr_string m_job_ini_name = "";
};

class Storage
{
private:
    Storage(void) = default;

public:
    inline static Storage& getInstance(void) noexcept
    {
        static Storage instance;
        return instance;
    }

    // Lord: или переместить в другой метод! Потестить
    ~Storage(void)
    {
        // @ Lord: подумать здесь нужно это удалять так или оно в другом месте?
        for (xr_map<std::uint16_t, Storage_Data>::value_type& it : this->m_storage)
        {
            if (it.second.getStorageAnimpoint().getAnimpoint())
            {
                Msg("[Scripts/DataBase/Storage/~dtor] Deleting: Animpoint -> [%s]",
                    it.second.getStorageAnimpoint().getCoverName().c_str());
                Script_Animpoint* instance = it.second.getStorageAnimpoint().getAnimpoint();
                delete instance;
                instance = nullptr;
            }

            for (const xr_map<xr_string, SubStorage_Data>::value_type& object : it.second.getData())
            {
                if (object.second.getActions().size())
                {
                    for (Script_ILogicEntity* entity : object.second.getActions())
                    {
                        if (entity)
                        {
                            Msg("[Scripts/DataBase/Storage/~dtor] Deleting Script_IEntity: %s",
                                entity->m_logic_name.c_str());
                            delete entity;
                            entity = nullptr;
                        }
                    }
                }
            }

            if (it.second.getClientObject())
            {
                CScriptGameObject* p_client_object = it.second.getClientObject();
                Msg("[Scripts/DataBase/Storage/~dtor] Deleting the m_object: %s", p_client_object->Name());
                delete p_client_object;
                p_client_object = nullptr;
            }

            if (it.second.getServerObject())
            {
                CSE_ALifeObject* p_server_object = it.second.getServerObject();
                Msg("[Scripts/DataBase/Storage/~dtor] Deleting the m_server_object: %s", p_server_object->name());
                delete p_server_object;
                p_server_object = nullptr;
            }

            if (it.second.getIni())
            {
                CInifile* p_ini = it.second.getIni();
                Msg("[Scripts/DataBase/Storage/~dtor] Delete the m_ini: %s", p_ini->fname());
                delete p_ini;
                p_ini = nullptr;
            }

            if (it.second.getSoundObject())
            {
                CScriptSound* p_sound = it.second.getSoundObject();
                Msg("[Scripts/DataBase/Storage/~dtor] Deleting the m_sound_object");
                delete p_sound;
                p_sound = nullptr;
            }
        }

        this->m_storage.clear();
    }

#pragma region Getters
    inline const xr_map<std::uint16_t, Storage_Data>& getStorage(void) const noexcept { return this->m_storage; }

    inline void setStorage(const xr_map<std::uint16_t, Storage_Data>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage/setStorage(map)] WARNING: map.size() = 0! You are trying to set an empty map! No "
                "assignment!");
            return;
        }

        this->m_storage = map;
    }

    inline void setStorage(const std::pair<std::uint16_t, Storage_Data>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setStorage(pair)] WARNING: pair.first = std::uint16_t(-1)! You are trying to set an "
                "undefined variable! No assignment!");
            return;
        }

        this->m_storage.insert(pair);
    }
    inline void setStorage(const std::uint16_t& id, const Storage_Data& data) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setStorage(id, data)] WARNING: id = std::uint16_t(-1)! You are trying to set an "
                "undefined variable! No assignment!");
            return;
        }

        this->m_storage[id] = data;
    }

    inline CScriptGameObject* getActor(void) const { return this->m_actor; }
    // Lord: переделать сюда идёт bind_anomaly_zone
    inline const xr_map<xr_string, CScriptGameObject*>& getAnomalyByName(void) const noexcept
    {
        return this->m_anomaly_by_name;
    }

    inline void setAnomalyByName(const xr_map<xr_string, CScriptGameObject*>& map)
    {
        if (!map.size())
        {
            Msg("[Scripts/DataBase/Storage/setAnomalyByName(map)] WARNING: map.size() = 0! You are trying to set empty "
                "map! Return!");
            return;
        }

        this->m_anomaly_by_name = map;
    }

    inline void setAnomalyByName(const std::pair<xr_string, CScriptGameObject*>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Scripts/DataBase/Storage/setAnomalyByName(pair)] WARNING: pair.first.size() = 0! You are trying to "
                "set an empty string! Return");
            return;
        }

        if (!pair.second)
        {
            Msg("[Scripts/DataBase/Storage/setAnomalyByName(pair)] WARNING: pair.second = nullptr! You are trying to "
                "set an empty object! Return!");
            return;
        }

        this->m_anomaly_by_name.insert(pair);
    }

    inline void setAnomalyByName(const xr_string& anomaly_name, CScriptGameObject* object)
    {
        if (!anomaly_name.size())
        {
            Msg("[Scripts/DataBase/Storage/setAnomalyByName(anomaly_name, object)] WARNING: anomaly_name.size() = 0! "
                "You are trying to set an empty string! Return;");
            return;
        }

        if (!object)
        {
            Msg("[Scripts/DataBase/Storage/setAnomalyByName(anomaly_name, object)] WARNING: object = nullptr! You are "
                "trying to set an empty object!");
            return;
        }

        this->m_anomaly_by_name[anomaly_name] = object;
    }

    inline const xr_map<xr_string, CScriptGameObject*>& getZoneByName(void) const noexcept
    {
        return this->m_zone_by_name;
    }

    inline void setZoneByName(const xr_map<xr_string, CScriptGameObject*>& map)
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage/setZoneByName(map)] WARNING: map.size() = 0! You are trying to set an empty map! No "
                "assignment!");
            return;
        }

        this->m_zone_by_name = map;
    }

    inline void setZoneByName(const std::pair<xr_string, CScriptGameObject*>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[DataBase/Storage/setZoneByName(pair)] WARNING: pair.first.size() = 0! You are "
                "trying to set an empty string! No assignment!");
            return;
        }

        if (!pair.second)
        {
            Msg("[DataBase/Storage/setZoneByName(pair)] WARNING: pair.second = null! You are "
                "trying to set an empty object! No assignment!");
            return;
        }

        this->m_zone_by_name.insert(pair);
    }

    inline void setZoneByName(const xr_string& zone_name, CScriptGameObject* p_client_zone)
    {
        if (!zone_name.size())
        {
            Msg("[DataBase/Storage/setZoneByName(zone_name, p_client_zone)] WARNING: zone_name.size() = 0! You are "
                "trying to set an empty string! No assignment!");
            return;
        }

        if (!p_client_zone)
        {
            Msg("[DataBase/Storage/setZoneByName(zone_name, p_client_zone)] WARNING: p_client_zone = null! You are "
                "trying to set an empty object! No assignment!");
            return;
        }

        this->m_zone_by_name[zone_name] = p_client_zone;
    }

    inline const xr_map<std::uint16_t, float>& getGoodwill_Sympathy(void) const noexcept
    {
        return this->m_goodwill.first;
    }

    inline void setGoodwill_Sympathy(const xr_map<std::uint16_t, float>& first) noexcept
    {
        if (!first.size())
        {
            Msg("DataBase/Storage/setGoodwill_Sympathy(first, second)] WARNING: first.size() = 0! You are trying to "
                "set an "
                "empty map! No assignment!");
            return;
        }

        this->m_goodwill.first = first;
    }

    inline void setGoodwill_Sympathy(const std::pair<std::uint16_t, float>& pair_first) noexcept
    {
        if (pair_first.first == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setGoodwill_Sympathy(pair_first)] WARNING: pair_first.first = std::uint16_t(-1)! "
                "You are "
                "trying to set an undefined value! No assignment!");
            return;
        }

        this->m_goodwill.first.insert(pair_first);
    }

    inline void setGoodwill_Sympathy(const std::uint16_t& id, const float& value) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setGoodwill_Sympathy(id, value)] WARNING: id = std::uint16_t(-1)! You are trying to "
                "set an "
                "undefined value! No assignment!");
            return;
        }

        this->m_goodwill.first[id] = value;
    }

    inline const xr_map<std::uint16_t, xr_string>& getGoodwill_Relations(void) const noexcept
    {
        return this->m_goodwill.second;
    }

    inline void setGoodwill_Relations(const xr_map<std::uint16_t, xr_string>& second) noexcept
    {
        if (!second.size())
        {
            Msg("[DataBase/Storage/setGoodwill_Relations(first, second] WARNING: second.size() = 0! You are trying to "
                "set an "
                "empty map! No assignment!");
            return;
        }

        this->m_goodwill.second = second;
    }

    inline void setGoodwill_Relations(const std::pair<std::uint16_t, xr_string>& pair_second) noexcept
    {
        if (pair_second.first == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setGoodwill_Relations(pair_second)] WARNING: pair_second.first = std::uint16_t(-1)! "
                "You are "
                "trying to set an undefined value! No assignment!");
            return;
        }

        this->m_goodwill.second.insert(pair_second);
    }

    inline void setGoodwill_Relations(const std::uint16_t& id, const xr_string& string) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setGoodwill_Relations(id, string)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to set an "
                "undefined value! No assignment!");
            return;
        }

        this->m_goodwill.second[id] = string;
    }

    inline const xr_map<std::uint16_t, std::uint32_t>& getSpawnedVertexByID(void) const noexcept
    {
        return this->m_spawned_vertex_by_id;
    }

    inline void setSpawnedVertexByID(const xr_map<std::uint16_t, std::uint32_t>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage/setSpawnedVertexByID(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map! No assignment!");
            return;
        }

        this->m_spawned_vertex_by_id = map;
    }

    inline void setSpawnedVertexByID(const std::pair<std::uint16_t, std::uint32_t>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setSpawnedVertexByID(pair)] WARNING: pair.first = std::uint16_t(-1)! You are trying "
                "to set an undefined value! No assignment!");
            return;
        }

        this->m_spawned_vertex_by_id.insert(pair);
    }

    inline void setSpawnedVertexByID(const std::uint16_t& id, const std::uint32_t& value) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setSpawnedVertexByID(id, value)] WARNING: id = std::uint16_t(-1)! You are trying to "
                "set an undefined value! No assignment!");
            return;
        }

        this->m_spawned_vertex_by_id[id] = value;
    }

    inline const xr_map<std::uint16_t, std::pair<std::uint16_t, xr_string>>& getOfflineObjects(void) const noexcept
    {
        return this->m_offline_objects;
    }

    /*
        inline void ClearOfflineObjects_Second(const std::uint16_t& id) noexcept
        {
            if (id == Globals::kUnsignedInt16Undefined)
            {
                Msg("[DataBase/Storage/ClearOfflineObjects_Second(id)] WARNING: id = std::uint16_t(-1)! You are trying
       to get access through an undefined value! NO assignment!"); return;
            }

            this->m_offline_objects[id].second.clear();
        }*/

    inline void setOfflineObjects(const xr_map<std::uint16_t, std::pair<std::uint16_t, xr_string>>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage/setOfflineObjects(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map!");
          //  return;
        }

        this->m_offline_objects = map;
    }

    inline void setOfflineObjects(const std::pair<std::uint16_t, std::pair<std::uint16_t, xr_string>>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setOfflineObjects(pair)] WARNING: pair.first = std::uint16_t(-1)! You are trying to "
                "set "
                "an empty pair!");
         //   return;
        }

        this->m_offline_objects.insert(pair);
    }

    inline void setOfflineObjects(const std::uint16_t& id, const std::pair<std::uint16_t, xr_string>& pair) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setOfflineObjects(id, pair)] WARNING: id = std::uint16_t(-1)! You are trying to set "
                "an undefined value!");
         //   return;
        }

        this->m_offline_objects[id] = pair;
    }

    // @ Lord: наименовать нормально аргументы
    inline void setOfflineObjects(const std::uint16_t& id, const std::uint16_t& _id, const xr_string& string) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setOfflineObjects(id, _id, string)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to set an undefined value!");
        //    return;
        }

        this->m_offline_objects[id] = {_id, string};
    }

    inline void setOfflineObjects(const std::uint16_t& id, const std::uint16_t& _id) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setOfflineObjects(id, _id, string)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to set an undefined value!");
          //  return;
        }

        this->m_offline_objects[id].first = _id;
    }

    inline void setOfflineObjects(const std::uint16_t& id, const xr_string& string) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setOfflineObjects(id, _id, string)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to set an undefined value!");
          //  return;
        }

        this->m_offline_objects[id].second = string;
    }

    inline const std::pair<xr_map<std::uint16_t, float>, xr_map<std::uint16_t, xr_string>>& getGoodwill(void) const
        noexcept
    {
        return this->m_goodwill;
    }

    inline void setGoodwill(
        const xr_map<std::uint16_t, float>& first, const xr_map<std::uint16_t, xr_string>& second) noexcept
    {
        if (!first.size())
        {
            Msg("DataBase/Storage/setGoodwill(first, second)] WARNING: first.size() = 0! You are trying to set an "
                "empty map! No assignment!");
            return;
        }

        if (!second.size())
        {
            Msg("[DataBase/Storage/setGoodwill(first, second] WARNING: second.size() = 0! You are trying to set an "
                "empty map! No assignment!");
            return;
        }

        this->m_goodwill.first = first;
        this->m_goodwill.second = second;
    }
#pragma endregion

#pragma region Setters
    inline void setActor(CScriptGameObject* object)
    {
        if (!object)
        {
            R_ASSERT2(false, "Can't set actor because the object was null!");
            return;
        }

        this->m_actor = object;
    }

    inline void setPStorBool(const std::uint16_t& id, const xr_string& varname, const bool& value) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setPStorNumber(id, varname, value)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to get access through an undefined value! No assignment!");
            return;
        }

        if (!varname.size())
        {
            Msg("[DataBase/Storage/setPStorNumber(id, varname, value)] WARNING: varname.size() = 0! You are trying to "
                "get an access through an empty string! No assignment!");
            return;
        }

        this->m_storage[id].m_pstor[varname].setBool(value);
    }

    inline void setPStorNumber(const std::uint16_t& id, const xr_string& varname, const std::uint8_t& value)
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setPStorNumber(id, varname, value)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to get access through an undefined value! No assignment!");
            return;
        }

        if (!varname.size())
        {
            Msg("[DataBase/Storage/setPStorNumber(id, varname, value)] WARNING: varname.size() = 0! You are trying to "
                "get an access through an empty string! No assignment!");
            return;
        }

        this->m_storage[id].m_pstor[varname].setNumber(value);
    }

    inline void setPStorString(const std::uint16_t& id, const xr_string& varname, const xr_string& value)
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setPStorString(id, varname, value)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to get an access through an undefined value! No assignment!");
            return;
        }

        if (!varname.size())
        {
            Msg("[DataBase/Storage/setPStorString(id, varname, value)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to get an access through an undefined value! No assignment!");
            return;
        }

        this->m_storage[id].m_pstor[varname].setString(value);
    }

    inline void setSignal(const std::uint16_t& id, const xr_string& signal_name, const bool& value) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setSignal(id, signal_name, value)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to get access through an undefined value! No assignment!");
            return;
        }

        this->m_storage[id].m_data[this->m_storage[id].getActiveSchemeName()].setSignal(signal_name, value);
    }
#pragma endregion

    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(Storage&&) = delete;

private:
    CScriptGameObject* m_actor;
    xr_map<std::uint16_t, Storage_Data> m_storage;
    xr_map<std::uint16_t, std::pair<std::uint16_t, xr_string>> m_offline_objects;
    xr_map<xr_string, CScriptGameObject*> m_zone_by_name;
    xr_map<xr_string, CScriptGameObject*> m_anomaly_by_name;
    xr_map<std::uint16_t, std::uint32_t> m_spawned_vertex_by_id;
    // first -> sympathy[ID] = std::uint32_t; | second -> relations[ID] = std::string;
    std::pair<xr_map<std::uint16_t, float>, xr_map<std::uint16_t, xr_string>> m_goodwill;
};

} // namespace DataBase
} // namespace Scripts
} // namespace Cordis
