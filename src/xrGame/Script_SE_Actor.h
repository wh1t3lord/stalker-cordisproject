#pragma once

#include "xrServer_Objects_ALife_Monsters.h"

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

    inline bool am_i_reached(void) { return Globals::Game::level::get_object_by_id(this->ID)->Alive(); }
    inline CALifeSmartTerrainTask& get_alife_task(void)
    {
        Msg("[Scripts/Script_SE_Actor/get_alife_task()] Returning alife task for object [%d] game_vertex [%d] "
            "level_vertex [%d] position: %d,%d,%d",
            this->ID, this->m_tGraphID, this->m_tNodeID, this->Position().x, this->Position().y, this->Position().z);
        return CALifeSmartTerrainTask(this->m_tNodeID, this->m_tGraphID);
    }

    inline float evaluate_priority(Script_SE_SimulationSquad* squad) 
    {
        return Script_SimulationObjects::getInstance().evaluate_priority(this, squad);
    }

    void on_reach_target(Script_SE_SimulationSquad* squad);
    bool IsSimulationAvailable(void);

private:
    bool m_is_registered;
    bool m_is_start_position_filled;
};

} // namespace Scripts
} // namespace Cordis
