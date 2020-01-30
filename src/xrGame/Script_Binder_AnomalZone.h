#pragma once

#include "script_binder_object.h"

namespace Cordis
{
namespace Scripts
{
class Script_Binder_Anomaly : public CScriptBinderObject
{
    using inherited = CScriptBinderObject;

public:
    Script_Binder_Anomaly(void) = delete;
    Script_Binder_Anomaly(CScriptGameObject* object);
    virtual ~Script_Binder_Anomaly(void);

    virtual void reinit(void);
    virtual void reload(LPCSTR section_name);
    virtual bool net_Spawn(SpawnType DC);
    virtual void net_Destroy(void);
    virtual void net_Import(NET_Packet* packet);
    virtual void net_Export(NET_Packet* packet);
    virtual void shedule_Update(std::uint32_t time_delta);
    virtual void save(NET_Packet* output_packet);
    virtual void load(IReader* input_packet);
    virtual bool net_SaveRelevant(void);
    virtual void net_Relcase(CScriptGameObject* object);

    inline std::uint32_t getSpawnedCount(void) const noexcept { return this->m_spawned_count; }

private:
    bool m_is_created_own_ini_file_from_cfg_section;
    bool m_is_disabled;
    bool m_is_turned_off;
    bool m_is_respawn_artefacts;
    bool m_is_forced_spawn;
    bool m_is_forced_spawn_override;
    std::uint32_t m_spawned_count;
    std::uint32_t m_layers_count;
    CScriptIniFile* m_p_ini;
    xr_string m_forced_artefact_name;
};

} // namespace Scripts
} // namespace Cordis
