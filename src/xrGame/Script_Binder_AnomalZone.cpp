#include "stdafx.h"
#include "Script_Binder_AnomalZone.h"

namespace Cordis
{
namespace Scripts
{
Script_Binder_Anomaly::Script_Binder_Anomaly(CScriptGameObject* object)
    : inherited(object), m_is_created_own_ini_file_from_cfg_section(false), m_is_disabled(false),
      m_is_turned_off(false), m_is_respawn_artefacts(true), m_is_forced_spawn(false), m_is_forced_spawn_override(false)
{
    this->m_p_ini = object->spawn_ini();

    if (!this->m_p_ini)
    {
        R_ASSERT2(false, "object is null!! something is wrong");
    }

    if (!this->m_p_ini->section_exist("anomal_zone"))
    {
        Msg("[Scripts/Script_Binder_Anomaly/ctor(object)] WARNING: no configuration!");
        this->m_is_disabled = true;
        return;
    }

    xr_string filename = Globals::Utils::cfg_get_string(this->m_p_ini, "anomal_zone", "cfg");
    if (!filename.empty())
    {
        this->m_is_created_own_ini_file_from_cfg_section = true;
        this->m_p_ini = new CScriptIniFile(filename.c_str());
    }

    this->m_layers_count = static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(this->m_p_ini, "anomal_zone", "layers_count"));

    if (!this->m_layers_count)
        this->m_layers_count = 1;
}

Script_Binder_Anomaly::~Script_Binder_Anomaly(void)
{
    if (this->m_is_created_own_ini_file_from_cfg_section)
    {
        Msg("[Scripts/Script_Binder_Anomaly/~dtor()] deleting m_p_ini %s", this->m_p_ini->fname());
        xr_delete(this->m_p_ini);
    }
}

void Script_Binder_Anomaly::reinit(void) {}

void Script_Binder_Anomaly::reload(LPCSTR section_name) {}

bool Script_Binder_Anomaly::net_Spawn(SpawnType DC) { return false; }

void Script_Binder_Anomaly::net_Destroy(void) {}

void Script_Binder_Anomaly::net_Import(NET_Packet* packet) {}

void Script_Binder_Anomaly::net_Export(NET_Packet* packet) {}

void Script_Binder_Anomaly::shedule_Update(std::uint32_t time_delta) {}

void Script_Binder_Anomaly::save(NET_Packet* output_packet) {}

void Script_Binder_Anomaly::load(IReader* input_packet) {}

bool Script_Binder_Anomaly::net_SaveRelevant(void) { return false; }

void Script_Binder_Anomaly::net_Relcase(CScriptGameObject* object) {}
} // namespace Scripts
} // namespace Cordis
