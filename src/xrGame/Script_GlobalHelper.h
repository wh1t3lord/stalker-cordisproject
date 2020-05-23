#pragma once

#include "object_factory.h"
#include "Script_XR_Patrol.h"
#include "Script_SmartTerrainControl.h"
#include "Script_ISchemeEntity.h"
#include "Script_ISchemeMonster.h"
#include "Script_ISchemeStalker.h"
#include "Script_MobStateManager.h"
#include "Script_HelicopterLook.h"
#include "Script_HelicopterFly.h"
#include "Script_HelicopterFire.h"
#include "Script_SchemeHelicopterMove.h"
#include "Script_SE_SmartTerrain.h"
#include "Script_GlobalDefinitions.h"

// @ PRIVATE PREPROCESSOR!
#define _REGISTER_FULL_FUNCTION_XR_CONDITION(function_name, function)                                       \
    this->m_registered_functions_xr_conditions[#function_name "_client"] = function##_client;               \
    this->m_registered_functions_xr_conditions[#function_name "_client_server"] = function##_client_server; \
    this->m_registered_functions_xr_conditions[#function_name "_server"] = function##_server;

// @ _client & _server
#define _REGISTER_CS_FUNCTION_XR_CONDITION(function_name, function)                                       \
    this->m_registered_functions_xr_conditions[#function_name "_client"] = function##_client;               \
    this->m_registered_functions_xr_conditions[#function_name "_server"] = function##_server;

// @ _client & _client_server
#define _REGISTER_CCS_FUNCTION_XR_CONDITION(function_name, function)                                       \
    this->m_registered_functions_xr_conditions[#function_name "_client"] = function##_client;               \
    this->m_registered_functions_xr_conditions[#function_name "_client_server"] = function##_client_server;

// @ _client
#define _REGISTER_C_FUNCTION_XR_CONDITION(function_name, function)                                       \
    this->m_registered_functions_xr_conditions[#function_name "_client"] = function##_client; 

// @ _client
#define _REGISTER_C_PURE_FUNCTION_XR_CONDITION(function_name, function)                                       \
    this->m_registered_functions_xr_conditions[#function_name "_client"] = function; 

// @ _server
#define _REGISTER_S_FUNCTION_XR_CONDITION(function_name, function)                                       \
    this->m_registered_functions_xr_conditions[#function_name "_server"] = function##_server; 

// @ _server
#define _REGISTER_S_PURE_FUNCTION_XR_CONDITION(function_name, function)                                       \
    this->m_registered_functions_xr_conditions[#function_name "_server"] = function; 

// @ _client_server & _server
#define _REGISTER_CSS_FUNCTION_XR_CONDITION(function_name, function)                                       \
    this->m_registered_functions_xr_conditions[#function_name "_client_server"] = function##_client_server;               \
    this->m_registered_functions_xr_conditions[#function_name "_server"] = function##_server;

// @ PRIVATE PREPROCESSOR!
#define _REGISTER_FUNCTION_DIALOG_MANAGER(function_name, function) \
    this->m_registered_functions_dialog_manager[#function_name] = function;

namespace Cordis
{
namespace Scripts
{
// Lord: удалить все '// return;'
// @ Сюда не добавлять сущности которые могут относится к CScriptGameObject, CSE_ALife, ибо всё это должно находится в
// DataBase
// @ Не захломлять Deallocate методы!!!!!!! И соответственно минимизировать количество сущностей которые будут это
// контролировать!
struct Script_GlobalHelper
{
private:
    Script_GlobalHelper(void);

public:
    inline static Script_GlobalHelper& getInstance(void) noexcept
    {
        static Script_GlobalHelper instance;
        return instance;
    }

    ~Script_GlobalHelper(void)
    {
        if (this->m_tutorial)
        {
            MESSAGEI("Deleting GlobalTutorial! %s", typeid(this->m_tutorial).name());
            delete this->m_tutorial;
            this->m_tutorial = nullptr;
        }
    }
    Script_GlobalHelper(const Script_GlobalHelper&) = delete;
    Script_GlobalHelper& operator=(const Script_GlobalHelper&) = delete;
    Script_GlobalHelper(Script_GlobalHelper&&) = delete;
    Script_GlobalHelper& operator=(Script_GlobalHelper&&) = delete;

    inline void InitializeAlifeSimulator(void) {}
    inline void RegisterFunctionsFromAnotherFiles(void)
    {
        // @ Lord: реализовать
#pragma region XR_CONDITION

#pragma endregion

#pragma region XR_EFFECTS

#pragma endregion
    }

    inline const xr_map<int, bool>& getMonsterClasses(void) const noexcept { return this->m_monster_classes; }

    inline void setMonsterClasses(const xr_map<int, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setMonsterClasses(map)] WARNING: map.size() = 0! You are "
                "trying to set an empty "
                "map!");
            //   return;
        }

        this->m_monster_classes = map;
    }

    inline void setMonsterClasses(const std::pair<int, bool>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setMonsterClasses(pair)] WARNING: pair.first = "
                "std::uint32_t(-1)! Your value is "
                "undefined!");
            //     return;
        }

        this->m_monster_classes.insert(pair);
    }

    inline void setMonsterClasses(const int monster_id, const bool& value) noexcept
    {
        if (monster_id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setMonsterClasses(monster_id, value)] WARNING: "
                "monster_id = std::uint32_t(-1)! "
                "Your value is undefined!");
            //     return;
        }

        this->m_monster_classes[monster_id] = value;
    }

    inline const xr_map<int, bool>& getStalkerClasses(void) const noexcept { return this->m_stalker_classes; }

    inline void setStalkerClasses(const xr_map<int, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setStalkerClasses(map)] WARNING: map.size() = 0! You are "
                "trying to set an empty "
                "map!");
            //   return;
        }

        this->m_stalker_classes = map;
    }

    inline void setStalkerClasses(const std::pair<int, bool>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setStalkerClasses(pair)] WARNING: pair.first = "
                "std::uint32_t(-1)! Your value is "
                "undefined!");
            //  return;
        }

        this->m_stalker_classes.insert(pair);
    }

    inline void setStalkerClasses(const int stalker_id, const bool& value) noexcept
    {
        if (stalker_id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setStalkerClasses(stalker_id, value)] WARNING: "
                "stalker_id = std::uint32_t(-1)! "
                "Your value is undefined!");
            //    return;
        }

        this->m_stalker_classes[stalker_id] = value;
    }

    inline const xr_map<int, bool>& getWeaponClasses(void) const noexcept { return this->m_weapon_classes; }

    inline void setWeaponClasses(const xr_map<int, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setWeaponClasses(map)] WARNING: map.size() = 0! You are "
                "trying to set an empty "
                "map!");
            //   return;
        }

        this->m_weapon_classes = map;
    }

    inline void setWeaponClasses(const std::pair<int, bool>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setWeaponClasses(pair)] WARNING: pair.first = "
                "std::uint32_t(-1)! You value is "
                "undefined!");
            // return;
        }

        this->m_weapon_classes.insert(pair);
    }

    inline void setWeaponClasses(const int weapon_id, const bool& value) noexcept
    {
        if (weapon_id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setWeaponClasses(weapon_id, value)] WARNING: weapon_id = "
                "std::uint32_t(-1)! Your "
                "value is undefined!");
            //    return;
        }

        this->m_weapon_classes[weapon_id] = value;
    }

    inline const xr_map<int, bool>& getArtefactClasses(void) const noexcept { return this->m_artefact_classes; }

    inline void setArtefactClasses(const xr_map<int, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setArtefactClasses(map)] WARNING: map.size() = 0! You "
                "are trying to set an empty "
                "map!");
            //   return;
        }

        this->m_artefact_classes = map;
    }

    inline void setArtefactClasses(const std::pair<int, bool>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setArtefactClasses(pair)] WARNING: pair.first == "
                "std::uint32_t(-1)! You are "
                "trying to set an undefined value!");
            //  return;
        }

        this->m_artefact_classes.insert(pair);
    }

    inline void setArtefactClasses(const int artefact_id, const bool& value) noexcept
    {
        if (artefact_id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setArtefactClasses(artefact_id, value)] WARNING: "
                "artefact_id = "
                "std::uint32_t(-1)! Your value is undefined!");
            //    return;
        }

        this->m_artefact_classes[artefact_id] = value;
    }

    inline const xr_map<std::uint32_t, bool>& getAmmoSection(void) const noexcept { return this->m_ammo_section; }

    inline void setAmmoSection(const xr_map<std::uint32_t, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setAmmoSection(map)] WARNING: map.size() = 0! You are "
                "trying to set an empty "
                "map!");
            //   return;
        }

        this->m_ammo_section = map;
    }

    inline void setAmmoSection(const std::pair<std::uint32_t, bool>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setAmmoSection(pair)] WARNING: pair.first = "
                "std::uint32_t(-1)! "
                "Your value is undefined!!!");
            //  return;
        }

        this->m_ammo_section.insert(pair);
    }

    inline void setAmmoSection(const std::uint32_t& ammo_section, const bool& value) noexcept
    {
        if (ammo_section == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setAmmoSection(ammo_section, value)] WARNING: "
                "ammo_section = std::uint32_t(-1)! "
                "Your value is undefined!!!");
            //  return;
        }

        this->m_ammo_section[ammo_section] = value;
    }

    inline const xr_map<xr_string, bool>& getQuestSection(void) const noexcept { return this->m_quest_section; }

    inline void setQuestSection(const xr_map<xr_string, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setQuestSection(map)] WARNING: map.size() = 0! You are "
                "trying to set an empty "
                "map!");
            // return;
        }

        this->m_quest_section = map;
    }

    inline void setQuestSection(const std::pair<xr_string, bool>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setQuestSection] WARNING: pair.first.size() = 0! You are "
                "trying to set an "
                "empty pair!");
            //   return;
        }

        this->m_quest_section.insert(pair);
    }

    inline void setQuestSection(const xr_string& section_name, const bool& value) noexcept
    {
        if (!section_name.size())
        {
            Msg("[Script_GlobalHelper/setQuestSection] WARNING: section_name.size() = 0! You "
                "are trying to set an "
                "empty string!");
            //    return;
        }

        this->m_quest_section[section_name] = value;
    }

    inline const xr_map<xr_string, xr_string>& getSquadCommunityByBehavior(void) const noexcept
    {
        return this->m_squad_community_by_behavior;
    }

    inline void setSquadCommunityByBehavior(const xr_map<xr_string, xr_string>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setSquadCommunityByBehavior(map)] WARNING: map.size() = "
                "0! You are trying to set "
                "an empty map!");
            //  return;
        }

        this->m_squad_community_by_behavior = map;
    }

    inline void setSquadCommunityByBehavior(const std::pair<xr_string, xr_string>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setSquadCommunityByBehavior(pair)] WARNING: "
                "pair.first.size() = 0! You are trying to set an empty pair!");
            //  return;
        }

        this->m_squad_community_by_behavior.insert(pair);
    }

    inline void setSquadCommunityByBehavior(const xr_string& community_name, const xr_string& behavior_name) noexcept
    {
        if (!community_name.size())
        {
            Msg("[Script_GlobalHelper/setSquadCommunityByBehavior(community_name, "
                "behavior_name)] WARNING: "
                "community_name.size() = 0! You are trying to set an empty string!");
            // return;
        }

        this->m_squad_community_by_behavior[community_name] = behavior_name;
    }

    inline xr_map<xr_string, AnyCallable<void>>& getRegisteredFunctionsXREffects(void) noexcept
    {
        return this->m_registered_functions_xr_effects;
    }

    inline void setRegisteredFunctionsXREffects(const xr_map<xr_string, AnyCallable<void>>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredFunctionsXREffects(map)] WARNING: "
                "map.size() = 0! You are trying to "
                "set an empty map!");
            //   return;
        }

        this->m_registered_functions_xr_effects = map;
    }

    inline void setRegisteredFunctionsXREffects(const std::pair<xr_string, AnyCallable<void>>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredFunctionsXREffects(pair)] WARNING: "
                "pair.first.size() = 0! You are trying to set an empty pair!");
            // return;
        }

        this->m_registered_functions_xr_effects.insert(pair);
    }

    inline void setRegisteredFunctionsXREffects(const xr_string& function_name, const AnyCallable<void>& function)
    {
        if (!function_name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredFunctionsXREffects(function_name, "
                "function)] WARNING: "
                "function_name.size() = 0! You are trying to set an empty string!");
            // return;
        }

        this->m_registered_functions_xr_effects[function_name] = function;
    }

    inline xr_map<xr_string, AnyCallable<bool>>& getRegisteredFunctionsXRCondition(void) noexcept
    {
        return this->m_registered_functions_xr_conditions;
    }

    inline void setRegisteredFunctionsXRCondition(const xr_map<xr_string, AnyCallable<bool>>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredFunctionsXRCondition(map)] WARNING: "
                "map.size() = 0! You are trying "
                "to set an empty map!");
            //  return;
        }

        this->m_registered_functions_xr_conditions = map;
    }

    inline void setRegisteredFunctionsXRCondition(const std::pair<xr_string, AnyCallable<bool>>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredFunctionsXRCondition(pair)] WARNING: "
                "pair.first.size() = 0! You are trying to set an empty pair!");
            //  return;
        }

        this->m_registered_functions_xr_conditions.insert(pair);
    }

    inline void setRegisteredFunctionsXRCondition(const xr_string& function_name, const AnyCallable<bool>& function)
    {
        if (!function_name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredFunctionsXRCondition(function_name, "
                "function)] WARNING: "
                "function_name.size() = 0! You are trying to set an empty string!");
            //    return;
        }

        this->m_registered_functions_xr_conditions[function_name] = function;
    }

    // @ It uses in Level Editor as list of smartcovers as spawnelement and it's using in-game
    inline const xr_map<xr_string, SmartCoverData>& getRegisteredSmartCovers(void) const noexcept
    {
        return this->m_registered_smartcovers;
    }

    inline void setRegisteredSmartCovers(const xr_map<xr_string, SmartCoverData>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartCovers(map)] WARNING: map.size() = 0! "
                "You are trying to set an "
                "empty map!");
            //     return;
        }

        this->m_registered_smartcovers = map;
    }

    inline void setRegisteredSmartCovers(const std::pair<xr_string, SmartCoverData>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartCovers(smart_name, data)] WARNING: "
                "pair.first.size() = 0! You "
                "are trying to set an empty string,");
            //   return;
        }

        this->m_registered_smartcovers.insert(pair);
    }

    inline void setRegisteredSmartCovers(const xr_string& smart_name, const SmartCoverData& data) noexcept
    {
        if (!smart_name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartCovers(smart_name, data)] WARNING: "
                "smart_name.size() = 0! You "
                "are trying to set an empty string.");
            //   return;
        }

        this->m_registered_smartcovers[smart_name] = data;
    }

    inline const xr_map<xr_string, xr_string>& getJobTypesByScheme(void) const noexcept
    {
        return this->m_job_type_by_scheme;
    }

    inline void setJobTypesByScheme(const xr_map<xr_string, xr_string>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setJobTypesByScheme(map)] WARNING: map.size() = 0! You "
                "are trying to set an "
                "empty map!");
            //   return;
        }

        this->m_job_type_by_scheme = map;
    }

    inline void setJobTypesByScheme(const std::pair<xr_string, xr_string>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setJobTypesByScheme(type_name, scheme_name)] WARNING: "
                "pair.first.size() = 0! You "
                "are trying to set an empty string");
            // return;
        }

        this->m_job_type_by_scheme.insert(pair);
    }

    inline void setJobTypesByScheme(const xr_string& type_name, const xr_string& scheme_name) noexcept
    {
        if (!type_name.size())
        {
            Msg("[Script_GlobalHelper/setJobTypesByScheme(type_name, scheme_name)] WARNING: "
                "type_name.size() = 0! You "
                "are trying to set an empty string,");
            //   return;
        }

        this->m_job_type_by_scheme[type_name] = scheme_name;
    }

#pragma region Cordis InGame
    // @ First - id | Second - distance
    inline const std::pair<std::uint16_t, float>& getGameNearestToActorServerSmartTerrain(void) const noexcept
    {
        return this->m_game_server_nearest_to_actor_smart_terrain;
    }

    inline void setGameNearestToActorServerSmartTerrain(const std::pair<std::uint16_t, float>& pair) noexcept
    {
        if (pair.first == 0)
        {
            MESSAGEW("Your data contains an invalid ID!");
        }

        this->m_game_server_nearest_to_actor_smart_terrain = pair;
    }

    inline void setGameNearestToActorServerSmartTerrain(const std::uint16_t id, const float distance) noexcept
    {
        if (id == 0)
        {
            MESSAGEW("Your data contains an invalid ID!");
        }

        this->m_game_server_nearest_to_actor_smart_terrain.first = id;
        this->m_game_server_nearest_to_actor_smart_terrain.second = distance;
    }

    inline const xr_map<xr_string, xr_string>& getGameSmartsByAssaultZones(void) const noexcept
    {
        return this->m_game_smarts_by_no_assault_zone;
    }

    inline void setGameSmartsByAssaultZones(const xr_map<xr_string, xr_string>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setGameSmartsByAssaultZones(map)] WARNING: map.size() = "
                "0! You are trying to set "
                "an empty map!");
            //  return;
        }

        this->m_game_smarts_by_no_assault_zone = map;
    }

    inline void setGameSmartsByAssaultZones(const std::pair<xr_string, xr_string>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setGameSmartsByAssaultZones(pair)] WARNING: "
                "pair.first.size() = 0! You are "
                "trying to set an empty string!");
            //  return;
        }

        this->m_game_smarts_by_no_assault_zone.insert(pair);
    }

    inline void setGameSmartsByAssaultZones(const xr_string& smart_name, const xr_string& zone_name) noexcept
    {
        if (!smart_name.size())
        {
            Msg("[Script_GlobalHelper/setGameSmartsByAssaultZones(smart_name, zone_name)] "
                "WARNING: smart_name.size() = "
                "0! You are trying to set an empty string!");
            //    return;
        }

        this->m_game_smarts_by_no_assault_zone[smart_name] = zone_name;
    }
#pragma endregion

#pragma region Cordis System variables
    inline CUISequencer* getGlobalTutorial(void) const { return this->m_tutorial; }
    inline void setGlobalTutorial(CUISequencer* sequence)
    {
        if (!sequence)
        {
            MESSAGE("sequence = nullptr!");
        }

        this->m_tutorial = sequence;
    }

    inline const xr_map<xr_string, bool>& getRegisteredSmartTerrainTerritoryType(void) const noexcept
    {
        return this->m_registered_smart_terrain_territory_type;
    }

    inline void setRegisteredSmartTerrainTerritoryType(const xr_map<xr_string, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainTerritoryType(map)] WARNING: "
                "map.size() = 0! You are "
                "trying to set an empty map!");
            //  return;
        }

        this->m_registered_smart_terrain_territory_type = map;
    }

    inline void setRegisteredSmartTerrainTerritoryType(const std::pair<xr_string, bool>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainTerritoryType(pair)] WARNING: "
                "pair.first.size() = 0! "
                "You are trying to set an empty pair!");
            //   return;
        }

        this->m_registered_smart_terrain_territory_type.insert(pair);
    }

    inline void setRegisteredSmartTerrainTerritoryType(const xr_string& name, const bool& value) noexcept
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainTerritoryType(name, value)] "
                "WARNING: name.size() = 0! "
                "You are trying to set an empty string!");
            //    return;
        }

        this->m_registered_smart_terrain_territory_type[name] = value;
    }

    inline const xr_vector<xr_string>& getRegisteredSmartTerrainPathFileds(void) const noexcept
    {
        return this->m_registered_smart_terrain_path_fields;
    }

    inline void setRegisteredSmartTerrainPathFields(const xr_vector<xr_string>& vector) noexcept
    {
        if (!vector.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainPathFields(vector)] WARNING: "
                "vector.size() = 0! You are "
                "trying to set an empty vector!");
            //    return;
        }

        this->m_registered_smart_terrain_path_fields = vector;
    }

    inline void setRegisteredSmartTerrainPathFields(const xr_string& name) noexcept
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainFields(name)] WARNING: "
                "name.size() = 0! You are trying "
                "to set an empty string!");
            //   return;
        }

        this->m_registered_smart_terrain_path_fields.push_back(name);
    }

    inline const xr_map<xr_string, std::uint32_t>& getSimulationBoardGroupIDLevelsByName(void) const noexcept
    {
        return this->m_simulationboard_group_id_by_levels_name;
    }

    inline void setSimulationBoardGroupIDLevelsByName(const xr_map<xr_string, std::uint32_t>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setSimulationBoardGroupIDLevelsByName(map)] WARNING: "
                "map.size() = 0! You are "
                "trying to set an empty map!");
            //   return;
        }

        this->m_simulationboard_group_id_by_levels_name = map;
    }

    inline void setSimulationBoardGroupIDLevelsByName(const std::pair<xr_string, std::uint32_t>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setSimulationBoardGroupIDLevelsByName(pair)] WARNING: "
                "pair.first.size() = 0! you "
                "are trying to set an empty string!");
            //   return;
        }

        this->m_simulationboard_group_id_by_levels_name.insert(pair);
    }

    inline void setSimulationBoardGroupIDLevelsByName(const xr_string& name, const std::uint32_t& id) noexcept
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setSimulationBoardGropIDLevelsByName(name, id)] WARNING: "
                "name.size() = 0! you "
                "are trying to set an empty string!");
            //    return;
        }

        this->m_simulationboard_group_id_by_levels_name[name] = id;
    }

    inline const xr_map<xr_string, bool>& getSimulationSquadIsSquadMonster(void) const noexcept
    {
        return this->m_simulationsquad_is_squad_monster_by_type;
    }

    inline void setSimulationSquadIsSquadMonster(const xr_map<xr_string, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setSimulationSquadIsSquadMonster(map)] map.size() = 0! "
                "you are trying to set an "
                "empty map!");
            // return;
        }

        this->m_simulationsquad_is_squad_monster_by_type = map;
    }

    inline void setSimulationSquadIsSquadMonster(const std::pair<xr_string, bool>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setSimulationSquadIsSquadMonster(pair)] WARNING: "
                "pair.first.size() = 0! you are "
                "trying to set an empty string");
            //  return;
        }

        this->m_simulationsquad_is_squad_monster_by_type.insert(pair);
    }

    inline void setSimulationSquadIsSquadMonster(const xr_string& name, const bool& value) noexcept
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setSimulationSquadIsSquadMonster(name, value)] WARNING: "
                "name.size() = 0! you are "
                "trying to set an empty value");
            // return;
        }

        this->m_simulationsquad_is_squad_monster_by_type[name] = value;
    }

    inline const xr_map<xr_string, Script_SmartTerrainControl_States>& getRegisteredSmartTerrainControlScriptStates(
        void) const noexcept
    {
        return this->m_registered_smart_terrain_control_script_states;
    }

    inline void setRegisteredSmartTerrainControlScriptStates(
        const xr_map<xr_string, Script_SmartTerrainControl_States>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainControlScriptStates(map)] "
                "WARNING: map.size() = 0! you "
                "are trying to set an empty map!");
            //   return;
        }

        this->m_registered_smart_terrain_control_script_states = map;
    }

    inline void setRegisteredSmartTerrainControlScriptStates(
        const std::pair<xr_string, Script_SmartTerrainControl_States>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainControlScriptStates(pair)] "
                "WARNING: pair.first is empty "
                "string! You are trying to set an empty string,");
            //   return;
        }

        this->m_registered_smart_terrain_control_script_states.insert(pair);
    }

    inline void setRegisteredSmartTerrainControlScriptStates(
        const xr_string& name, const Script_SmartTerrainControl_States& state) noexcept
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredSmartTerrainControlScriptStates(name, "
                "state)] WARNING: you are "
                "trying to set an empty string");
            //    return;
        }

        this->m_registered_smart_terrain_control_script_states[name] = state;
    }

    inline const xr_map<xr_string, bool>& getRegisteredEatableVisuals(void) const noexcept
    {
        return this->m_registered_eatable_visuals;
    }

    inline void setRegisteredEatableVisuals(const xr_map<xr_string, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredEatableVisuals(map)] WARNING: you're set an "
                "empty map!");
            //  return;
        }

        this->m_registered_eatable_visuals = map;
    }

    inline void setRegisteredEatableVisuals(const std::pair<xr_string, bool>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredEatableVisuals(pair)] WARNING: you're "
                "trying to set an empty pair "
                "to map!");
            //    return;
        }

        this->m_registered_eatable_visuals.insert(pair);
    }

    inline void setRegisteredEatableVisuals(const xr_string& name, const bool& is_used) noexcept
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredEatableVisuals(name, is_used)] WARNING: "
                "name.size() = 0! (string is "
                "null!)");
            //   return;
        }

        this->m_registered_eatable_visuals[name] = is_used;
    }

    inline const xr_map<xr_string, bool>& getRegisteredHarmonicaVisuals(void) const noexcept
    {
        return this->m_registered_harmonica_visuals;
    }

    inline void setRegisteredHarmonicaVisuals(const xr_map<xr_string, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredHarmonicaVisuals(map)] WARNING: map.size() "
                "= 0! You are trying to "
                "set an empty map! No assignment!");
            return;
        }

        this->m_registered_harmonica_visuals = map;
    }

    inline void setRegisteredHarmonicaVisuals(const std::pair<xr_string, bool>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredHarmonicaVisuals(pair)] WARNING: "
                "pair.first.size() = 0! You are "
                "trying to set an empty pair!");
            //  return;
        }

        this->m_registered_harmonica_visuals.insert(pair);
    }

    inline void setRegisteredHarmonicaVisuals(const xr_string& visual_name, const bool& is_used) noexcept
    {
        if (!visual_name.size())
        {
            Msg("[Script_GlobalHelper/setRegisteredHarmonicaVisuals(visual_name, value)] "
                "WARNING: visual_name.size() = "
                "0! You are trying to set an empty string!");
            //   return;
        }

        this->m_registered_harmonica_visuals[visual_name] = is_used;
    }

    inline const xr_map<xr_string, xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>>>&
    getAnimpointTable(void) noexcept
    {
        return this->m_animpoint_table;
    }

    inline void setAnimpointTable(
        const xr_map<xr_string, xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>>>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setAnimpointTable(pair)] WARNING: "
                "map.size() = 0! You are trying to set an empty string!");
            //  return;
        }

        this->m_animpoint_table = map;
    }

    inline const xr_map<xr_string, StateLibData>& getStateLibrary(void) const noexcept { return this->m_state_library; }
    inline void setStateLibrary(const xr_map<xr_string, StateLibData>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage/setStateLibrary(map)] WARNING: map.size() = 0! You are "
                "trying to set an empty "
                "map! ");
            //    return;
        }

        this->m_state_library = map;
    }

    inline void setStateLibrary(const std::pair<xr_string, StateLibData>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[DataBase/Storage/setStateLibrary(pair)] WARNING: pair.first.size() = 0! You "
                "are trying to set an "
                "empty pair!");
            //    return;
        }

        this->m_state_library.insert(pair);
    }

    inline void setStateLibrary(const xr_string& animation_name, const StateLibData& data) noexcept
    {
        if (!animation_name.size())
        {
            Msg("[DataBase/Storage/setStateLibrary(animation_name, data)] WARNING: "
                "animation_name.size() = 0! You are "
                "trying to set an empty string!");
            //   return;
        }

        this->m_state_library[animation_name] = data;
    }

    /*
        inline void setAnimpointTable(
            const std::pair<xr_string, std::pair<std::function<bool(std::uint16_t, bool)>,
       xr_string>>& pair)
        {
            if (!pair.first.size())
            {
                Msg("[Script_GlobalHelper/setAnimpointTable(pair)] WARNING: "
                    "pair.first.size() = 0! You are trying to set an empty string! No
       assignment!"); return;
            }

            if (!pair.second.first)
            {
                Msg("[Script_GlobalHelper/setAnimpointTable(pair)] WARNING: "
                    "pair.second.first = null! You are trying to set an empty function! No
       assignment!"); return;
            }

            if (!pair.second.second.size())
            {
                Msg("[Script_GlobalHelper/setAnimpointTable(pair)] WARNING: "
                    "pair.second.second.size() = 0! You are trying to set an empty string! No
       assignment!"); return;
            }

            this->m_animpoint_table.insert(pair);
        }*/

    inline void setAnimpointTable(
        const xr_string& animpoint_name, const std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>& pair)
    {
        if (!animpoint_name.size())
        {
            Msg("[Script_GlobalHelper/setAnimpointTable(animpoint_name, function, "
                "animpoint_animation_name)] WARNING: "
                "animation_name.size() = 0! You are trying to set an empty string! No "
                "assignment!");
            //  return;
        }

        if (!pair.first)
        {
            Msg("[Script_GlobalHelper/setAnimpointTable(animpoint_name, function, "
                "animpoint_animation_name)] WARNING: "
                "pair.first = null! You are trying to set an empty function!");
            // return;
        }

        if (!pair.second.size())
        {
            Msg("[Script_GlobalHelper/setAnimpointTable(animpoint_name, function, "
                "animpoint_animation_name)] WARNING: "
                "pair.second.size() = 0! You are trying to set an empty string!");
            //   return;
        }

        this->m_animpoint_table[animpoint_name].push_back(pair);
    }

    inline void setAnimpointTable(const xr_string& animpoint_name,
        const std::function<bool(std::uint16_t, bool)> function, const xr_string& animpoint_animation_name)
    {
        if (!animpoint_name.size())
        {
            Msg("[Script_GlobalHelper/setAnimpointTable(animpoint_name, function, "
                "animpoint_animation_name)] WARNING: "
                "animation_name.size() = 0! You are trying to set an empty string!");
            //  return;
        }

        if (!function)
        {
            Msg("[Script_GlobalHelper/setAnimpointTable(animpoint_name, function, "
                "animpoint_animation_name)] WARNING: "
                "function = null! You are trying to set an empty function!");
            //  return;
        }

        if (!animpoint_animation_name.size())
        {
            Msg("[Script_GlobalHelper/setAnimpointTable(animpoint_name, function, "
                "animpoint_animation_name)] WARNING: "
                "animpoint_animation_name.size() = 0! You are trying to set an empty string!");
            // return;
        }

        this->m_animpoint_table[animpoint_name].push_back({function, animpoint_animation_name});
    }

    inline const xr_map<xr_string, xr_map<std::uint32_t, DialogData>>& getPhraseTable(void) const noexcept
    {
        return this->m_phrase_table;
    }

    inline void setPhraseTable(
        const xr_string& phrase_category_name, const std::uint32_t id, const DialogData& data) noexcept
    {
        if (phrase_category_name.empty())
        {
            Msg("[Scripts/Script_GlobalHelper/setPhraseTable(phrase_category_name, data)] "
                "WARNING: "
                "phrase_category_name.empty() == true! Can't assign return...");
            return;
        }

        this->m_phrase_table[phrase_category_name][id] = data;
    }

    /*
        inline void setPhraseTable(const std::pair<xr_string, std::pair<std::uint32_t, DialogData>>& pair) noexcept
        {
            if (pair.first.empty())
            {
                Msg("[Scripts/Script_GlobalHelper/setPhraseTable(pair)] WARNING: "
                    "pair.first.empty() == true! Can't assign "
                    "return...");
                return;
            }

            this->m_phrase_table.insert(pair);
        }*/

    inline void setPhraseTable(const xr_map<xr_string, xr_map<std::uint32_t, DialogData>>& map) noexcept
    {
        if (map.empty())
        {
            Msg("[Scripts/Script_GlobalHelper/setPhraseTable(map)] WARNING: map.empty() == "
                "true! Can't assign "
                "return...");
            return;
        }

        this->m_phrase_table = map;
    }

    inline xr_map<xr_string, AnyCallable<void>>& getRegisteredFunctionsDialogManager(void)
    {
        return this->m_registered_functions_dialog_manager;
    }

    inline xr_map<xr_string, AnyCallable<bool>>& getRegisteredFunctionsBoolDialogManager(void)
    {
        return this->m_registered_functions_bool_dialog_manager;
    }

    inline const xr_map<xr_string, xr_map<std::uint16_t, xr_map<std::uint32_t, int>>>& getPhrasePriorityTable(
        void) const noexcept
    {
        return this->m_phrase_priority_table;
    }

    inline void setPhrasePriorityTable(
        const xr_map<xr_string, xr_map<std::uint16_t, xr_map<std::uint32_t, int>>>& map) noexcept
    {
        if (map.empty())
        {
            Msg("[Scripts/Script_GlobalHelper/setPhrasePriorityTable(map)] WARNING: map.empty() == true! You are "
                "trying to set an empty map");
        }

        this->m_phrase_priority_table = map;
    }

    inline void setPhrasePriorityTable(const xr_string& dialog_type_name, const std::uint16_t npc_id,
        const std::uint32_t phrase_id, const int priority) noexcept
    {
        if (dialog_type_name.empty())
        {
            Msg("[Scripts/Script_GlobalHelper/setPhrasePriorityTable(dialog_type_name, npc_id, phrase_id, priority)] "
                "WARNING: dialog_type_name.empty() == true! You are trying to set an empty string no assigment!");
            return;
        }

        if (npc_id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[Scripts/Script_GlobalHelper/setPhrasePriorityTable(dialog_type_name, npc_id, phrase_id, priority)] "
                "WARNING: npc_id is "
                "undefined! std::uint16_t(-1)!");
        }

        if (phrase_id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Scripts/Script_GlobalHelper/setPhrasePriorityTable(dialog_type_name, npc_id, phrase_id, priority)] "
                "WARNING: phrase_id "
                "is undefined! std::uint32_t(-1)!");
        }

        this->m_phrase_priority_table[dialog_type_name][npc_id][phrase_id] = priority;
    }

    // Lord: здесь переименовать комметарий под уже изменившийся файл когда используется CRD
    // @ Private, using in Script_XR_Gulag
    inline const xr_map<xr_string, CScriptIniFile*>& getDynamicLtx(void) const noexcept
    {
        return this->m_registered_generated_gulag_inis;
    }

    inline void setDynamicLtx(const xr_string& name, CScriptIniFile* file)
    {
        if (name.empty())
        {
            Msg("[Scripts/Script_GlobalHelper/setDynamicLtx(name, file)] WARNING: name.empty() == true! Can't assign "
                "return");
            return;
        }

        if (!file)
        {
            Msg("[Scripts/Script_GlobalHelper/setDynamicLtx(name, file)] WARNING: file == nullptr! Can't assign "
                "return");
            return;
        }

        this->m_registered_generated_gulag_inis[name] = file;
    }

    inline void setDynamicLtx(const std::pair<xr_string, CScriptIniFile*>& pair)
    {
        if (pair.first.empty())
        {
            Msg("[Scripts/Script_GlobalHelper/setDynamicLtx(pair)] WARNING: pair.first.empty() == true! Can't assign "
                "return");
            return;
        }

        if (!pair.second)
        {
            Msg("[Scripts/Script_GlobalHelper/setDynamicLtx(pair)] WARNING: pair.second == nullptr! Can't assign "
                "return");
            return;
        }

        this->m_registered_generated_gulag_inis.insert(pair);
    }

    inline void setDynamicLtx(const xr_map<xr_string, CScriptIniFile*> map)
    {
        if (map.empty())
        {
            Msg("[Scripts/Script_GlobalHelper/setDynamicLtx(map)] WARNING: map.empty() == true! Can't assign return");
            return;
        }

        this->m_registered_generated_gulag_inis = map;
    }

    // @ Private, don't call it personally!
    inline void DeallocateDynamicLtx(void)
    {
        if (!this->m_registered_generated_gulag_inis.empty())
        {
            for (std::pair<const xr_string, CScriptIniFile*>& it : this->m_registered_generated_gulag_inis)
            {
                Msg("[Scripts/Script_GlobalHelper/DeallocateDynamicLtx()] deleting generated CScriptIniFile logic "
                    "stuff from "
                    "GulagGenerator! %s",
                    it.second->fname());
                xr_delete(it.second);
            }
        }
    }

    inline std::uint32_t getMonsterAnimationToAction(const xr_string& animation_name)
    {
        if (this->m_monster_animation_to_action.empty())
        {
            R_ASSERT2(false, "user this structure must initialized!");
            return MonsterSpace::eAA_StandIdle;
        }

        if (this->m_monster_animation_to_action.find(animation_name) == this->m_monster_animation_to_action.end())
            return MonsterSpace::eAA_StandIdle;

        return this->m_monster_animation_to_action[animation_name];
    }

    inline const xr_map<xr_string, MonsterSound::EType>& getSoundNameToAction(void) const noexcept
    {
        return this->m_monster_sound_name_to_type_action;
    }

    inline xr_map<xr_string,
        std::function<void(
            CScriptGameObject* const, CScriptIniFile* const, const xr_string&, const xr_string&, const xr_string&)>>&
    getSchemesSetSchemeCallbacks(void) noexcept
    {
        return this->m_registered_schemes_set_scheme_callbacks;
    }

    inline xr_map<xr_string,
        std::function<void(CScriptGameObject* const, CScriptIniFile* const, const xr_string&, const xr_string&, DataBase::Storage_Scheme&)>>&
    getSchemesAddToBinderCallbacks(void) noexcept
    {
        return this->m_registered_schemes_add_to_binder_callbacks;
    }

    // @ Uses in Script_SchemeSRLight
    inline const xr_map<xr_string, bool>& getIndoorLevels(void) const noexcept { return this->m_indoor_levels; }
    inline void setIndoorLevels(const xr_string& level_name, const bool value) noexcept
    {
        if (level_name.empty())
        {
            Msg("[Scripts/Script_GlobalHelper/setIndoorLevels(level_name, value)] WARNING: level_name.empty() == true! "
                "You set an empty string return ...");
            return;
        }

        this->m_indoor_levels[level_name] = value;
    }

    inline float getXRDangerIgnoreDistanceByDangerType(const CDangerObject::EDangerType type) const noexcept
    {
        if (this->m_xr_danger_ignore_distance_by_danger_type.find(type) !=
            this->m_xr_danger_ignore_distance_by_danger_type.end())
            return this->m_xr_danger_ignore_distance_by_danger_type.at(type);

        Msg("[Scripts/Script_GlobalHelper/setXRDangerIgnoreDistanceByDangerType(type)] WARNING: returns 0.0f, because "
            "can't find a danger type -> %d",
            static_cast<std::uint32_t>(type));

        return 0.0f;
    }

    inline const xr_map<xr_string, bool>& getLookDirectionStates(void) const noexcept
    {
        return this->m_look_direction_states;
    }

    inline const xr_map<xr_string, StateManagerAnimationData>& getStateManagerAnimationList(void) const noexcept
    {
        return this->m_state_manager_animation_list;
    }

    inline const xr_map<xr_string, StateManagerAnimationData>& getStateManagerAnimationStateList(void) const noexcept
    {
        return this->m_state_manager_animationstate_list;
    }

    inline const xr_map<xr_string, xr_string>& getNewsManagerRegisteredSoundTips(void) const noexcept
    {
        return this->m_news_manager_registered_sound_tips;
    }

    inline const xr_string& getTranslatedSmartTerrainName(Script_SE_SmartTerrain* const p_server_smart)
    {
        if (!p_server_smart)
        {
            R_ASSERT2(false, "object is null!");
            return xr_string("can't obtain smart_terrain name");
        }

        xr_string level_name =
            Globals::get_level_name(Globals::Game::get_game_graph()->vertex(p_server_smart->m_tGraphID)->level_id());
        xr_string smart_terrain_name = p_server_smart->name_replace();

        if (this->m_smart_terrains_translated_name.find(level_name) != this->m_smart_terrains_translated_name.end())
        {
            if (this->m_smart_terrains_translated_name.at(level_name).find(smart_terrain_name) !=
                this->m_smart_terrains_translated_name.at(level_name).end())
            {
                return Globals::Game::translate_string(
                    this->m_smart_terrains_translated_name.at(level_name).at(smart_terrain_name).c_str());
            }
        }

        return smart_terrain_name;
    }

    inline const xr_vector<PDA_ChangeObjectData>& getPDAChangeObjects(void) const noexcept
    {
        return this->m_registered_pda_change_objects;
    }

    // .first -> target | .second -> hint
    inline const xr_vector<std::pair<xr_string, xr_string>>& getPDAPrimaryObjects(void) const noexcept
    {
        return this->m_registered_pda_primary_objects;
    }

    inline const xr_map<xr_string, std::uint32_t>& getRegisteredRelations(void) const noexcept 
    {
        return this->m_data_relations;
    }
#pragma endregion

    inline bool isGameOverCreditsStarted(void) const noexcept { return this->m_is_gameover_credits_started; }
    inline void setGameOverCreditsStarted(const bool value) noexcept { this->m_is_gameover_credits_started = value; }

#pragma region Cordis Dialog Zaton
    inline const xr_map<std::uint16_t, xr_string>& getZatB29InfopBringTable(void) const noexcept
    {
        return this->m_zat_b29_infop_bring_table;
    }
    inline const xr_map<std::uint16_t, xr_string>& getZatB29AfTable(void) const noexcept
    {
        return this->m_zat_b29_af_table;
    }

    inline const xr_map<std::uint16_t, xr_string>& getZatB29AfNamesTable(void) const noexcept
    {
        return this->m_zat_b29_af_names_table;
    }
    inline const xr_map<std::uint16_t, xr_string>& getZatB29InfopTable(void) const noexcept
    {
        return this->m_zat_b29_infop_table;
    }
#pragma endregion

    inline const xr_map<xr_string, xr_string>& getNewsManagerActionDescriptionByTypeName(void) const noexcept
    {
        return this->m_news_manager_action_description_by_type_name;
    }

    inline const xr_map<xr_string, bool>& getTaskValidValues(void) const noexcept { return this->m_task_valid_values; }

    inline const xr_map<xr_string, xr_map<xr_string, xr_string>>& getTaskGuidersByLevel(void) const noexcept
    {
        return this->m_task_guiders_by_level;
    }

    inline const xr_map<xr_string, std::uint8_t>& getTaskIDByStatus(void) const noexcept
    {
        return this->m_task_id_by_status;
    }
    inline const xr_map<std::uint8_t, xr_string>& getTaskStatusByID(void) const noexcept
    {
        return this->m_task_status_by_id;
    }

#pragma region Cordis Surge Manager
    inline const xr_map<xr_string, bool>& getSurgeManagerImmunedToSurgeSquads(void) const noexcept
    {
        return this->m_surge_manager_immuned_to_surge_squads;
    }
#pragma endregion

#pragma region Cordis Zaton Dialog
    inline const xr_map<std::uint32_t, std::uint32_t>& getZatItemCountByCategory(void) const noexcept
    {
        return this->m_zat_item_count_by_category;
    }

    inline const xr_map<std::uint32_t, xr_map<xr_string, std::uint32_t>>& getZatB51CostsTable(void) const noexcept
    {
        return this->m_zat_b51_costs_table;
    }

    inline const xr_map<std::uint32_t, xr_map<std::uint32_t, xr_vector<xr_string>>>& getZatB51BuyItemTable(void) const
        noexcept
    {
        return this->m_zat_b51_buy_item_table;
    }
#pragma endregion

#pragma region Cordis Dialogs Jupiter
    inline const xr_map<std::uint32_t, xr_string>& getJupA12AfTable(void) const noexcept 
    {
        return this->m_jup_a12_af_table;
    }

    inline const xr_vector<std::uint32_t>& getJupB9MoneyCount(void) const noexcept 
    {
        return this->m_jup_b9_money_count;
    }
#pragma endregion

#pragma region Cordis Dialogs Pripyat
    inline const xr_map<xr_string, xr_map<xr_string, std::uint32_t>> getPriA25MedicItems(void) const noexcept 
    {
        return this->m_pri_a25_medic_items;
    }

    inline const xr_map<xr_string, xr_map<xr_string, std::uint32_t>> getPriA22SuppItems(void) const noexcept
    {
        return this->m_pri_a22_supp_items;
    }
#pragma endregion

    #pragma region Cordis Schemes stuff
    inline const xr_map<xr_string, xr_string>& getXRWalkerAssociation(void) const noexcept 
    {
        return this->m_xr_walker_association;
    }

    inline const xr_map<std::uint32_t, xr_string>& getXRWoundedStates(void) const noexcept { return this->m_wounded_by_state; }

    inline const xr_map<xr_string, bool>& getXRCombatIgnoreSmarts(void) const noexcept { return this->m_ignored_smart; }

    inline const xr_map<xr_string, xr_string>& getXRCombatIgnoreSmartsByNoAssaultZones(void) const noexcept { return this->m_xr_combat_ignore_smarts_by_no_assault_zones; }

    inline const xr_map<xr_string, xr_vector<std::pair<Fvector, float>>>& getXRPatrolFormations(void) const noexcept { return this->m_xr_patrol_formations; }

    inline const xr_map<xr_string, xr_string>& getXRPatrolAccelByCurrentType(void) const noexcept { return this->m_xr_patrol_accel_by_curtype; }

    inline const xr_map<xr_string, xr_vector<std::pair<Fvector, float>>>& getXRReachTaskFormations(void) const noexcept { return this->m_xr_reach_task_formations; }

    inline const xr_map<xr_string, std::pair<xr_vector<xr_string>, xr_vector<xr_string>>>& getXRAnimpointAssociationTable(void) const noexcept {return this->m_xr_animpoint_association_table;}
    inline const xr_map<xr_string, xr_string>& getXRSmartCoverCoverSubState(void) const noexcept { return this->m_xr_smartcover_cover_substate; }
    #pragma endregion

private:
    bool m_is_gameover_credits_started;
    CUISequencer* m_tutorial;
    xr_map<int, bool> m_monster_classes;
    xr_map<int, bool> m_stalker_classes;
    xr_map<int, bool> m_weapon_classes;
    xr_map<int, bool> m_artefact_classes;
    xr_map<std::uint32_t, bool> m_ammo_section;
    xr_map<xr_string, bool> m_look_direction_states;
    xr_map<xr_string, bool> m_registered_eatable_visuals;
    xr_map<xr_string, bool> m_registered_harmonica_visuals;
    xr_map<xr_string, bool> m_quest_section;
    xr_map<xr_string, bool> m_surge_manager_immuned_to_surge_squads;
    xr_map<xr_string,
        std::function<void(
            CScriptGameObject* const, CScriptIniFile* const, const xr_string&, const xr_string&, const xr_string&)>>
        m_registered_schemes_set_scheme_callbacks;
    xr_map<xr_string,
        std::function<void(CScriptGameObject* const, CScriptIniFile* const, const xr_string&, const xr_string&,
            DataBase::Storage_Scheme&)>>
        m_registered_schemes_add_to_binder_callbacks;
    xr_map<xr_string, Script_SmartTerrainControl_States> m_registered_smart_terrain_control_script_states;
    xr_map<xr_string, xr_string> m_squad_community_by_behavior;
    xr_map<xr_string, xr_string> m_xr_smartcover_cover_substate;
    xr_map<xr_string, xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>>> m_animpoint_table;

#pragma region Cordis XR_EFFECTS Functions
    xr_map<xr_string, AnyCallable<void>> m_registered_functions_xr_effects;
#pragma endregion

#pragma region Cordis XR_CONDITION Functions
    xr_map<xr_string, AnyCallable<bool>> m_registered_functions_xr_conditions;
#pragma endregion

#pragma region Cordis Dialog Manager Functions
    xr_map<xr_string, AnyCallable<void>> m_registered_functions_dialog_manager;
    xr_map<xr_string, AnyCallable<bool>> m_registered_functions_bool_dialog_manager;
#pragma endregion

#pragma region Cordis Dialog functions of all levels
    // Если функция вызывалась как dialogs.my_name() то в данной карте будет nothing -> my_name(), а в xml файле просто
    // my_name(), если dialogs_название_уровня, то вы должны просто писать название своего уровня, но оно нигде не
    // привязывается (а в идеале надо сделать так), то есть просто вместо dialogs_zaton писать zaton.my_function_name()
    // в xml

    // По поводу привязки к уровням, мы просто должны заранее в карту занести список всех действительных уровней которые
    // имеет юзверь, это можно сделать через файлы либо занести в ручную!
    xr_map<xr_string, xr_map<xr_string, AnyCallable<void>>> m_registered_dialog_functions_void;
    xr_map<xr_string, xr_map<xr_string, AnyCallable<bool>>> m_registered_dialog_functions_bool;
    xr_map<xr_string, xr_map<xr_string, AnyCallable<xr_string>>> m_registered_dialog_functions_string;
#pragma endregion

    xr_map<xr_string, xr_map<std::uint32_t, DialogData>> m_phrase_table;
    xr_map<xr_string, xr_map<std::uint16_t, xr_map<std::uint32_t, int>>> m_phrase_priority_table;
    xr_map<xr_string, CScriptIniFile*> m_registered_generated_gulag_inis;
    xr_map<xr_string, StateLibData> m_state_library;
    xr_map<xr_string, StateManagerAnimationData> m_state_manager_animation_list;
    xr_map<xr_string, StateManagerAnimationData> m_state_manager_animationstate_list;
    xr_map<xr_string, SmartCoverData> m_registered_smartcovers;
    xr_map<xr_string, xr_string> m_job_type_by_scheme;
    xr_map<xr_string, xr_string> m_game_smarts_by_no_assault_zone;
    xr_map<xr_string, xr_string> m_news_manager_registered_sound_tips;
    xr_map<xr_string, xr_string> m_news_manager_action_description_by_type_name;
    xr_map<xr_string, xr_string> m_xr_walker_association;
    // @ Pair.first -> director 
    // @ Pair.second -> listener
    xr_map<xr_string, std::pair<xr_vector<xr_string>, xr_vector<xr_string>>> m_xr_animpoint_association_table;
    xr_map<xr_string, xr_string> m_xr_patrol_accel_by_curtype;
    xr_map<xr_string, xr_string> m_xr_combat_ignore_smarts_by_no_assault_zones;
    xr_map<xr_string, xr_map<xr_string, xr_string>> m_smart_terrains_translated_name;
    xr_map<xr_string, xr_map<xr_string, std::uint32_t>> m_pri_a25_medic_items;
    xr_map<xr_string, xr_map<xr_string, std::uint32_t>> m_pri_a22_supp_items;
    xr_map<xr_string, std::uint32_t> m_simulationboard_group_id_by_levels_name;
    xr_map<xr_string, std::uint32_t> m_monster_animation_to_action;
    xr_map<xr_string, std::uint32_t> m_data_relations;
    xr_map<xr_string, MonsterSound::EType> m_monster_sound_name_to_type_action;
    xr_map<xr_string, bool> m_registered_smart_terrain_territory_type;
    xr_map<xr_string, bool> m_simulationsquad_is_squad_monster_by_type;
    xr_map<xr_string, bool> m_indoor_levels;
    xr_map<xr_string, bool> m_task_valid_values;
    xr_map<xr_string, bool> m_ignored_smart;
    xr_map<xr_string, std::uint8_t> m_task_id_by_status;
    xr_map<std::uint8_t, xr_string> m_task_status_by_id;
    xr_map<std::uint16_t, xr_string> m_zat_b29_infop_bring_table;
    xr_map<std::uint16_t, xr_string> m_zat_b29_infop_table;
    xr_map<std::uint16_t, xr_string> m_zat_b29_af_names_table;
    xr_map<std::uint16_t, xr_string> m_zat_b29_af_table;
    xr_map<std::uint32_t, xr_string> m_jup_a12_af_table;
    xr_map<std::uint32_t, xr_string> m_wounded_by_state;
    xr_map<std::uint32_t, std::uint32_t> m_zat_item_count_by_category;
    xr_map<std::uint32_t, xr_map<xr_string, std::uint32_t>> m_zat_b51_costs_table;
    xr_map<std::uint32_t, xr_map<std::uint32_t, xr_vector<xr_string>>> m_zat_b51_buy_item_table;
    xr_map<xr_string, xr_map<xr_string, xr_string>> m_task_guiders_by_level;
    xr_map<xr_string, xr_vector<std::pair<Fvector, float>>> m_xr_patrol_formations;
    xr_map<xr_string, xr_vector<std::pair<Fvector, float>>> m_xr_reach_task_formations;
    xr_map<CDangerObject::EDangerType, float> m_xr_danger_ignore_distance_by_danger_type;
    // @ First - id | Second - distance
    std::pair<std::uint16_t, float> m_game_server_nearest_to_actor_smart_terrain;
    xr_vector<xr_string> m_registered_smart_terrain_path_fields;
    xr_vector<std::uint32_t> m_jup_b9_money_count;
#pragma region Cordis PDA the values of pda.script
    xr_vector<PDA_ChangeObjectData> m_registered_pda_change_objects;
    xr_vector<std::pair<xr_string, xr_string>> m_registered_pda_primary_objects;
#pragma endregion
};

} // namespace Scripts
} // namespace Cordis
