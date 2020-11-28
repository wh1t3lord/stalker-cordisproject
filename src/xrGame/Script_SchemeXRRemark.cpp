#include "stdafx.h"
#include "Script_SchemeXRRemark.h"

namespace Cordis
{
namespace Scripts
{
void init_target(CScriptGameObject* const p_client_object, const xr_string& target_buffer, Fvector& target_position,
    std::uint16_t& target_id, bool& is_target_initialized)
{
    auto func_parse_target = [](xr_string buffer, xr_string& target_name, xr_string& number_name) -> void {
        if (buffer.find(',') == xr_string::npos)
        {
            target_name = buffer;
        }
        else
        {
            number_name = buffer.substr(buffer.rfind(",") + 1);
            target_name = buffer.erase(buffer.find(","));

          //  boost::algorithm::trim(number_name);
            _Trim(number_name);
          //  boost::algorithm::trim(target_name);
            _Trim(target_name);
        }
    };

    auto func_parse_type = [](xr_string buffer, xr_string& job_type, xr_string& target_data) -> void {
        if (buffer.find('|') == xr_string::npos)
        {
            MESSAGEER("can't parse string "
                "because can't have symbol |");
            return;
        }

      //  boost::algorithm::trim(buffer);
        _Trim(buffer);
        target_data = buffer.substr(buffer.rfind("|") + 1);
     //   boost::algorithm::trim(target_data);
        _Trim(target_data);
        job_type = buffer.erase(buffer.find("|"));

      //  boost::algorithm::trim(job_type);
        _Trim(job_type);
    };

    target_position.set(0.0f, 0.0f, 0.0f);
    target_id = 0;
    is_target_initialized = false;

    if (target_buffer == "nil") // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
        return;

    if (target_buffer.empty())
    {
        R_ASSERT2(false, "it can't be!");
        return;
    }

    xr_string target_type_name;
    xr_string target_data;
    func_parse_type(target_buffer, target_type_name, target_data);
    if (target_type_name == "story")
    {
        xr_string story_id_name;
        xr_string not_used_string;
        func_parse_target(target_data, story_id_name, not_used_string);
        target_id = Globals::get_story_object_id(story_id_name);
        is_target_initialized = true;
    }
    else if (target_type_name == "path")
    {
        xr_string path_name;
        xr_string point_data_name;
        func_parse_target(target_data, path_name, point_data_name);
        std::uint32_t point = static_cast<std::uint32_t>(atoi(point_data_name.c_str()));
        target_position = CPatrolPathParams(path_name.c_str()).point(point);
        is_target_initialized = true;
    }
    else if (target_type_name == "job")
    {
        xr_string job_name;
        xr_string gulag_name;

        func_parse_target(target_data, job_name, gulag_name);
        Script_SE_SmartTerrain* p_server_gulag = nullptr;
        if (!gulag_name.empty())
        {
            p_server_gulag = Script_SimulationBoard::getInstance().getSmartTerrainsByName().at(gulag_name);
        }
        else
        {
            p_server_gulag = XR_GULAG::get_npc_smart(p_client_object)->cast_script_se_smartterrain();
        }

        if (!p_server_gulag)
        {
            R_ASSERT2(false, "it can't be!");
            return;
        }

        target_id = p_server_gulag->getIDNPCOnJob(job_name);
        is_target_initialized = (target_id != 0 && target_id != Globals::kUnsignedInt16Undefined);
    }
    else
    {
        R_ASSERT2(false, "it can't be!");
        return;
    }

    Msg("[Scripts/init_target(p_client_object, target_buffer, target_position, target_id, is_target_initialized)] "
        "Returned data: %f %f %f %d %d",
        target_position.x, target_position.y, target_position.z, target_id, std::uint8_t(is_target_initialized));
}

Script_EvaluatorNeedRemark::_value_type Script_EvaluatorNeedRemark::evaluate(void)
{
    return XR_LOGIC::is_active(this->m_object, this->m_p_storage);
}

Script_SchemeXRRemark::~Script_SchemeXRRemark(void) {}

void Script_SchemeXRRemark::initialize(void)
{
    CScriptActionBase::initialize();
    this->m_object->set_desired_position();
    this->m_object->set_desired_direction();
}

void Script_SchemeXRRemark::execute(void)
{
    CScriptActionBase::execute();
    this->update(0.0f);
}

void Script_SchemeXRRemark::finalize(void) { CScriptActionBase::finalize(); }

void Script_SchemeXRRemark::activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object)
{
    this->m_p_storage->ClearSignals();
    this->m_is_sound_end_signalled = false;
    this->m_is_action_end_signalled = false;
    this->m_is_animation_end_signalled = false;

    if (!this->m_p_storage->isSoundAnimationSync() && !this->m_p_storage->getSoundName().empty())
    {
        this->m_is_sound_scheduled = true;
    }
    else
    {
        this->m_is_sound_scheduled = false;
    }

    this->m_is_sound_started = false;
    this->m_state = XR_REMARK::kStateInitial;
}

void Script_SchemeXRRemark::update(const float delta)
{
    Msg("[Scripts/Script_SchemeXRRemark/update(delta)] %s state->%d", this->m_object->Name(), this->m_state);

    if (this->m_state == XR_REMARK::kStateInitial)
    {
        StateManagerCallbackData callback;
        callback.setCallbackTime(std::bind(&Script_SchemeXRRemark::time_callback, this));
        std::pair<CScriptGameObject*, Fvector> target = this->get_target();
        if (!target.first && Globals::is_vector_nil(target.second))
        {
            xr_string state_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
                this->m_object, this->m_p_storage->getAnimationCondlist());
            Globals::set_state(this->m_object, state_name, callback, 0,
                std::pair<Fvector, CScriptGameObject* const>(Fvector(), nullptr), StateManagerExtraData());
            this->m_state = XR_REMARK::kStateAnimation;
            return;
        }

        xr_string state_name = XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
            this->m_object, this->m_p_storage->getAnimationCondlist());
        Globals::set_state(this->m_object, state_name, callback, 0,
            std::pair<Fvector, CScriptGameObject* const>(target.second, target.first), StateManagerExtraData());
        this->m_state = XR_REMARK::kStateAnimation;
    }
    else if (this->m_state == XR_REMARK::kStateSound)
    {
        if (this->m_is_sound_scheduled)
        {
            this->m_is_sound_started = true;
            xr_string faction;
            XR_SOUND::set_sound_play(this->m_object->ID(), this->m_p_storage->getSoundName(), faction, 0);
        }

        if (!this->m_is_animation_end_signalled)
        {
            this->m_is_animation_end_signalled = true;
            this->m_p_storage->setSignals("anim_end", true);
        }

        if (this->m_p_storage->getSignals().at("sound_end") || this->m_p_storage->getSignals().at("theme_end"))
        {
            if (!this->m_is_sound_end_signalled)
                this->m_is_sound_end_signalled = true;
        }

        if (this->m_is_action_end_signalled && this->m_is_animation_end_signalled)
        {
            if (!this->m_is_action_end_signalled)
            {
                this->m_p_storage->setSignals("action_end", true);
                this->m_is_action_end_signalled = true;
            }
        }
    }
}

void Script_SchemeXRRemark::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Script_ComponentScheme_XRRemark* p_storage =
        XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_XRRemark>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
    p_storage->setSoundAnimationSync(Globals::Utils::cfg_get_bool(p_ini, section_name, "snd_anim_sync"));
    p_storage->setSoundName(Globals::Utils::cfg_get_string(p_ini, section_name, "snd"));
    xr_string animation_name = Globals::Utils::cfg_get_string(p_ini, section_name, "anim");
    if (animation_name.empty())
        animation_name = "wait";

    p_storage->setAnimationCondlist(XR_LOGIC::parse_condlist_by_script_object("anim", "anim", animation_name));

    xr_string target_name = Globals::Utils::cfg_get_string(p_ini, section_name, "target");

    if (target_name.empty())
        target_name = "nil"; // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()

    p_storage->setTargetName(target_name);
    p_storage->setTargetID(0);
    p_storage->setTargetPosition(Fvector());
}

std::pair<CScriptGameObject*, Fvector> Script_SchemeXRRemark::get_target(void)
{
    std::pair<CScriptGameObject*, Fvector> result;
    result.first = nullptr;
    Fvector target_position;
    std::uint16_t target_id = 0;
    bool is_target_initialized = false;

    init_target(
        this->m_npc, this->m_p_storage->getTargetName(), target_position, target_id, is_target_initialized);

    this->m_p_storage->setTargetID(target_id);
    this->m_p_storage->setTargetInitialized(is_target_initialized);
    this->m_p_storage->setTargetPosition(target_position);

    if (!is_target_initialized)
        return result;

    if (target_id)
    {
        result.first = Globals::Game::level::get_object_by_id(target_id);
    }

    result.second = target_position;

    return result;
}

} // namespace Scripts
} // namespace Cordis
