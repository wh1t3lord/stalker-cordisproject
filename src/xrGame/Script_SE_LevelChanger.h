#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_LevelChanger : public CSE_ALifeLevelChanger
{
    using inherited = CSE_ALifeLevelChanger;

public:
    Script_SE_LevelChanger(LPCSTR section_name);
    virtual ~Script_SE_LevelChanger(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual void STATE_Write(NET_Packet& packet);
    virtual void STATE_Read(NET_Packet& packet, const std::uint16_t size);
    
private: 
    bool m_is_enabled;
    xr_string m_hint_name;
};
} // namespace Scripts
} // namespace Cordis
