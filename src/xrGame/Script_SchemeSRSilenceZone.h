#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SchemeSRSilenceZone : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemeSRSilenceZone(void) = delete;
    Script_SchemeSRSilenceZone(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_SRSilenceZone* storage);
    ~Script_SchemeSRSilenceZone(void);

private: 
    DataBase::Script_ComponentScheme_SRSilenceZone* m_p_storage;
};
}
} // namespace Cordis

