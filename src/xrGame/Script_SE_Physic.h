#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Physic : public CSE_ALifeObjectPhysic
{
    using inherited = CSE_ALifeObjectPhysic;

public:
    Script_SE_Physic(LPCSTR section_name);
    virtual ~Script_SE_Physic(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void) const;

private:
    bool m_is_secret_item;
};
} // namespace Scripts
} // namespace Cordis
