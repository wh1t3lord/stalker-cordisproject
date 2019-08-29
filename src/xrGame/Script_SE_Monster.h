#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Monster : public CSE_ALifeMonsterBase
{
    using inherited = CSE_ALifeMonsterBase;

public:
    Script_SE_Monster(LPCSTR section_name);
    virtual ~Script_SE_Monster(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual void on_death(CSE_Abstract* killer);
    virtual void STATE_Read(NET_Packet& packet, std::uint16_t size);
    virtual void STATE_Write(NET_Packet& packet);
    virtual bool can_switch_offline(void) const;
    virtual bool can_switch_online(void) const;

private:
    bool m_is_registered;
};
}
} // namespace Cordis

