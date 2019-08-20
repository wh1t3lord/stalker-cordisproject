#pragma once

#include "xrServer_Objects_ALife_Monsters.h"

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

    inline std::uint32_t getSmartTerrainID(void) noexcept { return this->m_smart_terrain_id; }
    inline void setSmartTerrainID(const std::uint32_t& value) noexcept { this->m_smart_terrain_id = value; }

private:
    std::uint32_t m_smart_terrain_id;
};

} // namespace Scripts
} // namespace Cordis
