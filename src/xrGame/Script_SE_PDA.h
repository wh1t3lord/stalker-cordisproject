#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_PDA : public CSE_ALifeItemPDA
{
    using inherited = CSE_ALifeItemPDA;

public:
    Script_SE_PDA(LPCSTR section_name);
    virtual ~Script_SE_PDA(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void) const;

private:
    bool m_is_secret_item;
};
} // namespace Scripts
} // namespace Cordis
