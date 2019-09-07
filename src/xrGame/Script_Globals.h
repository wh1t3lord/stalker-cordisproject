#pragma once

#include "xrCore/xrCore.h"
#include "script_ini_file.h"
#include "xrEngine/IGame_Level.h"
#include "ai_space.h"
#include "date_time.h"
#include "alife_simulator_base.h"
#include "alife_object_registry.h"
#include "alife_graph_registry.h"
#include "alife_human_brain.h"
#include "alife_time_manager.h"
#include "relation_registry.h"
#include "InfoPortionDefs.h"
#include "xrServer_Objects_ALife_All.h"
#include "xrServer_Objects_Alife_Smartcovers.h"
#include "script_movement_action.h"
#include "xr_time.h"
#include <random>
#include "Script_GlobalDefinitions.h"
#include "xrAICore/AISpaceBase.hpp"
#include "xrAICore/Navigation/PatrolPath/patrol_path_storage.h"
#include "xrAICore/Navigation/PatrolPath/patrol_path_params.h"
#include "Level.h"
#include "map_manager.h"
#include "xrAICore/Navigation/game_graph.h"
#include "script_ini_file.h"
#include "Script_Database.h"
#include "Script_SE_SimulationSquad.h"

namespace Cordis
{
namespace Scripts
{
namespace Globals
{
#pragma region Cordis Custom Datas
/*
struct ReachTarget
{
    std::uint16_t m_squad_id = kUnsignedIntUndefined;
    xr_string m_name = "reach_target";

    inline void Clear(void) noexcept
    {
        this->m_squad_id = kUnsignedIntUndefined;
        this->m_name.clear();
    }

    inline bool update(const bool& is_under_simulation)
    {
        // Lord: нужно ли делать метод cast_to_script_se_simulationsquad в CSE_ALifeDynamicObject
        Script_SE_SimulationSquad* squad = (Script_SE_SimulationSquad*)ai().alife().objects().object(this->m_squad_id);
        Script_SE_SmartTerrain* squad_target =
Script_SimulationObjects::getInstance().getObjects()[squad->getAssignedTargetID()]->cast_script_se_smartterrain();

        if (!squad_target)
        {
            // R_ASSERT2(false, "object was null");
            squad->setAssignedTargetID(0);
            return true;
        }

        if (!is_under_simulation)
            squad_target = ai().alife().objects().object(squad->getAssignedTargetID())->cast_script_se_smartterrain();
    }
};*/

#pragma endregion

namespace Utils
{
inline xr_string cfg_get_string(CScriptIniFile* char_ini, const xr_string& section, const xr_string& field,
    bool mandatory = false, const xr_string& gulag_name = "")
{
    if (section.size() &&
        (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str())))
    {
        xr_string result = "";
        result.append(gulag_name);
        result.append("_");
        result.append(char_ini->r_string(section.c_str(), field.c_str()));
        if (gulag_name.size())
            return result;
        else
            return char_ini->r_string(section.c_str(), field.c_str());
    }

    if (!mandatory)
        return "";

    Msg("Attempt to read a non-existant string field %s in section %s", field.c_str(), section.c_str());
    R_ASSERT(false);

    return "";
}

inline float graph_distance(const std::uint16_t& gamevertexid_1, const std::uint16_t& gamevertexid_2)
{
    return ai()
        .game_graph()
        .vertex(gamevertexid_1)
        ->game_point()
        .distance_to(ai().game_graph().vertex(gamevertexid_2)->game_point());
}

inline bool cfg_get_bool(CScriptIniFile* char_ini, const xr_string& section, const xr_string& field,
    CScriptGameObject* object = nullptr, bool mandatory = false)
{
    xr_string object_name;

    if (!section.size() || !field.size())
    {
        R_ASSERT2(false, "string can't be null");
        return false;
    }

    if (!object)
        object_name = "Unknown Object";
    else
        object_name = object->Name();

    if (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str()))
    {
        bool result = char_ini->r_bool(section.c_str(), field.c_str());
        return result;
    }

    if (!mandatory)
    {
        return false;
    }

    Msg("[Script]: ERROR object %s attempt to read a non-existant boolean field %s in section %s", object_name.c_str(),
        field.c_str(), section.c_str());
    R_ASSERT(false);
    return false;
}

inline bool cfg_get_bool(CScriptIniFile* char_ini, const xr_string& section, const xr_string& field,
    CSE_ALifeDynamicObject* object, bool mandatory = false)
{
    xr_string object_name;

    if (!section.size() || !field.size())
    {
        R_ASSERT2(false, "string can't be null");
        return false;
    }

    if (!object)
        object_name = "Unknown Object";
    else
        object_name = object->name();

    if (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str()))
    {
        bool result = char_ini->r_bool(section.c_str(), field.c_str());
        return result;
    }

    if (!mandatory)
    {
        return false;
    }

    Msg("[Script]: ERROR object %s attempt to read a non-existant boolean field %s in section %s", object_name.c_str(),
        field.c_str(), section.c_str());
    R_ASSERT(false);
    return false;
}

inline float cfg_get_number(CScriptIniFile* char_ini, const xr_string& section, const xr_string& field,
    CScriptGameObject* object = nullptr, bool mandatory = false)
{
    xr_string object_name;

    if (!section.size() || !field.size())
    {
        R_ASSERT2(false, "string can't be null!");
        return 0.0f;
    }

    if (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str()))
        return char_ini->r_float(section.c_str(), field.c_str());

    Msg("[Script] cfg_get_number has returned a default value");
    return 0.0f;
}

inline float cfg_get_number(CScriptIniFile* char_ini, const xr_string& section, const xr_string& field,
    CSE_ALifeDynamicObject* object, bool mandatory = false)
{
    xr_string object_name;

    if (!section.size() || !field.size())
    {
        R_ASSERT2(false, "string can't be null!");
        return 0.0f;
    }

    if (char_ini->section_exist(section.c_str()) && char_ini->line_exist(section.c_str(), field.c_str()))
        return char_ini->r_float(section.c_str(), field.c_str());

    Msg("[Script] cfg_get_number has returned a default value");
    return 0.0f;
}

// xr_string cfg_get_string(const CInifile& char_ini, const xr_string& section, const xr_string& field,
//     bool mandatory = false, const xr_string& gulag_name = "")
// {
//     if (section.size() &&
//         (char_ini.section_exist(section.c_str()) && char_ini.line_exist(section.c_str(), field.c_str())))
//     {
//         xr_string result = "";
//         result.append(gulag_name);
//         result.append("_");
//         result.append(char_ini.r_string(section.c_str(), field.c_str()));
//         if (gulag_name.size())
//             return result;
//         else
//             return char_ini.r_string(section.c_str(), field.c_str());
//     }
//
//     if (!mandatory)
//         return "";
//
//     Msg("Attempt to read a non-existant string field %s in section %s", field.c_str(), section.c_str());
//     R_ASSERT(false);
//
//     return "";
// }
inline xr_string get_scheme_by_section(xr_string& data)
{
    if (!data.size())
    {
        R_ASSERT2(false, "string can't be empty!");
        return xr_string("sr_idle");
    }

    xr_string result = data;
    if (result.find('@') == xr_string::npos)
    {
        R_ASSERT2(false, "invalid string!");
        return xr_string("");
    }

    return result.erase(result.find('@'));
}

inline xr_vector<xr_string> parse_names(const xr_string& buffer)
{
    xr_vector<xr_string> result;
    if (!buffer.size())
    {
        R_ASSERT2(false, "You are trying to parse an empty string!");
        return result;
    }

    const char* pattern = "[^,]+";
    boost::regex regex(pattern);

    boost::sregex_iterator it(buffer.begin(), buffer.end(), regex);
    boost::sregex_iterator end;

    for (; it != end; ++it)
    {
        xr_string value_name = it->str().c_str();
        boost::algorithm::trim(value_name);

        result.push_back(value_name);
    }

    return result;
}

inline void r_2nums(const CInifile& spawn_ini, const xr_string& section_name, const xr_string& line_name,
    std::uint32_t& number_min, std::uint32_t& number_max)
{
    if (spawn_ini.line_exist(section_name.c_str(), line_name.c_str()))
    {
        xr_vector<xr_string> buffer = parse_names(spawn_ini.r_string(section_name.c_str(), line_name.c_str()));
        std::uint32_t count = buffer.size();

        if (count == 0)
            return;

        if (count == 1)
        {
            number_min = atoi(buffer[0].c_str());
        }
        else if (count > 1)
        {
            number_min = atoi(buffer[0].c_str());
            number_max = atoi(buffer[1].c_str());
        }
    }
}

// first -> section | second -> prob
inline xr_vector<std::pair<xr_string, float>> parse_spawns(const xr_string& buffer)
{
    xr_vector<std::pair<xr_string, float>> result;

    if (!buffer.size())
    {
        R_ASSERT2(false, "string can't be empty!");
        return result;
    }

    xr_vector<xr_string> buffer_names = parse_names(buffer);

    std::uint32_t count = buffer_names.size();

    std::uint32_t it = 0;

    while (it <= count)
    {
        std::pair<xr_string, std::uint32_t> data;
        data.first = buffer_names[it];

        // Lord: наверное лучше сделать через try
        if (buffer_names[it + 1].size())
        {
            std::uint32_t value = atoi(buffer_names[it + 1].c_str());

            if (value)
            {
                data.second = value;
                it += 2;
            }
            else
            {
                data.second = 1.0f;
                it += 1;
            }
        }
        else
        {
            data.second = 1.0f;
            it += 1;
        }

        result.push_back(data);
    }

    return result;
}

inline bool is_npc_in_zone(CScriptGameObject* npc, CScriptGameObject* zone)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!zone)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return zone->inside(npc->Position());
}

inline bool is_npc_in_zone(CSE_ALifeDynamicObject* server_object, CScriptGameObject* zone)
{
    if (!server_object)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!zone)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return zone->inside(server_object->Position());
}

} // namespace Utils
namespace Game
{
inline LPCSTR translate_string(LPCSTR str) { return *StringTable().translate(str); }
inline xrTime get_game_time(void) noexcept { return get_time_struct(); }
inline const CGameGraph* get_game_graph() { return &GEnv.AISpace->game_graph(); }
inline CSE_Abstract* alife_create(
    const xr_string& section, const Fvector& position, std::uint32_t level_vertex_id, std::uint16_t game_vertex_id)
{
    CALifeSimulator* alife = const_cast<CALifeSimulator*>(ai().get_alife());

    VERIFY(alife);

    return alife->spawn_item(section.c_str(), position, level_vertex_id, game_vertex_id, std::uint16_t(-1));
}

inline CSE_Abstract* alife_create(const xr_string& section, const Fvector& position,
    const std::uint32_t& level_vertex_id, const std::uint16_t& game_vertex_id, const std::uint16_t& parent_id)
{
    if (parent_id == kUnsignedInt16Undefined)
        return alife_create(section, position, level_vertex_id, game_vertex_id);

    CSE_ALifeDynamicObject* object = ai().alife().objects().object(parent_id, true);
    if (!object)
    {
        Msg("! invalid parent id [%d] specified", parent_id);
        return (0);
    }

    CALifeSimulator* alife = const_cast<CALifeSimulator*>(ai().get_alife());

    if (!object->m_bOnline)
        return (alife->spawn_item(section.c_str(), position, level_vertex_id, game_vertex_id, parent_id));

    NET_Packet packet;
    packet.w_begin(M_SPAWN);
    packet.w_stringZ(section.c_str());

    CSE_Abstract* item =
        alife->spawn_item(section.c_str(), position, level_vertex_id, game_vertex_id, parent_id, false);
    item->Spawn_Write(packet, FALSE);
    alife->server().FreeID(item->ID, 0);
    F_entity_Destroy(item);

    ClientID clientID;
    clientID.set(0xffff);

    u16 dummy;
    packet.r_begin(dummy);
    VERIFY(dummy == M_SPAWN);
    return (alife->server().Process_spawn(packet, clientID));
}

inline xr_string get_squad_relation_to_actor_by_id(const std::uint16_t& squad_id)
{
    Script_SE_SimulationSquad* squad = ai().alife().objects().object(squad_id)->cast_script_se_simulationsquad();

    if (!squad)
    {
        Msg("[Scripts/Globals/Game/get_squad_relation_to_actor_by_id(squad_id)] No scuh squad %s in board",
            std::to_string(squad_id).c_str());
        R_ASSERT(false);
        return "";
    }

    int goodwill = 0;
    int npc_count = 0;

    CScriptGameObject* actor = DataBase::Storage::getInstance().getActor();

    for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
             squad->squad_members().begin();
         it != squad->squad_members().end(); ++it)
    {
        CScriptGameObject* client_object = DataBase::Storage::getInstance().getStorage()[it->first].m_object;

        if (client_object && actor)
        {
            goodwill += client_object->GetAttitude(actor);
            npc_count += 1;
        }
    }

    if (npc_count)
    {
        int delta = goodwill / npc_count;

        if (delta <= kRelationKoeffEnemy)
            return kRelationsTypeEnemy;

        if (delta >= kRelationKoeffFriend)
            return kRelationsTypeFriends;

        if (delta < kRelationKoeffFriend && delta > kRelationKoeffEnemy)
            return kRelationsTypeNeutral;
    }

    return kRelationsTypeEnemy;
}

inline std::uint32_t time_global(void) noexcept { return Device.dwTimeGlobal; }

namespace level
{
inline CScriptGameObject* get_object_by_id(u16 id)
{
    CGameObject* pGameObject = smart_cast<CGameObject*>(Level().Objects.net_Find(id));
    if (!pGameObject)
        return nullptr;

    return pGameObject->lua_game_object();
}

inline LPCSTR get_name(void) { return (*Level().name()); }
inline void map_remove_object_spot(const std::uint16_t& id, LPCSTR spot_type)
{
    Level().MapManager().RemoveMapLocation(spot_type, id);
}

inline void map_add_object_spot(const std::uint16_t& id, LPCSTR spot_type, LPCSTR text)
{
    CMapLocation* ml = Level().MapManager().AddMapLocation(spot_type, id);
    if (xr_strlen(text))
    {
        ml->SetHint(text);
    }
}

inline std::uint16_t map_has_object_spot(const std::uint16_t& id, LPCSTR spot_type)
{
    return Level().MapManager().HasMapLocation(spot_type, id);
}

inline void map_add_object_spot_ser(const std::uint16_t& id, LPCSTR spot_type, LPCSTR text)
{
    CMapLocation* ml = Level().MapManager().AddMapLocation(spot_type, id);
    if (xr_strlen(text))
        ml->SetHint(text);

    ml->SetSerializable(true);
}

inline void map_change_spot_hint(const std::uint16_t& id, LPCSTR spot_type, LPCSTR text)
{
    CMapLocation* ml = Level().MapManager().GetMapLocation(spot_type, id);
    if (!ml)
        return;
    ml->SetHint(text);
}

} // namespace level

} // namespace Game
namespace GameRelations
{
inline void set_npc_sympathy(CScriptGameObject* npc, float new_sympathy)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (new_sympathy < 0.0f)
        new_sympathy = 0.0f;

    if (new_sympathy > 1.0f)
        new_sympathy = 1.0f;

    npc->SetSympathy(new_sympathy);
}

inline void set_npcs_relation(
    CScriptGameObject* client_from_object1, CScriptGameObject* client_to_object2, xr_string& new_relation_name)
{
    if (!client_from_object1)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (!client_to_object2)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (!new_relation_name.c_str())
    {
        Msg("[Scripts/Globals/GameRelations/set_npcs_relation(server_object, client_object, new_relation_name)] "
            "WARNING: new_relation_name was an empty string! Set default value => [%s]",
            kRelationsTypeNeutral);
        new_relation_name = kRelationsTypeNeutral;
    }

    int goodwill = 0;

    if (new_relation_name == kRelationsTypeEnemy)
        goodwill = -1000;
    else if (new_relation_name == kRelationsTypeFriends)
        goodwill = 1000;

    RELATION_REGISTRY().ForceSetGoodwill(client_from_object1->ID(), client_to_object2->ID(), goodwill);
}

inline void set_npcs_relation(CSE_ALifeMonsterAbstract* server_from_object, CSE_ALifeMonsterAbstract* server_to_object,
    xr_string& new_relation_name)
{
    if (!server_from_object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (!server_to_object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (!new_relation_name.c_str())
    {
        Msg("[Scripts/Globals/GameRelations/set_npcs_relation(server_object, client_object, new_relation_name)] "
            "WARNING: new_relation_name was an empty string! Set default value => [%s]",
            kRelationsTypeNeutral);
        new_relation_name = kRelationsTypeNeutral;
    }

    int goodwill = 0;

    if (new_relation_name == kRelationsTypeEnemy)
        goodwill = -1000;
    else if (new_relation_name == kRelationsTypeFriends)
        goodwill = 1000;

    RELATION_REGISTRY().ForceSetGoodwill(server_from_object->ID, server_to_object->ID, goodwill);
}

} // namespace GameRelations

inline int get_script_clsid(const CLASS_ID& clsid);
inline Fvector vertex_position(u32 level_vertex_id);
inline std::uint32_t vertex_in_direction(
    const std::uint32_t& level_vertex_id, const Fvector& direction, const float& max_distance);
inline float yaw(const Fvector& vector1, const Fvector& vector2);
inline float yaw_degree(const Fvector& vector1, const Fvector& vector2);
// @ 2D
inline Fvector vector_cross(const Fvector& vector1, const Fvector& vector2);
inline Fvector vector_rotate_y(Fvector& vector, float& angle);
inline bool patrol_path_exists(LPCSTR patrol_path);
inline void load_sound(void);
inline void init_npc_sound(CScriptGameObject* npc);
inline bool has_alife_info(LPCSTR info_id);
inline int get_general_goodwill_between(const std::uint16_t& from, const std::uint16_t& to);
inline bool check_squad_for_enemies(CSE_ALifeOnlineOfflineGroup* squad);
inline bool IsMonster(CScriptGameObject* object, int class_id);
inline bool IsStalker(CScriptGameObject* object, int class_id);
inline bool IsStalker(CSE_ALifeDynamicObject* server_object, int class_id);
inline bool IsArtefact(CScriptGameObject* object, int class_id);
inline bool IsWeapon(CScriptGameObject* object, int class_id);
inline xr_string character_community(CScriptGameObject* object);
inline void change_team_squad_group(CSE_ALifeDynamicObject* server_object, const std::uint8_t& team,
    const std::uint8_t& group, const std::uint8_t& squad);

constexpr const char* kSaveMarkerMode_Save = "save";
constexpr const char* kSaveMarkerMode_Load = "load";

inline bool is_level_present(void);

inline bool is_device_paused(void);

inline void set_device_paused(bool b);

inline const CInifile* get_system_ini(void);

#pragma region Instances
inline static xr_map<xr_string, unsigned int>& SaveMarkers(void) noexcept;
inline static xr_map<xr_string, xr_string>& Schemes(void) noexcept;
inline static xr_map<xr_string, unsigned int>& STypes(void) noexcept;
// @ Uses when parsing condlists
inline static xr_map<xr_string, Cordis::AnyCallable<void>>& getXREffects_Functions(void) noexcept;
#pragma endregion

inline void load_scheme(const xr_string& filename, const xr_string& scheme, unsigned int stype);
inline void start_game_callback(void);
inline void set_save_marker(NET_Packet& packet, const xr_string& mode, bool check, const xr_string& prefix);
// @ from simulation_objects
inline bool is_on_the_same_level(CSE_ALifeObject* object1, CSE_ALifeObject* object2);
inline float sim_dist_to(CSE_ALifeObject* object1, CSE_ALifeObject* object2);
inline ESingleGameDifficulty get_game_difficulty();
inline std::uint32_t get_time_days();
inline std::uint32_t get_time_hours();
inline std::uint32_t get_time_minutes();
inline bool in_time_interval(const std::uint32_t& value1, const std::uint32_t& value2);
inline xr_string get_job_restrictor(const xr_string& waypoint_name);
inline xr_string get_job_restrictor(const char* waypoint_name);
inline bool is_accessible_job(CSE_ALifeDynamicObject* server_object, const xr_string& waypoint_name);
inline bool is_accessible_job(CSE_ALifeDynamicObject* server_object, const char* waypoint_name);
} // namespace Globals
} // namespace Scripts
} // namespace Cordis

#include "Script_SimulationObjects.h"
#include "Script_EntitySounds.h"
#include "Script_SE_Actor.h"
#include "Script_SE_Monster.h"
#include "Script_SE_SmartCover.h"
#include "Script_SE_Outfit.h"
#include "Script_SE_Artefact.h"
#include "Script_SE_Helmet.h"
#include "Script_SE_Helicopter.h"
#include "Script_SE_Weapon.h"
#include "Script_SE_WeaponShotgun.h"
#include "Script_SE_WeaponAutomaticShotgun.h"
#include "Script_SE_WeaponMagazined.h"
#include "Script_SE_WeaponMagazinedWGL.h"
#include "Script_SE_Item.h"
#include "Script_SE_ItemTorch.h"
#include "Script_SE_Squad.h"
#include "Script_SE_Physic.h"
#include "Script_SE_Lamp.h"
#include "Script_SE_Ammo.h"
#include "Script_SE_Grenade.h"
#include "Script_SE_Eatable.h"
#include "Script_SE_InventoryBox.h"
#include "Script_SE_Explosive.h"
#include "Script_SE_PDA.h"
#include "Script_SE_Detector.h"
#include "Script_SE_LevelChanger.h"
#include "Script_SE_NewAttachableItem.h"
#include "Script_SE_Stalker.h"
#include "Script_SE_WeaponAutomaticShotgun.h"
#include "Script_SE_Restrictor.h"
#include "Script_SE_ZoneVisual.h"
#include "Script_SE_ZoneTorrid.h"
#include "Script_SE_ZoneAnomaly.h"
#include "Script_SmartTerrainControl.h"
/*#include "Script_GulagGenerator.h"*/
#include "Script_SE_SmartTerrain.h"

#include "Script_LogicEntity.h"
#include "Script_LogicManager.h"
#include "Script_TreasureManager.h"
#include "Script_SoundManager.h"
#include "Script_NewsManager.h"
#include "Script_XR_Condition.h"
#include "Script_XR_Gulag.h"
/*#include "Script_XR_Logic.h"*/
#include "Script_XR_Sound.h"
#include "Script_XR_Effects.h"
#include "Script_XR_Meet.h"
#include "Script_XR_Patrol.h"
#include "Script_SimulationBoard.h"
#include "Script_SurgeManager.h"
#include "Script_EntitySounds.h"
#include "Script_Globalsinline.h"
