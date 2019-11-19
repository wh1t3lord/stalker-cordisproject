#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SimulationObjects
{
private:
    Script_SimulationObjects(void) : m_props_ini("misc\\simulation_objects_props.ltx") {}

public:
    inline static Script_SimulationObjects& getInstance(void) noexcept
    {
        static Script_SimulationObjects instance;
        return instance;
    }

    ~Script_SimulationObjects(void);
    Script_SimulationObjects(const Script_SimulationObjects&) = delete;
    Script_SimulationObjects& operator=(const Script_SimulationObjects&) = delete;
    Script_SimulationObjects(Script_SimulationObjects&&) = delete;
    Script_SimulationObjects& operator=(Script_SimulationObjects&&) = delete;

#pragma region Getters
    inline xr_map<std::uint16_t, CSE_ALifeDynamicObject*>& getObjects(void) { return this->m_objects; }
#pragma endregion

    float evaluate_priority(CSE_ALifeDynamicObject* target, CSE_ALifeDynamicObject* squad);

    void registrate(CSE_ALifeDynamicObject* object);
    void unregistrate(CSE_ALifeDynamicObject* object);
    void update_avaliability(CSE_ALifeDynamicObject* object);
    inline void Deallocate(void)
    {
        if (!this->m_objects.empty())
        {
            for (std::pair<const std::uint16_t, CSE_ALifeDynamicObject*>& it : this->m_objects)
            {
                it.second = nullptr;
            }

            this->m_objects.clear();
        }
    }

private:
    inline float evaluate_priority_by_distance(CSE_ALifeDynamicObject* target, CSE_ALifeDynamicObject* squad)
    {
        float distance = Globals::sim_dist_to(target, squad);

        if (distance < 1.0f)
            distance = 1.0f;

        return (1.0f + 1.0f / distance);
    }

    void get_properties(CSE_ALifeDynamicObject* object);

private:
    xr_map<std::uint16_t, CSE_ALifeDynamicObject*> m_objects;
    CScriptIniFile m_props_ini;
};
} // namespace Scripts
} // namespace Cordis
