#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_InventoryBox : public CSE_ALifeInventoryBox
{
    using inherited = CSE_ALifeInventoryBox;

public:
    Script_SE_InventoryBox(LPCSTR section_name);
    virtual ~Script_SE_InventoryBox(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void) const;

private:
    bool m_is_secret_item;
};
} // namespace Scripts
} // namespace Cordis
