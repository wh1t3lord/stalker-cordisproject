#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_HelicopterFire
{
    friend class Script_HelicopterFireStorage;

public:
    Script_HelicopterFire(void) = delete;
    Script_HelicopterFire(CScriptGameObject* p_client_object)
        : m_p_npc(p_client_object), m_p_enemy(nullptr), m_enemy_id(0), m_enemy_time(0), m_is_show_health(false),
          m_fire_id(0), m_hit_count(0), m_is_flag_by_enemy(false), m_upd_vis(0), m_is_enemy_die(false),
          m_p_progress(nullptr)
    {
    }
    ~Script_HelicopterFire(void) {}

    inline void update_enemy_state(void)
    {
        CHelicopter* p_helicopter = this->m_p_npc->get_helicopter();

        if (!p_helicopter)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        if (this->m_hit_count > 2)
        {
            this->m_hit_count = 0;
            this->m_is_flag_by_enemy = true;
        }

        if (this->m_p_enemy && this->m_is_enemy_die && (this->m_enemy_type_name == "all"))
        {
            this->update_enemy_arr();
        }

        if (this->m_p_enemy && (Globals::get_time_global() - this->m_enemy_time > this->m_upd_vis * 1000))
        {
            if (!p_helicopter->isVisible(this->m_p_enemy))
            {
                if (this->m_enemy_type_name == "all")
                {
                    this->update_enemy_arr();
                }
            }

            this->m_enemy_time = Globals::get_time_global();
        }

        if (this->m_p_enemy)
        {
            if (!p_helicopter->isVisible(this->m_p_enemy))
            {
                this->m_is_flag_by_enemy = true;
            }
        }

        this->set_enemy();
    }

    inline void update_enemy_arr(void)
    {
        CHelicopter* p_helicopter = this->m_p_npc->get_helicopter();

        if (!p_helicopter)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        std::uint32_t index = 0;
        float min_distance2D = 65000.0f;
        while (index < DataBase::Storage::getInstance().getHelicopterCount())
        {
            if (DataBase::Storage::getInstance().getHelicopterEnemies().find(index) !=
                DataBase::Storage::getInstance().getHelicopterEnemies().end())
            {
                if (distance_2d(this->m_p_npc->Position(),
                        DataBase::Storage::getInstance().getHelicopterEnemies().at(index)->Position()) < min_distance2D)
                {
                    this->m_p_enemy = DataBase::Storage::getInstance().getHelicopterEnemies().at(index);
                    min_distance2D = distance_2d(this->m_p_npc->Position(), this->m_p_enemy->Position());
                    this->m_is_flag_by_enemy = true;
                }
            }

            ++index;
        }

        if ((p_helicopter->isVisible(DataBase::Storage::getInstance().getActor()) &&
                Globals::Script_RandomInt::getInstance().Generate(0, 1)) ||
            !DataBase::Storage::getInstance().getHelicopterCount())
        {
            if (distance_2d(this->m_p_npc->Position(), DataBase::Storage::getInstance().getActor()->Position()) <=
                min_distance2D * 2.0f)
            {
                this->m_p_enemy = DataBase::Storage::getInstance().getActor();
                min_distance2D =
                    distance_2d(this->m_p_npc->Position(), DataBase::Storage::getInstance().getActor()->Position());
            }
        }
    }

    inline std::uint32_t getUpdVis(void) const noexcept { return this->m_upd_vis; }
    inline void setUpdVis(const std::uint32_t value) noexcept { this->m_upd_vis = value; }

    inline const Fvector& getFirePoint(void) const noexcept { return this->m_fire_point; }
    inline void setFirePoint(const Fvector& point) noexcept { this->m_fire_point = point; }

    inline bool IsFlagByEnemy(void) const noexcept { return this->m_is_flag_by_enemy; }
    inline void setFlagByEnemy(const bool value) noexcept { this->m_is_flag_by_enemy = value; }

    inline CScriptGameObject* const getEnemy(void) { return this->m_p_enemy; }
    inline void setEnemy(CScriptGameObject* const p_enemy)
    {
        if (!p_enemy)
            Msg("[SCripts/Script_HelicopterFire/setEnemy(p_enemy)] WARNING: p_enemy == nullptr! You set an empty "
                "object");

        this->m_p_enemy = p_enemy;
    }

    inline const xr_string& getEnemyTypeName(void) const noexcept { return this->m_enemy_type_name; }
    inline void setEnemyTypeName(const xr_string& type_name) noexcept
    {
        if (type_name.empty())
            Msg("[Scripts/Script_HelicopterFire/setEnemyTypeName(type_name)] WARNING: type_name.empty() == true! You "
                "set an empty string");

        this->m_enemy_type_name = type_name;
    }

    inline void cs_heli(void)
    {
        CUIGameCustom* const p_hud = CurrentGameUI();
        StaticDrawableWrapper* const p_static = p_hud->GetCustomStatic("cs_heli_health");

        if (!p_static)
        {
            p_hud->AddCustomStatic("cs_heli_health", true);
            CScriptXmlInit xml;
            xml.ParseFile("heli_progress.xml");
            StaticDrawableWrapper* const p_static1 = p_hud->GetCustomStatic("cs_heli_health");
            CUIStatic* const p_window = p_static1->wnd();
            this->m_p_progress = xml.InitProgressBar("heli_health", p_window);
            this->set_cs_helicopter_progress_health();
        }
    }

    inline void cs_remove(void)
    {
        CUIGameCustom* const p_hud = CurrentGameUI();

        if (!p_hud)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        StaticDrawableWrapper* const p_static = p_hud->GetCustomStatic("cs_heli_health");

        if (p_static)
        {
            p_hud->RemoveCustomStatic("cs_heli_health");
        }
    }

    inline void set_cs_helicopter_progress_health(void)
    {
        CHelicopter* const p_helicopter = this->m_p_npc->get_helicopter();

        if (!p_helicopter)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        CUIGameCustom* const p_hud = CurrentGameUI();
        if (!p_hud)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        StaticDrawableWrapper* const p_static = p_hud->GetCustomStatic("cs_heli_health");
        CScriptXmlInit xml;
        xml.ParseFile("heli_progress.xml");

        if (p_static)
        {
            p_hud->AddCustomStatic("cs_heli_health", true);
            StaticDrawableWrapper* const p_static1 = p_hud->GetCustomStatic("cs_heli_health");
            CUIStatic* const p_window = p_static1->wnd();
            float _health = p_helicopter->GetfHealth() * 100.0f;
            if (_health > 0.0f)
            {
                this->m_p_progress->Show(true);
                this->m_p_progress->SetProgressPos(_health);
            }
            else
            {
                this->m_p_progress->Show(false);
                this->m_is_show_health = false;
                p_hud->RemoveCustomStatic("cs_heli_health");
            }
        }
    }

    inline void update_hit(void)
    {
        if (this->m_is_show_health)
        {
            this->set_cs_helicopter_progress_health();
        }
        else
        {
            this->cs_remove();
        }

        if (this->m_p_enemy->ID() == this->m_fire_id)
        {
            if (this->m_enemy_type_name != "nil")
            {
                ++this->m_hit_count;
            }
            else
            {
                this->m_hit_count = 0;
            }
        }
        else
        {
            this->m_fire_id = this->m_p_enemy->ID();
            ++this->m_hit_count;
        }
    }

private:
    inline float distance_2d(const Fvector& a, const Fvector& b) noexcept
    {
        return sqrtf(((b.x - a.x) * (b.x - a.x)) + ((b.z - a.z) * (b.z - a.z)));
    }

    inline void set_enemy(void)
    {
        CHelicopter* const p_helicopter = this->m_p_npc->get_helicopter();

        if (this->m_is_flag_by_enemy)
        {
            p_helicopter->UnSetEnemy();
            this->m_is_enemy_die = false;

            if (this->m_p_enemy)
            {
                if (p_helicopter->isVisible(this->m_p_enemy))
                {
                    p_helicopter->SetEnemy(this->m_p_enemy);
                    this->m_is_flag_by_enemy = false;
                }
            }
            else
            {
                if (!this->m_enemy_type_name.empty())
                {
                    if (this->m_enemy_type_name == "actor")
                    {
                        if (DataBase::Storage::getInstance().getActor())
                        {
                            this->m_p_enemy = DataBase::Storage::getInstance().getActor();
                        }
                    }
                    else
                    {
                        if (this->m_enemy_type_name == "all")
                        {
                            this->update_enemy_arr();
                        }
                        else
                        {
                            if (this->m_enemy_type_name != "nil")
                            {
                                this->m_enemy_id = ai().alife()
                                                       .story_objects()
                                                       .object(atoi(this->m_enemy_type_name.c_str()), true)
                                                       ->ID;
                                this->m_p_enemy = Globals::Game::level::get_object_by_id(this->m_enemy_id);
                            }
                        }

                        if (this->m_p_enemy)
                        {
                            p_helicopter->SetEnemy(this->m_p_enemy);
                        }
                        else
                        {
                            this->m_is_enemy_die = true;
                        }

                        this->m_is_flag_by_enemy = false;
                    }
                }
            }
        }

        if (!this->m_is_enemy_die && this->m_p_enemy->DeathTime() > 0)
        {
            p_helicopter->UnSetEnemy();
            this->m_is_enemy_die = true;
        }

        if (this->m_is_enemy_die &&
            (!fis_zero(this->m_fire_point.x) || !fis_zero(this->m_fire_point.y) || !fis_zero(this->m_fire_point.z)))
        {
            p_helicopter->SetEnemy(&this->m_fire_point);
        }
    }

private:
    bool m_is_show_health;
    bool m_is_flag_by_enemy;
    bool m_is_enemy_die;
    std::uint8_t m_hit_count;
    std::uint16_t m_enemy_id;
    std::uint16_t m_fire_id;
    std::uint32_t m_enemy_time;
    std::uint32_t m_upd_vis;
    CScriptGameObject* m_p_enemy;
    CScriptGameObject* m_p_npc;
    CUIProgressBar* m_p_progress;
    Fvector m_fire_point;
    xr_string m_enemy_type_name; // @ idk what is it
};

class Script_HelicopterFireStorage
{
private:
    Script_HelicopterFireStorage(void) = default;

public:
    static inline Script_HelicopterFireStorage& getInstance(void) noexcept
    {
        static Script_HelicopterFireStorage instance;
        return instance;
    }

    Script_HelicopterFireStorage(const Script_HelicopterFireStorage&) = delete;
    Script_HelicopterFireStorage& operator=(const Script_HelicopterFireStorage&) = delete;
    Script_HelicopterFireStorage(Script_HelicopterFireStorage&&) = delete;
    Script_HelicopterFireStorage& operator=(Script_HelicopterFireStorage&&) = delete;
    ~Script_HelicopterFireStorage(void)
    {
        if (!this->m_fires_storage.empty())
        {
            for (std::pair<const std::uint16_t, Script_HelicopterFire*>& it : this->m_fires_storage)
            {
                if (it.second)
                {
                    R_ASSERT2(false, "bad deallocation! THIS MUST BE DELETED FROM ~DTOR() Script_SchemeHelicopterMove");
                    xr_delete(it.second);
                }
            }

            this->m_fires_storage.clear();
        }
    }

    inline Script_HelicopterFire* AllocateFirer(CScriptGameObject* const p_client_object)
    {
        Script_HelicopterFire* result = nullptr;

        if (!p_client_object)
        {
            R_ASSERT2(false, "object is null!");
            return result;
        }

        std::uint16_t entity_id = p_client_object->ID();

        if (this->m_fires_storage[entity_id] == nullptr)
        {
            result = new Script_HelicopterFire(p_client_object);
            this->m_fires_storage[entity_id] = result;
        }

        return result;
    }

    inline void DeallocateFirer(const std::uint16_t entity_id)
    {
        if (this->m_fires_storage[entity_id])
        {
            Msg("[Scripts/Script_HelicopterFireStorage/DeallocateFirer(entity_id)] deleting %s %d", this->m_fires_storage[entity_id]->m_p_npc->Name(), this->m_fires_storage[entity_id]->m_p_npc->ID());
            xr_delete(this->m_fires_storage[entity_id]);
        }
    }

private:
    xr_map<std::uint16_t, Script_HelicopterFire*> m_fires_storage;
};

} // namespace Scripts
} // namespace Cordis
