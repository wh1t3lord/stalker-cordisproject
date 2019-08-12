#pragma once

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

// @ pair::first = direction | pair::second = distance
inline static xr_map<xr_string, xr_vector<std::pair<Fvector, float>>>& getFormations(void) noexcept
{
    xr_map<xr_string, xr_vector<std::pair<Fvector, float>>> instance;
    return instance;
}

struct PatrolNpcData
{
    bool m_accepted = false;
    int m_vertex_id = 0;
    float m_distance = 0.0f;
    CScriptGameObject* m_soldier = nullptr;
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

    void add_npc(CScriptGameObject* npc, const bool& is_leader)
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
            npc->Name(), this->m_waypoint_name);

        this->reset_positions();
    }

    void reset_positions(void)
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
