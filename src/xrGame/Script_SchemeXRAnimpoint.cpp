#include "stdafx.h"
#include "Script_SchemeXRAnimpoint.h"

Fvector angle_to_direction(const Fvector& oangle)
{
	return Fvector().setHP(oangle.y, oangle.x).normalize();
}

namespace Cordis
{
	namespace Scripts
	{
		Script_SchemeXRAnimpoint::Script_SchemeXRAnimpoint(const xr_string& name, DataBase::Storage_Scheme& storage) : Script_ISchemeStalker(nullptr, name, storage)
		{
		}

		Script_SchemeXRAnimpoint::~Script_SchemeXRAnimpoint(void)
		{
		}

		void Script_SchemeXRAnimpoint::initialize(void)
		{
		}

		void Script_SchemeXRAnimpoint::execute(void)
		{
		}

		void Script_SchemeXRAnimpoint::finalize(void)
		{
		}

		Script_ActionReachAnimpoint::Script_ActionReachAnimpoint(const xr_string& name, DataBase::Storage_Scheme& storage) : Script_ISchemeStalker(nullptr, name, storage)
		{
		}

		Script_ActionReachAnimpoint::~Script_ActionReachAnimpoint(void)
		{
		}

		void Script_ActionReachAnimpoint::initialize(void)
		{
		}

		void Script_ActionReachAnimpoint::execute(void)
		{
		}

		void Script_ActionReachAnimpoint::finalize(void)
		{
		}

		Script_EvaluatorReachAnimpoint::_value_type Script_EvaluatorReachAnimpoint::evaluate(void)
		{
			return _value_type();
		}

		Script_EvaluatorNeedAnimpoint::_value_type Script_EvaluatorNeedAnimpoint::evaluate(void)
		{
			return _value_type();
		}

		Script_Animpoint::Script_Animpoint(const std::uint16_t npc_id, DataBase::Storage_Scheme& storage) : m_p_storage(&storage), m_npc_id(npc_id), m_is_started(false), m_position_vertex(0), m_p_camp(nullptr)
		{
		}

		Script_Animpoint::~Script_Animpoint(void)
		{
		}

		void Script_Animpoint::initialize(void)
		{
			MESSAGE("IF THIS MESSAGE SHOWN THAT THIS FUNCTIONS IS WORKING, JUST DELETE THIS!");
		}

		void Script_Animpoint::activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object)
		{
			this->m_p_storage->ClearSignals();

			this->calculate_position();

			if (this->m_is_started)
			{
				if (!this->m_p_storage->isXRAnimpointUseCamp() && this->m_p_storage->getXRAnimpointCoverName() == this->m_cover_name)
				{
					this->fill_approved_actions();

					xr_string target_action_name;
					if (this->m_p_storage->getApprovedActions().empty() == false)
					{
						target_action_name = this->m_p_storage->getApprovedActions().at(Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(0, this->m_p_storage->getApprovedActions().size() - 1)).second;
					}
					else
					{
						MESSAGEER("Something is not right here!");
					}

					xr_string current_storage_animstate = Script_GlobalHelper::getInstance().getStateLibrary().at(target_action_name).getAnimStateTypeName();
					xr_string target_storage_animstate = Script_GlobalHelper::getInstance().getStateLibrary().at(this->m_action_name).getAnimStateTypeName();

					if (current_storage_animstate == target_storage_animstate)
					{
						if (target_storage_animstate != this->m_action_name)
						{
							this->m_action_name = this->m_p_storage->getApprovedActions().at(Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(0, this->m_p_storage->getApprovedActions().size() - 1)).second;
						}

						return;
					}

				}

				this->stop();
			}
		}

		void Script_Animpoint::calculate_position(void)
		{
			Script_SE_SmartCover* p_cover = nullptr;
			if (DataBase::Storage::getInstance().getGameRegisteredServerSmartCovers().find(this->m_p_storage->getXRAnimpointCoverName()) == DataBase::Storage::getInstance().getGameRegisteredServerSmartCovers().end())
			{
				p_cover = DataBase::Storage::getInstance().getGameRegisteredServerSmartCovers().at(this->m_p_storage->getXRAnimpointCoverName());
			}
			else
			{
				MESSAGEER("Can't find smart cover's data %s", this->m_p_storage->getXRAnimpointCoverName().c_str());
				return;
			}

			this->m_position = p_cover->position();
			this->m_position_vertex = Globals::Game::level::vertex_id(this->m_position);
			this->m_vertex_position = Globals::Game::level::vertex_position(this->m_position_vertex);

			this->m_smart_direction = angle_to_direction(p_cover->angle());

			Fvector look_dir = this->m_smart_direction.normalize();
			this->m_look_position = Fvector().set(this->m_position.x + 10.0f * look_dir.x, this->m_position.y, this->m_position.z + 10.0f * look_dir.z);

			xr_string description_name = p_cover->description();

			if (Script_GlobalHelper::getInstance().getAnimpointTable().find(description_name) == Script_GlobalHelper::getInstance().getAnimpointTable().end())
			{
				if (this->m_p_storage->getXRAnimpointAvailAnimations().empty())
				{
					MESSAGEER("Wrong animpoint smart_cover description %s | %s", description_name.c_str(), p_cover->name_replace());
					return;
				}
			}

			this->m_p_storage->setXRAnimpointDescriptionName(description_name);
			this->m_avail_actions = Script_GlobalHelper::getInstance().getAnimpointTable().at(description_name);
			this->m_p_storage->ClearApprovedActions(); // Lord: проверить если нужно вообще вызывать этот метод, если нет то удалить его из класса!
		}

		void Script_Animpoint::get_animation_params(Fvector& position, Fvector& smart_direction)
		{
			position = this->m_position;
			smart_direction = this->m_smart_direction;
		}

		bool Script_Animpoint::position_riched(void)
		{
			if (this->m_action_name.empty() == false)
				return true;

			if (Globals::is_vector_nil(this->m_position))
				return false;

			CScriptGameObject* p_object = nullptr;
			if (DataBase::Storage::getInstance().getStorage().find(this->m_npc_id) == DataBase::Storage::getInstance().getStorage().end())
			{
				MESSAGEWR("can't find npc's storage by id %d", this->m_npc_id);
				return;
			}
			else
			{
				p_object = DataBase::Storage::getInstance().getStorage().at(this->m_npc_id).getClientObject();
			}

			if (p_object == nullptr)
				return false;

			bool is_distance_reached = p_object->Position().distance_to_sqr(this->m_vertex_position) <= this->m_p_storage->getXRAnimpointReachDistance();

			float v1 = -deg(atan2(this->m_smart_direction.x, this->m_smart_direction.z));
			float v2 = -deg(atan2(p_object->Direction().x, p_object->Direction().z));

			float rot_y = min(abs(v1 - v2), 360.0f - abs(v1) - abs(v2));

			bool is_direction_reached = rot_y < 50.0f;

			return (is_direction_reached) && (is_direction_reached);
		}

		void Script_Animpoint::fill_approved_actions(void)
		{
			if (this->m_p_storage->getXRAnimpointAvailAnimations().empty() == false)
			{
				for (const xr_string& it : this->m_p_storage->getXRAnimpointAvailAnimations())
				{
					this->m_p_storage->setApprovedActions({ [](std::uint16_t, bool) -> bool { return true; }, it });
				}
			}
			else
			{
				if (this->m_avail_actions.empty() == false)
				{
					for (const std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>& it : this->m_avail_actions)
					{
						if (it.first(this->m_npc_id, !!(this->m_p_camp)))
						{
							this->m_p_storage->setApprovedActions(it);
						}
					}
				}
			}

			if (this->m_p_storage->getApprovedActions().empty())
			{
				MESSAGEE("can't be something is wrong! Check your code above ...");
			}
		}

		void Script_Animpoint::start(void)
		{
			if (this->m_p_storage->isXRAnimpointUseCamp())
				this->m_p_camp = Globals::get_current_camp(this->m_position);

			this->fill_approved_actions();

			if (this->m_p_camp)
			{
				this->m_p_camp->register_npc(this->m_npc_id);
			}
			else
			{
				this->m_action_name = this->m_p_storage->getApprovedActions().at(Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(0, this->m_p_storage->getApprovedActions().size() - 1)).second;
			}

			this->m_is_started = true;
			this->m_cover_name = this->m_p_storage->getXRAnimpointCoverName();
		}

		void Script_Animpoint::stop(void)
		{
			if (this->m_p_camp)
				this->m_p_camp->unregister_npc(this->m_npc_id);

			this->m_is_started = false;
			this->m_action_name.clear();
		}

		void Script_Animpoint::update(void)
		{
			const xr_string& description_name = this->m_p_storage->getXRAnimpointDescriptionName();
			xr_vector<xr_string> temp_actions;
			if (!this->m_p_storage->isXRAnimpointUseCamp())
			{
				if (this->m_p_storage->getXRAnimpointAvailAnimations().empty())
				{
					if (this->m_p_storage->getApprovedActions().empty())
					{
						MESSAGEER("animpoint not in camp and approved_actions is null. %s", this->m_p_storage->getXRAnimpointCoverName().c_str());
						return;
					}

					for (const std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>& it : this->m_p_storage->getApprovedActions())
					{
						temp_actions.push_back(it.second);
					}
				}
				else
				{
					for (const xr_string& it : this->m_p_storage->getXRAnimpointAvailAnimations())
					{
						temp_actions.push_back(it);
					}
				}

				this->m_action_name = temp_actions.at(Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(0, temp_actions.size() - 1));
				return;
			}

			if (!this->m_npc_id)
			{
				MESSAGEER("Trying to use destroyed object!");
				return;
			}


		}
	}
}