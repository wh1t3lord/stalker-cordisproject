#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Item : public CSE_ALifeItem
{
    using inherited = CSE_ALifeItem;

public:
    Script_SE_Item(LPCSTR section_name);
    virtual ~Script_SE_Item(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void) const;

private:
    bool m_is_secret_item;
};
} // namespace Scripts
} // namespace Cordis
