#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_Binder_SmartCover : public CScriptBinderObject
		{
		public:
			Script_Binder_SmartCover(void) = delete;
			Script_Binder_SmartCover(CScriptGameObject* const p_client_object);
			~Script_Binder_SmartCover(void);

			virtual bool net_Spawn(SpawnType DC);
			virtual void net_Destroy();
			virtual void shedule_Update(u32 time_delta);
		};
	}
}
