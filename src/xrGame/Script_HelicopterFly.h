#pragma once

class CHelicopter;

namespace Cordis
{
namespace Scripts
{
class Script_HelicopterFly
{
    friend class Script_HelicopterFlyStorage;

public:
    Script_HelicopterFly(void) = delete;
    Script_HelicopterFly(CScriptGameObject* p_client_object)
        : m_npc(p_client_object), m_is_block_flook(false), m_distance_by_look(0.0f), m_helicopter_acc_fw(6.0f),
          m_helicopter_acc_bw(4.0f), m_max_velocity(0.0f)
    {
    }
    ~Script_HelicopterFly(void) { this->m_npc = nullptr; }

    inline bool fly_on_point_with_vector(const Fvector& destination_point, const Fvector& destionation_direction,
        const float _destination_velocity, const bool flag_to_waypoint_callback, const bool flag_by_null_velocity)
    {
        CHelicopter* const p_helicopter = this->m_npc->get_helicopter();

        if (!p_helicopter)
        {
            R_ASSERT2(false, "object is null!");
            return false;
        }

        bool result = false;
        const Fvector& helicopter_current_position = p_helicopter->Position();
        const Fvector& helicopter_current_direction = p_helicopter->Direction();
        float helicopter_current_velocity = p_helicopter->GetCurrVelocity();
        float destination_velocity = (_destination_velocity)*1000.0f / 3600.0f;

        if (!flag_to_waypoint_callback)
        {
            float a_speed = 0.0f;
            std::uint32_t time_by_fly = 0;
            Fvector rez_point;
            float d_path = 0.0f;

            if (destination_velocity >= helicopter_current_velocity)
            {
                a_speed = this->m_helicopter_acc_fw;
                d_path = helicopter_current_velocity * 2.0f / a_speed;
            }
            else
            {
                a_speed = -this->m_helicopter_acc_bw;
                d_path = (-helicopter_current_velocity * 2.0f) / a_speed;
            }

            time_by_fly = (destination_velocity - helicopter_current_velocity) / a_speed;
            float delta = (helicopter_current_velocity * time_by_fly) + (a_speed * time_by_fly * time_by_fly / 2.0f);

            if (delta >= d_path)
            {
                this->m_helicopter_current_direction = helicopter_current_direction;
                this->m_destination_point = destination_point;
                this->m_helicopter_current_position = helicopter_current_position;

                rez_point = this->calculate_point();
                if (!this->m_is_block_flook)
                {
                    rez_point.x = rez_point.x + helicopter_current_direction.x * delta / 2.0f;
                    rez_point.z = rez_point.z + helicopter_current_direction.z * delta / 2.0f;
                }
                result = true;
            }
            else
            {
                rez_point = destination_point;
                result = false;
            }

            this->m_destination_point_ = rez_point;
        }
        else
        {
            this->m_destination_point_ = destination_point;
            result = false;
        }

        p_helicopter->SetDestPosition(&this->m_destination_point_);
        this->correct_velocity();

        if (flag_by_null_velocity)
        {
            p_helicopter->SetSpeedInDestPoint(0.0f);
        }
        else
        {
            p_helicopter->SetSpeedInDestPoint(p_helicopter->GetMaxVelocity());
        }

        return result;
    }

    inline void look_at_position(void)
    {
        if (this->m_is_block_flook)
        {
            CHelicopter* p_helicopter = this->m_npc->get_helicopter();
            p_helicopter->LookAtPoint(this->m_point_by_look, true);
        }
    }

    inline void set_block_flook(const bool state) noexcept { this->m_is_block_flook = state; }
    inline void set_look_point(const Fvector& point) noexcept { this->m_point_by_look = point; }
    inline bool IsBlockFlook(void) const noexcept { return this->m_is_block_flook; }
    inline void setMaxVelocity(const float value) noexcept { this->m_max_velocity = value; }
    inline void setAccFW(const float value) noexcept { this->m_helicopter_acc_fw = value; }
    inline float getAccFW(void) const noexcept { return this->m_helicopter_acc_fw; }
    inline void setAccBW(const float value) noexcept { this->m_helicopter_acc_bw = value; }
    inline float getAccBW(void) const noexcept { return this->m_helicopter_acc_bw; }

    inline const Fvector& getDestinationPoint(void) const noexcept { return this->m_destination_point; }

private:
    inline Fvector calculate_point(void)
    {
        Fvector rez_point;
        float arrX[3];
        float arrY[3];
        float arrZ[3];

        arrX[0] = this->m_helicopter_current_position.x;
        arrX[1] = this->m_destination_point.x;
        arrX[2] = this->m_helicopter_current_direction.x;

        arrY[0] = this->m_helicopter_current_position.y;
        arrY[1] = this->m_destination_point.y;
        arrY[2] = this->m_helicopter_current_direction.y;

        arrZ[0] = this->m_helicopter_current_position.z;
        arrZ[1] = this->m_destination_point.z;
        arrZ[2] = this->m_helicopter_current_direction.z;

        rez_point.y = (this->m_helicopter_current_position.y + this->m_destination_point.y) / 2.0f;

        if (rez_point.y == this->m_helicopter_current_position.y)
        {
            rez_point.z = (this->m_helicopter_current_position.z + this->m_destination_point.z) / 2.0f;
            if (rez_point.z == this->m_helicopter_current_position.z)
            {
                rez_point.x = (this->m_helicopter_current_position.x + this->m_destination_point.x) / 2.0f;
                rez_point.z = this->lagrange(rez_point.x, arrX, arrZ);
            }
            else
            {
                rez_point.x = this->lagrange(rez_point.z, arrZ, arrX);
            }
        }
        else
        {
            rez_point.x = this->lagrange(rez_point.y, arrY, arrX);
            rez_point.z = this->lagrange(rez_point.y, arrY, arrZ);
        }

        return rez_point;
    }

    inline float lagrange(const float x, const float* const arrX, const float* const arrY)
    {
        float result = 0.0f;
        float m = 0.0f;
        for (std::uint32_t i = 0; i < 2; ++i)
        {
            m = arrY[i];
            for (std::uint32_t j = 0; j < 2; ++i)
            {
                if (j != i)
                {
                    m = m * (x - arrX[j]) / (arrX[i] - arrX[j]);
                }
            }
            result = result + m;
        }

        return result;
    }

    inline void correct_velocity(void)
    {
        CHelicopter* p_helicopter = this->m_npc->get_helicopter();

        if (!p_helicopter)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        float helicopter_current_velocity = p_helicopter->GetCurrVelocity();
        float distance_to_destination_point = p_helicopter->GetDistanceToDestPosition();
        float a_speed = this->m_helicopter_acc_fw;
        float destination_velocity = sqrtf(((2.0f * a_speed * distance_to_destination_point +
                                                (helicopter_current_velocity * helicopter_current_velocity)) /
            3.0f));

        if ((this->m_max_velocity * 1000.0f / 3600.0f) < destination_velocity)
        {
            destination_velocity = this->m_max_velocity * 1000.0f / 3600.0f;
        }

        p_helicopter->SetMaxVelocity(destination_velocity);
    }

private:
    bool m_is_block_flook;
    float m_distance_by_look;
    float m_max_velocity;
    float m_helicopter_acc_fw;
    float m_helicopter_acc_bw;
    CScriptGameObject* m_npc;
    Fvector m_point_by_look;
    Fvector m_helicopter_current_position; // @ point_arr
    Fvector m_helicopter_current_direction; // @ point_arr
    Fvector m_destination_point; // @ point_arr
    Fvector m_destination_point_;
};

class Script_HelicopterFlyStorage
{
private:
    Script_HelicopterFlyStorage(void) = default;

public:
    static inline Script_HelicopterFlyStorage& getInstance(void) noexcept
    {
        static Script_HelicopterFlyStorage instance;
        return instance;
    }

    Script_HelicopterFlyStorage(const Script_HelicopterFlyStorage&) = delete;
    Script_HelicopterFlyStorage& operator=(const Script_HelicopterFlyStorage&) = delete;
    Script_HelicopterFlyStorage(Script_HelicopterFlyStorage&&) = delete;
    Script_HelicopterFlyStorage& operator=(Script_HelicopterFlyStorage&&) = delete;
    ~Script_HelicopterFlyStorage(void)
    {
        if (!this->m_fliers_storage.empty())
        {
            for (std::pair<const std::uint16_t, Script_HelicopterFly*>& it : this->m_fliers_storage)
            {
                if (it.second)
                {
                    R_ASSERT2(false, "bad deallocation. MUST IN ~DTOR() OF SCRIPT_SCHEMEHELICOPTERMOVE!");
                    xr_delete(it.second);
                }
            }

            this->m_fliers_storage.clear();
        }
    }

    inline Script_HelicopterFly* AllocateFlyer(CScriptGameObject* const p_client_object)
    {
        Script_HelicopterFly* result = nullptr;

        if (!p_client_object)
        {
            R_ASSERT2(false, "object is null!");
            return result;
        }

        std::uint16_t entity_id = p_client_object->ID();

        if (this->m_fliers_storage[entity_id] == nullptr)
        {
            result = new Script_HelicopterFly(p_client_object);
            this->m_fliers_storage[entity_id] = result;
        }

        return result;
    }

    inline void DeallocateFlyer(const std::uint16_t entity_id)
    {
        if (this->m_fliers_storage[entity_id])
        {
            Msg("[Scripts/Script_HelicopterFlyStorage/DeallocateFlyer(entity_id)] deleting %s %d",
                this->m_fliers_storage[entity_id]->m_npc->Name(), this->m_fliers_storage[entity_id]->m_npc->ID());
            xr_delete(this->m_fliers_storage[entity_id]);
        }
    }

private:
    xr_map<std::uint16_t, Script_HelicopterFly*> m_fliers_storage;
};

} // namespace Scripts
} // namespace Cordis
