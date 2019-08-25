#include "stdafx.h"
#include "Script_TreasureManager.h"

namespace Cordis
{
namespace Scripts
{
Script_TreasureManager::~Script_TreasureManager(void) {}

bool Script_TreasureManager::fill(CSE_ALifeDynamicObject* server_object, const xr_string& treasure_id_name)
{
    if (!server_object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (!treasure_id_name.size())
    {
        R_ASSERT2(false, "string can't be null!");
        return;
    }

    if (this->m_secrets[treasure_id_name].m_items[server_object->name()].size())
    {
        xr_vector<std::pair<std::pair<std::uint32_t, std::uint32_t>, xr_vector<std::uint16_t>>>& data =
            this->m_secrets[treasure_id_name].m_items[server_object->name()];

        for (std::uint32_t i = 0; i < data.size(); ++i)
        {
            int count = data[i].second.size();

            if (count < data[i].first.first)
            {
                data[i].second.push_back(server_object->ID);
                return true;
            }

        }
    }

    return false;
}

} // namespace Scripts
} // namespace Cordis
