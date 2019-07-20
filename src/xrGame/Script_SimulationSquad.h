#pragma once

#include "xrServer_Objects_ALife_Monsters.h"


namespace Cordis
{
namespace Scripts
{
class Script_SimulationSquad : public CSE_ALifeOnlineOfflineGroup
{
    using inherited = CSE_ALifeOnlineOfflineGroup;
public:
    Script_SimulationSquad(LPCSTR section);
    virtual ~Script_SimulationSquad(void);
};

}
} // namespace Cordis
