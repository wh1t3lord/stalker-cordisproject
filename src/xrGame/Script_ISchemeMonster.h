#pragma once

namespace Cordis
{
namespace Scripts
{
// @ Interface
class Script_ISchemeMonster : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_ISchemeMonster(void) = delete;
    Script_ISchemeMonster(CScriptGameObject* client_object, DataBase::Storage_Scheme& storage_scheme)
        : inherited_scheme(client_object, storage_scheme)
    {
    }

    virtual ~Script_ISchemeMonster(void) {}
};
} // namespace Scripts
} // namespace Cordis
