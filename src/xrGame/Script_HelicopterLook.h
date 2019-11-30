#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_HelicopterLook
{
    friend class Script_HelicopterLookerStorage;

public:
    Script_HelicopterLook(void) = delete;
    Script_HelicopterLook(CScriptGameObject* p_client_object) : m_npc(p_client_object), m_is_look_state(false) {}

    ~Script_HelicopterLook(void) { this->m_npc = nullptr; }

    inline void calculate_look_point(const Fvector& destination_point, const bool is_look_state) noexcept
    {
        this->m_is_look_state = is_look_state;

        if (is_look_state)
        {
            CHelicopter* p_helicopter = this->m_npc->get_helicopter();

            if (!p_helicopter)
            {
                R_ASSERT2(false, "object is null!");
                return;
            }

            float distance_to_destination_point = p_helicopter->GetDistanceToDestPosition();
            Fvector new_direction;
            const Fvector& helicopter_current_position = p_helicopter->Position();
            const Fvector& helicopter_current_direction = p_helicopter->Direction();
            float helicopter_velocity = p_helicopter->GetSpeedInDestPoint(0.0f);
            float helicopter_current_velocity = p_helicopter->GetCurrVelocity();

            new_direction.x = (destination_point.x - helicopter_current_position.x) / distance_to_destination_point;
            new_direction.y = (destination_point.y - helicopter_current_position.y) / distance_to_destination_point;
            new_direction.z = (destination_point.z - helicopter_current_position.z) / distance_to_destination_point;

            float delta = 0.0f;

            if (helicopter_velocity < 0.0f || fis_zero(helicopter_velocity))
            {
                delta = 0.0f;
            }
            else
            {
                delta = helicopter_current_velocity / helicopter_velocity;
                if (delta > 2.0f)
                {
                    delta = 2.0f;
                }
            }
            
            this->m_look_point.x = (helicopter_velocity * helicopter_velocity) *
                (helicopter_current_direction.x + new_direction.x / 2.0f * (2.0f - delta));
            this->m_look_point.y = (helicopter_velocity * helicopter_velocity) *
                (helicopter_current_direction.y + new_direction.y / 2.0f * (2.0f - delta));
            this->m_look_point.z = (helicopter_velocity * helicopter_velocity) *
                (helicopter_current_direction.z + new_direction.z / 2.0f * (2.0f - delta));

            p_helicopter->LookAtPoint(this->m_look_point, is_look_state);
        }
    }

    inline bool getLookState(void) const noexcept { return this->m_is_look_state; }

private:
    bool m_is_look_state;
    CScriptGameObject* m_npc;
    Fvector m_look_point;
};

class Script_HelicopterLookStorage
{
private:
    Script_HelicopterLookStorage(void) = default;

public:
    static inline Script_HelicopterLookStorage& getInstance(void) noexcept
    {
        static Script_HelicopterLookStorage instance;
        return instance;
    }

    Script_HelicopterLookStorage(const Script_HelicopterLookStorage&) = delete;
    Script_HelicopterLookStorage& operator(const Script_HelicopterLookStorage&) = delete;
    Script_HelicopterLookStorage(Script_HelicopterLookStorage&&) = delete;
    Script_HelicopterLookStorage& operator=(Script_HelicopterLookStorage&&) = delete;
    ~Script_HelicopterLookStorage(void)
    {
        if (!this->m_helicopter_lookers_storage.empty())
        {
            for (std::pair<const std::uint16_t, Script_HelicopterLook*>& it : this->m_helicopter_lookers_storage)
            {
                // @ Just for checking
                if (it.second)
                {
                    R_ASSERT2(
                        false, "bad deallocation. YOU MUST DELETE IT IN ~DTOR of Script_SchemeHelicopterMove!!!");
                    xr_delete(it.second);
                }
            }

            this->m_helicopter_lookers_storage.clear();
        }
    }

    inline Script_HelicopterLook* AllocateLooker(CScriptGameObject* p_client_object)
    {
        Script_HelicopterLook* result = nullptr;

        if (!p_client_object)
        {
            R_ASSERT2(false, "object is null!");
            return result;
        }

        std::uint16_t entity_id = p_client_object->ID();
        if (this->m_helicopter_lookers_storage[entity_id] == nullptr)
        {
            result = new Script_HelicopterLook(p_client_object);
            this->m_helicopter_lookers_storage[entity_id] = result;
        }

        return result;
    }

    // @ PRIVATE, calls in ~dtor() Script_SchemeHelicopterMove
    inline void DeallocateLooker(const std::uint16_t npc_id)
    {
        if (this->m_helicopter_lookers_storage[npc_id])
        {
            Msg("[Scripts/Script_HelicopterLookerStorage/DeallocateLooker(npc_id)] deleting %s %d",
                this->m_helicopter_lookers_storage[npc_id]->m_npc->Name(),
                this->m_helicopter_lookers_storage[npc_id]->m_npc->ID());
            xr_delete(this->m_helicopter_lookers_storage[npc_id]);
        }
    }

private:
    xr_map<std::uint16_t, Script_HelicopterLook*> m_helicopter_lookers_storage;
};

} // namespace Scripts
} // namespace Cordis
