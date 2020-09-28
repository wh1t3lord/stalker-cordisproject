#include "stdafx.h"
#include "Script_SchemeSRParticle.h"
#include "RenderVisual.h"
#include "ParticleCustom.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRParticle::Script_SchemeSRParticle(
    CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_SRParticle* storage)
    : inherited_scheme(p_client_object, storage), m_last_update(0), m_path(nullptr), m_is_started(false),
      m_is_first_played(false), m_p_storage(storage)
{
    this->m_scheme_name = "sr_particle";
}

Script_SchemeSRParticle::~Script_SchemeSRParticle(void)
{
    if (this->m_path)
    {
        MESSAGEI("deleting m_path %s!", this->m_path->m_path_name);
        xr_delete(this->m_path);
    }

    for (ParticleData& it : this->m_particles)
    {
        if (it.getParticle())
        {
            MESSAGEI("deleting m_particle");
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
                MESSAGEI("deleting particles! %s", p_pointer->m_particles->GetRenderData().visual->dcast_ParticleCustom()->Name().c_str());
                xr_delete(p_pointer);
                it.setParticle(nullptr);
            }
        }

        this->m_particles.clear();
    }

    if (this->m_p_storage->getMode() == 2)
    {
        if (this->m_path)
        {
			MESSAGEI("deallocating "
				"this->m_path! %s", this->m_path->m_path_name);
            xr_delete(this->m_path);

        }

        this->m_path = new CPatrolPathParams(this->m_p_storage->getPathName().c_str());
        CondlistWaypoints flags = Globals::Utils::path_parse_waypoints(this->m_p_storage->getPathName());
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
            data.setParticle(new CScriptParticles(this->m_p_storage->getName().c_str()));
            this->m_particles.emplace_back(std::move(data));
        }
    }
    else
    {
        ParticleData data;
        data.setPlayed(false);
        data.setTime(Globals::get_time_global());
        data.setDelay(0);
        data.setParticle(new CScriptParticles(this->m_p_storage->getName().c_str()));

        this->m_particles.emplace_back(std::move(data));

        if (this->m_path)
        {
            MESSAGEI("deleting m_path in sr_particle!");
            xr_delete(this->m_path);
        }
    }

    this->m_p_storage->ClearSignals();
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
        if (this->m_p_storage->getMode() == 1)
        {
            Msg("[Scripts/Script_SchemeSRParticle/update(delta)] Load path %s",
                this->m_p_storage->getPathName().c_str());
            this->m_particles[0].getParticle()->LoadPath(this->m_p_storage->getPathName().c_str());
            this->m_particles[0].getParticle()->StartPath(this->m_p_storage->isLooped());
            this->m_particles[0].getParticle()->Play();
            this->m_particles[0].setPlayed(true);
            this->m_is_first_played = true;
        }

        return;
    }

    if (this->m_p_storage->getMode() == 1)
    {
        this->update_mode_1();
    }
    else
    {
        this->update_mode_2();
    }

    this->IsEnd();

    XR_LOGIC::try_switch_to_another_section(
        this->m_npc, this->m_p_storage, DataBase::Storage::getInstance().getActor());
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

void Script_SchemeSRParticle::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Script_ComponentScheme_SRParticle* p_storage =
        XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_SRParticle>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
    p_storage->setPathName(Globals::Utils::cfg_get_string(p_ini, section_name, "path"));
    p_storage->setName(Globals::Utils::cfg_get_string(p_ini, section_name, "name"));
    std::uint32_t mode = static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, "mode"));

    if (!mode || mode == Globals::kUnsignedInt32Undefined)
        mode = 1;

    p_storage->setMode(mode);
    p_storage->setLooped(Globals::Utils::cfg_get_bool(p_ini, section_name, "looped"));

    if (p_storage->getPathName().empty())
    {
        R_ASSERT2(false, "can't be an empty string");
        return;
    }

    if (p_storage->getMode() != 1 && p_storage->getMode() != 2)
    {
        R_ASSERT2(false, "wrong mode you are set");
        return;
    }
}

bool Script_SchemeSRParticle::IsEnd(void)
{
    if (this->m_p_storage->isLooped() || !this->m_is_first_played)
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

    this->m_p_storage->setSignals("particle_end", true);
    return true;
}

void Script_SchemeSRParticle::update_mode_1(void)
{
    if (!this->m_particles[0].getParticle()->IsPlaying() && !this->m_p_storage->isLooped())
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
            if (this->m_p_storage->isLooped())
            {
                it.getParticle()->PlayAtPos(this->m_path->point(i));
            }
        }

        ++i;
    }
}

} // namespace Scripts
} // namespace Cordis
