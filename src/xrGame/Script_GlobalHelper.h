#pragma once

#include "object_factory.h"
#include "Script_XR_Patrol.h"

namespace Cordis
{
namespace Scripts
{
struct Script_GlobalHelper
{
private:
    Script_GlobalHelper(void)
    {
        // @ Здесь инициализация всех "локальных" карт которые находятся в каждом скрипте, проверить что оно реально
        // раньше всех инициализирует
#pragma region Cordis XR_PATROL Initializing
        std::pair<Fvector, float> xr_patrol_formations_data;
        xr_patrol_formations_data.first = Fvector().set(0.3f, 0.0f, -1.0f);
        xr_patrol_formations_data.second = 1.2f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_BACK].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-0.3f, 0.0f, -1.0f);
        xr_patrol_formations_data.second = 2.4f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_BACK].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(0.3f, 0.0f, -1.0f);
        xr_patrol_formations_data.second = 3.6f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_BACK].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-0.3f, 0.0f, -1.0f);
        xr_patrol_formations_data.second = 4.8f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_BACK].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(0.3f, 0.0f, -1.0f);
        xr_patrol_formations_data.second = 6.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_BACK].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-0.3f, 0.0f, -1.0f);
        xr_patrol_formations_data.second = 7.2f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_BACK].push_back(xr_patrol_formations_data);

        xr_patrol_formations_data.first = Fvector().set(-1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 2.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_LINE].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 4.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_LINE].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 6.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_LINE].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 2.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_LINE].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 4.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_LINE].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 6.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_LINE].push_back(xr_patrol_formations_data);

        xr_patrol_formations_data.first = Fvector().set(0.44721359f, 0.0f, -0.89442718f);
        xr_patrol_formations_data.second = 2.2360680f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_AROUND].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-0.44721359f, 0.0f, -0.89442718f);
        xr_patrol_formations_data.second = 2.2360680f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_AROUND].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 2.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_AROUND].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-1.0f, 0.0f, 0.0f);
        xr_patrol_formations_data.second = 2.0f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_AROUND].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(0.44721359f, 0.0f, -0.89442718f);
        xr_patrol_formations_data.second = 2.2360680f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_AROUND].push_back(xr_patrol_formations_data);
        xr_patrol_formations_data.first = Fvector().set(-0.44721359f, 0.0f, -0.89442718f);
        xr_patrol_formations_data.second = 2.2360680f;
        XR_PATROL::getFormations()[XR_PATROL::XR_PATROL_FORMATION_INDEX_AROUND].push_back(xr_patrol_formations_data);
#pragma endregion

#pragma region Cordis SmartCovers Initialzing

#pragma endregion

#pragma region Cordis Jobs Types Initializing
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameWalker] =
            Globals::GulagGenerator::kGulagJobPath;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameCamper] =
            Globals::GulagGenerator::kGulagJobPath;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNamePatrol] =
            Globals::GulagGenerator::kGulagJobPath;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameAnimpoint] =
            Globals::GulagGenerator::kGulagJobSmartCover;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameSmartCover] =
            Globals::GulagGenerator::kGulagJobSmartCover;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameRemark] =
            Globals::GulagGenerator::kGulagJobPoint;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameCover] =
            Globals::GulagGenerator::kGulagJobPoint;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameSleeper] =
            Globals::GulagGenerator::kGulagJobPath;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameMobWalker] =
            Globals::GulagGenerator::kGulagJobPath;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameMobHome] =
            Globals::GulagGenerator::kGulagJobPath;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameMobJump] =
            Globals::GulagGenerator::kGulagJobPoint;
        this->m_job_type_by_scheme[Globals::GulagGenerator::kGulagJobNameCompanion] =
            Globals::GulagGenerator::kGulagJobPoint;
#pragma endregion

#pragma region SmartTerrain Initiializing
        this->m_game_server_nearest_to_actor_smart_terrain.first = std::uint32_t(-1);
        this->m_game_server_nearest_to_actor_smart_terrain.second = std::uint32_t(-1);

#pragma endregion
    }

public:
    inline static Script_GlobalHelper& getInstance(void) noexcept
    {
        static Script_GlobalHelper instance;
        return instance;
    }

    ~Script_GlobalHelper(void) {}
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

    inline xr_map<std::uint32_t, bool>& getMonsterClasses(void) noexcept { return this->m_monster_classes; }
    inline xr_map<std::uint32_t, bool>& getStalkerClasses(void) noexcept { return this->m_stalker_classes; }
    inline xr_map<std::uint32_t, bool>& getWeaponClasses(void) noexcept { return this->m_weapon_classes; }
    inline xr_map<std::uint32_t, bool>& getArtefactClasses(void) noexcept { return this->m_artefact_classes; }
    inline xr_map<std::uint32_t, bool>& getAmmoSection(void) noexcept { return this->m_ammo_section; }
    inline xr_map<xr_string, bool>& getQuestSection(void) noexcept { return this->m_quest_section; }
    inline xr_map<xr_string, xr_string>& getSquadCommunityByBehavior(void) noexcept
    {
        return this->m_squad_community_by_behavior;
    }
    inline xr_map<xr_string, AnyCallable<void>>& getRegisteredFunctionsXREffects(void) noexcept
    {
        return this->m_registered_functions_xr_effects;
    }

    inline xr_map<xr_string, AnyCallable<bool>>& getRegisteredFunctionsXRCondition(void) noexcept
    {
        return this->m_registered_functions_xr_conditions;
    }

    // @ In-Game
    inline xr_map<xr_string, Script_SE_SmartCover*>& getGameRegisteredServerSmartCovers(void) noexcept
    {
        return this->m_game_registered_smartcovers;
    }

    // @ In-Game
    inline xr_map<std::uint8_t, xr_map<std::uint32_t, Script_SE_SmartCover*>>&
    getGameRegisteredServerSmartCoversByLevelID(void) noexcept
    {
        return this->m_game_registered_smartcovers_by_level_id;
    }

    // @ It uses in Level Editor as list of smartcovers as spawnelement and it's using in-game
    inline xr_map<xr_string, SmartCoverData>& getRegisteredSmartCovers(void) noexcept
    {
        return this->m_registered_smartcovers;
    }

    inline xr_map<xr_string, CScriptGameObject*>& getGameRegisteredCombatSpaceRestrictors(void) noexcept
    {
        return this->m_game_registered_combat_spacerestrictors;
    }

    inline xr_map<xr_string, xr_string>& getJobTypesByScheme(void) noexcept { return this->m_job_type_by_scheme; }

    inline xr_map<xr_string, Script_SE_SmartTerrain*>& getGameRegisteredServerSmartTerrainsByName(void) noexcept
    {
        return this->m_game_registered_server_smartterrains_by_name;
    }

    // @ First - id | Second - distance
    inline std::pair<std::uint32_t, std::uint32_t>& getGameNearestToActorServerSmartTerrain(void) noexcept
    {
        return this->m_game_server_nearest_to_actor_smart_terrain;
    }

    inline xr_map<xr_string, bool>& getRegisteredSmartTerrainTerritoryType(void) noexcept 
    {
        return this->m_registered_smart_terrain_territory_type;    
    }

private:
    xr_map<std::uint32_t, bool> m_monster_classes;
    xr_map<std::uint32_t, bool> m_stalker_classes;
    xr_map<std::uint32_t, bool> m_weapon_classes;
    xr_map<std::uint32_t, bool> m_artefact_classes;
    xr_map<std::uint32_t, bool> m_ammo_section;
    xr_map<xr_string, bool> m_quest_section;
    xr_map<xr_string, xr_string> m_squad_community_by_behavior;
    xr_map<xr_string, AnyCallable<void>> m_registered_functions_xr_effects;
    xr_map<xr_string, AnyCallable<bool>> m_registered_functions_xr_conditions;
    xr_map<xr_string, Script_SE_SmartCover*> m_game_registered_smartcovers;
    xr_map<std::uint8_t, xr_map<std::uint32_t, Script_SE_SmartCover*>> m_game_registered_smartcovers_by_level_id;
    xr_map<xr_string, SmartCoverData> m_registered_smartcovers;
    xr_map<xr_string, CScriptGameObject*> m_game_registered_combat_spacerestrictors;
    xr_map<xr_string, Script_SE_SmartTerrain*> m_game_registered_server_smartterrains_by_name;
    xr_map<xr_string, xr_string> m_job_type_by_scheme;
    xr_map<xr_string, bool> m_registered_smart_terrain_territory_type;
    // @ First - id | Second - distance
    std::pair<std::uint32_t, std::uint32_t> m_game_server_nearest_to_actor_smart_terrain;
};

} // namespace Scripts
} // namespace Cordis
