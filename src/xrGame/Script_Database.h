#pragma once



namespace Cordis
{
namespace Scripts
{
namespace DataBase
{
class Storage_Scheme;
class Storage_Data;
}
} // namespace Scripts
} // namespace Cordis

#include "script_sound.h"
#include "Script_ISchemeEntity.h"
#include "Script_SchemePHDoor.h"
#include "Script_MoveManager.h"
#include "Script_Binder_AnomalZone.h"
#include "Script_Binder_AnomalField.h"
#include "Script_Binder_DoorLabx8.h"
#include "Script_CampData.h"
#include "Script_TaskManager.h"
#include "Script_SchemeXRKamp.h"
#include "Script_SchemeXRMeet.h"
#include "Script_SchemeXRWounded.h"
#include "Script_SchemeXRPatrol.h"
#include "Script_SchemeXRReachTask.h"
#include "Script_SchemeXRAnimpoint.h"


namespace Cordis
{
namespace Scripts
{
namespace DataBase
{
// Lord: зачем оно и что это в итоге?
inline void add_enemy(CSE_Abstract* object)
{
    if (!object)
    {
        R_ASSERT2(false, "can't be null!");
        return;
    }
}

class Data_Overrides
{
public:
    Data_Overrides(void) = default;
    ~Data_Overrides(void) {}

    inline std::uint32_t getMinPostCombatTime(void) const noexcept { return this->m_min_post_combat_time; }
    inline void setMinPostCombatTime(const std::uint32_t value) noexcept { this->m_min_post_combat_time = value; }

    inline std::uint32_t getMaxPostCombatTime(void) const noexcept { return this->m_max_post_combat_time; }
    inline void setMaxPostCombatTime(const std::uint32_t value) noexcept { this->m_max_post_combat_time = value; }

    inline const xr_map<std::uint32_t, CondlistData>& getOnOfflineCondlist(void) const noexcept
    {
        return this->m_on_offline_condlist;
    }
    inline void setOnOfflineCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
    {
        this->m_on_offline_condlist = condlist;
    }

    inline const LogicData& getCombatIgnore(void) const noexcept { return this->m_combat_ignore; }
    inline void setCombatIgnore(const LogicData& data) noexcept { this->m_combat_ignore = data; }

    inline bool getCombatIgnoreKeepWhenAttacked(void) const noexcept
    {
        return this->m_combat_ignore_keep_when_attacked;
    }
    inline void setCombatIgnoreKeepWhenAttacked(const bool value) noexcept
    {
        this->m_combat_ignore_keep_when_attacked = value;
    }

    inline const LogicData& getCombatType(void) const noexcept { return this->m_combat_type; }
    inline void setCombatType(const LogicData& data) noexcept { this->m_combat_type = data; }

    inline const LogicData& getOnCombat(void) const noexcept { return this->m_on_combat; }
    inline void setOnCombat(const LogicData& data) noexcept { this->m_on_combat = data; }

    inline const xr_map<std::uint32_t, CondlistData>& getHelicopterHunterCondlist(void) const noexcept
    {
        return this->m_helicopter_hunter_condlist;
    }

    inline void setHelicopterHunterCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
    {
        this->m_helicopter_hunter_condlist = condlist;
    }

    inline const xr_string& getSoundGroupName(void) const noexcept { return this->m_sound_group_name; }
    inline void setSoundGroupName(const xr_string& sound_group_name) noexcept
    {
        if (sound_group_name.empty())
        {
            MESSAGEW("sound_group_name.empty() == "
                "true! You set an empty string!");
        }

        this->m_sound_group_name = sound_group_name;
    }

    inline bool isEmpty(void) const noexcept
    {
        return ((!this->m_combat_ignore_keep_when_attacked) && (!this->m_min_post_combat_time) &&
            (!this->m_max_post_combat_time) && (this->m_helicopter_hunter_condlist.empty()) &&
            (this->m_on_offline_condlist.empty()) && (this->m_sound_group_name.empty()) &&
            (this->m_combat_ignore.IsEmpty()) && (this->m_combat_type.IsEmpty()) && (this->m_on_combat.IsEmpty()));
    }

private:
    bool m_combat_ignore_keep_when_attacked = false;
    std::uint32_t m_min_post_combat_time = 0;
    std::uint32_t m_max_post_combat_time = 0;
    xr_map<std::uint32_t, CondlistData> m_helicopter_hunter_condlist;
    xr_map<std::uint32_t, CondlistData> m_on_offline_condlist;
    xr_string m_sound_group_name;
    LogicData m_combat_ignore;
    LogicData m_combat_type;
    LogicData m_on_combat;
};

class Script_XRAbuseManager
{
public:
    Script_XRAbuseManager(void) = delete;
    Script_XRAbuseManager(CScriptGameObject* const p_client_object, void* storage)
        : m_p_npc(p_client_object), m_p_storage(reinterpret_cast<Script_ComponentScheme_XRAbuse*>(storage)), m_is_enable(true), m_is_hit_done(false), m_abuse_rate(2.0f),
          m_abuse_threshold(5.0f), m_abuse_value(0.0f), m_last_update(0.0f)
    {
    }
    ~Script_XRAbuseManager(void) {}

    inline void setAbuseRate(const float value) noexcept { this->m_abuse_rate = value; }
    inline bool IsAbused(void) const noexcept { return (this->m_abuse_value >= this->m_abuse_threshold); }
    inline bool update(void) noexcept
    {
        if (!this->m_last_update)
            this->m_last_update = static_cast<float>(Globals::get_time_global());

        if (this->m_abuse_value)
            this->m_abuse_value =
                this->m_abuse_value - (static_cast<float>(Globals::get_time_global()) - this->m_last_update) * 0.00005f;
        else
            this->m_abuse_value = 0.0f;

        if (this->m_abuse_value > this->m_abuse_threshold * 1.1f)
            this->m_abuse_value = this->m_abuse_threshold * 1.1f;

        if (this->m_is_hit_done && (this->m_abuse_value < this->m_abuse_threshold * 2.0f / 3.0f))
            this->m_is_hit_done = false;

        this->m_last_update = static_cast<float>(Globals::get_time_global());

        if (this->IsAbused())
            return true;

        return false;
    }

    inline void AddAbuse(const float value) noexcept
    {
        if (this->m_is_enable)
            this->m_abuse_value = this->m_abuse_value + (value * this->m_abuse_rate);
    }

    inline void ClearAbuse(void) noexcept { this->m_abuse_value = 0.0f; }
    inline void EnableAbuse(void) noexcept { this->m_is_enable = true; }
    inline void DisableAbuse(void) noexcept { this->m_is_enable = false; }

private:
    bool m_is_enable;
    bool m_is_hit_done;
    float m_last_update;
    float m_abuse_value;
    float m_abuse_threshold;
    float m_abuse_rate;
    CScriptGameObject* m_p_npc;
    Script_ComponentScheme_XRAbuse* m_p_storage;
};

// Lord: сгруппировать всё по pragma region!!!
class Storage_Scheme
{
public:
    ~Storage_Scheme(void)
    {
        this->m_p_ini =
            nullptr; // @ Контролиться в Script_SE_SmartTerrain, либо от this->m_ltx, либо от this->m_job_data

/* 
        if (!this->m_actions.empty())
        {
            for (Script_ISchemeEntity* it : this->m_actions)
            {
                if (it)
                {
                    MESSAGEI("deleting scheme %s from actions",
                        it->getSchemeName().c_str());
                    xr_delete(it);
                }
            }
        }*/

        if (this->m_p_jump_path)
        {
            MESSAGEI("deleting CPatrolPathParams m_p_jump_path %s",
                this->m_p_jump_path->m_path_name);
            xr_delete(this->m_p_jump_path);
        }



        this->m_p_npc = nullptr;
    }

    /*
        inline void setAction(Script_ISchemeEntity* p_scheme)
        {
            if (!p_scheme)
            {
                Msg("[Scripts/DataBase/Storage_Scheme/setAction(p_scheme)] WARNING: you can't assign nullptr object
       return "
                    "...!");
                return;
            }

            this->m_p_action = p_scheme;
        }*/

    inline void ClearApprovedActions(void) noexcept 
    {
        MESSAGE("clearing approved actions");
        this->m_approved_actions.clear();
    }

    inline const xr_string& getPathWalkName(void) const noexcept { return this->m_path_walk_name; }
    inline void setPathWalkName(const xr_string& path_walk_name) noexcept
    {
        if (path_walk_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPathWalkName(path_walk_name)] WARNING: path_walk_name.empty() == "
                "true! You are assign an empty string");
        }

        this->m_path_walk_name = path_walk_name;
    }

    inline const xr_string& getPathLookName(void) const noexcept { return this->m_path_look_name; }
    inline void setPathLookName(const xr_string& path_look_name) noexcept
    {
        if (path_look_name.empty())
        {
            Msg("[Scripts/DataBase/Script_Scheme/setPathLookName(path_look_name)] WARNING: path_look_name.empty() == "
                "true! You set an empty string");
        }

        this->m_path_look_name = path_look_name;
    }

    inline const xr_string& getStateName(void) const noexcept { return this->m_state_name; }
    inline void setStateName(const xr_string& state_name) noexcept
    {
        if (state_name.empty())
        {
            MESSAGEW("state_name.empty() == true! You "
                "set an empty string");
        }

        this->m_state_name = state_name;
    }

    inline const xr_string& getSchemeName(void) const noexcept { return this->m_scheme_name; }
    inline void setSchemeName(const xr_string& scheme_name) noexcept
    {
        if (scheme_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSchemeName(scheme_name)] WARNING: scheme_name.empty() == true! "
                "You set an empty string!");
        }

        this->m_scheme_name = scheme_name;
    }



    inline const xr_string& getSoundName(void) const noexcept { return this->m_sound_name; }
    inline void setSoundName(const xr_string& data_name) noexcept
    {
        if (data_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSoundName(data_name)] WARNING: data_name.empty() == true! You set "
                "an empty string.");
        }

        this->m_sound_name = data_name;
    }

    inline const xr_string& getTipName(void) const noexcept { return this->m_tip_name; }
    inline void setTipName(const xr_string& message_name) noexcept
    {
        if (message_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setTipName(message_name)] WARNING: message_name.empty() == true! You "
                "set an empty string");
        }

        this->m_tip_name = message_name;
    }

    inline const xr_string& getTimeName(void) const noexcept { return this->m_time_name; }
    inline void setTimeName(const xr_string& data_name) noexcept
    {
        if (data_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setTimeName(data_name)] WARNING: data_name.empty() == true! You set "
                "an empty string");
        }

        this->m_time_name = data_name;
    }

    inline const xr_string& getAnimationName(void) const noexcept { return this->m_animation_name; }
    inline void setAnimationName(const xr_string& animation_name) noexcept
    {
        if (animation_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setAnimationName(animation_name)] WARNING: animation_name.empty() == "
                "true! You set an empty string");
        }

        this->m_animation_name = animation_name;
    }

    inline const xr_string& getAnimationHeadName(void) const noexcept { return this->m_animation_head_name; }
    inline void setAnimationHeadName(const xr_string& animation_head_name) noexcept
    {
        if (animation_head_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setAnimationHeadName(animation_head_name)] WARNING: "
                "animation_head_name.empty() == true! You set an empty string");
        }

        this->m_animation_head_name = animation_head_name;
    }

    inline const xr_string& getPathJumpName(void) const noexcept { return this->m_path_jump_name; }
    inline void setPathJumpName(const xr_string& path_name) noexcept
    {
        if (path_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPathJumpName(path_name)] WARNING: path_name.empty() == true! You "
                "set an empty string");
        }

        this->m_path_jump_name = path_name;
    }

    inline const xr_string& getHomeName(void) const noexcept { return this->m_home_name; }
    inline void setHomeName(const xr_string& home_name) noexcept
    {
        if (home_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setHomeNmae(home_name)] WARNING: home_name.empty() == true! You set "
                "an empty string");
        }

        this->m_home_name = home_name;
    }

    inline const CondlistWaypoints& getPathWalkInfo(void) const noexcept { return this->m_path_walk_info; }
    inline void setPathWalkInfo(const CondlistWaypoints& data) noexcept { this->m_path_walk_info = data; }

    inline const CondlistWaypoints& getPathLookInfo(void) const noexcept { return this->m_path_look_info; }
    inline void setPathLookInfo(const CondlistWaypoints& data) noexcept { this->m_path_look_info = data; }



    inline CScriptGameObject* const getClientObject(void) const { return this->m_p_npc; }
    inline void setClientObject(CScriptGameObject* const p_client_object) { this->m_p_npc = p_client_object; }

    inline CPatrolPathParams* const getJumpPath(void) const { return this->m_p_jump_path; }
    inline void setJumpPath(CPatrolPathParams* const p_path)
    {
        if (!p_path)
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setJumpPath(p_path)] WARNING: p_path == nullptr! Can't set because "
                "deleting is in dtor()!");
            return;
        }

        this->m_p_jump_path = p_path;
    }

    inline bool IsAnimationMovement(void) const noexcept { return this->m_is_animation_movement; }
    inline void setAnimationMovement(const bool value) noexcept { this->m_is_animation_movement = value; }

    inline bool IsNoReset(void) const noexcept { return this->m_is_no_reset; }
    inline void setNoReset(const bool value) noexcept { this->m_is_no_reset = value; }

    inline const xr_map<std::uint32_t, CondlistData>& getDialogCondlist(void) const noexcept
    {
        return this->m_dialog_condlist;
    }

    inline void setDialogCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
    {
        this->m_dialog_condlist = condlist;
    }

    inline const Fvector& getOffset(void) const noexcept { return this->m_offset; }
    inline void setOffset(const Fvector& data) noexcept { this->m_offset = data; }

    inline float getPHJumpFactor(void) const noexcept { return this->m_ph_jump_factor; }
    inline void setPHJumpFactor(const float value) noexcept { this->m_ph_jump_factor = value; }

    inline std::uint32_t getHomeMinRadius(void) const noexcept { return this->m_home_min_radius; }
    inline void setHomeMinRadius(const std::uint32_t value) noexcept { this->m_home_min_radius = value; }

    inline std::uint32_t getHomeMidRadius(void) const noexcept { return this->m_home_mid_radius; }
    inline void setHomeMidRadius(const std::uint32_t value) noexcept { this->m_home_mid_radius = value; }

    inline std::uint32_t getHomeMaxRadius(void) const noexcept { return this->m_home_max_radius; }
    inline void setHomeMaxRadius(const std::uint32_t value) noexcept { this->m_home_max_radius = value; }

    inline bool IsAggresive(void) const noexcept { return this->m_is_aggresive; }
    inline void setAggresive(const bool value) noexcept { this->m_is_aggresive = value; }

    inline bool IsGulagPoint(void) const noexcept { return this->m_is_gulag_point; }
    inline void setGulagPoint(const bool value) noexcept { this->m_is_gulag_point = value; }

    inline const xr_string& getLookPointName(void) const noexcept { return this->m_look_point_name; }
    inline void setLookPointName(const xr_string& point_name) noexcept
    {
        if (point_name.empty())
        {
            MESSAGE("point_name.empty() == true! "
                "You set an empty string");
        }

        this->m_look_point_name = point_name;
    }

    inline const xr_string& getHomePointName(void) const noexcept { return this->m_home_point_name; }
    inline void setHomePointName(const xr_string& home_name) noexcept
    {
        if (home_name.empty())
        {
            MESSAGEW("home_name.empty() == true! You "
                "set an empty string");
        }

        this->m_home_point_name = home_name;
    }

    inline std::uint32_t getTimeChangePoint(void) const noexcept { return this->m_time_change_point; }
    inline void setTimeChangePoint(const std::uint32_t value) noexcept { this->m_time_change_point = value; }

    inline bool IsSkipTransferEnemy(void) const noexcept { return this->m_is_skip_transfer_enemy; }
    inline void setSkipTransferEnemy(const bool value) noexcept { this->m_is_skip_transfer_enemy = value; }


    inline const xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>>& getHitOnBone(void) const noexcept
    {
        return this->m_hit_on_bone;
    }
    inline void setHitOnBone(const xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>>& data) noexcept
    {
        this->m_hit_on_bone = data;
    }
    inline const xr_map<std::uint32_t, CondlistData>& getOnUseCondlist(void) const noexcept
    {
        return this->m_on_use_condlist;
    }
    inline void setOnUseCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
    {
        this->m_on_use_condlist = condlist;
    }

    inline float getForce(void) const noexcept { return this->m_force; }
    inline void setForce(const float value) noexcept { this->m_force = value; }

    inline float getAngle(void) const noexcept { return this->m_angle; }
    inline void setAngle(const float value) noexcept { this->m_angle = value; }

    inline std::uint16_t getSelectedID(void) const noexcept { return this->m_selected_id; }
    inline void setSelectedID(const std::uint16_t value) noexcept { this->m_selected_id = value; }

    inline std::uint32_t getDangerTime(void) const noexcept { return this->m_danger_time; }
    inline void setDangerTime(const std::uint32_t value) noexcept { this->m_danger_time = value; }

    inline const xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>>& getApprovedActions(
        void) const noexcept
    {
        return this->m_approved_actions;
    }

    inline void setApprovedActions(const std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>& pair) noexcept
    {
        if (pair.first == nullptr)
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setApprovedActions(pair)] WARNING: can't add pair, because the first "
                "element (a function) is nullptr!");
            return;
        }

        this->m_approved_actions.push_back(pair);
    }

#pragma region Cordis Scheme PH FORCE

#pragma endregion

#pragma region Cordis Scheme PH Button
#pragma endregion

#pragma region Cordis Scheme PH Code
#pragma endregion

#pragma region Cordis Scheme PH Door
#pragma endregion

#pragma region Cordis Scheme PH Hit
#pragma endregion

#pragma region Cordis Scheme PH Idle
#pragma endregion

#pragma region Cordis Scheme PH Oscillate
#pragma endregion

#pragma region Cordis Scheme PH Sound
#pragma endregion

#pragma region Cordis Scheme SR Timer

#pragma endregion

#pragma region Cordis Scheme SR Teleport
#pragma endregion

#pragma region Cordis Scheme SR Psy Antenna
#pragma endregion

#pragma region Cordis Scheme SR Postprocess
#pragma endregion

#pragma region Cordis Scheme SR Particle
#pragma endregion

#pragma region Cordis Scheme SR Light
#pragma endregion

#pragma region Cordis Scheme SR Deimos
#pragma endregion

#pragma region Cordis Scheme XR Death

#pragma endregion

#pragma region Cordis Scheme XR Abuse

#pragma endregion

#pragma region Cordis Scheme XR Corpse Detection
    inline std::uint32_t getLevelVertexID(void) const noexcept { return this->m_level_vertex_id; }
    inline void setLevelVertexID(const std::uint32_t id) noexcept { this->m_level_vertex_id = id; }

    inline const Fvector& getVertexPosition(void) const noexcept { return this->m_vertex_position; }
    inline void setVertexPosition(const Fvector& position) noexcept { this->m_vertex_position = position; }
#pragma endregion

#pragma region Cordis Scheme XR Combat
#pragma endregion

#pragma region Cordis Scheme XR Help Wounded

#pragma endregion

#pragma region Cordis Scheme XR Gather Items
#pragma endregion

#pragma region Cordis Scheme XR Remark
#pragma endregion

#pragma region Cordis Scheme XR Walker

#pragma endregion

#pragma region Cordis Scheme XR Sleeper
#pragma endregion

#pragma region Cordis Scheme XR Kamp
#pragma endregion

#pragma region Cordis Scheme SR / XR Camp / Animpoint
    inline const xr_string& getBaseActionName(void) const noexcept { return this->m_base_action_name; }
    inline void setBaseActionName(const xr_string& action_name) noexcept
    {
        if (action_name.empty())
        {
            MESSAGEWR("action_name.empty() == true!");
            return;
        }

        this->m_base_action_name = action_name;
    }

    inline const xr_string& getDescriptionName(void) const noexcept { return this->m_description_name; }
    inline void setDescriptionName(const xr_string& description_name) noexcept
    {
        if (description_name.empty())
        {
            MESSAGEWR("description_name.empty() == true!");
            return;
        }

        this->m_description_name = description_name;
    }
#pragma endregion

#pragma region Cordis Scheme XR Camper
#pragma endregion

#pragma region Cordis Scheme XR Meet
#pragma endregion

#pragma region Cordis Scheme XR Wounded
#pragma endregion

#pragma region Cordis Scheme XR Combat Ignore
#pragma endregion

    inline void setAction(Script_ISchemeEntity* const p_action) { this->m_p_action = p_action; }
    inline Script_ISchemeEntity* getAction(void) const { return this->m_p_action; }

#pragma region Cordis Scheme XR Combat Zombied
#pragma endregion

#pragma region Cordis Scheme XR Combat Camper 
#pragma endregion

#pragma region Cordis Scheme XR Patrol 
#pragma endregion

#pragma region Cordis Scheme XR Animpoint
#pragma endregion

#pragma region Cordis Scheme XR Smart Cover
#pragma endregion

#pragma region Cordis Post Combat 
#pragma endregion

private:
    // @ Не понятно зачем в итоге но так у ПЫС, если в итоге оно находится в самом сторадже где уже зарегистрирован
    // сам НПС
    bool m_is_enabled = false;
    bool m_is_animation_movement = false;
    bool m_is_no_reset = false;
    bool m_is_aggresive = false;
    bool m_is_gulag_point = false;
    bool m_is_skip_transfer_enemy = false;
    std::uint16_t m_selected_id = 0;
    std::uint32_t m_home_min_radius = 0;
    std::uint32_t m_home_mid_radius = 0;
    std::uint32_t m_home_max_radius = 0;
    std::uint32_t m_time_change_point = 0;
 
    std::uint32_t m_level_vertex_id = 0;
    std::uint32_t m_danger_time = 0;
    float m_ph_jump_factor = 0.0f;

    float m_force = 0.0f;
    float m_angle = 0.0f;
    CScriptGameObject* m_p_npc = nullptr;
    /*
        Script_ISchemeEntity* m_p_action =
            nullptr; // @ для XR_LOGIC::unsubscrive_action, используется в очень редких схемах!*/
    CScriptIniFile* m_p_ini = nullptr;
    CPatrolPathParams* m_p_jump_path = nullptr;
    Script_ISchemeEntity* m_p_action = nullptr; // @ Здесь он не удаляется, пробрасываетс специально такой дизайн пыс (
    Fvector m_offset;
    Fvector m_vertex_position;
    xr_map<xr_string, bool> m_signals;
    xr_map<std::uint32_t, CondlistData> m_dialog_condlist;
    xr_map<std::uint32_t, CondlistData> m_on_use_condlist;
    xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>> m_hit_on_bone;
    xr_vector<Script_ISchemeEntity*> m_actions;
    xr_vector<LogicData> m_logic;
    xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>> m_approved_actions;
    xr_string m_path_walk_name;
    xr_string m_path_look_name;
    xr_string m_path_jump_name;
    xr_string m_state_name;
    xr_string m_scheme_name;
    xr_string m_logic_name;
    xr_string m_tip_name;
    xr_string m_sound_name;
    xr_string m_time_name;
    xr_string m_animation_name;
    xr_string m_animation_head_name;
    xr_string m_home_name;
    xr_string m_look_point_name;
    xr_string m_home_point_name;
    xr_string m_base_action_name;
    xr_string m_description_name;

    CondlistWaypoints m_path_walk_info;
    CondlistWaypoints m_path_look_info;
};

// Lord: не забудь что здесь ещё MANAGER для WOUNDED!!!!!!
class Data_Wounded
{
public:
    Data_Wounded(void) = default;
    ~Data_Wounded(void) = default;

    inline bool IsAutoHeal(void) const noexcept { return this->m_is_autoheal; }
    inline void setAutoHeal(const bool value) noexcept { this->m_is_autoheal = value; }

    inline bool IsUseMedkit(void) const noexcept { return this->m_is_use_medkit; }
    inline void setUseMedkit(const bool value) noexcept { this->m_is_use_medkit = value; }

    inline bool IsEnableTalk(void) const noexcept { return this->m_is_enable_talk; }
    inline void setEnableTalk(const bool value) noexcept { this->m_is_enable_talk = value; }

    inline bool IsForHelp(void) const noexcept { return this->m_is_not_for_help; }
    inline void setForHelp(const bool value) noexcept { this->m_is_not_for_help = value; }

    inline bool IsWoundedInitialized(void) const noexcept { return this->m_is_wounded_initialized; }
    inline void setWoundedInitialized(const bool value) noexcept { this->m_is_wounded_initialized = value; }

    inline const xr_string& getHPStateName(void) const noexcept { return this->m_hp_state_name; }
    inline void setHPStateName(const xr_string& state_name) noexcept
    {
        if (state_name.empty())
        {
            MESSAGEW("state_name.empty() == true! Trying to set an empty string!");
        }

        this->m_hp_state_name = state_name;
    }

    inline const xr_string& getHPStateSeeName(void) const noexcept { return this->m_hp_state_see_name; }
    inline void setHPStateSeeName(const xr_string& state_see_name) noexcept
    {
        if (state_see_name.empty())
        {
            MESSAGEW("state_see_name.empty() == true! Trying to set an empty string!");
        }

        this->m_hp_state_see_name = state_see_name;
    }

    inline const xr_string& getHPVictimName(void) const noexcept { return this->m_hp_victim_name; }
    inline void setHPVictimName(const xr_string& victim_name) noexcept { this->m_hp_victim_name = victim_name; }

    inline const xr_string& getHPCoverName(void) const noexcept { return this->m_hp_cover_name; }
    inline void setHPCoverName(const xr_string& cover_name) noexcept
    {
        if (cover_name.empty())
        {
            MESSAGEW("cover_name.empty() == true! Trying to set an empty string!");
        }

        this->m_hp_cover_name = cover_name;
    }

    inline const xr_string& getHPFightName(void) const noexcept { return this->m_hp_fight_name; }
    inline void setHPFightName(const xr_string& fight_name) noexcept
    {
        if (fight_name.empty())
        {
            MESSAGEW("fight_name.empty() == true! Trying to set an empty string!");
        }

        this->m_hp_fight_name = fight_name;
    }

    inline const xr_string& getPsyStateName(void) const noexcept { return this->m_psy_state_name; }
    inline void setPsyStateName(const xr_string& psy_state_name) noexcept
    {
        if (psy_state_name.empty())
        {
            MESSAGEW("psy_state_name.empty() == true! Trying to set an empty string!");
        }

        this->m_psy_state_name = psy_state_name;
    }

    inline const xr_string& getSynDataName(void) const noexcept { return this->m_syndata_name; }
    inline void setSynDataName(const xr_string& syndata_name) noexcept
    {
        if (syndata_name.empty())
        {
            MESSAGEW("syndata_name.empty() == true! Trying to set an empty string!");
        }

        this->m_syndata_name = syndata_name;
    }

    inline const xr_string& getHelpDialogName(void) const noexcept { return this->m_help_dialog_name; }
    inline void setHelpDialogName(const xr_string& dialog_name) noexcept
    {
        if (dialog_name.empty())
        {
            MESSAGEW("dialog_name.empty() == true! Trying to set an empty string!");
        }

        this->m_help_dialog_name = dialog_name;
    }

    inline const xr_string& getHelpStartDialogName(void) const noexcept { return this->m_help_start_dialog_name; }
    inline void setHelpStartDialogName(const xr_string& dialog_name) noexcept
    {
        if (dialog_name.empty())
        {
            MESSAGEW("dialog_name.empty() == true! Trying to set an empty string!");
        }

        this->m_help_start_dialog_name = dialog_name;
    }

    inline const xr_string& getWoundedSectionName(void) const noexcept { return this->m_wounded_section_name; }
    inline void setWoundedSectionName(const xr_string& section_name) noexcept
    {
        if (section_name.empty())
        {
            MESSAGEW("section_name.empty() == true! Trying to set an empty string!");
        }

        this->m_wounded_section_name = section_name;
    }

private:
    bool m_is_autoheal;
    bool m_is_use_medkit;
    bool m_is_enable_talk;
    bool m_is_not_for_help;
    bool m_is_wounded_initialized;
    xr_string m_hp_state_name;
    xr_string m_hp_state_see_name;
    xr_string m_hp_victim_name;
    xr_string m_hp_cover_name;
    xr_string m_hp_fight_name;
    xr_string m_psy_state_name;
    xr_string m_syndata_name;
    xr_string m_help_dialog_name;
    xr_string m_help_start_dialog_name;
    xr_string m_wounded_section_name;
};

class PStor_Data
{
    friend class Storage;
    char m_boolean =
        Globals::kPstorBooleanUndefined; // Globals::kPstorBooleanFalse -> False, Globals::kPstorBooleanUndefined -> Not
                                         // initialized, Globals::kPstorBooleanTrue -> True
    float m_number = 0.0f;
    xr_string m_string = Globals::kStringUndefined;

public:
    inline bool IsInitializedBool(void) const noexcept { return (this->m_boolean != Globals::kPstorBooleanUndefined); }
    inline bool IsInitializedNumber(void) const noexcept { return (!fis_zero(this->m_number)); }
    inline bool IsInitializedString(void) const noexcept { return (this->m_string != Globals::kStringUndefined); }
    inline bool IsInitializedSomething(void) const noexcept
    {
        return (this->IsInitializedBool() || this->IsInitializedNumber() || this->IsInitializedString());
    }
    inline bool getBool(void) const noexcept
    {
        switch (this->m_boolean)
        {
        case '-': {
            MESSAGE("Returns an initialized value -> False");
            break;
        }
        case '0': {
            MESSAGE("the m_boolean doesn't initialized!");
            break;
        }
        case '+': {
            MESSAGE("Returns an initialized value -> True");
            return true;
            break;
        }
        };

        return false;
    }

    inline float getNumber(void) const noexcept
    {
        MESSAGEI("Returns value [%d], because it was initialized!",
            this->m_number);
        return this->m_number;
    }

    inline xr_string getString(void) const noexcept
    {
        if (this->m_string == Globals::kStringUndefined)
        {
            MESSAGEWR("the m_string doesn't initialized. Returns an empty string.");
            return xr_string();
        }

        MESSAGEI("Returns value [%s], it's initialized!", this->m_string.c_str());
        return this->m_string;
    }

    inline void setBool(const bool value) noexcept
    {
        if (this->m_number != Globals::kUnsignedInt8Undefined || this->m_string != Globals::kStringUndefined)
        {
            MESSAGEWR("You can't assigned a value to m_boolean, because some "
                "member of structure was initialized!");
            return;
        }

        if (value)
        {
            MESSAGEI("-> True");
            this->m_boolean = Globals::kPstorBooleanTrue;
        }
        else
        {
            MESSAGEI("-> False");
            this->m_boolean = Globals::kPstorBooleanFalse;
        }
    }

    inline void setNumber(const float value) noexcept
    {
        if (this->m_boolean != Globals::kPstorBooleanUndefined || this->m_string != Globals::kStringUndefined)
        {
            MESSAGEWR("You can't assigned a value to m_number, because some "
                "member of structure was initialized!");
            return;
        }

        MESSAGEI("-> %f", value);
        this->m_number = value;
    }

    inline void setString(const xr_string& string) noexcept
    {
        if (this->m_boolean != Globals::kPstorBooleanUndefined || this->m_number != Globals::kUnsignedInt8Undefined)
        {
            MESSAGEWR("You can't assigned a value to m_string, because some "
                "member of structure was initialized!");
            return;
        }

        MESSAGEI("-> %s", string.c_str());
        this->m_string = string;
    }
};
// Lord: удалить SubStorage
/*
struct SubStorage_Data
{
private:
    friend class Storage;

public:
    SubStorage_Data(void) = default;
    ~SubStorage_Data(void) = default;

    inline void setSignal(const xr_map<xr_string, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/SubStorage_Data/setSignal(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map! No assignment!");
            return;
        }

        this->m_signals = map;
    }

    inline void setSignal(const std::pair<xr_string, bool>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("");
            return;
        }
    }
    inline void setSignal(const xr_string& id_name, const bool& value) noexcept
    {
        if (!id_name.size())
        {
            Msg("[DataBase/SubStorage_Data/setSignal(id_name, value)] WARNING: id_name.size() = 0! You are trying to "
                "set an empty string! No assignment!");
            return;
        }

        this->m_signals[id_name] = value;
    }

/ *
    inline void setAction(const xr_vector<Script_ILogicEntity*>& vector)
    {
        if (!vector.size())
        {
            Msg("[DataBase/SubStorage_Data/setAction(vector)] WARNING: vector.size() = 0! You are trying to set an "
                "empty vector! No assignment!");
            return;
        }

        this->m_actions = vector;
    }

    inline void setAction(Script_ILogicEntity* entity)
    {
        if (!entity)
        {
            Msg("[DataBase/SubStorage_Data/setAction(p_entity)] WARNING: p_entity = null! You are trying to set an "
                "empty object! No assignment!");
            return;
        }

        this->m_actions.push_back(entity);
    }* /

    inline const xr_map<xr_string, bool>& getSignals(void) const noexcept { return this->m_signals; }
/ *    inline const xr_vector<Script_ILogicEntity*>& getActions(void) const noexcept { return this->m_actions; }* /
    inline void ClearActions(void)
    {
        Msg("[DataBase/SubStorage_Data/ClearActions()] this->m_actions.clear() is called!");
        this->m_actions.clear();
    }
    inline void ClearSignals(void)
    {
        Msg("[DataBase/SubStorage_Data/ClearSignals()] this->m_signals");
        this->m_signals.clear();
    }

private:
    xr_map<xr_string, bool> m_signals;
/ *    xr_vector<Script_ILogicEntity*> m_actions;* /
};*/
// сделать private, public!
struct StorageAnimpoint_Data
{
private:
    friend class Storage;

public:
    inline void setCoverName(const xr_string& string) { this->m_cover_name = string; }
    inline bool setUseCamp(const bool& value) { this->m_is_use_camp = value; }
    inline void setAnimpoint(Script_Animpoint* object)
    {
        if (!object)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        this->m_animpoint = object;
    }
    inline void setReachedDistance(const float& value) { this->m_reached_distance = value; }
    inline void setAvailAnimation(const xr_string& animation_name)
    {
        if (!animation_name.size())
        {
            R_ASSERT2(false, "can't be empty!");
            return;
        }

        this->m_avail_animations.push_back(animation_name);
    }
    inline void setApprovedAction(const std::function<bool(std::uint16_t, bool)> function, const xr_string& action_name)
    {
        if (!action_name.size())
        {
            R_ASSERT2(false, "can't be empty!");
            return;
        }

        if (!function)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        std::pair<std::function<bool(std::uint16_t, bool)>, xr_string> insert_pair;

        insert_pair.first = function;
        insert_pair.second = action_name;
        this->m_approved_actions.push_back(insert_pair);
    }

    inline void setDescriptionName(const xr_string& description_name) noexcept
    {
        if (!description_name.size())
        {
            MESSAGEW("you are trying to set an empty string!");
        }

        this->m_description_name = description_name;
    }

    inline float getReachedDistance(void) const noexcept { return this->m_reached_distance; }
    inline xr_string getCoverName(void) const noexcept { return this->m_cover_name; }
    inline xr_string getDescriptionName(void) const noexcept { return this->m_description_name; }
    inline Script_Animpoint* getAnimpoint(void) const noexcept { return this->m_animpoint; }
    inline const xr_vector<xr_string>& getAvailAnimations(void) const noexcept { return this->m_avail_animations; }
    inline const xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>>& getApprovedActions(
        void) const noexcept
    {
        return this->m_approved_actions;
    }
    inline bool IsUseCamp(void) const noexcept { return this->m_is_use_camp; }

private:
    bool m_is_use_camp;
    float m_reached_distance;
    xr_vector<xr_string> m_avail_animations;
    xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>> m_approved_actions;
    xr_string m_cover_name;
    xr_string m_description_name;
    Script_Animpoint* m_animpoint =
        nullptr; // Lord: узнать где выделяется и удаляется данный класс (xr_animpoint.script)
};

struct Script_IComponentScheme
{
	virtual ~Script_IComponentScheme(void) {}

	inline const xr_vector<Script_ISchemeEntity*>& getActions(void) const noexcept { return this->m_actions; }
	inline void addAction(Script_ISchemeEntity* const p_data)
	{
		if (p_data == nullptr)
		{
			MESSAGEWR("You can't add a nullptr object!");
			return;
		}

		this->m_actions.push_back(p_data);
	}

	inline const xr_vector<LogicData>& getLogic(void) const noexcept { return this->m_logic; }
	inline void setLogic(const xr_vector<LogicData>& data) noexcept { this->m_logic = data; }

	inline CScriptIniFile* const getIni(void) const { return this->m_p_ini; }
	inline void setIni(CScriptIniFile* const p_ini) { this->m_p_ini = p_ini; }

	inline const xr_map<xr_string, bool>& getSignals(void) const noexcept { return this->m_signals; }

	inline void setSignals(const xr_map<xr_string, bool>& map) noexcept
	{
		if (map.empty())
		{
			MESSAGEWR("map.empty() == true! Can't assign an empty map return ...");
			return;
		}

		this->m_signals = map;
	}

	inline void setSignals(const std::pair<xr_string, bool>& pair) noexcept
	{
		if (pair.first.empty())
		{
			MESSAGEWR("pair.first.empty() == true! Can't assign "
				"an empty string return ...");
			return;
		}

		this->m_signals.insert(pair);
	}

	inline void setSignals(const xr_string& signal_name, const bool value) noexcept
	{
		if (signal_name.empty())
		{
			MESSAGEWR("signal_name.empty() == "
				"true! Can't assign an empty string return ...");
			return;
		}

		this->m_signals[signal_name] = value;
	}

	inline void ClearSignals(void) noexcept
	{
		MESSAGEI("signals are cleared!");
		this->m_signals.clear();
	}

	inline const xr_string& getLogicName(void) const noexcept { return this->m_logic_name; }
	inline void setLogicName(const xr_string& section_name) noexcept
	{
		if (section_name.empty())
		{
			MESSAGEW("section_name.empty() == "
				"true! You set an empty string.");
		}

		this->m_logic_name = section_name;
	}

	inline bool IsEnabled(void) const noexcept { return this->m_is_enabled; }
	inline void setEnabled(const bool value) noexcept { this->m_is_enabled = value; }

	inline void setActionSchemeID(const std::uint32_t scheme_id) noexcept
	{
		if (scheme_id == Globals::kUnsignedInt32Undefined)
		{
			MESSAGEWR("something not right. overflow sheat ...");
		}

		this->m_scheme_id_for_unsubscring = scheme_id;
	}

	inline void UnSubscribeAction(void) noexcept
	{
		if (!this->m_scheme_id_for_unsubscring || this->m_scheme_id_for_unsubscring == Globals::kUnsignedInt32Undefined)
		{
			MESSAGEWR("can't unsubscribe action, which not registered with ID ... YOUR ID is [%d]", this->m_scheme_id_for_unsubscring);
			return;
		}

		for (Script_ISchemeEntity* it : this->m_actions)
		{
			if (it)
			{
				if (it->getSchemeID() == this->m_scheme_id_for_unsubscring)
				{
					MESSAGEI("unsubscribe action for npc %s %d",
						it->getSchemeName().c_str(), it->getID());
					it->unsubscribe_action();
					return;
				}
			}
		}

		MESSAGEW("can't find action for unsubscribing YOUR "
			"ACTION ID -> %d",
			this->m_scheme_id_for_unsubscring);
	}

	inline CScriptGameObject* const getClientObject(void) const { return this->m_p_npc; }
	inline void setClientObject(CScriptGameObject* const p_client_object) { this->m_p_npc = p_client_object; }

	inline const xr_string& getSchemeName(void) const noexcept { return this->m_scheme_name; }
	inline void setSchemeName(const xr_string& scheme_name) noexcept
	{
		if (scheme_name.empty())
		{
			MESSAGEW("scheme_name.empty() == true! You set an empty string!");
		}

		this->m_scheme_name = scheme_name;
	}

protected:
    bool m_is_enabled = false;
    std::uint32_t m_scheme_id_for_unsubscring = 0;
	CScriptIniFile* m_p_ini = nullptr;
    CScriptGameObject* m_p_npc = nullptr;
	xr_map<xr_string, bool> m_signals;
	xr_vector<Script_ISchemeEntity*> m_actions;
	xr_vector<LogicData> m_logic;
	xr_string m_logic_name;
    xr_string m_scheme_name;
};

struct Script_ComponentScheme_PHButton : public Script_IComponentScheme
{
	Script_ComponentScheme_PHButton(void) : m_is_blending(false) {}

	inline const xr_string& getAnimationName(void) const noexcept { return this->m_animation_name; }
	inline void setAnimationName(const xr_string& animation_name) noexcept
	{
		if (animation_name.empty())
		{
			MESSAGEW("animation_name.empty() == true! You set an empty string");
		}
		this->m_animation_name = animation_name;
	}

	inline const xr_string& getToolTipName(void) const noexcept { return this->m_tooptip_name; }
	inline void setToolTipName(const xr_string& value_name) noexcept
	{
		if (value_name.empty())
		{
			MESSAGEW("value_name.empty() == "
				"true! You set an empty string");
		}

		this->m_tooptip_name = value_name;
	}

	inline bool IsBlending(void) const noexcept { return this->m_is_blending; }
	inline void setBlending(const bool value) noexcept { this->m_is_blending = value; }

	inline const xr_map<std::uint32_t, CondlistData>& getOnPressCondlist(void) const noexcept
	{
		return this->m_on_press_condlist;
	}

	inline void setOnPressCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
	{
		this->m_on_press_condlist = condlist;
	}

	inline void clear(void) noexcept
	{
		this->m_is_blending = false;
		this->m_on_press_condlist.clear();
		this->m_animation_name.clear();
		this->m_tooptip_name.clear();
	}

private:
	bool m_is_blending;
	xr_map<std::uint32_t, CondlistData> m_on_press_condlist;
	xr_string m_animation_name;
	xr_string m_tooptip_name;
};

struct Script_ComponentScheme_MobCamp : public Script_IComponentScheme
{
    Script_ComponentScheme_MobCamp(void) : m_is_skip_transfer_enemy(false), m_time_change_point(0), m_home_min_radius(0), m_home_mid_radius(0), m_home_max_radius(0) {}

	inline const xr_string& getLookPointName(void) const noexcept { return this->m_look_point_name; }
	inline void setLookPointName(const xr_string& point_name) noexcept
	{
		if (point_name.empty())
		{
			MESSAGE("point_name.empty() == true! You set an empty string");
		}

		this->m_look_point_name = point_name;
	}

	inline const xr_string& getHomePointName(void) const noexcept { return this->m_home_point_name; }
	inline void setHomePointName(const xr_string& home_name) noexcept
	{
		if (home_name.empty())
		{
			MESSAGEW("home_name.empty() == true! You set an empty string");
		}

		this->m_home_point_name = home_name;
	}

    inline void clear(void) noexcept 
    {
        this->m_is_skip_transfer_enemy = false;
        this->m_time_change_point = 0;
        this->m_home_max_radius = 0;
        this->m_home_mid_radius = 0;
        this->m_home_min_radius = 0;
        this->m_look_point_name.clear();
        this->m_home_point_name.clear();
        this->m_state_name.clear();
    }

	inline bool IsSkipTransferEnemy(void) const noexcept { return this->m_is_skip_transfer_enemy; }
	inline void setSkipTransferEnemy(const bool value) noexcept { this->m_is_skip_transfer_enemy = value; }

	inline std::uint32_t getTimeChangePoint(void) const noexcept { return this->m_time_change_point; }
	inline void setTimeChangePoint(const std::uint32_t value) noexcept { this->m_time_change_point = value; }

	inline const xr_string& getStateName(void) const noexcept { return this->m_state_name; }
	inline void setStateName(const xr_string& state_name) noexcept
	{
		if (state_name.empty())
		{
			MESSAGEW("state_name.empty() == true! You "
				"set an empty string");
		}

		this->m_state_name = state_name;
	}

	inline std::uint32_t getHomeMinRadius(void) const noexcept { return this->m_home_min_radius; }
	inline void setHomeMinRadius(const std::uint32_t value) noexcept { this->m_home_min_radius = value; }

	inline std::uint32_t getHomeMidRadius(void) const noexcept { return this->m_home_mid_radius; }
	inline void setHomeMidRadius(const std::uint32_t value) noexcept { this->m_home_mid_radius = value; }

	inline std::uint32_t getHomeMaxRadius(void) const noexcept { return this->m_home_max_radius; }
	inline void setHomeMaxRadius(const std::uint32_t value) noexcept { this->m_home_max_radius = value; }

private:
    bool m_is_skip_transfer_enemy;
    std::uint32_t m_time_change_point;
    std::uint32_t m_home_min_radius;
    std::uint32_t m_home_mid_radius;
    std::uint32_t m_home_max_radius;
    xr_string m_look_point_name;
    xr_string m_home_point_name;
    xr_string m_state_name;
};

struct Script_ComponentScheme_MobDeath : public Script_IComponentScheme
{
    Script_ComponentScheme_MobDeath(void) {}
};

struct Script_ComponentScheme_MobCombat : public Script_IComponentScheme
{
    Script_ComponentScheme_MobCombat(void) {}
};

struct Script_ComponentScheme_XRSmartCover : public Script_IComponentScheme
{
	Script_ComponentScheme_XRSmartCover(void) : m_is_precalc_cover(false), m_is_in_combat(false), m_idle_max_time(0), m_idle_min_time(0), m_lookout_min_time(0), m_lookout_max_time(0) {}

	inline void clear(void) noexcept
	{
		this->m_is_in_combat = false;
		this->m_is_precalc_cover = false;
		this->m_idle_max_time = 0;
		this->m_idle_min_time = 0;
		this->m_lookout_max_time = 0;
		this->m_lookout_min_time = 0;
		this->m_cover_name.clear();
		this->m_loophole_name.clear();
		this->m_cover_state_name.clear();
		this->m_target_enemy_name.clear();
		this->m_target_path_name.clear();
		this->m_exit_body_state_name.clear();
		this->m_weapon_type_name.clear();
		this->m_moving_name.clear();
		this->m_sound_idle_name.clear();
	}

	inline const xr_string& getXRSmartCoverCoverName(void) const noexcept { return this->m_cover_name; }
	inline void setXRSmartCoverCoverName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEW("setting empty string!"); } this->m_cover_name = name; }

	inline const xr_string& getXRSmartCoverLoopholeName(void) const noexcept { return this->m_loophole_name; }
	inline void setXRSmartCoverLoopholeName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEW("setting empty string!"); } this->m_loophole_name = name; }

	inline const xr_string& getXRSmartCoverCoverStateName(void) const noexcept { return this->m_cover_state_name; }
	inline void setXRSmartCoverCoverStateName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("invalid string!"); return; } this->m_cover_state_name = name; }

	inline const xr_string& getXRSmartCoverTargetEnemyName(void) const noexcept { return this->m_target_enemy_name; }
	inline void setXRSmartCoverTargetEnemyName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("invalid string!"); return; } this->m_target_enemy_name = name; }

	inline const xr_string& getXRSmartCoverTargetPathName(void) const noexcept { return this->m_target_path_name; }
	inline void setXRSmartCoverTargetPathName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("invalid string!"); return; } this->m_target_path_name = name; }

	inline const xr_string& getXRSmartCoverExitBodyStateName(void) const noexcept { return this->m_exit_body_state_name; }
	inline void setXRSmartCoverExitBodyStateName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("invalid string!"); return; } this->m_exit_body_state_name = name; }

	inline const xr_string& getXRSmartCoverWeaponTypeName(void) const noexcept { return this->m_weapon_type_name; }
	inline void setXRSmartCoverWeaponTypeName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("invalid string!"); return; } this->m_weapon_type_name = name; }

	inline const xr_string& getXRSmartCoverMovingName(void) const noexcept { return this->m_moving_name; }
	inline void setXRSmartCoverMovingName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("invalid string!"); return; } this->m_moving_name = name; }

	inline const xr_string& getXRSmartCoverSoundIdleName(void) const noexcept { return this->m_sound_idle_name; }
	inline void setXRSmartCoverSoundIdleName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("invalid string!"); return; } this->m_sound_idle_name; }

	inline bool isXRSmartCoverUsePreCalcCover(void) const noexcept { return this->m_is_precalc_cover; }
	inline void setXRSmartCoverUsePreCalcCover(const bool value) noexcept { this->m_is_precalc_cover = value; }

	inline bool isXRSmartCoverUseInCombat(void) const noexcept { return this->m_is_in_combat; }
	inline void setXRSmartCoverUseInCombat(const bool value) noexcept { this->m_is_in_combat = value; }

	inline std::uint32_t getXRSmartCoverIdleMinTime(void) const noexcept { return this->m_idle_min_time; }
	inline void setXRSmartCoverIdleMinTime(const std::uint32_t value) noexcept { this->m_idle_min_time = value; }

	inline std::uint32_t getXRSmartCoverIdleMaxTime(void) const noexcept { return this->m_idle_max_time; }
	inline void setXRSmartCoverIdleMaxTime(const std::uint32_t value) noexcept { this->m_idle_max_time = value; }

	inline float getXRSmartCoverLookOutMinTime(void) const noexcept { return this->m_lookout_min_time; }
	inline void setXRSmartCoverLookOutMinTime(const float value) noexcept { this->m_lookout_min_time = value; }

	inline float getXRSmartCoverLookOutMaxTime(void) const noexcept { return this->m_lookout_max_time; }
	inline void setXRSmartCoverLookOutMaxTime(const float value) noexcept { this->m_lookout_max_time = value; }

private:
	bool m_is_precalc_cover;
	bool m_is_in_combat;
	std::uint32_t m_idle_min_time;
	std::uint32_t m_idle_max_time;
	float m_lookout_min_time;
	float m_lookout_max_time;
	xr_string m_cover_name;
	xr_string m_loophole_name;
	xr_string m_cover_state_name;
	xr_string m_target_enemy_name;
	xr_string m_target_path_name;
	xr_string m_exit_body_state_name;
	xr_string m_weapon_type_name;
	xr_string m_moving_name;
	xr_string m_sound_idle_name;
};
struct Script_ComponentScheme_PostCombat : public Script_IComponentScheme
{
	Script_ComponentScheme_PostCombat(void) : m_last_best_enemy_id(0), m_timer(0), m_p_animation(nullptr) {}
	~Script_ComponentScheme_PostCombat(void)
	{
		if (this->m_p_animation)
		{
			MESSAGEI("deleting post_combat data");
			xr_delete(this->m_p_animation);
		}
	}

	inline std::uint16_t getLastBestEnemyID(void) const noexcept { return this->m_last_best_enemy_id; }
	inline void setLastBestEnemyID(const std::uint16_t id) noexcept { this->m_last_best_enemy_id = id; }

	inline std::uint32_t getTimer(void) const noexcept { return this->m_timer; }
	inline void setTimer(const std::uint32_t value) noexcept { this->m_timer = value; }

	inline const xr_string& getLastBestEnemyName(void) const noexcept { return this->m_last_best_enemy_name; }
	inline void setLastBestEnemyName(const xr_string& name) noexcept { this->m_last_best_enemy_name = name; }

	inline Script_StateAnimation* getAnimation(void) const { return this->m_p_animation; }
	inline void setAnimation(Script_StateAnimation* const p_data)
	{
		this->m_p_animation = p_data;
	}

	inline void clear(void)
	{
		this->m_last_best_enemy_id = 0;
		this->m_timer = 0;
		this->m_last_best_enemy_name.clear();

		if (this->m_p_animation)
		{
			MESSAGEI("deleting data!");
			xr_delete(this->m_p_animation);
		}
	}

private:
	std::uint16_t m_last_best_enemy_id;
	std::uint32_t m_timer;
	Script_StateAnimation* m_p_animation;
	xr_string m_last_best_enemy_name;
};
struct Script_ComponentScheme_XRAnimPoint : public Script_IComponentScheme
{
	Script_ComponentScheme_XRAnimPoint(void) : m_is_use_camp(false), m_reach_distance(0.0f), m_p_animpoint(nullptr) {}
	~Script_ComponentScheme_XRAnimPoint(void)
	{
		if (this->m_p_animpoint)
		{
			MESSAGEI("deleting animpoint");
			xr_delete(this->m_p_animpoint);
		}
	}

	inline const xr_string& getCoverName(void) const noexcept { return this->m_cover_name; }
	inline void setCoverName(const xr_string& name) noexcept
	{
		if (name.empty())
		{
			MESSAGEWR("You are trying to set an empty string!");
			return;
		}

		this->m_cover_name = name;
	}

	inline const xr_vector<xr_string>& getAvailAnimations(void) const noexcept {
		return this->m_avail_animations;
	}

	inline void setAvailAnimations(const xr_vector<xr_string>& data) noexcept
	{
		if (data.empty())
		{
			MESSAGEWR("You are trying to set an empty vector!");
			return;
		}

		this->m_avail_animations = data;
	}

	inline bool isUseCamp(void) const noexcept { return this->m_is_use_camp; }
	inline void setUseCamp(const bool value) noexcept { this->m_is_use_camp = value; }

	inline const xr_string& getReachMovementName(void) const noexcept { return this->m_reach_movement_name; }
	inline void setReachMovementName(const xr_string& name) noexcept {
		if (name.empty())
		{
			MESSAGEWR("You are trying to set an empty string!");
			return;
		}

		this->m_reach_movement_name = name;
	}

	inline float getReachDistance(void) const noexcept { return this->m_reach_distance; }
	inline void setReachDistance(const float value) noexcept { this->m_reach_distance = value; }

	inline const xr_string& getDescriptionName(void) const noexcept { return this->m_description_name; }
	inline void setDescriptionName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("you are trying to set an empty string!"); return; }  this->m_description_name = name; }

	inline const xr_string& getBaseActionName(void) const noexcept
	{
		return this->m_base_action_name;
	}

	inline void setBaseActionName(const xr_string& name) noexcept
	{
		if (name.empty())
		{
			MESSAGEWR("You are trying to set an empty string!");
			return;
		}

		this->m_base_action_name = name;
	}

	inline Script_Animpoint* getAnimpoint(void) const { return this->m_p_animpoint; }
	inline void setAnimpoint(Script_Animpoint* const p_animpoint)
	{
		if (p_animpoint == nullptr)
		{
			MESSAGEW("set an empty object!");
		}

		this->m_p_animpoint = p_animpoint;
	}

	inline void clear(void)
	{
		this->m_is_use_camp = false;
		this->m_avail_animations.clear();
		this->m_base_action_name.clear();
		this->m_cover_name.clear();
		this->m_reach_distance = 0.0f;
		this->m_description_name.clear();
		this->m_reach_movement_name.clear();

		if (this->m_p_animpoint)
		{
			MESSAGEI("deleting data!");
			xr_delete(this->m_p_animpoint);
		}
	}


private:
	bool m_is_use_camp;
	float m_reach_distance;
	Script_Animpoint* m_p_animpoint;
	xr_vector<xr_string> m_avail_animations;
	xr_string m_cover_name;
	xr_string m_reach_movement_name;
	xr_string m_description_name;
	xr_string m_base_action_name;
};
struct Script_ComponentScheme_XRPatrol : public Script_IComponentScheme
{
	Script_ComponentScheme_XRPatrol(void) : m_is_silent(false), m_is_commander(false) {}

	inline bool isCommander(void) const noexcept { return this->m_is_commander; }
	inline void setCommander(const bool value) noexcept { this->m_is_commander = value; }

	inline bool isSilent(void) const noexcept { return this->m_is_silent; }
	inline void setSilent(const bool value) noexcept { this->m_is_silent = value; }

	inline const xr_string& getPathName(void) const noexcept { return this->m_path_name; }
	inline void setPathName(const xr_string& path_name) noexcept { if (path_name.empty()) { MESSAGEWR("string is empty!"); return; } this->m_path_name = path_name; }

	inline const xr_string& getPatrolKeyName(void) const noexcept { return this->m_patrol_key_name; }
	inline void setPatrolKeyName(const xr_string& key_name) noexcept { if (key_name.empty()) { MESSAGEWR("string is empty!"); return; } this->m_patrol_key_name = key_name; }

	inline const xr_string& getFormationName(void) const noexcept { return this->m_formation_name; }
	inline void setFormationName(const xr_string& formation_name) noexcept { if (formation_name.empty()) { MESSAGEWR("string is empty!"); return; } this->m_formation_name = formation_name; }

	inline const xr_string& getMoveTypeName(void) const noexcept { return this->m_move_type_name; }
	inline void setMoveTypeName(const xr_string& type_name) noexcept { if (type_name.empty()) { MESSAGEWR("string is empty!"); return; } this->m_move_type_name = type_name; }

	inline const xr_map<xr_string, xr_string>& getSuggestedStates(void) const noexcept { return this->m_suggested_states; }
	inline void setSuggestedStates(const xr_string& state_name, const xr_string& value_name) noexcept
	{
		if (state_name.empty())
		{
			MESSAGEWR("can't be empty!");
			return;
		}

		this->m_suggested_states[state_name] = value_name;
	}

	inline void clear(void) noexcept
	{
		this->m_is_commander = false;
		this->m_is_silent = false;
		this->m_suggested_states.clear();
		this->m_path_name.clear();
		this->m_patrol_key_name.clear();
		this->m_formation_name.clear();
		this->m_move_type_name.clear();
	}


private:
	bool m_is_silent;
	bool m_is_commander;
	xr_map<xr_string, xr_string> m_suggested_states;
	xr_string m_patrol_key_name;
	xr_string m_path_name;
	xr_string m_formation_name;
	xr_string m_move_type_name;
};
struct Script_ComponentScheme_XRCombatCamper : public Script_IComponentScheme
{
	Script_ComponentScheme_XRCombatCamper(void) : m_is_camper_action(false) {}

	inline bool isAction(void) const noexcept { return this->m_is_camper_action; }
	inline void setAction(const bool value) noexcept { this->m_is_camper_action = value; }

	inline const Fvector& getLastSeenPosition(void) const noexcept { return this->m_camper_last_seen_position; }
	inline void setLastSeenPosition(const Fvector& position) noexcept { this->m_camper_last_seen_position = position; }

	inline void clear(void) noexcept
	{
		this->m_is_camper_action = false;
		this->m_camper_last_seen_position.x = 0.0f;
		this->m_camper_last_seen_position.y = 0.0f;
		this->m_camper_last_seen_position.z = 0.0f;
	}

private:
	bool m_is_camper_action;
	Fvector m_camper_last_seen_position;
};
struct Script_ComponentScheme_XRCombatZombied : public Script_IComponentScheme
{
	Script_ComponentScheme_XRCombatZombied(void) : m_zombied_current_action(0) {}

	inline std::uint32_t getCurrentAction(void) const noexcept { return this->m_zombied_current_action; }
	inline void setCurrentAction(const std::uint32_t action_id) noexcept { this->m_zombied_current_action = action_id; }

	inline void clear(void) noexcept { this->m_zombied_current_action = 0; }

private:
	std::uint32_t m_zombied_current_action;
};
struct Script_ComponentScheme_XRCombatIgnore : public Script_IComponentScheme
{
	Script_ComponentScheme_XRCombatIgnore(void) : m_is_ignore_enabled(false) {}

	inline bool isEnabled(void) const noexcept { return this->m_is_ignore_enabled; }
	inline void setEnabled(const bool value) noexcept { this->m_is_ignore_enabled = value; }

	inline void clear(void) noexcept { this->m_is_ignore_enabled = false; }
private:
	bool m_is_ignore_enabled;
};
struct Script_ComponentScheme_XRWounded : public Script_IComponentScheme
{
	Script_ComponentScheme_XRWounded(void) : m_is_xr_wounded_autoheal(false), m_is_xr_wounded_enable_talk(false), m_is_xr_wounded_not_for_help(false), m_is_xr_wounded_set(false), m_is_xr_wounded_use_medkit(false), m_p_wounded_manager(nullptr) {}
	~Script_ComponentScheme_XRWounded(void)
	{
		if (this->m_p_wounded_manager)
		{
			MESSAGEI("deleting wounded manager");
			xr_delete(this->m_p_wounded_manager);
		}
	}

	inline bool isXRWoundedSet(void) const noexcept { return this->m_is_xr_wounded_set; }
	inline void setXRWoundedSet(const bool value) noexcept { this->m_is_xr_wounded_set = value; }

	inline bool isXRWoundedUseMedkit(void) const noexcept { return this->m_is_xr_wounded_use_medkit; }
	inline void setXRWoundedUseMedkit(const bool value) noexcept { this->m_is_xr_wounded_use_medkit = value; }

	inline bool isXRWoundedAutoHeal(void) const noexcept { return this->m_is_xr_wounded_autoheal; }
	inline void setXRWoundedAutoHeal(const bool value) noexcept { this->m_is_xr_wounded_autoheal = value; }

	inline bool isXRWoundedEnableTalk(void) const noexcept { return this->m_is_xr_wounded_enable_talk; }
	inline void setXRWoundedEnableTalk(const bool value) noexcept { this->m_is_xr_wounded_enable_talk = value; }

	inline bool isXRWoundedNotForHelp(void) const noexcept { return this->m_is_xr_wounded_not_for_help; }
	inline void setXRWoundedNotForHelp(const bool value) noexcept { this->m_is_xr_wounded_not_for_help = value; }

	inline const xr_string& getXRWoundedHelpDialogName(void) const noexcept { return this->m_xr_wounded_help_dialog_name; }
	inline void setXRWoundedHelpDialogName(const xr_string& dialog_name) noexcept
	{
		if (dialog_name.empty())
		{
			MESSAGEW("you are trying to set an empty string!");
		}

		this->m_xr_wounded_help_dialog_name = dialog_name;
	}

	inline const xr_string& getXRWoundedHelpStartDialogName(void) const noexcept { return this->m_xr_wounded_help_start_dialog_name; }
	inline void setXRWoundedHelpStartDialogName(const xr_string& dialog_start_name) noexcept
	{
		if (dialog_start_name.empty())
		{
			MESSAGEW("you are trying to set an empty string!");
		}

		this->m_xr_wounded_help_start_dialog_name = dialog_start_name;
	}

	inline const xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& getXRWoundedHealthState(void) const noexcept { return this->m_xr_wounded_health_state; }
	inline void setXRWoundedHealthState(xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& data) noexcept
	{
		this->m_xr_wounded_health_state = data;
	}

	inline const xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& getXRWoundedHealthStateSee(void) const noexcept { return this->m_xr_wounded_health_state_see; }
	inline void setXRWoundedHealthStateSee(xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& data) noexcept
	{
		this->m_xr_wounded_health_state_see = data;
	}

	inline const xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& getXRWoundedPsyState(void) const noexcept { return this->m_xr_wounded_psy_state; }
	inline void setXRWoundedPsyState(xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& data) noexcept
	{
		this->m_xr_wounded_psy_state = data;
	}

	inline const xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& getXRWoundedHealthVictim(void) const noexcept { return this->m_xr_wounded_health_victim; }
	inline void setXRWoundedHealthVictim(xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& data) noexcept
	{
		this->m_xr_wounded_health_victim = data;
	}

	inline const xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& getXRWoundedHealthCover(void) const noexcept { return this->m_xr_wounded_health_cover; }
	inline void setXRWoundedHealthCover(xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& data) noexcept
	{
		this->m_xr_wounded_health_cover = data;
	}

	inline const xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& getXRWoundedHealthFight(void) const noexcept { return this->m_xr_wounded_health_fight; }
	inline void setXRWoundedHealthFight(xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& data) noexcept
	{
		this->m_xr_wounded_health_cover = data;
	}

	inline const xr_string& getXRWoundedWoundedSectionName(void) const noexcept { return this->m_xr_wounded_wounded_section_name; }
	inline void setXRWoundedWoundedSectionName(const xr_string& section_name) noexcept
	{
		if (section_name.empty())
		{
			MESSAGEW("you are trying to set an empty string!");
		}

		this->m_xr_wounded_wounded_section_name = section_name;
	}

	inline void clear(void) noexcept
	{
		this->m_is_xr_wounded_autoheal = false;
		this->m_is_xr_wounded_enable_talk = false;
		this->m_is_xr_wounded_not_for_help = false;
		this->m_is_xr_wounded_set = false;
		this->m_is_xr_wounded_use_medkit = false;

		this->m_xr_wounded_health_cover.clear();
		this->m_xr_wounded_health_fight.clear();
		this->m_xr_wounded_health_state.clear();
		this->m_xr_wounded_health_state_see.clear();
		this->m_xr_wounded_health_victim.clear();
		this->m_xr_wounded_psy_state.clear();

		this->m_xr_wounded_help_dialog_name.clear();
		this->m_xr_wounded_wounded_section_name.clear();
		this->m_xr_wounded_help_start_dialog_name.clear();

		if (this->m_p_wounded_manager)
		{
			MESSAGEI("deleting wounded manager");
			xr_delete(this->m_p_wounded_manager);
		}
	}

	inline Script_WoundedManager* getWoundedManager(void) const { return this->m_p_wounded_manager; }
	inline void setWoundedManager(Script_WoundedManager* const p_manager)
	{
		if (this->m_p_wounded_manager)
		{
			R_ASSERT2(false, "you can't set to existed instance you must deallocate this!");
			return;
		}

		if (!p_manager)
		{
			R_ASSERT2(false, "can't be you must allocate manager!");
			return;
		}

		this->m_p_wounded_manager = p_manager;
	}

private:
	bool m_is_xr_wounded_set;
	bool m_is_xr_wounded_use_medkit;
	bool m_is_xr_wounded_autoheal;
	bool m_is_xr_wounded_enable_talk;
	bool m_is_xr_wounded_not_for_help;
	Script_WoundedManager* m_p_wounded_manager;
	xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>> m_xr_wounded_health_state;
	xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>> m_xr_wounded_health_state_see;
	xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>> m_xr_wounded_psy_state;
	xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>> m_xr_wounded_health_victim;
	xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>> m_xr_wounded_health_cover;
	xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>> m_xr_wounded_health_fight;
	xr_string m_xr_wounded_help_dialog_name;
	xr_string m_xr_wounded_help_start_dialog_name;
	xr_string m_xr_wounded_wounded_section_name;
};
struct Script_ComponentScheme_XRMeet : public Script_IComponentScheme
{
	Script_ComponentScheme_XRMeet(void) : m_is_meet_only_at_path(true), m_is_set(false), m_reset_distance(30.0f), m_p_meet_manager(nullptr) {}
	~Script_ComponentScheme_XRMeet(void)
	{
		if (this->m_p_meet_manager)
		{
			MESSAGEI("deleted meet manager!");
			xr_delete(this->m_p_meet_manager);
		}
	}

	inline const xr_map<std::uint32_t, CondlistData>& getCloseDistance(void) const noexcept { return this->m_close_distance; }
	inline void setCloseDistance(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_close_distance = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getCloseAnim(void) const noexcept { return this->m_close_anim; }
	inline void setCloseAnim(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_close_anim = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getCloseSoundDistance(void) const noexcept { return this->m_close_sound_distance; }
	inline void setCloseSoundDistance(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_close_sound_distance = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getCloseSoundHello(void) const noexcept { return this->m_close_sound_hello; }
	inline void setCloseSoundHello(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_close_sound_hello = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getCloseSoundBye(void) const noexcept { return this->m_close_sound_bye; }
	inline void setCloseSoundBye(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!") this->m_close_sound_bye = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getCloseVictim(void) const noexcept { return this->m_close_victim; }
	inline void setCloseVictim(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_close_victim = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getFarDistance(void) const noexcept { return this->m_far_distance; }
	inline void setFarDistance(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_far_distance = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getFarAnim(void) const noexcept { return this->m_far_anim; }
	inline void setFarAnim(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_far_anim = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getFarSoundDistance(void) const noexcept { return this->m_far_sound_distance; }
	inline void setFarSoundDistance(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_far_sound_distance = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getFarVictim(void) const noexcept { return this->m_far_victim; }
	inline void setFarVictim(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_far_victim = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getSoundOnUse(void) const noexcept { return this->m_sound_on_use; }
	inline void setSoundOnUse(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_sound_on_use = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getUse(void) const noexcept { return this->m_use; }
	inline void setUse(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_use = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getMeetDialog(void) const noexcept { return this->m_meet_dialog; }
	inline void setMeetDialog(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_meet_dialog = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getAbuse(void) const noexcept { return this->m_abuse; }
	inline void setAbuse(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_abuse = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getTradeEnable(void) const noexcept { return this->m_trade_enable; }
	inline void setTradeEnable(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_trade_enable = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getAllowBreak(void) const noexcept { return this->m_allow_break; }
	inline void setAllowBreak(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_allow_break = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getMeetOnTalking(void) const noexcept { return this->m_meet_on_talking; }
	inline void setMeetOnTalking(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_meet_on_talking = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getUseText(void) const noexcept { return this->m_use_text; }
	inline void setUseText(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_use_text = data; }

	inline const xr_map<std::uint32_t, CondlistData>& getFarSound(void) const noexcept { return this->m_far_sound; }
	inline void setFarSound(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_far_sound = data; }

	inline float getResetDistance(void) const noexcept { return this->m_reset_distance; }
	inline void setResetDistance(const float value) noexcept { this->m_reset_distance = value; }

	inline bool isMeetOnlyAtPath(void) const noexcept { return this->m_is_meet_only_at_path; }
	inline void setMeetOnlyAtPath(const bool value) noexcept { this->m_reset_distance = value; }

	inline bool isSet(void) const noexcept { return this->m_is_set; }
	inline void setSet(const bool value) noexcept { this->m_is_set = value; }

	inline const xr_map<std::uint32_t, CondlistData>& getMeetDialogName(void) const noexcept { return this->m_meet_dialog; }
	inline void setMeetDialogName(const xr_map<std::uint32_t, CondlistData>& data) noexcept
	{
		if (data.empty()) MESSAGEW("you are trying to set an empty map!");
		this->m_meet_dialog = data;
	}

	inline const xr_string& getMeetSectionName(void) const noexcept { return this->m_meet_section_name; }
	inline void setMeetSectionName(const xr_string& name) noexcept
	{
		if (name.empty())
		{
			MESSAGEW("you are trying to set an empty map!");
		}

		this->m_meet_section_name = name;
	}

	inline void clear(void)
	{
		this->m_is_meet_only_at_path = true;
		this->m_is_set = false;
		this->m_reset_distance = 30.0f;
		this->m_close_anim.clear();
		this->m_close_distance.clear();
		this->m_close_sound_bye.clear();
		this->m_close_sound_distance.clear();
		this->m_close_sound_hello.clear();
		this->m_close_victim.clear();
		this->m_far_anim.clear();
		this->m_far_distance.clear();
		this->m_far_sound.clear();
		this->m_far_sound_distance.clear();
		this->m_far_victim.clear();
		this->m_sound_on_use.clear();
		this->m_use.clear();
		this->m_meet_dialog.clear();
		this->m_abuse.clear();
		this->m_trade_enable.clear();
		this->m_allow_break.clear();
		this->m_meet_on_talking.clear();
		this->m_use_text.clear();
		this->m_meet_section_name.clear();

		if (this->m_p_meet_manager)
		{
			MESSAGEI("deleted meet manager!");
			xr_delete(this->m_p_meet_manager);
		}
	}

	inline Script_XRMeetManager* const getMeetManager(void) const { return this->m_p_meet_manager; }
	inline void setMeetManager(Script_XRMeetManager* const p_meet)
	{
		if (this->m_p_meet_manager)
		{
			R_ASSERT2(false, "you can't set to existed instance you must deallocate this!");
			return;
		}

		if (!p_meet)
		{
			MESSAGEER("Can't set an empty instance check your code and allocation!");
			return;
		}

		this->m_p_meet_manager = p_meet;
	}
private:
	bool m_is_meet_only_at_path;
	bool m_is_set;
	float m_reset_distance;
	Script_XRMeetManager* m_p_meet_manager;
	xr_map<std::uint32_t, CondlistData> m_close_distance;
	xr_map<std::uint32_t, CondlistData> m_close_anim;
	xr_map<std::uint32_t, CondlistData> m_close_sound_distance;
	xr_map<std::uint32_t, CondlistData> m_close_sound_hello;
	xr_map<std::uint32_t, CondlistData> m_close_sound_bye;
	xr_map<std::uint32_t, CondlistData> m_close_victim;
	xr_map<std::uint32_t, CondlistData> m_far_distance;
	xr_map<std::uint32_t, CondlistData> m_far_anim;
	xr_map<std::uint32_t, CondlistData> m_far_sound_distance;
	xr_map<std::uint32_t, CondlistData> m_far_sound;
	xr_map<std::uint32_t, CondlistData> m_far_victim;
	xr_map<std::uint32_t, CondlistData> m_sound_on_use;
	xr_map<std::uint32_t, CondlistData> m_use;
	xr_map<std::uint32_t, CondlistData> m_meet_dialog;
	xr_map<std::uint32_t, CondlistData> m_abuse;
	xr_map<std::uint32_t, CondlistData> m_trade_enable;
	xr_map<std::uint32_t, CondlistData> m_allow_break;
	xr_map<std::uint32_t, CondlistData> m_meet_on_talking;
	xr_map<std::uint32_t, CondlistData> m_use_text;
	xr_string m_meet_section_name;
};
struct Script_ComponentScheme_XRCamper : public Script_IComponentScheme
{
	Script_ComponentScheme_XRCamper(void) : m_is_sniper(false), m_is_no_retreat(false), m_wp_flag(0),
		m_scan_begin(0), m_current_look_point(0), m_memory_enemy(0), m_idle(0), m_scantimefree(0),
		m_radius(0.0f)
	{}

	inline float getRadius(void) const noexcept { return this->m_radius; }
	inline void setRadius(const float value) noexcept { this->m_radius = value; }

	inline const xr_string& getAttackSoundName(void) const noexcept { return this->m_attack_sound_name; }
	inline void setAttackSoundName(const xr_string& sound_name) noexcept
	{
		if (sound_name.empty())
		{
			MESSAGEW("you are trying to set an empty string!");
		}

		this->m_attack_sound_name = sound_name;
	}

	inline float getEnemyDisp(void) const noexcept { return this->m_enemy_disp; }

	inline std::uint32_t getPostEnemyWait(void) const noexcept { return this->m_post_enemy_wait; }

	inline float getScanDelta(void) const noexcept { return this->m_scandelta; }
	inline std::uint32_t getTimeDelta(void) const noexcept { return this->m_timedelta; }
	inline std::uint32_t getTimeScanDelta(void) const noexcept { return this->m_timescandelta; }

	inline std::uint32_t getIdle(void) const noexcept { return this->m_idle; }
	inline void setIdle(const std::uint32_t value) noexcept { this->m_idle = value; }

	inline std::uint32_t getScanTimeFree(void) const noexcept { return this->m_scantimefree; }
	inline void setScanTimeFree(const std::uint32_t value) noexcept { this->m_scantimefree = value; }

	inline const xr_string& getSniperAnimName(void) const noexcept { return this->m_sniper_anim_name; }
	inline void setSniperAnimName(const xr_string& animation_name) noexcept
	{
		if (animation_name.empty())
		{
			MESSAGEW("you are trying to set an empty string");
		}

		this->m_sniper_anim_name = animation_name;
	}

	inline bool isSniper(void) const noexcept { return this->m_is_sniper; }
	inline void setSniper(const bool value) noexcept { this->m_is_sniper = value; }

	inline bool isNoRetreat(void) const noexcept { return this->m_is_no_retreat; }
	inline void setNoRetreat(const bool value) noexcept { this->m_is_no_retreat = value; }

	inline const xr_string& getShootName(void) const noexcept { return this->m_shoot_name; }
	inline void setShootName(const xr_string& name) noexcept
	{
		if (name.empty())
		{
			MESSAGEW("you are trying to set an empty string");
		}

		this->m_shoot_name = name;
	}

	inline const std::pair<std::uint32_t, Fvector>& getLastLookPoint(void) const noexcept { return this->m_last_look_point; }
	inline void setLastLookPointKey(const std::uint32_t value) noexcept { this->m_last_look_point.first = value; }
	inline void setLastLookPointPos(const Fvector& position) noexcept { this->m_last_look_point.second = position; }
	inline void setLastLookPoint(const std::uint32_t value, const Fvector& position) noexcept { this->m_last_look_point.first = value; this->m_last_look_point.second = position; }


	inline std::uint32_t getWpFlag(void) const noexcept { return this->m_wp_flag; }
	inline void setWpFlag(const std::uint32_t value) noexcept
	{
		if (value >= std::uint8_t(-1))
		{
			MESSAGEWR("deprecated value!");
			return;
		}
		this->m_wp_flag = value;
	}

	inline const xr_map<xr_string, xr_string>& getSuggestedStates(void) const noexcept { return this->m_suggested_states; }
	inline void setSuggestedState(const xr_string& state_name, const xr_string& value_name) noexcept
	{
		if (state_name.empty())
		{
			MESSAGEWR("you trying to set an empty string as a key");
			return;
		}

		if (value_name.empty())
		{
			MESSAGEW("you are trying to set an empty string");
			return;
		}

		this->m_suggested_states[state_name] = value_name;
	}

	inline const xr_map<std::uint32_t, xr_vector<std::pair<std::uint32_t, Fvector>>>& getScanTable(void) const noexcept { return this->m_scan_table; }
	inline void ClearScanTable(void) noexcept { this->m_scan_table.clear(); }
	inline void setScanTable(const std::uint32_t index, const std::uint32_t value, const Fvector& position) noexcept
	{
		this->m_scan_table[index].push_back({ value, position });
	}

	inline std::uint32_t getScanBegin(void) const noexcept { return this->m_scan_begin; }
	inline void setScanBegin(const std::uint32_t value) noexcept { this->m_scan_begin = value; }

	inline std::uint32_t getCurrentLookPoint(void) const noexcept { return this->m_current_look_point; }
	inline void setCurrentLookPoint(const std::uint32_t value) noexcept { this->m_current_look_point = value; }

	inline std::uint32_t getMemoryEnemy(void) const noexcept { return this->m_memory_enemy; }
	inline void setMemoryEnemy(const std::uint32_t value) noexcept { this->m_memory_enemy = value; }

	inline void clear(void) noexcept
	{
		this->m_is_no_retreat = false;
		this->m_is_sniper = false;
		this->m_wp_flag = 0;
		this->m_scan_begin = 0;
		this->m_current_look_point = 0;
		this->m_memory_enemy = 0;
		this->m_idle = 0;
		this->m_scantimefree = 0;
		this->m_radius = 0.0f;
		this->m_last_look_point.first = 0;
		this->m_last_look_point.second.x = 0.0f;
		this->m_last_look_point.second.y = 0.0f;
		this->m_last_look_point.second.z = 0.0f;
		this->m_scan_table.clear();
		this->m_suggested_states.clear();
		this->m_sniper_anim_name.clear();
		this->m_shoot_name.clear();
		this->m_attack_sound_name.clear();
	}

private:
	bool m_is_sniper;
	bool m_is_no_retreat;
	std::uint32_t m_wp_flag;
	std::uint32_t m_scan_begin;
	std::uint32_t m_current_look_point;
	std::uint32_t m_memory_enemy;
	const std::uint32_t m_post_enemy_wait = 5000;
	const std::uint32_t m_timedelta = 4000;
	const std::uint32_t m_timescandelta = this->m_timedelta / this->m_scandelta;
	std::uint32_t m_idle;
	std::uint32_t m_scantimefree;
	float m_radius;
	const float m_scandelta = 30.0f;
	const float m_enemy_disp = 7 / 57.2957f;
	std::pair<std::uint32_t, Fvector> m_last_look_point;
	xr_map<std::uint32_t, xr_vector<std::pair<std::uint32_t, Fvector>>> m_scan_table;
	xr_map<xr_string, xr_string> m_suggested_states;
	xr_string m_attack_sound_name;
	xr_string m_sniper_anim_name;
	xr_string m_shoot_name;
};
struct Script_ComponentScheme_XRKamp : public Script_IComponentScheme
{
	Script_ComponentScheme_XRKamp(void) : m_pos_vertex(0), m_radius(0), m_npc_position_num(0) {}

	inline std::uint32_t getPosVertex(void) const noexcept { return this->m_pos_vertex; }
	inline void setPosVertex(const std::uint32_t value) noexcept { this->m_pos_vertex = value; }

	inline std::uint32_t getRadius(void) const noexcept { return this->m_radius; }
	inline void setRadius(const std::uint32_t value) noexcept { this->m_radius = value; }

	inline const xr_string& getCenterPointName(void) const noexcept { return this->m_center_point_name; }
	inline void setCenterPointName(const xr_string& center_point_name) noexcept { this->m_center_point_name = center_point_name; }

	inline const xr_string& getDefStateMoving(void) const noexcept { return this->m_def_state_moving_name; }
	inline void setDefStateMoving(const xr_string& state_name) noexcept { this->m_def_state_moving_name = state_name; }

	inline std::uint32_t getNpcPositionNum(void) const noexcept { return this->m_npc_position_num; }
	inline void setNpcPositionNum(const std::uint32_t value) noexcept
	{
		this->m_npc_position_num = value;
	}

	inline void clear(void) noexcept
	{
		this->m_center_point_name.clear();
		this->m_def_state_moving_name.clear();
		this->m_radius = 0;
		this->m_pos_vertex = 0;
		this->m_npc_position_num = 0;
	}
private:
	std::uint32_t m_pos_vertex;
	std::uint32_t m_radius;
	std::uint32_t m_npc_position_num;
	xr_string m_center_point_name;
	xr_string m_def_state_moving_name;
};
struct Script_ComponentScheme_XRSleeper : public Script_IComponentScheme
{
	Script_ComponentScheme_XRSleeper(void) : m_is_wakeable(false) {}

	inline bool isWakeable(void) const noexcept { return this->m_is_wakeable; }
	inline void setWakeable(const bool value) noexcept { this->m_is_wakeable = value; }

	inline const xr_string& getPathMainName(void) const noexcept { return this->m_path_main_name; }
	inline void setPathMainName(const xr_string& path_name) noexcept
	{
		if (path_name.empty())
		{
			MESSAGEWR("path_name.empty() == true!");
			return;
		}

		this->m_path_main_name = path_name;
	}

	inline const xr_string& getPathWalkName(void) const noexcept { return this->m_path_walk_name; }
	inline void setPathWalkName(const xr_string& path_name) noexcept
	{
		if (path_name.empty())
		{
			MESSAGEWR("path_name.empty() == true!");
			return;
		}

		this->m_path_walk_name = path_name;
	}

	inline const xr_string& getPathLookName(void) const noexcept { return this->m_path_look_name; }
	inline void setPathLookName(const xr_string& path_name) noexcept
	{
		if (path_name.empty())
		{
			MESSAGEWR("path_name.empty() == true!");
			return;
		}

		this->m_path_look_name = path_name;
	}

	inline void clear(void) noexcept
	{
		this->m_is_wakeable = false;
		this->m_path_look_name.clear();
		this->m_path_main_name.clear();
		this->m_path_walk_name.clear();
	}
private:
	bool m_is_wakeable;
	xr_string m_path_main_name;
	xr_string m_path_walk_name;
	xr_string m_path_look_name;
};
struct Script_ComponentScheme_XRWalker : public Script_IComponentScheme
{
	Script_ComponentScheme_XRWalker(void) : m_is_use_camp(false) {}

	inline const xr_string& getPathWalkName(void) const noexcept { return this->m_path_walk_name; }
	inline void setPathWalkName(const xr_string& path_name) noexcept
	{
		if (path_name.empty())
		{
			MESSAGEWR("path_walk.empty() == "
				"true! You are set an empty string");
			return;
		}

		this->m_path_walk_name = path_name;
	}

	inline const xr_string& getPathLookName(void) const noexcept { return this->m_path_look_name; }
	inline void setPathLookName(const xr_string& path_name) noexcept
	{
		if (path_name.empty())
		{
			MESSAGEWR("path_name.empty() == true! You are set an empty string");
			return;
		}

		this->m_path_look_name = path_name;
	}

	inline const xr_string& getTeamName(void) const noexcept { return this->m_team_name; }
	inline void setTeamName(const xr_string& team_name) noexcept
	{
		if (team_name.empty())
		{
			MESSAGEWR("team_name.empty() == true! You are set an empty string");
			return;
		}

		this->m_team_name = team_name;
	}

	inline const xr_string& getDescriptionName(void) const noexcept
	{
		return this->m_description_name;
	}
	inline void setDescriptionName(const xr_string& description_name) noexcept
	{
		if (description_name.empty())
		{
			MESSAGEWR("description_name.empty() == true! You are set an empty string!");
			return;
		}

		this->m_description_name = description_name;
	}

	inline bool isUseCamp(void) const noexcept { return this->m_is_use_camp; }
	inline void setUseCamp(const bool is_using) noexcept { this->m_is_use_camp = is_using; }

	inline const xr_map<xr_string, xr_string>& getSuggestedStates(void) const noexcept
	{
		return this->m_suggested_states;
	}

	inline void setSuggestedStates(const xr_string& type_name, const xr_string& animation_name) noexcept
	{
		if (type_name.empty())
		{
			MESSAGEW("type_name.empty() == true! You are set an empty string!");
			return;
		}

		if (this->m_suggested_states.find(type_name) != this->m_suggested_states.end())
		{
			MESSAGEW("you are trying to change the existing value %s to %s",
				this->m_suggested_states.at(type_name).c_str(), animation_name.c_str());
		}

		this->m_suggested_states[type_name] = animation_name;
	}

	inline const xr_string& getSoundIdleName(void) const noexcept { return this->m_sound_idle_name; }
	inline void setSoundIdleName(const xr_string& sound_name) noexcept
	{
		if (sound_name.empty())
		{
			MESSAGEWR("sound_name.empty() == true!");
			return;
		}


		this->m_sound_idle_name = sound_name;
	}

	inline void clear(void) noexcept
	{
		this->m_is_use_camp = false;
		this->m_description_name.clear();
		this->m_path_look_name.clear();
		this->m_path_walk_name.clear();
		this->m_sound_idle_name.clear();
		this->m_suggested_states.clear();
		this->m_team_name.clear();
	}
private:
	bool m_is_use_camp;
	xr_map<xr_string, xr_string> m_suggested_states;
	xr_string m_description_name;
	xr_string m_path_walk_name;
	xr_string m_team_name;
	xr_string m_sound_idle_name;
	xr_string m_path_look_name;
};
struct Script_ComponentScheme_XRRemark : public Script_IComponentScheme
{
	Script_ComponentScheme_XRRemark(void) : m_is_target_initialized(false), m_is_sound_animation_sync(false), m_target_id(0) {}

	inline const xr_string& getTargetName(void) const noexcept { return this->m_target_name; }
	inline void setTargetName(const xr_string& target_name) noexcept
	{
		if (target_name.empty())
		{
			MESSAGEWR("target_name.empty() == "
				"true! You are trying to set an empty string Return ...");
			return;
		}

		this->m_target_name = target_name;
	}

	inline bool isTargetInitialized(void) const noexcept { return this->m_is_target_initialized; }
	inline void setTargetInitialized(const bool value) noexcept
	{
		this->m_is_target_initialized = value;
	}

	inline const Fvector& getTargetPosition(void) const noexcept { return this->m_target_position; }
	inline void setTargetPosition(const Fvector& target_position) noexcept
	{
		this->m_target_position = target_position;
	}

	inline std::uint16_t getTargetID(void) const noexcept { return this->m_target_id; }
	inline void setTargetID(const std::uint16_t value) noexcept { this->m_target_id = value; }

	inline bool isSoundAnimationSync(void) const noexcept { return this->m_is_sound_animation_sync; }
	inline void setSoundAnimationSync(const bool value) noexcept
	{
		this->m_is_sound_animation_sync = value;
	}

	inline const xr_string& getSoundName(void) const noexcept { return this->m_sound_name; }
	inline void setSoundName(const xr_string& sound_name) noexcept
	{
		if (sound_name.empty())
		{
			MESSAGEWR("sound_name.empty() == "
				"true! You are set an empty string Return ...");
			return;
		}

		this->m_sound_name = sound_name;
	}

	inline const xr_string& getTipsIDName(void) const noexcept { return this->m_tips_id_name; }
	inline void setTipsIDName(const xr_string& text) noexcept
	{
		if (text.empty())
		{
			MESSAGEWR("text.empty() == true! You are "
				"set an empty string! Return ...");
			return;
		}

		this->m_tips_id_name = text;
	}

	inline const xr_string& getTipsSenderName(void) const noexcept
	{
		return this->m_tips_sender_name;
	}
	inline void setTipsSenderName(const xr_string& text) noexcept
	{
		if (text.empty())
		{
			MESSAGEWR("text.empty() == treu! You "
				"are set an empty string! Return ...");
			return;
		}

		this->m_tips_sender_name = text;
	}

	inline const xr_map<std::uint32_t, CondlistData>& getAnimationCondlist(void) const noexcept
	{
		return this->m_animation_condlist;
	}

	inline void setAnimationCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
	{
		if (condlist.empty())
		{
			MESSAGEWR("condlist.empty() == "
				"true! You are trying to set an empty condlist");
			return;
		}

		this->m_animation_condlist = condlist;
	}

	inline void clear(void) noexcept
	{
		this->m_is_sound_animation_sync = false;
		this->m_is_target_initialized = false;
		this->m_target_id = 0;
		this->m_target_position.x = 0.0f;
		this->m_target_position.y = 0.0f;
		this->m_target_position.z = 0.0f;
		this->m_animation_condlist.clear();
		this->m_sound_name.clear();
		this->m_target_name.clear();
		this->m_tips_id_name.clear();
		this->m_tips_sender_name.clear();
	}

private:
	bool m_is_target_initialized;
	bool m_is_sound_animation_sync;
	std::uint16_t m_target_id;
	Fvector m_target_position;
	xr_map<std::uint32_t, CondlistData> m_animation_condlist;
	xr_string m_target_name;
	xr_string m_sound_name;
	xr_string m_tips_id_name;
	xr_string m_tips_sender_name;
};
struct Script_ComponentScheme_XRGatherItems : public Script_IComponentScheme
{
	Script_ComponentScheme_XRGatherItems(void) : m_is_gather_items_enabled(false) {}

	inline bool isGatherItemsEnabled(void) const noexcept { return this->m_is_gather_items_enabled; }
	inline void setGatherItemsEnabled(const bool value) noexcept { this->m_is_gather_items_enabled = value; }

	inline void clear(void) noexcept
	{
		this->m_is_gather_items_enabled = false;
	}

private:
	bool m_is_gather_items_enabled;
};
struct Script_ComponentScheme_XRHelpWounded : public Script_IComponentScheme
{
	Script_ComponentScheme_XRHelpWounded(void) : m_is_wounded_enabled(false) {}

	inline bool IsXRHelpWoundedHelpWoundedEnabled(void) const noexcept
	{
		return this->m_is_wounded_enabled;
	}

	inline void setXRHelpWoundedHelpWoundedEnabled(const bool value) noexcept
	{
		this->m_is_wounded_enabled = value;
	}

	inline void clear(void) noexcept
	{
		this->m_is_wounded_enabled = false;
	}

private:
	bool m_is_wounded_enabled;
};
struct Script_ComponentScheme_XRCombat : public Script_IComponentScheme
{
	Script_ComponentScheme_XRCombat(void) {}

	inline const xr_string& getXRCombatScriptCombatTypeName(void) const noexcept
	{
		return this->m_xr_combat_script_combat_type_name;
	}

	inline void setXRCombatScriptCombatTypeName(const xr_string& type_name) noexcept
	{
		this->m_xr_combat_script_combat_type_name = type_name;
	}

	inline const xr_map<std::uint32_t, CondlistData>& getXRCombatCombatTypeCondlist(void) const noexcept
	{
		return this->m_xr_combat_combat_type_condlist;
	}

	inline void setXRCombatCombatTypeCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
	{
		this->m_xr_combat_combat_type_condlist = condlist;
	}

	inline void clear(void) noexcept
	{
		this->m_xr_combat_combat_type_condlist.clear();
		this->m_xr_combat_script_combat_type_name.clear();
	}

private:
	xr_map<std::uint32_t, CondlistData> m_xr_combat_combat_type_condlist;
	xr_string m_xr_combat_script_combat_type_name;
};
struct Script_ComponentScheme_XRCorpseDetection : public Script_IComponentScheme
{
	Script_ComponentScheme_XRCorpseDetection(void) : m_selected_corpse_id(0) {}
	inline void clear(void) noexcept { this->m_selected_corpse_id = 0; }

	inline std::uint16_t getXRCorpseDetectionSelectedCorpseID(void) const noexcept
	{
		return this->m_selected_corpse_id;
	}
	inline void setXRCorpseDetectionSelectedCorpseID(const std::uint16_t id) noexcept
	{
		this->m_selected_corpse_id = id;
	}
private:
	std::uint16_t m_selected_corpse_id;
};
struct Script_ComponentScheme_XRAbuse : public Script_IComponentScheme
{
	Script_ComponentScheme_XRAbuse(void) : m_p_abuse_manager(nullptr) {}
	~Script_ComponentScheme_XRAbuse(void)
	{
		if (this->m_p_abuse_manager)
		{
			MESSAGEI("deleting abuse manager!");
			xr_delete(this->m_p_abuse_manager);
		}
	}

	inline Script_XRAbuseManager* const getManager(void) const { return this->m_p_abuse_manager; }
	inline void setManager(Script_XRAbuseManager* const p_object)
	{
		// Lord: проверить все таки manager* если можно будет ресетить на новый объект при условии что оно вообще удалется только в деструкторе ...
		if (this->m_p_abuse_manager)
		{
			R_ASSERT2(false, "you can't set to existed instance you must deallocate this!");
			return;
		}

		if (!p_object)
		{
			MESSAGEWR("you are trying to set an empty "
				"object return ...");
			return;
		}

		this->m_p_abuse_manager = p_object;
	}

	inline void clear(void)
	{
		if (this->m_p_abuse_manager)
		{
			MESSAGEI("deleting abuse manager!");
			xr_delete(this->m_p_abuse_manager);
		}
	}
private:
	Script_XRAbuseManager* m_p_abuse_manager;
};

struct Script_ComponentScheme_XRDanger : public Script_IComponentScheme
{
    Script_ComponentScheme_XRDanger(void) {}

    inline void clear(void) noexcept { }
};

struct Script_ComponentScheme_XRDeath : public Script_IComponentScheme
{
	Script_ComponentScheme_XRDeath(void) {}

	inline const xr_map<std::uint32_t, CondlistData>& getInfo(void) const noexcept
	{
		return this->m_xr_death_info;
	}
	inline void setInfo(const xr_map<std::uint32_t, CondlistData>& data) noexcept
	{
		this->m_xr_death_info = data;
	}

	inline const xr_map<std::uint32_t, CondlistData>& getInfo2(void) const noexcept
	{
		return this->m_xr_death_info2;
	}
	inline void setInfo2(const xr_map<std::uint32_t, CondlistData>& data) noexcept
	{
		this->m_xr_death_info2 = data;
	}

	inline void clear(void) noexcept
	{
		this->m_xr_death_info.clear();
		this->m_xr_death_info2.clear();
	}
private:
	xr_map<std::uint32_t, CondlistData> m_xr_death_info;
	xr_map<std::uint32_t, CondlistData> m_xr_death_info2;
};
struct Script_ComponentScheme_SRDeimos : public Script_IComponentScheme
{
	Script_ComponentScheme_SRDeimos(void) { this->clear(); }

	inline float getSRDeimosIntensity(void) const noexcept { return this->m_intensity; }
	inline void setSRDeimosIntensity(const float value) noexcept { this->m_intensity = value; }

	inline float getSRDeimosMovementSpeed(void) const noexcept { return this->m_movement_speed; }
	inline void setSRDeimosMovementSpeed(const float value) noexcept { this->m_movement_speed = value; }

	inline float getSRDeimosGrowingKoef(void) const noexcept { return this->m_growing_koef; }
	inline void setSRDeimosGrowingKoef(const float value) noexcept { this->m_growing_koef = value; }

	inline float getSRDeimosLoweringKoef(void) const noexcept { return this->m_lowering_koef; }
	inline void setSRDeimosLoweringKoef(const float value) noexcept { this->m_lowering_koef = value; }

	inline std::uint32_t getSRDeimosCameraEffectorRepeatingTime(void) const noexcept
	{
		return this->m_camera_effector_repeating_time;
	}
	inline void setSRDeimosCameraEffectorRepeatingTime(const std::uint32_t value) noexcept
	{
		this->m_camera_effector_repeating_time = value;
	}

	inline float getSRDeimosHealthLost(void) const noexcept { return this->m_health_lost; }
	inline void setSRDeimosHealthLost(const float value) noexcept { this->m_health_lost = value; }

	inline float getSRDeimosDisableBound(void) const noexcept { return this->m_disable_bound; }
	inline void setSRDeimosDisableBound(const float value) noexcept { this->m_disable_bound = value; }

	inline float getSRDeimosSwitchLowerBound(void) const noexcept { return this->m_switch_lower_bound; }
	inline void setSRDeimosSwitchLowerBound(const float value) noexcept
	{
		this->m_switch_lower_bound = value;
	}

	inline float getSRDeimosSwitchUpperBound(void) const noexcept { return this->m_switch_upper_bound; }
	inline void setSRDeimosSwitchUpperBound(const float value) noexcept
	{
		this->m_switch_upper_bound = value;
	}

	inline const xr_string& getSRDeimosPostProcessEffectorName(void) const noexcept
	{
		return this->m_postprocess_effector_name;
	}
	inline void setSRDeimosPostProcessEffectorName(const xr_string& effector_name) noexcept
	{
		if (effector_name.empty())
		{
			MESSAGEW("effector_name.empty() == true! You set an empty string");
		}

		this->m_postprocess_effector_name = effector_name;
	}

	inline const xr_string& getSRDeimosCameraEffectorName(void) const noexcept
	{
		return this->m_camera_effector_name;
	}
	inline void setSRDeimosCameraEffectorName(const xr_string& effector_name) noexcept
	{
		if (effector_name.empty())
		{
			MESSAGEW("effector_name.empty() == true! You set an empty string");
		}

		this->m_camera_effector_name = effector_name;
	}

	inline const xr_string& getSRDeimosPostProcessEffector2Name(void) const noexcept
	{
		return this->m_postprocess_effector2_name;
	}
	inline void setSRDeimosPostProcessEffector2Name(const xr_string& effector_name) noexcept
	{
		if (effector_name.empty())
		{
			MESSAGEW("effector_name.empty() == true! You set an empty string");
		}

		this->m_postprocess_effector2_name = effector_name;
	}

	inline const xr_string& getSRDeimosNoiseSoundName(void) const noexcept
	{
		return this->m_noise_sound_name;
	}
	inline void setSRDeimosNoiseSoundName(const xr_string& sound_name) noexcept
	{
		if (sound_name.empty())
		{
			MESSAGEW("sound_name.empty() "
				"== true! You set an empty string");
		}

		this->m_noise_sound_name = sound_name;
	}

	inline const xr_string& getSRDeimosHeartBeetSoundName(void) const noexcept
	{
		return this->m_heartbeet_sound_name;
	}
	inline void setSRDeimosHeartBeetSoundName(const xr_string& sound_name) noexcept
	{
		if (sound_name.empty())
		{
			MESSAGEW("sound_name.empty() == true! You set an empty string");
		}

		this->m_heartbeet_sound_name = sound_name;
	}

	inline void clear(void) noexcept
	{
		this->m_camera_effector_name.clear();
		this->m_camera_effector_repeating_time = 0;
		this->m_intensity = 0.0f;
		this->m_disable_bound = 0.0f;
		this->m_growing_speed = 0.0f;
		this->m_growing_koef = 0.0f;
		this->m_lowering_koef = 0.0f;
		this->m_switch_lower_bound = 0.0f;
		this->m_switch_upper_bound = 0.0f;
		this->m_health_lost = 0.0f;
		this->m_movement_speed = 0.0f;
		this->m_postprocess_effector_name.clear();
		this->m_postprocess_effector2_name.clear();
		this->m_heartbeet_sound_name.clear();
		this->m_noise_sound_name.clear();
	}

private:
	std::uint32_t m_camera_effector_repeating_time;
	float m_intensity;
	float m_growing_speed;
	float m_growing_koef;
	float m_lowering_koef;
	float m_disable_bound;
	float m_switch_lower_bound;
	float m_switch_upper_bound;
	float m_health_lost;
	float m_movement_speed;
	xr_string m_postprocess_effector_name;
	xr_string m_camera_effector_name;
	xr_string m_postprocess_effector2_name;
	xr_string m_noise_sound_name;
	xr_string m_heartbeet_sound_name;
};
struct Script_ComponentScheme_SRLight : public Script_IComponentScheme
{
	Script_ComponentScheme_SRLight(void) : m_is_light(false) {}

	inline bool IsSRLightLight(void) const noexcept { return this->m_is_light; }
	inline void setSRLightLight(const bool value) noexcept { this->m_is_light = value; }

	inline void clear(void) noexcept
	{
		this->m_is_light = false;
	}
private:
	bool m_is_light;
};
struct Script_ComponentScheme_SRParticle : public Script_IComponentScheme
{
	Script_ComponentScheme_SRParticle(void) : m_is_looped(false), m_mode(0) {}

	inline bool IsSRParticleLooped(void) const noexcept { return this->m_is_looped; }
	inline void setSRParticleLooped(const bool value) noexcept { this->m_is_looped = value; }

	inline std::uint32_t getSRParticleMode(void) const noexcept { return this->m_mode; }
	inline void setSRParticleMode(const std::uint32_t value) noexcept { this->m_mode = value; }

	inline const xr_string& getSRParticleName(void) const noexcept { return this->m_name; }
	inline void setSRParticleName(const xr_string& name) noexcept
	{
		if (name.empty())
		{
			MESSAGEW("name.empty() == true! You set an "
				"empty string");
		}

		this->m_name = name;
	}

	inline const xr_string& getSRParticlePathName(void) const noexcept { return this->m_path_name; }
	inline void setSRParticlePathName(const xr_string& path_name) noexcept
	{
		if (path_name.empty())
		{
			MESSAGEW("path_name.empty() == "
				"true! You set an empty string");
		}

		this->m_path_name = path_name;
	}

	inline void clear(void) noexcept
	{
		this->m_is_looped = false;
		this->m_mode = 0;
		this->m_name.clear();
		this->m_path_name.clear();
	}

private:
	bool m_is_looped;
	std::uint32_t m_mode;
	xr_string m_name;
	xr_string m_path_name;
};
struct Script_ComponentScheme_SRPostProcess : public Script_IComponentScheme
{
	Script_ComponentScheme_SRPostProcess(void) : m_hit_intensity(0.0f), m_intensity(0.0f), m_intensity_speed(0.0f) {}

	inline void clear(void) noexcept
	{
		this->m_hit_intensity = 0.0f;
		this->m_intensity_speed = 0.0f;
		this->m_intensity = 0.0f;
	}

	inline float getSRPostProcessIntensity(void) const noexcept { return this->m_intensity; }
	inline void setSRPostProcessIntensity(const float value) noexcept { this->m_intensity = value; }

	inline float getSRPostProcessIntensitySpeed(void) const noexcept { return this->m_intensity_speed; }
	inline void setSRPostProcessIntensitySpeed(const float value) noexcept
	{
		this->m_intensity_speed = value;
	}

	inline float getSRPostProcessHitIntensity(void) const noexcept { return this->m_hit_intensity; }
	inline void setSRPostProcessHitIntensity(const float value) noexcept
	{
		this->m_hit_intensity = value;
	}

private:
	float m_intensity;
	float m_intensity_speed;
	float m_hit_intensity;
};
struct Script_ComponentScheme_SRPsyAntenna : public Script_IComponentScheme
{
	Script_ComponentScheme_SRPsyAntenna(void) { this->clear(); }

	inline void clear(void) noexcept
	{
		this->m_is_sr_psy_antenna_no_static = false;
		this->m_is_sr_psy_antenna_no_mumble = false;
		this->m_sr_psy_antenna_intensity = 0.0f;
		this->m_sr_psy_antenna_hit_intensity = 0.0f;
		this->m_sr_psy_antenna_phantom_probability = 0.0f;
		this->m_sr_psy_antenna_mute_sound_threshold = 0.0f;
		this->m_sr_psy_antenna_hit_frequency = 0.0f;
		this->m_sr_psy_antenna_hit_type_name.clear();
		this->m_sr_psy_antenna_postprocess_name.clear();
	}

	inline float getSRPsyAntennaIntensity(void) const noexcept { return this->m_sr_psy_antenna_intensity; }
	inline void setSRPsyAntennaIntensity(const float value) noexcept { this->m_sr_psy_antenna_intensity = value; }

	inline float getSRPsyAntennaHitIntensity(void) const noexcept { return this->m_sr_psy_antenna_hit_intensity; }
	inline void setSRPsyAntennaHitIntensity(const float value) noexcept
	{
		this->m_sr_psy_antenna_hit_intensity = value;
	}

	inline float getSRPsyAntennaPhantomProbability(void) const noexcept
	{
		return this->m_sr_psy_antenna_phantom_probability;
	}
	inline void setSRPsyAntennaPhantomProbability(const float value) noexcept
	{
		this->m_sr_psy_antenna_phantom_probability = value;
	}

	inline float getSRPsyAntennaMuteSoundThreshold(void) const noexcept
	{
		return this->m_sr_psy_antenna_mute_sound_threshold;
	}

	inline void setSRPsyAntennaMuteSoundThreshold(const float value) noexcept
	{
		this->m_sr_psy_antenna_mute_sound_threshold = value;
	}

	inline float getSRPsyAntennaHitFrequency(void) const noexcept { return this->m_sr_psy_antenna_hit_frequency; }
	inline void setSRPsyAntennaHitFrequency(const float value) noexcept
	{
		this->m_sr_psy_antenna_hit_frequency = value;
	}

	inline bool IsSRPsyAntennaNoStatic(void) const noexcept { return this->m_is_sr_psy_antenna_no_static; }
	inline void setSRPsyAntennaNoStatic(const bool value) noexcept { this->m_is_sr_psy_antenna_no_static = value; }

	inline bool IsSRPsyAntennaNoMumble(void) const noexcept { return this->m_is_sr_psy_antenna_no_mumble; }
	inline void setSRPsyAntennaNoMumble(const bool value) noexcept { this->m_is_sr_psy_antenna_no_mumble = value; }

	inline const xr_string& getSRPsyAntennaPostProcessName(void) const noexcept
	{
		return this->m_sr_psy_antenna_postprocess_name;
	}
	inline void setSRPsyAntennaPostProcessName(const xr_string& postprocess_name) noexcept
	{
		if (postprocess_name.empty())
		{
			MESSAGEW("postprocess_name.empty() == true! You set an empty string");
		}

		this->m_sr_psy_antenna_postprocess_name = postprocess_name;
	}

	inline const xr_string& getSRPsyAntennaHitTypeName(void) const noexcept
	{
		return this->m_sr_psy_antenna_hit_type_name;
	}
	inline void setSRPsyAntennaHitTypeName(const xr_string& hit_type_name) noexcept
	{
		if (hit_type_name.empty())
		{
			MESSAGEW("hit_type_name.empty() == true! You set an empty string");
		}

		this->m_sr_psy_antenna_hit_type_name = hit_type_name;
	}

private:
	bool m_is_sr_psy_antenna_no_static;
	bool m_is_sr_psy_antenna_no_mumble;
	float m_sr_psy_antenna_intensity;
	float m_sr_psy_antenna_hit_intensity;
	float m_sr_psy_antenna_phantom_probability;
	float m_sr_psy_antenna_mute_sound_threshold;
	float m_sr_psy_antenna_hit_frequency;
	xr_string m_sr_psy_antenna_postprocess_name;
	xr_string m_sr_psy_antenna_hit_type_name;
};
struct Script_ComponentScheme_SRTeleport : public Script_IComponentScheme
{
	Script_ComponentScheme_SRTeleport(void) : m_timeout(0) {}

	inline std::uint32_t getTimeout(void) const noexcept { return this->m_timeout; }
	inline void setTimeout(const std::uint32_t value) noexcept { this->m_timeout = value; }

	inline const xr_vector<std::pair<std::uint32_t, std::pair<xr_string, xr_string>>>& getPoints(void) const
		noexcept
	{
		return this->m_points;
	}

	inline void setPoints(
		const xr_vector<std::pair<std::uint32_t, std::pair<xr_string, xr_string>>>& data) noexcept
	{
		this->m_points = data;
	}

	inline void clear(void) noexcept
	{
		this->m_points.clear();
		this->m_timeout = 0;
	}

private:
	std::uint32_t m_timeout;
	xr_vector<std::pair<std::uint32_t, std::pair<xr_string, xr_string>>> m_points;
};
struct Script_ComponentScheme_SRTimer : public Script_IComponentScheme
{
	Script_ComponentScheme_SRTimer(void) : m_p_sr_timer_ui(nullptr), m_p_sr_timer_timer(nullptr), m_sr_timer_start_value(0) {}

	inline const xr_string& getSRTimerTypeName(void) const noexcept { return this->m_sr_timer_type_name; }
	inline void setSRTimerTypeName(const xr_string& timer_type_name) noexcept
	{
		if (timer_type_name.empty())
		{
			MESSAGEW("timer_type_name.empty() == true! You set an empty string");
		}

		this->m_sr_timer_type_name = timer_type_name;
	}

	inline const xr_string& getSRTimerTimerIDName(void) const noexcept { return this->m_sr_timer_timer_id_name; }
	inline void setSRTimerTimerIDName(const xr_string& timer_id_name) noexcept
	{
		if (timer_id_name.empty())
		{
			MESSAGEW("timer_id_name.empty() "
				"== true! You set an empty string");
		}

		this->m_sr_timer_timer_id_name = timer_id_name;
	}

	inline const xr_string& getSRTimerStringName(void) const noexcept { return this->m_sr_timer_string_name; }
	inline void setSRTimerStringName(const xr_string& string_name) noexcept
	{
		if (string_name.empty())
		{
			MESSAGEW("string_name.empty() == "
				"true! You set an empty string");
		}

		this->m_sr_timer_string_name = string_name;
	}

	inline std::uint32_t getSRTimerStartValue(void) const noexcept { return this->m_sr_timer_start_value; }
	inline void setSRTimerStartValue(const std::uint32_t value) noexcept { this->m_sr_timer_start_value = value; }

	inline CUIGameCustom* const getSRTimerUI(void) const { return this->m_p_sr_timer_ui; }
	inline void setSRTimerUI(CUIGameCustom* const p_ui)
	{
		if (!p_ui)
		{
			MESSAGEW("p_ui == nullptr! You set an empty "
				"object");
		}

		this->m_p_sr_timer_ui = p_ui;
	}

	inline CUIStatic* const getSRTimerTimer(void) const { return this->m_p_sr_timer_timer; }
	inline void setSRTimerTimer(CUIStatic* const p_static)
	{
		if (!p_static)
		{
			MESSAGEW("p_static == nullptr! You set an "
				"empty object");
		}

		this->m_p_sr_timer_timer = p_static;
	}

	inline const xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>>& getSRTimerOnValue(void) const noexcept
	{
		return this->m_sr_timer_on_value;
	}
	inline void setSRTimerOnValue(const xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>>& data) noexcept
	{
		this->m_sr_timer_on_value = data;
	}

	inline void clear(void) noexcept
	{
		this->m_sr_timer_start_value = 0;
		this->m_p_sr_timer_timer = nullptr;
		this->m_p_sr_timer_ui = nullptr;
		this->m_sr_timer_string_name.clear();
		this->m_sr_timer_timer_id_name.clear();
		this->m_sr_timer_type_name.clear();
	}

private:
	std::uint32_t m_sr_timer_start_value;
	CUIGameCustom* m_p_sr_timer_ui;
	CUIStatic* m_p_sr_timer_timer;
	xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>> m_sr_timer_on_value;
	xr_string m_sr_timer_type_name;
	xr_string m_sr_timer_timer_id_name;
	xr_string m_sr_timer_string_name;
};
struct Script_ComponentScheme_PHSound : public Script_IComponentScheme
{
	Script_ComponentScheme_PHSound(void) : m_is_no_hit(false), m_is_looped(false), m_is_random(false), m_pause_min(0), m_pause_max(0) {}

	inline bool IsLooped(void) const noexcept { return this->m_is_looped; }
	inline void setLooped(const bool value) noexcept { this->m_is_looped = value; }

	inline bool IsRandom(void) const noexcept { return this->m_is_random; }
	inline void setRandom(const bool value) noexcept { this->m_is_random = value; }

	inline bool IsNoHit(void) const noexcept { return this->m_is_no_hit; }
	inline void setNoHit(const bool value) noexcept { this->m_is_no_hit = value; }

	inline const xr_string& getThemeName(void) const noexcept { return this->m_theme_name; }
	inline void setThemeName(const xr_string& theme_name) noexcept
	{
		if (theme_name.empty())
		{
			MESSAGEW("theme_name.empty() == "
				"true! You set an empty string");
		}

		this->m_theme_name = theme_name;
	}

	inline std::uint32_t getPauseMin(void) const noexcept { return this->m_pause_min; }
	inline void setPauseMin(const std::uint32_t value) noexcept { this->m_pause_min = value; }

	inline std::uint32_t getPauseMax(void) const noexcept { return this->m_pause_max; }
	inline void setPauseMax(const std::uint32_t value) noexcept { this->m_pause_max = value; }

	inline void clear(void) noexcept
	{
		this->m_is_random = false;
		this->m_is_no_hit = false;
		this->m_is_looped = false;
		this->m_pause_min = 0;
		this->m_pause_max = 0;
		this->m_theme_name.clear();
	}

private:
	bool m_is_no_hit;
	bool m_is_looped;
	bool m_is_random;
	std::uint32_t m_pause_min;
	std::uint32_t m_pause_max;
	xr_string m_theme_name;
};
struct Script_ComponentScheme_PHOscillate : public Script_IComponentScheme
{
	Script_ComponentScheme_PHOscillate(void) : m_period(0) {}

	inline const xr_string& getJointName(void) const noexcept { return this->m_joint_name; }
	inline void setJointName(const xr_string& joint_name) noexcept
	{
		if (joint_name.empty())
		{
			MESSAGEW("joint_name.empty() == "
				"true! You set an empty string!");
		}

		this->m_joint_name = joint_name;
	}

	inline std::uint32_t getPeriod(void) const noexcept { return this->m_period; }
	inline void setPeriod(const std::uint32_t value) noexcept { this->m_period = value; }

	inline void clear(void) noexcept
	{
		this->m_period = 0;
		this->m_joint_name.clear();
	}

private:
	std::uint32_t m_period;
	xr_string m_joint_name;
};
struct Script_ComponentScheme_PHIdle : public Script_IComponentScheme
{
	Script_ComponentScheme_PHIdle(void) : m_is_nonscript_usable(false) {}

	inline bool IsNonScriptUsable(void) const noexcept { return this->m_is_nonscript_usable; }
	inline void setNonScriptUsable(const bool value) noexcept { this->m_is_nonscript_usable = value; }

	inline void clear(void) noexcept
	{
		this->m_is_nonscript_usable = false;
	}

private:
	bool m_is_nonscript_usable;
};
struct Script_ComponentScheme_PHHit : public Script_IComponentScheme
{
	Script_ComponentScheme_PHHit(void) : m_power(0.0f), m_impulse(0.0f) {}

	inline void clear(void) noexcept
	{
		this->m_bone_name.clear();
		this->m_direction_path_name.clear();
		this->m_power = 0.0f;
		this->m_impulse = 0.0f;
	}

	inline float getPower(void) const noexcept { return this->m_power; }
	inline void setPower(const float value) noexcept { this->m_power = value; }

	inline float getImpulse(void) const noexcept { return this->m_impulse; }
	inline void setImpulse(const float value) noexcept { this->m_impulse = value; }

	inline const xr_string& getBoneName(void) const noexcept { return this->m_bone_name; }
	inline void setBoneName(const xr_string& bone_name) noexcept
	{
		if (bone_name.empty())
		{
			MESSAGEW("bone_name.empty() == true! You "
				"set "
				"an empty string");
		}

		this->m_bone_name = bone_name;
	}

	inline const xr_string& getDirectionPathName(void) const noexcept
	{
		return this->m_direction_path_name;
	}
	inline void setDirectionPathName(const xr_string& path_name) noexcept
	{
		if (path_name.empty())
		{
			MESSAGEW("path_name.empty() == "
				"true! You set an empty string!");
		}

		this->m_direction_path_name = path_name;
	}

private:
	float m_power;
	float m_impulse;
	xr_string m_bone_name;
	xr_string m_direction_path_name;
};
struct Script_ComponentScheme_PHDoor : public Script_IComponentScheme
{
	Script_ComponentScheme_PHDoor(void) { this->clear(); }

	inline Script_SchemePHDoor* getDoorAction(void) { return this->m_p_door_action; }
	inline void setDoorAction(Script_SchemePHDoor* p_action) { this->m_p_door_action = p_action; }

	inline bool IsClosed(void) const noexcept { return this->m_is_closed; }
	inline void setClosed(const bool value) noexcept { this->m_is_closed = value; }

	inline bool IsLocked(void) const noexcept { return this->m_is_locked; }
	inline void setLocked(const bool value) noexcept { this->m_is_locked = value; }

	inline bool IsNoForce(void) const noexcept { return this->m_is_no_force; }
	inline void setNoForce(const bool value) noexcept { this->m_is_no_force = value; }

	inline bool IsNotForNpc(void) const noexcept { return this->m_is_not_for_npc; }
	inline void setNotForNpc(const bool value) noexcept { this->m_is_not_for_npc = value; }

	inline bool IsShowTips(void) const noexcept { return this->m_is_show_tips; }
	inline void setShowTips(const bool value) noexcept { this->m_is_show_tips = value; }

	inline const xr_string& getTipOpenName(void) const noexcept { return this->m_tip_open_name; }
	inline void setTipOpenName(const xr_string& tip_name) noexcept
	{
		if (tip_name.empty())
		{
			MESSAGEW("tip_name.empty() == true! "
				"You set an "
				"empty string!");
		}

		this->m_tip_open_name = tip_name;
	}

	inline const xr_string& getTipUnlockName(void) const noexcept { return this->m_tip_unlock_name; }
	inline void setTipUnlockName(const xr_string& tip_name) noexcept
	{
		if (tip_name.empty())
		{
			MESSAGEW("tip_name.empty() == true! "
				"You set an empty string!");
		}

		this->m_tip_unlock_name = tip_name;
	}

	inline const xr_string& getTipCloseName(void) const noexcept { return this->m_tip_close_name; }
	inline void setTipCloseName(const xr_string& tip_name) noexcept
	{
		if (tip_name.empty())
		{
			MESSAGEW("tip_name.empty() == true! "
				"You set an empty string!");
		}

		this->m_tip_close_name = tip_name;
	}

	inline bool IsSlider(void) const noexcept { return this->m_is_slider; }
	inline void setSlider(const bool value) noexcept { this->m_is_slider = value; }

	inline const xr_string& getSoundOpenStartName(void) const noexcept
	{
		return this->m_sound_open_start_name;
	}
	inline void setSoundOpenStartName(const xr_string& sound_name) noexcept
	{
		if (sound_name.empty())
		{
			MESSAGEW("sound_name.empty() "
				"== true! You set an empty string!");
		}

		this->m_sound_open_start_name = sound_name;
	}

	inline const xr_string& getSoundCloseStartName(void) const noexcept
	{
		return this->m_sound_close_start_name;
	}
	inline void setSoundCloseStartName(const xr_string& sound_name) noexcept
	{
		if (sound_name.empty())
		{
			MESSAGEW("sound_name.empty() == true! You set an empty string!");
		}

		this->m_sound_close_start_name = sound_name;
	}

	inline const xr_string& getSoundCloseStopName(void) const noexcept
	{
		return this->m_sound_close_stop_name;
	}
	inline void setSoundCloseStopName(const xr_string& sound_name) noexcept
	{
		if (sound_name.empty())
		{
			MESSAGEW("sound_name.empty() "
				"== true! You set an empty string!");
		}

		this->m_sound_close_stop_name = sound_name;
	}

	inline bool IsScriptUsedMoreThanOnce(void) const noexcept
	{
		return this->m_is_script_used_more_than_once;
	}
	inline void setScriptUsedMoreThanOnce(const bool value) noexcept
	{
		this->m_is_script_used_more_than_once = value;
	}

	inline void clear(void) noexcept
	{
		this->m_is_closed = false;
		this->m_is_locked = false;
		this->m_is_no_force = false;
		this->m_is_not_for_npc = false;
		this->m_is_show_tips = false;
		this->m_is_slider = false;
		this->m_is_script_used_more_than_once = false;
		this->m_p_door_action = nullptr;
		this->m_sound_close_start_name.clear();
		this->m_sound_close_stop_name.clear();
		this->m_sound_open_start_name.clear();
		this->m_tip_unlock_name.clear();
		this->m_tip_close_name.clear();
		this->m_tip_open_name.clear();
	}


private:
	bool m_is_closed;
	bool m_is_locked;
	bool m_is_no_force;
	bool m_is_not_for_npc;
	bool m_is_show_tips;
	bool m_is_slider;
	bool m_is_script_used_more_than_once;
	Script_SchemePHDoor* m_p_door_action; // @ Используется исключительно когда у нас схема -> ph_door, удаляется как обычный зарегистрированный  action, но сам pointer зануляется в деструкторе схемы!
	xr_string m_tip_open_name;
	xr_string m_tip_unlock_name;
	xr_string m_tip_close_name;
	xr_string m_sound_open_start_name;
	xr_string m_sound_close_start_name;
	xr_string m_sound_close_stop_name;
};
struct Script_ComponentScheme_PHCode : public Script_IComponentScheme
{
	Script_ComponentScheme_PHCode(void) : m_code(0) {}

	inline void clear(void) noexcept
	{
		this->m_code = 0;
		this->m_on_code_condlist.clear();
		this->m_on_check_code.clear();
		this->m_tip_name.clear();
	}

	inline const xr_string& getTipName(void) const noexcept { return this->m_tip_name; }
	inline void setTipName(const xr_string& text_name) noexcept { this->m_tip_name = text_name; }

	inline std::uint32_t getCode(void) const noexcept { return this->m_code; }
	inline void setCode(const std::uint32_t value) noexcept { this->m_code = value; }

	inline const xr_map<std::uint32_t, CondlistData>& getOnCodeCondlist(void) const noexcept
	{
		return this->m_on_code_condlist;
	}
	inline void setOnCodeCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
	{
		this->m_on_code_condlist = condlist;
	}

	inline const xr_map<xr_string, xr_map<std::uint32_t, CondlistData>>& getOnCheckCode(void) const noexcept
	{
		return this->m_on_check_code;
	}

	inline void setOnCheckCode(const xr_map<xr_string, xr_map<std::uint32_t, CondlistData>>& data) noexcept
	{
		this->m_on_check_code = data;
	}

private:
	std::uint32_t m_code;
	xr_map<std::uint32_t, CondlistData> m_on_code_condlist;
	xr_map<xr_string, xr_map<std::uint32_t, CondlistData>> m_on_check_code;
	xr_string m_tip_name;
};
struct Script_ComponentScheme_PHForce : public Script_IComponentScheme
{
	Script_ComponentScheme_PHForce(void) : m_time(0), m_delay(0) {}

	inline void clear(void) noexcept
	{
		this->m_time = 0;
		this->m_delay = 0;
		this->m_point.x = 0.0f;
		this->m_point.y = 0.0f;
		this->m_point.z = 0.0f;
	}

	inline std::uint32_t getTime(void) const noexcept { return this->m_time; }
	inline void setTime(const std::uint32_t value) noexcept { this->m_time = value; }

	inline std::uint32_t getDelay(void) const noexcept { return this->m_delay; }
	inline void setDelay(const std::uint32_t value) noexcept { this->m_delay = value; }

	inline const Fvector& getPoint(void) const noexcept { return this->m_point; }
	inline void setPoint(const Fvector& point) noexcept { this->m_point = point; }

private:
	std::uint32_t m_time;
	std::uint32_t m_delay;
	Fvector m_point;
};
struct Script_ComponentScheme_Helicopter : public Script_IComponentScheme
{
	Script_ComponentScheme_Helicopter(void) { this->clear(); }

	inline const xr_string& getPathMoveName(void) const noexcept { return this->m_path_move_name; }
	inline void setPathMoveName(const xr_string& path_name) noexcept
	{
		if (path_name.empty())
		{
			MESSAGEW("path_name.empty() == "
				"true! You set an empty string");
		}

		this->m_path_move_name = path_name;
	}

	inline const xr_string& getPathLookName(void) const noexcept { return this->m_path_look_name; }
	inline void setPathLookName(const xr_string& path_name) noexcept
	{
		if (path_name.empty())
		{
			MESSAGEW("path_name.empty() == "
				"true! You set an empty string");
		}

		this->m_path_look_name = path_name;
	}

	inline const xr_string& getEnemyName(void) const noexcept { return this->m_enemy_name; }
	inline void setEnemyName(const xr_string& enemy_name) noexcept
	{
		if (enemy_name.empty())
		{
			MESSAGEW("enemy_name.empty() == "
				"true! You set an empty string");
		}

		this->m_enemy_name = enemy_name;
	}

	inline const xr_string& getFirePointName(void) const noexcept
	{
		return this->m_fire_point_name;
	}
	inline void setFirePointName(const xr_string& fire_point_name) noexcept
	{
		if (fire_point_name.empty())
		{
			MESSAGEW("fire_point_name.empty() == true! You set an empty string");
		}

		this->m_fire_point_name = fire_point_name;
	}

	inline bool IsUseRocket(void) const noexcept { return this->m_is_use_rocket; }
	inline void setUseRocket(const bool value) noexcept { this->m_is_use_rocket = value; }

	inline bool IsUseMinigun(void) const noexcept { return this->m_is_use_minigun; }
	inline void setUseMinigun(const bool value) noexcept { this->m_is_use_minigun = value; }

	inline bool IsFireTrail(void) const noexcept { return this->m_is_fire_trail; }
	inline void setFireTrail(const bool value) noexcept { this->m_is_fire_trail = value; }

	inline bool IsEngineSound(void) const noexcept { return this->m_is_engine_sound; }
	inline void setEngineSound(const bool value) noexcept { this->m_is_engine_sound = value; }

	inline bool IsShowHealth(void) const noexcept { return this->m_is_show_health; }
	inline void setShowHealth(const bool value) noexcept { this->m_is_show_health = value; }

	inline std::uint32_t getUpdVis(void) const noexcept { return this->m_upd_vis; }
	inline void setUpdVis(const std::uint32_t value) noexcept { this->m_upd_vis = value; }

	inline float getMaxRocketDistance(void) const noexcept { return this->m_max_rocket_distance; }
	inline void setMaxRocketDistance(const float value) noexcept
	{
		this->m_max_rocket_distance = value;
	}

	inline float getMaxMinigunDistance(void) const noexcept
	{
		return this->m_max_minigun_distance;
	}
	inline void setMaxMinigunDistance(const float value) noexcept
	{
		this->m_max_minigun_distance = value;
	}

	inline float getMinRocketDistance(void) const noexcept { return this->m_min_rocket_distance; }
	inline void setMinRocketDistance(const float value) noexcept
	{
		this->m_min_rocket_distance = value;
	}

	inline float getMinMinigunDistance(void) const noexcept
	{
		return this->m_min_minigun_distance;
	}
	inline void setMinMinigunDistance(const float value) noexcept
	{
		this->m_min_minigun_distance = value;
	}

	inline float getVelocity(void) const noexcept { return this->m_velocity; }
	inline void setVelocity(const float value) noexcept { this->m_velocity = value; }

	inline bool IsStopFire(void) const noexcept { return this->m_is_stop_fire; }
	inline void setStopFire(const bool value) noexcept { this->m_is_stop_fire = value; }

	inline void clear(void) noexcept
	{
		this->m_is_use_rocket = false;
		this->m_is_show_health = false;
		this->m_is_fire_trail = false;
		this->m_is_engine_sound = false;
		this->m_is_use_minigun = false;
		this->m_is_stop_fire = false;
		this->m_upd_vis = 0;
		this->m_min_rocket_distance = 0.0f;
		this->m_min_minigun_distance = 0.0f;
		this->m_max_rocket_distance = 0.0f;
		this->m_max_minigun_distance = 0.0f;
		this->m_velocity = 0.0f;
		this->m_path_move_name.clear();
		this->m_path_look_name.clear();
		this->m_enemy_name.clear();
		this->m_fire_point_name.clear();
	}

private:
	bool m_is_use_rocket;
	bool m_is_show_health;
	bool m_is_fire_trail;
	bool m_is_engine_sound;
	bool m_is_use_minigun;
	bool m_is_stop_fire;
	std::uint32_t m_upd_vis;
	float m_min_rocket_distance;
	float m_min_minigun_distance;
	float m_max_rocket_distance;
	float m_max_minigun_distance;
	float m_velocity;
	xr_string m_path_move_name;
	xr_string m_path_look_name;
	xr_string m_enemy_name;
	xr_string m_fire_point_name;
};

template<class ComponentType>
struct Script_AllocatorScheme
{
public:
	Script_AllocatorScheme(void) {}
	~Script_AllocatorScheme(void) {}
	Script_AllocatorScheme(const Script_AllocatorScheme&) = delete;
	Script_AllocatorScheme& operator=(const Script_AllocatorScheme&) = delete;
	Script_AllocatorScheme(Script_AllocatorScheme&&) = delete;
	Script_AllocatorScheme& operator=(Script_AllocatorScheme&&) = delete;

	inline void* allocate(const std::uint16_t npc_id) noexcept
	{
		if (this->m_buffer.find(npc_id) != this->m_buffer.end())
		{
			ComponentType& reference = this->m_buffer.at(npc_id);
			reference.clear();
		}

		ComponentType& instance = this->m_buffer[npc_id];

		return static_cast<void*>(&instance);
	}

private:
	xr_map<std::uint16_t, ComponentType> m_buffer;
};

struct Storage_Data
{
private:
    friend class Storage;

public:
    Storage_Data(void) = default;

    class DeathData
    {
    public:
        DeathData(void) : m_killer_id(0) {}
        ~DeathData(void) = default;

        inline const xr_string& getKillerName(void) const noexcept { return this->m_killer_name; }
        inline void setKillerName(const xr_string& name) noexcept
        {
            if (name.empty())
            {
                MESSAGEW("name.empty() == true! You set an empty string");
            }

            this->m_killer_name = name;
        }

        inline std::uint16_t getKillerID(void) const noexcept { return this->m_killer_id; }
        inline void setKillerID(const std::uint16_t ID) noexcept { this->m_killer_id = ID; }

    private:
        std::uint16_t m_killer_id;
        xr_string m_killer_name;
    };

    class HitData
    {
    public:
        HitData(void) : m_is_deadly_hit(false), m_who_id(0), m_bone_index(0) {}
        ~HitData(void) = default;

        inline bool IsDeadlyHit(void) const noexcept { return this->m_is_deadly_hit; }
        inline void setDeadlyHit(const bool value) noexcept { this->m_is_deadly_hit = value; }

        inline std::uint16_t getWhoID(void) const noexcept { return this->m_who_id; }
        inline void setWhoID(const std::uint16_t value) noexcept { this->m_who_id = value; }

        inline std::uint32_t getBoneIndex(void) const noexcept { return this->m_bone_index; }
        inline void setBoneIndex(const std::int16_t value) noexcept { this->m_bone_index; }

    private:
        bool m_is_deadly_hit;
        std::uint16_t m_who_id;
        std::int16_t m_bone_index;
    };

    ~Storage_Data(void)
    {
        this->m_p_client_object = nullptr;
        this->m_p_server_object = nullptr;
        this->m_p_sound_object = nullptr; // @ Lord: нужно ли удалять?

        if (this->m_p_ini)
        {
            MESSAGEI("deleting CScriptIniFile");

            if (this->m_is_allocated_ini)
                xr_delete(this->m_p_ini);
            else
                this->m_p_ini = nullptr;
        }

        if (this->m_p_state_manager)
        {
            MESSAGEI("deleting m_p_state_manager");
            xr_delete(this->m_p_state_manager);
        }

        if (this->m_p_move_manager)
        {
            MESSAGEI("deleting m_p_move_manager");
            xr_delete(this->m_p_move_manager);
        }

        // Lord: проверить деаллокацию
/*
        for (std::pair<const xr_string, Storage_Scheme*>& it : this->m_schemes)
        {
            if (it.second)
            {
                MESSAGEI("Deleting storage_scheme of %d %s %s",
                    it.second->getClientObject()->ID(), it.second->getClientObject()->Name(),
                    it.second->getSchemeName().c_str());
                xr_delete(it.second);
            }
        }*/

        this->m_schemes.clear();
    }
    inline bool IsInvulnerable(void) const noexcept { return this->m_is_invulnerable; }
    inline void setInvulnerable(const bool value) noexcept { this->m_is_invulnerable = value; }
    inline bool IsImmortal(void) const noexcept { return this->m_is_immortal; }
    inline void setImmortal(const bool value) noexcept { this->m_is_immortal = value; }
    inline bool IsMute(void) const noexcept { return this->m_is_mute; }
    inline void setMute(const bool value) noexcept { this->m_is_mute = value; }
    inline bool IsAnimMovement(void) const noexcept { return this->m_is_anim_movement; }
    inline void setAnimMovement(const bool value) noexcept { this->m_is_anim_movement = value; }
    inline bool IsEnabled(void) const noexcept { return this->m_is_enabled; }
    inline void setEnabled(const bool value) noexcept { this->m_is_enabled = value; }

    inline std::uint16_t getEnemyID(void) const noexcept { return this->m_enemy_id; }
    inline void setEnemyID(const std::uint16_t& value) noexcept
    {
        if (value == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("value = std::uint16_t(-1)! You are trying to set an undefined value! No assignment!");
            return;
        }

        this->m_enemy_id = value;
    }

    inline CScriptGameObject* getClientObject(void) const { return this->m_p_client_object; }
    inline void setClientObject(CScriptGameObject* p_client_object)
    {
        if (!p_client_object)
        {
            MESSAGEI("'deleting' object from database -> "
                "%s",
                this->m_p_client_object ? this->m_p_client_object->Name() :
                                          "using undefined object, you must add to database at first!");
        }
        else
        {
            this->m_p_client_object = p_client_object;
            MESSAGEWR("adding object to database -> %s",
                p_client_object->Name());
        }
    }

    inline const StorageAnimpoint_Data& getStorageAnimpoint(void) const noexcept { return this->m_storage_animpoint; }
    inline void setStorageAnimpoint(const StorageAnimpoint_Data& data) noexcept { this->m_storage_animpoint = data; } 

    inline CSE_ALifeObject* getServerObject(void) const { return this->m_p_server_object; }
    inline void setServerObject(CSE_ALifeObject* p_server_object)
    {
        if (!p_server_object)
        {
            MESSAGEWR("p_server_object = null! You are "
                "trying to set an empty object! No assignment!");
            return;
        }

        this->m_p_server_object = p_server_object;
    }

    inline CScriptSound* getSoundObject(void) const { return this->m_p_sound_object; }
    inline void setSoundObject(CScriptSound* p_sound_object)
    {
        if (!p_sound_object)
        {
            MESSAGEWR("p_sound_object = null! You are trying "
                "to set an empty object! No assignment!");
            return;
        }

        this->m_p_sound_object = p_sound_object;
    }

    inline CScriptIniFile* getIni(void) const { return this->m_p_ini; }
    inline void setIni(CScriptIniFile* p_ini, const bool is_allocated)
    {
        if (!p_ini)
        {
            MESSAGEWR("p_ini = null! You are trying to set an empty "
                "object! No assignment!");
            return;
        }
        this->m_is_allocated_ini = is_allocated;
        this->m_p_ini = p_ini;
    }

    /*    inline void ResetSignals(void) { this->m_data.clear(); }*/

    // @ Gets signals xr_map<xr_string, bool>
    /*
        inline const SubStorage_Data& operator[](const xr_string& id) { return m_data[id]; }

        inline const xr_map<xr_string, SubStorage_Data>& getData(void) const noexcept { return this->m_data; }

        inline void setData(const xr_map<xr_string, SubStorage_Data>& map)
        {
            if (!map.size())
            {
                Msg("[DataBase/Storage_Data/setData(map)] WARNING: map.size() = 0! You are trying to set an empty map!
       No " "assignment!"); return;
            }

            this->m_data = map;
        }

        inline void setData(const std::pair<xr_string, SubStorage_Data>& pair)
        {
            if (!pair.first.size())
            {
                Msg("[DataBase/Storage_Data/setData(pair)] WARNING: pair.first.size() = 0! You are trying to set an
       empty " "string! No assignment!"); return;
            }

            this->m_data.insert(pair);
        }

        inline void setData(const xr_string& id_name, const SubStorage_Data& data)
        {
            if (!id_name.size())
            {
                Msg("[DataBase/Storage_Data/setData(id_name, data)] WARNING: id_name.size() = 0! You are trying to set
       an " "empty string! No assignment!"); return;
            }

            this->m_data[id_name] = data;
        }*/

    inline const xr_map<xr_string, PStor_Data>& getPStor(void) const noexcept { return this->m_pstor; }

    inline void setPStor(const xr_map<xr_string, PStor_Data>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage_Data/setPStor(map)] WARNING: map.size() = 0! You are trying to set an empty map! No "
                "assignment!");
            return;
        }

        this->m_pstor = map;
    }

    inline void setPStor(const std::pair<xr_string, PStor_Data>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[DataBase/Storage_Data/setPStor(pair)] WARNING: pair.first.size() = 0! You are trying to set an empty "
                "pair! No assignment!");
            return;
        }

        this->m_pstor.insert(pair);
    }

    inline void setPStor(const xr_string& id_name, const PStor_Data& data) noexcept
    {
        if (!id_name.size())
        {
            MESSAGEWR("id_name.size() = 0! You are trying to set an "
                "empty string! No assignment!");
            return;
        }

        this->m_pstor[id_name] = data;
    }

    inline const xr_string& getActiveSchemeName(void) const noexcept { return this->m_active_scheme_name; }
    inline void setActiveSchemeName(const xr_string& scheme_name) noexcept { this->m_active_scheme_name = scheme_name; }

    inline const xr_string& getActiveSectionName(void) const noexcept { return this->m_active_section_name; }
    inline void setActiveSectionName(const xr_string& section_name) noexcept
    {
        this->m_active_section_name = section_name;
    }

    inline const xr_string& getSoundName(void) const noexcept { return this->m_sound_name; }
    inline void setSoundName(const xr_string& sound_name) noexcept { this->m_sound_name = sound_name; }

    inline const xr_string& getAnimationName(void) const noexcept { return this->m_animation_name; }
    inline void setAnimationName(const xr_string& animation_name) noexcept { this->m_animation_name = animation_name; }

    inline const xr_string& getAnimationHeadName(void) const noexcept { return this->m_animation_head_name; }
    inline void setAnimationHeadName(const xr_string& animation_head_name) noexcept
    {
        this->m_animation_head_name = animation_head_name;
    }

    inline const xr_string& getTipName(void) const noexcept { return this->m_tip_name; }
    inline void setTipName(const xr_string& tip_name) noexcept { this->m_tip_name = tip_name; }

    inline const xr_string& getTimeName(void) const noexcept { return this->m_time_name; }
    inline void setTimeName(const xr_string& time_name) noexcept { this->m_time_name = time_name; }

    inline const xr_string& getJobIniName(void) const noexcept { return this->m_job_ini_name; }
    inline void setJobIniName(const xr_string& job_ini_name) noexcept { this->m_job_ini_name = job_ini_name; }

    inline const xr_string& getLoadedInifilename(void) const noexcept { return this->m_loaded_ini_filename; }
    inline void setLoadedInifilename(const xr_string& filename) noexcept { this->m_loaded_ini_filename = filename; }

    inline const xr_string& getLoadedSectionLogicName(void) const noexcept { return this->m_loaded_section_logic_name; }
    inline void setLoadedSectionLogicName(const xr_string& logic_name) noexcept
    {
        this->m_loaded_section_logic_name = logic_name;
    }

    inline const xr_string& getLoadedActiveSectionName(void) const noexcept
    {
        return this->m_loaded_active_section_name;
    }
    inline void setLoadedActiveSectionName(const xr_string& section_name) noexcept
    {
        this->m_loaded_active_section_name = section_name;
    }

    inline const xr_string& getLoadedGulagName(void) const noexcept { return this->m_loaded_gulag_name; }
    inline void setLoadedGulagName(const xr_string& gulag_name) noexcept { this->m_loaded_gulag_name = gulag_name; }

    inline const xr_string& getInifilename(void) const noexcept { return this->m_ini_filename; }
    inline void setInifilename(const xr_string& filename) noexcept { this->m_ini_filename = filename; }

    inline const xr_string& getSectionLogicName(void) const noexcept { return this->m_section_logic_name; }
    inline void setSectionLogicName(const xr_string& section_logic_name) noexcept
    {
        this->m_section_logic_name = section_logic_name;
    }

    inline const xr_string& getGulagName(void) const noexcept { return this->m_gulag_name; }
    inline void setGulagName(const xr_string& gulag_name) noexcept { this->m_gulag_name = gulag_name; }

    inline const xrTime& getActivationGameTime(void) const noexcept { return this->m_activation_game_time; }
    inline void setActivationGameTime(const xrTime& time) noexcept { this->m_activation_game_time = time; }

    inline const std::int32_t getActivationTime(void) const noexcept { return this->m_activation_time; }
    inline void setActivationTime(const std::int32_t value) noexcept { this->m_activation_time = value; }

    inline const xr_map<xr_string, Script_IComponentScheme*>& getSchemes(void) const noexcept { return this->m_schemes; }

/*
    inline void setScheme(const xr_map<xr_string, void*>& map) noexcept
    {
        if (map.empty())
        {
            MESSAGEWR("map.empty() == true! Can't assign an empty "
                "map!");
            return;
        }

        this->m_schemes = map;
    }

    inline void setScheme(const std::pair<xr_string, void*>& pair) noexcept
    {
        if (pair.first.empty())
        {
            MESSAGEWR("pair.first.empty() == true! Can't assign an "
                "empty string");
            return;
        }

        this->m_schemes.insert(pair);
    }

    inline void setScheme(const xr_string& scheme_name, void* const data) noexcept
    {
        if (scheme_name.empty())
        {
            MESSAGEWR("scheme_name.empty() == true! "
                "Can't "
                "assign return ...");
            return;
        }

        this->m_schemes[scheme_name] = data;
    }*/

     inline void setSchemesActions(const xr_string& scheme_name, Script_ISchemeEntity* p_scheme)
    {
        if (scheme_name.empty())
        {
            MESSAGEWR("scheme_name.empty() "
                "== true! Can't assign");
            return;
        }

        this->m_schemes[scheme_name]->addAction(p_scheme);
    }

    inline void setSchemesSectionName(const xr_string& scheme_name, const xr_string& section_name) noexcept
    {
        if (scheme_name.empty())
        {
            MESSAGEWR("scheme_name.empty() == true! Can't assign");
            return;
        }

        this->m_schemes[scheme_name]->setLogicName(section_name);
    }

    inline void setSchemesEnabled(const xr_string& scheme_name, const bool value) noexcept
    {
        if (scheme_name.empty())
        {
            MESSAGEWR("scheme_name.empty() == "
                "true! Can't assign");
            return;
        }

        this->m_schemes[scheme_name]->setEnabled(value);
    } 

    inline const DeathData& getDeathData(void) const noexcept { return this->m_death; }
    inline void setDeathData(const DeathData& data) noexcept { this->m_death = data; }
    inline void setDeathDataKillerName(const xr_string& name) noexcept { this->m_death.setKillerName(name); }
    inline void setDeathDataKillerID(const std::uint16_t npc_id) noexcept { this->m_death.setKillerID(npc_id); }

    inline const Data_Overrides& getOverrides(void) const noexcept { return this->m_overrides; }
    inline void setOverrides(const Data_Overrides& data) noexcept { this->m_overrides = data; }

    inline std::uint8_t getSchemeType(void) const noexcept { return this->m_scheme_type; }
    inline void setSchemeType(const std::uint8_t stype) noexcept { this->m_scheme_type = stype; }

    inline const HitData& getHit(void) const noexcept { return this->m_hit; }
    inline void setHitDeadlyHit(const bool value) noexcept { this->m_hit.setDeadlyHit(value); }
    inline void setHitWhoID(const std::uint16_t value) noexcept { this->m_hit.setWhoID(value); }
    inline void setHitBoneIndex(const std::int16_t value) noexcept { this->m_hit.setBoneIndex(value); }

    inline std::uint16_t getCorpseAlreadySelected(void) const noexcept { return this->m_corpse_already_selected; }
    inline void setCorpseAlreadySelected(const std::uint16_t value) noexcept
    {
        this->m_corpse_already_selected = value;
    }

    inline std::uint16_t getWoundedAlreadySelected(void) const noexcept { return this->m_wounded_already_selected; }
    inline void setWoundedAlreadySelected(const std::uint16_t value) noexcept
    {
        this->m_wounded_already_selected = value;
    }

    inline bool IsDangerFlag(void) const noexcept { return this->m_is_danger_flag; }
    inline void setDangerFlag(const bool value) noexcept { this->m_is_danger_flag = value; }

    inline Script_StateManager* const getStateManager(void) const noexcept { return this->m_p_state_manager; }
    inline void setStateManager(Script_StateManager* const p_state_manager)
    {
        if (!p_state_manager)
        {
            MESSAGEWR("you can't set nullptr "
                "for object");
            return;
        }

        this->m_p_state_manager = p_state_manager;
    }

    inline void setStateManagerSetState(const xr_string& state_name, StateManagerCallbackData& callback,
        const std::uint32_t timeout, std::pair<Fvector, CScriptGameObject* const> target,
        const StateManagerExtraData& extra)
    {
        this->m_p_state_manager->set_state(state_name, callback, timeout, target, extra);
    }

    inline Script_MoveManager* const getMoveManager(void) const noexcept { return this->m_p_move_manager; }

    inline void setMoveManager(Script_MoveManager* const p_move_manager)
    {
        if (!p_move_manager)
        {
            MESSAGEW("p_move_manager == nullptr! "
                "You setan empty object is not right ... Return");
            return;
        }

        this->m_p_move_manager = p_move_manager;
    }

    inline void setClientEnemy(CScriptGameObject* const p_enemy)
    {
        if (!p_enemy)
        {
            MESSAGEW("p_enemy == nullptr! You set an empty "
                "object!");
        }

        this->m_p_client_enemy = p_enemy;
    }

    inline CScriptGameObject* const getClientEnemy(void) const noexcept { return this->m_p_client_enemy; }

    inline const xr_string& getScriptCombatTypeName(void) const noexcept { return this->m_script_combat_type_name; }
    inline void setScriptCombatTypeName(const xr_string& type_name) noexcept
    {
        if (type_name.empty())
        {
            MESSAGEW("type_name.empty() == "
                "true! You are set an empty string!");
        }

        this->m_script_combat_type_name = type_name;
    }

    inline std::uint32_t getDisableInputIdle(void) const noexcept { return this->m_disable_input_idle; }
    inline void setDisableInputIdle(const std::uint32_t value) noexcept { this->m_disable_input_idle = value; }

    inline const xrTime& getDisableInputTime(void) const noexcept { return this->m_disable_input_time; }
    inline void setDisableInputTime(const xrTime& time) noexcept { this->m_disable_input_time = time; }

    inline std::uint16_t getRegisteredCamp(void) const noexcept { return this->m_registered_camp; }
    inline void setRegisteredCamp(const std::uint16_t id) noexcept { this->m_registered_camp = id; }

private:
    bool m_is_invulnerable = false;
    bool m_is_immortal = false;
    bool m_is_mute = false;
    bool m_is_enabled = false;
    bool m_is_anim_movement = false;
    bool m_is_allocated_ini = false;
    bool m_is_danger_flag = false;
    std::uint8_t m_scheme_type = 0;
    std::uint16_t m_enemy_id = Globals::kUnsignedInt16Undefined;
    std::uint16_t m_corpse_already_selected = 0;
    std::uint16_t m_wounded_already_selected = 0;
    std::uint16_t m_registered_camp = 0;
    std::int32_t m_activation_time = 0;
    std::uint32_t m_disable_input_idle;
    xrTime m_disable_input_time;
    HitData m_hit;
    CScriptGameObject* m_p_client_object = nullptr;
    CScriptGameObject* m_p_client_enemy = nullptr;
    StorageAnimpoint_Data m_storage_animpoint;
    CSE_ALifeObject* m_p_server_object = nullptr;
    CScriptSound* m_p_sound_object = nullptr;
    CScriptIniFile* m_p_ini = nullptr;
    Script_StateManager* m_p_state_manager = nullptr;
    Script_MoveManager* m_p_move_manager = nullptr;
    xrTime m_activation_game_time;
    /*    xr_map<xr_string, SubStorage_Data> m_data;*/
    xr_map<xr_string, PStor_Data> m_pstor;
    xr_map<xr_string, Script_IComponentScheme*> m_schemes;
    xr_string m_active_scheme_name;
    xr_string m_active_section_name;
    xr_string m_sound_name;
    xr_string m_animation_name;
    xr_string m_animation_head_name;
    xr_string m_tip_name;
    xr_string m_time_name;
    xr_string m_job_ini_name;
    xr_string m_loaded_ini_filename; // @ Idk but this doesn't use well as planned, CUZ IT'S GSC
    xr_string m_loaded_section_logic_name; // @ Idk but this doesn't use well as planned, CUZ IT'S GSC
    xr_string m_loaded_active_section_name; // @ Idk but this doesn't use well as planned, CUZ IT'S GSC
    xr_string m_loaded_gulag_name;
    xr_string m_ini_filename;
    xr_string m_section_logic_name;
    xr_string m_gulag_name;
    xr_string m_script_combat_type_name;
    DeathData m_death;
    Data_Overrides m_overrides;
};

class Storage
{
private:
    Storage(void) = default;

public:
    inline static Storage& getInstance(void) noexcept
    {
        static Storage instance;
        return instance;
    }

    // Lord: или переместить в другой метод! Потестить
    ~Storage(void)
    {
        // @ Lord: подумать здесь нужно это удалять так или оно в другом месте?
        for (xr_map<std::uint16_t, Storage_Data>::value_type& it : this->m_storage)
        {
            if (it.second.getStorageAnimpoint().getAnimpoint())
            {
                MESSAGEI("Deleting: Animpoint -> [%s]",
                    it.second.getStorageAnimpoint().getCoverName().c_str());
                Script_Animpoint* instance = it.second.getStorageAnimpoint().getAnimpoint();
                delete instance;
                instance = nullptr;
            }

            /*
                        for (const xr_map<xr_string, SubStorage_Data>::value_type& object : it.second.getData())
                        {
                            if (object.second.getActions().size())
                            {
                                for (Script_ILogicEntity* entity : object.second.getActions())
                                {
                                    if (entity)
                                    {
                                        Msg("[Scripts/DataBase/Storage/~dtor] Deleting Script_IEntity: %s",
                                            entity->m_logic_name.c_str());
                                        delete entity;
                                        entity = nullptr;
                                    }
                                }
                            }
                        }*/

            /* Lord: удалить данный комментарий!
                        if (it.second.getClientObject())
                        {
                            CScriptGameObject* p_client_object = it.second.getClientObject();
                            Msg("[Scripts/DataBase/Storage/~dtor] Deleting the m_object: %s", p_client_object->Name());
                            delete p_client_object;
                            p_client_object = nullptr;
                        }

                        if (it.second.getServerObject())
                        {
                            CSE_ALifeObject* p_server_object = it.second.getServerObject();
                            Msg("[Scripts/DataBase/Storage/~dtor] Deleting the m_server_object: %s",
               p_server_object->name()); delete p_server_object; p_server_object = nullptr;
                        }*/

            if (it.second.getIni())
            {
                CInifile* p_ini = it.second.getIni();
                MESSAGEI("Delete the m_ini: %s", p_ini->fname());
                delete p_ini;
                p_ini = nullptr;
            }

            if (it.second.getSoundObject())
            {
                CScriptSound* p_sound = it.second.getSoundObject();
                MESSAGEI("Deleting the m_sound_object");
                delete p_sound;
                p_sound = nullptr;
            }
        }

        this->m_storage.clear();
    }

#pragma region Getters
    inline const xr_map<std::uint16_t, Storage_Data>& getStorage(void) const noexcept { return this->m_storage; }

    inline const xr_map<xr_string, xr_map<std::uint16_t, CZoneCampfire*>>& getCampfiresBySmartName(void) const noexcept { return this->m_campfires_by_smart_name; }
    inline void setCampfiresBySmartName(const xr_string& smart_name, const std::uint16_t object_id, CZoneCampfire* const p_zone) 
    {
        if (smart_name.empty())
        {
            MESSAGEWR("invalid string!");
            return;
        }

        if (object_id == 0 || object_id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("invalid id!");
            return;
        }

        if (p_zone == nullptr)
        {
            MESSAGEW("you are set an empty object!");
        }

        this->m_campfires_by_smart_name[smart_name][object_id] = p_zone;
    }

    inline Script_CampData* getCampsCamp(const std::uint16_t object_id) const
    {
        if (this->m_camps.find(object_id) == this->m_camps.end())
        {
            MESSAGEWR("can't obtain camp data by id %d", object_id);
            return nullptr;
        }

        return this->m_camps.at(object_id).first;
    }

    inline CScriptGameObject* getCampsObject(const std::uint16_t object_id) const 
    {
        return this->m_camps.at(object_id).second;
    }

    inline const xr_map<std::uint16_t, std::pair<Script_CampData*, CScriptGameObject*>>& getCamps(void) const noexcept { return this->m_camps; } 
    inline void setStorage(const xr_map<std::uint16_t, Storage_Data>& map) noexcept
    {
        if (map.empty())
        {
            MESSAGEWR("map.size() = 0! You are trying to set an empty map! No "
                "assignment!");
            return;
        }

        this->m_storage = map;
    }

    inline void setStorage(const std::pair<std::uint16_t, Storage_Data>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("pair.first = std::uint16_t(-1)! You are trying to set an "
                "undefined variable! No assignment!");
            return;
        }

        this->m_storage.insert(pair);
    }
    inline void setStorage(const std::uint16_t& id, const Storage_Data& data) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("id = std::uint16_t(-1)! You are trying to set an "
                "undefined variable! No assignment!");
            return;
        }

        this->m_storage[id] = data;
    }

    inline CScriptGameObject* getActor(void) const { return this->m_actor; }
    inline CScriptGameObject* getInvetoryUpgradesVictim(void) const { return this->m_p_victim; }

    // Это need_victim() в inventory_upgrades!
    inline void setInventoryUpgradesVictim(CScriptGameObject* const p_object)
    {
        if (!p_object)
        {
            MESSAGEWR("p_object = nullptr!");
            return;
        }

        this->m_p_victim = p_object;
    }

    // Lord: переделать сюда идёт bind_anomaly_zone
    inline const xr_map<xr_string, CScriptGameObject*>& getAnomalyByName(void) const noexcept
    {
        return this->m_anomaly_by_name;
    }

    inline void setAnomalyByName(const xr_map<xr_string, CScriptGameObject*>& map)
    {
        if (!map.size())
        {
            MESSAGEWR("map.size() = 0! You are trying to set empty "
                "map!");
            return;
        }

        this->m_anomaly_by_name = map;
    }

    inline void setAnomalyByName(const std::pair<xr_string, CScriptGameObject*>& pair)
    {
        if (!pair.first.size())
        {
            MESSAGEWR("pair.first.size() = 0! You are trying to "
                "set an empty string!");
            return;
        }

        if (!pair.second)
        {
            MESSAGEWR("pair.second = nullptr! You are trying to "
                "set an empty object!");
            return;
        }

        this->m_anomaly_by_name.insert(pair);
    }

    inline void setAnomalyByName(const xr_string& anomaly_name, CScriptGameObject* object)
    {
        if (anomaly_name.empty())
        {
            MESSAGEWR("anomaly_name.size() = 0! "
                "You are trying to set an empty string!");
            return;
        }

        if (!object)
        {
            MESSAGEWR("object = nullptr! You are "
                "trying to set an empty object!");
            return;
        }

        this->m_anomaly_by_name[anomaly_name] = object;
    }

    inline const xr_map<xr_string, CScriptGameObject*>& getZoneByName(void) const noexcept
    {
        return this->m_zone_by_name;
    }

    inline void setZoneByName(const xr_map<xr_string, CScriptGameObject*>& map)
    {
        if (map.empty())
        {
            MESSAGEWR("map.size() = 0! You are trying to set an empty map! No "
                "assignment!");
            return;
        }

        this->m_zone_by_name = map;
    }

    inline void setZoneByName(const std::pair<xr_string, CScriptGameObject*>& pair)
    {
        if (pair.first.empty())
        {
            MESSAGEWR("pair.first.size() = 0! You are "
                "trying to set an empty string! No assignment!");
            return;
        }

        if (!pair.second)
        {
            MESSAGEWR("pair.second = null! You are "
                "trying to set an empty object! No assignment!");
            return;
        }

        this->m_zone_by_name.insert(pair);
    }

    inline void setZoneByName(const xr_string& zone_name, CScriptGameObject* p_client_zone)
    {
        if (zone_name.empty())
        {
            MESSAGEWR("zone_name.size() = 0! You are "
                "trying to set an empty string! No assignment!");
            return;
        }

        if (!p_client_zone)
        {
            MESSAGEWR("p_client_zone = null! You are "
                "trying to set an empty object! No assignment!");
            return;
        }

        this->m_zone_by_name[zone_name] = p_client_zone;
    }

    inline const xr_map<std::uint16_t, float>& getGoodwill_Sympathy(void) const noexcept
    {
        return this->m_goodwill.first;
    }

    inline void setGoodwill_Sympathy(const xr_map<std::uint16_t, float>& first) noexcept
    {
        if (first.empty())
        {
            MESSAGEWR("first.size() = 0! You are trying to "
                "set an "
                "empty map! No assignment!");
            return;
        }

        this->m_goodwill.first = first;
    }

    inline void setGoodwill_Sympathy(const std::pair<std::uint16_t, float>& pair_first) noexcept
    {
        if (pair_first.first == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("pair_first.first = std::uint16_t(-1)! "
                "You are "
                "trying to set an undefined value! No assignment!");
            return;
        }

        this->m_goodwill.first.insert(pair_first);
    }

    inline void setGoodwill_Sympathy(const std::uint16_t& id, const float& value) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("id = std::uint16_t(-1)! You are trying to "
                "set an "
                "undefined value! No assignment!");
            return;
        }

        this->m_goodwill.first[id] = value;
    }

    inline const xr_map<std::uint16_t, xr_string>& getGoodwill_Relations(void) const noexcept
    {
        return this->m_goodwill.second;
    }

    inline void setGoodwill_Relations(const xr_map<std::uint16_t, xr_string>& second) noexcept
    {
        if (second.empty())
        {
            MESSAGEWR("second.size() = 0! You are trying to "
                "set an "
                "empty map! No assignment!");
            return;
        }

        this->m_goodwill.second = second;
    }

    inline void setGoodwill_Relations(const std::pair<std::uint16_t, xr_string>& pair_second) noexcept
    {
        if (pair_second.first == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("[DataBase/Storage/setGoodwill_Relations(pair_second)] WARNING: pair_second.first = std::uint16_t(-1)! "
                "You are "
                "trying to set an undefined value! No assignment!");
            return;
        }

        this->m_goodwill.second.insert(pair_second);
    }

    inline void setGoodwill_Relations(const std::uint16_t& id, const xr_string& string) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("id = std::uint16_t(-1)! You are trying "
                "to set an "
                "undefined value! No assignment!");
            return;
        }

        this->m_goodwill.second[id] = string;
    }

    inline const xr_map<std::uint16_t, std::uint32_t>& getSpawnedVertexByID(void) const noexcept
    {
        return this->m_spawned_vertex_by_id;
    }

    inline void setSpawnedVertexByID(const xr_map<std::uint16_t, std::uint32_t>& map) noexcept
    {
        if (map.empty())
        {
            MESSAGEWR("map.size() = 0! You are trying to set an empty "
                "map! No assignment!");
            return;
        }

        this->m_spawned_vertex_by_id = map;
    }

    inline void setSpawnedVertexByID(const std::pair<std::uint16_t, std::uint32_t>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("pair.first = std::uint16_t(-1)! You are trying "
                "to set an undefined value! No assignment!");
            return;
        }

        this->m_spawned_vertex_by_id.insert(pair);
    }

    inline void setSpawnedVertexByID(const std::uint16_t& id, const std::uint32_t& value) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("id = std::uint16_t(-1)! You are trying to "
                "set an undefined value! No assignment!");
            return;
        }

        this->m_spawned_vertex_by_id[id] = value;
    }

    inline const xr_map<std::uint16_t, std::pair<std::uint16_t, xr_string>>& getOfflineObjects(void) const noexcept
    {
        return this->m_offline_objects;
    }

    /*
        inline void ClearOfflineObjects_Second(const std::uint16_t& id) noexcept
        {
            if (id == Globals::kUnsignedInt16Undefined)
            {
                Msg("[DataBase/Storage/ClearOfflineObjects_Second(id)] WARNING: id = std::uint16_t(-1)! You are trying
       to get access through an undefined value! NO assignment!"); return;
            }

            this->m_offline_objects[id].second.clear();
        }*/

    inline void setOfflineObjects(const xr_map<std::uint16_t, std::pair<std::uint16_t, xr_string>>& map) noexcept
    {
        if (map.empty())
        {
            MESSAGEW("map.size() = 0! You are trying to set an empty "
                "map!");
            //  return;
        }

        this->m_offline_objects = map;
    }

    inline void setOfflineObjects(const std::pair<std::uint16_t, std::pair<std::uint16_t, xr_string>>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEW("pair.first = std::uint16_t(-1)! You are trying to "
                "set "
                "an empty pair!");
            //   return;
        }

        this->m_offline_objects.insert(pair);
    }

    inline void setOfflineObjects(const std::uint16_t& id, const std::pair<std::uint16_t, xr_string>& pair) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEW("id = std::uint16_t(-1)! You are trying to set "
                "an undefined value!");
            //   return;
        }

        this->m_offline_objects[id] = pair;
    }

    // @ Lord: наименовать нормально аргументы
    inline void setOfflineObjects(const std::uint16_t& id, const std::uint16_t& _id, const xr_string& string) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEW("id = std::uint16_t(-1)! You are trying "
                "to set an undefined value!");
            //    return;
        }

        this->m_offline_objects[id] = {_id, string};
    }

    inline void setOfflineObjects(const std::uint16_t& id, const std::uint16_t& _id) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEW("id = std::uint16_t(-1)! You are trying "
                "to set an undefined value!");
            //  return;
        }

        this->m_offline_objects[id].first = _id;
    }

    inline void setOfflineObjects(const std::uint16_t& id, const xr_string& string) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEW("id = std::uint16_t(-1)! You are trying "
                "to set an undefined value!");
            //  return;
        }

        this->m_offline_objects[id].second = string;
    }

    inline const std::pair<xr_map<std::uint16_t, float>, xr_map<std::uint16_t, xr_string>>& getGoodwill(void) const
        noexcept
    {
        return this->m_goodwill;
    }

    inline void setGoodwill(
        const xr_map<std::uint16_t, float>& first, const xr_map<std::uint16_t, xr_string>& second) noexcept
    {
        if (first.empty())
        {
            MESSAGEWR("first.size() = 0! You are trying to set an "
                "empty map! No assignment!");
            return;
        }

        if (second.empty())
        {
            MESSAGEWR("second.size() = 0! You are trying to set an "
                "empty map! No assignment!");
            return;
        }

        this->m_goodwill.first = first;
        this->m_goodwill.second = second;
    }
#pragma endregion

#pragma region Cordis Setters
#pragma region Cordis DataBase Storage_Data setters
 
    inline void UnsubscribeAction(const std::uint16_t npc_id, const xr_string& scheme_name) noexcept
    {
        if (scheme_name.empty())
        {
            MESSAGEWR("scheme_name.empty() == "
                "true!");
            return;
        }

        if (this->m_storage[npc_id].getSchemes().find(scheme_name) == this->m_storage[npc_id].getSchemes().end())
        {
            MESSAGEWR("can't find scheme by "
                "scheme_name %s",
                scheme_name.c_str());
            return;
        }

        this->m_storage[npc_id].m_schemes[scheme_name]->UnSubscribeAction();
    } 

    void setStorageStateManager(CScriptGameObject* const p_client_object, Script_StateManager* const p_state_manager);
    void setStorageMoveManager(CScriptGameObject* const p_client_object, Script_MoveManager* const p_move_manager);

    inline void setStorageCampsCamp(const std::uint16_t object_id, Script_CampData* p_camp) 
    {
        if (!p_camp)
           MESSAGEW("p_camp == nullptr! You are trying to set a null value");

        this->m_camps[object_id].first = p_camp;
    }

    inline void setStorageCampsObject(const std::uint16_t object_id, CScriptGameObject* const p_client_object)
    {
        if (!p_client_object)
            MESSAGEW("p_client_object == nullptr! You are trying to set a null value!");

        this->m_camps[object_id].second = p_client_object;
    }

    inline void setStorageDisableInputIdle(const std::uint16_t npc_id, const std::uint32_t value) noexcept
    {
        this->m_storage[npc_id].setDisableInputIdle(value);
    }

    inline void setStorageDisableInputTime(const std::uint16_t npc_id, const xrTime& time) noexcept
    {
        this->m_storage[npc_id].setDisableInputTime(time);
    }

    inline void setStorageScriptCombatTypeName(const std::uint16_t npc_id, const xr_string& type_name)
    {
        this->m_storage[npc_id].setScriptCombatTypeName(type_name);
    }

    inline void setStorageEnemy(const std::uint16_t npc_id, CScriptGameObject* const p_enemy)
    {
        this->m_storage[npc_id].setClientEnemy(p_enemy);
    }

    inline void setStorageStateManagerSetState(const std::uint16_t npc_id, const xr_string& state_name,
        StateManagerCallbackData& callback, const std::uint32_t timeout,
        std::pair<Fvector, CScriptGameObject* const> target, const StateManagerExtraData& extra) noexcept
    {
        this->m_storage[npc_id].setStateManagerSetState(state_name, callback, timeout, target, extra);
    }

    // Lord: обновлять сеттеры покуда обновляется сам Storage_Data
    inline void setStorageInvulnerable(const std::uint16_t npc_id, const bool value) noexcept
    {
        this->m_storage[npc_id].setInvulnerable(value);
    }

    inline void setStorageImmortal(const std::uint16_t npc_id, const bool value) noexcept
    {
        this->m_storage[npc_id].setImmortal(value);
    }

    inline void setStorageMute(const std::uint16_t npc_id, const bool value) noexcept
    {
        this->m_storage[npc_id].setMute(value);
    }

    inline void setStorageEnabled(const std::uint16_t npc_id, const bool value) noexcept
    {
        this->m_storage[npc_id].setEnabled(value);
    }

    inline void setStorageAnimMovement(const std::uint16_t npc_id, const bool value) noexcept
    {
        this->m_storage[npc_id].setAnimMovement(value);
    }

    inline void setStorageEnemyID(const std::uint16_t npc_id, const std::uint16_t enemy_id) noexcept
    {
        this->m_storage[npc_id].setEnemyID(enemy_id);
    }

    inline void setStorageAnimpointStorage(const std::uint16_t npc_id, const StorageAnimpoint_Data& data) noexcept
    {
        this->m_storage[npc_id].setStorageAnimpoint(data);
    }

    inline void setStorageServerObject(const std::uint16_t npc_id, CSE_ALifeObject* server_object) noexcept
    {
        this->m_storage[npc_id].setServerObject(server_object);
    }

    inline void setStorageSoundObject(const std::uint16_t npc_id, CScriptSound* p_sound_object) noexcept
    {
        this->m_storage[npc_id].setSoundObject(p_sound_object);
    }

    inline void setStorageIniFile(const std::uint16_t npc_id, CScriptIniFile* ini, const bool is_allocated) noexcept
    {
        this->m_storage[npc_id].setIni(ini, is_allocated);
    }


    inline void setStorageSignal(const std::uint16_t id, const xr_string& signal_name, const bool value) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("id = std::uint16_t(-1)! You are trying to get access through an undefined value! No assignment!");
            return;
        }

        this->m_storage[id].m_schemes[this->m_storage[id].getActiveSchemeName()]->setSignals(signal_name, value);
    }

    inline void setStorageActiveSchemeName(const std::uint16_t npc_id, const xr_string& active_scheme_name) noexcept
    {
        this->m_storage[npc_id].setActiveSchemeName(active_scheme_name);
    }

    inline void setStorageActiveSectionName(const std::uint16_t npc_id, const xr_string& active_section_name) noexcept
    {
        this->m_storage[npc_id].setActiveSectionName(active_section_name);
    }

    inline void setStorageSoundName(const std::uint16_t npc_id, const xr_string& sound_name) noexcept
    {
        this->m_storage[npc_id].setSoundName(sound_name);
    }

    inline void setStorageAnimationName(const std::uint16_t npc_id, const xr_string& animation_name) noexcept
    {
        this->m_storage[npc_id].setAnimationName(animation_name);
    }

    inline void setStorageAnimationHeadName(const std::uint16_t npc_id, const xr_string& animation_head_name) noexcept
    {
        this->m_storage[npc_id].setAnimationHeadName(animation_head_name);
    }

    inline void setStorageTipName(const std::uint16_t npc_id, const xr_string& tip_name) noexcept
    {
        this->m_storage[npc_id].setTipName(tip_name);
    }

    inline void setStorageTimeName(const std::uint16_t npc_id, const xr_string& time_name) noexcept
    {
        this->m_storage[npc_id].setTimeName(time_name);
    }

    inline void setStorageJobIniName(const std::uint16_t npc_id, const xr_string& job_ini_name) noexcept
    {
        this->m_storage[npc_id].setJobIniName(job_ini_name);
    }

    inline void setStorageLoadedInifilename(const std::uint16_t npc_id, const xr_string& filename) noexcept
    {
        this->m_storage[npc_id].setLoadedInifilename(filename);
    }

    inline void setStorageLoadedSectionLogicName(const std::uint16_t npc_id, const xr_string& logic_name) noexcept
    {
        this->m_storage[npc_id].setLoadedSectionLogicName(logic_name);
    }

    inline void setStorageLoadedActiveSectionName(
        const std::uint16_t npc_id, const xr_string& active_section_name) noexcept
    {
        this->m_storage[npc_id].setLoadedActiveSectionName(active_section_name);
    }

    inline void setStorageLoadedGulagName(const std::uint16_t npc_id, const xr_string& gulag_name) noexcept
    {
        this->m_storage[npc_id].setLoadedGulagName(gulag_name);
    }

    inline void setStorageActivationGameTime(const std::uint16_t npc_id, const xrTime& time) noexcept
    {
        this->m_storage[npc_id].setActivationGameTime(time);
    }

    inline void setStorageActivationTime(const std::uint16_t npc_id, const std::int32_t value) noexcept
    {
        this->m_storage[npc_id].setActivationTime(value);
    }

    inline void setStorageInifilename(const std::uint16_t npc_id, const xr_string& filename) noexcept
    {
        this->m_storage[npc_id].setInifilename(filename);
    }

    inline void setStorageSectionLogicName(const std::uint16_t npc_id, const xr_string& section_logic_name) noexcept
    {
        this->m_storage[npc_id].setSectionLogicName(section_logic_name);
    }

    inline void setStorageGulagName(const std::uint16_t npc_id, const xr_string& gulag_name) noexcept
    {
        this->m_storage[npc_id].setGulagName(gulag_name);
    }

 
/*
    inline void setStorageSchemes(const std::uint16_t npc_id, const xr_map<xr_string, void*>& map) noexcept
    {
        if (map.empty())
        {
            MESSAGEWR("can't assign an empty map!");
            return;
        }

        this->m_storage[npc_id].setScheme(map);
    } */

/*
    inline void setStorageScheme(const std::uint16_t npc_id, const std::pair<xr_string, void*>& pair) noexcept
    {
        if (pair.first.empty())
        {
            MESSAGEWR("pair.first.empty() == true! Can't assign");
            return;
        }

        this->m_storage[npc_id].setScheme(pair);
    }*/

/*
    inline void setStorageScheme(
        const std::uint16_t npc_id, const xr_string& scheme_name, void* const data) noexcept
    {
        if (scheme_name.empty())
        {
            MESSAGEWR("scheme_name.empty() == true! Can't assign");
            return;
        }

        MESSAGEI("Registered scheme %s for npc [%d]", scheme_name, npc_id);

        this->m_storage[npc_id].setScheme(scheme_name, data);
    }*/

 
    inline void setStorageSchemesActionSchemeIDForUnSubscribing(
        const std::uint16_t npc_id, const xr_string& scheme_name, const std::uint32_t scheme_id)
    {
        if (scheme_name.empty())
        {
            MESSAGEWR("scheme_name.empty() == true!");
            return;
        }

        if (this->m_storage[npc_id].getSchemes().find(scheme_name) == this->m_storage[npc_id].getSchemes().end())
        {
            MESSAGEWR("can't find scheme %s",
                scheme_name.c_str());
            return;
        }

        this->m_storage[npc_id].m_schemes[scheme_name]->setActionSchemeID(scheme_id);
    } 

 
    inline void setStorageSchemesActions(
        const std::uint16_t npc_id, const xr_string& scheme_name, Script_ISchemeEntity* p_scheme)
    {
        if (scheme_name.empty())
        {
            MESSAGEWR("scheme_name.empty() == true! Can't assign");
            return;
        }
        p_scheme->subscribe_action();
        this->m_storage[npc_id].setSchemesActions(scheme_name, p_scheme);
    } 

/*
    inline void setStorageSchemesSectionName(
        const std::uint16_t npc_id, const xr_string& scheme_name, const xr_string& section_name) noexcept
    {
        if (scheme_name.empty())
        {
            MESSAGEWR("scheme_name.empty() == true! Can't assign");
            return;
        }

        this->m_storage[npc_id].setSchemesSectionName(scheme_name, section_name);
    }*/

 
    inline void setStorageSchemesEnabled(
        const std::uint16_t npc_id, const xr_string& scheme_name, const bool value) noexcept
    {
        if (scheme_name.empty())
        {
            MESSAGEWR("scheme_name.empty() == true! Can't assign");
            return;
        }

        this->m_storage[npc_id].setSchemesEnabled(scheme_name, value);
    } 

    inline void setStorageDeathData(const std::uint16_t npc_id, const Storage_Data::DeathData& data) noexcept
    {
        this->m_storage[npc_id].setDeathData(data);
    }

    // @ sets killer's name
    inline void setStorageDeathDataKillerName(const std::uint16_t npc_id, const xr_string& name) noexcept
    {
        this->m_storage[npc_id].setDeathDataKillerName(name);
    }

    // @ sets killer's id
    inline void setStorageDeathDataKillerID(const std::uint16_t npc_id, const std::uint16_t id) noexcept
    {
        this->m_storage[npc_id].setDeathDataKillerID(id);
    }

    inline void setStorageOverrides(const std::uint16_t npc_id, const Data_Overrides& data) noexcept
    {
        this->m_storage[npc_id].setOverrides(data);
    }

    inline void setStorageSType(const std::uint16_t npc_id, const std::uint8_t stype) noexcept
    {
        this->m_storage[npc_id].setSchemeType(stype);
    }

    inline void setStorageHitDeadlyHit(const std::uint16_t npc_id, const bool value) noexcept
    {
        this->m_storage[npc_id].setHitDeadlyHit(value);
    }

    inline void setStorageHitWhoID(const std::uint16_t npc_id, const std::uint16_t who_id) noexcept
    {
        this->m_storage[npc_id].setHitWhoID(who_id);
    }

    inline void setStorageHitBoneIndex(const std::uint16_t npc_id, const std::int16_t bone_index) noexcept
    {
        this->m_storage[npc_id].setHitBoneIndex(bone_index);
    }

    inline void setStorageCorpseAlreadySelected(const std::uint16_t npc_id, const std::uint16_t corpse_id) noexcept
    {
        this->m_storage[npc_id].setCorpseAlreadySelected(corpse_id);
    }

    inline void setStorageWoundedAlreadySelected(const std::uint16_t npc_id, const std::uint16_t wounded_id) noexcept
    {
        this->m_storage[npc_id].setWoundedAlreadySelected(wounded_id);
    }

    inline void setStorageDangerFlag(const std::uint16_t npc_id, const bool value) noexcept
    {
        this->m_storage[npc_id].setDangerFlag(value);
    }

    inline void setStorageRegisteredCamp(const std::uint16_t npc_id, const std::uint16_t camp_id) noexcept 
    {
        this->m_storage[npc_id].setRegisteredCamp(camp_id);
    }
#pragma endregion

    // @ For helicopters only
    inline void addEnemy(CScriptGameObject* p_object)
    {
        this->m_helicopter_enemies[this->m_helicopter_count] = p_object;
        ++this->m_helicopter_count;
    }

    // @ For helicopters only
    inline void deleteEnemy(const std::uint32_t index)
    {
        this->m_helicopter_enemies[index] = nullptr;
        // Lord: I guess it will be much better if I minus this->m_helicopter_count, cuz the scripts doesn't handle it.
        // It means that the value this->m_helicopter_count only grows up!
    }

    inline void addObject(CScriptGameObject* object)
    {
        if (!object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        this->m_storage[object->ID()].setClientObject(object);
    }

    inline void deleteObject(CScriptGameObject* object)
    {
        if (!object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }


        // Lord: протестировать ибо зануляют!
        this->m_storage.erase(object->ID());
    }

    inline void addAnimationObject(CScriptGameObject* const p_animation_object, Script_Binder_DoorLabx8* const p_binder)
    {
        this->m_animation_object_by_name[p_animation_object->Name()] = p_binder;
        this->addObject(p_animation_object);
    }

    inline void deleteAnimationObject(CScriptGameObject* const p_animation_object)
    {
        this->m_animation_object_by_name[p_animation_object->Name()] = nullptr;
        this->deleteObject(p_animation_object);
    }

    inline void addZone(CScriptGameObject* zone)
    {
        if (!zone)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        this->m_zone_by_name[zone->Name()] = zone;
        MESSAGEI("adding to database %s", zone->Name());
    }

    inline void deleteZone(CScriptGameObject* zone)
    {
        if (!zone)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        this->m_zone_by_name[zone->Name()] = nullptr;
        MESSAGEI("'deleting' from database %s", zone->Name());
    }

    inline void addAnomaly(CScriptGameObject* anomaly)
    {
        if (!anomaly)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        this->m_anomaly_by_name[anomaly->Name()] = anomaly;

        MESSAGEI("adding object to database %s", anomaly->Name());
    }

    inline void deleteAnomaly(CScriptGameObject* anomaly)
    {
        if (!anomaly)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        this->m_anomaly_by_name[anomaly->Name()] = nullptr;

        MESSAGEI("'deleting' object from database %s", anomaly->Name());
    }

    inline void setActor(CScriptGameObject* object)
    {
        if (!object)
        {
            R_ASSERT2(false, "Can't set actor because the object was null!");
            return;
        }

        this->m_actor = object;
        this->addObject(object);
        MESSAGEI("registering actor to database!");
    }

    inline void deleteActor(CScriptGameObject* object)
    {
        this->m_actor = nullptr;
        this->deleteObject(object);

        MESSAGEI("'deleting' actor from database!");
    }

    inline void setPStorBool(const std::uint16_t& id, const xr_string& varname, const bool& value) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("id = std::uint16_t(-1)! You are trying "
                "to get access through an undefined value! No assignment!");
            return;
        }

        if (varname.empty())
        {
            MESSAGEWR("varname.size() = 0! You are trying to "
                "get an access through an empty string! No assignment!");
            return;
        }

        this->m_storage[id].m_pstor[varname].setBool(value);
    }

    inline void setPStorNumber(const std::uint16_t& id, const xr_string& varname, const std::uint32_t value)
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("id = std::uint16_t(-1)! You are trying "
                "to get access through an undefined value! No assignment!");
            return;
        }

        if (varname.empty())
        {
            MESSAGEWR("varname.size() = 0! You are trying to "
                "get an access through an empty string! No assignment!");
            return;
        }

        this->m_storage[id].m_pstor[varname].setNumber(value);
    }

    inline void setPStorString(const std::uint16_t& id, const xr_string& varname, const xr_string& value)
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("id = std::uint16_t(-1)! You are trying "
                "to get an access through an undefined value! No assignment!");
            return;
        }

        if (varname.empty())
        {
            MESSAGEWR("id = std::uint16_t(-1)! You are trying "
                "to get an access through an undefined value! No assignment!");
            return;
        }

        this->m_storage[id].m_pstor[varname].setString(value);
    }

    inline const xr_map<xr_string, Script_SE_SmartTerrain*>& getGameRegisteredServerSmartTerrainsByName(void) const
        noexcept
    {
        return this->m_game_registered_server_smartterrains_by_name;
    }

    inline void setGameRegisteredServerSmartTerrainsByName(const xr_map<xr_string, Script_SE_SmartTerrain*>& map)
    {
        if (map.empty())
        {
            MESSAGEW("map.size() = 0! You are trying to set an empty map!");
            //  return;
        }

        this->m_game_registered_server_smartterrains_by_name = map;
    }

    inline void setGameRegisteredServerSmartTerrainsByName(const std::pair<xr_string, Script_SE_SmartTerrain*>& pair)
    {
        if (pair.first.empty())
        {
            MESSAGEW("pair.first.size() = 0! You are trying to set an empty string!");
            //  return;
        }

        if (!pair.second)
        {
            MESSAGEW("pair.second = null! You are trying to set an empty object!");
            // return;
        }

        this->m_game_registered_server_smartterrains_by_name.insert(pair);
    }

    inline void setGameRegisteredServerSmartTerrainsByName(const xr_string& name, Script_SE_SmartTerrain* server_smart)
    {
        if (name.empty())
        {
            MESSAGEW("name.size() = 0! You are trying to set an empty string!");
            //  return;
        }

        if (!server_smart)
        {
            MESSAGEW("server_smart = null! You are trying to set an empty object!");
            //    return;
        }

        this->m_game_registered_server_smartterrains_by_name[name] = server_smart;
    }

    inline const xr_map<xr_string, CScriptGameObject*>& getSmartCoversByName(void) const noexcept { return this->m_smart_covers_by_name; }
    inline void setSmartCoversByName(const xr_string& name, CScriptGameObject* const p_client_object) noexcept 
    {
        if (name.empty())
        {
            MESSAGEWR("can't set by empty id!");
            return;
        }

        this->m_smart_covers_by_name[name] = p_client_object;
    }

    // @ In-Game
    inline const xr_map<xr_string, Script_SE_SmartCover*>& getGameRegisteredServerSmartCovers(void) const noexcept
    {
        return this->m_game_registered_smartcovers;
    }

    inline void setGameRegisteredServerSmartCovers(const xr_map<xr_string, Script_SE_SmartCover*>& map)
    {
        if (!map.size())
        {
            MESSAGEW("map.size() = 0! You are trying to set an empty map!");
            //  return;
        }

        this->m_game_registered_smartcovers = map;
    }

    inline void setGameRegisteredServerSmartCovers(const std::pair<xr_string, Script_SE_SmartCover*>& pair)
    {
        if (!pair.first.size())
        {
            MESSAGEW("pair.first.size() = 0! You are trying to set an empty string!");
            //  return;
        }

        if (!pair.second)
        {
            MESSAGEW("pair.second = null! You are trying to set an empty object!");
            //    return;
        }

        this->m_game_registered_smartcovers.insert(pair);
    }

    inline void setGameRegisteredServerSmartCovers(const xr_string& name, Script_SE_SmartCover* server_smartcover)
    {
        if (name.empty())
        {
            MESSAGEW("name.size() = 0! You are trying to set an empty string!");
            //   return;
        }

        if (!server_smartcover)
        {
            MESSAGEW("server_smartcover = null! You are trying to set an empty object!");
            // return;
        }

        this->m_game_registered_smartcovers[name] = server_smartcover;
    }

    inline const xr_map<xr_string, CScriptGameObject*>& getGameRegisteredCombatSpaceRestrictors(void) const noexcept
    {
        return this->m_game_registered_combat_spacerestrictors;
    }

    inline void setGameRegisteredCombatSpaceRestrictors(const xr_map<xr_string, CScriptGameObject*>& map)
    {
        if (map.empty())
        {
            MESSAGEW("map.size() = 0! You are trying to set an empty map!");
            //    return;
        }

        this->m_game_registered_combat_spacerestrictors = map;
    }

    inline void setGameRegisteredCombatSpaceRestrictors(const std::pair<xr_string, CScriptGameObject*>& pair)
    {
        if (pair.first.empty())
        {
            MESSAGEW("pair.first.size() = 0! You are trying to set an empty string!");
            //   return;
        }

        if (!pair.second)
        {
            MESSAGEW("pair.second = null! You are trying to set an empty object!");
            //  return;
        }

        this->m_game_registered_combat_spacerestrictors.insert(pair);
    }

    inline void setGameRegisteredCombatSpaceRestrictors(const xr_string& space_name, CScriptGameObject* client_zone)
    {
        if (space_name.empty())
        {
            MESSAGEW("space_name.size() = 0! You are trying to set an empty string!");
            //    return;
        }

        if (!client_zone)
        {
            MESSAGEW("client_zone = null! You are trying to set an empty object!");
            //    return;
        }

        this->m_game_registered_combat_spacerestrictors[space_name] = client_zone;
    }

    // @ In-Game
    inline const xr_map<std::uint8_t, xr_map<std::uint32_t, Script_SE_SmartCover*>>&
    getGameRegisteredServerSmartCoversByLevelID(void) const noexcept
    {
        return this->m_game_registered_smartcovers_by_level_id;
    }

    // Lord: Проверить данные сеттеры на правильность наименования аргументов!
    inline void setGameRegisteredServerSmartCoversByLevelID(
        const xr_map<std::uint8_t, xr_map<std::uint32_t, Script_SE_SmartCover*>>& map)
    {
        if (map.empty())
        {
            MESSAGEW("map.size() = 0! You are trying to set an empty map!");
            //  return;
        }

        this->m_game_registered_smartcovers_by_level_id = map;
    }

    inline void setGameRegisteredServerSmartCoversByLevelID(
        const std::uint8_t& level_id, const xr_map<std::uint32_t, Script_SE_SmartCover*>& map)
    {
        if (level_id == Globals::kUnsignedInt8Undefined)
        {
            MESSAGEW("level_id = std::uint8_t(-1)! You are trying to set an undefined number of unsigned int!");
            //  return;
        }

        if (map.empty())
        {
            MESSAGEW("map.size() = 0! You are trying to set an empty map!");
            //  return;
        }

        this->m_game_registered_smartcovers_by_level_id[level_id] = map;
    }

    inline void setGameRegisteredServerSmartCoversByLevelID(
        const std::uint8_t& level_id, const std::pair<std::uint32_t, Script_SE_SmartCover*>& pair)
    {
        if (level_id == Globals::kUnsignedInt8Undefined)
        {
            MESSAGEW("level_id = std::uint8_t(-1)! You are trying to set an undefined number of unsigned int!");
            //  return;
        }

        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            MESSAGEW("pair.first = std::uint32_t(-1)! You are trying to set an undefined "
                "number of unsigned int! ");
            //   return;
        }

        if (!pair.second)
        {
            MESSAGEW("pair.second = null! You are trying to set an empty object!");
            //  return;
        }

        this->m_game_registered_smartcovers_by_level_id[level_id].insert(pair);
    }

    inline void setGameRegisteredServerSmartCoversByLevelID(
        const std::uint8_t& level_id, const std::uint32_t& id, Script_SE_SmartCover* server_smartcover)
    {
        if (level_id == Globals::kUnsignedInt8Undefined)
        {
            MESSAGEW("level_id = std::uint8_t(-1)! You are trying to set an undefined number of unsigned int!");
            //  return;
        }

        if (id == Globals::kUnsignedInt32Undefined)
        {
            MESSAGEW("id = std::uint32_t(-1)! You are trying to set an undefined number of unsigned int!");
            //    return;
        }

        if (!server_smartcover)
        {
            MESSAGEW("server_smartcover = null! You are trying to set an empty object!");
            //   return;
        }

        this->m_game_registered_smartcovers_by_level_id[level_id][id] = server_smartcover;
    }

    inline void deleteStorage(const std::uint16_t object_id)
    {
        MESSAGEI("deleting storage -> %d", object_id);
        this->m_storage.erase(object_id);
    }
    // @ Lord: проверить ВСЁ на очистку!
    inline void Deallocate(void)
    {
        this->m_actor = nullptr;
        this->m_storage.clear();
        if (!this->m_zone_by_name.empty())
        {
            for (std::pair<const xr_string, CScriptGameObject*>& it : this->m_zone_by_name)
                it.second = nullptr;

            this->m_zone_by_name.clear();
        }

        if (!this->m_anomaly_by_name.empty())
        {
            for (std::pair<const xr_string, CScriptGameObject*>& it : this->m_anomaly_by_name)
                it.second = nullptr;

            this->m_anomaly_by_name.clear();
        }

        if (!this->m_game_registered_combat_spacerestrictors.empty())
        {
            for (std::pair<const xr_string, CScriptGameObject*>& it : this->m_game_registered_combat_spacerestrictors)
                it.second = nullptr;

            this->m_game_registered_combat_spacerestrictors.clear();
        }

        if (!this->m_game_registered_server_smartterrains_by_name.empty())
        {
            for (std::pair<const xr_string, Script_SE_SmartTerrain*>& it :
                this->m_game_registered_server_smartterrains_by_name)
                it.second = nullptr;

            this->m_game_registered_server_smartterrains_by_name.clear();
        }

        if (!this->m_game_registered_smartcovers.empty())
        {
            for (std::pair<const xr_string, Script_SE_SmartCover*>& it : this->m_game_registered_smartcovers)
                it.second = nullptr;

            this->m_game_registered_smartcovers.clear();
        }

        if (!this->m_game_registered_smartcovers_by_level_id.empty())
        {
            for (std::pair<const std::uint8_t, xr_map<std::uint32_t, Script_SE_SmartCover*>>& it :
                this->m_game_registered_smartcovers_by_level_id)
            {
                for (std::pair<const std::uint32_t, Script_SE_SmartCover*>& it_sub : it.second)
                    it_sub.second = nullptr;
            }

            this->m_game_registered_smartcovers_by_level_id.clear();
        }

        for (std::pair<const std::uint16_t, std::pair<Script_CampData*, CScriptGameObject*>>& it : this->m_camps)
        {
            if (it.second.first)
                xr_delete(it.second.first);
        }

        if (!this->m_kamps.empty())
        {
            for (std::pair<const xr_string, Script_XRKamp*>& it : this->m_kamps)
            {
                if (it.second)
                {
                    xr_delete(it.second);
                }
            }
        }

        for (std::pair<const xr_string, Script_XRPatrolManager*>& it : this->m_patrols_xr_patrol)
        {
            if (it.second)
            {
                MESSAGEI("deallocating %s", typeid(it.second).name());
                xr_delete(it.second);
            }
        }

        for (std::pair<const xr_string, Script_ReachTaskManager*>& it : this->m_patrols_xr_reach_task)
        {
            if (it.second)
            {
                MESSAGEI("deallocating %s", typeid(it.second).name());
                xr_delete(it.second);
            }
        }

        if (this->m_temporaries_ini_files.empty() == false)
        {
            for (std::pair<const xr_string, CScriptIniFile*>& it : this->m_temporaries_ini_files)
            {
                MESSAGEI("deleted temporary ini_file of object %s", it.first.c_str());
                xr_delete(it.second);
            }
        }

        this->m_temporaries_ini_files.clear();
        this->m_smart_covers_by_name.clear();
        this->m_patrols_xr_reach_task.clear();
        this->m_patrols_xr_patrol.clear();
        this->m_offline_objects.clear();
        this->m_spawned_vertex_by_id.clear();
        this->m_goodwill.first.clear();
        this->m_goodwill.second.clear();
        this->m_camp_storage.clear();
        this->m_noweapon_zones.clear();
        this->m_kamps_stalker.clear();
        this->m_script_ids.clear();

        Script_TaskManager::getInstance().Deallocate();
    }

    inline const xr_map<xr_string, xr_map<std::uint32_t, bool>>& getCampStorage(void) const noexcept
    {
        return this->m_camp_storage;
    }

    inline void setCampStorage(const xr_string& point_name, const std::uint32_t index, const bool value) noexcept
    {
        this->m_camp_storage[point_name][index] = value;
    }

    inline std::uint32_t getHelicopterCount(void) const noexcept { return this->m_helicopter_count; }

    // @ ++value;
    inline void AddHelicopterCount(void) noexcept { ++this->m_helicopter_count; }

    // @ --value;
    inline void DeleteHelicopterCount(void) noexcept { --this->m_helicopter_count; }

    inline const xr_map<std::uint32_t, CScriptGameObject*>& getHelicopterEnemies(void) const noexcept
    {
        return this->m_helicopter_enemies;
    }

    inline const xr_map<xr_string, bool>& getNoWeaponZones(void) const noexcept { return this->m_noweapon_zones; }
    inline void setNoWeaponZones(const xr_string& zone_name, const bool value) noexcept
    {
        if (zone_name.empty())
        {
            Msg("[Scripts/Database/Storage/setNoWeaponZones(zone_name, value)] WARNING: zone_name.empty() == true! You "
                "are trying to set an empty string! Retunr ...");
            return;
        }

        this->m_noweapon_zones[zone_name] = value;
    }
#pragma endregion

#pragma region Cordis Script_Binder_Anomaly stuff
    inline const xr_map<std::uint16_t, xr_string>& getArtefactWaysByID(void) const noexcept
    {
        return this->m_artefact_ways_by_id;
    }
    inline void setArtefactWaysByID(const std::uint16_t id, const xr_string& path_name) noexcept
    {
        if (path_name.empty())
        {
            Msg("[Scripts/DataBase/Storage/setArtefactWaysByID(id, path_name)] WARNING: path_name.empty() == true! ");
        }

        this->m_artefact_ways_by_id[id] = path_name;
    }

    inline const xr_map<std::uint16_t, std::uint32_t>& getArtefactPointsByID(void) const noexcept
    {
        return this->m_artefact_points_by_id;
    }

    inline void setArtefactPointsByID(const std::uint16_t id, const std::uint32_t point) noexcept
    {
        this->m_artefact_points_by_id[id] = point;
    }

    inline const xr_map<std::uint16_t, Script_Binder_Anomaly*>& getParentZonesArtefactByID(void)
    {
        return this->m_parent_zones_by_artefact_id;
    }

    inline void setParentZonesArtefactByID(const std::uint16_t id, Script_Binder_Anomaly* const p_binder_object)
    {
        if (!p_binder_object)
        {
            Msg("[Scripts/DataBase/Storage/setParentZonesArtefactByID(id, p_binder_object)] WARNING: p_binder_object "
                "== nullptr!");
        }

        this->m_parent_zones_by_artefact_id[id] = p_binder_object;
    }
#pragma endregion

#pragma region Cordis Script_Binder_Field
    inline const xr_map<xr_string, Script_Binder_AnomalField*>& getFieldsByName(void) const noexcept
    {
        return this->m_fields_by_name;
    }
    inline void setFieldsByName(const xr_string& field_name, Script_Binder_AnomalField* const p_field)
    {
        if (!p_field)
        {
            Msg("[Scripts/DataBase/Storage/setFieldsByName(field_name, p_field)] WARNING: p_field == nullptr!");
        }

        this->m_fields_by_name[field_name] = p_field;
    }
#pragma endregion

#pragma region Cordis Script_Binder_DoorLabx8
    inline const xr_map<xr_string, Script_Binder_DoorLabx8*>& getAnimationObjectsByName(void) const noexcept
    {
        return this->m_animation_object_by_name;
    }
#pragma endregion

#pragma region Cordis Script_Binder_Crow

    inline const xr_map<std::uint16_t, std::uint16_t>& getCrowStorage(void) const noexcept
    {
        return this->m_crow_storage;
    }

    inline void setCrowStorage(const std::uint16_t object_id, const std::uint16_t objectid)
    { 
        this->m_crow_storage[object_id] = objectid;
    }

    inline void addCrowToCount(void) noexcept { ++this->m_crow_count; }
    inline void deleteCrowToCount(void) noexcept {--this->m_crow_count;}
#pragma endregion

    #pragma region Cordis Scheme XR Kamp
    inline const xr_map<xr_string, Script_XRKamp*>& getKamps(void) const noexcept { return this->m_kamps; }

    inline void setKamps(const xr_string& center_point_name, Script_XRKamp* const p_kamp)
    {
        if (center_point_name.empty())
        {
            Msg("[Scripts/DataBase/Storage/setKamps(center_point_name, p_kamp)] WARNING: center_point_name.empty() == "
                "true! Return ...");
            return;
        }

        if (!p_kamp)
        {
            Msg("[Scripts/DataBase/Storage/setKamps(center_point_name, p_kamp)] WARNING: p_kamp == nullptr! You are "
                "trying to set a null object! But this object must be deallocated manually!");
            return;
        }

        this->m_kamps[center_point_name] = p_kamp;
    }

    inline const xr_map<std::uint16_t, bool>& getKampsStalker(void) const noexcept { return this->m_kamps_stalker; }
    
    inline void setKampsStalker(const std::uint16_t npc_id, const bool value) noexcept 
    {
        if (npc_id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[Scripts/DataBase/Storage/setKampsStalker(npc_id, value)] WARNING: npc_id is undefined value! Return ...");
            return;
        }

        if (!npc_id)
            Msg("[Scripts/DataBase/Storage/setKampsStalker(npc_id, value)] WARNING: npc_id == 0! You are trying to set ID equals to 0!");

        this->m_kamps_stalker[npc_id] = value;
        
    }
    #pragma endregion

    inline const xr_map<std::uint16_t, xr_string>& getScriptIDS(void) const noexcept { return this->m_script_ids; }
    inline void setScriptIDS(const std::uint16_t id, const xr_string& name) noexcept
    {
        if (name.empty())
        {
            MESSAGEW("name.empty() == true! You set an empty string");
        }

        this->m_script_ids[id] = name;
    }

#pragma region Cordis Scheme XR Combat Ignore 
    inline const xr_map<std::uint16_t, bool>& getXRCombatIgnoreFightingWithActorNpcs(void) const noexcept { return this->m_fighting_with_actor_npcs; }
    
    inline void setXRCombatIgnoreFightingWithActorNpcs(const std::uint16_t npc_id, const bool value) noexcept 
    {
        if (npc_id == 0 || npc_id == Globals::kUnsignedInt16Undefined)
        {
            MESSAGEWR("bad id check your code!");
            return;
        }

        this->m_fighting_with_actor_npcs[npc_id] = value;
    }
#pragma endregion

#pragma region Cordis Scheme XR Patrol
    inline const xr_map<xr_string, Script_XRPatrolManager*>& getPatrolsXRPatrol(void) const { return this->m_patrols_xr_patrol; }
    inline void setPatrolsXRPatrol(const xr_string& path_name, Script_XRPatrolManager* const p_manager) 
    {
        if (path_name.empty())
        {
            MESSAGEWR("your index is empty!");
            return;
        }

        if (p_manager == nullptr)
        {
            MESSAGEWR("your value is nullptr, can't be because it must be deallocated!");
            return;
        }

        this->m_patrols_xr_patrol[path_name] = p_manager;
    } 
#pragma endregion

#pragma region Cordis Scheme XR Reach Task
    inline const xr_map<xr_string, Script_ReachTaskManager*>& getPatrolsXRReachTask(void) const noexcept 
    {
        return this->m_patrols_xr_reach_task;
    }

    inline void setPatrolsXRReachTask(const xr_map<xr_string, Script_ReachTaskManager*>& data) noexcept 
    {
        if (data.empty())
        {
            MESSAGEWR("You are trying to set an empty map!");
            return;
        }

        this->m_patrols_xr_reach_task = data;
    }

    inline void setPatrolsXRReachTask(const xr_string& id_name, Script_ReachTaskManager* const p_manager)
    {
        if (id_name.empty())
        {
            MESSAGEWR("You are trying to set with id which it is empty!");
            return;
        }
        
        if (this->m_patrols_xr_reach_task.find(id_name) != this->m_patrols_xr_reach_task.end())
        {
            MESSAGEWR("You are trying to add an existing element!");
            return;
        }

        if (p_manager == nullptr)
        {
            MESSAGEWR("Value can't be equals to nullptr!");
            return;
        }

        this->m_patrols_xr_reach_task[id_name] = p_manager;
    }
#pragma endregion

#pragma region Cordis Temporaries XR_LOGIC
    inline void RegisterAllocatedTemporaryIni(const xr_string& object_name, CScriptIniFile* p_pointer)
    {
        if (this->m_temporaries_ini_files.find(object_name) != this->m_temporaries_ini_files.end())
        {
            MESSAGEER("You are trying to set already allocated instance! Memory leak!");
            return;
        }

        this->m_temporaries_ini_files[object_name] = p_pointer;
    }
#pragma endregion 

    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(Storage&&) = delete;

private:
    std::uint32_t m_crow_count = 0;
    std::uint32_t m_helicopter_count = 0;
    CScriptGameObject* m_actor = nullptr;

#pragma region Cordis Inventory Upgrades
    CScriptGameObject* m_p_victim = nullptr;
#pragma endregion

    xr_map<std::uint16_t, Storage_Data> m_storage;
    xr_map<std::uint16_t, xr_string> m_script_ids;
    xr_map<xr_string, xr_map<std::uint32_t, bool>> m_camp_storage; // @ Uses in mob_camp only
    xr_map<std::uint16_t, std::pair<std::uint16_t, xr_string>> m_offline_objects;
    xr_map<std::uint32_t, CScriptGameObject*> m_helicopter_enemies;
    xr_map<xr_string, bool> m_noweapon_zones;
    xr_map<std::uint16_t, bool> m_kamps_stalker;
    xr_map<xr_string, Script_XRKamp*> m_kamps;
    xr_map<std::uint16_t, std::uint16_t> m_crow_storage;
    xr_map<xr_string, CScriptGameObject*> m_zone_by_name;
    xr_map<xr_string, CScriptGameObject*> m_anomaly_by_name;
    xr_map<xr_string, CScriptGameObject*> m_smart_covers_by_name;
    xr_map<xr_string, CScriptGameObject*> m_game_registered_combat_spacerestrictors;
    xr_map<xr_string, Script_SE_SmartTerrain*> m_game_registered_server_smartterrains_by_name;
    xr_map<std::uint8_t, xr_map<std::uint32_t, Script_SE_SmartCover*>> m_game_registered_smartcovers_by_level_id;
    xr_map<xr_string, Script_SE_SmartCover*> m_game_registered_smartcovers;
    xr_map<std::uint16_t, std::uint32_t> m_spawned_vertex_by_id;
#pragma region Cordis Script_Binder_AnomalZone
    xr_map<std::uint16_t, xr_string> m_artefact_ways_by_id;
    xr_map<std::uint16_t, std::uint32_t> m_artefact_points_by_id;
    xr_map<std::uint16_t, Script_Binder_Anomaly*> m_parent_zones_by_artefact_id;
#pragma endregion

#pragma region Cordis Script_Binder_Field
    xr_map<xr_string, Script_Binder_AnomalField*> m_fields_by_name;
#pragma endregion

#pragma region Cordis Script_Binder_DoorLabx8
    xr_map<xr_string, Script_Binder_DoorLabx8*> m_animation_object_by_name;
#pragma endregion

    #pragma region Cordis Script_Binder_Camp
    // @ second.first -> .camp!
    // @ second.second -> .object!
    xr_map<std::uint16_t, std::pair<Script_CampData*, CScriptGameObject*>> m_camps;
    #pragma endregion

#pragma region Cordis Script_Binder_Campfire
    xr_map<xr_string, xr_map<std::uint16_t, CZoneCampfire*>> m_campfires_by_smart_name;
#pragma endregion

#pragma region Cordis Scheme XR Combat Ignore 
    xr_map<std::uint16_t, bool> m_fighting_with_actor_npcs;
#pragma endregion

#pragma region Cordis Scheme XR Patrol
    xr_map<xr_string, Script_XRPatrolManager*> m_patrols_xr_patrol;
#pragma endregion

#pragma region Cordis Scheme XR Reach Task
    xr_map<xr_string, Script_ReachTaskManager*> m_patrols_xr_reach_task;
#pragma endregion
#pragma region Cordis Temporary from XR_LOGIC determine_...
    xr_map<xr_string, CScriptIniFile*> m_temporaries_ini_files;
#pragma endregion 
    // first -> sympathy[ID] = std::uint32_t; | second -> relations[ID] = std::string;
    std::pair<xr_map<std::uint16_t, float>, xr_map<std::uint16_t, xr_string>> m_goodwill;
};

} // namespace DataBase
} // namespace Scripts
} // namespace Cordis