#pragma once

namespace Cordis
{
	namespace Scripts
	{
		namespace DataBase
		{
			struct Script_ComponentScheme_PHButton
			{
				Script_ComponentScheme_PHButton(void) : m_is_blending(false) {}

				inline const xr_string& getAnimationName(void) const noexcept { return this->m_animation_name; }
				inline void setAnimationName(const xr_string& animation_name) noexcept
				{
					if (animation_name.empty())
					{
						MESSAGEW("animation_name.empty() == true! You set an empty string");
					}
					this->m_animation_name = animation_name;
				}

				inline const xr_string& getToolTipName(void) const noexcept { return this->m_tooptip_name; }
				inline void setToolTipName(const xr_string& value_name) noexcept
				{
					if (value_name.empty())
					{
						MESSAGEW("value_name.empty() == "
							"true! You set an empty string");
					}

					this->m_tooptip_name = value_name;
				}

				inline bool IsBlending(void) const noexcept { return this->m_is_blending; }
				inline void setBlending(const bool value) noexcept { this->m_is_blending = value; }

				inline const xr_map<std::uint32_t, CondlistData>& getOnPressCondlist(void) const noexcept
				{
					return this->m_on_press_condlist;
				}

				inline void setOnPressCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
				{
					this->m_on_press_condlist = condlist;
				}

				inline void clear(void) noexcept 
				{
					this->m_is_blending = false;
					this->m_on_press_condlist.clear();
					this->m_animation_name.clear();
					this->m_tooptip_name.clear();
				}

			private:
				bool m_is_blending;
				xr_map<std::uint32_t, CondlistData> m_on_press_condlist;
				xr_string m_animation_name;
				xr_string m_tooptip_name;
			};
			struct Script_ComponentScheme_XRSmartCover
			{
				Script_ComponentScheme_XRSmartCover(void) : m_is_precalc_cover(false), m_is_in_combat(false), m_idle_max_time(0), m_idle_min_time(0), m_lookout_min_time(0), m_lookout_max_time(0) {}

				inline void clear(void) noexcept
				{
					this->m_is_in_combat = false;
					this->m_is_precalc_cover = false;
					this->m_idle_max_time = 0;
					this->m_idle_min_time = 0;
					this->m_lookout_max_time = 0;
					this->m_lookout_min_time = 0;
					this->m_cover_name.clear();
					this->m_loophole_name.clear();
					this->m_cover_state_name.clear();
					this->m_target_enemy_name.clear();
					this->m_target_path_name.clear();
					this->m_exit_body_state_name.clear();
					this->m_weapon_type_name.clear();
					this->m_moving_name.clear();
					this->m_sound_idle_name.clear();
				}

				inline const xr_string& getXRSmartCoverCoverName(void) const noexcept { return this->m_cover_name; }
				inline void setXRSmartCoverCoverName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEW("setting empty string!"); } this->m_cover_name = name; }

				inline const xr_string& getXRSmartCoverLoopholeName(void) const noexcept { return this->m_loophole_name; }
				inline void setXRSmartCoverLoopholeName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEW("setting empty string!"); } this->m_loophole_name = name; }

				inline const xr_string& getXRSmartCoverCoverStateName(void) const noexcept { return this->m_cover_state_name; }
				inline void setXRSmartCoverCoverStateName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("invalid string!"); return; } this->m_cover_state_name = name; }

				inline const xr_string& getXRSmartCoverTargetEnemyName(void) const noexcept { return this->m_target_enemy_name; }
				inline void setXRSmartCoverTargetEnemyName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("invalid string!"); return; } this->m_target_enemy_name = name; }

				inline const xr_string& getXRSmartCoverTargetPathName(void) const noexcept { return this->m_target_path_name; }
				inline void setXRSmartCoverTargetPathName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("invalid string!"); return; } this->m_target_path_name = name; }

				inline const xr_string& getXRSmartCoverExitBodyStateName(void) const noexcept { return this->m_exit_body_state_name; }
				inline void setXRSmartCoverExitBodyStateName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("invalid string!"); return; } this->m_exit_body_state_name = name; }

				inline const xr_string& getXRSmartCoverWeaponTypeName(void) const noexcept { return this->m_weapon_type_name; }
				inline void setXRSmartCoverWeaponTypeName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("invalid string!"); return; } this->m_weapon_type_name = name; }

				inline const xr_string& getXRSmartCoverMovingName(void) const noexcept { return this->m_moving_name; }
				inline void setXRSmartCoverMovingName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("invalid string!"); return; } this->m_moving_name = name; }

				inline const xr_string& getXRSmartCoverSoundIdleName(void) const noexcept { return this->m_sound_idle_name; }
				inline void setXRSmartCoverSoundIdleName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("invalid string!"); return; } this->m_sound_idle_name; }

				inline bool isXRSmartCoverUsePreCalcCover(void) const noexcept { return this->m_is_precalc_cover; }
				inline void setXRSmartCoverUsePreCalcCover(const bool value) noexcept { this->m_is_precalc_cover = value; }

				inline bool isXRSmartCoverUseInCombat(void) const noexcept { return this->m_is_in_combat; }
				inline void setXRSmartCoverUseInCombat(const bool value) noexcept { this->m_is_in_combat = value; }

				inline std::uint32_t getXRSmartCoverIdleMinTime(void) const noexcept { return this->m_idle_min_time; }
				inline void setXRSmartCoverIdleMinTime(const std::uint32_t value) noexcept { this->m_idle_min_time = value; }

				inline std::uint32_t getXRSmartCoverIdleMaxTime(void) const noexcept { return this->m_idle_max_time; }
				inline void setXRSmartCoverIdleMaxTime(const std::uint32_t value) noexcept { this->m_idle_max_time = value; }

				inline float getXRSmartCoverLookOutMinTime(void) const noexcept { return this->m_lookout_min_time; }
				inline void setXRSmartCoverLookOutMinTime(const float value) noexcept { this->m_lookout_min_time = value; }

				inline float getXRSmartCoverLookOutMaxTime(void) const noexcept { return this->m_lookout_max_time; }
				inline void setXRSmartCoverLookOutMaxTime(const float value) noexcept { this->m_lookout_max_time = value; }

			private:
				bool m_is_precalc_cover;
				bool m_is_in_combat;
				std::uint32_t m_idle_min_time;
				std::uint32_t m_idle_max_time;
				float m_lookout_min_time;
				float m_lookout_max_time;
				xr_string m_cover_name;
				xr_string m_loophole_name;
				xr_string m_cover_state_name;
				xr_string m_target_enemy_name;
				xr_string m_target_path_name;
				xr_string m_exit_body_state_name;
				xr_string m_weapon_type_name;
				xr_string m_moving_name;
				xr_string m_sound_idle_name;
			};
			struct Script_ComponentScheme_PostCombat
			{
				Script_ComponentScheme_PostCombat(void) : m_last_best_enemy_id(0), m_timer(0), m_p_animation(nullptr) {}
				~Script_ComponentScheme_PostCombat(void) 
				{
					if (this->m_p_animation)
					{
						MESSAGEI("deleting post_combat data");
						xr_delete(this->m_p_animation);
					}
				}

				inline std::uint16_t getPostCombatLastBestEnemyID(void) const noexcept { return this->m_last_best_enemy_id; }
				inline void setPostCombatLastBestEnemyID(const std::uint16_t id) noexcept { this->m_last_best_enemy_id = id; }

				inline std::uint32_t getPostCombatTimer(void) const noexcept { return this->m_timer; }
				inline void setPostCombatTimer(const std::uint32_t value) noexcept { this->m_timer = value; }

				inline const xr_string& getPostCombatLastBestEnemyName(void) const noexcept { return this->m_last_best_enemy_name; }
				inline void setPostCombatLastBestEnemyName(const xr_string& name) noexcept { this->m_last_best_enemy_name = name; }

				inline Script_StateAnimation* getPostCombatAnimation(void) const { return this->m_p_animation; }
				inline void setPostCombatAnimation(Script_StateAnimation* const p_data)
				{
					this->m_p_animation = p_data;
				}

				inline void clear(void) 
				{
					this->m_last_best_enemy_id = 0;
					this->m_timer = 0;
					this->m_last_best_enemy_name.clear();

					if (this->m_p_animation)
					{
						MESSAGEI("deleting data!");
						xr_delete(this->m_p_animation);
					}
				}

			private:
				std::uint16_t m_last_best_enemy_id;
				std::uint32_t m_timer;
				Script_StateAnimation* m_p_animation;
				xr_string m_last_best_enemy_name;
			};
			struct Script_ComponentScheme_XRAnimPoint
			{
				Script_ComponentScheme_XRAnimPoint(void) : m_is_use_camp(false), m_reach_distance(0.0f), m_p_animpoint(nullptr) {}
				~Script_ComponentScheme_XRAnimPoint(void) 
				{
					if (this->m_p_animpoint)
					{
						MESSAGEI("deleting animpoint");
						xr_delete(this->m_p_animpoint);
					}
				}

				inline const xr_string& getCoverName(void) const noexcept { return this->m_cover_name; }
				inline void setCoverName(const xr_string& name) noexcept
				{
					if (name.empty())
					{
						MESSAGEWR("You are trying to set an empty string!");
						return;
					}

					this->m_cover_name = name;
				}

				inline const xr_vector<xr_string>& getAvailAnimations(void) const noexcept {
					return this->m_avail_animations;
				}

				inline void setAvailAnimations(const xr_vector<xr_string>& data) noexcept
				{
					if (data.empty())
					{
						MESSAGEWR("You are trying to set an empty vector!");
						return;
					}

					this->m_avail_animations = data;
				}

				inline bool isUseCamp(void) const noexcept { return this->m_is_use_camp; }
				inline void setUseCamp(const bool value) noexcept { this->m_is_use_camp = value; }

				inline const xr_string& getReachMovementName(void) const noexcept { return this->m_reach_movement_name; }
				inline void setReachMovementName(const xr_string& name) noexcept {
					if (name.empty())
					{
						MESSAGEWR("You are trying to set an empty string!");
						return;
					}

					this->m_reach_movement_name = name;
				}

				inline float getReachDistance(void) const noexcept { return this->m_reach_distance; }
				inline void setReachDistance(const float value) noexcept { this->m_reach_distance = value; }

				inline const xr_string& getDescriptionName(void) const noexcept { return this->m_description_name; }
				inline void setDescriptionName(const xr_string& name) noexcept { if (name.empty()) { MESSAGEWR("you are trying to set an empty string!"); return; }  this->m_description_name = name; }

				inline const xr_string& getBaseActionName(void) const noexcept
				{
					return this->m_base_action_name;
				}

				inline void setBaseActionName(const xr_string& name) noexcept
				{
					if (name.empty())
					{
						MESSAGEWR("You are trying to set an empty string!");
						return;
					}

					this->m_base_action_name = name;
				}

				inline Script_Animpoint* getAnimpoint(void) const { return this->m_p_animpoint; }
				inline void setAnimpoint(Script_Animpoint* const p_animpoint)
				{
					if (p_animpoint == nullptr)
					{
						MESSAGEW("set an empty object!");
					}

					this->m_p_animpoint = p_animpoint;
				}

				inline void clear(void) 
				{
					this->m_is_use_camp = false;
					this->m_avail_animations.clear();
					this->m_base_action_name.clear();
					this->m_cover_name.clear();
					this->m_reach_distance = 0.0f;
					this->m_description_name.clear();
					this->m_reach_movement_name.clear();

					if (this->m_p_animpoint)
					{
						MESSAGEI("deleting data!");
						xr_delete(this->m_p_animpoint);
					}
				}


			private:
				bool m_is_use_camp;
				float m_reach_distance;
				Script_Animpoint* m_p_animpoint;
				xr_vector<xr_string> m_avail_animations;
				xr_string m_cover_name;
				xr_string m_reach_movement_name;
				xr_string m_description_name;
				xr_string m_base_action_name;
			};
			struct Script_ComponentScheme_XRPatrol
			{
				Script_ComponentScheme_XRPatrol(void) : m_is_silent(false), m_is_commander(false) {}

				inline bool isCommander(void) const noexcept { return this->m_is_commander; }
				inline void setCommander(const bool value) noexcept { this->m_is_commander = value; }

				inline bool isSilent(void) const noexcept { return this->m_is_silent; }
				inline void setSilent(const bool value) noexcept { this->m_is_silent = value; }

				inline const xr_string& getPathName(void) const noexcept { return this->m_path_name; }
				inline void setPathName(const xr_string& path_name) noexcept { if (path_name.empty()) { MESSAGEWR("string is empty!"); return; } this->m_path_name = path_name; }

				inline const xr_string& getPatrolKeyName(void) const noexcept { return this->m_patrol_key_name; }
				inline void setPatrolKeyName(const xr_string& key_name) noexcept { if (key_name.empty()) { MESSAGEWR("string is empty!"); return; } this->m_patrol_key_name = key_name; }

				inline const xr_string& getFormationName(void) const noexcept { return this->m_formation_name; }
				inline void setFormationName(const xr_string& formation_name) noexcept { if (formation_name.empty()) { MESSAGEWR("string is empty!"); return; } this->m_formation_name = formation_name; }

				inline const xr_string& getMoveTypeName(void) const noexcept { return this->m_move_type_name; }
				inline void setMoveTypeName(const xr_string& type_name) noexcept { if (type_name.empty()) { MESSAGEWR("string is empty!"); return; } this->m_move_type_name = type_name; }

				inline const xr_map<xr_string, xr_string>& getSuggestedStates(void) const noexcept { return this->m_suggested_states; }
				inline void setSuggestedStates(const xr_string& state_name, const xr_string& value_name) noexcept
				{
					if (state_name.empty())
					{
						MESSAGEWR("can't be empty!");
						return;
					}

					this->m_suggested_states[state_name] = value_name;
				}

				inline void clear(void) noexcept 
				{
					this->m_is_commander = false;
					this->m_is_silent = false;
					this->m_suggested_states.clear();
					this->m_path_name.clear();
					this->m_patrol_key_name.clear();
					this->m_formation_name.clear();
					this->m_move_type_name.clear();
				}


			private:
				bool m_is_silent;
				bool m_is_commander;
				xr_map<xr_string, xr_string> m_suggested_states;
				xr_string m_patrol_key_name;
				xr_string m_path_name;
				xr_string m_formation_name;
				xr_string m_move_type_name;
			};
			struct Script_ComponentScheme_XRCombatCamper
			{
				Script_ComponentScheme_XRCombatCamper(void) : m_is_camper_action(false) {}

				inline bool isAction(void) const noexcept { return this->m_is_camper_action; }
				inline void setAction(const bool value) noexcept { this->m_is_camper_action = value; }

				inline const Fvector& getLastSeenPosition(void) const noexcept { return this->m_camper_last_seen_position; }
				inline void setLastSeenPosition(const Fvector& position) noexcept { this->m_camper_last_seen_position = position; }

				inline void clear(void) noexcept 
				{
					this->m_is_camper_action = false;
					this->m_camper_last_seen_position.x = 0.0f;
					this->m_camper_last_seen_position.y = 0.0f;
					this->m_camper_last_seen_position.z = 0.0f;
				}

			private:
				bool m_is_camper_action;
				Fvector m_camper_last_seen_position;
			};
			struct Script_ComponentScheme_XRCombatZombied
			{
				Script_ComponentScheme_XRCombatZombied(void) : m_zombied_current_action(0) {}

				inline std::uint32_t getCurrentAction(void) const noexcept { return this->m_zombied_current_action; }
				inline void setCurrentAction(const std::uint32_t action_id) noexcept { this->m_zombied_current_action = action_id; }

				inline void clear(void) noexcept { this->m_zombied_current_action = 0; }

			private:
				std::uint32_t m_zombied_current_action;
			};
			struct Script_ComponentScheme_XRCombatIgnore
			{
				Script_ComponentScheme_XRCombatIgnore(void) : m_is_ignore_enabled(false) {}

				inline bool isEnabled(void) const noexcept { return this->m_is_ignore_enabled; }
				inline void setEnabled(const bool value) noexcept { this->m_is_ignore_enabled = value; }

				inline void clear(void) noexcept { this->m_is_ignore_enabled = false; }
			private:
				bool m_is_ignore_enabled;
			};
			struct Script_ComponentScheme_XRWounded
			{
				Script_ComponentScheme_XRWounded(void) : m_is_xr_wounded_autoheal(false), m_is_xr_wounded_enable_talk(false), m_is_xr_wounded_not_for_help(false), m_is_xr_wounded_set(false), m_is_xr_wounded_use_medkit(false), m_p_wounded_manager(nullptr) {}
				~Script_ComponentScheme_XRWounded(void) 
				{
					if (this->m_p_wounded_manager)
					{
						MESSAGEI("deleting wounded manager");
						xr_delete(this->m_p_wounded_manager);
					}
				}

				inline bool isXRWoundedSet(void) const noexcept { return this->m_is_xr_wounded_set; }
				inline void setXRWoundedSet(const bool value) noexcept { this->m_is_xr_wounded_set = value; }

				inline bool isXRWoundedUseMedkit(void) const noexcept { return this->m_is_xr_wounded_use_medkit; }
				inline void setXRWoundedUseMedkit(const bool value) noexcept { this->m_is_xr_wounded_use_medkit = value; }

				inline bool isXRWoundedAutoHeal(void) const noexcept { return this->m_is_xr_wounded_autoheal; }
				inline void setXRWoundedAutoHeal(const bool value) noexcept { this->m_is_xr_wounded_autoheal = value; }

				inline bool isXRWoundedEnableTalk(void) const noexcept { return this->m_is_xr_wounded_enable_talk; }
				inline void setXRWoundedEnableTalk(const bool value) noexcept { this->m_is_xr_wounded_enable_talk = value; }

				inline bool isXRWoundedNotForHelp(void) const noexcept { return this->m_is_xr_wounded_not_for_help; }
				inline void setXRWoundedNotForHelp(const bool value) noexcept { this->m_is_xr_wounded_not_for_help = value; }

				inline const xr_string& getXRWoundedHelpDialogName(void) const noexcept { return this->m_xr_wounded_help_dialog_name; }
				inline void setXRWoundedHelpDialogName(const xr_string& dialog_name) noexcept
				{
					if (dialog_name.empty())
					{
						MESSAGEW("you are trying to set an empty string!");
					}

					this->m_xr_wounded_help_dialog_name = dialog_name;
				}

				inline const xr_string& getXRWoundedHelpStartDialogName(void) const noexcept { return this->m_xr_wounded_help_start_dialog_name; }
				inline void setXRWoundedHelpStartDialogName(const xr_string& dialog_start_name) noexcept
				{
					if (dialog_start_name.empty())
					{
						MESSAGEW("you are trying to set an empty string!");
					}

					this->m_xr_wounded_help_start_dialog_name = dialog_start_name;
				}

				inline const xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& getXRWoundedHealthState(void) const noexcept { return this->m_xr_wounded_health_state; }
				inline void setXRWoundedHealthState(xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& data) noexcept
				{
					this->m_xr_wounded_health_state = data;
				}

				inline const xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& getXRWoundedHealthStateSee(void) const noexcept { return this->m_xr_wounded_health_state_see; }
				inline void setXRWoundedHealthStateSee(xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& data) noexcept
				{
					this->m_xr_wounded_health_state_see = data;
				}

				inline const xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& getXRWoundedPsyState(void) const noexcept { return this->m_xr_wounded_psy_state; }
				inline void setXRWoundedPsyState(xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& data) noexcept
				{
					this->m_xr_wounded_psy_state = data;
				}

				inline const xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& getXRWoundedHealthVictim(void) const noexcept { return this->m_xr_wounded_health_victim; }
				inline void setXRWoundedHealthVictim(xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& data) noexcept
				{
					this->m_xr_wounded_health_victim = data;
				}

				inline const xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& getXRWoundedHealthCover(void) const noexcept { return this->m_xr_wounded_health_cover; }
				inline void setXRWoundedHealthCover(xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& data) noexcept
				{
					this->m_xr_wounded_health_cover = data;
				}

				inline const xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& getXRWoundedHealthFight(void) const noexcept { return this->m_xr_wounded_health_fight; }
				inline void setXRWoundedHealthFight(xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>>& data) noexcept
				{
					this->m_xr_wounded_health_cover = data;
				}

				inline const xr_string& getXRWoundedWoundedSectionName(void) const noexcept { return this->m_xr_wounded_wounded_section_name; }
				inline void setXRWoundedWoundedSectionName(const xr_string& section_name) noexcept
				{
					if (section_name.empty())
					{
						MESSAGEW("you are trying to set an empty string!");
					}

					this->m_xr_wounded_wounded_section_name = section_name;
				}

				inline void clear(void) noexcept 
				{
					this->m_is_xr_wounded_autoheal = false;
					this->m_is_xr_wounded_enable_talk = false;
					this->m_is_xr_wounded_not_for_help = false;
					this->m_is_xr_wounded_set = false;
					this->m_is_xr_wounded_use_medkit = false;

					this->m_xr_wounded_health_cover.clear();
					this->m_xr_wounded_health_fight.clear();
					this->m_xr_wounded_health_state.clear();
					this->m_xr_wounded_health_state_see.clear();
					this->m_xr_wounded_health_victim.clear();
					this->m_xr_wounded_psy_state.clear();

					this->m_xr_wounded_help_dialog_name.clear();
					this->m_xr_wounded_wounded_section_name.clear();
					this->m_xr_wounded_help_start_dialog_name.clear();

					if (this->m_p_wounded_manager)
					{
						MESSAGEI("deleting wounded manager");
						xr_delete(this->m_p_wounded_manager);
					}
				}

				inline Script_WoundedManager* getWoundedManager(void) const { return this->m_p_wounded_manager; }
				inline void setWoundedManager(Script_WoundedManager* const p_manager)
				{
					if (this->m_p_wounded_manager)
					{
						R_ASSERT2(false, "you can't set to existed instance you must deallocate this!");
						return;
					}

					if (!p_manager)
					{
						R_ASSERT2(false, "can't be you must allocate manager!");
						return;
					}

					this->m_p_wounded_manager = p_manager;
				}

			private:
				bool m_is_xr_wounded_set;
				bool m_is_xr_wounded_use_medkit;
				bool m_is_xr_wounded_autoheal;
				bool m_is_xr_wounded_enable_talk;
				bool m_is_xr_wounded_not_for_help;
				Script_WoundedManager* m_p_wounded_manager;
				xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>> m_xr_wounded_health_state;
				xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>> m_xr_wounded_health_state_see;
				xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>> m_xr_wounded_psy_state;
				xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>> m_xr_wounded_health_victim;
				xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>> m_xr_wounded_health_cover;
				xr_map<std::uint32_t, std::tuple<std::uint32_t, xr_map<std::uint32_t, CondlistData>, xr_map<std::uint32_t, CondlistData>>> m_xr_wounded_health_fight;
				xr_string m_xr_wounded_help_dialog_name;
				xr_string m_xr_wounded_help_start_dialog_name;
				xr_string m_xr_wounded_wounded_section_name;
			};
			struct Script_ComponentScheme_XRMeet 
			{
				Script_ComponentScheme_XRMeet(void) : m_is_meet_only_at_path(true), m_is_set(false), m_reset_distance(30.0f), m_p_meet_manager(nullptr) {}
				~Script_ComponentScheme_XRMeet(void) 
				{
					if (this->m_p_meet_manager)
					{
						MESSAGEI("deleted meet manager!");
						xr_delete(this->m_p_meet_manager);
					}
				}

				inline const xr_map<std::uint32_t, CondlistData>& getCloseDistance(void) const noexcept { return this->m_close_distance; }
				inline void setCloseDistance(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_close_distance = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getCloseAnim(void) const noexcept { return this->m_close_anim; }
				inline void setCloseAnim(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_close_anim = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getCloseSoundDistance(void) const noexcept { return this->m_close_sound_distance; }
				inline void setCloseSoundDistance(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_close_sound_distance = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getCloseSoundHello(void) const noexcept { return this->m_close_sound_hello; }
				inline void setCloseSoundHello(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_close_sound_hello = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getCloseSoundBye(void) const noexcept { return this->m_close_sound_bye; }
				inline void setCloseSoundBye(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!") this->m_close_sound_bye = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getCloseVictim(void) const noexcept { return this->m_close_victim; }
				inline void setCloseVictim(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_close_victim = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getFarDistance(void) const noexcept { return this->m_far_distance; }
				inline void setFarDistance(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_far_distance = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getFarAnim(void) const noexcept { return this->m_far_anim; }
				inline void setFarAnim(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_far_anim = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getFarSoundDistance(void) const noexcept { return this->m_far_sound_distance; }
				inline void setFarSoundDistance(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_far_sound_distance = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getFarVictim(void) const noexcept { return this->m_far_victim; }
				inline void setFarVictim(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_far_victim = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getSoundOnUse(void) const noexcept { return this->m_sound_on_use; }
				inline void setSoundOnUse(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_sound_on_use = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getUse(void) const noexcept { return this->m_use; }
				inline void setUse(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_use = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getMeetDialog(void) const noexcept { return this->m_meet_dialog; }
				inline void setMeetDialog(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_meet_dialog = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getAbuse(void) const noexcept { return this->m_abuse; }
				inline void setAbuse(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_abuse = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getTradeEnable(void) const noexcept { return this->m_trade_enable; }
				inline void setTradeEnable(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_trade_enable = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getAllowBreak(void) const noexcept { return this->m_allow_break; }
				inline void setAllowBreak(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_allow_break = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getMeetOnTalking(void) const noexcept { return this->m_meet_on_talking; }
				inline void setMeetOnTalking(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_meet_on_talking = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getUseText(void) const noexcept { return this->m_use_text; }
				inline void setUseText(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_use_text = data; }

				inline const xr_map<std::uint32_t, CondlistData>& getFarSound(void) const noexcept { return this->m_far_sound; }
				inline void setFarSound(const xr_map<std::uint32_t, CondlistData>& data) noexcept { if (data.empty()) MESSAGEW("you are set an empty map!"); this->m_far_sound = data; }

				inline float getResetDistance(void) const noexcept { return this->m_reset_distance; }
				inline void setResetDistance(const float value) noexcept { this->m_reset_distance = value; }

				inline bool isMeetOnlyAtPath(void) const noexcept { return this->m_is_meet_only_at_path; }
				inline void setMeetOnlyAtPath(const bool value) noexcept { this->m_reset_distance = value; }

				inline bool isSet(void) const noexcept { return this->m_is_set; }
				inline void setSet(const bool value) noexcept { this->m_is_set = value; }

				inline const xr_map<std::uint32_t, CondlistData>& getMeetDialogName(void) const noexcept { return this->m_meet_dialog; }
				inline void setMeetDialogName(const xr_map<std::uint32_t, CondlistData>& data) noexcept
				{
					if (data.empty()) MESSAGEW("you are trying to set an empty map!");
					this->m_meet_dialog = data;
				}

				inline const xr_string& getMeetSectionName(void) const noexcept { return this->m_meet_section_name; }
				inline void setMeetSectionName(const xr_string& name) noexcept
				{
					if (name.empty())
					{
						MESSAGEW("you are trying to set an empty map!");
					}

					this->m_meet_section_name = name;
				}

				inline void clear(void) 
				{
					this->m_is_meet_only_at_path = true;
					this->m_is_set = false;
					this->m_reset_distance = 30.0f;
					this->m_close_anim.clear();
					this->m_close_distance.clear();
					this->m_close_sound_bye.clear();
					this->m_close_sound_distance.clear();
					this->m_close_sound_hello.clear();
					this->m_close_victim.clear();
					this->m_far_anim.clear();
					this->m_far_distance.clear();
					this->m_far_sound.clear();
					this->m_far_sound_distance.clear();
					this->m_far_victim.clear();
					this->m_sound_on_use.clear();
					this->m_use.clear();
					this->m_meet_dialog.clear();
					this->m_abuse.clear();
					this->m_trade_enable.clear();
					this->m_allow_break.clear();
					this->m_meet_on_talking.clear();
					this->m_use_text.clear();
					this->m_meet_section_name.clear();

					if (this->m_p_meet_manager)
					{
						MESSAGEI("deleted meet manager!");
						xr_delete(this->m_p_meet_manager);
					}
				}

				inline Script_XRMeetManager* const getMeetManager(void) const { return this->m_p_meet_manager; }
				inline void setMeetManager(Script_XRMeetManager* const p_meet)
				{
					if (this->m_p_meet_manager)
					{
						R_ASSERT2(false, "you can't set to existed instance you must deallocate this!");
						return;
					}

					if (!p_meet)
					{
						MESSAGEER("Can't set an empty instance check your code and allocation!");
						return;
					}

					this->m_p_meet_manager = p_meet;
				}
			private:
				bool m_is_meet_only_at_path;
				bool m_is_set;
				float m_reset_distance;
				Script_XRMeetManager* m_p_meet_manager;
				xr_map<std::uint32_t, CondlistData> m_close_distance;
				xr_map<std::uint32_t, CondlistData> m_close_anim;
				xr_map<std::uint32_t, CondlistData> m_close_sound_distance;
				xr_map<std::uint32_t, CondlistData> m_close_sound_hello;
				xr_map<std::uint32_t, CondlistData> m_close_sound_bye;
				xr_map<std::uint32_t, CondlistData> m_close_victim;
				xr_map<std::uint32_t, CondlistData> m_far_distance;
				xr_map<std::uint32_t, CondlistData> m_far_anim;
				xr_map<std::uint32_t, CondlistData> m_far_sound_distance;
				xr_map<std::uint32_t, CondlistData> m_far_sound;
				xr_map<std::uint32_t, CondlistData> m_far_victim;
				xr_map<std::uint32_t, CondlistData> m_sound_on_use;
				xr_map<std::uint32_t, CondlistData> m_use;
				xr_map<std::uint32_t, CondlistData> m_meet_dialog;
				xr_map<std::uint32_t, CondlistData> m_abuse;
				xr_map<std::uint32_t, CondlistData> m_trade_enable;
				xr_map<std::uint32_t, CondlistData> m_allow_break;
				xr_map<std::uint32_t, CondlistData> m_meet_on_talking;
				xr_map<std::uint32_t, CondlistData> m_use_text;
				xr_string m_meet_section_name;
			};
			struct Script_ComponentScheme_XRCamper
			{
				Script_ComponentScheme_XRCamper(void) : m_is_xr_camper_sniper(false), m_is_xr_camper_no_retreat(false), m_xr_camper_wp_flag(0),
					m_xr_camper_scan_begin(0), m_xr_camper_current_look_point(0), m_xr_camper_memory_enemy(0), m_xr_camper_idle(0), m_xr_camper_scantimefree(0),
					m_xr_camper_radius(0.0f)
				{}

				inline float getXRCamperRadius(void) const noexcept { return this->m_xr_camper_radius; }
				inline void setXRCamperRadius(const float value) noexcept { this->m_xr_camper_radius = value; }

				inline const xr_string& getXRCamperAttackSoundName(void) const noexcept { return this->m_xr_camper_attack_sound_name; }
				inline void setXRCamperAttackSoundName(const xr_string& sound_name) noexcept
				{
					if (sound_name.empty())
					{
						MESSAGEW("you are trying to set an empty string!");
					}

					this->m_xr_camper_attack_sound_name = sound_name;
				}

				inline float getXRCamperEnemyDisp(void) const noexcept { return this->m_xr_camper_enemy_disp; }

				inline std::uint32_t getXRCamperPostEnemyWait(void) const noexcept { return this->m_xr_camper_post_enemy_wait; }

				inline float getXRCamperScanDelta(void) const noexcept { return this->m_xr_camper_scandelta; }
				inline std::uint32_t getXRCamperTimeDelta(void) const noexcept { return this->m_xr_camper_timedelta; }
				inline std::uint32_t getXRCamperTimeScanDelta(void) const noexcept { return this->m_xr_camper_timescandelta; }

				inline std::uint32_t getXRCamperIdle(void) const noexcept { return this->m_xr_camper_idle; }
				inline void setXRCamperIdle(const std::uint32_t value) noexcept { this->m_xr_camper_idle = value; }

				inline std::uint32_t getXRCamperScanTimeFree(void) const noexcept { return this->m_xr_camper_scantimefree; }
				inline void setXRCamperScanTimeFree(const std::uint32_t value) noexcept { this->m_xr_camper_scantimefree = value; }

				inline const xr_string& getXRCamperSniperAnimName(void) const noexcept { return this->m_xr_camper_sniper_anim_name; }
				inline void setXRCamperSniperAnimName(const xr_string& animation_name) noexcept
				{
					if (animation_name.empty())
					{
						MESSAGEW("you are trying to set an empty string");
					}

					this->m_xr_camper_sniper_anim_name = animation_name;
				}

				inline bool isXRCamperSniper(void) const noexcept { return this->m_is_xr_camper_sniper; }
				inline void setXRCamperSniper(const bool value) noexcept { this->m_is_xr_camper_sniper = value; }

				inline bool isXRCamperNoRetreat(void) const noexcept { return this->m_is_xr_camper_no_retreat; }
				inline void setXRCamperNoRetreat(const bool value) noexcept { this->m_is_xr_camper_no_retreat = value; }

				inline const xr_string& getXRCamperShootName(void) const noexcept { return this->m_xr_camper_shoot_name; }
				inline void setXRCamperShootName(const xr_string& name) noexcept
				{
					if (name.empty())
					{
						MESSAGEW("you are trying to set an empty string");
					}

					this->m_xr_camper_shoot_name = name;
				}

				inline const std::pair<std::uint32_t, Fvector>& getXRCamperLastLookPoint(void) const noexcept { return this->m_xr_camper_last_look_point; }
				inline void setXRCamperLastLookPointKey(const std::uint32_t value) noexcept { this->m_xr_camper_last_look_point.first = value; }
				inline void setXRCamperLastLookPointPos(const Fvector& position) noexcept { this->m_xr_camper_last_look_point.second = position; }
				inline void setXRCamperLastLookPoint(const std::uint32_t value, const Fvector& position) noexcept { this->m_xr_camper_last_look_point.first = value; this->m_xr_camper_last_look_point.second = position; }


				inline std::uint32_t getXRCamperWpFlag(void) const noexcept { return this->m_xr_camper_wp_flag; }
				inline void setXRCamperWpFlag(const std::uint32_t value) noexcept
				{
					if (value >= std::uint8_t(-1))
					{
						MESSAGEWR("deprecated value!");
						return;
					}
					this->m_xr_camper_wp_flag = value;
				}

				inline const xr_map<xr_string, xr_string>& getXRCamperSuggestedStates(void) const noexcept { return this->m_xr_camper_suggested_states; }
				inline void setXRCamperSuggestedState(const xr_string& state_name, const xr_string& value_name) noexcept
				{
					if (state_name.empty())
					{
						MESSAGEWR("you trying to set an empty string as a key");
						return;
					}

					if (value_name.empty())
					{
						MESSAGEW("you are trying to set an empty string");
						return;
					}

					this->m_xr_camper_suggested_states[state_name] = value_name;
				}

				inline const xr_map<std::uint32_t, xr_vector<std::pair<std::uint32_t, Fvector>>>& getXRCamperScanTable(void) const noexcept { return this->m_xr_camper_scan_table; }
				inline void ClearXRCamperScanTable(void) noexcept { this->m_xr_camper_scan_table.clear(); }
				inline void setXRCamperScanTable(const std::uint32_t index, const std::uint32_t value, const Fvector& position) noexcept
				{
					this->m_xr_camper_scan_table[index].push_back({ value, position });
				}

				inline std::uint32_t getXRCamperScanBegin(void) const noexcept { return this->m_xr_camper_scan_begin; }
				inline void setXRCamperScanBegin(const std::uint32_t value) noexcept { this->m_xr_camper_scan_begin = value; }

				inline std::uint32_t getXRCamperCurrentLookPoint(void) const noexcept { return this->m_xr_camper_current_look_point; }
				inline void setXRCamperCurrentLookPoint(const std::uint32_t value) noexcept { this->m_xr_camper_current_look_point = value; }

				inline std::uint32_t getXRCamperMemoryEnemy(void) const noexcept { return this->m_xr_camper_memory_enemy; }
				inline void setXRCamperMemoryEnemy(const std::uint32_t value) noexcept { this->m_xr_camper_memory_enemy = value; }

				inline void clear(void) noexcept 
				{
					this->m_is_xr_camper_no_retreat = false;
					this->m_is_xr_camper_sniper = false;
				}

			private:
				bool m_is_xr_camper_sniper;
				bool m_is_xr_camper_no_retreat;
				std::uint32_t m_xr_camper_wp_flag;
				std::uint32_t m_xr_camper_scan_begin;
				std::uint32_t m_xr_camper_current_look_point;
				std::uint32_t m_xr_camper_memory_enemy;
				const std::uint32_t m_xr_camper_post_enemy_wait = 5000;
				const std::uint32_t m_xr_camper_timedelta = 4000;
				const std::uint32_t m_xr_camper_timescandelta = this->m_xr_camper_timedelta / this->m_xr_camper_scandelta;
				std::uint32_t m_xr_camper_idle;
				std::uint32_t m_xr_camper_scantimefree;
				float m_xr_camper_radius;
				const float m_xr_camper_scandelta = 30.0f;
				const float m_xr_camper_enemy_disp = 7 / 57.2957f;
				std::pair<std::uint32_t, Fvector> m_xr_camper_last_look_point;
				xr_map<std::uint32_t, xr_vector<std::pair<std::uint32_t, Fvector>>> m_xr_camper_scan_table;
				xr_map<xr_string, xr_string> m_xr_camper_suggested_states;
				xr_string m_xr_camper_attack_sound_name;
				xr_string m_xr_camper_sniper_anim_name;
				xr_string m_xr_camper_shoot_name;
			};
		}
	}
}

#include "Script_ComponentSchemes_Allocators.h"