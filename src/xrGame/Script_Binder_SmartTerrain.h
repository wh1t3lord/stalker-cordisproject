#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_Binder_SmartTerrain : public CScriptBinderObject
		{
		public:
			Script_Binder_SmartTerrain(void) = delete;
			Script_Binder_SmartTerrain(CScriptGameObject* const p_client_object);
			~Script_Binder_SmartTerrain(void);

			virtual bool net_Spawn(SpawnType DC);
			virtual void net_Destroy();
			virtual void net_Relcase(CScriptGameObject* object);
			virtual void shedule_Update(u32 time_delta);
		private:
			Script_SE_SmartTerrain* m_p_server_instance;
		};
	}
}

