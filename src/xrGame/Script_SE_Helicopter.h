#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Helicopter : public CSE_ALifeHelicopter
{
    using inherited = CSE_ALifeHelicopter;
public:
    Script_SE_Helicopter(LPCSTR section_name);
    virtual ~Script_SE_Helicopter(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    
    virtual bool keep_saved_data_anyway(void);
};
} // namespace Scripts
} // namespace Cordis
