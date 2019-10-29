#include "stdafx.h"
#include "Script_StoryObject.h"
#include "ai_space.h"
#include "alife_simulator_base.h"
#include "alife_object_registry.h"

namespace Cordis
{
namespace Scripts
{
Script_StoryObject::~Script_StoryObject(void)
{
    this->m_id_by_story_id.clear();
    this->m_story_id_by_id.clear();
}

void Script_StoryObject::registrate(std::uint16_t id, const xr_string& story_id)
{
    if (story_id.empty())
    {
        // Lord: добавить сюда логирование прям в игру!
        R_ASSERT2(false, "You're trying to add null object!");
        return;
    }

    if (this->m_id_by_story_id[story_id])
    {
        if (id != this->m_id_by_story_id[story_id])
        {
            xr_string exist_object_name = ai().get_alife()->objects().object(this->m_id_by_story_id[story_id])->name();
            xr_string adding_object_name = ai().get_alife()->objects().object(id)->name();

            Msg("You're trying to spawn two or more objects with the same story_id: [%s] --> [%s] try to add: "
                "adding_object_name",
                story_id.c_str(), exist_object_name.c_str());
            R_ASSERT(false);
        }
    }

    if (this->m_story_id_by_id[id].size())
    {
        if (this->m_story_id_by_id[id] != story_id)
        {
            Msg("Object [%s] is already in Script_StoryObject instance with story_id [%s]", std::to_string(id).c_str(),
                story_id.c_str());
            R_ASSERT(false);
            //   R_ASSERT4(false, "Object [%s] is already in Script_StoryObject instance with story_id [%s]",
            //     std::to_string(id).c_str(), story_id.c_str());
        }
    }

    this->m_id_by_story_id[story_id] = id;
    this->m_story_id_by_id[id] = story_id;
}

void Script_StoryObject::unregistrate_by_id(std::uint16_t id)
{
    if (this->m_story_id_by_id[id].size())
    {
        this->m_id_by_story_id[this->m_story_id_by_id[id]] = 0;
        this->m_story_id_by_id[id] = "";
    }
}

void Script_StoryObject::unregistrate_by_story_id(const xr_string& story_id)
{
    if (this->m_id_by_story_id[story_id])
    {
        this->m_story_id_by_id[this->m_id_by_story_id[story_id]] = "";
        this->m_id_by_story_id[story_id] = 0;
    }
}

void Script_StoryObject::save(NET_Packet& packet)
{
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, false, "story_objects");
    std::uint16_t count = 0;

    for (const xr_map<xr_string, std::uint16_t>::value_type& it : this->m_id_by_story_id)
    {
        count += 1;
    }

    if (count > 65534)
    {
        R_ASSERT2(false, "There is too many story_ids!!!! (m_id_by_story_id)");
    }

    packet.w_u16(count);
    for (const xr_map<xr_string, std::uint16_t>::value_type& it : this->m_id_by_story_id)
    {
        packet.w_stringZ(it.first.c_str());
        packet.w_u16(it.second);
    }
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, true, "story_objects");
}

void Script_StoryObject::load(NET_Packet& packet)
{
    Globals::set_save_marker(packet, "load", false, "story_objects");
    unsigned int count = packet.r_u16();
    // Lord: ведь не с нуля?
    for (unsigned int i = 0; i < count; ++i)
    {
        xr_string story_id = "";
        packet.r_stringZ(story_id);
        std::uint16_t obj_id;
        packet.r_u16(obj_id);

        this->m_id_by_story_id[story_id] = obj_id;
        this->m_story_id_by_id[obj_id] = story_id;
    }

    Globals::set_save_marker(packet, "load", false, "story_objects");
}

void Script_StoryObject::check_spawn_ini_for_story_id(CSE_ALifeDynamicObject* item)
{
    if (!item)
    {
        R_ASSERT2(false, "item was null!");
        return;
    }

    CInifile& spawn_ini = item->spawn_ini();

    if (spawn_ini.section_exist("story_object"))
    {
        pcstr* N;
        pcstr* V;
        shared_str section = "story_object";
        spawn_ini.r_line(section.c_str(), 0, N, V);
        xr_string id = *N;
        xr_string value = *V;

        if (id != "story_object")
        {
            R_ASSERT3(false, "There is no 'story_id' field in [story_object] section: object -> ", item->name_replace());
        }

        if (!value.size())
        {
            R_ASSERT3(false, "A field 'story_id' in [story_object] section got no value: object -> ", item->name_replace());
        }
        this->registrate(item->ID, value);
        return;
    }

    CScriptIniFile _ini(Globals::kSystemLtxFileName);
    xr_string spawn_section = item->name();  
    xr_string story_id = Globals::Utils::cfg_get_string(&_ini, spawn_section, "story_id");

    if (story_id.size())
        this->registrate(item->ID, story_id);
    
}

} // namespace Scripts
} // namespace Cordis
