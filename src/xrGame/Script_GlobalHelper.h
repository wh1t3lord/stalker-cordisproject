#pragma once

#include "object_factory.h"

namespace Cordis
{
	namespace Scripts
	{
		struct GlobalHelper
		{
		private:
			GlobalHelper(void) = default;

		public:
			inline static GlobalHelper& getInstance(void) noexcept
			{
				static GlobalHelper instance;
				return instance;
			}

			~GlobalHelper(void) {}
			GlobalHelper(const GlobalHelper&) = delete;
			GlobalHelper& operator=(const GlobalHelper&) = delete;
			GlobalHelper(GlobalHelper&&) = delete;
			GlobalHelper& operator=(GlobalHelper&&) = delete;


			inline void InitializeAlifeSimulator(void)
			{
				
			}


			xr_map<xr_string, bool> m_monster_classes;
			xr_map<xr_string, bool> m_stalker_classes;
			xr_map<xr_string, bool> m_weapon_classes;
			xr_map<xr_string, bool> m_artefact_classes;
			xr_map<xr_string, bool> m_ammo_section;
			xr_map<xr_string, bool> m_quest_section;
			xr_map<xr_string, xr_string> m_squad_community_by_behavior;
		};
		 
	}
}