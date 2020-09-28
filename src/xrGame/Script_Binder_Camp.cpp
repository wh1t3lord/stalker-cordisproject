#include "stdafx.h"
#include "Script_Binder_Camp.h"

namespace Cordis
{
    namespace Scripts
{
    Script_Binder_Camp::Script_Binder_Camp(CScriptGameObject* const p_client_object) : inherited(p_client_object) {}

    Script_Binder_Camp::~Script_Binder_Camp(void) {}

    void Script_Binder_Camp::reinit() 
    { 
        inherited::reinit();
        DataBase::Storage::getInstance().setStorageCampsObject(this->m_object->ID(), this->m_object);
    }

    bool Script_Binder_Camp::net_Spawn(SpawnType DC) 
    {
        if (!inherited::net_Spawn(DC))
            return false;
        
        CScriptIniFile* const p_ini = this->m_object->spawn_ini();
        CScriptIniFile* p_camp_ini = nullptr;
        if (p_ini->section_exist("camp"))
        {
            xr_string filename = Globals::Utils::cfg_get_string(p_ini, "camp", "cfg");

            bool is_do_deallocation = false;
            if (!filename.empty())
            {
                p_camp_ini = new CScriptIniFile(filename.c_str());
                is_do_deallocation = true;
            }


            if (p_camp_ini == nullptr)
                p_camp_ini = p_ini;

            DataBase::Storage::getInstance().setStorageCampsCamp(this->m_object->ID(), new Script_CampData(this->m_object, p_camp_ini, is_do_deallocation));
        }

        return true;
    }

    void Script_Binder_Camp::net_Destroy() 
    {
        // @ Lord: Нужно ли здесь производить удаление? Может быть при переходе на локацию будет оставаться storage?
        inherited::net_Destroy();
    }

    void Script_Binder_Camp::shedule_Update(u32 time_delta) 
    {
        Script_CampData* const p_camp = DataBase::Storage::getInstance().getCampsCamp(this->m_object->ID());

        if (p_camp)
            p_camp->update();
    }

    bool Script_Binder_Camp::net_SaveRelevant() { return true; }
    } // namespace Scripts
}
