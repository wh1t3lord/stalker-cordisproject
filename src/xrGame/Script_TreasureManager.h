#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_TreasureManager
{
private:
    Script_TreasureManager(void) 
    {

    }

public:
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


private:
    bool m_is_items_spawned;
    xr_map<xr_string, std::uint16_t> m_secret_restrictors;
};
} // namespace Scripts
} // namespace Cordis
