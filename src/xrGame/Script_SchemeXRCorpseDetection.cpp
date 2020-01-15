#include "stdafx.h"
#include "Script_SchemeXRCorpseDetection.h"

namespace Cordis
{
namespace Scripts
{
Script_EvaluatorCorpse::_value_type Script_EvaluatorCorpse::evaluate(void)
{
    // Lord: доделать когда будет release_body_manager

    return false;
}

Script_SchemeXRCorpseDetection::~Script_SchemeXRCorpseDetection(void) {}

void Script_SchemeXRCorpseDetection::initialize(void)
{
    CScriptActionBase::initialize();
    this->m_object->set_desired_position();
    this->m_object->set_desired_direction();
    this->m_object->set_dest_level_vertex_id(this->m_p_storage->getLevelVertexID());
    Globals::set_state(this->m_object, "patrol", StateManagerCallbackData(), 0,
        std::pair<Fvector, CScriptGameObject* const>(Fvector().set(0.0f,0.0f,0.0f), nullptr), StateManagerExtraData());
}

void Script_SchemeXRCorpseDetection::execute(void)
{
    CScriptActionBase::execute();

    if (this->m_object->Position().distance_to_sqr(this->m_p_storage->getVertexPosition()) > 2.0f)
    {
        return;
    }
    std::pair<Fvector, CScriptGameObject* const> target(this->m_p_storage->getVertexPosition(), nullptr);
    Globals::set_state(this->m_object, "search_corpse", StateManagerCallbackData(), 0, target, StateManagerExtraData());
    xr_string faction_name;
    XR_SOUND::set_sound_play(this->m_object->ID(), "corpse_loot_begin", faction_name, 0);
}

void Script_SchemeXRCorpseDetection::finalize(void)
{
    if (this->m_p_storage->getXRCorpseDetectionSelectedCorpseID() &&
        (DataBase::Storage::getInstance().getStorage().find(
             this->m_p_storage->getXRCorpseDetectionSelectedCorpseID()) !=
            DataBase::Storage::getInstance().getStorage().end()))
    {
        DataBase::Storage::getInstance().setStorageCorpseAlreadySelected(
            this->m_p_storage->getXRCorpseDetectionSelectedCorpseID(), 0);
    }

    CScriptActionBase::finalize();
}

} // namespace Scripts
} // namespace Cordis
