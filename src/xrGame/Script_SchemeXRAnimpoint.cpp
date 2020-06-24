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
		Script_SchemeXRAnimpoint::Script_SchemeXRAnimpoint(const xr_string& name, DataBase::Script_ComponentScheme_XRAnimPoint* storage) : Script_ISchemeStalker(nullptr, name, storage), m_p_storage(storage)
		{
		}

		Script_SchemeXRAnimpoint::~Script_SchemeXRAnimpoint(void)
		{
		}

		void Script_SchemeXRAnimpoint::initialize(void)
		{
            CScriptActionBase::initialize();
            this->m_p_storage->getAnimpoint()->start();
		}

		void Script_SchemeXRAnimpoint::execute(void)
		{ 
            CScriptActionBase::execute();

            Fvector position, direction;
            this->m_p_storage->getAnimpoint()->get_animation_params(position, direction);

            if (!this->m_p_storage->getAnimpoint()->isStarted())
            {
                this->m_p_storage->getAnimpoint()->start();
            }

            Globals::set_state(this->m_object, this->m_p_storage->getAnimpoint()->getActionName(),
                StateManagerCallbackData(), 0,
                std::pair<Fvector, CScriptGameObject* const>(
                    this->m_p_storage->getAnimpoint()->getLookPosition(), nullptr),
                StateManagerExtraData());
		}

		void Script_SchemeXRAnimpoint::finalize(void)
		{
            this->m_p_storage->getAnimpoint()->stop();
            CScriptActionBase::finalize();
		}

		void Script_SchemeXRAnimpoint::net_destroy(CScriptGameObject* const p_client_object)
		{
			this->m_p_storage->getAnimpoint()->stop();
		}

		void Script_SchemeXRAnimpoint::add_to_binder(CScriptGameObject* const p_client_object, 
			CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, DataBase::Script_IComponentScheme* storage)
		{
			if (p_client_object == nullptr)
			{
				MESSAGEWR("Invalid npc!");
				return;
			}

			if (p_ini == nullptr)
			{
				MESSAGEWR("invalid ini!");
				return;
			}

			xr_map<xr_string, std::uint32_t> operators;
			xr_map<xr_string, std::uint32_t> properties;

			CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(p_client_object);

			properties["need_animpoint"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kAnimpointProperty + 1;
			properties["reach_animpoint"] = Globals::XR_ACTIONS_ID::kAnimationPointAction + 2;
			properties["state_mgr_logic_active"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStateManager + 4;

			operators["action_animpoint"] = Globals::XR_ACTIONS_ID::kAnimationPointAction + 1;
			operators["action_reach_animpoint"] = Globals::XR_ACTIONS_ID::kAnimationPointAction + 2;

			p_planner->add_evaluator(properties.at("need_animpoint"), new Script_EvaluatorNeedAnimpoint("animpoint_need", static_cast<DataBase::Script_ComponentScheme_XRAnimPoint*>(storage)));
			p_planner->add_evaluator(properties.at("reach_animpoint"), new Script_EvaluatorReachAnimpoint("animpoint_reach", static_cast<DataBase::Script_ComponentScheme_XRAnimPoint*>(storage)));


			static_cast<DataBase::Script_ComponentScheme_XRAnimPoint*>(storage)->setAnimpoint(new Script_Animpoint(p_client_object->ID(), static_cast<DataBase::Script_ComponentScheme_XRAnimPoint*>(storage)));
			DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, static_cast<DataBase::Script_ComponentScheme_XRAnimPoint*>(storage)->getAnimpoint());

			Script_ActionReachAnimpoint* const p_scheme_reach = new Script_ActionReachAnimpoint("action_reach_animpoint", static_cast<DataBase::Script_ComponentScheme_XRAnimPoint*>(storage));
			p_scheme_reach->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAlive, true));
			p_scheme_reach->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAnomaly, false));
			p_scheme_reach->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
			p_scheme_reach->add_condition(CWorldProperty(properties.at("need_animpoint"), true));
			p_scheme_reach->add_condition(CWorldProperty(properties.at("reach_animpoint"), false));
			p_scheme_reach->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStoheMeetBase + 1, false));
			p_scheme_reach->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase, false));
			p_scheme_reach->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kAbuseBase, false));
			p_scheme_reach->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kWoundedExist, false));
			p_scheme_reach->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kCorpseExist, false));
			p_scheme_reach->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyItems, false));

			p_scheme_reach->add_effect(CWorldProperty(properties.at("need_animpoint"), false));
			p_scheme_reach->add_effect(CWorldProperty(properties.at("state_mgr_logic_active"), false));
			p_planner->add_operator(operators.at("action_reach_animpoint"), p_scheme_reach);
			DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme_reach);

			Script_SchemeXRAnimpoint* const p_scheme = new Script_SchemeXRAnimpoint("action_animpoint", static_cast<DataBase::Script_ComponentScheme_XRAnimPoint*>(storage));
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAlive, true));
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAnomaly, false));
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyEnemy, false));
			p_scheme->add_condition(CWorldProperty(properties.at("need_animpoint"), true));
			p_scheme->add_condition(CWorldProperty(properties.at("reach_animpoint"), true));
			p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kStoheMeetBase + 1, false));
			p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase, false));
			p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kAbuseBase, false));
			p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kWoundedExist, false));
			p_scheme->add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kCorpseExist, false));
			p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyItems, false));

			p_scheme->add_effect(CWorldProperty(properties.at("need_animpoint"), false));
			p_scheme->add_effect(CWorldProperty(properties.at("state_mgr_logic_active"), false));
			p_planner->add_operator(operators.at("action_animpoint"), p_scheme);
			DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme);

			p_planner->action(Globals::XR_ACTIONS_ID::kAlife).add_condition(CWorldProperty(properties.at("need_animpoint"), false));
		}

		void Script_SchemeXRAnimpoint::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
		{
			DataBase::Script_ComponentScheme_XRAnimPoint* const p_storage = XR_LOGIC::assign_storage_and_bind<DataBase::Script_ComponentScheme_XRAnimPoint>(p_client_object, p_ini, scheme_name, section_name, gulag_name);

			p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
			xr_string cover_name = Globals::Utils::cfg_get_string(p_ini, section_name, "cover_name");
			if (cover_name.empty())
				cover_name = "$script_id$_cover";
			p_storage->setCoverName(cover_name);
			p_storage->setUseCamp(Globals::Utils::cfg_get_bool(p_ini, section_name, "use_camp"));
			float reach_distance = Globals::Utils::cfg_get_number(p_ini, section_name, "reach_distance");

			if (fis_zero(reach_distance))
				reach_distance = 0.75f;

			reach_distance *= reach_distance;
			p_storage->setReachDistance(reach_distance);

			xr_string movement_name = Globals::Utils::cfg_get_string(p_ini, section_name, "reach_movement");

			if (movement_name.empty())
				movement_name = "walk";

			p_storage->setReachMovementName(movement_name);

			p_storage->setAvailAnimations(Globals::Utils::parse_names(Globals::Utils::cfg_get_string(p_ini, section_name, "avail_animations")));
		}

		Script_ActionReachAnimpoint::Script_ActionReachAnimpoint(const xr_string& name, DataBase::Script_ComponentScheme_XRAnimPoint* storage) : Script_ISchemeStalker(nullptr, name, storage), m_p_storage(storage)
		{
		}

		Script_ActionReachAnimpoint::~Script_ActionReachAnimpoint(void)
		{
		}

		void Script_ActionReachAnimpoint::initialize(void)
		{ CScriptActionBase::initialize();
            this->m_p_storage->getAnimpoint()->calculate_position();
		}

		void Script_ActionReachAnimpoint::execute(void)
		{ CScriptActionBase::execute();
            this->m_object->set_dest_level_vertex_id(this->m_p_storage->getAnimpoint()->getPositionVertex());
            this->m_object->set_desired_direction(&this->m_p_storage->getAnimpoint()->getSmartDirection());
            this->m_object->set_path_type(MovementManager::ePathTypeLevelPath);

            bool is_distance_reached =
                this->m_object->Position().distance_to_sqr(this->m_p_storage->getAnimpoint()->getVertexPosition()) <=
                this->m_p_storage->getReachDistance();

            if (is_distance_reached)
            {
                Globals::set_state(this->m_object, this->m_p_storage->getReachMovementName(),
                    StateManagerCallbackData(), 0,
                    std::pair<Fvector, CScriptGameObject* const>(this->m_p_storage->getAnimpoint()->getLookPosition(), nullptr),
                    StateManagerExtraData());
            }
            else
            {
                Globals::set_state(this->m_object, this->m_p_storage->getReachMovementName(),
                    StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(Fvector(), nullptr),
                    StateManagerExtraData());
            }
		}

		void Script_ActionReachAnimpoint::finalize(void)
		{ 
            CScriptActionBase::finalize();
		}

		Script_EvaluatorReachAnimpoint::_value_type Script_EvaluatorReachAnimpoint::evaluate(void)
		{
			return this->m_p_storage->getAnimpoint()->position_riched();
		}

		Script_EvaluatorNeedAnimpoint::_value_type Script_EvaluatorNeedAnimpoint::evaluate(void)
		{
			return XR_LOGIC::is_active(this->m_object, this->m_p_storage);
		}

		Script_Animpoint::Script_Animpoint(const std::uint16_t npc_id, DataBase::Script_ComponentScheme_XRAnimPoint* storage) : Script_ISchemeEntity(nullptr, storage), m_p_storage(&storage), m_npc_id(npc_id),
			m_is_started(false), m_position_vertex(0), m_p_camp(nullptr), m_p_storage(storage)
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
				if (!this->m_p_storage->isUseCamp() && this->m_p_storage->getCoverName() == this->m_cover_name)
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
			if (DataBase::Storage::getInstance().getGameRegisteredServerSmartCovers().find(this->m_p_storage->getCoverName()) == DataBase::Storage::getInstance().getGameRegisteredServerSmartCovers().end())
			{
				p_cover = DataBase::Storage::getInstance().getGameRegisteredServerSmartCovers().at(this->m_p_storage->getCoverName());
			}
			else
			{
				MESSAGEER("Can't find smart cover's data %s", this->m_p_storage->getCoverName().c_str());
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
				if (this->m_p_storage->getAvailAnimations().empty())
				{
					MESSAGEER("Wrong animpoint smart_cover description %s | %s", description_name.c_str(), p_cover->name_replace());
					return;
				}
			}

			this->m_p_storage->setDescriptionName(description_name);
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
				return false;
			}
			else
			{
				p_object = DataBase::Storage::getInstance().getStorage().at(this->m_npc_id).getClientObject();
			}

			if (p_object == nullptr)
				return false;

			bool is_distance_reached = p_object->Position().distance_to_sqr(this->m_vertex_position) <= this->m_p_storage->getReachDistance();

			float v1 = -deg(atan2(this->m_smart_direction.x, this->m_smart_direction.z));
			float v2 = -deg(atan2(p_object->Direction().x, p_object->Direction().z));

			float rot_y = std::min<float>(static_cast<float>(fabs(v1 - v2)), static_cast<float>(360.0f - fabs(v1) - fabs(v2)));

			bool is_direction_reached = rot_y < 50.0f;

			return (is_direction_reached) && (is_direction_reached);
		}

		void Script_Animpoint::fill_approved_actions(void)
		{
			if (this->m_p_storage->getAvailAnimations().empty() == false)
			{
				for (const xr_string& it : this->m_p_storage->getAvailAnimations())
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
			if (this->m_p_storage->isUseCamp())
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
			this->m_cover_name = this->m_p_storage->getCoverName();
		}

		void Script_Animpoint::stop(void)
		{
			if (this->m_p_camp)
				this->m_p_camp->unregister_npc(this->m_npc_id);

			this->m_is_started = false;
			this->m_action_name.clear();
		}

		void Script_Animpoint::update(const float delta)
		{
			const xr_string& description_name = this->m_p_storage->getDescriptionName();
			xr_vector<xr_string> temp_actions;
			if (!this->m_p_storage->isUseCamp())
			{
				if (this->m_p_storage->getAvailAnimations().empty())
				{
					if (this->m_p_storage->getApprovedActions().empty())
					{
						MESSAGEER("animpoint not in camp and approved_actions is null. %s", this->m_p_storage->getCoverName().c_str());
						return;
					}

					for (const std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>& it : this->m_p_storage->getApprovedActions())
					{
						temp_actions.push_back(it.second);
					}
				}
				else
				{
					for (const xr_string& it : this->m_p_storage->getAvailAnimations())
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

            xr_string camp_action_name;
            bool is_director;
            this->m_p_camp->get_camp_action(this->m_npc_id, camp_action_name, is_director);
            
            xr_vector<xr_string> vector;
            if (is_director)
            {
                vector = Script_GlobalHelper::getInstance().getXRAnimpointAssociationTable().at(camp_action_name).first;
            }
            else
            {
                vector = Script_GlobalHelper::getInstance().getXRAnimpointAssociationTable().at(camp_action_name).second;
            }

            bool is_found = false;

            for (const std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>& it :
                this->m_p_storage->getApprovedActions())
            {
                for (const xr_string& it2 : vector)
                {
                    if (xr_string(description_name).append(it2) == it.second)
                    {
                        temp_actions.push_back(it.second);
                        is_found = true;
                    }
                }
            }

            if (!is_found)
            {
                temp_actions.push_back(description_name);
            }

            std::uint32_t generated_id = Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(0, temp_actions.size() - 1);
            xr_string picked_action_name = temp_actions.at(generated_id);
            xr_string as_iterator = temp_actions.at(generated_id);
            if (this->m_p_storage->getBaseActionName().empty() == false)
            {
                if (this->m_p_storage->getBaseActionName() == xr_string(description_name).append("_weapon"))
                {
                    picked_action_name = description_name;
                    picked_action_name.append("_weapon");
                }

                if (picked_action_name == xr_string(description_name).append("_weapon") && (this->m_p_storage->getBaseActionName() == description_name))
                {
                    temp_actions.erase(temp_actions.begin() + generated_id);
                    picked_action_name = temp_actions.at(Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(0, temp_actions.size() - 1));
                }
            }
            else
            {
                if (picked_action_name == xr_string(description_name).append("_weapon"))
                {
                    this->m_p_storage->setBaseActionName(picked_action_name);
                }
                else
                {
                    this->m_p_storage->setBaseActionName(description_name);
                }
            }

            this->m_action_name = picked_action_name;
		}
	}
}
