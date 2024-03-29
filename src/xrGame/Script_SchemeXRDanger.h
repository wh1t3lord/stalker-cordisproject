#pragma once

namespace Cordis
{
namespace Scripts
{
namespace XR_DANGER
{
inline xr_string get_danger_name(const CDangerObject* const p_best_danger)
{
    xr_string result;
    if (!p_best_danger)
    {
        R_ASSERT2(false, "object is null!");
        return result;
    }

    const CEntityAlive* p_client_danger_object = p_best_danger->object();
    CDangerObject::EDangerType best_danger_type = p_best_danger->type();
    const IGameObject* p_client_object = nullptr;
    if (best_danger_type != CDangerObject::eDangerTypeGrenade && p_best_danger->dependent_object())
    {
        p_client_object = p_best_danger->dependent_object();
    }

    if (!p_client_danger_object && p_client_object)
        return p_client_object->Name();
    else if (!p_client_object && p_client_danger_object)
        return p_client_danger_object->Name();
    else
        return "none";
}

inline bool is_danger(CScriptGameObject* const p_client_object)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "object is null!");
        return false;
    }

    const CDangerObject* p_client_best_danger = p_client_object->GetBestDanger();

    if (!p_client_best_danger)
    {
        return false;
    }

    const CEntityAlive* p_client_best_danger_object = p_client_best_danger->object();
    CDangerObject::EDangerType best_danger_type = p_client_best_danger->type();
    const IGameObject* p_client_entity = nullptr;
    if (best_danger_type != CDangerObject::eDangerTypeGrenade && p_client_best_danger->dependent_object())
    {
        p_client_entity = p_client_best_danger->dependent_object();
    }

    if (!p_client_entity && !p_client_best_danger_object)
        return false;

    if ((best_danger_type != CDangerObject::eDangerTypeFreshEntityCorpse) &&
        (best_danger_type != CDangerObject::eDangerTypeGrenade) &&
        ((p_client_object->GetRelationType(p_client_best_danger_object ?
                 p_client_best_danger_object->lua_game_object() :
                 p_client_entity->lua_game_object())) != ALife::eRelationTypeEnemy))
    {
        return false;
    }

    if (best_danger_type == CDangerObject::eDangerTypeGrenade)
    {
        if (Globals::character_community(p_client_object) == "zombied")
        {
            return false;
        }
    }

    if (best_danger_type == CDangerObject::eDangerTypeFreshEntityCorpse)
        return false;

    // Lord: доделать когда будеь xr_combat_ignore

    float danger_distance_sqrt = p_client_best_danger->position().distance_to_sqr(p_client_object->Position());
    float ignored_distance_by_type =
        Script_GlobalHelper::getInstance().getXRDangerIgnoreDistanceByDangerType(best_danger_type);

    if (!fis_zero(ignored_distance_by_type))
    {
        if (danger_distance_sqrt >= ignored_distance_by_type * ignored_distance_by_type)
        {
            return false;
        }
    }
    else if (danger_distance_sqrt >= XR_DANGER::kDangerIgnoreDistance * XR_DANGER::kDangerIgnoreDistance)
    {
        return false;
    }

    // Lord: доделать когда будет Crd_Wounded is_heavy_wounded_by_id

    return true;
}
inline std::uint32_t get_danger_time(const CDangerObject* p_client_danger_object)
{
    if (!p_client_danger_object)
    {
        R_ASSERT2(false, "object is null!");
        return Globals::kUnsignedInt32Undefined;
    }

    if (p_client_danger_object->type() == CDangerObject::eDangerTypeFreshEntityCorpse)
    {
        const CEntityAlive* p_client_entity = p_client_danger_object->object();
        if (!p_client_entity)
        {
            R_ASSERT2(false, "object is null!");
            return Globals::kUnsignedInt32Undefined;
        }

        return p_client_entity->GetGameDeathTime();
    }

    return p_client_danger_object->time();
}
} // namespace XR_DANGER

class Script_EvaluatorDanger : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorDanger(const xr_string& evaluator_name, DataBase::Script_ComponentScheme_XRDanger* storage)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(storage), m_p_manager(nullptr)
    {
    }

    ~Script_EvaluatorDanger(void) {}

    virtual _value_type evaluate(void);

    // @ PRIVATE uses, in XR_LOGIC
    static inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, DataBase::Script_IComponentScheme* storage)
    {
        if (!p_client_object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        if (!p_ini)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        MESSAGEI("added scheme to binder, name=%s scheme=%s section=%s",
            p_client_object->Name(), scheme_name.c_str(), section_name.c_str());

        CScriptActionPlanner* p_planner = Globals::get_script_action_planner(p_client_object);

        if (!p_planner)
        {
            R_ASSERT2(false, "object is null");
            return;
        }

        CScriptActionPlanner* p_danger_action_planner =
            Globals::cast_planner(&p_planner->action(StalkerDecisionSpace::eWorldOperatorDangerPlanner));

        if (!p_danger_action_planner)
        {
            R_ASSERT2(false, "bad cast!");
            return;
        }

        p_planner->remove_evaluator(StalkerDecisionSpace::eWorldPropertyDanger);
        p_planner->add_evaluator(StalkerDecisionSpace::eWorldPropertyDanger, new Script_EvaluatorDanger("danger", static_cast<DataBase::Script_ComponentScheme_XRDanger*>(storage)));

        p_danger_action_planner->remove_evaluator(StalkerDecisionSpace::eWorldPropertyDanger);
        p_danger_action_planner->add_evaluator(StalkerDecisionSpace::eWorldPropertyDanger, new Script_EvaluatorDanger("danger", static_cast<DataBase::Script_ComponentScheme_XRDanger*>(storage)));
    }

    static inline void set_danger(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
    {
        if (!p_client_object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        DataBase::Script_ComponentScheme_XRDanger* p_storage = XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_XRDanger>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

        DataBase::Storage::getInstance().setStorageDangerFlag(p_client_object->ID(), false);
    }

    // Lord: наверное не стоит реализовать данную функцию
    /*
        static inline void reset_danger(
            CScriptGameObject* const p_client_object, const xr_string& scheme_name, const xr_string& section_name)
        {

        }*/

private:
    CScriptActionPlanner* m_p_manager;
    DataBase::Script_ComponentScheme_XRDanger* m_p_storage;
};

} // namespace Scripts
} // namespace Cordis
