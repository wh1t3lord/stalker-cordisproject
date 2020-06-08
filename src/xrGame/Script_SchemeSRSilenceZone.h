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
    Script_SchemeSRSilenceZone(CScriptGameObject* const p_client_object, void* storage);
    ~Script_SchemeSRSilenceZone(void);
};
}
} // namespace Cordis

