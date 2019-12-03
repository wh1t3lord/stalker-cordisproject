#pragma once

namespace Cordis
{
namespace Scripts
{
    // @ Можете доделать!
    // Lord: не используется
class Script_SchemePHSound : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemePHSound(void) = delete;
    Script_SchemePHSound(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage);
    ~Script_SchemePHSound(void);

private:
    bool m_is_destructed;
};
}
} // namespace Cordis

