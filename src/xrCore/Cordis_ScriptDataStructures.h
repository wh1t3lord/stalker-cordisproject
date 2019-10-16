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

        inline const xr_map<xr_string, xr_vector<xr_string>>& getAnimations(void) noexcept
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

    void register_action(const SmartCoverActionsData& data) noexcept { this->m_actions.push_back(data); }
    void register_transition(const SmartCoverTransitionsData& data) noexcept { this->m_transitions.push_back(data); }
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

} // namespace Scripts
} // namespace Cordis
