#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_MobStateManager
{
private:
    Script_MobStateManager(void) {}

public:
    // @ Вызывать после инициализации игры и скриптов game_start_callback()
    inline static Script_MobStateManager& getInstance() noexcept
    {
        static Script_MobStateManager instance;
        return instance;
    }

    Script_MobStateManager(const Script_MobStateManager&) = delete;
    Script_MobStateManager& operator=(const Script_MobStateManager&) = delete;
    Script_MobStateManager(Script_MobStateManager&&) = delete;
    Script_MobStateManager& operator=(Script_MobStateManager&&) = delete;
    ~Script_MobStateManager(void) {}

    inline xr_string get_state(CScriptIniFile* ini, const xr_string& section_name)
    {
        xr_string state_name = Globals::Utils::cfg_get_string(ini, section_name, "state");

        return state_name;
    }

    inline void set_state(CScriptGameObject* p_client_object, const xr_string& state_name)
    {
        if (state_name.empty())
        {
            Msg("[Scripts/Script_MobStateManager/set_state(p_client_object, p_client_actor, state_name)] WARNING: "
                "state_name.empty() == true! Return ...");
            return;
        }

        if (!p_client_object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        if (!Globals::IsMonster(p_client_object, 0))
        {
            return;
        }

        const int& clsid_object = p_client_object->clsid();

        if (clsid_object == Globals::get_script_clsid(CLSID_SE_MONSTER_BLOODSUCKER))
        {
            if (state_name == "invis")
            {
                Msg("[Scripts/Script_MobStateManager/set_state(p_client_object, state_name)] bloodsucker %s %d is "
                    "invisible!",
                    p_client_object->Name(), p_client_object->ID());
                p_client_object->set_invisible(true);
            }
            else if (state_name == "vis")
            {
                Msg("[Scripts/Script_MobStateManager/set_state(p_client_object, state_name)] bloodsucker %s %d is "
                    "uninvisible",
                    p_client_object->Name(), p_client_object->ID());
                p_client_object->set_invisible(false);
            }
        }
    }
};
} // namespace Scripts
} // namespace Cordis
