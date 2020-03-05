#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_Binder_Actor : public CScriptBinderObject
{
    using inherited = CScriptBinderObject;

public:
    Script_Binder_Actor(void) = delete;
    Script_Binder_Actor(CScriptGameObject* object);
    virtual ~Script_Binder_Actor(void);
    virtual void reinit(void);
    virtual void reload(LPCSTR section_name);
    virtual bool net_Spawn(SpawnType DC);
    virtual void net_Destroy(void);
    virtual void net_Import(NET_Packet* packet);
    virtual void net_Export(NET_Packet* packet);
    virtual void shedule_Update(std::uint32_t time_delta);
    virtual void save(NET_Packet* output_packet);
    virtual void load(IReader* input_packet);
    virtual bool net_SaveRelevant(void);
    virtual void net_Relcase(CScriptGameObject* object);

    inline bool IsHideWeapon(void) const noexcept { return this->m_is_hide_weapon; }
    inline void setHideWeapon(const bool value) noexcept { this->m_is_hide_weapon = value; }

    inline float getDeimosIntensity(void) const noexcept { return this->m_deimos_intensity; }
    inline void setDeimosIntensity(const float value) noexcept { this->m_deimos_intensity = value; }

    inline xr_string getGoodGunName(void) const noexcept { return this->m_good_gun_name; }
    inline void setGoodGunName(const xr_string& gun_name) noexcept
    {
        this->m_good_gun_name = gun_name;
    }

private:
    bool m_is_hide_weapon;
    float m_deimos_intensity;
    xr_string m_good_gun_name; 
};
} // namespace Scripts
} // namespace Cordis
