#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_Binder_Phantom : public CScriptBinderObject
{
    using inherited = CScriptBinderObject;

public:
    Script_Binder_Phantom(void) = delete;
    Script_Binder_Phantom(CScriptGameObject* const p_client_object);
    virtual void net_Destroy(void);
};

}
} // namespace Cordis
