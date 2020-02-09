#pragma once

#include "GameTask.h"

namespace Cordis
{
namespace Scripts
{
class Script_Task
{
public:
    Script_Task(void) = delete;
    Script_Task(CScriptIniFile* p_ini, const xr_string& task_id_name);
    ~Script_Task(void);

    inline void reverse_task(void) { this->m_last_check_task_name = "reversed"; }

    void give_task(void);
    void check_task(void);
    void give_reward(void);
    void deactivate_task(CGameTask* task);
    void check_level(const std::uint16_t target_id);
    void remove_guider_spot(void);

    void save(NET_Packet& packet);
    void load(NET_Packet& packet);

private:
    bool m_is_storyline;
    bool m_is_send_update_news;
    /*
        std::uint16_t m_current_title_id;
        std::uint16_t m_current_description_id;*/
        std::uint16_t m_current_target_id;
    std::uint32_t m_priority;
    std::uint32_t m_wait_time;
    std::uint32_t m_check_time;
    std::int32_t m_community_relation_delta_fail;
    std::int32_t m_community_relation_delta_complete;
    CScriptIniFile* m_p_ini;
    xrTime m_initied_time;
    xr_map<std::uint32_t, CondlistData> m_on_init_condlist;
    xr_map<std::uint32_t, CondlistData> m_on_complete_condlist;
    xr_map<std::uint32_t, CondlistData> m_on_reversed_condlist;
    xr_map<std::uint32_t, CondlistData> m_on_reward_money;
    xr_map<std::uint32_t, CondlistData> m_on_reward_item;
    xr_vector<xr_map<std::uint32_t, CondlistData>> m_condlist;
    xr_string m_title_name;
    xr_string m_title_functor_name;
    xr_string m_description_name;
    xr_string m_description_functor_name;
    xr_string m_target_name;
    xr_string m_target_functor_name;
    xr_string m_icon_name;
    xr_string m_spot_name;
    xr_string m_task_id_name;
    xr_string m_status_name;
    xr_string m_current_title_name;
    xr_string m_current_description_name;
   // xr_string m_current_target_name;
    xr_string m_last_check_task_name;
    CGameTask* m_task;
};
} // namespace Scripts
} // namespace Cordis
