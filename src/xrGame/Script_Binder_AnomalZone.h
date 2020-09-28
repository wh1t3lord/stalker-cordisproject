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
    virtual void shedule_Update(std::uint32_t time_delta);
    virtual void save(NET_Packet* output_packet);
    virtual void load(IReader* input_packet);
    virtual bool net_SaveRelevant(void);

    inline std::uint32_t getSpawnedCount(void) const noexcept { return this->m_spawned_count; }
    inline void set_forced_override(const xr_string& artefact_name)
    {
        if (artefact_name.empty())
            Msg("[Scripts/Script_Binder_AnomalZone/set_forced_override(artefact_name)] WARNING: artefact_name.empty() "
                "== true! You set an empty string");

        this->m_forced_artefact_name = artefact_name;
        this->m_is_forced_spawn_override = true;

        Msg("[Scripts/Script_Binder_AnomalZone/set_forced_override(artefact_name)] set forced override zone %s, "
            "artefact %s",
            this->m_object->Name(), artefact_name.c_str());
    }

    inline const xr_map<std::uint16_t, xr_string>& getArtefactWaysByID(void) const noexcept
    {
        return this->m_artefact_ways_by_id;
    }

    void disable_anomaly_fields(void);
    void respawn_artefacts_and_replace_anomaly_zone(void);
    void spawn_artefact_randomly(void);
    xr_string get_artefact_path(void);
    void turn_on(const bool value);
    void turn_off(void);

    // @ For Client & for Server
    void on_artefact_take(const std::uint16_t object_id);

    inline float getApplyingForceXZ(void) const noexcept { return this->m_applying_force_xz; }
    inline float getApplyingForceY(void) const noexcept { return this->m_applying_force_y; }

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
    float m_applying_force_xz;
    float m_applying_force_y;
    CScriptIniFile* m_p_ini;
    xr_map<std::uint16_t, xr_string> m_artefact_ways_by_id;
    xr_map<std::uint16_t, std::uint32_t> m_artefact_points_by_id;
    xr_map<xr_string, std::uint32_t> m_table_respawn_tries;
    xr_map<xr_string, std::uint32_t> m_table_max_artefacts;
    // .first -> xz | .second -> y
    xr_map<xr_string, std::pair<float, float>> m_table_forces;
    xr_map<xr_string, xr_vector<xr_string>> m_table_mines;
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
