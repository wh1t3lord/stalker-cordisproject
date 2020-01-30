#include "stdafx.h"
#include "Script_Binder_AnomalField.h"

namespace Cordis
{
namespace Scripts
{
Script_Binder_AnomalField::Script_Binder_AnomalField(CScriptBinderObject* object) : inherited(object), m_last_update(0)
{
    this->m_last_update = Globals::get_time_global();
}

Script_Binder_AnomalField::~Script_Binder_AnomalField(void) {}

void Script_Binder_AnomalField::reinit(void) { inherited::reinit(); }

void Script_Binder_AnomalField::reload(LPCSTR section_name) { inherited::reload(section_name); }

bool Script_Binder_AnomalField::net_Spawn(SpawnType DC)
{
    if (!inherited::net_Spawn(DC))
        return false;

    DataBase::Storage::getInstance().addZone(this->m_object);
    DataBase::Storage::getInstance().addObject(this->m_object);
    DataBase::Storage::getInstance().setFieldsByName(this->m_object->Name(), this);

    return true;
}

void Script_Binder_AnomalField::net_Destroy(void)
{
    DataBase::Storage::getInstance().deleteZone(this->m_object);
    DataBase::Storage::getInstance().deleteObject(this->m_object);
    // Lord: здесь производится зануление стороджа нужно ли делать?
    DataBase::Storage::getInstance().setFieldsByName(this->m_object->Name(), nullptr);

    inherited::net_Destroy();
}

void Script_Binder_AnomalField::shedule_Update(std::uint32_t time_delta) 
{
    inherited::shedule_Update(time_delta); 

    if (Globals::get_time_global() - this->m_last_update < 10000)
        return;

    this->m_last_update = Globals::get_time_global();
}

bool Script_Binder_AnomalField::net_SaveRelevant(void) { return true; }
} // namespace Scripts
} // namespace Cordis
