#include "stdafx.h"
#include "Script_SchemeXRSleeper.h"

constexpr std::uint32_t kStateWalking = 0;
constexpr std::uint32_t kStateSleeping = 1;


namespace Cordis
{
    namespace Scripts
    {
   

    Script_SchemeXRSleeper::~Script_SchemeXRSleeper(void) {}

    void Script_SchemeXRSleeper::initialize(void) 
    {
        CScriptActionBase::initialize();

        this->m_object->set_desired_position();
        this->m_object->set_desired_direction();

        this->reset_scheme(false, nullptr);
    }

    void Script_SchemeXRSleeper::execute(void) 
    {
        CScriptActionBase::execute();
        if (!this->m_is_reset)
            this->reset_scheme(false, nullptr);

        if (this->m_state == kStateWalking)
        {
            this->m_p_move_manager->update();
            return;
        }
    }

    void Script_SchemeXRSleeper::finalize(void) 
    { 
        this->m_p_move_manager->finalize(nullptr);
        CScriptActionBase::finalize();
    }

    void Script_SchemeXRSleeper::activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object) 
    {
        this->m_is_reset = true;
    }

    void Script_SchemeXRSleeper::reset_scheme(const bool value, CScriptGameObject* const p_client_object) 
    {
        this->m_p_storage->ClearSignals();
        this->m_state = kStateWalking;

        if (this->m_p_storage->getPathWalkInfo().getData().empty())
        {
            CPatrolPathParams patrol_main = CPatrolPathParams(this->m_p_storage->getPathMainName().c_str());

            if (patrol_main.m_path == nullptr)
            {
                MESSAGEWR("can't find path by %s", this->m_p_storage->getPathMainName().c_str());
                return;
            }

            std::uint32_t num_ways = patrol_main.count();

            if (num_ways == 1)
            {
                this->m_p_storage->setPathWalkName(this->m_p_storage->getPathMainName());
                this->m_p_storage->setPathWalkInfo(Globals::Utils::path_parse_waypoints_argumentlist(
                    this->m_p_storage->getPathMainName(), 1, {{0, "wp00|ret=1"}}));
                this->m_p_storage->setPathLookName("");
                this->m_p_storage->setPathLookInfo(CondlistWaypoints());
            }
            else if (num_ways == 2)
            {
                this->m_p_storage->setPathWalkName(this->m_p_storage->getPathMainName());
                this->m_p_storage->setPathWalkInfo(Globals::Utils::path_parse_waypoints_argumentlist(
                    this->m_p_storage->getPathMainName(), 2, {{1, "wp00"}, {0, "wp01"}}));
                this->m_p_storage->setPathLookName(this->m_p_storage->getPathMainName());
                this->m_p_storage->setPathLookInfo(Globals::Utils::path_parse_waypoints_argumentlist(
                    this->m_p_storage->getPathMainName(), 2, {{0, "wp00"}, {1, "wp01|ret=1"}}));
            }
            else
            {
                Msg("[Scripts/Script_SchemeXRSleeper/reset_scheme(value, p_client_object)] WARNING: must count of waypoints == 1 OR == 2! Return ...");
                return;
            }
        }

        std::function<bool(std::uint32_t, std::uint32_t)> my_callback = std::bind(&Script_SchemeXRSleeper::callback, this, std::placeholders::_1, std::placeholders::_2);
        this->m_p_move_manager->reset(this->m_p_storage->getPathWalkName(),
            this->m_p_storage->getPathWalkInfo(), this->m_p_storage->getPathLookName(),
            this->m_p_storage->getPathLookInfo(), "", xr_map<xr_string, xr_string>(), my_callback, true); // Lord: протестировать будет ли регистрировать callback и не будет ли он удаляться! В ином случае брать как копию!
        this->m_is_reset = true;
    }

    bool Script_SchemeXRSleeper::callback(std::uint32_t mode, std::uint32_t number) 
    { 
        this->m_state = kStateSleeping;
        Fvector position;
        CPatrolPathParams patrol = CPatrolPathParams(this->m_p_storage->getPathMainName().c_str());
        if (patrol.count() == 2)
            position = patrol.point(1);

        if (this->m_p_storage->isWakeable())
        {
            Globals::set_state(this->m_object, "sit", StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(position, nullptr), StateManagerExtraData());
        }
        else 
        {
            Globals::set_state(this->m_object, "sleep", StateManagerCallbackData(), 0,
                std::pair<Fvector, CScriptGameObject* const>(position, nullptr), StateManagerExtraData());
        }

        return true;
    }

    void Script_SchemeXRSleeper::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
    {
        DataBase::Storage_Scheme* const p_storage =
            XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);

        p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
        p_storage->setPathMainName(Globals::Utils::cfg_get_string(p_ini, section_name, "path_main"));
        p_storage->setWakeable(Globals::Utils::cfg_get_bool(p_ini, section_name, "wakeable"));
    }

    } // namespace Scripts
}
