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
    virtual void reset_scheme(void) { Msg("[Scripts/Script_ILogicEntity/reset_scheme] The method not overridden!"); }

    virtual void update(const float& delta)
    {
        Msg("[Scripts/Script_ILogicEntity/update(delta)] The method not overridden!");
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
            "not overridden!");
    }

    virtual void death_callback(CScriptGameObject* victim, CScriptGameObject* who)
    {
        if (!victim || !who)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        Msg("[Scripts/Script_ILogicEntity/death_callback(victim, who)] The method not overridden!");
    }

    virtual void net_destroy(void) { Msg("[Scripts/Script_ILogicEntity/net_destroy()] The method not overridden!"); }
    // @ Системное наименование
    static std::string m_logic_name;
};

std::string Script_ILogicEntity::m_logic_name = LOGIC_ENTITY_EMPTY;

} // namespace Scripts
} // namespace Cordis
