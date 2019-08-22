#pragma once

#include "xrCore/xrCore.h"
#include "script_ini_file.h"
#include "xrEngine/IGame_Level.h"
#include "ai_space.h"
#include "date_time.h"
#include "alife_simulator_base.h"
#include "alife_object_registry.h"
#include "alife_graph_registry.h"
#include "alife_time_manager.h"
#include "relation_registry.h"
#include "InfoPortionDefs.h"
#include "xrServer_Objects_ALife_All.h"
#include "xrServer_Objects_Alife_Smartcovers.h"
#include "script_movement_action.h"
#include "xr_time.h"
#include <random>
#include "xrAICore/AISpaceBase.hpp"
#include "xrAICore/Navigation/PatrolPath/patrol_path_storage.h"
#include "xrAICore/Navigation/PatrolPath/patrol_path_params.h"

namespace Cordis
{
namespace Scripts
{
namespace Globals
{
#pragma region Cordis Constants
constexpr unsigned int kSTypeStalker = 0;
constexpr unsigned int kSTypeMobile = 1;
constexpr unsigned int kSTypeItem = 2;
constexpr unsigned int kSTypeHelicopter = 3;
constexpr unsigned int kSTypeRestrictor = 4;

#pragma region Cordis SimulationSquad
constexpr float kSimulationSquadActionsStayPointIdleMin = 180.0f * 60.0f;
constexpr float kSimulationSquadActionsStayPointIdleMax = 300.0f * 60.0f;
constexpr const char* kSimulationSquadCurrentActionIDStayOnTarget = "stay_target";
constexpr const char* kSimulationSquadCurrentActionIDReachTarget = "reach_target";
#pragma endregion

#pragma region Cordis SmartTerrain
constexpr std::uint32_t kSmartTerrainDeathIdleTime = 600;
constexpr std::uint32_t kSmartTerrainRespawnIdle = 1000;
constexpr std::uint32_t kSmartTerrainRespawnRadius = 150;
constexpr const char* kSmartTerrainTerritoryDefault = "default";
constexpr const char* kSmartTerrainTerritoryBase = "base";
constexpr const char* kSmartTerrainTerritoryResource = "resource";
constexpr const char* kSmartTerrainTerritoryTerritory = "territory";
constexpr const char* kSmartTerrainSMRTSection = "smart_terrain";
constexpr const char* kSmartTerrainPathFieldPathWalk = "path_walk";
constexpr const char* kSmartTerrainPathFieldPathMain = "path_main";
constexpr const char* kSmartTerrainPathFieldPathHome = "path_home";
constexpr const char* kSmartTerrainPathFieldCenterPoint = "center_point";
#pragma endregion

#pragma region Cordis SYSTEM
constexpr std::uint64_t kUnsignedInt64Undefined = std::uint64_t(-1);
constexpr std::uint32_t kUnsignedInt32Undefined = std::uint32_t(-1);
constexpr std::uint16_t kUnsignedInt16Undefined = std::uint16_t(-1);
constexpr std::uint8_t kUnsignedInt8Undefined = std::uint8_t(-1);
#pragma endregion

namespace XR_ACTIONS_ID
{
constexpr std::uint32_t kGlobalActions = StalkerDecisionSpace::eWorldOperatorScript;
constexpr std::uint32_t kActionScript = StalkerDecisionSpace::eWorldOperatorScript + 1;
constexpr std::uint32_t kAlife = StalkerDecisionSpace::eWorldOperatorALifePlanner;
constexpr std::uint32_t kScriptCombatPlanner = StalkerDecisionSpace::eWorldOperatorScript + 3;
constexpr std::uint32_t kReachTaskLocation = StalkerDecisionSpace::eWorldOperatorScript + 4;
constexpr std::uint32_t kCorpseExist = StalkerDecisionSpace::eWorldOperatorScript + 50;
constexpr std::uint32_t kWoundedExist = StalkerDecisionSpace::eWorldOperatorScript + 55;
constexpr std::uint32_t kStateManager = StalkerDecisionSpace::eWorldOperatorScript + 100;
constexpr std::uint32_t kSmartcoverAction = StalkerDecisionSpace::eWorldOperatorScript + 215;
constexpr std::uint32_t kTestReaction = StalkerDecisionSpace::eWorldOperatorScript + 218;

constexpr std::uint32_t kStoheActions = StalkerDecisionSpace::eWorldOperatorScript + 220;
constexpr std::uint32_t kStoheTradesellerInit = kStoheActions + 1;
constexpr std::uint32_t kStoheTradesellerBase = kStoheActions + 10;
constexpr std::uint32_t kStoheKampBase = kStoheActions + 20;
constexpr std::uint32_t kStoheMeetBase = kStoheActions + 30;
constexpr std::uint32_t kStoheReactionsBase = kStoheActions + 40;
constexpr std::uint32_t kStoheStateBase = kStoheActions + 50;
constexpr std::uint32_t kStohePosition = kStoheActions + 60;
constexpr std::uint32_t kStoheGulagBase = kStoheActions + 70;
constexpr std::uint32_t kStoheCoverBase = kStoheActions + 80;
constexpr std::uint32_t kStoheCamperBase = kStoheActions + 90;
constexpr std::uint32_t kAssistance = kStoheActions + 100;
constexpr std::uint32_t kDeath = kStoheActions + 110;
constexpr std::uint32_t kAbuseBase = kStoheActions + 120;
constexpr std::uint32_t kPostCombat = kStoheActions + 130;
constexpr std::uint32_t kAnimationPointAction = kStoheActions + 140;
constexpr std::uint32_t kSidorActions = kStoheActions + 1024;
constexpr std::uint32_t kSidorActCommander = kSidorActions + 0; // @ ???
constexpr std::uint32_t kSidorActSoldier = kSidorActions + 5;
constexpr std::uint32_t kSidorActPatrol = kSidorActions + 0; // @ ???
constexpr std::uint32_t kSidorActWoundedBase = kSidorActions + 10;
constexpr std::uint32_t kSidorActBanditsBase = kSidorActions + 20;
constexpr std::uint32_t kSidorActSwatBase = kSidorActions + 40;
constexpr std::uint32_t kSidorActFollowerBase = kSidorActions + 50;
constexpr std::uint32_t kSidorActReportLeader = kSidorActions + 60;
constexpr std::uint32_t kSidorActReportSoldier = kSidorActions + 70;
constexpr std::uint32_t kSidorActRest = kSidorActions + 80;
constexpr std::uint32_t kSidorActBar = kSidorActions + 90;
constexpr std::uint32_t kSidorActChaser = kSidorActions + 100;
constexpr std::uint32_t kSidorActSos = kSidorActions + 110;
constexpr std::uint32_t kSidorActPtr = kSidorActions + 120;
constexpr std::uint32_t kSidorActYantar = kSidorActions + 200;
constexpr std::uint32_t kSidorActSecretLab = kSidorActions + 220;
constexpr std::uint32_t kSidorAttendantAct = kSidorActions + 230;

constexpr std::uint32_t kChugaiActions = kSidorActions + 1024;
constexpr std::uint32_t kChugaiActTutorial = kChugaiActions + 10;
constexpr std::uint32_t kChugaiCommanderBase = kChugaiActions + 20;
constexpr std::uint32_t kChugaiScoutBase = kChugaiActions + 30;
constexpr std::uint32_t kChugaiMilitaryCommanderBase = kChugaiActions + 40;
constexpr std::uint32_t kChugaiEscapeFactoryBanditsBase = kChugaiActions + 50;
constexpr std::uint32_t kChugaiBarmanBase = kChugaiActions + 60;
constexpr std::uint32_t kChugaiAmbushBase = kChugaiActions + 70;
constexpr std::uint32_t kChugaiFormationBase = kChugaiActions + 80;
constexpr std::uint32_t kChugaiReplicsBase = kChugaiActions + 90;
constexpr std::uint32_t kChugaiHelicopterHunterBase = kChugaiActions + 100;
constexpr std::uint32_t kChugaiZombiedBase = kChugaiActions + 110;
constexpr std::uint32_t kChugaiMonolithBase = kChugaiActions + 120;
constexpr std::uint32_t kChugaiCamperBase = kChugaiActions + 130;

constexpr std::uint32_t kZmeyActions = kChugaiActions + 1024;
constexpr std::uint32_t kZmeyActionGuardActivity = kZmeyActions + 100;
constexpr std::uint32_t kZmeyGuardBase = kZmeyActions + 200;
constexpr std::uint32_t kZmeyRobberBase = kZmeyActions + 230;
constexpr std::uint32_t kZmeyCamperBase = kZmeyActions + 260;
constexpr std::uint32_t kZmeySleeperBase = kZmeyActions + 290;
constexpr std::uint32_t kZmeyTalkerBase = kZmeyActions + 320;
constexpr std::uint32_t kZmeyBodyGuardBase = kZmeyActions + 350;
constexpr std::uint32_t kZmeyPunchBase = kZmeyActions + 380;
constexpr std::uint32_t kZmeyGuideBase = kZmeyActions + 410;
constexpr std::uint32_t kZmeyRobberGuardBase = kZmeyActions + 440;
constexpr std::uint32_t kZmeyWalkerBase = kZmeyActions + 480;
constexpr std::uint32_t kZmeyAssassinBase = kZmeyActions + 510;
constexpr std::uint32_t kZmeyScaredBase = kZmeyActions + 540;
constexpr std::uint32_t kZmeyZoneGuardBase = kZmeyActions + 570;
constexpr std::uint32_t kZmeyTestBase = kZmeyActions + 600;
constexpr std::uint32_t kZmeyRemarkBase = kZmeyActions + 630;
constexpr std::uint32_t kZmeyCompanionBase = kZmeyActions + 660;

constexpr std::uint32_t kJonActions = kZmeyActions + 1024;
constexpr std::uint32_t kJonStalkerZombie = kJonActions + 0; // @ ???
constexpr std::uint32_t kJonStalkerNovice = kJonActions + 1;
constexpr std::uint32_t kJonStalkerNovice2 = kJonActions + 2;
constexpr std::uint32_t kJonStalkerNovice3 = kJonActions + 3;
constexpr std::uint32_t kJonStalkerAttack = kJonActions + 4;
constexpr std::uint32_t kJonScientist = kJonActions + 5;
constexpr std::uint32_t kJonSecurity = kJonActions + 6;
constexpr std::uint32_t kJonSniperEnabled = kJonActions + 10;
constexpr std::uint32_t kJonSniperSeeMonster = kJonActions + 11;
constexpr std::uint32_t kJonSniperSeeEnemy = kJonActions + 12;
constexpr std::uint32_t kJonSniperCamper = kJonActions + 13;
constexpr std::uint32_t kJonOutcast = kJonActions + 20;
constexpr std::uint32_t kJonOutcastReplics = kJonActions + 21;

constexpr std::uint32_t kNarratorActions = kJonActions + 1024;
constexpr std::uint32_t kPeacemakerActions = kNarratorActions + 1024;
constexpr std::uint32_t kPeacemakerShBase = kPeacemakerActions + 10;
} // namespace XR_ACTIONS_ID

namespace GulagGenerator
{
constexpr const char* kGulagJobPoint = "point_job";
constexpr const char* kGulagJobPath = "path_job";
constexpr const char* kGulagJobSmartCover = "smartcover_job";
constexpr const char* kGulagJobNameWalker = "walker";
constexpr const char* kGulagJobNameCamper = "camper";
constexpr const char* kGulagJobNamePatrol = "patrol";
constexpr const char* kGulagJobNameAnimpoint = "animpoint";
constexpr const char* kGulagJobNameSmartCover = "smartcover";
constexpr const char* kGulagJobNameRemark = "remark";
constexpr const char* kGulagJobNameCover = "cover";
constexpr const char* kGulagJobNameSleeper = "sleeper";
constexpr const char* kGulagJobNameMobWalker = "mob_walker";
constexpr const char* kGulagJobNameMobHome = "mob_home";
constexpr const char* kGulagJobNameMobJump = "mob_jump";
constexpr const char* kGulagJobNameCompanion = "companion";

enum
{
    kJobsStalker,
    kJobsMonster
};
} // namespace GulagGenerator

#pragma endregion

#pragma region RandomImplementation
class Script_RandomInt
{
private:
    Script_RandomInt(void) : m_generator(std::random_device()()) {}

public:
    inline static Script_RandomInt& getInstance(void) noexcept
    {
        static Script_RandomInt instance;
        return instance;
    }

    ~Script_RandomInt(void) = default;
    Script_RandomInt(const Script_RandomInt&) = delete;
    Script_RandomInt& operator=(const Script_RandomInt&) = delete;
    Script_RandomInt(Script_RandomInt&&) = delete;
    Script_RandomInt& operator=(Script_RandomInt&&) = delete;

    inline int Generate(const int& minimum, const int& maximum)
    {
        if (minimum > maximum)
        {
            R_ASSERT2(false, "It can't be! Rewrite values minimum and maximum!!!");
        }

        std::uniform_int_distribution<int> distribution(minimum, maximum);
        return distribution(this->m_generator);
    }

    inline std::uint32_t Generate(const std::uint32_t& minimum, const std::uint32_t& maximum)
    {
        if (minimum > maximum)
        {
            R_ASSERT2(false, "It can't be! Rewrite values minimum and maximum!!!");
        }

        std::uniform_int_distribution<std::uint32_t> distribution(minimum, maximum);
        return distribution(this->m_generator);
    }

private:
    std::mt19937 m_generator;
};

class Script_RandomFloat
{
private:
    Script_RandomFloat(void) : m_generator(std::random_device()()) {}

public:
    inline static Script_RandomFloat& getInstance(void) noexcept
    {
        static Script_RandomFloat instance;
        return instance;
    }

    ~Script_RandomFloat(void) = default;
    Script_RandomFloat(const Script_RandomFloat&) = delete;
    Script_RandomFloat& operator=(const Script_RandomFloat&) = delete;
    Script_RandomFloat(Script_RandomFloat&&) = delete;
    Script_RandomFloat& operator=(Script_RandomFloat&&) = delete;

    inline float Generate(const float& minimum, const float& maximum)
    {
        if (minimum > maximum)
        {
            R_ASSERT2(false, "It can't be. Rewrite values minimum and maximum!");
        }

        std::uniform_real_distribution<float> distribution(minimum, maximum);
        return distribution(this->m_generator);
    }

private:
    std::mt19937 m_generator;
};
#pragma endregion

#pragma region Cordis Custom Datas
struct StayReachOnTarget
{
    float m_idle_time = Globals::Script_RandomFloat::getInstance().Generate(
        kSimulationSquadActionsStayPointIdleMin, kSimulationSquadActionsStayPointIdleMax);
    std::uint16_t m_squad_id = kUnsignedInt16Undefined;
    xrTime m_start_time = 0;
    xr_string m_name = "";

    inline bool update(const bool& value)
    {
        if (!this->m_name.size())
        {
            R_ASSERT2(false, "You must indentifying your class's ID!");
            return false;
        }

        if (this->m_squad_id && this->m_name == kSimulationSquadCurrentActionIDReachTarget)
        {
        }
        else
        {
            R_ASSERT2(false, "Something bad!");
        }

        if (this->m_squad_id == kUnsignedInt16Undefined)
        {
            if (!value)
                return true;

            return get_time_struct().diffSec(this->m_start_time) > this->m_idle_time;
        }
        else
        {
            R_ASSERT2(false, "Something bad!");
        }

        return false;
    }

    inline void make(const bool& value) { this->m_start_time = get_time_struct(); }

    inline void Clear(void) noexcept
    {
        this->m_name.clear();
        this->m_idle_time = 0.0f;
        this->m_start_time = 0;
    }
};

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
inline xr_string cfg_get_string(const CInifile* char_ini, const xr_string& section, const xr_string& field,
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

inline bool cfg_get_bool(const CInifile* char_ini, const xr_string& section, const xr_string& field,
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

inline bool cfg_get_bool(const CInifile* char_ini, const xr_string& section, const xr_string& field,
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

inline float cfg_get_number(const CInifile* char_ini, const xr_string& section, const xr_string& field,
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

inline float cfg_get_number(const CInifile* char_ini, const xr_string& section, const xr_string& field,
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

} // namespace Utils
namespace Game
{
inline LPCSTR translate_string(LPCSTR str) { return *StringTable().translate(str); }
inline xrTime get_game_time(void) noexcept { return get_time_struct(); }
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

        CSE_ALifeDynamicObject* object = ai().alife().objects().object(id_parent, true);
    if (!object)
    {
        Msg("! invalid parent id [%d] specified", id_parent);
        return (0);
    }

    CALifeSimulator* alife = const_cast<CALifeSimulator*>(ai().get_alife());

    if (!object->m_bOnline)
        return (alife->spawn_item(section.c_str(), position, level_vertex_id, game_vertex_id, id_parent));

    NET_Packet packet;
    packet.w_begin(M_SPAWN);
    packet.w_stringZ(section.c_str());

    CSE_Abstract* item = alife->spawn_item(section.c_str(), position, level_vertex_id, game_vertex_id, id_parent, false);
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

namespace level
{
inline LPCSTR get_name(void) { return (*Level().name()); }
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
} // namespace GameRelations

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
inline void StartGameCallback(void);
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

#include "Script_EntitySounds.h"
#include "Script_SE_Actor.h"
#include "Script_SE_SmartCover.h"
#include "Script_SmartTerrainControl.h"
/*#include "Script_GulagGenerator.h"*/
#include "Script_SE_SmartTerrain.h"
#include "Script_SE_SimulationSquad.h"
#include "Script_LogicEntity.h"
#include "Script_LogicManager.h"
#include "Script_Database.h"
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
#include "Script_SimulationObjects.h"
#include "Script_SurgeManager.h"
#include "Script_EntitySounds.h"
#include "Script_Globalsinline.h"
