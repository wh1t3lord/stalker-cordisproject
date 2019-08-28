#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Squad : public CSE_ALifeOnlineOfflineGroup
{
    using inherited = CSE_ALifeOnlineOfflineGroup;

public:
    Script_SE_Squad(LPCSTR section_name);
    virtual ~Script_SE_Squad(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
};

} // namespace Scripts
} // namespace Cordis
