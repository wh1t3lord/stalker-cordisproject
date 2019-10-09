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

    virtual Script_SE_SmartCover* cast_script_se_smartcover(void) { return this; }
    virtual void STATE_Write(NET_Packet& Packet);
    virtual void STATE_Read(NET_Packet& Packet, std::uint16_t size);
    virtual void on_before_register(void);
    virtual void on_register(void);
    virtual void on_unregister(void);

    inline const xr_map<xr_string, bool>& getLoopholes(void) noexcept { return this->m_loopholes; }

    inline void setLoopholes(const xr_map<xr_string, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[Script_SE_SmartCover/setLoopholes(map)] WARNING: map.size() = 0! You are trying to set an empty map! No assignment!");
            return;
        }

        this->m_loopholes = map;
    }

    inline void setLoopholes(const std::pair<xr_string, bool>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[Script_SE_SmartCover/setLoopholes(pair)] WARNING: pair.first.size() = 0! You are trying to set an "
                "empty pair! No assignment!");
            return;
        }

        this->m_loopholes.insert(pair);
    }

    inline void setLoopholes(const xr_string& loophole_name, const bool& value) noexcept
    {
        if (!loophole_name.size())
        {
            Msg("[Script_SE_SmartCover/setLoopholes(loophole_name, value)] WARNING: loophole_name.size() = 0! You are "
                "trying to set an empty string! No assignment!");
            return;
        }

        this->m_loopholes[loophole_name] = value;
    }
    inline xr_string& getLastDescriptionName(void) noexcept { return this->m_lastdescription_name; }

private:
    xr_map<xr_string, bool> m_loopholes;
    xr_string m_lastdescription_name;
};
} // namespace Scripts
} // namespace Cordis
