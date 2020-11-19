#pragma once

namespace Cordis
{
namespace Scripts
{
struct CondlistData
{
    struct CondlistValues
    {
        bool m_required = false;
        bool m_expected = false;
        std::uint32_t m_prob = 0;
        xr_string m_function_name = "";
        xr_string m_infopotion_name = "";
        xr_string m_params = "";

        inline void clear(void) noexcept
        {
            this->m_required = false;
            this->m_expected = false;
            this->m_prob = 0;
            this->m_function_name.clear();
            this->m_infopotion_name.clear();
            this->m_params.clear();
        }
    };

    xr_map<std::uint32_t, CondlistValues> m_infop_check;
    xr_map<std::uint32_t, CondlistValues> m_infop_set;
    xr_string m_text_name;
};

struct CondlistParsingData
{
    xr_string m_text_name;
    xr_string m_infocheck_name;
    xr_string m_infoset_name;

    inline void Clear(void) noexcept
    {
        this->m_infocheck_name.clear();
        this->m_text_name.clear();
        this->m_infoset_name.clear();
    }
};

struct SmartCoverLoopholeData
{
    struct SmartCoverActionsData
    {
        inline void register_animation(const xr_string& type_name, const xr_string& animation_name) noexcept
        {
            if (this->m_animations[type_name].size())
            {
                for (xr_string& it : this->m_animations[type_name])
                {
                    if (it == animation_name)
                    {
                        Msg("[Scripts/SmartCoverLoopholeData/SmartCoverActionsData/register_animation(type_name, "
                            "animation_name)] You are trying to register an existing animation! [%s]",
                            animation_name.c_str());

                        Msg("[Scripts/SmartCoverLoopholeData/SmartCoverActionsData/register_animation(type_name, "
                            "animation_name)] Ignored!");

                        return;
                    }
                }
            }

            this->m_animations[type_name].push_back(animation_name);
        }

        inline const xr_map<xr_string, xr_vector<xr_string>>& getAnimations(void) const noexcept
        {
            return this->m_animations;
        }

    private:
        xr_map<xr_string, xr_vector<xr_string>> m_animations;

    public:
        xr_string m_id; // idle, lookout, fire, fire_no_lookout, reload, etc
    };

    struct SmartCoverTransitionsData
    {
        float m_weight;
        xr_vector<xr_string> m_animations;
        xr_string m_action_from;
        xr_string m_action_to;
    };

    bool m_is_usable;
    bool m_is_exitable;
    bool m_is_enterable;
    float m_fieldofview;
    float m_fieldofview_danger;
    float m_range;
    Fvector m_fieldofview_position;
    Fvector m_fieldofview_direction;
    Fvector m_enter_direction;
    Fvector m_danger_fieldofview_direction;

private:
    xr_vector<SmartCoverActionsData> m_actions;
    xr_vector<SmartCoverTransitionsData> m_transitions;

public:
    xr_string m_id = "";

    inline void register_action(const SmartCoverActionsData& data) noexcept { this->m_actions.push_back(data); }
    inline void register_transition(const SmartCoverTransitionsData& data) noexcept
    {
        this->m_transitions.push_back(data);
    }
    inline const xr_vector<SmartCoverActionsData>& getActions(void) const noexcept { return this->m_actions; }
    inline const xr_vector<SmartCoverTransitionsData>& getTransitions(void) const noexcept
    {
        return this->m_transitions;
    }
};

struct SmartCoverData
{
private:
    bool m_is_need_weapon;
    xr_vector<SmartCoverLoopholeData> m_loopholes;

public:
    struct SmartCoverTransitionsData
    {
        struct SmartCoverActionsData
        {
            xr_string m_precondition_functor;
            xr_string m_preconditions_params;
            struct Actions
            {
                std::uint32_t m_body_state;
                std::uint32_t m_movement_type;
                Fvector m_position;
                xr_string m_animation;
            };
            Actions m_action;
        };
        float m_weight;
        xr_string m_vertex0;
        xr_string m_vertex1;
        xr_vector<SmartCoverActionsData> m_actions;
    };

private:
    xr_vector<SmartCoverTransitionsData> m_transitions;

public:
    inline bool IsNeedWeapon(void) const noexcept { return this->m_is_need_weapon; }
    inline void setNeedWeapon(const bool& value) noexcept { this->m_is_need_weapon = value; }

    inline void register_loophole(const SmartCoverLoopholeData& data) noexcept { this->m_loopholes.push_back(data); }
    inline void register_transition(const SmartCoverTransitionsData& data) noexcept
    {
        this->m_transitions.push_back(data);
    }

    inline const xr_vector<SmartCoverLoopholeData>& getLoopholes(void) const noexcept { return this->m_loopholes; }
    inline void setLoopholes(const xr_vector<SmartCoverLoopholeData>& vector) noexcept
    {
        if (!vector.size())
        {
            Msg("[SmartCoverData/setLoopholes(vector)] WARNING: vector.size() = 0! You are trying to set an empty "
                "vector! No assignment!");
            return;
        }

        this->m_loopholes = vector;
    }
    inline void setLoopholes(const SmartCoverLoopholeData& data) noexcept { this->m_loopholes.push_back(data); }
    inline const xr_vector<SmartCoverTransitionsData>& getTransitions(void) const noexcept
    {
        return this->m_transitions;
    }

    inline void setTransitions(const xr_vector<SmartCoverTransitionsData>& vector) noexcept
    {
        if (!vector.size())
        {
            Msg("[SmartCoverData/setTransitions(vector)] WARNING: vector.size() = 0! You are trying to set an empty "
                "vector! No assignment!");
            return;
        }

        this->m_transitions = vector;
    }

    inline void setTransitions(const SmartCoverTransitionsData& data) noexcept { this->m_transitions.push_back(data); }
};

class DialogData
{
public:
    DialogData(void) = default;
    ~DialogData(void) = default;

    inline const xr_map<std::uint32_t, CondlistData::CondlistValues>& getInfo(void) const noexcept
    {
        return this->m_info;
    }
    inline const xr_vector<xr_string>& getNpcCommunities(void) const noexcept { return this->m_npc_communities; }
    inline const xr_vector<xr_string>& getActorCommunities(void) const noexcept { return this->m_actor_communities; }
    inline const xr_vector<xr_string>& getLevels(void) const noexcept { return this->m_levels; }
    inline const xr_string& getName(void) const noexcept { return this->m_name; }
    inline const xr_string& getWounded(void) const noexcept { return this->m_wounded_type; }
    inline const xr_string& getOnceStateName(void) const noexcept { return this->m_once_state_name; }
    inline const xr_string& getSmartTerrainName(void) const noexcept { return this->m_smart_terrain_name; }

    inline std::uint32_t getID(void) const noexcept { return this->m_id; }

    inline void setName(const xr_string& name) noexcept
    {
        if (name.empty())
        {
            Msg("[Scripts/DialogData/setName(name)] WARNING: name.empty() == true! You are trying to set an empty "
                "string");
        }

        this->m_name = name;
    }
    inline void setWounded(const xr_string& wounded_name) noexcept
    {
        if (wounded_name.empty())
        {
            Msg("[Scripts/DialogData/setWounded(wounded_name)] WARNING: wounded_name.empty() == true! You are trying "
                "to set an empty string");
        }

        this->m_wounded_type = wounded_name;
    }
    inline void setID(const std::uint32_t id) noexcept
    {
        if (id == std::uint16_t(-1))
        {
            Msg("[Scripts/DialogData/setID(id)] WARNING: id == std::uint16_t(-1)! You are trying to set an undefined "
                "value");
        }

        this->m_id = id;
    }
    inline void setSmartTerrainName(const xr_string& smart_terrain_name) noexcept
    {
        if (smart_terrain_name.empty())
        {
            Msg("[Scripts/DialogData/setSmartTerrainName(smart_terrain_name)] WARNING: smart_terrain_name.empty() == "
                "true! Empty string to assign");
        }

        this->m_smart_terrain_name = smart_terrain_name;
    }
    inline void setOnceStateName(const xr_string& once_name) noexcept
    {
        if (once_name.empty())
        {
            Msg("[Scripts/DialogData/setOnceStateName(once_name)] WARNING: once_name.empty() == true! Empty string to "
                "assign");
        }

        this->m_once_state_name = once_name;
    }

    inline void setActorCommunities(const xr_vector<xr_string>& actor_communities) noexcept
    {
        if (actor_communities.empty())
        {
            Msg("[Scripts/DialogData/setActorCommunityName(actor_community_name)] WARNING: "
                "actor_community_name.empty() == true! Empty string to assign");
        }

        this->m_actor_communities = actor_communities;
    }

    inline void setLevels(const xr_vector<xr_string>& levels) noexcept
    {
        if (levels.empty())
        {
            Msg("[Scripts/DialogData/setLevelName(level_name)] WARNING: level_name.empty() == true! Empty string to "
                "assign");
        }

        this->m_levels = levels;
    }
    inline void setNpcCommunities(const xr_vector<xr_string>& communities) noexcept
    {
        if (communities.empty())
        {
            Msg("[Scripts/DialogData/setNpcCommunityName(community_name)] WARNING: community_name.empty() == true! "
                "Empty string to assign");
        }

        this->m_npc_communities = communities;
    }

    inline void setInfo(const xr_map<std::uint32_t, CondlistData::CondlistValues>& map) noexcept
    {
        if (map.empty())
        {
            Msg("[Scripts/DIalogData/setInfo(map)] WARNING: map.empty() == true! Empty map to assign!");
        }

        this->m_info = map;
    }

private:
    std::uint32_t m_id;
    xr_vector<xr_string> m_npc_communities;
    xr_vector<xr_string> m_levels;
    xr_vector<xr_string> m_actor_communities;
    xr_string m_name;
    xr_string m_once_state_name;
    xr_string m_smart_terrain_name;
    xr_string m_wounded_type;
    xr_map<std::uint32_t, CondlistData::CondlistValues> m_info;
};

// @ Используется в логике монстров
class CondlistWaypoints
{
public:
    class CondlistWayPointsData
    {
    public:
        CondlistWayPointsData(void) = default;
        ~CondlistWayPointsData(void) = default;

        inline const Flags32& getFlags(void) const noexcept { return this->m_flags; }
        inline void setFlags(const Flags32& flags) noexcept { this->m_flags = flags; }

        inline xr_string getValue(const xr_string& key_name) const noexcept
        {
            if (key_name.empty())
            {
                MESSAGEWR("key_name.empty() == true! Can't find "
                    "with empty string key!");
                return xr_string();
            }

            if (this->m_data.find(key_name) == this->m_data.end())
            {
                MESSAGEWR("can't find key_name %s in map!",
                    key_name.c_str());
                return xr_string();
            }

            return this->m_data.at(key_name);
        }

        inline void setData(const xr_string& field_name, const xr_string& value_name) noexcept
        {
            if (field_name.empty())
            {
                Msg("field_name.empty() == true! Can't assign an empty string!");
                return;
            }

            this->m_data[field_name] = value_name;
        }

    private:
        xr_map<xr_string, xr_string> m_data;
        Flags32 m_flags;
    };

    CondlistWaypoints(void) = default;
    ~CondlistWaypoints(void) = default;
    inline const xr_vector<CondlistWayPointsData>& getData(void) const noexcept { return this->m_data; }

    inline void setData(const CondlistWayPointsData& data) noexcept { this->m_data.push_back(data); }
    inline void clear(void) noexcept { this->m_data.clear(); }
private:
    xr_vector<CondlistWayPointsData> m_data;
};

class LogicData
{
public:
    LogicData(void) = default;
    ~LogicData(void) {}

    inline std::uint16_t getNpcID(void) const noexcept { return this->m_npc_id; }
    inline void setNpcID(const std::uint16_t npc_id) noexcept { this->m_npc_id = npc_id; }
    inline const xr_string& getFieldName(void) const noexcept { return this->m_field_name; }
    inline void setFieldName(const xr_string& field_name) noexcept
    {
        if (field_name.empty())
        {
            Msg("[Scripts/LogicData/setFieldName(field_name)] WARNING: field_name.empty() == true! You set an empty "
                "string");
        }

        this->m_field_name = field_name;
    }

    inline const xr_string& getFirstValueName(void) const noexcept { return this->m_value_name; }
    inline void setFirstValueName(const xr_string& value_name) noexcept
    {
        if (value_name.empty())
        {
            Msg("[Scripts/LogicData/setFirstValueName(value_name)] WARNING: value_name.empty() == true! You set an "
                "empty "
                "string");
        }

        this->m_value_name = value_name;
    }

    inline const xr_string& getSecondValue1Name(void) const noexcept { return this->m_value1_name; }
    inline void setSecondValue1Name(const xr_string& value_name) noexcept
    {
        if (value_name.empty())
        {
            Msg("[Scripts/LogicData/setSecondValueName(value_name)] WARNING: value_name.empty() == true! You set an "
                "empty string");
        }

        this->m_value1_name = value_name;
    }

    inline const xr_map<std::uint32_t, CondlistData>& getCondlist(void) const noexcept { return this->m_condlist; }
    inline void setCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
    {
        this->m_condlist = condlist;
    }

    inline bool IsEmpty(void) const noexcept
    {
        return (this->m_npc_id == 0) && (this->m_field_name.empty()) && (this->m_value1_name.empty()) &&
            (this->m_value_name.empty()) && (this->m_condlist.empty());
    }

private:
    std::uint16_t m_npc_id = 0;
    xr_string m_field_name; // @ .name
    xr_string m_value_name;
    xr_string m_value1_name;
    xr_map<std::uint32_t, CondlistData> m_condlist;
};
} // namespace Scripts
} // namespace Cordis
