#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_Binder_Artefact : public CScriptBinderObject
		{
			using inherited = CScriptBinderObject;

		public:
			Script_Binder_Artefact(void) = delete;
			Script_Binder_Artefact(CScriptGameObject* const p_client_object);
			~Script_Binder_Artefact(void);

			virtual bool net_Spawn(SpawnType DC);
			virtual void net_Destroy();
			virtual void shedule_Update(u32 time_delta);

		private:
			bool m_is_first_call;
		};

	}
}
