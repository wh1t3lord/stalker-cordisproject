#pragma once

#include "script_sound.h"

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

struct SubStorage_Data
{
    SubStorage_Data(void) = default;
    ~SubStorage_Data(void) = default;
    xr_map<xr_string, bool> m_signals;
    xr_vector<Script_ILogicEntity*> m_actions;
};

struct Storage_Data
{
    bool m_invulnerable = false;
    bool m_immortal = false;
    bool m_mute = false;
    bool m_enabled = false;
    bool m_anim_movement = false;
    CScriptGameObject* m_object = nullptr;
    CSE_ALifeObject* m_server_object = nullptr;
    CScriptSound* m_sound_object = nullptr;
    CInifile* m_ini = nullptr;
    xr_map<xr_string, SubStorage_Data> m_data;
    xr_string m_active_scheme = "";
    xr_string m_active_section = "";
    xr_string m_sound = "";
    xr_string m_animation = "";
    xr_string m_animation_head = "";
    xr_string m_tip = "";
    xr_string m_time = "";

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
            for (xr_map<xr_string, SubStorage_Data>::value_type& object : it.second.m_data)
            {
                if (object.second.m_actions.size())
                {
                    for (Script_ILogicEntity* entity : object.second.m_actions)
                    {
                        if (entity)
                        {
                            Msg("[Scripts/DataBase/Storage/~dtor] Deleting Script_IEntity: %s",
                                entity->m_logic_name.c_str());
                            delete entity;
                            entity = nullptr;
                        }
                    }

                    object.second.m_actions.clear();
                }

                object.second.m_signals.clear();
            }

            if (it.second.m_object)
            {
                Msg("[Scripts/DataBase/Storage/~dtor] Deleting the m_object: %s", it.second.m_object->Name());
                delete it.second.m_object;
                it.second.m_object = nullptr;
            }

            if (it.second.m_server_object)
            {
                Msg("[Scripts/DataBase/Storage/~dtor] Deleting the m_server_object: %s", it.second.m_server_object->name());
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

    inline xr_map<std::uint16_t, Storage_Data>& getStorage(void) noexcept { return this->m_storage; }
    inline CScriptGameObject* getActor(void) { return this->m_actor; }
    inline void setActor(CScriptGameObject* object)
    {
        if (!object)
        {
            R_ASSERT2(false, "Can't set actor because the object was null!");
            return;
        }

        this->m_actor = object;
    }
    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(Storage&&) = delete;

private:
    CScriptGameObject* m_actor;
    xr_map<std::uint16_t, Storage_Data> m_storage;
};

} // namespace DataBase
} // namespace Scripts
} // namespace Cordis
