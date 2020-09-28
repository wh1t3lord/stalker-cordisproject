#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_TravelManager
{
private:
    Script_TravelManager(void) {}

public:
    inline static Script_TravelManager& getInstance(void) noexcept
    {
        static Script_TravelManager instance;
        return instance;
    }
    ~Script_TravelManager(void);
    Script_TravelManager(const Script_TravelManager&) = delete;
    Script_TravelManager& operator=(const Script_TravelManager&) = delete;
    Script_TravelManager(Script_TravelManager&&) = delete;
    Script_TravelManager& operator=(Script_TravelManager&&) = delete;

    // Lord: продолжитЬ!
};
}
} // namespace Cordis

