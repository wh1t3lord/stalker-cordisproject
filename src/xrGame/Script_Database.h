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
    char m_boolean =
        Globals::kPstorBooleanUndefined; // Globals::kPstorBooleanFalse -> False, Globals::kPstorBooleanUndefined -> Not
                                         // initialized, Globals::kPstorBooleanTrue -> True
    std::uint8_t m_number = Globals::kUnsignedInt8Undefined;
    xr_string m_string = Globals::kStringUndefined;

public:
    inline bool IsInitializedBool(void) noexcept { return (this->m_boolean != Globals::kPstorBooleanUndefined); }
    inline bool IsInitializedNumber(void) noexcept { return (this->m_number != Globals::kUnsignedInt8Undefined); }
    inline bool IsInitializedString(void) noexcept { return (this->m_string != Globals::kStringUndefined); }
    inline bool IsInitializedSomething(void) noexcept
    {
        return (this->IsInitializedBool() || this->IsInitializedNumber() || this->IsInitializedString());
    }
    inline bool getBool(void) noexcept
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

    inline std::uint8_t getNumber(void) noexcept
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

    inline xr_string getString(void) noexcept
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
    SubStorage_Data(void) = default;
    ~SubStorage_Data(void) = default;

    inline void setSignal(const xr_string& id, const bool& value) noexcept { this->m_signals[id] = value; }
    inline void setAction(Script_ILogicEntity* entity)
    {
        if (!entity)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        this->m_actions.push_back(entity);
    }

    inline xr_map<xr_string, bool>& getSignals(void) noexcept { return this->m_signals; }
    inline xr_vector<Script_ILogicEntity*>& getActions(void) noexcept { return this->m_actions; }

private:
    xr_map<xr_string, bool> m_signals;
    xr_vector<Script_ILogicEntity*> m_actions;
};
// сделать private, public!
struct StorageAnimpoint_Data
{
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
    bool m_invulnerable = false;
    bool m_immortal = false;
    bool m_mute = false;
    bool m_enabled = false;
    bool m_anim_movement = false;
    std::uint16_t m_enemy_id = Globals::kUnsignedInt16Undefined;
    CScriptGameObject* m_object = nullptr;
    StorageAnimpoint_Data m_storage_animpoint;
    CSE_ALifeObject* m_server_object = nullptr;
    CScriptSound* m_sound_object = nullptr;
    CInifile* m_ini = nullptr;
    xr_map<xr_string, SubStorage_Data> m_data;
    xr_map<xr_string, PStor_Data> m_pstor;
    xr_string m_active_scheme = "";
    xr_string m_active_section = "";
    xr_string m_sound = "";
    xr_string m_animation = "";
    xr_string m_animation_head = "";
    xr_string m_tip = "";
    xr_string m_time = "";
    xr_string m_job_ini = "";

    inline void ResetSignals(void) { this->m_data.clear(); }

    // @ Gets singnals xr_map<xr_string, bool>
    inline SubStorage_Data& operator[](const xr_string& id) { return m_data[id]; }
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
            if (it.second.m_storage_animpoint.getAnimpoint())
            {
                Msg("[Scripts/DataBase/Storage/~dtor] Deleting: Animpoint -> [%s]",
                    it.second.m_storage_animpoint.getCoverName().c_str());
                Script_Animpoint* instance = it.second.m_storage_animpoint.getAnimpoint();
                delete instance;
                instance = nullptr;
            }

            for (xr_map<xr_string, SubStorage_Data>::value_type& object : it.second.m_data)
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

                    object.second.getActions().clear();
                }

                object.second.getSignals().clear();
            }

            if (it.second.m_object)
            {
                Msg("[Scripts/DataBase/Storage/~dtor] Deleting the m_object: %s", it.second.m_object->Name());
                delete it.second.m_object;
                it.second.m_object = nullptr;
            }

            if (it.second.m_server_object)
            {
                Msg("[Scripts/DataBase/Storage/~dtor] Deleting the m_server_object: %s",
                    it.second.m_server_object->name());
                delete it.second.m_server_object;
                it.second.m_server_object = nullptr;
            }

            if (it.second.m_ini)
            {
                Msg("[Scripts/DataBase/Storage/~dtor] Delete the m_ini: %s", it.second.m_ini->fname());
                delete it.second.m_ini;
                it.second.m_ini = nullptr;
            }

            if (it.second.m_sound_object)
            {
                Msg("[Scripts/DataBase/Storage/~dtor] Deleting the m_sound_object");
                delete it.second.m_sound_object;
                it.second.m_sound_object = nullptr;
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

    inline const xr_map<xr_string, CScriptGameObject*>& getZoneByName(void) const noexcept
    {
        return this->m_zone_by_name;
    }

    inline void setZoneByName(const xr_map<xr_string, CScriptGameObject*>& map)
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage/setZoneByName(map)] WARNING: map.size() = 0! You are trying to set an empty map! No assignment!");
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
        if (!pair_first.first == Globals::kUnsignedInt16Undefined)
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
        if (!pair.first == Globals::kUnsignedInt16Undefined)
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

    inline void setOfflineObjects(const xr_map<std::uint16_t, std::pair<std::uint16_t, xr_string>>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage/setOfflineObjects(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map! No assignment!");
            return;
        }

        this->m_offline_objects = map;
    }

    inline void setOfflineObjects(const std::pair<std::uint16_t, std::pair<std::uint16_t, xr_string>>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setOfflineObjects(pair)] WARNING: pair.first = std::uint16_t(-1)! You are trying to "
                "set "
                "an empty pair! No assignment!");
            return;
        }

        this->m_offline_objects.insert(pair);
    }

    inline void setOfflineObjects(const std::uint16_t& id, const std::pair<std::uint16_t, xr_string>& pair) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setOfflineObjects(id, pair)] WARNING: id = std::uint16_t(-1)! You are trying to set "
                "an undefined value! No assignment!");
            return;
        }

        this->m_offline_objects[id] = pair;
    }

    // @ Lord: наименовать нормально аргументы
    inline void setOfflineObjects(const std::uint16_t& id, const std::uint16_t& _id, const xr_string& string) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setOfflineObjects(id, _id, string)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to set an undefined value! No assignment!");
            return;
        }

        this->m_offline_objects[id] = {_id, string};
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
    xr_map<std::uint16_t, std::uint32_t> m_spawned_vertex_by_id;
    // first -> sympathy[ID] = std::uint32_t; | second -> relations[ID] = std::string;
    std::pair<xr_map<std::uint16_t, float>, xr_map<std::uint16_t, xr_string>> m_goodwill;
};

} // namespace DataBase
} // namespace Scripts
} // namespace Cordis
