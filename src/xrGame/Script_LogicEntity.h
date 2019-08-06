#pragma once

namespace Cordis
{
namespace Scripts
{
constexpr const char* LOGIC_ENTITY_EMPTY = "Unknown";

class Script_ILogicEntity
{
public:
    virtual ~Script_ILogicEntity(void) = default;

    virtual void deactivate(CScriptGameObject* npc)
    {
        if (!npc)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        Msg("[Scripts/Script_ILogicEntity/deactivate(npc)] The method is not overridden!");
    }

    virtual void reset_scheme(void) { Msg("[Scripts/Script_ILogicEntity/reset_scheme] The method is overridden!"); }

    virtual void save(void) { Msg("[Scripts/Script_ILogicEntity/save] The method is not overridden!"); }

    virtual void load(void) { Msg("[Scripts/Script_ILogicEntity/load] The method is not overridden!"); }

    virtual void extrapolate_callback(void)
    {
        Msg("[Scripts/Script_ILogicEntity/extrapolate_callback] The method is not overridden!");
    }

    virtual void activate_scheme(void)
    {
        Msg("[Scripts/Script_ILogicEntity/activate_scheme] The method is not overridden!");
    }

    virtual void cutscene_callback(void)
    {
        Msg("[Scripts/Script_ILogicEntity/cutscene_callback] The method is not overridden!");
    }

    virtual void update(const float& delta)
    {
        Msg("[Scripts/Script_ILogicEntity/update(delta)] The method is not overridden!");
    }

    virtual void hit_callback(
        CScriptGameObject* object, int amount, const Fvector& local_direction, CScriptGameObject* who, int bone_index)
    {
        if (!object || !who)
        {
            R_ASSERT2(false, "was null!");
            return;
        }

        Msg("[Scripts/Script_ILogicEntity/hit_callback(object, amount, local_direction, who, bone_index)] The method "
            "is not overridden!");
    }

    virtual void death_callback(CScriptGameObject* victim, CScriptGameObject* who)
    {
        if (!victim || !who)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        Msg("[Scripts/Script_ILogicEntity/death_callback(victim, who)] The method is not overridden!");
    }

    virtual void net_destroy(void) { Msg("[Scripts/Script_ILogicEntity/net_destroy()] The method is not overridden!"); }
    virtual void waitpoint_callback(CScriptGameObject* object, const int& action_type, const int& index)
    {
        if (!object)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        Msg("[Scripts/Script_ILogicEntity/waypoint_callback(object, action_type, index)] The method is not overridden!");
    }

    virtual void use_callback(CScriptGameObject* victim, CScriptGameObject* who)
    {
        if (!victim || !who)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        Msg("[Scripts/Script_ILogicEntity/use_callback(victim, who)] The method is not overridden!");
    }

    // @ Системное наименование
    static xr_string m_logic_name;
};

} // namespace Scripts
} // namespace Cordis
