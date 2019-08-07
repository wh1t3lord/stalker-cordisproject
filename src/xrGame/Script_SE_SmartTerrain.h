#pragma once

namespace Cordis
{
namespace Scripts
{
struct NpcInfo
{ // @ Lord: пока что не вся заполнена!
    bool m_is_monster;
    bool m_begin_job;
    std::uint32_t m_stype;
    int m_job_prioprity;
    int m_job_id;
    CSE_ALifeDynamicObject* m_server_object; // @ Lord: определить где оно именно удаляется в итоге
    xr_string m_need_job;
};

class Script_SE_SmartTerrain : public CSE_ALifeSmartZone
{
    using inherited = CSE_ALifeSmartZone;

public:
    Script_SE_SmartTerrain(LPCSTR section);
    virtual ~Script_SE_SmartTerrain(void);

    inline NpcInfo fill_npc_info(CSE_ALifeDynamicObject* server_object)
    {
        if (!server_object)
        {
            R_ASSERT2(false, "object was null!");
            return NpcInfo();
        }

        NpcInfo data;

        data.m_is_monster = Globals::IsStalker(server_object);
        data.m_server_object = server_object;
        data.m_need_job = "nil";
        data.m_job_prioprity = -1;
        data.m_job_id = -1;
        data.m_begin_job = false;

        if (data.m_is_monster)
            data.m_stype = Globals::STYPE_MOBILE;
        else
            data.m_stype = Globals::STYPE_STALKER;

        return data;
    }

    void read_params(void);
    inline void refresh_script_logic(const std::uint32_t& object_id)
    {
        CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(object_id);
        std::uint16_t stype = Globals::STYPE_MOBILE;

        if (Globals::IsStalker(server_object))
            stype = Globals::STYPE_STALKER;

        
    }

private:
    bool m_is_initialized;
    bool m_is_registered;
    std::uint32_t m_population;
    xr_string m_smart_level;
};
} // namespace Scripts
} // namespace Cordis
