#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SchemeMobWalker : public Script_ISchemeMonster
{
    using inherited_scheme = Script_ISchemeMonster;

public:
    Script_SchemeMobWalker(void) = delete;
    Script_SchemeMobWalker(CScriptGameObject* p_client_object, DataBase::Storage_Scheme& storage);
    ~Script_SchemeMobWalker(void);

    virtual void reset_scheme(void);
    virtual void update(const float delta);

    virtual inline bool is_arrived_to_first_waypoint(void)
    {
        // Lord: доделать!
        return false;
    }

private:
    CPatrolPathParams m_path_walk;
    CPatrolPathParams m_path_look;
    CondlistWaypoints m_path_walk_info;
    CondlistWaypoints m_path_look_info;
};
} // namespace Scripts
} // namespace Cordis
