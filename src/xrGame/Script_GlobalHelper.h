#pragma once

#include "object_factory.h"

namespace Cordis
{
namespace Scripts
{
struct Script_GlobalHelper
{
private:
    Script_GlobalHelper(void) 
    {
        // @ Здесь инициализация всех "локальных" карт которые находятся в каждом скрипте, проверить что оно реально раньше всех инициализирует
#pragma region XR_PATROL
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

    inline xr_map<xr_string, bool>& getMonsterClasses(void) noexcept { return this->m_monster_classes; }
    inline xr_map<xr_string, bool>& getStalkerClasses(void) noexcept { return this->m_stalker_classes; }
    inline xr_map<xr_string, bool>& getWeaponClasses(void) noexcept { return this->m_weapon_classes; }
    inline xr_map<xr_string, bool>& getArtefactClasses(void) noexcept { return this->m_artefact_classes; }
    inline xr_map<xr_string, bool>& getAmmoSection(void) noexcept { return this->m_ammo_section; }
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

private:
    xr_map<xr_string, bool> m_monster_classes;
    xr_map<xr_string, bool> m_stalker_classes;
    xr_map<xr_string, bool> m_weapon_classes;
    xr_map<xr_string, bool> m_artefact_classes;
    xr_map<xr_string, bool> m_ammo_section;
    xr_map<xr_string, bool> m_quest_section;
    xr_map<xr_string, xr_string> m_squad_community_by_behavior;
    xr_map<xr_string, AnyCallable<void>> m_registered_functions_xr_effects;
    xr_map<xr_string, AnyCallable<bool>> m_registered_functions_xr_conditions;
};

} // namespace Scripts
} // namespace Cordis
