#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_EvaluatorAbuse : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorAbuse(const xr_string& evaluator_name, DataBase::Storage_Scheme& storage)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(&storage)
    {
    }

    virtual _value_type evaluate(void);

private:
    DataBase::Storage_Scheme* m_p_storage;
};

// @ Можете доделать!
class Script_SchemeXRAbuse : public Script_ISchemeStalker
{
    using inherited_scheme = Script_ISchemeStalker;

public:
    Script_SchemeXRAbuse(const xr_string& action_name, DataBase::Storage_Scheme& storage)
        : inherited_scheme(nullptr, action_name, storage)
    {
    }

    virtual void execute(void);
    virtual void finalize(void);
    virtual void initialize(void);


    // @ PRIVATE uses, in XR_LOGIC
    static inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, DataBase::Storage_Scheme& storage)
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

        Msg("[Scripts/add_to_binder(p_client_object, p_ini, scheme_name, section_name, storage)] added "
            "Script_SchemeMobWalker scheme to binder, name=%s scheme=%s section=%s",
            p_client_object->Name(), scheme_name.c_str(), section_name.c_str());
    }

    static inline void set_abuse(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name)
    {
    }

    static inline void add_abuse(CScriptGameObject* const p_client_object, const float value)
    {
        if (!p_client_object)
        {
            Msg("[Scripts/Script_SchemeXRAbuse/add_abuse(p_client_object, value)] WARNING: can't add because "
                "p_client_object == nullptr!");
            return;
        }

        if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().find("abuse") !=
            DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().end())
        {
            DataBase::Storage::getInstance()
                .getStorage()
                .at(p_client_object->ID())
                .getSchemes()
                .at("abuse")
                .getXRAbuseManager()
                ->AddAbuse(value);
        }
        else
        {
            Msg("[Scripts/Script_SchemeXRAbuse/add_abuse(p_client_object, value)] wARNING: can't add because abuse "
                "scheme doesnt exist!");
            return;
        }
    }

    static inline void clear_abuse(CScriptGameObject* const p_client_object)
    {
        if (!p_client_object)
        {
            Msg("[Scripts/Script_SchemeXRAbuse/clear_abuse(p_client_object)] WARNING: can't add because "
                "p_client_object == nullptr!");
            return;
        }

        if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().find("abuse") !=
            DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().end())
        {
            DataBase::Storage::getInstance()
                .getStorage()
                .at(p_client_object->ID())
                .getSchemes()
                .at("abuse")
                .getXRAbuseManager()
                ->ClearAbuse();
        }
        else
        {
            Msg("[Scripts/Script_SchemeXRAbuse/clear_abuse(p_client_object)] wARNING: can't add because abuse "
                "scheme doesnt exist!");
            return;
        }
    }

    static inline void disable_abuse(CScriptGameObject* const p_client_object)
    {
        if (!p_client_object)
        {
            Msg("[Scripts/Script_SchemeXRAbuse/disable_abuse(p_client_object)] WARNING: can't add because "
                "p_client_object == nullptr!");
            return;
        }

        if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().find("abuse") !=
            DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().end())
        {
            DataBase::Storage::getInstance()
                .getStorage()
                .at(p_client_object->ID())
                .getSchemes()
                .at("abuse")
                .getXRAbuseManager()
                ->DisableAbuse();
        }
        else
        {
            Msg("[Scripts/Script_SchemeXRAbuse/disable_abuse(p_client_object)] wARNING: can't add because abuse "
                "scheme doesnt exist!");
            return;
        }
    }

    static inline void enable_abuse(CScriptGameObject* const p_client_object)
    {
        if (!p_client_object)
        {
            Msg("[Scripts/Script_SchemeXRAbuse/enable_abuse(p_client_object)] WARNING: can't add because "
                "p_client_object == nullptr!");
            return;
        }

        if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().find("abuse") !=
            DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().end())
        {
            DataBase::Storage::getInstance()
                .getStorage()
                .at(p_client_object->ID())
                .getSchemes()
                .at("abuse")
                .getXRAbuseManager()
                ->EnableAbuse();
        }
        else
        {
            Msg("[Scripts/Script_SchemeXRAbuse/enable_abuse(p_client_object)] wARNING: can't add because abuse "
                "scheme doesnt exist!");
            return;
        }
    }
};

class Script_XRAbuseManager
{
public:
    Script_XRAbuseManager(void) = delete;
    Script_XRAbuseManager(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
        : m_p_npc(p_client_object), m_p_storage(&storage), m_is_enable(true), m_is_hit_done(false), m_abuse_rate(2.0f),
          m_abuse_threshold(5.0f), m_abuse_value(0.0f), m_last_update(0.0f)
    {
    }
    ~Script_XRAbuseManager(void) {}

    inline void setAbuseRate(const float value) noexcept { this->m_abuse_rate = value; }
    inline bool IsAbused(void) const noexcept { return (this->m_abuse_value >= this->m_abuse_threshold); }
    inline bool update(void) noexcept
    {
        if (!this->m_last_update)
            this->m_last_update = static_cast<float>(Globals::get_time_global());

        if (this->m_abuse_value)
            this->m_abuse_value =
                this->m_abuse_value - (static_cast<float>(Globals::get_time_global()) - this->m_last_update) * 0.00005f;
        else
            this->m_abuse_value = 0.0f;

        if (this->m_abuse_value > this->m_abuse_threshold * 1.1f)
            this->m_abuse_value = this->m_abuse_threshold * 1.1f;

        if (this->m_is_hit_done && (this->m_abuse_value < this->m_abuse_threshold * 2.0f / 3.0f))
            this->m_is_hit_done = false;

        this->m_last_update = static_cast<float>(Globals::get_time_global());

        if (this->IsAbused())
            return true;

        return false;
    }

    inline void AddAbuse(const float value) noexcept
    {
        if (this->m_is_enable)
            this->m_abuse_value = this->m_abuse_value + (value * this->m_abuse_rate);
    }

    inline void ClearAbuse(void) noexcept { this->m_abuse_value = 0.0f; }
    inline void EnableAbuse(void) noexcept { this->m_is_enable = true; }
    inline void DisableAbuse(void) noexcept { this->m_is_enable = false; }

private:
    bool m_is_enable;
    bool m_is_hit_done;
    float m_last_update;
    float m_abuse_value;
    float m_abuse_threshold;
    float m_abuse_rate;
    CScriptGameObject* m_p_npc;
    DataBase::Storage_Scheme* m_p_storage;
};
} // namespace Scripts
} // namespace Cordis
