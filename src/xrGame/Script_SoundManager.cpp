#include "stdafx.h"
#include "Script_SoundManager.h"

namespace Cordis
{
namespace Scripts
{
xr_map<xr_string, Script_SoundManager> Script_SoundManager::m_sound_managers;
}
} // namespace Cordis

namespace Cordis
{
namespace Scripts
{
Script_SoundManager::Script_SoundManager(const xr_string& id) { this->m_id = id; }
Script_SoundManager::~Script_SoundManager(void) {}

Story::Story(const xr_string& story_id) {}

Story::~Story(void) {}

} // namespace Scripts
} // namespace Cordis
