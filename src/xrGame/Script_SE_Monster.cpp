#include "stdafx.h"
#include "Script_SE_Monster.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_Monster::Script_SE_Monster(LPCSTR section_name) : inherited(section_name), m_is_registered(false) { Msg("[Scripts/Script_SE_Monster/ctor(section_name)] %s", section_name); }

Script_SE_Monster::~Script_SE_Monster(void) {}

void Script_SE_Monster::on_register(void)
{
    inherited::on_register();
    Msg("[Scripts/Script_SE_Monster/on_register()] registering monster -> %s ", this->name_replace());

    this->m_is_registered = true;
    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);
    // отключаем старый метод выбора smart_terrain
    this->brain().can_choose_alife_tasks(false);

    xr_string smart_name = Globals::Utils::cfg_get_string(this->spawn_ini(), "logic", "smart_terrain");
    Script_SE_SmartTerrain* server_smart_object = nullptr; 
    if (smart_name.size())
        server_smart_object = Script_SimulationBoard::getInstance().getSmartTerrainsByName().at(smart_name);

    if (!server_smart_object)
        return;

    // Lord: полагаю что это так ')
    ai().alife().objects().object(server_smart_object->ID)->cast_script_se_smartterrain()->register_npc(this);
}

void Script_SE_Monster::on_unregister(void)
{
    std::uint16_t smart_terrain_id = this->m_smart_terrain_id;

    if (smart_terrain_id != 65535)
    {
        Script_SE_SmartTerrain* server_smart_object =
            ai().alife().objects().object(smart_terrain_id)->cast_script_se_smartterrain();
        if (server_smart_object)
            server_smart_object->unregister_npc(this);
    }

    DataBase::Storage::getInstance().setOfflineObjects(this->ID, Globals::kUnsignedInt16Undefined, "");

    Script_StoryObject::getInstance().unregistrate_by_id(this->ID);
    inherited::on_unregister();
}

void Script_SE_Monster::on_death(CSE_Abstract* killer)
{
    if (!killer)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    inherited::on_death(killer);

    if (ai().get_alife())
    {
        CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(this->ID);

        if (!server_object)
        {
            //    R_ASSERT2(false, "object was null!");
            return;
        }

        std::uint16_t smart_terrain_id = server_object->cast_monster_abstract()->m_smart_terrain_id;

        if (smart_terrain_id != 65535)
        {
            Msg("[Scripts/Script_SE_Monster/on_death(killer)] Clear dead object %s", this->name_replace());
            ai().alife().objects().object(this->ID)->cast_script_se_smartterrain()->clear_dead(this);
        }
    }

    if (this->m_group_id != Globals::kUnsignedInt16Undefined)
    {
        Script_SE_SimulationSquad* squad =
            ai().alife().objects().object(this->m_group_id)->cast_script_se_simulationsquad();

        if (!squad)
        {
            R_ASSERT2(false, "there is no squad with id above!");
            return;
        }

        squad->on_npc_death(this);
    }
}

void Script_SE_Monster::STATE_Read(NET_Packet& packet, std::uint16_t size)
{
    inherited::STATE_Read(packet, size);

    if (this->m_script_version > 10)
    {
        xr_string old_level_vertex_id_name = "";
        packet.r_stringZ(old_level_vertex_id_name);

        const std::pair<std::uint16_t, xr_string>& offline_objects_data =
            DataBase::Storage::getInstance().getOfflineObjects().at(this->ID);

        packet.r_stringZ(xr_string(offline_objects_data.second.c_str()));

        if (offline_objects_data.second == "nil")
            DataBase::Storage::getInstance().setOfflineObjects(this->ID, "");
        //    offline_objects_data.second.clear();

        if (old_level_vertex_id_name != "nil")
            DataBase::Storage::getInstance().setOfflineObjects(this->ID, atoi(old_level_vertex_id_name.c_str()));
        //    offline_objects_data.first = atoi(old_level_vertex_id_name.c_str());
    }
}

void Script_SE_Monster::STATE_Write(NET_Packet& packet)
{
    inherited::STATE_Write(packet);

    if (this->m_bOnline)
    {
        if (Globals::Game::level::get_object_by_id(this->ID) &&
            Globals::Game::level::get_object_by_id(this->ID)->level_vertex_id())
            packet.w_stringZ(
                std::to_string(Globals::Game::level::get_object_by_id(this->ID)->level_vertex_id()).c_str());
        //packet.w_stringZ()
    }
    else
    {
        if (DataBase::Storage::getInstance().getOfflineObjects().find(this->ID) != DataBase::Storage::getInstance().getOfflineObjects().end()) 
            packet.w_stringZ(std::to_string((DataBase::Storage::getInstance().getOfflineObjects().at(this->ID).first ? DataBase::Storage::getInstance().getOfflineObjects().at(this->ID).first : 0)).c_str());
    }

    if (DataBase::Storage::getInstance().getOfflineObjects().find(this->ID) !=
        DataBase::Storage::getInstance().getOfflineObjects().end())
        packet.w_stringZ(DataBase::Storage::getInstance().getOfflineObjects().at(this->ID).second.size() ?
                DataBase::Storage::getInstance().getOfflineObjects().at(this->ID).second.c_str() :
                "nil");
}

bool Script_SE_Monster::can_switch_offline(void) const 
{
    if (this->m_group_id != Globals::kUnsignedInt16Undefined)
        return true;

    return inherited::can_switch_offline(); 
}

bool Script_SE_Monster::can_switch_online(void) const 
{
    if (this->m_group_id != Globals::kUnsignedInt16Undefined)
        return true;

    return inherited::can_switch_online();
}

} // namespace Scripts
} // namespace Cordis
