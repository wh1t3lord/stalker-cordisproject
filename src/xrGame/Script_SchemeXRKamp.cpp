#include "stdafx.h"
#include "Script_SchemeXRKamp.h"

namespace Cordis
{
    namespace Scripts
{
    void Script_ActionXRKampGoPosition::initialize(void) 
    {
        CScriptActionBase::initialize();
        
        this->m_p_object->set_desired_position();
        this->m_p_object->set_desired_direction();

        this->m_p_storage->setXRKampPosVertex(0);
        this->m_p_storage->setXRKampNpcPositionNum(0);
        this->m_p_storage->ClearSignals();
    }

    void Script_ActionXRKampGoPosition::execute(void) 
    {
        CScriptActionBase::execute();


    }

    void Script_ActionXRKampGoPosition::finalize(void) 
    {
    
    }

    Script_XRKamp::Script_XRKamp(const xr_string& path_name)
        : m_kamp_name(path_name), m_kamp_state_name("idle"), m_p_patrol(new CPatrolPathParams(path_name.c_str())),
          m_population(0), m_director_id(0), m_begin(0)
    {
        this->m_position.emplace_back(Fvector().set(1.0f, 0.0f, 0.0f), 0);
        this->m_position.emplace_back(Fvector().set(1.0f, 0.0f, 1.0f), 0);
        this->m_position.emplace_back(Fvector().set(0.0f, 0.0f, 1.0f), 0);
        this->m_position.emplace_back(Fvector().set(-1.0f, 0.0f, 1.0f), 0);
        this->m_position.emplace_back(Fvector().set(-1.0f, 0.0f, 0.0f), 0);
        this->m_position.emplace_back(Fvector().set(-1.0f, 0.0f, -1.0f), 0);
        this->m_position.emplace_back(Fvector().set(0.0f, 0.0f, -1.0f), 0);
        this->m_position.emplace_back(Fvector().set(1.0f, 0.0f, -1.0f), 0);

        this->m_avail_states["idle"].first = {
            "wait", "sit", "sit_ass", "sit_knee", "eat_kolbasa", "eat_vodka", "eat_energy", "eat_bread", "trans"};
        this->m_avail_states["pre_harmonica"].first = {"wait_harmonica"};
        this->m_avail_states["pre_harmonica"].second = {
            "wait", "sit", "sit_ass", "sit_knee", "eat_kolbasa", "eat_vodka", "eat_energy", "eat_bread", "trans"};
        this->m_avail_states["harmonica"].first = {"play_harmonica"};
        this->m_avail_states["harmonica"].second = {
            "wait", "sit", "sit_ass", "sit_knee", "eat_kolbasa", "eat_vodka", "eat_energy", "eat_bread", "trans"};
        this->m_avail_states["post_harmonica"].first = {"wait_harmonica"};
        this->m_avail_states["post_harmonica"].second = {
            "wait", "sit", "sit_ass", "sit_knee", "eat_kolbasa", "eat_vodka", "eat_energy", "eat_bread", "trans"};
        this->m_avail_states["pre_guitar"].first = {"wait_guitar"};
        this->m_avail_states["pre_guitar"].second = {
            "wait", "sit", "sit_ass", "sit_knee", "eat_kolbasa", "eat_vodka", "eat_energy", "eat_bread", "trans"};
        this->m_avail_states["guitar"].first = {"play_guitar"};
        this->m_avail_states["guitar"].second = {
            "wait", "sit", "sit_ass", "sit_knee", "eat_kolbasa", "eat_vodka", "eat_energy", "eat_bread", "trans"};
        this->m_avail_states["post_guitar"].first = {"wait_guitar"};
        this->m_avail_states["post_guitar"].second = {
            "wait", "sit", "sit_ass", "sit_knee", "eat_kolbasa", "eat_vodka", "eat_energy", "eat_bread", "trans"};
        this->m_avail_states["story"].first = {"declarate"};
        this->m_avail_states["story"].second = {
            "wait", "sit", "sit_ass", "sit_knee", "eat_kolbasa", "eat_vodka", "eat_energy", "eat_bread", "trans"};

        this->m_avail_sounds["idle"].first = "idle";
        this->m_avail_sounds["pre_harmonica"].first = "pre_harmonica";
        this->m_avail_sounds["harmonica"];
        this->m_avail_sounds["post_harmonica"].second = "reac_harmonica";
        this->m_avail_sounds["pre_guitar"].first = "pre_guitar";
        this->m_avail_sounds["guitar"];
        this->m_avail_sounds["post_guitar"].second = "reac_guitar";
        this->m_avail_sounds["story"];

        this->m_timeout["idle"].first = 3000;
        this->m_timeout["idle"].second = false;
        this->m_timeout["pre_harmonica"].first = 3000;
        this->m_timeout["pre_harmonica"].second = false;
        this->m_timeout["harmonica"].first = 5000;
        this->m_timeout["harmonica"].second = true;
        this->m_timeout["post_harmonica"].first = 3000;
        this->m_timeout["post_harmonica"].second = false;
        this->m_timeout["pre_guitar"].first = 3000;
        this->m_timeout["pre_guitar"].second = false;
        this->m_timeout["guitar"].first = 5000;
        this->m_timeout["guitar"].second = true;
        this->m_timeout["post_guitar"].first = 3000;
        this->m_timeout["post_guitar"].second = false;
        this->m_timeout["story"].first = 1000;
        this->m_timeout["story"].second = true;

        this->m_kamp_states["idle"] = true;
        this->m_kamp_states["pre_harmonica"] = false;
        this->m_kamp_states["harmonica"] = false;
        this->m_kamp_states["post_harmonica"] = false;
        this->m_kamp_states["pre_guitar"] = false;
        this->m_kamp_states["guitar"] = false;
        this->m_kamp_states["post_guitar"] = false;
        this->m_kamp_states["story"] = true;

        this->m_trans_kamp["idle"]["idle"] = 0;
        this->m_trans_kamp["idle"]["pre_harmonica"] = 0;
        this->m_trans_kamp["idle"]["pre_guitar"] = 50;
        this->m_trans_kamp["idle"]["story"] = 50;

        this->m_trans_kamp["pre_harmonica"]["harmonica"] = 100;
        this->m_trans_kamp["harmonica"]["post_harmonica"] = 100;
        this->m_trans_kamp["post_harmonica"]["idle"] = 70;
        this->m_trans_kamp["post_harmonica"]["harmonica"] = 30;
        this->m_trans_kamp["pre_guitar"]["guitar"] = 100;
        this->m_trans_kamp["guitar"]["post_guitar"] = 100;
        this->m_trans_kamp["post_guitar"]["idle"] = 70;
        this->m_trans_kamp["post_guitar"]["guitar"] = 30;
        this->m_trans_kamp["story"]["idle"] = 100;

        this->m_p_sound_manager = &Script_SoundManager::getSoundManager(xr_string("kamp_").append(path_name));
    }

    Script_XRKamp::~Script_XRKamp(void) 
    {
        if (this->m_p_patrol)
        {
            Msg("[Scripts/Script_XRKamp/~dtor()] deleting patrol path %s", this->m_p_patrol->m_path_name);
            xr_delete(this->m_p_patrol);
        }
    }

    void Script_XRKamp::select_position(const std::uint16_t npc_id) 
    {
        xr_vector<std::uint32_t> indecies;

        for (std::uint32_t i = 0; i < this->m_position.size(); ++i)
        {
            if (this->m_position[i].second == 0)
            {
                indecies.push_back(i);
            }
        }

        if (indecies.empty() == false)
        {
            std::uint32_t selected_index = Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(0, indecies.size() - 1);
            this->m_position[indecies[selected_index]].second = npc_id;
            this->m_npc[npc_id].setPosition(indecies[selected_index]);
        }
    }

    void Script_XRKamp::getDestVertex(
        CScriptGameObject* const p_npc, const float radius, std::uint32_t& dest_vertex, std::uint32_t& position)
    {
        if (!p_npc)
        {
            Msg("[Scripts/Script_XRKamp/getDestVertex(p_npc, radius, dest_vertex, position)] WARNING: p_npc == nullptr! Return ...");
            return;
        }

        std::uint32_t npc_id = p_npc->ID();

        if (this->m_npc.find(npc_id) == this->m_npc.end())
        {
            Msg("[Scripts/Script_XRKamp/getDestVertex(p_npc, radius, dest_vertex, position)] WARNING: can't find npc_id in this->m_npc by %d Return ...", npc_id);
            return;
        }

        if (this->m_npc.at(npc_id).getPosition() == 0)
        {
            Msg("[Scripts/Script_XRKamp/getDestVertex(p_npc, radius, dest_vertex, position)] WARNING: positon == 0! Return ...");
            return;
        }

        Fvector pp = this->m_p_patrol->point(static_cast<std::uint32_t>(0));
        Fvector dir = this->m_position.at(this->m_npc[npc_id].getPosition()).first;

        dir.x = dir.x + (Globals::Script_RandomFloat::getInstance().Generate(-1.0f, 1.0f) / 5.0f);
        dir.z = dir.z + (Globals::Script_RandomFloat::getInstance().Generate(-1.0f, 1.0f) / 5.0f);
        dir.normalize();

        float _radius = radius;

        _radius += Globals::Script_RandomFloat::getInstance().Generate(-0.3f, 0.3f);

        dest_vertex = 4294967295;

        while (dest_vertex == 4294967295)
        {
            Fvector temp_pos;
            temp_pos.x = pp.x + dir.x * _radius;
            temp_pos.y = pp.y;
            temp_pos.z = pp.z + dir.z * _radius;

            dest_vertex = Globals::Game::level::vertex_id(temp_pos);

            if (dest_vertex == 4294967295)
            {
                if (_radius < 1.0f)
                {
                    Msg("[Scripts/Scritp_XRKamp/getDestVertex(p_npc, radius, dest_vertex, position)] WARNING: Invalid AI map at kamp point %d Return ...", dest_vertex);
                    return;
                }
            }
            else
            {
                _radius -= 0.5f;
            }
        }
        position = this->m_npc.at(npc_id).getPosition();
        if (!p_npc->accessible_vertex_id(dest_vertex))
        {
            Fvector vertex_position = Globals::Game::level::vertex_position(dest_vertex);
            std::uint32_t nearest_vertex = p_npc->accessible_nearest(vertex_position, Fvector());
            dest_vertex = nearest_vertex;
            return;
        }
    }

    void Script_XRKamp::proceedState(CScriptGameObject* const p_npc) 
    {
        if (!p_npc)
        {
            Msg("[Scripts/Script_XRKamp/proceedState(p_npc)] WARNING: p_npc == nullptr! Return ...");
            return;
        }

        std::uint16_t npc_id = p_npc->ID();
        std::uint32_t active_sound_count = p_npc->active_sound_count();

        if (this->m_npc.find(npc_id) == this->m_npc.end())
        {
            Msg("[Scripts/Script_XRKamp/proceedState(p_npc)] WARNING: can't find data by id %d Return ...", npc_id);
            return;
        }

        if (this->m_npc.at(npc_id).isNeedSoundBegin())
        {
            if (!active_sound_count)
                return;
            else
                this->m_npc.at(npc_id).setNeedSoundBegin(false);
        }

        if (this->m_begin && (Globals::get_time_global() - this->m_begin < this->m_timeout.at(this->m_kamp_state_name).first))
            return;
        
        if (active_sound_count)
            return;

        if (!this->m_p_sound_manager->isFinished())
        {
            this->m_p_sound_manager->update();
            return;
        }

        std::uint32_t max_rand = 0;
        xr_map<xr_string, std::uint32_t> temp; 
        for (const std::pair<xr_string, std::uint32_t>& it : this->m_trans_kamp.at(this->m_kamp_state_name))
        {
            if (this->m_kamp_states[it.first])
            {
                temp[it.first] = it.second;
                max_rand += it.second;
            }
        }

        if (max_rand == 0)
        {
            temp["idle"] = 100;
            max_rand = 100;
        }

        std::uint32_t p = Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(0, max_rand);

        for (const std::pair<xr_string, std::uint32_t>& it : temp)
        {
            p -= it.second;

            if (p <= 0)
            {
                if (it.first == "idle")
                {
                    this->m_director_id = 0;
                    if (this->m_kamp_state_name != "idle")
                        this->m_npc[npc_id].setBegin(0);
                }
                else if (it.first == "story")
                {
                    this->m_p_sound_manager->set_story("test_story");
                    this->m_director_id = npc_id;
                }
                else
                {
                    this->m_npc.at(npc_id).setBegin(0);
                    if (this->m_timeout.at(it.first).second)
                        this->m_npc.at(npc_id).setNeedSoundBegin(true);

                    this->m_director_id = npc_id;
                }

                this->m_kamp_state_name = it.first;
                this->m_begin = Globals::get_time_global();

                for (std::pair<const std::uint16_t, NpcData>& it2 : this->m_npc)
                {
                    it2.second.setNew(true);
                }

                return;
            }
        }
    }

    void Script_XRKamp::proceedRole(CScriptGameObject* const p_npc, const bool is_director, xr_string& state_name, xr_string& sound_name) 
    {
        if (!p_npc)
        {
            Msg("[Scripts/Script_XRKamp/proceedRole(p_npc, state_name, sound_name)] WARNING: p_npc == nullptr! Return ...");
            return;
        }

        std::uint32_t states = 0;
        xr_vector<xr_string> state;

        std::uint16_t npc_id = p_npc->ID();

        if (!this->m_npc.at(npc_id).getBegin() || (Globals::get_time_global() - this->m_npc.at(npc_id).getBegin() >= this->m_npc.at(npc_id).getStateIdle()))
        {
            if (is_director)
            {
                state = this->m_avail_states.at(this->m_kamp_state_name).first;
            }
            else
            {
                state = this->m_avail_states.at(this->m_kamp_state_name).second;
            }

            xr_vector<xr_string> temp;

            for (const xr_string& it : state)
            {
                if (this->m_npc.at(npc_id).getStates().find(it) != this->m_npc.at(npc_id).getStates().end())
                {
                    if (this->m_npc.at(npc_id).getStates().at(it))
                        temp.push_back(it);
                }
            }

            this->m_npc[npc_id].setBegin(Globals::get_time_global());
            state_name = temp[Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(0, temp.size() - 1)];
            this->m_npc[npc_id].setSelectedStateName(state_name);
            this->m_npc[npc_id].setStateIdle(
                Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(15000, 20000));
        }
        else
        {
            state_name = this->m_npc.at(npc_id).getSelectedStateName();
        }

        if (this->m_kamp_state_name == "story")
        {
            sound_name = "";
        }
        else
        {
            if (is_director)
                sound_name = this->m_avail_sounds.at(this->m_kamp_state_name).first;
            else
                sound_name = this->m_avail_sounds.at(this->m_kamp_state_name).second;
        }
    }

    void Script_XRKamp::updateNpc(
        CScriptGameObject* const p_npc, xr_string& state_name, xr_string& sound_name, std::uint32_t& substate)
    {
/* @ NOT USED!
        this->checkNpcAbility(p_npc);
        std::uint16_t npc_id = p_npc->ID();
        bool is_director = (this->m_director_id == 0) ? true : (this->m_director_id == npc_id);

        if (is_director)
            this->proceedState(p_npc);

        xr_string state_name, sound_name;
        this->proceedRole(p_npc, is_director, state_name, sound_name);*/
    }

    void Script_XRKamp::checkNpcAbility(CScriptGameObject* const p_npc) 
    {
        if (!p_npc)
        {
            Msg("[Scripts/Script_XRKamp/checkNpcAbility(p_npc)] WARNING: p_npc == nullptr! Return ...");
            return;
        }

        std::uint16_t npc_id = p_npc->ID();

        if (Globals::character_community(p_npc) != "monolith" && Globals::character_community(p_npc) != "zombied")
        {
            if (p_npc->GetObjectByName("kolbasa"))
                this->m_npc[npc_id].setState("eat_kolbasa", true);
            else
                this->m_npc[npc_id].setState("eat_kolbasa", false);
            
            if (p_npc->GetObjectByName("vodka"))
                this->m_npc[npc_id].setState("eat_vodka", true);
            else
                this->m_npc[npc_id].setState("eat_vodka", false);

            if (p_npc->GetObjectByName("energy_drink"))
                this->m_npc[npc_id].setState("eat_energy", true);
            else
                this->m_npc[npc_id].setState("eat_energy", false);

            if (p_npc->GetObjectByName("bread"))
                this->m_npc[npc_id].setState("eat_bread", true);
            else
                this->m_npc[npc_id].setState("eat_bread", false);

            this->m_npc[npc_id].setState("play_harmonica", false);
            this->m_npc[npc_id].setState("wait_harmonica", false);
            this->m_kamp_states["pre_harmonica"] = false;
            this->m_kamp_states["harmonica"] = false;
            this->m_kamp_states["post_harmonica"] = false;

            this->m_npc[npc_id].setState("play_guitar", false);
            this->m_npc[npc_id].setState("wait_guitar", false);
            this->m_kamp_states["pre_guitar"] = false;
            this->m_kamp_states["guitar"] = false;
            this->m_kamp_states["post_guitar"] = false;
        }
    }

    void Script_XRKamp::addNpc(CScriptGameObject* const p_npc)
    {
        if (!p_npc)
        {
            MESSAGEWR("can't register npc, because p_npc == nullptr!");
            return;
        }

        if (this->m_npc.find(p_npc->ID()) != this->m_npc.end())
        {
            MESSAGEWR("npc %d is already exist, can't add ", p_npc->ID());
            return;
        }

        if (Globals::character_community(p_npc) == "monolith" || Globals::character_community(p_npc) == "zombied")
        {
            NpcData data;
            data.setNpcName(p_npc->Name());
			data.setState("stand_wait", false);
			data.setState("sit", false);
			data.setState("sit_ass", false);
			data.setState("sit_knee", false);
			data.setState("declarate", true);
			data.setState("eat_kolbasa", false);
			data.setState("eat_vodka", false);
			data.setState("eat_energy", false);
			data.setState("eat_bread", false);
			data.setState("trans", true);
			data.setState("play_harmonica", false);
			data.setState("play_guitar", false);
            this->m_npc[p_npc->ID()] = data;
        }
        else
        {
            NpcData data;
            data.setNpcName(p_npc->Name());
			data.setState("stand_wait", true);
			data.setState("sit", true);
			data.setState("sit_ass", true);
			data.setState("sit_knee", true);
			data.setState("declarate", true);
			data.setState("eat_kolbasa", false);
			data.setState("eat_vodka", false);
			data.setState("eat_energy", false);
			data.setState("eat_bread", false);
            data.setState("trans", false);
			data.setState("play_harmonica", false);
			data.setState("play_guitar", false);
            this->m_npc[p_npc->ID()] = data;
        }


        this->select_position(p_npc->ID());
        this->m_p_sound_manager->register_npc(p_npc->ID());
    }

    void Script_XRKamp::removeNpc(CScriptGameObject* const p_npc)
    {
        this->m_p_sound_manager->unregister_npc(p_npc->ID());
        std::uint16_t npc_id = p_npc->ID();

        if (this->m_npc.find(npc_id) != this->m_npc.end())
        {

            if (!this->m_npc.at(npc_id).isEmpty())
            {
				if (this->m_director_id == npc_id)
				{
					this->m_director_id = 0;
					this->m_npc.at(npc_id).setBegin(0);
					this->m_kamp_state_name = "idle";
					this->m_begin = Globals::get_time_global();

					for (std::pair<const std::uint16_t, NpcData>& it : this->m_npc)
					{
						it.second.setNew(true);
					}

					Globals::stop_play_sound(p_npc);
				}
            }
            

            std::uint32_t position = this->m_npc.at(npc_id).getPosition();
            if (position >= this->m_position.size())
            {
                MESSAGEWR("Something is wrong, size can't be large or equal existed xr_vector, check your code please");
                return;
            }
            this->m_position.at(position).second = 0;
            this->m_npc[npc_id] = NpcData();
        }
    }

    void Script_XRKamp::increasePops(void)
    {
        ++this->m_population;

    }

    void Script_XRKamp::decreasePops(void) {}

    Script_EvaluatorKampEnd::_value_type Script_EvaluatorOnPosition::evaluate(void)
    {
        if (this->m_object->level_vertex_id() == this->m_p_storage->getXRKampPosVertex())
        {
            return true;
        }

        return false;
    }

    Script_EvaluatorKampEnd::_value_type Script_EvaluatorKampEnd::evaluate(void)
    {
        return !XR_LOGIC::is_active(this->m_object, *this->m_p_storage);
    }

    } // namespace Scripts
    }
