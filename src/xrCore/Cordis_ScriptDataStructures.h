#pragma once

namespace Cordis
{
namespace Scripts
{
struct CondlistData
{
    struct CondlistValues
    {
        bool m_required;
        bool m_expected;
        xr_string m_section_name;
        xr_string m_function_name;
    };

    xr_map<std::uint32_t, xr_map<xr_string, bool>> m_infop_check;
    xr_map<std::uint32_t, xr_map<xr_string, bool>> m_infop_set;
};
}
} // namespace Cordis
