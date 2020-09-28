#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_WeaponMagazinedWGL : public CSE_ALifeItemWeaponMagazinedWGL
{
    using inherited = CSE_ALifeItemWeaponMagazinedWGL;

public:
    Script_SE_WeaponMagazinedWGL(LPCSTR section_name);
    virtual ~Script_SE_WeaponMagazinedWGL(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void) const;

private:
    bool m_is_secret_item;
};
} // namespace Scripts
} // namespace Cordis
