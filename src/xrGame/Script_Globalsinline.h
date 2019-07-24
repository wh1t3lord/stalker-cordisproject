#pragma once

#include "alife_registry_container_composition.h"

namespace Cordis
{
namespace Scripts
{
namespace Globals
{
inline void load_sound(void)
{
    CInifile sound_ini = CInifile("misc\\script_sound.ltx");

    if (!sound_ini.section_exist("list"))
    {
        R_ASSERT2(false, "There is no list. Please check your config (misc\\script_sound.ltx)!");
    }

    std::uint32_t count_line = sound_ini.line_count("list");
    xr_string id, value, category;

    for (int i = 0; i < count_line; ++i)
    {
        xr_string section;
        xr_string value;
        pcstr s;
        pcstr v;
        bool result = !!sound_ini.r_line("list", i, &s, &v);
        if (result)
        {
            section = s;
            value = v;

            xr_string sound_type_name = Globals::Utils::cfg_get_string(&sound_ini, section, "type");

            if (sound_type_name == "npc")
                Script_SoundThemeDataBase::getInstance().getTheme()[section] = new Script_SoundNPC(sound_ini, section);
            else if (sound_type_name == "actor")
                Script_SoundThemeDataBase::getInstance().getTheme()[section] =
                    new Script_SoundActor(sound_ini, section);
            else if (sound_type_name == "3d")
                Script_SoundThemeDataBase::getInstance().getTheme()[section] =
                    new Script_SoundObject(sound_ini, section);
            else if (sound_type_name == "looped")
                Script_SoundThemeDataBase::getInstance().getTheme()[section] =
                    new Script_SoundLooped(sound_ini, section);
        }
    }
}

inline void init_npc_sound(CScriptGameObject* npc)
{
    for (std::pair<xr_string, IScript_Sound*> it : Script_SoundThemeDataBase::getInstance().getTheme())
    {
        if (it.second)
        {
            if (it.second->getSoundType() == SCRIPTSOUNDTYPE_NPC)
            {
                if (it.second->getAvailCommunities()[character_community(npc)])
                {
                    it.second->initialize_npc(npc);
                }
            }
        }
    }
}

inline bool has_alife_info(LPCSTR info_id)
{
    const KNOWN_INFO_VECTOR* known_info =
        ai().alife().registry < CALifeAbstractRegistry<u16, KNOWN_INFO_VECTOR>>(NULL).object(0, true);
    
    if (!known_info)
    {
        R_ASSERT2(false, "object in null!");
        return false;
    }

    if (!known_info->size())
        return (false);

    if (std::find_if(known_info->begin(), known_info->end(), CFindByIDPred(info_id)) == known_info->end())
        return (false);

    return (true);
}

inline int get_general_goodwill_between(const std::uint16_t& from, const std::uint16_t& to)
{
    CHARACTER_GOODWILL presonal_goodwill = RELATION_REGISTRY().GetGoodwill(from, to);
    VERIFY(presonal_goodwill != NO_GOODWILL);

    CSE_ALifeTraderAbstract* from_obj = smart_cast<CSE_ALifeTraderAbstract*>(ai().alife().objects().object(from));
    CSE_ALifeTraderAbstract* to_obj = smart_cast<CSE_ALifeTraderAbstract*>(ai().alife().objects().object(to));

    if (!from_obj || !to_obj)
    {
        //         GEnv.ScriptEngine->script_log(LuaMessageType::Error,
        //             "RELATION_REGISTRY::get_general_goodwill_between  : cannot convert obj to
        //             CSE_ALifeTraderAbstract!");
        R_ASSERT2(false, "object is null!");
        return (0);
    }
    CHARACTER_GOODWILL community_to_obj_goodwill = RELATION_REGISTRY().GetCommunityGoodwill(from_obj->Community(), to);
    CHARACTER_GOODWILL community_to_community_goodwill =
        RELATION_REGISTRY().GetCommunityRelation(from_obj->Community(), to_obj->Community());

    return presonal_goodwill + community_to_obj_goodwill + community_to_community_goodwill;
}

inline bool check_squad_for_enemies(CSE_ALifeOnlineOfflineGroup* squad)
{
    if (!squad)
    {
        R_ASSERT2(false, "object is null!");
        return false;
    }

    for (std::pair<std::uint16_t, CSE_ALifeMonsterAbstract*> it : squad->squad_members())
    {
        if (it.second)
        {
            if (get_general_goodwill_between(it.second->ID, ai().alife().graph().actor()->ID))
                return true;
        }
    }

    return false;
}

inline bool IsMonster(CScriptGameObject* object, int class_id = 0)
{
    if (!object)
    {
        R_ASSERT2(false, "object is null!");
        return false;
    }

    int result = class_id ? class_id : object->clsid();

    return (getMonsterClasses()[result] == true);
}

inline bool IsStalker(CScriptGameObject* object, int class_id = 0)
{
    if (!object)
    {
        R_ASSERT2(false, "object in null!");
        return false;
    }

    int result = class_id ? class_id : object->clsid();

    return (getStalkerClasses()[class_id] == true);
}

inline bool IsArtefact(CScriptGameObject* object, int class_id = 0)
{
    if (!object)
    {
        R_ASSERT2(false, "object in null!");
        return false;
    }

    int result = class_id ? class_id : object->clsid();

    return (getArtefactClasses()[class_id] == true);
}

inline bool IsWeapon(CScriptGameObject* object, int class_id = 0)
{
    if (!object)
    {
        R_ASSERT2(false, "object is null!");
        return false;
    }

    int result = class_id ? class_id : object->clsid();

    return (getWeaponClasses()[class_id] == true);
}

inline xr_string character_community(CScriptGameObject* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object in null!");
        return "monster";
    }

    if (IsStalker(object))
    {
        return object->CharacterCommunity();
    }

    return "monster";
}

inline bool is_level_present(void) { return (!!g_pGameLevel); }

inline bool is_device_paused(void) { return !!Device.Paused(); }

inline void set_device_paused(bool b) { Device.Pause(b, TRUE, FALSE, "set_device_paused_script"); }

inline const CInifile* get_system_ini(void) { return (pSettings); }

inline static xr_map<int, bool>& getMonsterClasses(void) noexcept
{
    static xr_map<int, bool> instance;
    return instance;
}

inline static xr_map<int, bool>& getStalkerClasses(void) noexcept
{
    static xr_map<int, bool> instance;
    return instance;
}

inline static xr_map<int, bool>& getWeaponClasses(void) noexcept
{
    static xr_map<int, bool> instance;
    return instance;
}

inline static xr_map<int, bool>& getArtefactClasses(void) noexcept
{
    static xr_map<int, bool> instance;
    return instance;
}

inline static xr_map<xr_string, unsigned int>& SaveMarkers(void) noexcept
{
    static xr_map<xr_string, unsigned int> save_markers;
    return save_markers;
}

inline static xr_map<xr_string, xr_string>& Schemes(void) noexcept
{
    static xr_map<xr_string, xr_string> schemes;
    return schemes;
}

inline static xr_map<xr_string, unsigned int>& STypes(void) noexcept
{
    static xr_map<xr_string, unsigned int> stypes;
    return stypes;
}

inline void load_scheme(const xr_string& filename, const xr_string& scheme, unsigned int stype)
{
    Schemes()[scheme] = filename;
    STypes()[scheme] = stype;
}

inline void StartGameCallback(void)
{
    // Loads schemes Lord: переименовать соответвующие наименования скриптов, но если _G[schemes[scheme]] -> нужно
    // расписывать через case, потому что строка будет указывать на namespace или getInstance() самого класса То есть
    // switch (scheme_name) { case "Script_StoryObject": Script_StoryObject::getInstance().DoSomething(); etc... };
    load_scheme("xr_danger", "danger", STYPE_STALKER);
    load_scheme("xr_gather_items", "gather_items", STYPE_STALKER);
    load_scheme("xr_abuse", "abuse", STYPE_STALKER);
    load_scheme("xr_walker", "walker", STYPE_STALKER);
    load_scheme("xr_remark", "remark", STYPE_STALKER);
    load_scheme("xr_cover", "cover", STYPE_STALKER);
    load_scheme("xr_sleeper", "sleeper", STYPE_STALKER);
    load_scheme("xr_kamp", "kamp", STYPE_STALKER);
    load_scheme("xr_camper", "camper", STYPE_STALKER);
    load_scheme("xr_meet", "meet", STYPE_STALKER);
    load_scheme("xr_help_wounded", "help_wounded", STYPE_STALKER);
    load_scheme("xr_corpse_detection", "corpse_detection", STYPE_STALKER);
    load_scheme("xr_combat", "combat", STYPE_STALKER);
    load_scheme("xr_death", "death", STYPE_STALKER);
    load_scheme("xr_hit", "hit", STYPE_STALKER);
    load_scheme("xr_wounded", "wounded", STYPE_STALKER);
    load_scheme("xr_meet", "actor_dialogs", STYPE_STALKER);
    load_scheme("xr_combat_ignore", "combat_ignore", STYPE_STALKER);
    load_scheme("xr_combat_zombied", "combat_zombied", STYPE_STALKER);
    load_scheme("xr_patrol", "patrol", STYPE_STALKER);
    load_scheme("xr_smartcover", "smartcover", STYPE_STALKER);
    load_scheme("xr_companion", "companion", STYPE_STALKER);
    load_scheme("xr_animpoint", "animpoint", STYPE_STALKER);
    load_scheme("xr_reach_task", "reach_task", STYPE_STALKER);

    load_scheme("mob_remark", "mob_remark", STYPE_MOBILE);
    load_scheme("mob_walker", "mob_walker", STYPE_MOBILE);
    load_scheme("mob_combat", "mob_combat", STYPE_MOBILE);
    load_scheme("mob_death", "mob_death", STYPE_MOBILE);
    load_scheme("mob_jump", "mob_jump", STYPE_MOBILE);
    load_scheme("mob_home", "mob_home", STYPE_MOBILE);

    load_scheme("ph_door", "ph_door", STYPE_ITEM);
    load_scheme("ph_idle", "ph_idle", STYPE_ITEM);
    load_scheme("ph_hit", "ph_hit", STYPE_ITEM);
    load_scheme("ph_on_hit", "ph_on_hit", STYPE_ITEM);
    load_scheme("ph_button", "ph_button", STYPE_ITEM);
    load_scheme("ph_code", "ph_code", STYPE_ITEM);
    load_scheme("ph_death", "ph_on_death", STYPE_ITEM);
    load_scheme("ph_minigun", "ph_minigun", STYPE_ITEM);
    // load_scheme("ph_target","ph_target",STYPE_ITEM);
    load_scheme("ph_oscillate", "ph_oscillate", STYPE_ITEM);

    load_scheme("heli_move", "heli_move", STYPE_HELI);

    load_scheme("sr_no_weapon", "sr_no_weapon", STYPE_RESTRICTOR);
    load_scheme("sr_teleport", "sr_teleport", STYPE_RESTRICTOR);
    load_scheme("sr_idle", "sr_idle", STYPE_RESTRICTOR);
    load_scheme("sr_light", "sr_light", STYPE_RESTRICTOR);
    load_scheme("sr_timer", "sr_timer", STYPE_RESTRICTOR);
    load_scheme("sr_psy_antenna", "sr_psy_antenna", STYPE_RESTRICTOR);
    load_scheme("sr_postprocess", "sr_postprocess", STYPE_RESTRICTOR);
    load_scheme("sr_particle", "sr_particle", STYPE_RESTRICTOR);
    load_scheme("sr_cutscene", "sr_cutscene", STYPE_RESTRICTOR);
    // load_scheme("sr_bloodsucker","sr_bloodsucker",STYPE_RESTRICTOR);
    load_scheme("sr_monster", "sr_monster", STYPE_RESTRICTOR);
    // load_scheme("sr_robbery","sr_robbery",STYPE_RESTRICTOR);
    // load_scheme("sr_survival","sr_survival",STYPE_RESTRICTOR);
    load_scheme("sr_crow_spawner", "sr_crow_spawner", STYPE_RESTRICTOR);
    // load_scheme("sr_shooting","sr_shooting",STYPE_RESTRICTOR);
    // load_scheme("sr_recoveritem","sr_recoveritem",STYPE_RESTRICTOR);
    load_scheme("sr_silence", "sr_silence", STYPE_RESTRICTOR);
    load_scheme("sr_deimos", "sr_deimos", STYPE_RESTRICTOR);
}

inline void set_save_marker(NET_Packet& packet, const xr_string& mode, bool check, const xr_string& prefix)
{
    xr_string result = "_";
    result.append(prefix);

    if (check)
    {
        if (SaveMarkers()[result] == 0)
        {
            R_ASSERT3(false, "Trying to check without marker ", result.c_str());
        }

        if (mode == SAVE_MARKER_MODE_SAVE)
        {
            unsigned int delta = packet.w_tell() - SaveMarkers()[result];
            // Lord: переделать в In-Game логгирование
            xr_string info = "";
            info.append(result);
            info.append(":SAVE DELTA: ");
            Log(info.c_str(), delta);

            if (delta >= 8000)
            {
                // Lord: переделать в In-Game логгирование
                Log("WARNING! Maybe this is problem save point");
            }

            if (delta >= 10240)
            {
                // empty code here
                // R_ASSERT2(false, "You are saving too much");
            }
            packet.w_u16(delta);
        }
        else
        {
            const unsigned int c_delta = packet.r_tell() - SaveMarkers()[result];
            unsigned int delta = packet.r_u16();

            if (delta != c_delta)
            {
                Msg("INCORRECT LOAD [%s].[%d][%d]", result.c_str(), delta, c_delta);
                R_ASSERT(false);
            }
            else
            {
                xr_string info = "";
                info.append(result);
                info.append(": LOAD DELTA: %d");
                Msg(info.c_str(), delta);
            }
        }
        return;
    }

    if (mode == SAVE_MARKER_MODE_SAVE)
    {
        xr_string info = "";
        info.append(result);
        info.append(": set save marker: %d");
        Msg(info.c_str(), packet.w_tell());
        SaveMarkers()[result] = packet.w_tell();

        if (packet.w_tell() > 16000)
        {
            R_ASSERT2(false, "You are trying save too much");
        }
    }
    else
    {
        xr_string info = "";
        info.append(result);
        info.append(": set load marker: %d");
        Msg(info.c_str(), packet.r_tell());
        SaveMarkers()[result] = packet.r_tell();
    }
}

inline bool is_on_the_same_level(CSE_ALifeObject* object1, CSE_ALifeObject* object2)
{
    if (!object1 || !object2)
    {
        R_ASSERT2(false, "Object was null!");
        return false;
    }

    return (ai().game_graph().vertex(object1->m_tGraphID)->level_id() ==
        ai().game_graph().vertex(object2->m_tGraphID)->level_id());
}

inline float sim_dist_to(CSE_ALifeObject* object1, CSE_ALifeObject* object2)
{
    if (!object1 || !object2)
    {
        R_ASSERT2(false, "Object was null!");
        return 0.0f;
    }

    std::uint16_t gamevertexid_1 = object1->m_tGraphID;
    std::uint16_t gamevertexid_2 = object2->m_tGraphID;

    return Utils::graph_distance(gamevertexid_1, gamevertexid_2);
}

inline ESingleGameDifficulty get_game_difficulty() { return g_SingleGameDifficulty; }

inline std::uint32_t get_time_days()
{
    std::uint32_t year = 0, month = 0, day = 0, hours = 0, mins = 0, secs = 0, milisecs = 0;
    split_time((g_pGameLevel && Level().game) ? Level().GetGameTime() : ai().alife().time_manager().game_time(), year,
        month, day, hours, mins, secs, milisecs);
    return day;
}

inline std::uint32_t get_time_hours()
{
    std::uint32_t year = 0, month = 0, day = 0, hours = 0, mins = 0, secs = 0, milisecs = 0;
    split_time((g_pGameLevel && Level().game) ? Level().GetGameTime() : ai().alife().time_manager().game_time(), year,
        month, day, hours, mins, secs, milisecs);
    return hours;
}

inline std::uint32_t get_time_minutes()
{
    std::uint32_t year = 0, month = 0, day = 0, hours = 0, mins = 0, secs = 0, milisecs = 0;
    split_time((g_pGameLevel && Level().game) ? Level().GetGameTime() : ai().alife().time_manager().game_time(), year,
        month, day, hours, mins, secs, milisecs);
    return mins;
}

inline bool in_time_interval(const std::uint32_t& value1, const std::uint32_t& value2)
{
    std::uint32_t game_hours = get_time_hours();

    if (value1 >= value2)
    {
        return ((game_hours < value2) || (game_hours >= value1));
    }
    else
    {
        return ((game_hours < value2) && (game_hours >= value1));
    }

    return false;
}
} // namespace Globals
} // namespace Scripts
} // namespace Cordis
