#include "stdafx.h"
#include "Script_TaskManager.h"
#include "Script_Task.h"

namespace Cordis
{
namespace Scripts
{
Script_TaskManager::~Script_TaskManager(void)
{
    if (this->m_p_ini)
    {
        Msg("[Scripts/Script_TaskManager/~dtor()] deleting this->m_p_ini");
        xr_delete(this->m_p_ini);
    }

    if (!this->m_task_info.empty())
    {
        for (std::pair<const xr_string, Script_Task*>& it : this->m_task_info)
        {
            Msg("[Scripts/Scritp_TaskManager/~dtor()] deleting task %s", it.second->getTaskIDName().c_str());
            xr_delete(it.second);
        }

        this->m_task_info.clear();
    }
}

void Script_TaskManager::save(NET_Packet& packet)
{
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, false, "Script_TaskManager");
    packet.w_u16(this->m_task_info.size());

    for (const std::pair<xr_string, Script_Task*>& it : this->m_task_info)
    {
        packet.w_stringZ(it.first.c_str());
        it.second->save(packet);
    }

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, true, "Script_TaskManager");
}

void Script_TaskManager::load(IReader& packet)
{
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, false, "Script_TaskManager");
    std::uint16_t size = packet.r_u16();

    xr_string id_name;
    for (std::uint16_t i = 0; i < size; ++i)
    {
        packet.r_stringZ(id_name);
        Script_Task* p_task = new Script_Task(this->m_p_ini, id_name);
        p_task->load(packet);
        this->m_task_info[id_name] = p_task;
    }

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, true, "Script_TaskManager");
}

void Script_TaskManager::give_task(const xr_string& task_id_name)
{
    Msg("[Scripts/Script_TaskManager/give_task(task_id_name)] %s", task_id_name.c_str());

    if (!this->m_p_ini->section_exist(task_id_name.c_str()))
    {
        R_ASSERT2(false, "There is no task in task ini file or ini file is not included!");
        return;
    }

    this->m_task_info[task_id_name] = new Script_Task(this->m_p_ini, task_id_name);
    this->m_task_info[task_id_name]->give_task();
}

bool Script_TaskManager::task_complete(const xr_string& task_id_name)
{
    Script_Task* const p_task = this->m_task_info.at(task_id_name);

    if (!p_task)
        return false;

    p_task->check_task();
    if (p_task->getLastCheckTaskName() == "complete")
    {
        p_task->give_reward();
        // Lord: когда будет xr_statistic
        return true;
    }

    return false;
}

bool Script_TaskManager::task_fail(const xr_string& task_id_name)
{
    Script_Task* const p_task = this->m_task_info.at(task_id_name);

    if (!p_task)
        return false;

    p_task->check_task();

    return (p_task->getLastCheckTaskName() == "fail" || p_task->getLastCheckTaskName() == "reversed");
}

void Script_TaskManager::task_callback(CGameTask* p_task, const std::uint32_t status)
{
    xr_string task_id_name = p_task->GetID_script();

    if (this->m_task_info.find(task_id_name) == this->m_task_info.end())
        return;

    Msg("[Scripts/Script_TaskManager/task_callback(p_task, status)] clear task %s", task_id_name.c_str());

    this->m_task_info.at(task_id_name)->deactivate_task(p_task);

    Script_Task* pointer_to_delete = this->m_task_info.at(task_id_name);

    Msg("[Scripts/Script_TaskManager/task_callback(p_task, status)] deleting task %S", pointer_to_delete->getTaskIDName().c_str());
    xr_delete(pointer_to_delete);
}

void Script_TaskManager::Deallocate(void)
{
    if (this->m_p_ini)
    {
        Msg("[Scripts/Script_TaskManager/Deallocate()] deleting this->m_p_ini");
        xr_delete(this->m_p_ini);
    }

    if (!this->m_task_info.empty())
    {
        for (std::pair<const xr_string, Script_Task*>& it : this->m_task_info)
        {
            Msg("[Scripts/Scritp_TaskManager/Deallocate()] deleting task %s", it.second->getTaskIDName().c_str());
            xr_delete(it.second);
        }

        this->m_task_info.clear();
    }
}

} // namespace Scripts
} // namespace Cordis
