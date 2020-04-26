#include "stdafx.h"
#include "Script_Binder_Campfire.h"

namespace Cordis
{
	namespace Scripts
	{
		Script_Binder_Campfire::Script_Binder_Campfire(CScriptGameObject* const p_client_object) : inherited(p_client_object)
		{
			
		}

		Script_Binder_Campfire::~Script_Binder_Campfire(void)
		{
		}

		bool Script_Binder_Campfire::net_Spawn(SpawnType DC)
		{
			if (inherited::net_Spawn(DC) == false)
				return false;
			

		}

		void Script_Binder_Campfire::shedule_Update(u32 time_delta)
		{
		}
	}
}