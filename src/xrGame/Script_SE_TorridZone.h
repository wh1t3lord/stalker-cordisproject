#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_TorridZone : public CSE_ALifeTorridZone
{
    using inherited = CSE_ALifeTorridZone;

public:
    Script_SE_TorridZone(LPCSTR section_name);
    virtual ~Script_SE_TorridZone(void);

    virtual void on_register(void);
    virtual void STATE_Read(NET_Packet& packet, std::uint16_t size);

private:
    bool m_is_registered;
};
} // namespace Scripts
} // namespace Cordis
