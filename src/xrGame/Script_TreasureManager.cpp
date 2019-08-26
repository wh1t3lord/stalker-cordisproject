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
        xr_vector<std::pair<std::pair<std::uint32_t, float>, xr_vector<std::uint16_t>>>& data =
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
            Msg("[Scripts/Script_TreasureManager/fill(server_object, treasure_id_name)] Attempt to register unknown "
                "item [%s] in secret [%s]",
                server_object->name_replace(), treasure_id_name.c_str());
            R_ASSERT(false);
        }
    }
    else
    {
        Msg("[Scripts/Script_TreasureManager/fill(server_object, treasure_id_name)] Attempt to register item [%s] in "
            "unexistent secret [%s]",
            server_object->name_replace(), treasure_id_name.c_str());
        R_ASSERT(false);
    }

    return false;
}

bool Script_TreasureManager::register_item(CSE_ALifeDynamicObject* server_object)
{
    if (!server_object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (server_object->spawn_ini().section_exist("secret"))
    {
        const char* Field;
        const char* Value;

        if (server_object->spawn_ini().r_line("secret", 0, &Field, &Value))
        {
            if (strcmp(Field, "name"))
            {
                Msg("[Scripts/Scripts_TreasureManager/register_item(server_object)] There is no 'name' field in "
                    "[secret] section for object [%s]",
                    server_object->name_replace());
                R_ASSERT(false);
            }

            if (!strlen(Value))
            {
                Msg("[Scripts/Scripts_TreasureManager/register_item(server_object)] Field 'name' in [secret] section "
                    "got no value for object [%s]",
                    server_object->name_replace());
                R_ASSERT(false);
            }

            return this->fill(server_object, Value);
        }
    }

    return false;
}

void Script_TreasureManager::register_restrictor(CSE_ALifeDynamicObject* server_object)
{
    if (!server_object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    if (server_object->spawn_ini().section_exist("secret"))
        this->m_secret_restrictors[server_object->name_replace()] = server_object->ID;
}

void Script_TreasureManager::update(void)
{
    if (!this->m_is_items_spawned)
    {
        for (std::pair<const xr_string, DataSecret>& it : this->m_secrets)
        {
            this->spawn_treasure(it.first);
        }

        this->m_is_items_spawned = true;
    }

    std::uint32_t global_time = Globals::Game::time_global();

    if (this->m_check_time != Globals::kUnsignedInt32Undefined && global_time - this->m_check_time <= 500)
        return;

    this->m_check_time = global_time;

    for (std::pair<const xr_string, DataSecret>& it : this->m_secrets)
    {
        if (it.second.m_is_given)
        {
            if (it.second.m_condlist_empty.size())
            {
                xr_string section_name =
                    XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
                        (CScriptGameObject*)(nullptr), it.second.m_condlist_empty);

                if (section_name == "true" && !it.second.m_is_checked)
                {
                    Globals::Game::level::map_remove_object_spot(this->m_secret_restrictors[it.first], "treasure");
                    // Lord: добавить xr_statistic.inc_founded_secrets_counter();
                    it.second.m_condlist_empty.clear();
                    it.second.m_is_checked = true;

                    Msg("[Scripts/Script_TreasureManager/update()] Empty secret [%s] remove map spot!",
                        it.first.c_str());
                }
            }
        }
        else if (it.second.m_condlist_refreshing.size() && it.second.m_is_checked)
        {
            xr_string section_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
                (CScriptGameObject*)(nullptr), it.second.m_condlist_refreshing);

            if (section_name == "true")
            {
                it.second.m_is_given = false;
                it.second.m_is_checked = false;

                Msg("[Scripts/Script_TreasureManager/update()] [%s] now is avaliable!", it.first.c_str());
            }
        }
    }
}

void Script_TreasureManager::spawn_treasure(const xr_string& treasure_id_name)
{
    if (!treasure_id_name.c_str())
    {
        R_ASSERT2(false, "string can't be empty!");
        return;
    }

    if (!this->m_secrets[treasure_id_name].m_items.size())
    {
        R_ASSERT2(false, "There is no stored secret!!!");
        return;
    }

    if (this->m_secrets[treasure_id_name].m_is_given)
    {
        Msg("Secret [%s] already given!", treasure_id_name.c_str());
        return;
    }

    for (xr_map<xr_string, xr_vector<std::pair<std::pair<std::uint32_t, float>, xr_vector<std::uint16_t>>>>::value_type&
             it : this->m_secrets[treasure_id_name].m_items)
    {
        for (std::pair<std::pair<std::uint32_t, float>, xr_vector<std::uint16_t>>& it_vec : it.second)
        {
            for (std::uint32_t i = 0; i < it_vec.first.first; ++i)
            {
                float chance = Globals::Script_RandomFloat::getInstance().Generate();

                if (chance < it_vec.first.second)
                {
                    if (it_vec.second[i])
                    {
                        CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(it_vec.second[i]);

                        if (!server_object)
                        {
                            R_ASSERT2(false, "Something went wrong!");
                            return;
                        }

                        CSE_Abstract* created_server_object = Globals::Game::alife_create(
                            it.first, server_object->Position(), server_object->m_tNodeID, server_object->m_tGraphID);

                        if (!created_server_object)
                        {
                            R_ASSERT2(false, "Something went wrong!");
                            return;
                        }

                        created_server_object->o_Angle = server_object->o_Angle;
                        created_server_object->cast_alife_object()->use_ai_locations(
                            server_object->used_ai_locations());

                        this->m_items_from_secrects[created_server_object->ID] =
                            this->m_secret_restrictors[treasure_id_name];

                        this->m_secrets[treasure_id_name].m_to_find += 1;
                    }
                }
            }
        }
    }
}

void Script_TreasureManager::give_treasure(const xr_string& treasure_id_name, const bool& is_spawn)
{
    if (!treasure_id_name.size())
    {
        R_ASSERT2(false, "string can't empty!");
        return;
    }

    if (!this->m_secrets[treasure_id_name].m_items.size())
    {
        R_ASSERT2(false, "There is no secret!!!");
        return;
    }

    if (this->m_secrets[treasure_id_name].m_is_given)
    {
        Msg("[Scripts/Script_TreasureManager/give_treasure(treasure_id_name, is_spawn)] Secret [%s] already given!",
            treasure_id_name.c_str());
        return;
    }

    if (!this->m_secrets[treasure_id_name].m_to_find && !(this->m_secrets[treasure_id_name].m_condlist_empty.size()))
    {
        // Lord: добавить news_manager.send_treasure(2);
        Msg("[Scripts/Script_TreasureManager/give_treasure(treasure_id_name, is_spawn)] Secret [%s] already empty",
            treasure_id_name.c_str());
        return;
    }

    if (is_spawn)
    {
        Msg("[Scripts/Script_TreasureManager/give_treasure(treasure_id_name, is_spawn)] is_spawn = true!");
        this->spawn_treasure(treasure_id_name);
    }

    Globals::Game::level::map_add_object_spot_ser(this->m_secret_restrictors[treasure_id_name], "treasure", "");

    this->m_secrets[treasure_id_name].m_is_given = true;

    // Lord: добавить news_manager.send_treasure(0);
    Msg("[Scripts/Script_TreasureManager/give_treasure(treasure_id_name, is_spawn)] Give secret [%s]",
        treasure_id_name.c_str());
}

void Script_TreasureManager::on_item_take(const std::uint16_t& object_id)
{
    std::uint16_t restirctor_id = this->m_items_from_secrects[object_id];
    xr_string treasure_id_name = "";

    for (std::pair<const xr_string, std::uint16_t>& it : this->m_secret_restrictors)
    {
        if (restirctor_id == it.second)
        {
            treasure_id_name = it.first;
        }
    }

    if (treasure_id_name.size())
    {
        DataSecret& current_secret = this->m_secrets[treasure_id_name];
        current_secret.m_to_find -= 1;
        if (!current_secret.m_to_find)
        {
            Globals::Game::level::map_remove_object_spot(this->m_secret_restrictors[treasure_id_name], "treasure");
            // Lord: добавить xr_statistics.inc_founded_secrets_counter();
            Msg("[Scripts/Script_TreasureManager/on_item_take()] Secret [%s] now is empty!", treasure_id_name.c_str());
            current_secret.m_is_checked = true;
            // Lord: добавить news_manager.send_treasure(1);
        }

        this->m_items_from_secrects[object_id] = 0;
    }
}

void Script_TreasureManager::save(NET_Packet& packet)
{
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, false, "CTreasureManager");

    packet.w_u8(this->m_is_items_spawned ? 1 : 0);
    packet.w_u16(this->m_items_from_secrects.size());

    for (std::pair<const std::uint16_t, std::uint16_t>& it : this->m_items_from_secrects)
    {
        packet.w_u16(it.first);
        packet.w_u16(it.second);
    }

    packet.w_u16(this->m_secrets.size());

    for (std::pair<const xr_string, DataSecret>& it : this->m_secrets)
    {
        if (!this->m_secret_restrictors[it.first])
            packet.w_u16(Globals::kUnsignedInt16Undefined);
        else
            packet.w_u16(this->m_secret_restrictors[it.first]);

        packet.w_u8(it.second.m_is_given ? 1 : 0);
        packet.w_u8(it.second.m_is_checked ? 1 : 0);
        packet.w_u8(it.second.m_to_find);
    }

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Save, true, "CTreasureManager");
}

void Script_TreasureManager::load(NET_Packet& packet)
{
    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, false, "CTreasureManager");

    this->m_is_items_spawned = !!packet.r_u8();

    std::uint16_t total_size = packet.r_u16();

    for (std::uint16_t i = 0; i < total_size; ++i)
    {
        std::uint16_t key = packet.r_u16();
        std::uint16_t value = packet.r_u16();

        this->m_items_from_secrects[key] = value;
    }

    total_size = packet.r_u16();
    std::uint16_t id = packet.r_u16();
    xr_string founded_id_name;
    for (std::uint16_t i = 0; i < total_size; ++i)
    {
        founded_id_name = "";
        for (std::pair<const xr_string, std::uint16_t>& it : this->m_secret_restrictors)
        {
            if (it.second == id)
            {
                founded_id_name = it.first;
                break;
            }
        }

        bool is_given = !!packet.r_u8();
        bool is_checked = !!packet.r_u8();
        std::uint8_t to_find = packet.r_u8();

        // Lord: может быть не убедительная проверка со стороны this->m_secrets, пока без неё (смотреть оригинал)
        if (id != 65535 /*&& this->m_secrets[founded_id_name].m_items.size()*/)
        {
            this->m_secrets[founded_id_name].m_is_checked = is_checked;
            this->m_secrets[founded_id_name].m_is_given = is_given;
            this->m_secrets[founded_id_name].m_to_find = to_find;
        }
    }

    Globals::set_save_marker(packet, Globals::kSaveMarkerMode_Load, true, "CTreasureManager");
}

} // namespace Scripts
} // namespace Cordis
