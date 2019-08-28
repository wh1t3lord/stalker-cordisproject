#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Stalker : public CSE_ALifeHumanStalker
{
    using inherited = CSE_ALifeHumanStalker;

public:
    Script_SE_Stalker(LPCSTR section_name);
    virtual ~Script_SE_Stalker(void);

    virtual bool can_switch_online(void) const;
    virtual bool can_switch_offline(void) const;
    virtual void STATE_Write(NET_Packet& packet);
    virtual void STATE_Read(NET_Packet& packet, const std::uint16_t& size);
    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual void on_death(CSE_Abstract* killer);

    inline bool IsDroppedDeath(void) { return this->m_is_dropped_death; }
    inline bool setDroppedDeath(const bool& value) { this->m_is_dropped_death = value; };

private:
    bool m_is_registered;
    bool m_is_dropped_death;
};
} // namespace Scripts
} // namespace Cordis
