#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Restrictor : public CSE_ALifeSpaceRestrictor
{
    using inherited = CSE_ALifeSpaceRestrictor;

public:
    Script_SE_Restrictor(LPCSTR section_name);
    virtual ~Script_SE_Restrictor(void);

    virtual void on_register(void);
    virtual bool keep_saved_data_anyway(void);
};
} // namespace Scripts
} // namespace Cordis
