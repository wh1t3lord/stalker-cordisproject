#include "stdafx.h"
#include "Script_Binder_Artefact.h"
#include "physics_element_scripted.h"
#include "physics_joint_scripted.h"

namespace Cordis
{
	namespace Scripts
	{
		Script_Binder_Artefact::Script_Binder_Artefact(CScriptGameObject* const p_client_object) : inherited(p_client_object), m_is_first_call(false)
		{
		}

		Script_Binder_Artefact::~Script_Binder_Artefact(void)
		{
		}
		
		bool Script_Binder_Artefact::net_Spawn(SpawnType DC)
		{
			if (inherited::net_Spawn(DC) == false)
				return false;
			std::uint16_t id = this->m_object->ID();
			DataBase::Storage::getInstance().addObject(this->m_object);
			CArtefact* const p_artefact = this->m_object->get_artefact();

			if (DataBase::Storage::getInstance().getArtefactWaysByID().find(id) == DataBase::Storage::getInstance().getArtefactWaysByID().end())
			{
				MESSAGEWR("Can't find way by id %d", id);
				return true;
			}

			const xr_string& way_name = DataBase::Storage::getInstance().getArtefactWaysByID().at(id);

			if (way_name.empty())
			{
				MESSAGEWR("way_name is empty! Check your database for ways!");
				return true;
			}

			if (DataBase::Storage::getInstance().getParentZonesArtefactByID().find(id) == DataBase::Storage::getInstance().getParentZonesArtefactByID().end())
			{
				MESSAGEWR("can't find parent zone by id %d", id);
				return true;
			}


			Script_Binder_Anomaly* const p_zone = DataBase::Storage::getInstance().getParentZonesArtefactByID().at(id);

			if (p_zone == nullptr)
			{
				MESSAGEWR("invalid parent zone!");
				return true;
			}

			if (DataBase::Storage::getInstance().getArtefactPointsByID().find(id) == DataBase::Storage::getInstance().getArtefactPointsByID().end())
			{
				MESSAGEWR("can't find point by id %d", id);
				return true;
			}

			std::uint32_t point_index = DataBase::Storage::getInstance().getArtefactPointsByID().at(id);

			p_artefact->FollowByPath(way_name.c_str(), point_index, Fvector().set(p_zone->getApplyingForceXZ(), p_zone->getApplyingForceY(), p_zone->getApplyingForceXZ()));
			this->m_is_first_call = true;

			return true;
		}

		void Script_Binder_Artefact::net_Destroy()
		{
			DataBase::Storage::getInstance().deleteObject(this->m_object);
			inherited::net_Destroy();
		}

		void Script_Binder_Artefact::shedule_Update(u32 time_delta)
		{
			inherited::shedule_Update(time_delta);
			MESSAGE("artefact [%s] position %f %f %f", this->m_object->Name(), this->m_object->Position().x, this->m_object->Position().y, this->m_object->Position().z);

			if (this->m_is_first_call)
			{
				CScriptIniFile* const p_ini = this->m_object->spawn_ini();

				if (p_ini && (p_ini->section_exist("fixed_bone")))
				{
					xr_string bone_name = p_ini->r_string("fixed_bone", "name");
					cphysics_shell_scripted* const p_shell = this->m_object->get_physics_shell();

					if (p_shell == nullptr)
						return;

					cphysics_element_scripted* const p_element = p_shell->get_Element(bone_name.c_str());

					if (p_element->isFixed() == false)
						p_element->Fix();

					this->m_is_first_call = false;
				}
				else
				{
					this->m_is_first_call = false;
				}


			}
		}
	}
}