#pragma once

#include "action_base.h"

namespace Cordis
{
namespace Scripts
{
// @ Interface
class Script_ISchemeStalker : public CScriptActionBase, public Script_ISchemeEntity
{
private:
    using inherited_scheme = Script_ISchemeEntity;
    using inherited = CScriptActionBase;
public:
    Script_ISchemeStalker(void) = delete;
    Script_ISchemeStalker(CScriptGameObject* const p_client_object, const xr_string& action_name, DataBase::Script_IComponentScheme* storage_scheme)
        : inherited(nullptr, action_name.c_str()), inherited_scheme(p_client_object, storage_scheme)
    {
    
    }
    virtual ~Script_ISchemeStalker(void) {};
};

}
} // namespace Cordis

