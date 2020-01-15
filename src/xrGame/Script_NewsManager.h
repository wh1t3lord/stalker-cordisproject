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
