#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Explosive : public CSE_ALifeItemExplosive
{
    using inherited = CSE_ALifeItemExplosive;

public:
    Script_SE_Explosive(LPCSTR section_name);
    virtual ~Script_SE_Explosive(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void) const;

private:
    bool m_is_secret_item;
};
} // namespace Scripts
} // namespace Cordis
