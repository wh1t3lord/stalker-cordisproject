#pragma once

#include "xrCore/xrCore.h"
#include "xrEngine/IGame_Level.h"

namespace Cordis
{
namespace Scripts
{
namespace Globals
{
    inline bool is_level_present(void) 
    { 
        return (!!g_pGameLevel);
    }
} // namespace Globals
} // namespace Scripts
} // namespace Cordis
