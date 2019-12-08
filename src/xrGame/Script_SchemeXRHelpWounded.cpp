#include "stdafx.h"
#include "Script_SchemeXRHelpWounded.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeXRHelpWounded::Script_SchemeXRHelpWounded(const xr_string& action_name, DataBase::Storage_Scheme& storage)
    : inherited_scheme(nullptr, action_name, storage)
{
    this->m_scheme_name = "help_wounded";
}

Script_SchemeXRHelpWounded::~Script_SchemeXRHelpWounded(void) {}

void Script_SchemeXRHelpWounded::initialize(void)
{
    CScriptActionBase::initialize();
    this->m_object->set_desired_position();
    this->m_object->set_desired_direction();
    this->m_object->set_dest_level_vertex_id(this->m_p_storage->getLevelVertexID());

    // Lord: реализовать когда будет state_manager
}

void Script_SchemeXRHelpWounded::execute(void)
{
    CScriptActionBase::execute();

    if (this->m_object->Position().distance_to_sqr(this->m_p_storage->getVertexPosition()) > 2.0f)
    {
        return;
    }

    // Lord: реализовать когда будет state_manager
}

void Script_SchemeXRHelpWounded::finalize(void) { CScriptActionBase::finalize(); }
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
