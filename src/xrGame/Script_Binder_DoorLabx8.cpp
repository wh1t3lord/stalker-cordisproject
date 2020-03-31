#include "stdafx.h"
#include "Script_Binder_DoorLabx8.h"
#include "PhysicObject.h"

constexpr const char* _kAnimatedObjectName = "animated_object";

namespace Cordis
{
namespace Scripts
{
Script_Binder_DoorLabx8::Script_Binder_DoorLabx8(CScriptGameObject* object)
    : inherited(object), m_is_idle(true), m_is_loaded(false), m_is_play_forward(false), m_p_sound_stop(nullptr),
      m_p_sound_start(nullptr), m_p_sound_idle(nullptr), m_animation_time(0.0f)
{
    CScriptIniFile* p_ini = this->m_object->spawn_ini();
    bool is_deallocate_ini = false;

    if (!p_ini)
    {
        R_ASSERT2(false, "something wrong");
        return;
    }

    if (!p_ini->section_exist(_kAnimatedObjectName))
    {
        Msg("[Scripts/Script_Binder_DoorLabx8/ctor()] WARNING: can't find section_name \"animatied_object\"! Return "
            "...");
        return;
    }

    xr_string filename = Globals::Utils::cfg_get_string(p_ini, _kAnimatedObjectName, "cfg");
    if (!filename.empty())
    {
        p_ini = new CScriptIniFile(filename.c_str());
        is_deallocate_ini = true;
    }

    xr_string sound_idle_name = Globals::Utils::cfg_get_string(p_ini, _kAnimatedObjectName, "idle_snd");
    if (sound_idle_name.empty())
        sound_idle_name = "device\\airtight_door_idle";

    xr_string sound_start_name = Globals::Utils::cfg_get_string(p_ini, _kAnimatedObjectName, "start_snd");
    if (sound_start_name.empty())
        sound_start_name = "device\\airtight_door_start";

    xr_string sound_stop_name = Globals::Utils::cfg_get_string(p_ini, _kAnimatedObjectName, "stop_snd");
    if (sound_stop_name.empty())
        sound_stop_name = "device\\airtight_door_stop";

    if (sound_idle_name.empty() == false) // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
        this->m_p_sound_idle = new CScriptSound(sound_idle_name.c_str());

    if (sound_start_name.empty() == false) // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
        this->m_p_sound_start = new CScriptSound(sound_start_name.c_str());

    if (sound_stop_name.empty() == false) // LorD: проверить будет ли дропать nil, если будет то найти и исправить когда это будет, чтобы все "nil" просто проверялись всегда как .empty()
        this->m_p_sound_stop = new CScriptSound(sound_stop_name.c_str());

    this->m_tip_condlist = XR_LOGIC::parse_condlist_by_script_object(
        "Script_Binder_DoorLabx8", "tip_condlist", Globals::Utils::cfg_get_string(p_ini, _kAnimatedObjectName, "tip"));

    xr_string on_use_name = "true";
    xr_string on_start_name = "true";
    xr_string on_stop_name = "true";

    if (p_ini->line_exist(_kAnimatedObjectName, "on_use"))
        on_use_name = p_ini->r_string(_kAnimatedObjectName, "on_use");

    this->m_on_use_condlist =
        XR_LOGIC::parse_condlist_by_script_object("Script_Binder_DoorLabx8", "on_use", on_use_name);

    if (p_ini->line_exist(_kAnimatedObjectName, "on_start"))
        on_start_name = p_ini->r_string(_kAnimatedObjectName, "on_start");

    this->m_on_start_condlist =
        XR_LOGIC::parse_condlist_by_script_object("Script_Binder_DoorLabx8", "on_start", on_start_name);

    if (p_ini->line_exist(_kAnimatedObjectName, "on_stop"))
        on_stop_name = p_ini->r_string(_kAnimatedObjectName, "on_stop");

    this->m_on_stop_condlist =
        XR_LOGIC::parse_condlist_by_script_object("Script_Binder_DoorLabx8", "on_stop", on_stop_name);

    this->m_idle_delay = Globals::Utils::cfg_get_number(p_ini, _kAnimatedObjectName, "idle_delay");

    if (!this->m_idle_delay)
        this->m_idle_delay = 2000.0f;

    this->m_start_delay = Globals::Utils::cfg_get_number(p_ini, _kAnimatedObjectName, "start_delay");
}

Script_Binder_DoorLabx8::~Script_Binder_DoorLabx8(void)
{
    if (this->m_p_sound_idle)
    {
        Msg("[Scripts/Script_Binder_DoorLabx8/~dtor()] deleting m_p_sound_idle");
        xr_delete(this->m_p_sound_idle);
    }

    if (this->m_p_sound_start)
    {
        Msg("[Scripts/Script_Binder_DoorLabx8/~dtor()] deleting m_p_sound_start");
        xr_delete(this->m_p_sound_start);
    }

    if (this->m_p_sound_stop)
    {
        Msg("[Scripts/Script_Binder_DoorLabx8/~dtor()] deleting m_p_sound_end");
        xr_delete(this->m_p_sound_stop);
    }
}

bool Script_Binder_DoorLabx8::net_Spawn(SpawnType DC)
{
    if (!inherited::net_Spawn(DC))
    {
        return false;
    }

    DataBase::Storage::getInstance().addAnimationObject(this->m_object, this);
    this->m_object->get_physics_object()->stop_anim();
    this->m_object->get_physics_object()->anim_time_set(0.0f);
    this->setCanUseAnimationCallback(true);
    return true;
}

void Script_Binder_DoorLabx8::net_Destroy()
{
    if (this->m_p_sound_idle)
    {
        this->m_p_sound_idle->Stop();
    }

    if (this->m_p_sound_start)
    {
        this->m_p_sound_start->Stop();
    }

    if (this->m_p_sound_stop)
    {
        this->m_p_sound_stop->Stop();
    }

    this->setCanUseAnimationCallback(false);
    DataBase::Storage::getInstance().deleteAnimationObject(this->m_object);
    inherited::net_Destroy();
}

void Script_Binder_DoorLabx8::shedule_Update(u32 time_delta)
{
    inherited::shedule_Update(time_delta);

    if (!fis_zero(this->m_animation_time) && this->m_is_loaded)
    {
        this->m_object->get_physics_object()->anim_time_set(this->m_animation_time);
        this->m_animation_time = 0;
    }

    if (!this->m_is_idle)
    {
        if (this->m_is_play_forward)
            this->m_object->get_physics_object()->run_anim_forward();
        else
            this->m_object->get_physics_object()->run_anim_back();
    }
    else
    {
        this->m_object->get_physics_object()->stop_anim();
        if (this->m_animation_time)
            this->m_object->get_physics_object()->anim_time_set(this->m_animation_time);

        if (this->m_p_sound_idle)
            this->m_p_sound_idle->Stop();
    }

    xr_string tip_string = XR_LOGIC::pick_section_from_condlist(
        Globals::get_story_object("actor"), static_cast<CScriptGameObject*>(nullptr), this->m_tip_condlist);

    if (tip_string != "none")
    {
        this->m_object->SetTipText(tip_string.c_str());
    }
    else
    {
        this->m_object->SetTipText("");
    }
}

void Script_Binder_DoorLabx8::save(NET_Packet* output_packet)
{
    Globals::set_save_marker(*output_packet, "save", false, "Script_Binder_DoorLabx8");
    inherited::save(output_packet);

    XR_LOGIC::save_object(this->m_object, *output_packet);
    output_packet->w_u8(this->m_is_idle);
    output_packet->w_u8(this->m_is_play_forward);
    output_packet->w_float(this->m_animation_time);
    Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, true, "Script_Binder_DoorLabx8");
}

void Script_Binder_DoorLabx8::load(IReader* input_packet)
{
    Globals::set_save_marker(*input_packet, Globals::kSaveMarkerMode_Load, false, "Script_Binder_DoorLabx8");
    inherited::load(input_packet);
    XR_LOGIC::load_object(this->m_object, *input_packet);
    this->m_is_idle = input_packet->r_u8();
    this->m_is_play_forward = input_packet->r_u8();
    this->m_animation_time = input_packet->r_float();
    this->m_is_loaded = true;
    Globals::set_save_marker(*input_packet, Globals::kSaveMarkerMode_Load, true, "Script_Binder_DoorLabx8");
}

bool Script_Binder_DoorLabx8::net_SaveRelevant() { return true; }

void Script_Binder_DoorLabx8::use_callback(CScriptGameObject* p_client_object, CScriptGameObject* p_client_who)
{
    XR_LOGIC::pick_section_from_condlist(Globals::get_story_object("actor"), p_client_object, this->m_on_use_condlist);
}

void Script_Binder_DoorLabx8::animation_callback(const bool is_end)
{
    if (!this->isCanUseAnimationCallback())
        return;

    if (is_end)
    {
        if (this->m_p_sound_stop)
        {
            this->m_p_sound_stop->PlayAtPos(this->m_object, this->m_object->Position(), 0, 0);
        }

        this->m_is_idle = true;
        this->m_animation_time = this->m_object->get_physics_object()->anim_time_get();
        XR_LOGIC::pick_section_from_condlist(
            Globals::get_story_object("actor"), static_cast<CScriptGameObject*>(nullptr), this->m_on_stop_condlist);
    }
}

void Script_Binder_DoorLabx8::animation_forward(void)
{
    if (this->m_p_sound_idle)
        this->m_p_sound_idle->Stop();

    this->m_object->get_physics_object()->stop_anim();

    if (this->m_p_sound_start)
        this->m_p_sound_start->PlayAtPos(
            this->m_object, this->m_object->Position(), this->m_start_delay / 1000.0f, 0);

    if (this->m_p_sound_idle)
        this->m_p_sound_idle->PlayAtPos(
            this->m_object, this->m_object->Position(), (this->m_start_delay + this->m_idle_delay) / 1000.0f, sm_Looped);

    this->m_is_idle = false;
    this->m_is_play_forward = false;

    XR_LOGIC::pick_section_from_condlist(Globals::get_story_object("actor"), static_cast<CScriptGameObject*>(nullptr), this->m_on_start_condlist);
}

void Script_Binder_DoorLabx8::animation_backward(void) 
{ 
    if (this->m_p_sound_idle)
        this->m_p_sound_idle->Stop();

    this->m_object->get_physics_object()->stop_anim();

    if (this->m_p_sound_start)
        this->m_p_sound_start->PlayAtPos(this->m_object, this->m_object->Position(), this->m_start_delay / 1000.0f, 0);

    if (this->m_p_sound_idle)
        this->m_p_sound_idle->PlayAtPos(this->m_object, this->m_object->Position(),
            (this->m_start_delay + this->m_idle_delay) / 1000.0f, sm_Looped);

    this->m_is_idle = false;
    this->m_is_play_forward = false;

    XR_LOGIC::pick_section_from_condlist(
        Globals::get_story_object("actor"), static_cast<CScriptGameObject*>(nullptr), this->m_on_start_condlist);
}

void Script_Binder_DoorLabx8::animation_stop(void) 
{
    this->m_object->get_physics_object()->stop_anim();
    this->m_is_idle = true;

    if (this->m_p_sound_stop)
        this->m_p_sound_stop->PlayAtPos(this->m_object, this->m_object->Position(), 0.0f, 0);

    this->m_animation_time = this->m_object->get_physics_object()->anim_time_get();
    XR_LOGIC::pick_section_from_condlist(
        Globals::get_story_object("actor"), static_cast<CScriptGameObject*>(nullptr), this->m_on_stop_condlist);
}

} // namespace Scripts
} // namespace Cordis
