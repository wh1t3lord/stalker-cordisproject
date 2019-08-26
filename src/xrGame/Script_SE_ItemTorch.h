#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_ItemTorch : public CSE_ALifeItemTorch
{
    using inherited = CSE_ALifeItemTorch;

public:
    Script_SE_ItemTorch(LPCSTR section_name);
    virtual ~Script_SE_ItemTorch(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void) const;

private:
    bool m_is_secret_item;
};
} // namespace Scripts
} // namespace Cordis
