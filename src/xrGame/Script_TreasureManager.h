#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_TreasureManager
{
private:
    Script_TreasureManager(void) : m_check_time(Globals::kUnsignedInt32Undefined)
    {
        CInifile ini = CInifile("misc\\secrets.ltx");
        std::uint32_t count_lines = ini.line_count("list");

        for (std::uint32_t i = 0; i < count_lines; ++i)
        {
            const char* N;
            const char* V;
            if (ini.r_line("list", i, &N, &V))
            {
                if (ini.section_exist(N))
                {
                    DataSecret data;
                    std::uint32_t item_count = ini.line_count(N);
                    xr_string item_section_name = "";

                    for (std::uint32_t j = 0; j < item_count; ++j)
                    {
                        const char* N1;
                        const char* V1;

                        if (ini.r_line(N, j, &N1, &V1))
                        {
                            if (!strcmp(N1, "empty"))
                            {
                                data.m_condlist_empty =
                                    XR_LOGIC::parse_condlist_by_server_object("treasure_manager", "empty_cond", V1);
                            }
                            else if (!strcmp(N1, "refreshing"))
                            {
                                data.m_condlist_refreshing = XR_LOGIC::parse_condlist_by_server_object(
                                    "treasure_manager", "refreshing_cond", V1);
                            }
                            else
                            {
                                xr_vector<std::pair<xr_string, float>> buffer =
                                    Globals::Utils::parse_spawns(V1);

                                if (!buffer.size())
                                {
                                    Msg("[Scripts/Script_TreasureManager/ctor] There is no items count set for "
                                        "treasure [%s], item [%s]",
                                        N, V1);
                                    R_ASSERT(false);
                                }

                                for (std::pair<xr_string, float>& it : buffer)
                                {
                                    std::pair<std::pair<std::uint32_t, std::uint32_t>, xr_vector<std::uint16_t>> data;
                                    data.first.first = atoi(it.first.c_str());
                                    data.first.second = it.second ? it.second : 1.0f;
                                    this->m_secrets[N].m_items[V1].push_back(data);
                                }
                            }
                        }
                    }
                }
                else
                {
                    Msg("[Scripts/Script_TreasureManager/ctor] There is no section [%s] in secrects.ltx", N);
                    R_ASSERT(false);
                }
            }
        }
        // if (ini.section_exist())
    }

public:
    struct DataSecret
    {
        bool m_is_given;
        bool m_is_checked;
        std::uint8_t m_to_find;
        xr_map<std::uint32_t, CondlistData> m_condlist_refreshing;
        xr_map<std::uint32_t, CondlistData> m_condlist_empty;

        // first -> count | second -> prob
        xr_map<xr_string, xr_vector<std::pair<std::pair<std::uint32_t, float>, xr_vector<std::uint16_t>>>> m_items;
    };

    inline static Script_TreasureManager& getInstance(void) noexcept
    {
        static Script_TreasureManager instance;
        return instance;
    }

    ~Script_TreasureManager(void);
    Script_TreasureManager(const Script_TreasureManager&) = delete;
    Script_TreasureManager& operator=(const Script_TreasureManager&) = delete;
    Script_TreasureManager(Script_TreasureManager&&) = delete;
    Script_TreasureManager& operator=(Script_TreasureManager&&) = delete;

    bool fill(CSE_ALifeDynamicObject* server_object, const xr_string& treasure_id_name);
    bool register_item(CSE_ALifeDynamicObject* server_object);
    void register_restrictor(CSE_ALifeDynamicObject* server_object);
    void update(void);
    void spawn_treasure(const xr_string& treasure_id_name);

private:
    bool m_is_items_spawned;
    std::uint32_t m_check_time;
    xr_map<xr_string, std::uint16_t> m_secret_restrictors;
    xr_map<std::uint16_t, xr_string> m_items_from_secrects;
    xr_map<xr_string, DataSecret> m_secrets;
};

} // namespace Scripts
} // namespace Cordis
