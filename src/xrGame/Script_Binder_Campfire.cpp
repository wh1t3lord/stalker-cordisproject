#include "stdafx.h"
#include "Script_Binder_Campfire.h"

namespace Cordis
{
	namespace Scripts
	{
		Script_Binder_Campfire::Script_Binder_Campfire(CScriptGameObject* const p_client_object) : CScriptBinderObject(p_client_object), m_p_campfire(this->m_object->get_campfire())
		{
			
		}

		Script_Binder_Campfire::~Script_Binder_Campfire(void)
		{
		}

		bool Script_Binder_Campfire::net_Spawn(SpawnType DC)
		{
			if (CScriptBinderObject::net_Spawn(DC) == false)
				return false;
			
            xr_string smart_name = this->m_object->Name();

            if (smart_name.find("_campfire") != xr_string::npos)
                smart_name.erase(smart_name.find("_campfire"));
            
            if (Script_SimulationBoard::getInstance().getSmartTerrainsByName().find(smart_name) != Script_SimulationBoard::getInstance().getSmartTerrainsByName().end())
            {
                this->m_p_campfire->turn_off_script();

                DataBase::Storage::getInstance().setCampfiresBySmartName(
                    smart_name, this->m_object->ID(), this->m_p_campfire);
            }

            return true;
		}

		void Script_Binder_Campfire::shedule_Update(u32 time_delta) { CScriptBinderObject::shedule_Update(time_delta); }
	}
}
