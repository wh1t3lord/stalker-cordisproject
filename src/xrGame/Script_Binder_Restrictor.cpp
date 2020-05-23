#include "stdafx.h"
#include "Script_Binder_Restrictor.h"

Cordis::Scripts::Script_Binder_Restrictor::Script_Binder_Restrictor(CScriptGameObject* const p_client_object) : CScriptBinderObject(p_client_object), m_is_loaded(false), m_is_initialized(false)
{
}

Cordis::Scripts::Script_Binder_Restrictor::~Script_Binder_Restrictor(void)
{
}

void Cordis::Scripts::Script_Binder_Restrictor::reinit()
{
	CScriptBinderObject::reinit();
	
	if (DataBase::Storage::getInstance().getStorage().find(this->m_object->ID()) == DataBase::Storage::getInstance().getStorage().end())
	{
		R_ASSERT(false);
		return;
	}
}

bool Cordis::Scripts::Script_Binder_Restrictor::net_Spawn(SpawnType DC)
{
	if (!CScriptBinderObject::net_Spawn(DC))
		return false;

	DataBase::Storage::getInstance().addObject(this->m_object);
	DataBase::Storage::getInstance().addZone(this->m_object);

	std::uint16_t id = this->m_object->ID();
	if (XR_SOUND::getLoopedSoundDatabase().find(id) != XR_SOUND::getLoopedSoundDatabase().end())
	{
		for (const std::pair<xr_string, Script_ISoundEntity*>& it : XR_SOUND::getLoopedSoundDatabase().at(id))
		{
			XR_SOUND::play_sound_looped(id, it.first);
		}
	}

	return true;
}

void Cordis::Scripts::Script_Binder_Restrictor::net_Destroy()
{
	XR_SOUND::stop_sounds_by_id(this->m_object->ID());

	if (DataBase::Storage::getInstance().getStorage().find(this->m_object->ID()) != DataBase::Storage::getInstance().getStorage().end())
	{
		const xr_string& active_scheme_name = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSchemeName();
		if (active_scheme_name.empty() == false)
		{
			DataBase::Storage_Scheme* const p_storage = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().at(active_scheme_name);
			for (Script_ISchemeEntity* const it : p_storage->getActions())
			{
				if (it)
					it->net_destroy(nullptr); // Lord: протестить наверное передаётся nullptr
			}
		}
	}


	DataBase::Storage::getInstance().deleteZone(this->m_object);
	DataBase::Storage::getInstance().deleteObject(this->m_object);

	CScriptBinderObject::net_Destroy();
}

void Cordis::Scripts::Script_Binder_Restrictor::shedule_Update(u32 time_delta)
{
	if (!this->m_is_initialized && DataBase::Storage::getInstance().getActor())
	{
		this->m_is_initialized = true;
		XR_LOGIC::intialize_job(this->m_object, DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()), this->m_is_loaded, DataBase::Storage::getInstance().getActor(), Globals::kSTypeRestrictor);
	}

	this->m_object->info_clear();
	const xr_string& active_section_name = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSectionName();
	if (active_section_name.empty() == false)
	{
		this->m_object->info_add(xr_string("section: ").append(active_section_name).c_str());
	}

	this->m_object->info_add(xr_string("name: [").append(this->m_object->Name()).append("] id [").append(std::to_string(this->m_object->ID()).c_str()).append("]").c_str());

	if (active_section_name.empty() == false)
	{
		const xr_string& active_scheme_name = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getActiveSchemeName();
		DataBase::Storage_Scheme* const p_storage = DataBase::Storage::getInstance().getStorage().at(this->m_object->ID()).getSchemes().at(active_scheme_name);

		for (Script_ISchemeEntity* const it : p_storage->getActions())
		{
			it->update(static_cast<float>(time_delta));
		}
	}

	XR_SOUND::update(this->m_object->ID());
}

void Cordis::Scripts::Script_Binder_Restrictor::save(NET_Packet* output_packet)
{
	Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, false, "restrictor_binder");
	CScriptBinderObject::save(output_packet);
	XR_LOGIC::save_object(this->m_object, *output_packet);
	Globals::set_save_marker(*output_packet, Globals::kSaveMarkerMode_Save, true, "restrictor_binder");
}

void Cordis::Scripts::Script_Binder_Restrictor::load(IReader* input_packet)
{
	Globals::set_save_marker(*input_packet, Globals::kSaveMarkerMode_Load, false, "restrictor_binder");
	this->m_is_loaded = true;
	CScriptBinderObject::load(input_packet);
	XR_LOGIC::load_object(this->m_object, *input_packet);
	Globals::set_save_marker(*input_packet, Globals::kSaveMarkerMode_Load, true, "restrictor_binder");
}

bool Cordis::Scripts::Script_Binder_Restrictor::net_SaveRelevant()
{
	return true;
}
