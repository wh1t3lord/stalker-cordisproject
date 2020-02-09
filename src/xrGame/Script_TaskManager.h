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
    inline static Script_TaskManager& getInstance(void) noexcept
    {
        static Script_TaskManager instance;
        return instance;
    }
    ~Script_TaskManager(void);
    Script_TaskManager(const Script_TaskManager&) = delete;
    Script_TaskManager& operator=(const Script_TaskManager&) = delete;
    Script_TaskManager(Script_TaskManager&&) = delete;
    Script_TaskManager& operator=(Script_TaskManager&&) = delete;

    void save(NET_Packet& packet);
    void load(IReader& packet);

    void give_task(const xr_string& task_id_name);
    bool task_complete(const xr_string& task_id_name);
    bool task_fail(const xr_string& task_id_name);

    void task_callback(CGameTask* p_task, const std::uint32_t status);

    // be carefull with that method
    void Deallocate(void);

    static void TaskCallback(CGameTask* p_task, const std::uint32_t status) 
    {
        if (status == eTaskStateFail || status == eTaskStateCompleted)
        {
            Script_TaskManager::getInstance().task_callback(p_task, status);
        }
    }

private:
    CScriptIniFile* m_p_ini;
    xr_map<xr_string, Script_Task*> m_task_info;
};
} // namespace Scripts
} // namespace Cordis
