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

    void on_register(void);
private:
    bool m_registred;
    bool m_start_position_filled;
};

}
} // namespace Cordis
