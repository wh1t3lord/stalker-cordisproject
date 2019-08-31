#pragma once

#include "Script_XR_Meet.h"

namespace Cordis
{
namespace Scripts
{
namespace XR_PATROL
{
constexpr const char* XR_PATROL_STATE_PATROL = "patrol";
constexpr const char* XR_PATROL_FORMATION_INDEX_BACK = "back";
constexpr const char* XR_PATROL_FORMATION_INDEX_LINE = "line";
constexpr const char* XR_PATROL_FORMATION_INDEX_AROUND = "around";

struct NpcCommandData
{
    std::uint32_t m_vertex_id;
    Fvector m_direction;
    xr_string m_current_state_name;
};

enum PatrolStates
{
    kPatrolMove,
    kPatrolHide,
    kPatrolSprint,
    kPatrolRun,
    kPatrolStop
};

inline static xr_map<xr_string, xr_string>& getAccelByCurrentType(void) noexcept
{
    static xr_map<xr_string, xr_string> instance = {
        {"walk", "run"}, {"patrol", "rush"}, {"raid", "assault"}, {"sneak", "sneak_run"}, {"sneak_run", "assault"}};
    return instance;
}

// @ pair::first = direction | pair::second = distance
inline static xr_map<xr_string, xr_vector<std::pair<Fvector, float>>>& getFormations(void) noexcept
{
    static xr_map<xr_string, xr_vector<std::pair<Fvector, float>>> instance;
    return instance;
}

struct PatrolNpcData
{
    bool m_accepted = false;
    int m_vertex_id = 0;
    float m_distance = 0.0f;
    CScriptGameObject* m_soldier = nullptr; // Lord: наверное здесь не нужно следить за удалением
    Fvector m_direction = Fvector().set(1.0f, 0.0f, 0.0f);
};

class Script_PatrolEntity
{
public:
    Script_PatrolEntity(const char* waypoint_name)
        : m_waypoint_name(waypoint_name), m_current_state_name(XR_PATROL_STATE_PATROL), m_commander_id(-1),
          m_commander_leader(-1), m_commander_direction(Fvector().set(0.0f, 0.0f, 1.0f)), m_npc_count(0),
          m_formation_name(XR_PATROL_FORMATION_INDEX_BACK)
    {
    }
    ~Script_PatrolEntity(void) {}

    inline void add_npc(CScriptGameObject* npc, const bool& is_leader)
    {
        if (!npc)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        if (!npc->Alive())
        {
            Msg("[Scripts/XR_PATROL/Script_PatrolEntity/add_npc(npc, is_leader)] Entity [%s] is dead, can't add npc!",
                npc->Name());
            return;
        }

        if (this->m_npc_list[npc->ID()].m_soldier)
        {
            Msg("[Scripts/XR_PATROL/Script_PatrolEntity/add_npc(npc, is_leader)] Cannot add the npc [%s], because he "
                "is already in m_npc_list!",
                npc->Name());
            return;
        }

        if (this->m_npc_count == 7)
        {
            R_ASSERT2(false,
                "[Scripts/XR_PATROL/Script_PatrolEntity/add_npc(npc, is_leader)] attemp to add more than 7 npc to "
                "list!");
            return;
        }

        PatrolNpcData data;
        data.m_soldier = npc;
        this->m_npc_list[npc->ID()] = data;
        ++(this->m_npc_count);
        if (this->m_npc_count == 1 || is_leader)
        {
            this->m_commander_id = npc->ID();
            Msg("[Scripts/XR_PATROL/Script_PatrolEntity/add_npc(npc, is_leader)] ASSIGNED NPC %s AS PATROL COMMANDER",
                npc->Name());
        }

        Msg("[Scripts/XR_PATROL/Script_PatrolEntity/add_npc(npc, is_leader)] NPC %s added to patrol manager %s",
            npc->Name(), this->m_waypoint_name.c_str());

        this->reset_positions();
    }

    inline void remove_npc(CScriptGameObject* npc)
    {
        if (!npc)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        if (!this->m_npc_list[npc->ID()].m_soldier)
        {
            R_ASSERT2(false, "Your object had unregistered!");
            return;
        }

        Msg("[Scripts/XR_PATROL/Script_PatrolEntity/remove_npc(npc)] NPC [%s] removed from patrol manager %s",
            npc->Name(), this->m_waypoint_name.c_str());
        this->m_npc_list[npc->ID()] = PatrolNpcData();
        --(this->m_npc_count);

        if (npc->ID() == this->m_commander_id)
        {
            this->m_commander_id = -1;
            this->reset_positions();
        }
    }

    inline void reset_positions(void)
    {
        xr_vector<std::pair<Fvector, float>>& formation_buffer = getFormations()[this->m_formation_name];
        std::uint16_t it = 0;

        for (std::pair<const std::uint32_t, PatrolNpcData>& pr : this->m_npc_list)
        {
            if (this->m_commander_id == -1 && it == 0)
                this->m_commander_id = pr.second.m_soldier->ID();

            if (this->m_commander_id != pr.second.m_soldier->ID())
            {
                pr.second.m_direction = formation_buffer[it].first;
                pr.second.m_distance = formation_buffer[it].second;
                pr.second.m_vertex_id = -1;
                pr.second.m_accepted = true;

                ++it;
            }
        }
    }

    inline void set_formation(const char* formation_name)
    {
        if (!formation_name)
        {
            R_ASSERT2(false, "Invalid string!");
            return;
        }

        if (strcmp(formation_name, XR_PATROL_FORMATION_INDEX_BACK) ||
            strcmp(formation_name, XR_PATROL_FORMATION_INDEX_AROUND) ||
            strcmp(formation_name, XR_PATROL_FORMATION_INDEX_LINE))
        {
            Msg("[Scripts/XR_PATROL/Script_PatrolEntity/set_formation(formation_name)] Invalid formation (%s) for "
                "PatrolManager [%s]",
                formation_name, this->m_waypoint_name.c_str());
            R_ASSERT(false);
        }

        this->m_formation_name = formation_name;
        this->reset_positions();
    }

    inline void set_formation(const xr_string& data) { this->set_formation(data.c_str()); }

    CScriptGameObject* get_commander(CScriptGameObject* npc)
    {
        if (!npc)
        {
            R_ASSERT2(false, "object was null!");
            return nullptr;
        }

        const std::uint16_t& npc_id = npc->ID();

        if (!this->m_npc_list[npc_id].m_soldier)
        {
            R_ASSERT2(false, "Your npc doesn't registered");
            return nullptr;
        }

        if (npc_id == this->m_commander_id)
        {
            R_ASSERT2(false, "Patrol commander called function get_commander!");
            return nullptr;
        }

        CScriptGameObject* commander = this->m_npc_list[this->m_commander_id].m_soldier;

        if (!commander)
        {
            R_ASSERT2(false, "Commander doesn't registered in npc_list!");
            return nullptr;
        }

        return commander;
    }

    NpcCommandData get_npc_command(CScriptGameObject* npc)
    {
        if (!npc)
        {
            R_ASSERT2(false, "object was null!");
            return NpcCommandData();
        }

        const std::uint16_t& npc_id = npc->ID();

        if (!this->m_npc_list[npc_id].m_soldier)
        {
            R_ASSERT2(false, "Your NPC doesn't registered in list!");
            return NpcCommandData();
        }

        if (npc_id == this->m_commander_id)
        {
            R_ASSERT2(false, "Patrol commander called function!");
            return NpcCommandData();
        }

        CScriptGameObject* commander = this->m_npc_list[this->m_commander_id].m_soldier;

        Fvector direction_commander = commander->Direction();
        Fvector position = Fvector().set(0.0f, 0.0f, 0.0f);
        std::uint32_t vertex_id = commander->location_on_path(5, &position);
        if (Globals::vertex_position(vertex_id).distance_to(
                this->m_npc_list[this->m_commander_id].m_soldier->Position()) > 5.0f)
            vertex_id = commander->level_vertex_id();

        direction_commander.y = 0.0f;
        direction_commander.normalize();

        Fvector direction_soldier = this->m_npc_list[npc_id].m_direction;
        float distance_soldier = this->m_npc_list[npc_id].m_distance;

        float angle = Globals::yaw_degree(direction_soldier, (Fvector().set(0.0f, 0.0f, 1.0f)));
        Fvector result = Globals::vector_cross(direction_soldier, Fvector().set(0.0f, 0.0f, 1.0f));

        if (result.y < 0.0f)
            angle = -angle;

        direction_soldier = Globals::vector_rotate_y(direction_soldier, angle);

        float max_distance = 2.0f;
        std::uint32_t vertex_id_to =
            Globals::vertex_in_direction(Globals::vertex_in_direction(vertex_id, direction_soldier, distance_soldier),
                direction_commander, max_distance);
        this->m_npc_list[npc_id].m_vertex_id = vertex_id_to;

        float distance = commander->Position().distance_to(this->m_npc_list[npc_id].m_soldier->Position());

        if (distance > distance_soldier + 2.0f)
        {
            xr_string new_state = getAccelByCurrentType()[this->m_current_state_name];

            if (new_state.size())
            {
                NpcCommandData data;
                data.m_current_state_name = new_state;
                data.m_vertex_id = vertex_id_to;
                data.m_direction = direction_commander;
                return data;
            }
        }

        NpcCommandData data;
        data.m_current_state_name = this->m_current_state_name;
        data.m_direction = direction_commander;
        data.m_vertex_id = vertex_id_to;

        return data;
    }

    inline void set_command(CScriptGameObject* npc, const xr_string& state, const xr_string& formation)
    {
        if (!npc)
        {
            R_ASSERT2(false, "Object was null!");
            return;
        }

        if (!npc->Alive())
        {
            this->remove_npc(npc);
            return;
        }

        if (npc->ID() == this->m_commander_id)
            return;

        this->m_current_state_name = state;
        if (this->m_formation_name != formation)
        {
            this->m_formation_name = formation;
            this->set_formation(formation);
        }

        this->m_commander_leader = npc->level_vertex_id();
        this->m_commander_direction = npc->Direction();

        // Lord : подумать используется здесь данный метод ибо не понятно где вообще self.tm и оно используется(
        // основаная догадка что нет)
        //   this->update();
    }

    inline bool is_commander(const std::uint16_t& npc_id) { return npc_id == this->m_commander_id; }

    inline bool is_commander_in_meet(void)
    {
        if (this->m_commander_id == -1)
            return false;

        CScriptGameObject* npc = this->m_npc_list[this->m_commander_id].m_soldier;

        if (!npc)
        {
            R_ASSERT2(false, "object was null!");
            return false;
        }

        if (npc->Alive())
            return XR_MEET::is_meet(npc);

        return false;
    }
    /*
void update(void)
{

}*/

private:
    std::uint32_t m_npc_count;
    int m_commander_id;
    int m_commander_leader;
    Fvector m_commander_direction;
    xr_map<std::uint32_t, PatrolNpcData> m_npc_list;
    xr_string m_current_state_name;
    xr_string m_formation_name;
    xr_string m_waypoint_name;
};

} // namespace XR_PATROL
} // namespace Scripts
} // namespace Cordis
