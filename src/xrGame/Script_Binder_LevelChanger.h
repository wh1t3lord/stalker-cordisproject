#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_Binder_LevelChanger : public CScriptBinderObject
		{
		public:
			Script_Binder_LevelChanger(void) = delete;
			Script_Binder_LevelChanger(CScriptGameObject* const p_object);
			~Script_Binder_LevelChanger(void);

			virtual bool net_Spawn(SpawnType DC);
			virtual void net_Destroy();
			virtual void save(NET_Packet* output_packet);
			virtual void load(IReader* input_packet);
			virtual bool net_SaveRelevant();
		};

	}
}
