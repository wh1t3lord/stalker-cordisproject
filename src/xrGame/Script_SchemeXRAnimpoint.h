#pragma once

namespace Cordis
{
	namespace Scripts
	{
		class Script_Animpoint : public Script_ISchemeEntity
		{
		public:
			Script_Animpoint(void) = delete;
			Script_Animpoint(const std::uint16_t npc_id, DataBase::Storage_Scheme& storage);
			~Script_Animpoint(void);

			void initialize(void);
			virtual void activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object);
			void calculate_position(void);
			void get_animation_params(Fvector& position, Fvector& smart_direction);
			bool position_riched(void);
			void fill_approved_actions(void);
			void start(void);
			void stop(void);
			virtual void update(const float delta);
			const xr_string& getActionName(void) noexcept { return this->m_action_name; }
            inline const Fvector& getSmartDirection(void) const noexcept { return this->m_smart_direction; }
            inline const Fvector& getVertexPosition(void) const noexcept { return this->m_vertex_position; }
            inline const Fvector& getLookPosition(void) const noexcept { return this->m_look_position; }
            inline bool isStarted(void) const noexcept { return this->m_is_started; }
            inline std::uint32_t getPositionVertex(void) const noexcept { return this->m_position_vertex; }
			
		private:
			bool m_is_started;
			std::uint16_t m_npc_id;
			std::uint32_t m_position_vertex;
			DataBase::Storage_Scheme* m_p_storage;
			Script_CampData* m_p_camp;
			Fvector m_position;
			Fvector m_smart_direction;
			Fvector m_vertex_position;
			Fvector m_look_position;
			xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>> m_avail_actions;
			xr_string m_cover_name;
			xr_string m_action_name;
		};

		class Script_SchemeXRAnimpoint : public Script_ISchemeStalker
		{
		public:
			Script_SchemeXRAnimpoint(void) = delete;
			Script_SchemeXRAnimpoint(const xr_string& name, DataBase::Storage_Scheme& storage);
			~Script_SchemeXRAnimpoint(void);

			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);
			virtual void net_destroy(CScriptGameObject* const p_client_object);

			static inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
				const xr_string& scheme_name, const xr_string& section_name, DataBase::Storage_Scheme& storage)
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

				p_planner->add_evaluator(properties.at("need_animpoint"), new Script_EvaluatorNeedAnimpoint("animpoint_need", storage));
				p_planner->add_evaluator(properties.at("reach_animpoint"), new Script_EvaluatorReachAnimpoint("animpoint_reach", storage));


				storage.setAnimpoint(new Script_Animpoint(p_client_object->ID(), storage));
				DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, storage.getAnimpoint());

				Script_ActionReachAnimpoint* const p_scheme_reach = new Script_ActionReachAnimpoint("action_reach_animpoint", storage);
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

				Script_SchemeXRAnimpoint* const p_scheme = new Script_SchemeXRAnimpoint("action_animpoint", storage);
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

			static inline void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
				const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
			{
				DataBase::Storage_Scheme* const p_storage = XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);

				p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
				xr_string cover_name = Globals::Utils::cfg_get_string(p_ini, section_name, "cover_name");
				if (cover_name.empty())
					cover_name = "$script_id$_cover";
				p_storage->setXRAnimpointCoverName(cover_name);
				p_storage->setXRAnimpointUseCamp(Globals::Utils::cfg_get_bool(p_ini, section_name, "use_camp"));
				float reach_distance = Globals::Utils::cfg_get_number(p_ini, section_name, "reach_distance");

				if (fis_zero(reach_distance))
					reach_distance = 0.75f;

				reach_distance *= reach_distance;
				p_storage->setXRAnimpointReachDistance(reach_distance);
				
				xr_string movement_name = Globals::Utils::cfg_get_string(p_ini, section_name, "reach_movement");

				if (movement_name.empty())
					movement_name = "walk";

				p_storage->setXRAnimpointReachMovementName(movement_name);

				p_storage->setXRAnimpointAvailAnimations(Globals::Utils::parse_names(Globals::Utils::cfg_get_string(p_ini, section_name, "avail_animations")));
			}
		};

		class Script_ActionReachAnimpoint : public Script_ISchemeStalker
		{
		public:
			Script_ActionReachAnimpoint(void) = delete;
			Script_ActionReachAnimpoint(const xr_string& name, DataBase::Storage_Scheme& storage);
			~Script_ActionReachAnimpoint(void);

			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);
		};

		class Script_EvaluatorReachAnimpoint : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorReachAnimpoint(const xr_string& name, DataBase::Storage_Scheme& storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(&storage) {}
			~Script_EvaluatorReachAnimpoint(void) {}

			virtual _value_type evaluate(void);

		private:
			DataBase::Storage_Scheme* m_p_storage;
		};

		class Script_EvaluatorNeedAnimpoint : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorNeedAnimpoint(const xr_string& name, DataBase::Storage_Scheme& storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(&storage) {}

			~Script_EvaluatorNeedAnimpoint(void) {}

			virtual _value_type evaluate(void);

		private:
			DataBase::Storage_Scheme* m_p_storage;
		};
	}
}


