#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_TaskManager
{
private:
    Script_TaskManager(void) {}

public:
    inline static Script_TaskManager& getInstance(void) noexcept { static Script_TaskManager instance;  return instance;}
    Script_TaskManager(const Script_TaskManager&) = delete;
    Script_TaskManager& operator=(const Script_TaskManager&) = delete;
    Script_TaskManager(Script_TaskManager&&) = delete;
    Script_TaskManager& operator=(Script_TaskManager&&) = delete;

private:
    CScriptIniFile* m_p_ini;
  //  xr_map<xr_string, 
};
} // namespace Scripts
} // namespace Cordis
