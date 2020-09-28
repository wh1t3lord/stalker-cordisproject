#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_NewsManager
{
private:
    Script_NewsManager(void) {}

public:
    inline static Script_NewsManager& getInstance(void) noexcept
    {
        static Script_NewsManager instance;
        return instance;
    }

    ~Script_NewsManager(void);
    Script_NewsManager(const Script_NewsManager&) = delete;
    Script_NewsManager& operator=(const Script_NewsManager&) = delete;
    Script_NewsManager(Script_NewsManager&&) = delete;
    Script_NewsManager& operator=(Script_NewsManager&&) = delete;

    void SendSound(CScriptGameObject* object, const xr_string& faction, const xr_string& point, const xr_string& str,
        const xr_string& str1 = xr_string(""), const float& delay = 0.0f);
    bool SendTip(CScriptGameObject* actor, const xr_string& news_id_name, const xr_string& sender_name,
        const std::uint32_t timeout = 0, const std::uint32_t show_time = 0, const xr_string& sender_id_name = "");
    void SendTask(CScriptGameObject* const p_actor, const xr_string& type_name, CGameTask* const p_task);
    void relocate_money(const xr_string& type_name, const int amount);
    void relocate_item(const xr_string& type_name, const xr_string& section_name, const std::uint32_t amount = 1);
    /*
    inline void send_tip_nosound(const xr_string& tip_id_name) {}
    inline CScriptSound& get_sound_news(const xr_string& tip_id_name)
    {
        if (tip_id_name.empty())
        {
            Msg("[Scripts/Script_NewsManager/get_sound_news(tip_id_name)] WARNING: tip_id_name.empty() == true! Return "
                "...");
            return;
        }

 
    }*/
};
} // namespace Scripts
} // namespace Cordis
