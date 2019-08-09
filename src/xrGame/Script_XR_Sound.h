#pragma once

namespace Cordis
{
namespace Scripts
{
namespace XR_SOUND
{
inline static xr_map<xr_string, Script_ISoundEntity*>& getSoundTable(void) noexcept
{
    static xr_map<xr_string, Script_ISoundEntity*> instance;
    return instance;
}

CScriptSound* set_sound_play(
    const std::uint16_t& npc_id, const xr_string& sound, xr_string& faction, const std::uint16_t& point)
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

    if (getSoundTable()[sound] == nullptr || sound_entity->IsPlayAlways())
    {
        if (getSoundTable()[sound])
        {
            getSoundTable()[sound]->reset(npc_id);
        }

        if (sound_entity->play(npc_id, faction, point))
        {
            getSoundTable()[sound] = sound_entity;
        }
    }
    else
    {
        return getSoundTable()[sound]->getSoundObject();
    }

    return getSoundTable()[sound]->getSoundObject();
}
} // namespace XR_SOUND
} // namespace Scripts
} // namespace Cordis
