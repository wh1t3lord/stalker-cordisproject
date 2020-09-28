#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Lamp : public CSE_ALifeObjectHangingLamp
{
    using inherited = CSE_ALifeObjectHangingLamp;

public:
    Script_SE_Lamp(LPCSTR section_name);
    virtual ~Script_SE_Lamp(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void) const;

private:
    bool m_is_secret_item;
};
} // namespace Scripts
} // namespace Cordis
