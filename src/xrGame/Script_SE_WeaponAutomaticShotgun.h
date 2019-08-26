#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_WeaponAutomaticShotgun : public CSE_ALifeItemWeaponAutoShotGun
{
    using inherited = CSE_ALifeItemWeaponAutoShotGun;

public:
    Script_SE_WeaponAutomaticShotgun(LPCSTR section_name);
    virtual ~Script_SE_WeaponAutomaticShotgun(void);
    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void) const;

private: 
    bool m_is_secret_item;
};
} // namespace Scripts
} // namespace Cordis
