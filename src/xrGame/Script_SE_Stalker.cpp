#include "stdafx.h"
#include "Script_SE_Stalker.h"
#include "Script_StoryObject.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_Stalker::Script_SE_Stalker(LPCSTR section_name) : inherited(section_name) {}

Script_SE_Stalker::~Script_SE_Stalker(void) {}

bool Script_SE_Stalker::can_switch_online(void) const
{
    if (this->m_group_id != Globals::kUnsignedInt16Undefined)
        return true;

    return inherited::can_switch_online();
}

bool Script_SE_Stalker::can_switch_offline(void) const
{
    if (this->m_group_id != Globals::kUnsignedInt16Undefined)
        return true;

    return inherited::can_switch_offline();
}

void Script_SE_Stalker::STATE_Write(NET_Packet& packet)
{
    inherited::STATE_Write(packet);
    if (this->m_bOnline)
    {
        if (Globals::Game::level::get_object_by_id(this->ID) &&
            Globals::Game::level::get_object_by_id(this->ID)->level_vertex_id())
            packet.w_stringZ(
                std::to_string(Globals::Game::level::get_object_by_id(this->ID)->level_vertex_id()).c_str());
    }
    else
    {
        if (DataBase::Storage::getInstance()
                .getOfflineObjects()[this->ID]
                .second.size()) // Lord: нужно ли писать альтернативу?
        {
            packet.w_stringZ(
                std::to_string(DataBase::Storage::getInstance().getOfflineObjects()[this->ID].first).c_str());
            packet.w_stringZ(DataBase::Storage::getInstance().getOfflineObjects()[this->ID].second.c_str());
        }

        packet.w_u8(this->m_is_dropped_death ? 1 : 0);
    }
}

void Script_SE_Stalker::STATE_Read(NET_Packet& packet, std::uint16_t size)
{
    inherited::STATE_Read(packet, size);

    if (this->m_script_version > 10)
    {
        xr_string old_level_vertex_id_name = "";
        packet.r_stringZ(old_level_vertex_id_name);

        packet.r_stringZ(DataBase::Storage::getInstance().getOfflineObjects()[this->ID].second);
        if (DataBase::Storage::getInstance().getOfflineObjects()[this->ID].second == "nil")
        {
            DataBase::Storage::getInstance().getOfflineObjects()[this->ID].second.clear();
        }

        if (old_level_vertex_id_name != "nil")
        {
           // DataBase::Storage::getInstance().getOfflineObjects().at(this->ID).first =
           //     atoi(old_level_vertex_id_name.c_str());
            DataBase::Storage::getInstance().setOfflineObjects(this->ID, atoi(old_level_vertex_id_name.c_str()));
        }

        this->m_is_dropped_death = packet.r_u8() ? true : false;
    }
}

void Script_SE_Stalker::on_register(void)
{
    inherited::on_register();

    Script_StoryObject::getInstance().check_spawn_ini_for_story_id(this);

    this->m_is_registered = true;

    Msg("[Scripts/Script_SE_Stalker/on_register()] Registering stlaker %s", this->name_replace());

    CALifeHumanBrain* brain = &this->brain();
    brain->can_choose_alife_tasks(false);

    xr_string smart_name = Globals::Utils::cfg_get_string(&CScriptIniFile(this->spawn_ini().fname()), "logic", "smart_terrain");
    Script_SE_SmartTerrain* server_smart = Script_SimulationBoard::getInstance().getSmartTerrainsByName().at(smart_name);

    if (!server_smart)
        return;

    ai().alife().objects().object(server_smart->ID)->cast_script_se_smartterrain()->register_npc(this);
}

void Script_SE_Stalker::on_unregister(void)
{
    std::uint16_t smart_terrain_id = this->m_smart_terrain_id;

    if (smart_terrain_id != Globals::kUnsignedInt16Undefined)
    {
        Script_SE_SmartTerrain* smart = ai().alife().objects().object(smart_terrain_id)->cast_script_se_smartterrain();

        if (smart)
            smart->unregister_npc(this);
    }
    DataBase::Storage::getInstance().setOfflineObjects(this->ID, Globals::kUnsignedInt16Undefined, "");
    Script_StoryObject::getInstance().unregistrate_by_id(this->ID);

    inherited::on_unregister();
}

void Script_SE_Stalker::on_death(CSE_Abstract* killer)
{
    if (!killer)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (ai().get_alife())
    {
        CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(this->ID);

        if (!server_object)
        {
            //    R_ASSERT2(false, "object was null!");
            return;
        }

        std::uint16_t smart_terrain_id = server_object->cast_monster_abstract()->m_smart_terrain_id;

        if (smart_terrain_id != Globals::kUnsignedInt16Undefined)
        {
            Msg("[Scripts/Script_SE_Stalker/on_death(killer)] Clear dead object %s", this->name_replace());
            ai().alife().objects().object(this->ID)->cast_script_se_smartterrain()->clear_dead(this);
        }
    }

    if (this->m_group_id != Globals::kUnsignedInt16Undefined)
    {
        Script_SE_SimulationSquad* squad =
            ai().alife().objects().object(this->m_group_id)->cast_script_se_simulationsquad();

        if (!squad)
        {
            R_ASSERT2(false, "bad casting!");
            return;
        }

        squad->on_npc_death(this);
    }
}

} // namespace Scripts
} // namespace Cordis
