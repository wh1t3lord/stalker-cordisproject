#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_SmartCover : public CSE_SmartCover
{
    using inherited = CSE_SmartCover;

public:
    Script_SE_SmartCover(LPCSTR section_name);
    virtual ~Script_SE_SmartCover(void);

    virtual void STATE_Write(NET_Packet& Packet);
    virtual void STATE_Read(NET_Packet& Packet, std::uint16_t size);
    virtual void on_before_register(void);
    virtual void on_register(void);
    virtual void on_unregister(void);

    inline xr_map<xr_string, bool>& getLoopholes(void) noexcept { return this->m_loopholes; }
    inline xr_string& getLastDescriptionName(void) noexcept { return this->m_lastdescription_name; }

private:
    xr_map<xr_string, bool> m_loopholes;
    xr_string m_lastdescription_name;
};
} // namespace Scripts
} // namespace Cordis
