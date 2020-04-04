#include "stdafx.h"
#include "Script_SchemeXRWalker.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeXRWalker::Script_SchemeXRWalker(const xr_string& action_name, DataBase::Storage_Scheme& storage)
    : inherited_scheme(nullptr, action_name, storage), m_is_in_camp(false), m_p_camp(nullptr)
{
    this->m_p_storage->setXRWalkerDescriptionName("walker_camp");
    this->m_avail_actions =
        Script_GlobalHelper::getInstance().getAnimpointTable().at(this->m_p_storage->getXRWalkerDescriptionName());

    for (std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>& it : this->m_avail_actions)
    {
        if (it.first(this->m_npc->ID(), false))
        {
            this->m_p_storage->setApprovedActions(it);
        }
    }
}

Script_SchemeXRWalker::~Script_SchemeXRWalker(void) {}

void Script_SchemeXRWalker::initialize(void)
{
    CScriptActionBase::initialize();
    this->m_object->set_desired_position();
    this->m_object->set_desired_direction();
    this->reset_scheme(false, this->m_npc);
}

void Script_SchemeXRWalker::execute(void)
{
    CScriptActionBase::execute();

    Script_MoveManager* const p_move_manager = DataBase::Storage::getInstance().getStorage().at(this->m_npc->ID()).getMoveManager();

    if (!p_move_manager)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_move_manager->update();

    Script_CampData* const p_camp = Globals::get_current_camp(this->m_object->Position());

    if (p_camp && this->m_p_storage->isXRWalkerUseCamp())
    {
        this->m_p_camp = p_camp;
        this->m_p_camp->register_npc(this->m_object->ID());
        this->m_is_in_camp = true;
    }
    else
    {
        if (this->m_is_in_camp)
        {
            this->m_p_camp->unregister_npc(this->m_object->ID());
            this->m_is_in_camp = false;
        }
    }

    if (!this->m_is_in_camp && !this->m_p_storage->getXRWalkerSoundIdleName().empty())
    {
        xr_string faction_name;
        XR_SOUND::set_sound_play(this->m_object->ID(), this->m_p_storage->getXRWalkerSoundIdleName(), faction_name, 0);
    }
}

void Script_SchemeXRWalker::finalize(void)
{
    Script_MoveManager* const p_move_manager = DataBase::Storage::getInstance().getStorage().at(this->m_npc->ID()).getMoveManager();

    if (!p_move_manager)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    if (this->m_is_in_camp)
    {
        this->m_p_camp->unregister_npc(this->m_object->ID());
        this->m_is_in_camp = false;
    }

    CScriptActionBase::finalize();
}

void Script_SchemeXRWalker::activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object)
{
    this->m_p_storage->ClearSignals();
    this->reset_scheme(is_loading, p_client_object);
}

void Script_SchemeXRWalker::update(const float delta) 
{ 
    if (this->m_p_camp == nullptr)
        return;

    xr_string camp_action_name;
    bool is_director = false;

    this->m_p_camp->get_camp_action(this->m_object->ID(), camp_action_name, is_director);

    if (!is_director)
        return;

    xr_string animation_name = Script_GlobalHelper::getInstance().getXRWalkerAssociation().at(camp_action_name);

    Globals::set_state(this->m_object, animation_name, StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(Fvector(), nullptr), StateManagerExtraData());
}

void Script_SchemeXRWalker::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    DataBase::Storage_Scheme* const p_storage =
        XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);
    
    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
    xr_string path_walk_name = Globals::Utils::cfg_get_string(p_ini, section_name, "path_walk");
    if (path_walk_name.empty())
        path_walk_name = gulag_name;
    p_storage->setXRWalkerPathWalkName(path_walk_name);
    
    if (!Globals::patrol_path_exists(path_walk_name.c_str()))
    {
        Msg("[Scripts/Script_SchemeXRWalker/set_scheme(p_client_object, p_ini, scheme_name, section_name, gulag_name)] WARNING: can't find path_walk by name %s Return ...", path_walk_name.c_str());
        return;
    }

    xr_string path_look_name = Globals::Utils::cfg_get_string(p_ini, section_name, "path_look");

    if (path_look_name.empty())
        path_look_name = gulag_name;

    p_storage->setXRWalkerPathLookName(path_look_name);

    if (path_walk_name == path_look_name)
    {
        Msg("[Scripts/Script_SchemeXRWalker/set_scheme(p_client_object, p_ini, scheme_name, section_name, gulag_name)] WARNING: your scheme doesn't have any paths! Return ...");
        return;
    }

    xr_string team_name = Globals::Utils::cfg_get_string(p_ini, section_name, "team");

    if (team_name.empty())
        team_name = gulag_name;

    p_storage->setXRWalkerTeamName(team_name);
    p_storage->setXRWalkerSoundIdleName(Globals::Utils::cfg_get_string(p_ini, section_name, "sound_idle"));
    p_storage->setXRWalkerUseCamp(Globals::Utils::cfg_get_bool(p_ini, section_name, "use_camp"));

    p_storage->setXRWalkerSuggestedStates("standing", Globals::Utils::cfg_get_string(p_ini, section_name, "def_state_standing"));
    p_storage->setXRWalkerSuggestedStates("moving", Globals::Utils::cfg_get_string(p_ini, section_name, "def_state_moving1"));
    p_storage->setXRWalkerSuggestedStates("moving", Globals::Utils::cfg_get_string(p_ini, section_name, "def_state_moving"));
}

} // namespace Scripts
} // namespace Cordis
