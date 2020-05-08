#pragma once
/*

#include "xrServer_Objects_ALife_Monsters.h"*/
#include "Script_XR_Logic.h"
#include "Script_SoundManager.h"

namespace Cordis
{
namespace Scripts
{
class Script_SE_SmartTerrain;
namespace Globals
{
class Script_RandomFloat;
}
} // namespace Scripts
} // namespace Cordis

namespace Cordis
{
namespace Scripts
{
struct StayReachOnTarget
{
private:
    float m_idle_time = Globals::Script_RandomFloat::getInstance().Generate(
        Globals::kSimulationSquadActionsStayPointIdleMin, Globals::kSimulationSquadActionsStayPointIdleMax);
    std::uint16_t m_squad_id;
    xrTime m_start_time = 0;
    xr_string m_name;

public:
    // @ It is for stay_target
    StayReachOnTarget(void) : m_squad_id(0), m_name(Globals::kSimulationSquadCurrentActionIDStayOnTarget) {}
    // @ It is for reach_target
    StayReachOnTarget(const std::uint16_t squad_id) : m_squad_id(squad_id), m_start_time(xrTime()), m_name(Globals::kSimulationSquadCurrentActionIDReachTarget) {}

    inline const xr_string& getName(void) const noexcept { return this->m_name; }
    inline const xrTime& getStartTime(void) const noexcept { return this->m_start_time; }
    inline std::uint16_t getSquadID(void) const noexcept { return this->m_squad_id; }
    inline float getIdleTime(void) const noexcept { return this->m_idle_time; }

    inline void setName(const xr_string& name) noexcept
    {
        if (name.empty())
        {
            MESSAGEWR("You are trying to set an empty string!");
            return;
        }

        this->m_name = name;
    }

    inline void setStartTime(const xrTime& time) noexcept { this->m_start_time = time; }

    inline void setSquadID(const std::uint16_t id) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[Scripts/StayReachOnTarget/setSquadID(id)] WARNING: id is undefined! You are trying to set it!");
        }

        this->m_squad_id = id;
    }

    inline void setIdleTime(const float value) noexcept { this->m_idle_time = value; }

     bool update(const bool is_under_simulation);

     void make(const bool is_under_simulation);

    inline void Clear(void) noexcept
    {
        this->m_name.clear();
        this->m_idle_time = 0.0f;
        this->m_start_time = 0;
    }
};
} // namespace Scripts
} // namespace Cordis

namespace Cordis
{
namespace Scripts
{
class Script_SE_SimulationSquad : public CSE_ALifeOnlineOfflineGroup
{
    using inherited = CSE_ALifeOnlineOfflineGroup;

public:
    Script_SE_SimulationSquad(LPCSTR section);
    virtual ~Script_SE_SimulationSquad(void);

    virtual Script_SE_SimulationSquad* cast_script_se_simulationsquad(void) { return this; }
    virtual CALifeSmartTerrainTask* get_current_task(void);
    virtual void STATE_Read(NET_Packet& packet, u16 size);
    virtual void STATE_Write(NET_Packet& packet);
    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual void update();
#pragma region Cordis Getters
    inline bool IsAlwaysArrived(void) const noexcept { return this->m_is_always_arrived; }
    inline std::uint32_t getSmartTerrainID(void) noexcept { return this->m_smart_terrain_id; }
    inline std::uint32_t getCurrentSpotID(void) noexcept { return this->m_current_spot_id; }
    inline std::uint32_t getCurrentTargetID(void) noexcept { return this->m_current_target_id; }
    inline std::uint32_t getAssignedTargetID(void) noexcept { return this->m_assigned_target_id; }
    inline std::uint32_t getEnteredSmartID(void) noexcept { return this->m_entered_smart_id; }
    inline xr_string& getSettingsID(void) noexcept { return this->m_settings_id_name; }
    std::uint16_t getScriptTarget(void);

    inline const StayReachOnTarget& getCurrentAction(void) noexcept { return this->m_current_action; }
    inline std::uint32_t getPlayerID(void) noexcept
    {
        return this->m_player_id;
    } // Lord: сделать под simulationActivities in Script_SimulationBoard как айдишник на который ссылаться

    inline const xr_string& getPlayerIDName(void) const noexcept { return this->m_player_id_name; }

    inline CALifeSmartTerrainTask* getAlifeSmartTerrainTask(void)
    { // Lord: проверить не изменяются ли this->m_tGraphID, this->m_tNodeID!!!
        Msg("[Scripts/Script_SE_SimulationSquad/getAlifeSmartTerrainTask()] game_vertex_id -> [%d] | level_vertex_id "
            "-> [%d], %s",
            this->m_tGraphID, this->m_tNodeID, this->name_replace());
        if (!this->m_alife_smart_terrain_task.get())
        {
            this->m_alife_smart_terrain_task =
                std::make_unique<CALifeSmartTerrainTask>(this->m_tGraphID, this->m_tNodeID);
        }

        return this->m_alife_smart_terrain_task.get();
    }

#pragma endregion

#pragma region Cordis Setters
    inline const xr_map<xr_string, xr_string>& getBehavior(void) const noexcept { return this->m_behaviour; }
    inline void setSmartTerrainID(const std::uint32_t value) noexcept { this->m_smart_terrain_id = value; }
    inline void setCurrentSpotID(const std::uint32_t value) noexcept { this->m_current_spot_id = value; }
    inline void setCurrentTargetID(const std::uint32_t value) noexcept { this->m_current_target_id = value; }
    inline void setAssignedTargetID(const std::uint32_t value) noexcept { this->m_assigned_target_id = value; }
    inline void setEnteredSmartID(const std::uint32_t value) noexcept { this->m_entered_smart_id = value; }
    inline void set_squad_positon(const Fvector& position) noexcept
    {
        if (!this->m_bOnline)
            this->force_change_position(position);

        for (AssociativeVector<std::uint16_t, CSE_ALifeMonsterAbstract*>::const_iterator it =
                 this->squad_members().begin();
             it != this->squad_members().end(); ++it)
        {
            CScriptGameObject* const p_client_object = Globals::Game::level::get_object_by_id(it->first);
            DataBase::Storage::getInstance().setOfflineObjects(it->first, Globals::Game::level::vertex_id(position));
            if (p_client_object)
            {
                XR_EFFECTS::_reset_animation(p_client_object);
#ifdef DEBUG
                MESSAGE("[Scripts/Script_SE_SimulationSquad/set_squad_position(position)] teleporting npc %d %s", it->first,
                    it->second->name_replace());
#endif // DEBUG
                p_client_object->SetNpcPosition(position);
#ifdef DEBUG
                MESSAGE("[Scripts/Script_SE_SimulationSquad/set_squad_position(position)] end teleporting npc %d %s",
                    it->first, it->second->name_replace());
#endif // DEBUG
            }
            else
            {
                it->second->o_Position = position;
            }
        }
    }
#pragma endregion

    inline bool am_i_reached(void) noexcept { return (this->npc_count() == 0); }

    void set_location_types(const xr_string& new_smart_name);
    void create_npc(Script_SE_SmartTerrain* spawn_smart);
    std::uint16_t add_squad_member(const xr_string& spawn_section_name, const Fvector& spawn_position,
        const std::uint32_t& level_vertex_id, const std::uint16_t& game_vertex_id);
    void on_npc_death(CSE_ALifeDynamicObject* server_object);
    void remove_squad(void);
    void set_squad_relation(const xr_string& relation_name = "");
    void assign_smart(Script_SE_SmartTerrain* smart = static_cast<Script_SE_SmartTerrain*>(nullptr));
    void assign_squad_member_to_smart(
        const std::uint16_t& object_id, Script_SE_SmartTerrain* smart, const std::uint16_t& old_smart_terrain_id);

private:
    inline xr_string& pick_next_target(void) { return this->m_parsed_targets[this->m_next_target_index]; }
    void set_location_types_section(const xr_string& section);
    void set_squad_sympathy(const float& sympathy = 0.0f);
    void set_squad_behaviour(void);
    void init_squad_on_load(void);
    void refresh(void);
    void show(void);
    void hide(void);
    bool check_squad_come_to_point(void);
    void check_invulnerability(void);
    bool update_current_action(void);
    void get_next_action(const bool is_under_simulation);
    void generic_update(void);
private:
    bool m_is_always_walk; // @ Init by utils
    bool m_is_always_arrived; // @ Init by utils
    bool m_is_need_to_reset_location_masks;
    bool m_is_need_free_update;
    bool m_is_show_disabled;
    std::uint16_t m_smart_terrain_id;
    std::uint32_t m_current_spot_id;
    std::uint32_t m_respawn_point_id;
    std::uint32_t m_current_target_id;
    std::uint32_t m_assigned_target_id;
    std::uint32_t m_next_target_index;
    std::uint32_t m_entered_smart_id;
    std::uint32_t m_player_id;
    float m_sympathy; // @ Init by utils
    xr_map<std::uint32_t, CondlistData> m_condlist_action; // @ Init by Utils
    xr_map<std::uint32_t, CondlistData> m_condlist_death; // @ Init by Utils
    xr_map<std::uint32_t, CondlistData> m_condlist_invulnerability; // @ Init by Utils
    xr_map<std::uint32_t, CondlistData> m_condlist_show_spot; // @ Init by Utils
    xr_map<xr_string, xr_string> m_behaviour;
    xr_vector<xr_string> m_parsed_targets;
    xr_string m_relationship_name; // @ Init by utils
    xr_string m_player_id_name; // @ Init by utils
    xr_string m_settings_id_name;
    xr_string m_last_target_name;
    xr_string m_spot_section_name;
    xr_string m_respawn_point_prop_section_name;
    std::unique_ptr<CALifeSmartTerrainTask> m_alife_smart_terrain_task;
    StayReachOnTarget m_current_action;
    Script_SoundManager m_sound_manager;
};

} // namespace Scripts
} // namespace Cordis
