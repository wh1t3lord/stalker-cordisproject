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

inline static xr_map<std::uint16_t, xr_map<xr_string, Script_ISoundEntity*>>& getLoopedSoundDatabase(void) noexcept
{
    static xr_map<std::uint16_t, xr_map<xr_string, Script_ISoundEntity*>> instance;
    return instance;
}

inline CScriptSound* set_sound_play(
    const std::uint16_t& npc_id, const xr_string& sound, xr_string& faction, const std::uint16_t point)
{
    if (sound.empty())
    {
        R_ASSERT2(false, "string was empty!");
        return nullptr;
    }
    
    CScriptSound* p_result = getSoundDatabase().find(npc_id) == getSoundDatabase().end() ? nullptr : getSoundDatabase().at(npc_id)->getSoundObject();
    
    Script_ISoundEntity* sound_entity = Script_SoundThemeDataBase::getInstance().getTheme()[sound];

    if (!sound_entity)
    {
        MESSAGEWR("object was null! Wrong sound theme [%s], npc_id - %s", sound.c_str(), std::to_string(npc_id).c_str());
        R_ASSERT(false);
        return nullptr;
    }

    if (sound_entity->getSoundType() == SCRIPTSOUNDTYPE_LOOPED)
    {
        MESSAGEWR("You are trying to play sound [%s] which type is [%s]", sound.c_str(), SCRIPTSOUNDTYPE_LOOPED);
        R_ASSERT(false);
        return nullptr;
    }

    if ((getSoundDatabase().find(npc_id) == getSoundDatabase().end()) || sound_entity->IsPlayAlways())
    {
        if (getSoundDatabase().find(npc_id) != getSoundDatabase().end())
        {
            getSoundDatabase().at(npc_id)->reset(npc_id);
        }

        if (sound_entity->play(npc_id, faction, point))
        {
            getSoundDatabase()[npc_id] = sound_entity;
        }
    }
    else
    {
        return p_result;
    }

    return p_result;
}

inline void update(const std::uint16_t npc_id)
{
    if (getSoundDatabase().find(npc_id) != getSoundDatabase().end())
    {
        if (getSoundDatabase().at(npc_id))
        {
            if (!getSoundDatabase().at(npc_id)->is_playing(npc_id))
            {
                getSoundDatabase().at(npc_id)->callback(npc_id);
                getSoundDatabase()[npc_id] = nullptr;
            }       
        }
    }
}

inline void play_sound_looped(const std::uint16_t npc_id, const xr_string& sound_name)
{
    if (sound_name.empty())
    {
#ifdef DEBUG
        MESSAGEWR("sound_name.empty() == true! Empty "
            "string return ...");
#endif // DEBUG
        return;
    }

    Script_ISoundEntity* const p_sound = Script_SoundThemeDataBase::getInstance().getTheme()[sound_name];
    if (!p_sound)
    {
        R_ASSERT2(false, "it can't be!");
        return;
    }

    if (!(p_sound->getSoundType() == "looped_sound"))
    {
        R_ASSERT2(false, "WRONG TYPE CANT BE!");
        return;
    }

    if (getLoopedSoundDatabase().at(npc_id)[sound_name] &&
        getLoopedSoundDatabase().at(npc_id)[sound_name]->is_playing(npc_id))
    {
        return;
    }

    if (p_sound->play(npc_id))
    {
        MESSAGE("%s %s", sound_name.c_str(),
            std::to_string(npc_id).c_str());
        getLoopedSoundDatabase()[npc_id][sound_name] = p_sound;
    }
}

inline void stop_sound_looped(const std::uint16_t npc_id, const xr_string& sound_name)
{
    /*
        if (sound_name.empty())
        {
            Msg("[Scripts/XR_SOUND/stop_sound_looped(npc_id, sound_name)] WARNING: sound_name.empty() == true! Empty "
                "string return ...");
            return;
        }
    */

    if (getLoopedSoundDatabase().at(npc_id)[sound_name] &&
        getLoopedSoundDatabase().at(npc_id)[sound_name]->is_playing(npc_id))
    {
        getLoopedSoundDatabase().at(npc_id)[sound_name]->stop();
        getLoopedSoundDatabase()[npc_id][sound_name] = nullptr;
    }
    else
    {
        for (std::pair<const xr_string, Script_ISoundEntity*>& it : getLoopedSoundDatabase().at(npc_id))
        {
            if (it.second && it.second->is_playing(npc_id))
                it.second->stop(npc_id);

            it.second = nullptr; // Lord: проверить на всякий случай по поводу деалокации
        }

        getLoopedSoundDatabase().at(npc_id).clear();
    }
}

inline void stop_sounds_by_id(const std::uint16_t object_id)
{
    if (getSoundDatabase().find(object_id) != getSoundDatabase().end())
    {
		Script_ISoundEntity* const p_sound = getSoundDatabase().at(object_id);
		if (p_sound)
			p_sound->stop(object_id);
    }

    if (getLoopedSoundDatabase().find(object_id) != getLoopedSoundDatabase().end())
    {
		const xr_map<xr_string, Script_ISoundEntity*>& looped_sounds = getLoopedSoundDatabase().at(object_id);

		for (const std::pair<xr_string, Script_ISoundEntity*>& it : looped_sounds)
		{
			if (it.second && it.second->is_playing(object_id))
				it.second->stop(object_id);
		}
    }
}

inline void set_volume_sound_looped(const std::uint16_t npc_id, const xr_string& sound_name, const float value)
{
    if (!getLoopedSoundDatabase()[npc_id].empty())
    {
        if (getLoopedSoundDatabase().at(npc_id)[sound_name] &&
            getLoopedSoundDatabase().at(npc_id)[sound_name]->is_playing(npc_id))
        {
            Msg("[Scripts/XR_SOUND/set_volume_sound_looped(npc_id, sound_name, value)] %s %d %f", sound_name, npc_id,
                value);
            getLoopedSoundDatabase().at(npc_id)[sound_name]->set_volume(value);
        }
    }
}

} // namespace XR_SOUND
} // namespace Scripts
} // namespace Cordis
