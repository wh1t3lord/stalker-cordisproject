#pragma once

#include "xrCore/xrCore.h"

namespace Cordis
{
namespace Scripts
{
class Script_StoryObject
{
    Script_StoryObject(void) : m_id_by_story_id(), m_story_id_by_id() {}

public:
    inline static Script_StoryObject& getInstance(void) noexcept
    {
        static Script_StoryObject instance;
        return instance;
    }

    ~Script_StoryObject(void);
    Script_StoryObject(const Script_StoryObject&) = delete;
    Script_StoryObject& operator=(const Script_StoryObject&) = delete;
    Script_StoryObject(Script_StoryObject&&) = delete;
    Script_StoryObject& operator=(Script_StoryObject&&) = delete;
    
    inline std::uint16_t get(const xr_string& story_id) { return this->m_id_by_story_id[story_id]; }
    inline xr_string get_story_id(std::uint16_t id) { return this->m_story_id_by_id[id]; }

    void registrate(std::uint16_t id, const xr_string& story_id);
    void unregistrate_by_id(std::uint16_t id);
    void unregistrate_by_story_id(const xr_string& story_id);
    void save(NET_Packet& packet);
    void load(NET_Packet& packet);
    void check_spawn_ini_for_story_id(CSE_ALifeDynamicObject* item);
    //  void register(std::uint16_t id, CLASS_ID story_id);
    
private:
    xr_map<xr_string, std::uint16_t> m_id_by_story_id;
    xr_map<std::uint16_t, xr_string> m_story_id_by_id;
};
} // namespace Scripts
} // namespace Cordis
