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

        if (data.size())
        {
            for (std::uint32_t i = 0; i < data.size(); ++i)
            {
                if (data[i].second.size() < data[i].first.first)
                {
                    data[i].second.push_back(server_object->ID);
                    return true;
                }
            }
        }
        else
        {
            Msg("[Scripts/Script_TreasureManager/fill(server_object, treasure_id_name)] Attempt to register unknown item [%s] in secret [%s]", server_object->name_replace(), treasure_id_name.c_str());
            R_ASSERT(false);
        }
    }
    else
    {
        Msg("[Scripts/Script_TreasureManager/fill(server_object, treasure_id_name)] Attempt to register item [%s] in unexistent secret [%s]",
            server_object->name_replace(), treasure_id_name.c_str());
        R_ASSERT(false);
    }

    return false;
}

} // namespace Scripts
} // namespace Cordis
