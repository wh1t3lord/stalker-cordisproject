#pragma once

namespace Cordis
{
    namespace Scripts
    {
    class Script_Binder_Camp : public CScriptBinderObject
    {
        using inherited = CScriptBinderObject;

    public:
        Script_Binder_Camp(void) = delete;
        Script_Binder_Camp(CScriptGameObject* const p_client_object);
        ~Script_Binder_Camp(void);

        virtual void reinit();
        virtual bool net_Spawn(SpawnType DC);
        virtual void net_Destroy();
        virtual void shedule_Update(u32 time_delta);
        virtual bool net_SaveRelevant();
    };
    }
}

