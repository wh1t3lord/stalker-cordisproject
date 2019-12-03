#pragma once

namespace Cordis
{
namespace Scripts
{
namespace XR_SOUND
{
inline static xr_map<std::uint16_t, Script_ISoundEntity*>& getSoundDatabase(void) noexcept
{
    static xr_map<std::uint16_t, Script_ISoundEntity*> instance;
    return instance;
}

inline static xr_map<std::uint16_t, Script_ISoundEntity*>& getLoopedSoundDatabase(void) noexcept 
{
    static xr_map<std::uint16_t, Script_ISoundEntity*> instance;
    return instance;
}

inline CScriptSound* set_sound_play(
    const std::uint16_t& npc_id, const xr_string& sound, xr_string& faction, const std::uint16_t point)
{
    if (!sound.size())
    {
        R_ASSERT2(false, "string was empty!");
        return nullptr;
    }

    Script_ISoundEntity* sound_entity = Script_SoundThemeDataBase::getInstance().getTheme()[sound];

    if (!sound_entity)
    {
        Msg("object was null! Wrong sound theme [%s], npc_id - %s", sound.c_str(), std::to_string(npc_id).c_str());
        R_ASSERT(false);
        return nullptr;
    }

    if (sound_entity->getSoundType() == SCRIPTSOUNDTYPE_LOOPED)
    {
        Msg("You are trying to play sound [%s] which type is [%s]", sound.c_str(), SCRIPTSOUNDTYPE_LOOPED);
        R_ASSERT(false);
        return nullptr;
    }

    if (getSoundDatabase()[npc_id] == nullptr || sound_entity->IsPlayAlways())
    {
        if (getSoundDatabase()[npc_id])
        {
            getSoundDatabase()[npc_id]->reset(npc_id);
        }

        if (sound_entity->play(npc_id, faction, point))
        {
            getSoundDatabase()[npc_id] = sound_entity;
        }
    }
    else
    {
        return getSoundDatabase()[npc_id]->getSoundObject();
    }

    return getSoundDatabase()[npc_id]->getSoundObject();
}

inline void update(const std::uint16_t& npc_id)
{
    if (getSoundDatabase()[npc_id])
    {
        if (!getSoundDatabase()[npc_id]->is_playing(npc_id))
        {
            getSoundDatabase()[npc_id]->callback(npc_id);
            getSoundDatabase()[npc_id] = nullptr;
        }
    }
}

} // namespace XR_SOUND
} // namespace Scripts
} // namespace Cordis
