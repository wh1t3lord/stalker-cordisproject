#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Helmet : public CSE_ALifeItemHelmet
{
    using inherited = CSE_ALifeItemHelmet;
public:
    Script_SE_Helmet(LPCSTR section_name);
    virtual ~Script_SE_Helmet(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void) const;

private:
    bool m_is_secret_item;
};
} // namespace Scripts
} // namespace Cordis
