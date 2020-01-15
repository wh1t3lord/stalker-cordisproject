#include "stdafx.h"
#include "Script_MoveManager.h"

namespace Cordis
{
namespace Scripts
{
constexpr std::uint32_t kStateNone = 0;
constexpr std::uint32_t kStateMoving = 1;
constexpr std::uint32_t kStateStanding = 2;
constexpr std::uint32_t kKeepStateMinTime = 1500;
constexpr float kDistanceWalk = 10.0f;
constexpr float kDistanceRun = 2500.0f;
constexpr std::uint32_t kWalkMinTime = 3000;
constexpr std::uint32_t kRunMinTime = 2000;
constexpr std::uint32_t kDefaultWaitTime = 10000;
constexpr const char* kDefaultStateStandingName = "guard";
constexpr const char* kDefaultStateMovingName1 = "patrol";
constexpr const char* kDefaultStateMovingName2 = "patrol";
constexpr const char* kDefaultStateMovingName3 = "patrol";
constexpr std::uint32_t kArrivialAfterRotation = 1;
xr_map<xr_string, xr_map<std::uint16_t, bool>> synchronization;



Script_MoveManager::Script_MoveManager(CScriptGameObject* const p_client_object)
    : m_p_client_object(p_client_object), m_is_at_terminal_waypoint(false), m_is_no_validation(false),
      m_p_patrol_walk(nullptr), m_is_can_use_get_current_point_index(false), m_current_point_initialization_time(0),
      m_current_point_index(0), m_last_index(0), m_p_patrol_look(nullptr), m_synchronization_signal_set_tm(0),
      m_keep_state_until(0), m_pt_wait_time(0), m_last_look_index(0), m_retval_after_rotation(0),
      m_move_callback_info(nullptr)
{
}

Script_MoveManager::~Script_MoveManager(void) {}

void Script_MoveManager::validate_paths(void)
{
    if (this->m_is_no_validation)
        return;

    std::uint32_t patrol_walk_count = this->m_p_patrol_walk->count();
    if (patrol_walk_count == 1)
    {
        if (!this->m_path_walk_info.getData()[0].getFlags().get())
        {
            R_ASSERT2(false, "path_walk has 1 waypoint, but has no flags");
        }
    }
}

void Script_MoveManager::extrapolate_callback(CScriptGameObject* const p_client_object)
{
    this->m_is_can_use_get_current_point_index = true;
    this->m_current_point_initialization_time = Globals::get_time_global();
    this->m_current_point_index = this->m_p_client_object->get_current_patrol_point_index();
}

bool Script_MoveManager::standing_on_terminal_waypoint(std::uint32_t& return_index)
{
    for (std::uint32_t index = 0; index < this->m_p_patrol_walk->count(); ++index)
    {
        if (Globals::Utils::is_stalker_at_waypoint(this->m_p_client_object, *this->m_p_patrol_walk, index) &&
            this->m_p_patrol_walk->terminal(index))
        {
            return_index = index;
            return true;
        }
    }

    return false;
}

void Script_MoveManager::Continue(void) { this->setup_movement_by_patrol_path(); }

void Script_MoveManager::setup_movement_by_patrol_path(void)
{
    this->m_p_client_object->set_path_type(MovementManager::ePathTypePatrolPath);
    this->m_p_client_object->set_detail_path_type(DetailPathManager::eDetailPathTypeSmooth);

    if (this->m_current_point_index)
    {
        this->m_p_client_object->set_start_point(this->m_current_point_index);
        this->m_p_client_object->set_patrol_path(
            this->m_path_walk_name.c_str(), ePatrolStartTypeNext, ePatrolRouteTypeContinue, true);
    }
    else
        this->m_p_client_object->set_patrol_path(
            this->m_path_walk_name.c_str(), ePatrolStartTypeNearest, ePatrolRouteTypeContinue, true);

    this->m_state = kStateMoving;

    std::uint32_t index = 0;
    bool is_terminal = this->standing_on_terminal_waypoint(index);

    if (is_terminal)
    {
        Msg("[Scripts/Script_MoveManager/setup_movement_by_patrol_path(void)] terminal waypoint! %s",
            this->m_p_client_object->Name());
        this->waypoint_callback(this->m_p_client_object, 0, index);
    }
    else
    {
        this->update_movement_state();
    }
}

bool Script_MoveManager::is_arrived_to_first_waypoint(void) { return (!!this->m_last_index); }

bool Script_MoveManager::is_synchronization_succesful(void)
{
    if (!this->m_team_name.empty())
    {
        xr_map<std::uint16_t, bool>& data = synchronization[this->m_team_name];
        std::uint16_t self_id = this->m_p_client_object->ID();

        for (std::pair<const std::uint16_t, bool>& it : data)
        {
            CScriptGameObject* const p_client_object = Globals::Game::level::get_object_by_id(it.first);
            if (p_client_object && p_client_object->Alive())
            {
                if (!it.second)
                    return false;
            }
            else
            {
                synchronization[this->m_team_name][it.first] = false;
            }
        }
    }

    return true;
}

void Script_MoveManager::update(void)
{
    if (!this->m_synchronization_signal_name.empty() &&
        (Globals::get_time_global() >= this->m_synchronization_signal_set_tm))
    {
        if (this->is_synchronization_succesful())
        {
            this->scheme_set_signal(this->m_synchronization_signal_name);
            this->m_synchronization_signal_name.clear();
        }
    }

    if (this->m_is_can_use_get_current_point_index && !this->is_arrived_to_first_waypoint())
    {
        std::uint32_t current_time = Globals::get_time_global();

        if (current_time >= this->m_keep_state_until)
        {
            this->m_keep_state_until = current_time + kKeepStateMinTime;
            std::uint32_t current_point_index = this->m_current_point_index;
            float distance =
                this->m_p_client_object->Position().distance_to(this->m_p_patrol_walk->point(current_point_index));

            if (distance <= kDistanceWalk || current_time < this->m_walk_until)
            {
                this->m_current_state_moving_name =
                    XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
                        this->m_p_client_object, this->m_default_state_moving1_condlist);
            }
            else if (distance <= kDistanceRun || current_time < this->m_run_until)
            {
                this->m_current_state_moving_name =
                    XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
                        this->m_p_client_object, this->m_default_state_moving2_condlist);
            }
            else
            {
                this->m_current_state_moving_name =
                    XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(),
                        this->m_p_client_object, this->m_default_state_moving3_condlist);
            }

            this->update_movement_state();
        }
    }
}

void Script_MoveManager::finalize(CScriptGameObject* const p_client_object)
{
    if (!this->m_team_name.empty())
    {
        synchronization[this->m_team_name][this->m_p_client_object->ID()] = false;
    }

    this->m_p_client_object->set_path_type(MovementManager::EPathType::ePathTypeLevelPath);
}

void Script_MoveManager::update_movement_state(void)
{
    Globals::set_state(this->m_p_client_object, this->m_current_state_moving_name, StateManagerCallbackData(), 0,
        std::pair<Fvector, CScriptGameObject* const>(Fvector(), nullptr), StateManagerExtraData());
}

void Script_MoveManager::update_standing_state(const Fvector& look_position)
{
    StateManagerCallbackData callback;
    callback.setCallbackTime(std::bind(&Script_MoveManager::time_callback, this));
    callback.setCallbackTurnEnd(std::bind(&Script_MoveManager::turn_end_callback, this));
    Globals::set_state(this->m_p_client_object, this->m_current_state_moving_name, callback, this->m_pt_wait_time,
        std::pair<Fvector, CScriptGameObject* const>(look_position, nullptr), StateManagerExtraData());
}

void Script_MoveManager::time_callback(void)
{
    xr_string sigtm = this->m_path_look_info.getData()[this->m_last_look_index].getValue("sigtm");
    if (!sigtm.empty())
    {
        this->scheme_set_signal(sigtm);
    }

    if (DataBase::Storage::getInstance().getStorage().at(this->m_p_client_object->ID()).getActiveSchemeName().empty())
        return;

    if (this->m_last_index && this->m_p_patrol_walk->terminal(this->m_last_index))
    {
        if (Globals::Utils::is_stalker_at_waypoint(this->m_p_client_object, *this->m_p_patrol_walk, this->m_last_index))
        {
            this->waypoint_callback(this->m_p_client_object, 0, this->m_last_index);
            return;
        }
    }
    else
    {
        this->update_movement_state();
        xr_string synchonization_name = this->m_path_look_info.getData()[this->m_last_look_index].getValue("syn");
        if (!synchonization_name.empty())
        {
            R_ASSERT2(false, "syn flag used on non-terminal waypoint");
        }
    }
}

void Script_MoveManager::scheme_set_signal(const xr_string& signal_name)
{
    const DataBase::Storage_Data& storage =
        DataBase::Storage::getInstance().getStorage().at(this->m_p_client_object->ID());

    if (storage.getSchemes().find(storage.getActiveSchemeName()) != storage.getSchemes().end())
    {
        DataBase::Storage::getInstance().setStorageSignal(this->m_p_client_object->ID(), signal_name, true);
    }
}

void Script_MoveManager::turn_end_callback(void)
{
    xr_string synchronization_name = this->m_path_look_info.getData()[this->m_last_look_index].getValue("syn");

    if (!synchronization_name.empty())
    {
        this->m_synchronization_signal_name = this->m_path_look_info.getData()[this->m_last_look_index].getValue("sig");
        if (this->m_synchronization_signal_name.empty())
        {
            R_ASSERT2(false, "syn flag used without sig flag");
        }

        if (!this->m_team_name.empty())
        {
            synchronization[this->m_team_name][this->m_p_client_object->ID()] = true;
        }
    }
    else
    {
        xr_string signal_name = this->m_path_look_info.getData()[this->m_last_look_index].getValue("sig");
        if (!signal_name.empty())
        {
            this->scheme_set_signal(signal_name);
        }
        else
        {
            this->scheme_set_signal("turn_end");
        }
    }

    if (this->m_retval_after_rotation)
    {
        if (!this->m_move_callback_info)
        {
            R_ASSERT2(false,
                "you don't pass the argument for callback in reset method in this class! Check any instances of that "
                "class in your code!");
        }

        Globals::set_state(this->m_p_client_object, this->m_current_state_moving_name, StateManagerCallbackData(), 0,
            std::pair<Fvector, CScriptGameObject* const>(Fvector(), nullptr), StateManagerExtraData());

        if (this->m_move_callback_info)
        {
            if (this->m_move_callback_info(kArrivialAfterRotation, this->m_retval_after_rotation))
            {
                return;
            }
        }

        this->update_standing_state(this->m_p_patrol_look->point(this->m_last_look_index));
    }
}

void Script_MoveManager::waypoint_callback(
    CScriptGameObject* p_client_object, const std::uint32_t action_type_movement, const std::uint32_t point_index)
{
    if (!point_index || point_index == Globals::kUnsignedInt32Undefined)
        return;

    this->m_last_index = point_index;

    if (this->m_p_patrol_walk->terminal(point_index))
        this->m_is_at_terminal_waypoint = true;

    xr_string condlist_data_name = this->m_path_walk_info.getData()[point_index].getValue("a");
    if (!condlist_data_name.empty())
    {
        xr_map<std::uint32_t, CondlistData> condlist_animation =
            XR_LOGIC::parse_condlist_by_script_object("waypoint_data", "anim_state", condlist_data_name);
        this->m_current_state_moving_name = XR_LOGIC::pick_section_from_condlist(
            DataBase::Storage::getInstance().getActor(), this->m_p_client_object, condlist_animation);

        if (this->m_current_state_moving_name == "true")
        {
            R_ASSERT2(false,
                "you can't pass here true or false statements. Only animation state name as patrol binocular etc");
            return;
        }
    }
    else
    {
        this->m_current_state_moving_name =
            XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_p_client_object,
                this->m_default_state_moving1_condlist);
        if (this->m_current_state_moving_name == "true")
        {
            R_ASSERT2(
                false, "you can't pass here true or false statements. Only animation state name as patrol guard etc");
        }
    }

    xr_string retv_name = this->m_path_walk_info.getData()[point_index].getValue("ret");

    if (!retv_name.empty())
    {
        std::uint32_t retv_number = boost::lexical_cast<std::uint32_t>(retv_name);
        if (!this->m_move_callback_info)
        {
            R_ASSERT2(false, "you couldn't pass as argument function for that member's class");
        }

        if (this->m_move_callback_info)
        {
            if (this->m_move_callback_info(kArrivialAfterRotation, retv_number))
            {
                return;
            }
        }
    }

    // Lord: проверить здесь point_index и размер с -1
    xr_string signal_name = this->m_path_walk_info.getData()[point_index].getValue("sig");
    if (!signal_name.empty())
    {
        this->scheme_set_signal(signal_name);
    }
    else if (point_index == this->m_path_walk_info.getData().size() - 1)
    {
        this->scheme_set_signal("path_end");
    }

    xr_string stop_probability_name = this->m_path_walk_info.getData()[point_index].getValue("p");
    std::uint32_t probability = boost::lexical_cast<std::uint32_t>(stop_probability_name);
    if (!this->m_p_patrol_look || (probability < Globals::Script_RandomInt::getInstance().Generate(1, 100)))
    {
        this->update_movement_state();
        return;
    }

    if (!this->m_path_walk_info.getData()[point_index].getFlags().get())
    {
        this->update_movement_state();
        return;
    }


}

} // namespace Scripts
} // namespace Cordis
