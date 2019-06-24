#pragma once

#include "SDK_FileSystemState.h"
#include "xrCore/Cordis/XML/cordis_xml.h"
#include <filesystem>

namespace Cordis
{
	namespace SDK
	{
		const char xml_data_default_english[] = "<?xml version=\"1.0\" ?>\
<string_table>\
	<!-- System information -->\
	<string id=\"st_language\">Integrated English</string>\
	<!-- System information -->\
	<!-- Main data -->\
	<string id=\"st_direction\">Direction</string>\
	<string id=\"st_position\">Position</string>\
	<string id=\"st_scale\">Scale</string>\
	<string id=\"st_altitude\">Altitude</string>\
	<string id=\"st_magnitude\">Magnitude</string>\
	<string id=\"st_\"></string>\
	<string id=\"st_x\">X</string>\
	<string id=\"st_y\">Y</string>\
	<string id=\"st_z\">Z</string>\
	<string id=\"st_warning\">Warning</string>\
	<string id=\"st_error\">Error</string>\
	<!-- Main data -->\
	<!-- Camera Manager -->\
	<string id=\"st_manager_show_camera\">Show camera manager</string>\
	<string id=\"st_label_manager_camera\">Camera manager</string>\
	<!-- Camera Manager -->\
	<!-- Log Manager -->\
	<string id=\"st_manager_show_log\">Show log</string>\
	<string id=\"st_label_manager_log\">Log manager</string>\
	<!-- Log Manager -->\
	<!-- Weather Manager -->\
	<string id=\"st_manager_show_weather\">Show weather manager</string>\
	<!-- Weather Manager -->\
	<!-- Right Window -->\
	<string id=\"st_name_staticgeometry\">Static geometry</string>\
	<string id=\"st_name_groups\">Groups</string>\
	<string id=\"st_name_light\">Lights</string>\
	<string id=\"st_name_staticparticles\">Static particles</string>\
	<string id=\"st_name_waypoints\">Way points</string>\
	<string id=\"st_name_sound_environment\">Sound environments</string>\
	<string id=\"st_name_ainodes\">AI-Nodes</string>\
	<string id=\"st_name_spawnelements\">Spawn elements</string>\
	<!-- Right Window -->\
	<!-- Overlay -->\
	<string id=\"st_grid\">Grid</string>\
	<string id=\"st_background_color\">Background color</string>\
	<string id=\"st_grid_color\">Grid color</string>\
	<!-- Overlay -->\
	<!-- Main Menu Bar -->\
	<string id=\"st_help\">Help</string>\
	<string id=\"st_file\">File</string>\
	<string id=\"st_quit\">Quit</string>\
	<string id=\"st_recent_files\">Recent files</string>\
	<!-- Main Menu Bar -->\
</string_table>";

		class SDK_Names
		{
		private:
			SDK_Names(void) noexcept : m_generated_index(0) 
			{
			
			}

		public:
			inline static SDK_Names& getInstance(void) noexcept
			{
				static SDK_Names instance;
				return instance;
			}

			SDK_Names(const SDK_Names&) = delete;
			SDK_Names& operator=(const SDK_Names&) = delete;
			SDK_Names(SDK_Names&&) = delete;
			SDK_Names& operator=(SDK_Names&&) = delete;
			~SDK_Names(void) = default;

			inline xr_string getName(const xr_string& id)
			{
				if (id.empty() || this->m_current_language.empty())
					return u8"st_invalid_id";

				for (std::pair<xr_string, xr_map<xr_string, xr_string>> it : this->m_data)
				{
					if (it.first == this->m_current_language)
					{
						xr_string result = it.second[id];
						if (result.empty())
						{
							result = id;
						}
							
						return result;
					}
				}

				return u8"st_invalid_id";
			}
			
			inline const xr_string& getCurrentLanguage(void)
			{
				if (this->m_current_language.empty())
					return xr_string(TEXT("Empty"));
				 
				return this->m_current_language;
			}

			inline void setCurrentLanguage(const xr_string& str)
			{
				if (str.empty())
					return;

				this->m_current_language = str;
			}

			void Initialize(void);

		private:
			bool LoadXml(const xr_string& filepath);
		private:
			int m_generated_index;
			xr_string m_current_language;
		public:
			xr_vector<xr_string> m_languages;
		private:
			// @ Key - name from xml, value - from <text> Heh </text>
			xr_list<std::pair<xr_string, xr_map<xr_string, xr_string>>> m_data;
		};
	}
}