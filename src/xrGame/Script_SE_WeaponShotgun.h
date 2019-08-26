#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_WeaponShotgun : public CSE_ALifeItemWeaponShotGun
{
    using inherited = CSE_ALifeItemWeaponShotGun;

public:
    Script_SE_WeaponShotgun(LPCSTR section_name);
    virtual ~Script_SE_WeaponShotgun(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void) const;

private:
    bool m_is_secret_item;
};
} // namespace Scripts
} // namespace Cordis
