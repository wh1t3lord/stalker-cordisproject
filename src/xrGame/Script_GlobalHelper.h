#pragma once

#include "object_factory.h"

namespace Cordis
{
namespace Scripts
{
struct Script_GlobalHelper
{
private:
    Script_GlobalHelper(void) = default;

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
        // @ Lord реализовать
#pragma region XR_CONDITION

#pragma endregion

#pragma region XR_EFFECTS

#pragma endregion
    }

    inline xr_map<xr_string, bool>& getMonsterClasses(void) noexcept { return this->m_monster_classes; }
    inline xr_map<xr_string, bool>& getStalkerClasses(void) noexcept { return this->m_stalker_classes; }
    inline xr_map<xr_string, bool>& getWeaponClasses(void) noexcept { return this->m_weapon_classes; }
    inline xr_map<xr_string, bool>& getArtefactClasses(void) noexcept { return this->m_artefact_classes; }

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
