#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Actor : public CSE_ALifeCreatureActor
{
    using inherited = CSE_ALifeCreatureActor;

public:
    Script_SE_Actor(LPCSTR section);
    virtual ~Script_SE_Actor(void);

    virtual Script_SE_Actor* cast_script_se_actor(void) { return this; }
    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual void STATE_Write(NET_Packet& packet);
    virtual void STATE_Read(NET_Packet& packet, std::uint16_t size);
    virtual bool IsSimulationAvailable(void);

    inline bool am_i_reached(void) { return Globals::Game::level::get_object_by_id(this->ID)->Alive(); }
    inline CALifeSmartTerrainTask* getAlifeSmartTerrainTask(void)
    {
        Msg("[Scripts/Script_SE_Actor/get_alife_task()] Returning alife task for object [%d] game_vertex [%d] "
            "level_vertex [%d] position: %d,%d,%d",
            this->ID, this->m_tGraphID, this->m_tNodeID, this->Position().x, this->Position().y, this->Position().z);

        if (!this->m_alife_smart_terrain_task.get())
        {
            this->m_alife_smart_terrain_task = std::make_unique<CALifeSmartTerrainTask>(this->m_tGraphID, this->m_tNodeID);
        }

        return this->m_alife_smart_terrain_task.get();
    }

    inline float evaluate_priority(Script_SE_SimulationSquad* squad)
    {
        return Script_SimulationObjects::getInstance().evaluate_priority(this, squad);
    }

    void on_reach_target(Script_SE_SimulationSquad* squad);

    bool target_precondition(Script_SE_SimulationSquad* squad);

private:
    bool m_is_registered;
    bool m_is_start_position_filled;
    std::unique_ptr<CALifeSmartTerrainTask> m_alife_smart_terrain_task;
};

} // namespace Scripts
} // namespace Cordis
