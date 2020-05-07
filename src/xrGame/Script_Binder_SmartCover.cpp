#include "stdafx.h"
#include "Script_Binder_SmartCover.h"

Cordis::Scripts::Script_Binder_SmartCover::Script_Binder_SmartCover(CScriptGameObject* const p_client_object) : CScriptBinderObject(p_client_object)
{
}

Cordis::Scripts::Script_Binder_SmartCover::~Script_Binder_SmartCover(void)
{
}

bool Cordis::Scripts::Script_Binder_SmartCover::net_Spawn(SpawnType DC)
{
	if (!CScriptBinderObject::net_Spawn(DC))
		return false;

	DataBase::Storage::getInstance().setSmartCoversByName(this->m_object->Name(), this->m_object);
#ifdef DEBUG
	MESSAGE("smart_cover %s is registered!", this->m_object->Name());
#endif // DEBUG
	return true;
}

void Cordis::Scripts::Script_Binder_SmartCover::net_Destroy()
{
	DataBase::Storage::getInstance().setSmartCoversByName(this->m_object->Name(), nullptr);

#ifdef DEBUG
	MESSAGEI("smart_cover %s is unregistered!", this->m_object->Name());
#endif // DEBUG

	CScriptBinderObject::net_Destroy();
}

void Cordis::Scripts::Script_Binder_SmartCover::shedule_Update(u32 time_delta)
{
}
