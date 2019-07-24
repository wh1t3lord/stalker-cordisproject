#pragma once

#include "xrCore/xrCore.h"
#include "xrEngine/IGame_Level.h"
#include "ai_space.h"
#include "date_time.h"
#include "alife_simulator_base.h"
#include "alife_object_registry.h"
#include "alife_graph_registry.h"
#include "alife_time_manager.h"
#include "relation_registry.h"
#include "InfoPortionDefs.h"
#include <random>

namespace Cordis
{
namespace Scripts
{
namespace Globals
{
constexpr unsigned int STYPE_STALKER = 0;
constexpr unsigned int STYPE_MOBILE = 1;
constexpr unsigned int STYPE_ITEM = 2;
constexpr unsigned int STYPE_HELI = 3;
constexpr unsigned int STYPE_RESTRICTOR = 4;

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
        std::uniform_int_distribution<int> distribution(minimum, maximum);
        return distribution(this->m_generator);
    }

    inline std::uint32_t Generate(const std::uint32_t& minimum, const std::uint32_t& maximum)
    {
        std::uniform_int_distribution<std::uint32_t> distribution(minimum, maximum);
        return distribution(this->m_generator);
    }

    inline std::uint16_t Generate(const std::uint16_t& minimum, const std::uint16_t& maximum)
    {
        std::uniform_int_distribution<std::uint16_t> distribution(minimum, maximum);
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
        std::uniform_real_distribution<float> distribution(minimum, maximum);
        return distribution(this->m_generator);
    }

private:
    std::mt19937 m_generator;
};

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
    CScriptGameObject* object, bool mandatory = false)
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

inline float cfg_get_number(const CInifile* char_ini, const xr_string& section, const xr_string& field,
    CScriptGameObject* object, bool mandatory = false)
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

} // namespace Utils

namespace Game
{
inline LPCSTR translate_string(LPCSTR str) { return *StringTable().translate(str); }
} // namespace Game

inline void load_sound(void);
inline void init_npc_sound(CScriptGameObject* npc);
inline bool has_alife_info(LPCSTR info_id);
inline int get_general_goodwill_between(const std::uint16_t& from, const std::uint16_t& to);
inline bool check_squad_for_enemies(CSE_ALifeOnlineOfflineGroup* squad);
inline bool IsMonster(CScriptGameObject* object, int class_id);
inline bool IsStalker(CScriptGameObject* object, int class_id);
inline bool IsArtefact(CScriptGameObject* object, int class_id);
inline bool IsWeapon(CScriptGameObject* object, int class_id);
inline xr_string character_community(CScriptGameObject* object);

constexpr const char* SAVE_MARKER_MODE_SAVE = "save";

inline bool is_level_present(void);

inline bool is_device_paused(void);

inline void set_device_paused(bool b);

inline const CInifile* get_system_ini(void);

#pragma region Instances
inline static xr_map<int, bool>& getMonsterClasses(void) noexcept;
inline static xr_map<int, bool>& getStalkerClasses(void) noexcept;
inline static xr_map<int, bool>& getWeaponClasses(void) noexcept;
inline static xr_map<int, bool>& getArtefactClasses(void) noexcept;
inline static xr_map<xr_string, unsigned int>& SaveMarkers(void) noexcept;
inline static xr_map<xr_string, xr_string>& Schemes(void) noexcept;
inline static xr_map<xr_string, unsigned int>& STypes(void) noexcept;
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
} // namespace Globals
} // namespace Scripts
} // namespace Cordis

#include "Script_Database.h"
#include "Script_SoundManager.h"
#include "Script_NewsManager.h"
#include "Script_XR_Condition.h"
#include "Script_XR_Logic.h"
#include "Script_XR_Sound.h"
#include "Script_SimulationBoard.h"
#include "Script_SimulationObjects.h"
#include "Script_SurgeManager.h"
#include "Script_EntitySounds.h"

#include "Script_Globalsinline.h"
