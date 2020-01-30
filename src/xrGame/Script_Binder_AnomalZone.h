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

    void disable_anomaly_fields(void);

private:
    bool m_is_created_own_ini_file_from_cfg_section;
    bool m_is_custom_placement;
    bool m_is_disabled;
    bool m_is_turned_off;
    bool m_is_respawn_artefacts;
    bool m_is_forced_spawn;
    bool m_is_forced_spawn_override;
    std::uint32_t m_spawned_count;
    std::uint32_t m_layers_count;
    std::uint32_t m_respawn_tries;
    std::uint32_t m_max_artefacts;
    std::uint32_t m_applying_force_xz;
    std::uint32_t m_applying_force_y;
    CScriptIniFile* m_p_ini;
    xr_map<xr_string, std::uint32_t> m_table_respawn_tries;
    xr_map<xr_string, std::uint32_t> m_table_max_artefacts;
    // .first -> xz | .second -> y
    xr_map<xr_string, std::pair<std::uint32_t, std::uint32_t>> m_table_forces;
    xr_map<xr_string, xr_string> m_table_mines;
    xr_map<xr_string, xr_vector<xr_string>> m_table_artefacts;
    xr_map<xr_string, xr_vector<xr_string>> m_table_start_artefacts;
    xr_map<xr_string, xr_vector<xr_string>> m_table_path;
    xr_map<xr_string, xr_vector<xr_string>> m_table_fields;
    xr_map<xr_string, xr_vector<int>> m_table_artefacts_coeff;
    xr_string m_forced_artefact_name;
    xr_string m_current_layer_name;
};

} // namespace Scripts
} // namespace Cordis
