#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Ammo : public CSE_ALifeItemAmmo
{
    using inherited = CSE_ALifeItemAmmo;

public:
    Script_SE_Ammo(LPCSTR section_name);
    virtual ~Script_SE_Ammo(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void) const;

private:
    bool m_is_secret_item;
};
} // namespace Scripts
} // namespace Cordis
