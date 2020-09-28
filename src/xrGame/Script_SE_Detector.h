#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Detector : public CSE_ALifeItemDetector
{
    using inherited = CSE_ALifeItemDetector;

public:
    Script_SE_Detector(LPCSTR section_name);
    virtual ~Script_SE_Detector(void);
    
    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_online(void) const;

private:
    bool m_is_secret_item;
};
} // namespace Scripts
} // namespace Cordis
