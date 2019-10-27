#include "stdafx.h"
#include "Script_Animpoint.h"

#include "Script_GlobalHelper.h"

namespace Cordis
{
namespace Scripts
{
Script_Animpoint::Script_Animpoint(CScriptGameObject* npc, DataBase::StorageAnimpoint_Data& storage)
    : m_npc_id(npc->ID()), m_storage(&storage)
{
}

Script_Animpoint::~Script_Animpoint(void)
{
    Msg("[Scripts/Script_Animpoint/~dtor] Deleting: [%s]", this->m_cover_name.c_str());
}

bool Script_Animpoint::is_position_riched(void)
{
    if (this->m_current_action_name.size())
        return true;

    if (fis_zero(this->m_position.x) && fis_zero(this->m_position.y) && fis_zero(this->m_position.z))
        return false;

    CScriptGameObject* client_npc = DataBase::Storage::getInstance().getStorage().at(this->m_npc_id).getClientObject();

    if (!client_npc)
        return false;

    bool is_distance_reached =
        client_npc->Position().distance_to_sqr(this->m_position_vertex) <= this->m_storage->getReachedDistance();

    float v1 = -deg(atan2f(this->m_smart_direction.x, this->m_smart_direction.z));
    float v2 = -deg(atan2f(client_npc->Direction().x, client_npc->Direction().z));

    float rot_y = std::min<float>(abs(v1 - v2), (360.0f - abs(v1) - abs(v2)));

    bool is_direction_reached = rot_y < 50.0f;

    return is_distance_reached && is_direction_reached;
}

void Script_Animpoint::start(void)
{
    // Lord: реализовать когда будет сделан sr_camp.get_current_camp
//     if (this->m_storage->IsUseCamp())
//         this->m_is_camp = 
    
}

void Script_Animpoint::stop(void)
{
    // Lord: реализовать когда будет сделан camp
}

void Script_Animpoint::update(void) 
{
   
}

void Script_Animpoint::activate_scheme(void)
{
    this->calculate_position();

    if (this->m_is_started)
    {
        if (!this->m_storage->IsUseCamp() && this->m_cover_name == this->m_storage->getCoverName())
        {
            this->fill_approved_actions();

            const xr_string& target_action_name = this->m_storage->getApprovedActions()
                                                      .at(Globals::Script_RandomInt::getInstance().Generate(
                                                          0, this->m_storage->getApprovedActions().size()))
                                                      .second;

            const xr_string& current_state_animstate_name =
                Script_GlobalHelper::getInstance().getStateLibrary().at(target_action_name).getAnimStateTypeName();
            const xr_string& target_state_animstate_name = Script_GlobalHelper::getInstance()
                                                               .getStateLibrary()
                                                               .at(this->m_current_action_name)
                                                               .getAnimStateTypeName();

            if (current_state_animstate_name == target_state_animstate_name)
            {
                Msg("[Script_Animpoint/activate_scheme()] No animpoint stop");

                if (target_action_name != this->m_current_action_name)
                {
                    this->m_current_action_name = this->m_storage->getApprovedActions()
                                                      .at(Globals::Script_RandomInt::getInstance().Generate(
                                                          0, this->m_storage->getApprovedActions().size()))
                                                      .second;
                    return;
                }
            }

            this->stop();
        }
    }
}

void Script_Animpoint::fill_approved_actions(void)
{
    bool is_in_camp = this->m_is_camp;

    if (this->m_storage->getAvailAnimations().size())
    {
        for (const xr_string& it : this->m_storage->getAvailAnimations())
        {
            this->m_storage->setApprovedAction(Globals::predicate_const_true, it);
        }
    }
    else
    {
        if (this->m_avail_actions.size())
        {
            for (std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>& it : this->m_avail_actions)
            {
                if (it.first(this->m_npc_id, is_in_camp))
                {
                    this->m_storage->setApprovedAction(
                        nullptr, it.second); // Lord: проследить где вызывается метод если он в итоге вызывается, а то
                                             // может вылететь из-за nullptr, хотя так и должны устаналивать!
                }
            }
        }
    }

    if (!this->m_storage->getApprovedActions().size())
    {
        Msg("[Script_Animpoint/fill_approved_actions()] ERROR: There is no approved actions for stalker[%s] in "
            "animpoint[%s]",
            DataBase::Storage::getInstance().getStorage().at(this->m_npc_id).getClientObject()->Name());
        R_ASSERT(false);
    }
}

void Script_Animpoint::calculate_position(void)
{
    Script_SE_SmartCover* server_smartcover =
        Script_GlobalHelper::getInstance().getGameRegisteredServerSmartCovers().at(this->m_cover_name);

    if (!server_smartcover)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    this->m_position = server_smartcover->Position();
    this->m_vertex_id = Globals::Game::level::vertex_id(this->m_position);
    this->m_position_vertex = Globals::Game::level::vertex_position(this->m_vertex_id);
    // TODO: Lord проверить правильно работает yaw и otich ибо в скрипте вообще Fvector().yaw?????????
    float yaw = server_smartcover->angle().y;
    float pitch = server_smartcover->angle().x;

    this->m_smart_direction = Fvector().setHP(yaw, pitch).normalize();

    Fvector look_direction = this->m_smart_direction.normalize();
    this->m_look_position = Fvector().set(this->m_position.x + (10.0f * look_direction.x), this->m_position.y,
        this->m_position.z + (10 * look_direction.z));

    xr_string description_name = server_smartcover->description();

    if (!Script_GlobalHelper::getInstance().getAnimpointTable().at(description_name).size())
    {
        if (!this->m_storage->getAvailAnimations().size())
        {
            Msg("Wrong animpoint smart_cover description %s, name %s", description_name.c_str(),
                server_smartcover->name_replace());
            R_ASSERT(false);
        }
    }

    this->m_storage->setDescriptionName(description_name);
    this->m_avail_actions = Script_GlobalHelper::getInstance().getAnimpointTable().at(description_name);
}

} // namespace Scripts
} // namespace Cordis
