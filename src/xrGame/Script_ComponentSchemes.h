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
				Script_ComponentScheme_XRCamper(void) : m_is_sniper(false), m_is_no_retreat(false), m_wp_flag(0),
					m_scan_begin(0), m_current_look_point(0), m_memory_enemy(0), m_idle(0), m_scantimefree(0),
					m_radius(0.0f)
				{}

				inline float getRadius(void) const noexcept { return this->m_radius; }
				inline void setRadius(const float value) noexcept { this->m_radius = value; }

				inline const xr_string& getAttackSoundName(void) const noexcept { return this->m_attack_sound_name; }
				inline void setAttackSoundName(const xr_string& sound_name) noexcept
				{
					if (sound_name.empty())
					{
						MESSAGEW("you are trying to set an empty string!");
					}

					this->m_attack_sound_name = sound_name;
				}

				inline float getEnemyDisp(void) const noexcept { return this->m_enemy_disp; }

				inline std::uint32_t getPostEnemyWait(void) const noexcept { return this->m_post_enemy_wait; }

				inline float getScanDelta(void) const noexcept { return this->m_scandelta; }
				inline std::uint32_t getTimeDelta(void) const noexcept { return this->m_timedelta; }
				inline std::uint32_t getTimeScanDelta(void) const noexcept { return this->m_timescandelta; }

				inline std::uint32_t getIdle(void) const noexcept { return this->m_idle; }
				inline void setIdle(const std::uint32_t value) noexcept { this->m_idle = value; }

				inline std::uint32_t getScanTimeFree(void) const noexcept { return this->m_scantimefree; }
				inline void setScanTimeFree(const std::uint32_t value) noexcept { this->m_scantimefree = value; }

				inline const xr_string& getSniperAnimName(void) const noexcept { return this->m_sniper_anim_name; }
				inline void setSniperAnimName(const xr_string& animation_name) noexcept
				{
					if (animation_name.empty())
					{
						MESSAGEW("you are trying to set an empty string");
					}

					this->m_sniper_anim_name = animation_name;
				}

				inline bool isSniper(void) const noexcept { return this->m_is_sniper; }
				inline void setSniper(const bool value) noexcept { this->m_is_sniper = value; }

				inline bool isNoRetreat(void) const noexcept { return this->m_is_no_retreat; }
				inline void setNoRetreat(const bool value) noexcept { this->m_is_no_retreat = value; }

				inline const xr_string& getShootName(void) const noexcept { return this->m_shoot_name; }
				inline void setShootName(const xr_string& name) noexcept
				{
					if (name.empty())
					{
						MESSAGEW("you are trying to set an empty string");
					}

					this->m_shoot_name = name;
				}

				inline const std::pair<std::uint32_t, Fvector>& getLastLookPoint(void) const noexcept { return this->m_last_look_point; }
				inline void setLastLookPointKey(const std::uint32_t value) noexcept { this->m_last_look_point.first = value; }
				inline void setLastLookPointPos(const Fvector& position) noexcept { this->m_last_look_point.second = position; }
				inline void setLastLookPoint(const std::uint32_t value, const Fvector& position) noexcept { this->m_last_look_point.first = value; this->m_last_look_point.second = position; }


				inline std::uint32_t getWpFlag(void) const noexcept { return this->m_wp_flag; }
				inline void setWpFlag(const std::uint32_t value) noexcept
				{
					if (value >= std::uint8_t(-1))
					{
						MESSAGEWR("deprecated value!");
						return;
					}
					this->m_wp_flag = value;
				}

				inline const xr_map<xr_string, xr_string>& getSuggestedStates(void) const noexcept { return this->m_suggested_states; }
				inline void setSuggestedState(const xr_string& state_name, const xr_string& value_name) noexcept
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

					this->m_suggested_states[state_name] = value_name;
				}

				inline const xr_map<std::uint32_t, xr_vector<std::pair<std::uint32_t, Fvector>>>& getScanTable(void) const noexcept { return this->m_scan_table; }
				inline void ClearScanTable(void) noexcept { this->m_scan_table.clear(); }
				inline void setScanTable(const std::uint32_t index, const std::uint32_t value, const Fvector& position) noexcept
				{
					this->m_scan_table[index].push_back({ value, position });
				}

				inline std::uint32_t getScanBegin(void) const noexcept { return this->m_scan_begin; }
				inline void setScanBegin(const std::uint32_t value) noexcept { this->m_scan_begin = value; }

				inline std::uint32_t getCurrentLookPoint(void) const noexcept { return this->m_current_look_point; }
				inline void setCurrentLookPoint(const std::uint32_t value) noexcept { this->m_current_look_point = value; }

				inline std::uint32_t getMemoryEnemy(void) const noexcept { return this->m_memory_enemy; }
				inline void setMemoryEnemy(const std::uint32_t value) noexcept { this->m_memory_enemy = value; }

				inline void clear(void) noexcept 
				{
					this->m_is_no_retreat = false;
					this->m_is_sniper = false;
					this->m_wp_flag = 0;
					this->m_scan_begin = 0;
					this->m_current_look_point = 0;
					this->m_memory_enemy = 0;
					this->m_idle = 0;
					this->m_scantimefree = 0;
					this->m_radius = 0.0f;
					this->m_last_look_point.first = 0;
					this->m_last_look_point.second.x = 0.0f;
					this->m_last_look_point.second.y = 0.0f;
					this->m_last_look_point.second.z = 0.0f;
					this->m_scan_table.clear();
					this->m_suggested_states.clear();
					this->m_sniper_anim_name.clear();
					this->m_shoot_name.clear();
					this->m_attack_sound_name.clear();
				}

			private:
				bool m_is_sniper;
				bool m_is_no_retreat;
				std::uint32_t m_wp_flag;
				std::uint32_t m_scan_begin;
				std::uint32_t m_current_look_point;
				std::uint32_t m_memory_enemy;
				const std::uint32_t m_post_enemy_wait = 5000;
				const std::uint32_t m_timedelta = 4000;
				const std::uint32_t m_timescandelta = this->m_timedelta / this->m_scandelta;
				std::uint32_t m_idle;
				std::uint32_t m_scantimefree;
				float m_radius;
				const float m_scandelta = 30.0f;
				const float m_enemy_disp = 7 / 57.2957f;
				std::pair<std::uint32_t, Fvector> m_last_look_point;
				xr_map<std::uint32_t, xr_vector<std::pair<std::uint32_t, Fvector>>> m_scan_table;
				xr_map<xr_string, xr_string> m_suggested_states;
				xr_string m_attack_sound_name;
				xr_string m_sniper_anim_name;
				xr_string m_shoot_name;
			};
			struct Script_ComponentScheme_XRKamp
			{
				Script_ComponentScheme_XRKamp(void) : m_pos_vertex(0), m_radius(0), m_npc_position_num(0) {}

				inline std::uint32_t getPosVertex(void) const noexcept { return this->m_pos_vertex; }
				inline void setPosVertex(const std::uint32_t value) noexcept { this->m_pos_vertex = value; }

				inline std::uint32_t getRadius(void) const noexcept { return this->m_radius; }
				inline void setRadius(const std::uint32_t value) noexcept { this->m_radius = value; }

				inline const xr_string& getCenterPointName(void) const noexcept { return this->m_center_point_name; }
				inline void setCenterPointName(const xr_string& center_point_name) noexcept { this->m_center_point_name = center_point_name; }

				inline const xr_string& getDefStateMoving(void) const noexcept { return this->m_def_state_moving_name; }
				inline void setDefStateMoving(const xr_string& state_name) noexcept { this->m_def_state_moving_name = state_name; }

				inline std::uint32_t getNpcPositionNum(void) const noexcept { return this->m_npc_position_num; }
				inline void setNpcPositionNum(const std::uint32_t value) noexcept
				{
					this->m_npc_position_num = value;
				}

				inline void clear(void) noexcept 
				{
					this->m_center_point_name.clear();
					this->m_def_state_moving_name.clear();
					this->m_radius = 0;
					this->m_pos_vertex = 0;
					this->m_npc_position_num = 0;
				}
			private:
				std::uint32_t m_pos_vertex;
				std::uint32_t m_radius;
				std::uint32_t m_npc_position_num;
				xr_string m_center_point_name;
				xr_string m_def_state_moving_name;
			};
			struct Script_ComponentScheme_XRSleeper
			{
				Script_ComponentScheme_XRSleeper(void) : m_is_wakeable(false) {}

				inline bool isWakeable(void) const noexcept { return this->m_is_wakeable; }
				inline void setWakeable(const bool value) noexcept { this->m_is_wakeable = value; }

				inline const xr_string& getPathMainName(void) const noexcept { return this->m_path_main_name; }
				inline void setPathMainName(const xr_string& path_name) noexcept
				{
					if (path_name.empty())
					{
						MESSAGEWR("path_name.empty() == true!");
						return;
					}

					this->m_path_main_name = path_name;
				}

				inline const xr_string& getPathWalkName(void) const noexcept { return this->m_path_walk_name; }
				inline void setPathWalkName(const xr_string& path_name) noexcept
				{
					if (path_name.empty())
					{
						MESSAGEWR("path_name.empty() == true!");
						return;
					}

					this->m_path_walk_name = path_name;
				}

				inline const xr_string& getPathLookName(void) const noexcept { return this->m_path_look_name; }
				inline void setPathLookName(const xr_string& path_name) noexcept
				{
					if (path_name.empty())
					{
						MESSAGEWR("path_name.empty() == true!");
						return;
					}

					this->m_path_look_name = path_name;
				}

				inline void clear(void) noexcept 
				{
					this->m_is_wakeable = false;
					this->m_path_look_name.clear();
					this->m_path_main_name.clear();
					this->m_path_walk_name.clear();
				}
			private:
				bool m_is_wakeable;
				xr_string m_path_main_name;
				xr_string m_path_walk_name;
				xr_string m_path_look_name;
			};
			struct Script_ComponentScheme_XRWalker
			{
				Script_ComponentScheme_XRWalker(void) : m_is_use_camp(false) {}

				inline const xr_string& getPathWalkName(void) const noexcept { return this->m_path_walk_name; }
				inline void setPathWalkName(const xr_string& path_name) noexcept
				{
					if (path_name.empty())
					{
						MESSAGEWR("path_walk.empty() == "
							"true! You are set an empty string");
						return;
					}

					this->m_path_walk_name = path_name;
				}

				inline const xr_string& getPathLookName(void) const noexcept { return this->m_path_look_name; }
				inline void setPathLookName(const xr_string& path_name) noexcept
				{
					if (path_name.empty())
					{
						MESSAGEWR("path_name.empty() == true! You are set an empty string");
						return;
					}

					this->m_path_look_name = path_name;
				}

				inline const xr_string& getTeamName(void) const noexcept { return this->m_team_name; }
				inline void setTeamName(const xr_string& team_name) noexcept
				{
					if (team_name.empty())
					{
						MESSAGEWR("team_name.empty() == true! You are set an empty string");
						return;
					}

					this->m_team_name = team_name;
				}

				inline const xr_string& getDescriptionName(void) const noexcept
				{
					return this->m_description_name;
				}
				inline void setDescriptionName(const xr_string& description_name) noexcept
				{
					if (description_name.empty())
					{
						MESSAGEWR("description_name.empty() == true! You are set an empty string!");
						return;
					}

					this->m_description_name = description_name;
				}

				inline bool isUseCamp(void) const noexcept { return this->m_is_use_camp; }
				inline void setUseCamp(const bool is_using) noexcept { this->m_is_use_camp = is_using; }

				inline const xr_map<xr_string, xr_string>& getSuggestedStates(void) const noexcept
				{
					return this->m_suggested_states;
				}

				inline void setSuggestedStates(const xr_string& type_name, const xr_string& animation_name) noexcept
				{
					if (type_name.empty())
					{
						MESSAGEW("type_name.empty() == true! You are set an empty string!");
						return;
					}

					if (this->m_suggested_states.find(type_name) != this->m_suggested_states.end())
					{
						MESSAGEW("you are trying to change the existing value %s to %s",
							this->m_suggested_states.at(type_name).c_str(), animation_name.c_str());
					}

					this->m_suggested_states[type_name] = animation_name;
				}

				inline const xr_string& getSoundIdleName(void) const noexcept { return this->m_sound_idle_name; }
				inline void setSoundIdleName(const xr_string& sound_name) noexcept
				{
					if (sound_name.empty())
					{
						MESSAGEWR("sound_name.empty() == true!");
						return;
					}


					this->m_sound_idle_name = sound_name;
				}

				inline void clear(void) noexcept 
				{
					this->m_is_use_camp = false;
					this->m_description_name.clear();
					this->m_path_look_name.clear();
					this->m_path_walk_name.clear();
					this->m_sound_idle_name.clear();
					this->m_suggested_states.clear();
					this->m_team_name.clear();
				}
			private:
				bool m_is_use_camp;
				xr_map<xr_string, xr_string> m_suggested_states;
				xr_string m_description_name;
				xr_string m_path_walk_name;
				xr_string m_team_name;
				xr_string m_sound_idle_name;
				xr_string m_path_look_name;
			};
			struct Script_ComponentScheme_XRRemark
			{
				Script_ComponentScheme_XRRemark(void) : m_is_target_initialized(false), m_is_sound_animation_sync(false), m_target_id(0) {}

				inline const xr_string& getTargetName(void) const noexcept { return this->m_target_name; }
				inline void setTargetName(const xr_string& target_name) noexcept
				{
					if (target_name.empty())
					{
						MESSAGEWR("target_name.empty() == "
							"true! You are trying to set an empty string Return ...");
						return;
					}

					this->m_target_name = target_name;
				}

				inline bool isTargetInitialized(void) const noexcept { return this->m_is_target_initialized; }
				inline void setTargetInitialized(const bool value) noexcept
				{
					this->m_is_target_initialized = value;
				}

				inline const Fvector& getTargetPosition(void) const noexcept { return this->m_target_position; }
				inline void setTargetPosition(const Fvector& target_position) noexcept
				{
					this->m_target_position = target_position;
				}

				inline std::uint16_t getTargetID(void) const noexcept { return this->m_target_id; }
				inline void setTargetID(const std::uint16_t value) noexcept { this->m_target_id = value; }

				inline bool isSoundAnimationSync(void) const noexcept { return this->m_is_sound_animation_sync; }
				inline void setSoundAnimationSync(const bool value) noexcept
				{
					this->m_is_sound_animation_sync = value;
				}

				inline const xr_string& getSoundName(void) const noexcept { return this->m_sound_name; }
				inline void setSoundName(const xr_string& sound_name) noexcept
				{
					if (sound_name.empty())
					{
						MESSAGEWR("sound_name.empty() == "
							"true! You are set an empty string Return ...");
						return;
					}

					this->m_sound_name = sound_name;
				}

				inline const xr_string& getTipsIDName(void) const noexcept { return this->m_tips_id_name; }
				inline void setTipsIDName(const xr_string& text) noexcept
				{
					if (text.empty())
					{
						MESSAGEWR("text.empty() == true! You are "
							"set an empty string! Return ...");
						return;
					}

					this->m_tips_id_name = text;
				}

				inline const xr_string& getTipsSenderName(void) const noexcept
				{
					return this->m_tips_sender_name;
				}
				inline void setTipsSenderName(const xr_string& text) noexcept
				{
					if (text.empty())
					{
						MESSAGEWR("text.empty() == treu! You "
							"are set an empty string! Return ...");
						return;
					}

					this->m_tips_sender_name = text;
				}

				inline const xr_map<std::uint32_t, CondlistData>& getAnimationCondlist(void) const noexcept
				{
					return this->m_animation_condlist;
				}

				inline void setAnimationCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
				{
					if (condlist.empty())
					{
						MESSAGEWR("condlist.empty() == "
							"true! You are trying to set an empty condlist");
						return;
					}

					this->m_animation_condlist = condlist;
				}

				inline void clear(void) noexcept 
				{
					this->m_is_sound_animation_sync = false;
					this->m_is_target_initialized = false;
					this->m_target_id = 0;
					this->m_target_position.x = 0.0f;
					this->m_target_position.y = 0.0f;
					this->m_target_position.z = 0.0f;
					this->m_animation_condlist.clear();
					this->m_sound_name.clear();
					this->m_target_name.clear();
					this->m_tips_id_name.clear();
					this->m_tips_sender_name.clear();
				}

			private:
				bool m_is_target_initialized;
				bool m_is_sound_animation_sync;
				std::uint16_t m_target_id;
				Fvector m_target_position;
				xr_map<std::uint32_t, CondlistData> m_animation_condlist;
				xr_string m_target_name;
				xr_string m_sound_name;
				xr_string m_tips_id_name;
				xr_string m_tips_sender_name;
			};
			struct Script_ComponentScheme_XRGatherItems
			{
				Script_ComponentScheme_XRGatherItems(void) : m_is_enabled(false) {}

				inline bool isEnabled(void) const noexcept { return this->m_is_enabled; }
				inline void setEnabled(const bool value) noexcept { this->m_is_enabled = value; }

				inline void clear(void) noexcept 
				{
					this->m_is_enabled = false;
				}

			private:
				bool m_is_enabled;
			};
			struct Script_ComponentScheme_XRHelpWounded
			{
				Script_ComponentScheme_XRHelpWounded(void) : m_is_wounded_enabled(false) {}
				
				inline bool IsXRHelpWoundedHelpWoundedEnabled(void) const noexcept
				{
					return this->m_is_wounded_enabled;
				}

				inline void setXRHelpWoundedHelpWoundedEnabled(const bool value) noexcept
				{
					this->m_is_wounded_enabled = value;
				}

				inline void clear(void) noexcept 
				{
					this->m_is_wounded_enabled = false;
				}

			private:
				bool m_is_wounded_enabled;
			};
			struct Script_ComponentScheme_XRCombat
			{
				Script_ComponentScheme_XRCombat(void) {}

				inline const xr_string& getXRCombatScriptCombatTypeName(void) const noexcept
				{
					return this->m_xr_combat_script_combat_type_name;
				}

				inline void setXRCombatScriptCombatTypeName(const xr_string& type_name) noexcept
				{
					this->m_xr_combat_script_combat_type_name = type_name;
				}

				inline const xr_map<std::uint32_t, CondlistData>& getXRCombatCombatTypeCondlist(void) const noexcept
				{
					return this->m_xr_combat_combat_type_condlist;
				}

				inline void setXRCombatCombatTypeCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
				{
					this->m_xr_combat_combat_type_condlist = condlist;
				}

				inline void clear(void) noexcept 
				{
					this->m_xr_combat_combat_type_condlist.clear();
					this->m_xr_combat_script_combat_type_name.clear();
				}

			private:
				xr_map<std::uint32_t, CondlistData> m_xr_combat_combat_type_condlist;
				xr_string m_xr_combat_script_combat_type_name;
			};
			struct Script_ComponentScheme_XRCorpseDetection
			{
				Script_ComponentScheme_XRCorpseDetection(void) : m_selected_corpse_id(0) {}
				inline void clear(void) noexcept { this->m_selected_corpse_id = 0; }

				inline std::uint16_t getXRCorpseDetectionSelectedCorpseID(void) const noexcept
				{
					return this->m_selected_corpse_id;
				}
				inline void setXRCorpseDetectionSelectedCorpseID(const std::uint16_t id) noexcept
				{
					this->m_selected_corpse_id = id;
				}
			private:
				std::uint16_t m_selected_corpse_id;
			};
			struct Script_ComponentScheme_XRAbuse
			{
				Script_ComponentScheme_XRAbuse(void) : m_p_abuse_manager(nullptr) {}
				~Script_ComponentScheme_XRAbuse(void) 
				{
					if (this->m_p_abuse_manager)
					{
						MESSAGEI("deleting abuse manager!");
						xr_delete(this->m_p_abuse_manager);
					}
				}

				inline Script_XRAbuseManager* const getManager(void) const { return this->m_p_abuse_manager; }
				inline void setManager(Script_XRAbuseManager* const p_object)
				{
					// Lord: проверить все таки manager* если можно будет ресетить на новый объект при условии что оно вообще удалется только в деструкторе ...
					if (this->m_p_abuse_manager)
					{
						R_ASSERT2(false, "you can't set to existed instance you must deallocate this!");
						return;
					}

					if (!p_object)
					{
						MESSAGEWR("you are trying to set an empty "
							"object return ...");
						return;
					}

					this->m_p_abuse_manager = p_object;
				}

				inline void clear(void) 
				{
					if (this->m_p_abuse_manager)
					{
						MESSAGEI("deleting abuse manager!");
						xr_delete(this->m_p_abuse_manager);
					}
				}
			private:
				Script_XRAbuseManager* m_p_abuse_manager;
			};
			struct Script_ComponentScheme_XRDeath
			{
				Script_ComponentScheme_XRDeath(void) {}

				inline const xr_map<std::uint32_t, CondlistData>& getInfo(void) const noexcept
				{
					return this->m_xr_death_info;
				}
				inline void setInfo(const xr_map<std::uint32_t, CondlistData>& data) noexcept
				{
					this->m_xr_death_info = data;
				}

				inline const xr_map<std::uint32_t, CondlistData>& getInfo2(void) const noexcept
				{
					return this->m_xr_death_info2;
				}
				inline void setInfo2(const xr_map<std::uint32_t, CondlistData>& data) noexcept
				{
					this->m_xr_death_info2 = data;
				}

				inline void clear(void) noexcept
				{
					this->m_xr_death_info.clear();
					this->m_xr_death_info2.clear();
				}
			private:
				xr_map<std::uint32_t, CondlistData> m_xr_death_info;
				xr_map<std::uint32_t, CondlistData> m_xr_death_info2;
			};
			struct Script_ComponentScheme_SRDeimos
			{
				Script_ComponentScheme_SRDeimos(void) { this->clear(); }

				inline float getSRDeimosIntensity(void) const noexcept { return this->m_intensity; }
				inline void setSRDeimosIntensity(const float value) noexcept { this->m_intensity = value; }

				inline float getSRDeimosMovementSpeed(void) const noexcept { return this->m_movement_speed; }
				inline void setSRDeimosMovementSpeed(const float value) noexcept { this->m_movement_speed = value; }

				inline float getSRDeimosGrowingKoef(void) const noexcept { return this->m_growing_koef; }
				inline void setSRDeimosGrowingKoef(const float value) noexcept { this->m_growing_koef = value; }

				inline float getSRDeimosLoweringKoef(void) const noexcept { return this->m_lowering_koef; }
				inline void setSRDeimosLoweringKoef(const float value) noexcept { this->m_lowering_koef = value; }

				inline std::uint32_t getSRDeimosCameraEffectorRepeatingTime(void) const noexcept
				{
					return this->m_camera_effector_repeating_time;
				}
				inline void setSRDeimosCameraEffectorRepeatingTime(const std::uint32_t value) noexcept
				{
					this->m_camera_effector_repeating_time = value;
				}

				inline float getSRDeimosHealthLost(void) const noexcept { return this->m_health_lost; }
				inline void setSRDeimosHealthLost(const float value) noexcept { this->m_health_lost = value; }

				inline float getSRDeimosDisableBound(void) const noexcept { return this->m_disable_bound; }
				inline void setSRDeimosDisableBound(const float value) noexcept { this->m_disable_bound = value; }

				inline float getSRDeimosSwitchLowerBound(void) const noexcept { return this->m_switch_lower_bound; }
				inline void setSRDeimosSwitchLowerBound(const float value) noexcept
				{
					this->m_switch_lower_bound = value;
				}

				inline float getSRDeimosSwitchUpperBound(void) const noexcept { return this->m_switch_upper_bound; }
				inline void setSRDeimosSwitchUpperBound(const float value) noexcept
				{
					this->m_switch_upper_bound = value;
				}

				inline const xr_string& getSRDeimosPostProcessEffectorName(void) const noexcept
				{
					return this->m_postprocess_effector_name;
				}
				inline void setSRDeimosPostProcessEffectorName(const xr_string& effector_name) noexcept
				{
					if (effector_name.empty())
					{
						MESSAGEW("effector_name.empty() == true! You set an empty string");
					}

					this->m_postprocess_effector_name = effector_name;
				}

				inline const xr_string& getSRDeimosCameraEffectorName(void) const noexcept
				{
					return this->m_camera_effector_name;
				}
				inline void setSRDeimosCameraEffectorName(const xr_string& effector_name) noexcept
				{
					if (effector_name.empty())
					{
						MESSAGEW("effector_name.empty() == true! You set an empty string");
					}

					this->m_camera_effector_name = effector_name;
				}

				inline const xr_string& getSRDeimosPostProcessEffector2Name(void) const noexcept
				{
					return this->m_postprocess_effector2_name;
				}
				inline void setSRDeimosPostProcessEffector2Name(const xr_string& effector_name) noexcept
				{
					if (effector_name.empty())
					{
						MESSAGEW("effector_name.empty() == true! You set an empty string");
					}

					this->m_postprocess_effector2_name = effector_name;
				}

				inline const xr_string& getSRDeimosNoiseSoundName(void) const noexcept
				{
					return this->m_noise_sound_name;
				}
				inline void setSRDeimosNoiseSoundName(const xr_string& sound_name) noexcept
				{
					if (sound_name.empty())
					{
						MESSAGEW("sound_name.empty() "
							"== true! You set an empty string");
					}

					this->m_noise_sound_name = sound_name;
				}

				inline const xr_string& getSRDeimosHeartBeetSoundName(void) const noexcept
				{
					return this->m_heartbeet_sound_name;
				}
				inline void setSRDeimosHeartBeetSoundName(const xr_string& sound_name) noexcept
				{
					if (sound_name.empty())
					{
						MESSAGEW("sound_name.empty() == true! You set an empty string");
					}

					this->m_heartbeet_sound_name = sound_name;
				}

				inline void clear(void) noexcept 
				{
					this->m_camera_effector_name.clear();
					this->m_camera_effector_repeating_time = 0;
					this->m_intensity = 0.0f;
					this->m_disable_bound = 0.0f;
					this->m_growing_speed = 0.0f;
					this->m_growing_koef = 0.0f;
					this->m_lowering_koef = 0.0f;
					this->m_switch_lower_bound = 0.0f;
					this->m_switch_upper_bound = 0.0f;
					this->m_health_lost = 0.0f;
					this->m_movement_speed = 0.0f;
					this->m_postprocess_effector_name.clear();
					this->m_postprocess_effector2_name.clear();
					this->m_heartbeet_sound_name.clear();
					this->m_noise_sound_name.clear();
				}

			private:
				std::uint32_t m_camera_effector_repeating_time;
				float m_intensity;
				float m_growing_speed;
				float m_growing_koef;
				float m_lowering_koef;
				float m_disable_bound;
				float m_switch_lower_bound;
				float m_switch_upper_bound;
				float m_health_lost;
				float m_movement_speed;
				xr_string m_postprocess_effector_name;
				xr_string m_camera_effector_name;
				xr_string m_postprocess_effector2_name;
				xr_string m_noise_sound_name;
				xr_string m_heartbeet_sound_name;
			};
			struct Script_ComponentScheme_SRLight
			{
				Script_ComponentScheme_SRLight(void) : m_is_light(false) {}

				inline bool IsSRLightLight(void) const noexcept { return this->m_is_light; }
				inline void setSRLightLight(const bool value) noexcept { this->m_is_light = value; }

				inline void clear(void) noexcept 
				{
					this->m_is_light = false;
				}
			private:
				bool m_is_light;
			};
			struct Script_ComponentScheme_SRParticle 
			{
				Script_ComponentScheme_SRParticle(void) : m_is_looped(false), m_mode(0) {}

				inline bool IsSRParticleLooped(void) const noexcept { return this->m_is_looped; }
				inline void setSRParticleLooped(const bool value) noexcept { this->m_is_looped = value; }

				inline std::uint32_t getSRParticleMode(void) const noexcept { return this->m_mode; }
				inline void setSRParticleMode(const std::uint32_t value) noexcept { this->m_mode = value; }

				inline const xr_string& getSRParticleName(void) const noexcept { return this->m_name; }
				inline void setSRParticleName(const xr_string& name) noexcept
				{
					if (name.empty())
					{
						MESSAGEW("name.empty() == true! You set an "
							"empty string");
					}

					this->m_name = name;
				}

				inline const xr_string& getSRParticlePathName(void) const noexcept { return this->m_path_name; }
				inline void setSRParticlePathName(const xr_string& path_name) noexcept
				{
					if (path_name.empty())
					{
						MESSAGEW("path_name.empty() == "
							"true! You set an empty string");
					}

					this->m_path_name = path_name;
				}

				inline void clear(void) noexcept 
				{
					this->m_is_looped = false;
					this->m_mode = 0;
					this->m_name.clear();
					this->m_path_name.clear();
				}

			private:
				bool m_is_looped;
				std::uint32_t m_mode;
				xr_string m_name;
				xr_string m_path_name;
			};
			struct Script_ComponentScheme_SRPostProcess 
			{
				Script_ComponentScheme_SRPostProcess(void) : m_hit_intensity(0.0f), m_intensity(0.0f), m_intensity_speed(0.0f) {}

				inline void clear(void) noexcept 
				{
					this->m_hit_intensity = 0.0f;
					this->m_intensity_speed = 0.0f;
					this->m_intensity = 0.0f;
				}

				inline float getSRPostProcessIntensity(void) const noexcept { return this->m_intensity; }
				inline void setSRPostProcessIntensity(const float value) noexcept { this->m_intensity = value; }

				inline float getSRPostProcessIntensitySpeed(void) const noexcept { return this->m_intensity_speed; }
				inline void setSRPostProcessIntensitySpeed(const float value) noexcept
				{
					this->m_intensity_speed = value;
				}

				inline float getSRPostProcessHitIntensity(void) const noexcept { return this->m_hit_intensity; }
				inline void setSRPostProcessHitIntensity(const float value) noexcept
				{
					this->m_hit_intensity = value;
				}

			private:
				float m_intensity;
				float m_intensity_speed;
				float m_hit_intensity;
			};
			struct Script_ComponentScheme_SRPsyAntenna
			{
				Script_ComponentScheme_SRPsyAntenna(void) { this->clear(); }

				inline void clear(void) noexcept 
				{
					this->m_is_sr_psy_antenna_no_static = false;
					this->m_is_sr_psy_antenna_no_mumble = false;
					this->m_sr_psy_antenna_intensity = 0.0f;
					this->m_sr_psy_antenna_hit_intensity = 0.0f;
					this->m_sr_psy_antenna_phantom_probability = 0.0f;
					this->m_sr_psy_antenna_mute_sound_threshold = 0.0f;
					this->m_sr_psy_antenna_hit_frequency = 0.0f;
					this->m_sr_psy_antenna_hit_type_name.clear();
					this->m_sr_psy_antenna_postprocess_name.clear();
				}

				inline float getSRPsyAntennaIntensity(void) const noexcept { return this->m_sr_psy_antenna_intensity; }
				inline void setSRPsyAntennaIntensity(const float value) noexcept { this->m_sr_psy_antenna_intensity = value; }

				inline float getSRPsyAntennaHitIntensity(void) const noexcept { return this->m_sr_psy_antenna_hit_intensity; }
				inline void setSRPsyAntennaHitIntensity(const float value) noexcept
				{
					this->m_sr_psy_antenna_hit_intensity = value;
				}

				inline float getSRPsyAntennaPhantomProbability(void) const noexcept
				{
					return this->m_sr_psy_antenna_phantom_probability;
				}
				inline void setSRPsyAntennaPhantomProbability(const float value) noexcept
				{
					this->m_sr_psy_antenna_phantom_probability = value;
				}

				inline float getSRPsyAntennaMuteSoundThreshold(void) const noexcept
				{
					return this->m_sr_psy_antenna_mute_sound_threshold;
				}

				inline void setSRPsyAntennaMuteSoundThreshold(const float value) noexcept
				{
					this->m_sr_psy_antenna_mute_sound_threshold = value;
				}

				inline float getSRPsyAntennaHitFrequency(void) const noexcept { return this->m_sr_psy_antenna_hit_frequency; }
				inline void setSRPsyAntennaHitFrequency(const float value) noexcept
				{
					this->m_sr_psy_antenna_hit_frequency = value;
				}

				inline bool IsSRPsyAntennaNoStatic(void) const noexcept { return this->m_is_sr_psy_antenna_no_static; }
				inline void setSRPsyAntennaNoStatic(const bool value) noexcept { this->m_is_sr_psy_antenna_no_static = value; }

				inline bool IsSRPsyAntennaNoMumble(void) const noexcept { return this->m_is_sr_psy_antenna_no_mumble; }
				inline void setSRPsyAntennaNoMumble(const bool value) noexcept { this->m_is_sr_psy_antenna_no_mumble = value; }

				inline const xr_string& getSRPsyAntennaPostProcessName(void) const noexcept
				{
					return this->m_sr_psy_antenna_postprocess_name;
				}
				inline void setSRPsyAntennaPostProcessName(const xr_string& postprocess_name) noexcept
				{
					if (postprocess_name.empty())
					{
						MESSAGEW("postprocess_name.empty() == true! You set an empty string");
					}

					this->m_sr_psy_antenna_postprocess_name = postprocess_name;
				}

				inline const xr_string& getSRPsyAntennaHitTypeName(void) const noexcept
				{
					return this->m_sr_psy_antenna_hit_type_name;
				}
				inline void setSRPsyAntennaHitTypeName(const xr_string& hit_type_name) noexcept
				{
					if (hit_type_name.empty())
					{
						MESSAGEW("hit_type_name.empty() == true! You set an empty string");
					}

					this->m_sr_psy_antenna_hit_type_name = hit_type_name;
				}

			private:
				bool m_is_sr_psy_antenna_no_static;
				bool m_is_sr_psy_antenna_no_mumble;
				float m_sr_psy_antenna_intensity;
				float m_sr_psy_antenna_hit_intensity;
				float m_sr_psy_antenna_phantom_probability;
				float m_sr_psy_antenna_mute_sound_threshold;
				float m_sr_psy_antenna_hit_frequency;
				xr_string m_sr_psy_antenna_postprocess_name;
				xr_string m_sr_psy_antenna_hit_type_name;
			};
			struct Script_ComponentScheme_SRTeleport
			{
				Script_ComponentScheme_SRTeleport(void) : m_timeout(0) {}

				inline std::uint32_t getTimeout(void) const noexcept { return this->m_timeout; }
				inline void setTimeout(const std::uint32_t value) noexcept { this->m_timeout = value; }

				inline const xr_vector<std::pair<std::uint32_t, std::pair<xr_string, xr_string>>>& getPoints(void) const
					noexcept
				{
					return this->m_points;
				}

				inline void setPoints(
					const xr_vector<std::pair<std::uint32_t, std::pair<xr_string, xr_string>>>& data) noexcept
				{
					this->m_points = data;
				}

				inline void clear(void) noexcept 
				{
					this->m_points.clear();
					this->m_timeout = 0;
				}

			private:
				std::uint32_t m_timeout;
				xr_vector<std::pair<std::uint32_t, std::pair<xr_string, xr_string>>> m_points;
			};
			struct Script_ComponentScheme_SRTimer 
			{
				Script_ComponentScheme_SRTimer(void) : m_p_sr_timer_ui(nullptr), m_p_sr_timer_timer(nullptr), m_sr_timer_start_value(0){}

				inline const xr_string& getSRTimerTypeName(void) const noexcept { return this->m_sr_timer_type_name; }
				inline void setSRTimerTypeName(const xr_string& timer_type_name) noexcept
				{
					if (timer_type_name.empty())
					{
						MESSAGEW("timer_type_name.empty() == true! You set an empty string");
					}

					this->m_sr_timer_type_name = timer_type_name;
				}

				inline const xr_string& getSRTimerTimerIDName(void) const noexcept { return this->m_sr_timer_timer_id_name; }
				inline void setSRTimerTimerIDName(const xr_string& timer_id_name) noexcept
				{
					if (timer_id_name.empty())
					{
						MESSAGEW("timer_id_name.empty() "
							"== true! You set an empty string");
					}

					this->m_sr_timer_timer_id_name = timer_id_name;
				}

				inline const xr_string& getSRTimerStringName(void) const noexcept { return this->m_sr_timer_string_name; }
				inline void setSRTimerStringName(const xr_string& string_name) noexcept
				{
					if (string_name.empty())
					{
						MESSAGEW("string_name.empty() == "
							"true! You set an empty string");
					}

					this->m_sr_timer_string_name = string_name;
				}

				inline std::uint32_t getSRTimerStartValue(void) const noexcept { return this->m_sr_timer_start_value; }
				inline void setSRTimerStartValue(const std::uint32_t value) noexcept { this->m_sr_timer_start_value = value; }

				inline CUIGameCustom* const getSRTimerUI(void) const { return this->m_p_sr_timer_ui; }
				inline void setSRTimerUI(CUIGameCustom* const p_ui)
				{
					if (!p_ui)
					{
						MESSAGEW("p_ui == nullptr! You set an empty "
							"object");
					}

					this->m_p_sr_timer_ui = p_ui;
				}

				inline CUIStatic* const getSRTimerTimer(void) const { return this->m_p_sr_timer_timer; }
				inline void setSRTimerTimer(CUIStatic* const p_static)
				{
					if (!p_static)
					{
						MESSAGEW("p_static == nullptr! You set an "
							"empty object");
					}

					this->m_p_sr_timer_timer = p_static;
				}

				inline const xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>>& getSRTimerOnValue(void) const noexcept
				{
					return this->m_sr_timer_on_value;
				}
				inline void setSRTimerOnValue(const xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>>& data) noexcept
				{
					this->m_sr_timer_on_value = data;
				}

				inline void clear(void) noexcept 
				{
					this->m_sr_timer_start_value = 0;
					this->m_p_sr_timer_timer = nullptr;
					this->m_p_sr_timer_ui = nullptr;
					this->m_sr_timer_string_name.clear();
					this->m_sr_timer_timer_id_name.clear();
					this->m_sr_timer_type_name.clear();
				}
				
			private:
				std::uint32_t m_sr_timer_start_value;
				CUIGameCustom* m_p_sr_timer_ui;
				CUIStatic* m_p_sr_timer_timer;
				xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>> m_sr_timer_on_value;
				xr_string m_sr_timer_type_name;
				xr_string m_sr_timer_timer_id_name;
				xr_string m_sr_timer_string_name;
			};
			struct Script_ComponentScheme_PHSound
			{
				Script_ComponentScheme_PHSound(void) : m_is_no_hit(false), m_is_looped(false), m_is_random(false), m_pause_min(0), m_pause_max(0) {}

				inline bool IsLooped(void) const noexcept { return this->m_is_looped; }
				inline void setLooped(const bool value) noexcept { this->m_is_looped = value; }

				inline bool IsRandom(void) const noexcept { return this->m_is_random; }
				inline void setRandom(const bool value) noexcept { this->m_is_random = value; }

				inline bool IsNoHit(void) const noexcept { return this->m_is_no_hit; }
				inline void setNoHit(const bool value) noexcept { this->m_is_no_hit = value; }

				inline const xr_string& getThemeName(void) const noexcept { return this->m_theme_name; }
				inline void setThemeName(const xr_string& theme_name) noexcept
				{
					if (theme_name.empty())
					{
						MESSAGEW("theme_name.empty() == "
							"true! You set an empty string");
					}

					this->m_theme_name = theme_name;
				}

				inline std::uint32_t getPauseMin(void) const noexcept { return this->m_pause_min; }
				inline void setPauseMin(const std::uint32_t value) noexcept { this->m_pause_min = value; }

				inline std::uint32_t getPauseMax(void) const noexcept { return this->m_pause_max; }
				inline void setPauseMax(const std::uint32_t value) noexcept { this->m_pause_max = value; }

				inline void clear(void) noexcept 
				{
					this->m_is_random = false;
					this->m_is_no_hit = false;
					this->m_is_looped = false;
					this->m_pause_min = 0;
					this->m_pause_max = 0;
					this->m_theme_name.clear();
				}

			private:
				bool m_is_no_hit;
				bool m_is_looped;
				bool m_is_random;
				std::uint32_t m_pause_min;
				std::uint32_t m_pause_max;
				xr_string m_theme_name;
			};
			struct Script_ComponentScheme_PHOscillate
			{
				Script_ComponentScheme_PHOscillate(void) : m_period(0) {}

				inline const xr_string& getJointName(void) const noexcept { return this->m_joint_name; }
				inline void setJointName(const xr_string& joint_name) noexcept
				{
					if (joint_name.empty())
					{
						MESSAGEW("joint_name.empty() == "
							"true! You set an empty string!");
					}

					this->m_joint_name = joint_name;
				}

				inline std::uint32_t getPeriod(void) const noexcept { return this->m_period; }
				inline void setPeriod(const std::uint32_t value) noexcept { this->m_period = value; }

				inline void clear(void) noexcept 
				{
					this->m_period = 0;
					this->m_joint_name.clear();
				}

			private:
				std::uint32_t m_period;
				xr_string m_joint_name;
			};
			struct Script_ComponentScheme_PHIdle
			{
				Script_ComponentScheme_PHIdle(void) : m_is_nonscript_usable(false) {}

				inline bool IsNonScriptUsable(void) const noexcept { return this->m_is_nonscript_usable; }
				inline void setNonScriptUsable(const bool value) noexcept { this->m_is_nonscript_usable = value; }

				inline void clear(void) noexcept 
				{
					this->m_is_nonscript_usable = false;
				}

			private:
				bool m_is_nonscript_usable;
			};
			struct Script_ComponentScheme_PHHit
			{
				Script_ComponentScheme_PHHit(void) : m_power(0.0f), m_impulse(0.0f) {}

				inline void clear(void) noexcept 
				{
					this->m_bone_name.clear();
					this->m_direction_path_name.clear();
					this->m_power = 0.0f;
					this->m_impulse = 0.0f;
				}

				inline float getPower(void) const noexcept { return this->m_power; }
				inline void setPower(const float value) noexcept { this->m_power = value; }

				inline float getImpulse(void) const noexcept { return this->m_impulse; }
				inline void setImpulse(const float value) noexcept { this->m_impulse = value; }

				inline const xr_string& getBoneName(void) const noexcept { return this->m_bone_name; }
				inline void setBoneName(const xr_string& bone_name) noexcept
				{
					if (bone_name.empty())
					{
						MESSAGEW("bone_name.empty() == true! You "
							"set "
							"an empty string");
					}

					this->m_bone_name = bone_name;
				}

				inline const xr_string& getDirectionPathName(void) const noexcept
				{
					return this->m_direction_path_name;
				}
				inline void setDirectionPathName(const xr_string& path_name) noexcept
				{
					if (path_name.empty())
					{
						MESSAGEW("path_name.empty() == "
							"true! You set an empty string!");
					}

					this->m_direction_path_name = path_name;
				}

			private:
				float m_power;
				float m_impulse;
				xr_string m_bone_name;
				xr_string m_direction_path_name;
			};
			struct Script_ComponentScheme_PHDoor
			{
				Script_ComponentScheme_PHDoor(void) { this->clear(); }

				inline Script_SchemePHDoor* getDoorAction(void) { return this->m_p_door_action; }
				inline void setDoorAction(Script_SchemePHDoor* p_action) { this->m_p_door_action = p_action; }

				inline bool IsClosed(void) const noexcept { return this->m_is_closed; }
				inline void setClosed(const bool value) noexcept { this->m_is_closed = value; }

				inline bool IsLocked(void) const noexcept { return this->m_is_locked; }
				inline void setLocked(const bool value) noexcept { this->m_is_locked = value; }

				inline bool IsNoForce(void) const noexcept { return this->m_is_no_force; }
				inline void setNoForce(const bool value) noexcept { this->m_is_no_force = value; }

				inline bool IsNotForNpc(void) const noexcept { return this->m_is_not_for_npc; }
				inline void setNotForNpc(const bool value) noexcept { this->m_is_not_for_npc = value; }

				inline bool IsShowTips(void) const noexcept { return this->m_is_show_tips; }
				inline void setShowTips(const bool value) noexcept { this->m_is_show_tips = value; }

				inline const xr_string& getTipOpenName(void) const noexcept { return this->m_tip_open_name; }
				inline void setTipOpenName(const xr_string& tip_name) noexcept
				{
					if (tip_name.empty())
					{
						MESSAGEW("tip_name.empty() == true! "
							"You set an "
							"empty string!");
					}

					this->m_tip_open_name = tip_name;
				}

				inline const xr_string& getTipUnlockName(void) const noexcept { return this->m_tip_unlock_name; }
				inline void setTipUnlockName(const xr_string& tip_name) noexcept
				{
					if (tip_name.empty())
					{
						MESSAGEW("tip_name.empty() == true! "
							"You set an empty string!");
					}

					this->m_tip_unlock_name = tip_name;
				}

				inline const xr_string& getTipCloseName(void) const noexcept { return this->m_tip_close_name; }
				inline void setTipCloseName(const xr_string& tip_name) noexcept
				{
					if (tip_name.empty())
					{
						MESSAGEW("tip_name.empty() == true! "
							"You set an empty string!");
					}

					this->m_tip_close_name = tip_name;
				}

				inline bool IsSlider(void) const noexcept { return this->m_is_slider; }
				inline void setSlider(const bool value) noexcept { this->m_is_slider = value; }

				inline const xr_string& getSoundOpenStartName(void) const noexcept
				{
					return this->m_sound_open_start_name;
				}
				inline void setSoundOpenStartName(const xr_string& sound_name) noexcept
				{
					if (sound_name.empty())
					{
						MESSAGEW("sound_name.empty() "
							"== true! You set an empty string!");
					}

					this->m_sound_open_start_name = sound_name;
				}

				inline const xr_string& getSoundCloseStartName(void) const noexcept
				{
					return this->m_sound_close_start_name;
				}
				inline void setSoundCloseStartName(const xr_string& sound_name) noexcept
				{
					if (sound_name.empty())
					{
						MESSAGEW("sound_name.empty() == true! You set an empty string!");
					}

					this->m_sound_close_start_name = sound_name;
				}

				inline const xr_string& getSoundCloseStopName(void) const noexcept
				{
					return this->m_sound_close_stop_name;
				}
				inline void setSoundCloseStopName(const xr_string& sound_name) noexcept
				{
					if (sound_name.empty())
					{
						MESSAGEW("sound_name.empty() "
							"== true! You set an empty string!");
					}

					this->m_sound_close_stop_name = sound_name;
				}

				inline bool IsScriptUsedMoreThanOnce(void) const noexcept
				{
					return this->m_is_script_used_more_than_once;
				}
				inline void setScriptUsedMoreThanOnce(const bool value) noexcept
				{
					this->m_is_script_used_more_than_once = value;
				}

				inline void clear(void) noexcept 
				{
					this->m_is_closed = false;
					this->m_is_locked = false;
					this->m_is_no_force = false;
					this->m_is_not_for_npc = false;
					this->m_is_show_tips = false;
					this->m_is_slider = false;
					this->m_is_script_used_more_than_once = false;
					this->m_p_door_action = nullptr;
					this->m_sound_close_start_name.clear();
					this->m_sound_close_stop_name.clear();
					this->m_sound_open_start_name.clear();
					this->m_tip_unlock_name.clear();
					this->m_tip_close_name.clear();
					this->m_tip_open_name.clear();
				}


			private:
				bool m_is_closed;
				bool m_is_locked;
				bool m_is_no_force;
				bool m_is_not_for_npc;
				bool m_is_show_tips;
				bool m_is_slider;
				bool m_is_script_used_more_than_once;
				Script_SchemePHDoor* m_p_door_action; // @ Используется исключительно когда у нас схема -> ph_door, удаляется как обычный зарегистрированный  action, но сам pointer зануляется в деструкторе схемы!
				xr_string m_tip_open_name;
				xr_string m_tip_unlock_name;
				xr_string m_tip_close_name;
				xr_string m_sound_open_start_name;
				xr_string m_sound_close_start_name;
				xr_string m_sound_close_stop_name;
			};
			struct Script_ComponentScheme_PHCode
			{
				Script_ComponentScheme_PHCode(void) : m_code(0) {}

				inline void clear(void) noexcept 
				{
					this->m_code = 0;
					this->m_on_code_condlist.clear();
					this->m_on_check_code.clear();
					this->m_tip_name.clear();
				}

				inline const xr_string& getTipName(void) const noexcept { return this->m_tip_name; }
				inline void setTipName(const xr_string& text_name) noexcept { this->m_tip_name = text_name; }

				inline std::uint32_t getCode(void) const noexcept { return this->m_code; }
				inline void setCode(const std::uint32_t value) noexcept { this->m_code = value; }

				inline const xr_map<std::uint32_t, CondlistData>& getOnCodeCondlist(void) const noexcept
				{
					return this->m_on_code_condlist;
				}
				inline void setOnCodeCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
				{
					this->m_on_code_condlist = condlist;
				}

				inline const xr_map<xr_string, xr_map<std::uint32_t, CondlistData>>& getOnCheckCode(void) const noexcept
				{
					return this->m_on_check_code;
				}

				inline void setOnCheckCode(const xr_map<xr_string, xr_map<std::uint32_t, CondlistData>>& data) noexcept
				{
					this->m_on_check_code = data;
				}

			private:
				std::uint32_t m_code;
				xr_map<std::uint32_t, CondlistData> m_on_code_condlist;
				xr_map<xr_string, xr_map<std::uint32_t, CondlistData>> m_on_check_code;
				xr_string m_tip_name;
			};
			struct Script_ComponentScheme_PHForce
			{
				Script_ComponentScheme_PHForce(void) : m_time(0), m_delay(0) {}

				inline void clear(void) noexcept 
				{
					this->m_time = 0;
					this->m_delay = 0;
					this->m_point.x = 0.0f;
					this->m_point.y = 0.0f;
					this->m_point.z = 0.0f;
				}

				inline std::uint32_t getTime(void) const noexcept { return this->m_time; }
				inline void setTime(const std::uint32_t value) noexcept { this->m_time = value; }

				inline std::uint32_t getDelay(void) const noexcept { return this->m_delay; }
				inline void setDelay(const std::uint32_t value) noexcept { this->m_delay = value; }

				inline const Fvector& getPoint(void) const noexcept { return this->m_point; }
				inline void setPoint(const Fvector& point) noexcept { this->m_point = point; }

			private:
				std::uint32_t m_time;
				std::uint32_t m_delay;
				Fvector m_point;
			};
			struct Script_ComponentScheme_Helicopter
			{
				Script_ComponentScheme_Helicopter(void) { this->clear(); }

				inline const xr_string& getPathMoveName(void) const noexcept { return this->m_path_move_name; }
				inline void setPathMoveName(const xr_string& path_name) noexcept
				{
					if (path_name.empty())
					{
						MESSAGEW("path_name.empty() == "
							"true! You set an empty string");
					}

					this->m_path_move_name = path_name;
				}

				inline const xr_string& getPathLookName(void) const noexcept { return this->m_path_look_name; }
				inline void setPathLookName(const xr_string& path_name) noexcept
				{
					if (path_name.empty())
					{
						MESSAGEW("path_name.empty() == "
							"true! You set an empty string");
					}

					this->m_path_look_name = path_name;
				}

				inline const xr_string& getEnemyName(void) const noexcept { return this->m_enemy_name; }
				inline void setEnemyName(const xr_string& enemy_name) noexcept
				{
					if (enemy_name.empty())
					{
						MESSAGEW("enemy_name.empty() == "
							"true! You set an empty string");
					}

					this->m_enemy_name = enemy_name;
				}

				inline const xr_string& getFirePointName(void) const noexcept
				{
					return this->m_fire_point_name;
				}
				inline void setFirePointName(const xr_string& fire_point_name) noexcept
				{
					if (fire_point_name.empty())
					{
						MESSAGEW("fire_point_name.empty() == true! You set an empty string");
					}

					this->m_fire_point_name = fire_point_name;
				}

				inline bool IsUseRocket(void) const noexcept { return this->m_is_use_rocket; }
				inline void setUseRocket(const bool value) noexcept { this->m_is_use_rocket = value; }

				inline bool IsUseMinigun(void) const noexcept { return this->m_is_use_minigun; }
				inline void setUseMinigun(const bool value) noexcept { this->m_is_use_minigun = value; }

				inline bool IsFireTrail(void) const noexcept { return this->m_is_fire_trail; }
				inline void setFireTrail(const bool value) noexcept { this->m_is_fire_trail = value; }

				inline bool IsEngineSound(void) const noexcept { return this->m_is_engine_sound; }
				inline void setEngineSound(const bool value) noexcept { this->m_is_engine_sound = value; }

				inline bool IsShowHealth(void) const noexcept { return this->m_is_show_health; }
				inline void setShowHealth(const bool value) noexcept { this->m_is_show_health = value; }

				inline std::uint32_t getUpdVis(void) const noexcept { return this->m_upd_vis; }
				inline void setUpdVis(const std::uint32_t value) noexcept { this->m_upd_vis = value; }

				inline float getMaxRocketDistance(void) const noexcept { return this->m_max_rocket_distance; }
				inline void setMaxRocketDistance(const float value) noexcept
				{
					this->m_max_rocket_distance = value;
				}

				inline float getMaxMinigunDistance(void) const noexcept
				{
					return this->m_max_minigun_distance;
				}
				inline void setMaxMinigunDistance(const float value) noexcept
				{
					this->m_max_minigun_distance = value;
				}

				inline float getMinRocketDistance(void) const noexcept { return this->m_min_rocket_distance; }
				inline void setMinRocketDistance(const float value) noexcept
				{
					this->m_min_rocket_distance = value;
				}

				inline float getMinMinigunDistance(void) const noexcept
				{
					return this->m_min_minigun_distance;
				}
				inline void setMinMinigunDistance(const float value) noexcept
				{
					this->m_min_minigun_distance = value;
				}

				inline float getVelocity(void) const noexcept { return this->m_velocity; }
				inline void setVelocity(const float value) noexcept { this->m_velocity = value; }

				inline bool IsStopFire(void) const noexcept { return this->m_is_stop_fire; }
				inline void setStopFire(const bool value) noexcept { this->m_is_stop_fire = value; }

				inline void clear(void) noexcept 
				{
					this->m_is_use_rocket = false;
					this->m_is_show_health = false;
					this->m_is_fire_trail = false;
					this->m_is_engine_sound = false;
					this->m_is_use_minigun = false;
					this->m_is_stop_fire = false;
					this->m_upd_vis = 0;
					this->m_min_rocket_distance = 0.0f;
					this->m_min_minigun_distance = 0.0f;
					this->m_max_rocket_distance = 0.0f;
					this->m_max_minigun_distance = 0.0f;
					this->m_velocity = 0.0f;
					this->m_path_move_name.clear();
					this->m_path_look_name.clear();
					this->m_enemy_name.clear();
					this->m_fire_point_name.clear();
				}

			private:
				bool m_is_use_rocket;
				bool m_is_show_health;
				bool m_is_fire_trail;
				bool m_is_engine_sound;
				bool m_is_use_minigun;
				bool m_is_stop_fire;
				std::uint32_t m_upd_vis;
				float m_min_rocket_distance;
				float m_min_minigun_distance;
				float m_max_rocket_distance;
				float m_max_minigun_distance;
				float m_velocity;
				xr_string m_path_move_name;
				xr_string m_path_look_name;
				xr_string m_enemy_name;
				xr_string m_fire_point_name;
			};
		}
	}
}

#include "Script_ComponentSchemes_Allocators.h"