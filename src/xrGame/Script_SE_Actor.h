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
    void on_register(void);
private:
    bool m_is_registered;
    bool m_is_start_position_filled;
};

}
} // namespace Cordis
