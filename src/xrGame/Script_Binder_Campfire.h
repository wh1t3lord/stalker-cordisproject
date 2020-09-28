#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_Binder_Campfire : public CScriptBinderObject
		{
		public:
			Script_Binder_Campfire(void) = delete;
			Script_Binder_Campfire(CScriptGameObject* const p_client_object);
			~Script_Binder_Campfire(void);

			virtual bool net_Spawn(SpawnType DC);
			virtual void shedule_Update(u32 time_delta);

		private:
			CZoneCampfire* m_p_campfire;
		};
	}
}

