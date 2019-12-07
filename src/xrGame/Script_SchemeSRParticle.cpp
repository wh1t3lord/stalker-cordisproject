#include "stdafx.h"
#include "Script_SchemeSRParticle.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRParticle::Script_SchemeSRParticle(
    CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage), m_last_update(0), m_path(nullptr), m_is_started(false),
      m_is_first_played(false)
{
    this->m_scheme_name = "sr_particle";
}

Script_SchemeSRParticle::~Script_SchemeSRParticle(void)
{
    if (this->m_path)
    {
        Msg("[Scripts/Script_SchemeSRParticle/~dtor()] deleting m_path %s!", this->m_path->m_path_name);
        xr_delete(this->m_path);
    }

    for (ParticleData& it : this->m_particles)
    {
        if (it.getParticle())
        {
            Msg("[Scripts/Script_SchemeSRParticle/~dtor()] deleting m_particle");
            CScriptParticles* p_pointer = it.getParticle();
            xr_delete(p_pointer);
            it.setParticle(nullptr);
        }
    }

    this->m_particles.clear();
}

void Script_SchemeSRParticle::reset_scheme(const bool value, CScriptGameObject* const p_client_object)
{
    if (!this->m_particles.empty())
    {
        for (ParticleData& it : this->m_particles)
        {
            if (it.getParticle())
            {
                CScriptParticles* p_pointer = it.getParticle();
                xr_delete(p_pointer);
                it.setParticle(nullptr);
            }
        }

        this->m_particles.clear();
        Msg("[Scripts/Script_SchemeSRParticle/reset_scheme(is_loading, p_client_object)] deallocating "
            "this->m_particles!");
    }

    if (this->m_storage->getSRParticleMode() == 2)
    {
        if (this->m_path)
        {
            xr_delete(this->m_path);
            Msg("[Scripts/Script_SchemeSRParticle/reset_scheme(is_loading, p_client_object)] deallocating "
                "this->m_path!");
        }

        this->m_path = new CPatrolPathParams(this->m_storage->getSRParticlePathName().c_str());
        CondlistWaypoints flags = Globals::Utils::path_parse_waypoints(this->m_storage->getSRParticlePathName());
        const std::uint32_t count = this->m_path->count();

        // Lord: проверить итерацию!
        for (std::uint32_t i = 0; i < count; ++i)
        {
            std::uint32_t delay = 0;
            if (!flags.getData().at(i).getValue("d").empty())
            {
                delay = atoi(flags.getData().at(i).getValue("d").c_str());
            }

            xr_string sound_name;

            if (!flags.getData().at(i).getValue("s").empty())
            {
                sound_name = flags.getData().at(i).getValue("s");
            }

            ParticleData data;
            data.setDelay(delay);
            data.setPlayed(false);
            data.setTime(Globals::get_time_global());
            data.setParticle(new CScriptParticles(this->m_storage->getSRParticleName().c_str()));
            this->m_particles.push_back(data);
        }
    }
    else
    {
        ParticleData data;
        data.setPlayed(false);
        data.setTime(Globals::get_time_global());
        data.setDelay(0);
        data.setParticle(new CScriptParticles(this->m_storage->getSRParticleName().c_str()));

        this->m_particles.push_back(data);

        if (this->m_path)
        {
            xr_delete(this->m_path);
        }
    }

    this->m_storage->ClearSignals();
    this->m_last_update = 0;
    this->m_is_first_played = false;
    this->m_is_started = false;
}

void Script_SchemeSRParticle::update(const float delta)
{
    std::uint32_t time = Globals::get_time_global();
    if (this->m_last_update)
    {
        if (time - this->m_last_update < 50)
        {
            return;
        }
        else
        {
            this->m_last_update = time;
        }
    }
    else
    {
        this->m_last_update = time;
    }

    if (!this->m_is_started)
    {
        this->m_is_started = true;
        if (this->m_storage->getSRParticleMode() == 1)
        {
            Msg("[Scripts/Script_SchemeSRParticle/update(delta)] Load path %s",
                this->m_storage->getSRParticlePathName().c_str());
            this->m_particles[0].getParticle()->LoadPath(this->m_storage->getSRParticlePathName().c_str());
            this->m_particles[0].getParticle()->StartPath(this->m_storage->IsSRParticleLooped());
            this->m_particles[0].getParticle()->Play();
            this->m_particles[0].setPlayed(true);
            this->m_is_first_played = true;
        }

        return;
    }

    if (this->m_storage->getSRParticleMode() == 1)
    {
        this->update_mode_1();
    }
    else
    {
        this->update_mode_2();
    }

    this->IsEnd();

    XR_LOGIC::try_switch_to_another_section(this->m_npc, *this->m_storage, DataBase::Storage::getInstance().getActor());
}

void Script_SchemeSRParticle::deactivate(CScriptGameObject* const p_client_object)
{
    Msg("[Scripts/Script_SchemeSRParticle/deactivate(p_client_object)] deleting data!");
    for (ParticleData& it : this->m_particles)
    {
        if (!it.getParticle())
            continue;

        if (it.getParticle()->IsPlaying())
        {
            it.getParticle()->Stop();
        }

        CScriptParticles* p_object = it.getParticle();
        xr_delete(p_object);
        it.setParticle(nullptr);
    }
}

bool Script_SchemeSRParticle::IsEnd(void)
{
    if (this->m_storage->IsSRParticleLooped() || !this->m_is_first_played)
    {
        return false;
    }

    if (this->m_particles.empty())
        return true;

    for (ParticleData& it : this->m_particles)
    {
        if (!it.getParticle())
            continue;

        if (it.getParticle()->IsPlaying())
            return false;
    }

    this->m_storage->setSignals("particle_end", true);
    return true;
}

void Script_SchemeSRParticle::update_mode_1(void)
{
    if (!this->m_particles[0].getParticle()->IsPlaying() && !this->m_storage->IsSRParticleLooped())
    {
        this->m_particles[0].getParticle()->Play();
    }
}

void Script_SchemeSRParticle::update_mode_2(void)
{
    if (this->m_particles.empty())
        return;

    std::uint32_t time = Globals::get_time_global();
    std::uint32_t i = 0;
    for (ParticleData& it : this->m_particles)
    {
        if (((time - it.getTime()) > it.getDelay()) && !it.getParticle()->IsPlaying())
        {
            if (!it.IsPlayed())
            {
                it.getParticle()->PlayAtPos(this->m_path->point(i));
                it.setPlayed(true);
                this->m_is_first_played = true;
            }
        }
        else
        {
            if (this->m_storage->IsSRParticleLooped())
            {
                it.getParticle()->PlayAtPos(this->m_path->point(i));
            }
        }

        ++i;
    }
}

} // namespace Scripts
} // namespace Cordis
