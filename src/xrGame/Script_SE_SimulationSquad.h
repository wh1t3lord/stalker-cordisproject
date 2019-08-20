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
};

}
} // namespace Cordis
