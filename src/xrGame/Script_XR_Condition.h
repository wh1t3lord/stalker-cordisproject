#pragma once
#include "Script_SurgeManager.h"

namespace Cordis
{
namespace Scripts
{
namespace XR_CONDITION
{
inline bool is_surge_started(void) { return Script_SurgeManager::getInstance().IsStarted(); }
} // namespace XR_CONDITION
} // namespace Scripts
} // namespace Cordis
