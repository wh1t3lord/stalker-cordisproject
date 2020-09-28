#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_Artefact : public CSE_ALifeItemArtefact
{
    using inherited = CSE_ALifeItemArtefact;
public:
    Script_SE_Artefact(LPCSTR section_name);
    virtual ~Script_SE_Artefact(void);

    virtual void on_register(void);
    virtual void on_unregister(void);
    virtual bool can_switch_offline(void) const;
    virtual bool can_switch_online(void) const;
};

} // namespace Scripts
} // namespace Cordis
