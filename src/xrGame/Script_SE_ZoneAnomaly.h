#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_ZoneAnomaly : public CSE_ALifeAnomalousZone
{
    using inherited = CSE_ALifeAnomalousZone;

public:
    Script_SE_ZoneAnomaly(LPCSTR section_name);
    virtual ~Script_SE_ZoneAnomaly(void);
    
    virtual void on_register(void);
    virtual void STATE_Read(NET_Packet& packet, std::uint16_t size);

private:
    bool m_is_registered;
};
}
} // namespace Cordis

