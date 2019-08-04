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
        xr_string m_section_name = "";
        xr_string m_function_name = "";
    };

    xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistValues>> m_infop_check;
    xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistValues>> m_infop_set;
};
} // namespace Scripts
} // namespace Cordis
