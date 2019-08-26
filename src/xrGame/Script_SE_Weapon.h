#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Weapon : public CSE_ALifeItemWeapon
{
    using inherited = CSE_ALifeItemWeapon;
public:
    Script_SE_Weapon(LPCSTR section_name);
    virtual ~Script_SE_Weapon(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void) const;
private:
    bool m_is_secret_item;
};
} // namespace Scripts
} // namespace Cordis
