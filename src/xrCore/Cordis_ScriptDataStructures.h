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

} // namespace Scripts
} // namespace Cordis
