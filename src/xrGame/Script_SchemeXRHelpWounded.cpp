#include "stdafx.h"
#include "Script_SchemeXRHelpWounded.h"

namespace Cordis
{
namespace Scripts
{
Script_ActionXRHelpWounded::Script_ActionXRHelpWounded(const xr_string& action_name, DataBase::Storage_Scheme& storage)
    : inherited_scheme(nullptr, action_name.c_str()), m_p_storage(&storage)
{
    this->m_scheme_name = "help_wounded";
}

Script_ActionXRHelpWounded::~Script_ActionXRHelpWounded(void) {}

void Script_ActionXRHelpWounded::initialize(void)
{
    CScriptActionBase::initialize();
    this->m_object->set_desired_position();
    this->m_object->set_desired_direction();
    this->m_object->set_dest_level_vertex_id(this->m_p_storage->getLevelVertexID());

    Globals::set_state(this->m_object, "patrol", StateManagerCallbackData(), 0,
        std::pair<Fvector, CScriptGameObject* const>(Fvector(), nullptr), StateManagerExtraData());
}

void Script_ActionXRHelpWounded::execute(void)
{
    CScriptActionBase::execute();

    if (this->m_object->Position().distance_to_sqr(this->m_p_storage->getVertexPosition()) > 2.0f)
    {
        return;
    }

    std::pair<Fvector, CScriptGameObject* const> target(this->m_p_storage->getVertexPosition(), nullptr);
    Globals::set_state(this->m_object, "help_wounded", StateManagerCallbackData(), 0, target, StateManagerExtraData());
}

void Script_ActionXRHelpWounded::finalize(void) { CScriptActionBase::finalize(); }

void Script_ActionXRHelpWounded::set_help_wounded(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    DataBase::Storage_Scheme* p_storage =
        XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, "");

    if (!p_storage)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }
}

Script_EvaluatorWoundedExist::_value_type Script_EvaluatorWoundedExist::evaluate(void)
{
    if (!this->m_object->Alive())
        return false;

    if (this->m_object->GetBestEnemy())
        return false;

    if (!strcmp(this->m_object->CharacterCommunity(), "zombied"))
        return false;

    if (!this->m_p_storage->IsXRHelpWoundedHelpWoundedEnabled())
        return false;

    if (CRD_Wounded::is_wounded(this->m_object))
        return false;

    if (!strcmp(this->m_object->Section(), "actor_visual_stalker"))
        return false;

    float nearest_distance = 900.0f;
    std::uint32_t nearest_vertex_id = 0;
    Fvector nearest_position;
    std::uint16_t selected_id = 0;

    for (const MemorySpace::CVisibleObject& it : this->m_object->memory_visible_objects())
    {
        CScriptGameObject* const p_see_object = it.m_object->lua_game_object();
        std::uint16_t id = p_see_object->ID();
        std::uint16_t npc_id = this->m_object->ID();

        if (this->m_object->CheckObjectVisibility(p_see_object) && CRD_Wounded::is_wounded(p_see_object) &&
            (DataBase::Storage::getInstance().getStorage().at(id).getWoundedAlreadySelected() == 0 ||
                DataBase::Storage::getInstance().getStorage().at(id).getWoundedAlreadySelected() == npc_id) &&
            p_see_object->Alive())
        {
            const Fvector& npc_position = this->m_object->Position();
            const Fvector& see_object_position = p_see_object->Position();

            if (npc_position.distance_to_sqr(see_object_position) < nearest_distance)
            {
                std::uint32_t vertex = Globals::Game::level::vertex_id(see_object_position);
                if (this->m_object->accessible_vertex_id(vertex))
                {
                    nearest_distance = npc_position.distance_to_sqr(see_object_position);
                    nearest_vertex_id = vertex;
                    nearest_position = npc_position;
                    selected_id = npc_id;
                }
            }
        }
    }

    if (nearest_vertex_id)
    {
        this->m_p_storage->setLevelVertexID(nearest_vertex_id);
        this->m_p_storage->setVertexPosition(nearest_position);

        if (this->m_p_storage->getSelectedID() && this->m_p_storage->getSelectedID() != selected_id &&
            DataBase::Storage::getInstance().getStorage().find(this->m_p_storage->getSelectedID()) !=
                DataBase::Storage::getInstance().getStorage().end())
        {
            DataBase::Storage::getInstance().setStorageWoundedAlreadySelected(this->m_p_storage->getSelectedID(), 0);
        }

        this->m_p_storage->setSelectedID(selected_id);

        DataBase::Storage::getInstance().setStorageWoundedAlreadySelected(
            this->m_p_storage->getSelectedID(), this->m_object->ID());

        return true;
    }

    return false;
}

} // namespace Scripts
} // namespace Cordis
