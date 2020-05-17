#include "stdafx.h"
#include "Script_Binder_LevelChanger.h"

namespace Cordis
{
	namespace Scripts
	{
		Script_Binder_LevelChanger::Script_Binder_LevelChanger(CScriptGameObject* const p_object) : CScriptBinderObject(p_object)
		{
		}
		
		Script_Binder_LevelChanger::~Script_Binder_LevelChanger(void)
		{
		}

		bool Script_Binder_LevelChanger::net_Spawn(SpawnType DC)
		{
			if (!CScriptBinderObject::net_Spawn(DC))
				return false;

			DataBase::Storage::getInstance().addObject(this->m_object);
			CSE_ALifeObject* const p_server_object = ai().alife().objects().object(this->m_object->ID());
			DataBase::Storage::getInstance().setStorageServerObject(this->m_object->ID(), p_server_object);

			this->m_object->enable_level_changer(false);
			this->m_object->set_level_changer_invitation(nullptr);
				
			return true;
		}
		
		void Script_Binder_LevelChanger::net_Destroy()
		{
			DataBase::Storage::getInstance().deleteObject(this->m_object);
			CScriptBinderObject::net_Destroy();
		}
		
		void Script_Binder_LevelChanger::save(NET_Packet* output_packet)
		{
			Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, false, "level_changer_binder");
			CScriptBinderObject::save(output_packet);
			XR_LOGIC::save_object(this->m_object, *output_packet);
			Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, true, "level_changer_binder");
		}
		
		void Script_Binder_LevelChanger::load(IReader* input_packet)
		{
			Globals::set_save_marker(*input_packet, Globals::kSaveMarkerMode_Load, false, "level_changer_binder");
			CScriptBinderObject::load(input_packet);
			XR_LOGIC::load_object(this->m_object, *input_packet);
			Globals::set_save_marker(*input_packet, Globals::kSaveMarkerMode_Load, true, "level_changer_binder");
		}

		bool Script_Binder_LevelChanger::net_SaveRelevant()
		{
			return true;
		}
	}
}