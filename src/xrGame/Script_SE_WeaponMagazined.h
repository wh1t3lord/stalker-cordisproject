#pragma once

 

namespace Cordis
{
namespace Scripts
{
class Script_SE_WeaponMagazined : public CSE_ALifeItemWeaponMagazined
{
    using inherited = CSE_ALifeItemWeaponMagazined;

public:
    Script_SE_WeaponMagazined(LPCSTR section_name);
    virtual ~Script_SE_WeaponMagazined(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void);

private:
    bool m_is_secret_item;
};
}
} // namespace Cordis
