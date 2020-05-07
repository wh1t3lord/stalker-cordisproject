#include "stdafx.h"
#include "Script_Binder_SmartTerrain.h"

Cordis::Scripts::Script_Binder_SmartTerrain::Script_Binder_SmartTerrain(CScriptGameObject* const p_client_object)
{
}

Cordis::Scripts::Script_Binder_SmartTerrain::~Script_Binder_SmartTerrain(void)
{
}

bool Cordis::Scripts::Script_Binder_SmartTerrain::net_Spawn(SpawnType DC)
{
	if (!CScriptBinderObject::net_Spawn(DC))
		return false;

	this->m_p_server_instance = ai().alife().objects().object(DC->ID)->cast_script_se_smartterrain();

	if (this->m_p_server_instance == nullptr)
	{
		R_ASSERT2(false, "bad cast!");
		return false;
	}

	DataBase::Storage::getInstance().addZone(this->m_object);
	

	return true;
}

void Cordis::Scripts::Script_Binder_SmartTerrain::net_Destroy()
{
	XR_SOUND::stop_sounds_by_id(this->m_object->ID());

	DataBase::Storage::getInstance().deleteZone(this->m_object);
}

void Cordis::Scripts::Script_Binder_SmartTerrain::net_Relcase(CScriptGameObject* object)
{
	if (this->m_p_server_instance->getNpcInfo().find(object->ID()) != this->m_p_server_instance->getNpcInfo().end())
	{
		// Lord: понять зачем оно так было?
	}
}

void Cordis::Scripts::Script_Binder_SmartTerrain::shedule_Update(u32 time_delta)
{
	CScriptBinderObject::shedule_Update(time_delta);

	this->m_p_server_instance->update();
}
