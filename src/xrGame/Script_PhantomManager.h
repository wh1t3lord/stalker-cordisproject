#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_PhantomManager
{
private:
    Script_PhantomManager(void) : m_phantom_count(0) {}

public:
    static inline Script_PhantomManager& getInstance(void) noexcept
    {
        static Script_PhantomManager instance;
        return instance;
    }

    Script_PhantomManager(const Script_PhantomManager&) = delete;
    Script_PhantomManager& operator=(const Script_PhantomManager&) = delete;
    Script_PhantomManager(Script_PhantomManager&&) = delete;
    Script_PhantomManager& operator=(Script_PhantomManager&&) = delete;
    ~Script_PhantomManager(void) {}


    inline void add_phantom(void) noexcept { ++this->m_phantom_count; }
    inline void remove_phantom(void) noexcept { --this->m_phantom_count; }
    inline std::uint32_t getPhantomCount(void) const noexcept { return this->m_phantom_count; }
    inline void spawn_phantom(const Fvector& position) noexcept { Globals::Game::level::spawn_phantom(position); }

private:
    std::uint32_t m_phantom_count;
};
} // namespace Scripts
} // namespace Cordis
