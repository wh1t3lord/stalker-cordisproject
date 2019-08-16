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
        xr_string m_id; // idle, lookout, fire, fire_no_lookout, reload, etc
        xr_map<xr_string, xr_vector<xr_string>> m_animations;
    };

    struct SmartCoverTransitionsData
    {
        float m_weight;
        xr_vector<xr_string> m_animations;
        xr_string m_action_from;
        xr_string m_action_to;
    };

    bool m_is_usable;
    float m_fieldofview;
    float m_fieldofview_danger;
    float m_range;
    Fvector m_fieldofview_position;
    Fvector m_fieldofview_direction;
    Fvector m_enter_direction;
    xr_vector<SmartCoverActionsData> m_actions;
    xr_vector<SmartCoverTransitionsData> m_transitions;
    xr_string m_id;
};

struct SmartCoverData
{
    bool m_is_need_weapon;
    xr_vector<SmartCoverLoopholeData> m_loopholes;

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

    xr_vector<SmartCoverTransitionsData> m_transitions;
};



} // namespace Scripts
} // namespace Cordis
