#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Outfit : public CSE_ALifeItemCustomOutfit
{
    using inherited = CSE_ALifeItemCustomOutfit;

public:
    Script_SE_Outfit(LPCSTR section_name);
    virtual ~Script_SE_Outfit(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online() const;

private:
    bool m_is_secret_item;
};

}
} // namespace Cordis
