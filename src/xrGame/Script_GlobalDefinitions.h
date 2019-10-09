#pragma once

namespace Cordis
{
namespace Scripts
{
namespace XR_CONDITION
{
inline bool is_surge_started(void);
}
} // namespace Scripts
} // namespace Cordis

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
constexpr const char* kRelationsTypeEnemy = "enemy";
constexpr const char* kRelationsTypeFriends = "friends";
constexpr const char* kRelationsTypeNeutral = "neutral";
constexpr int kRelationKoeffEnemy = -1000;
constexpr int kRelationKoeffNeutral = 0;
constexpr int kRelationKoeffFriend = 1000;
constexpr float kRelationDefaultSympathy = 0.1f;
constexpr const char* kGeneratedIdForBadValue = "EmptyNameItWasEmptyValue";
#pragma region Cordis Animpoint prediction functions
inline bool predicate_const_true(std::uint16_t, bool);
inline bool predicate_animpoint_bread(std::uint16_t npc_id, bool);
inline bool predicate_animpoint_kolbasa(std::uint16_t npc_id, bool);
inline bool predicate_animpoint_vodka(std::uint16_t npc_id, bool);
inline bool predicate_animpoint_energy(std::uint16_t npc_id, bool);
inline bool predicate_animpoint_guitar(std::uint16_t npc_id, bool is_in_camp);
inline bool predicate_animpoint_harmonica(std::uint16_t npc_id, bool is_in_camp);
inline bool predicate_animpoint_weapon(std::uint16_t npc_id, bool);
#pragma endregion

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
constexpr const char* kStringUndefined = "Undefined";
constexpr char kPstorBooleanTrue = '+';
constexpr char kPstorBooleanUndefined = '0';
constexpr char kPstorBooleanFalse = '-';
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

    inline float Generate(const float& minimum = 0.0f, const float& maximum = 1.0f)
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
} // namespace Globals
} // namespace Scripts
} // namespace Cordis
